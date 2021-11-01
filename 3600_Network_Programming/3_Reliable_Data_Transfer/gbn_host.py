from network_simulator import NetworkSimulator, Packet, EventEntity
from enum import Enum
from struct import pack, unpack

class GBNHost():

    # The __init__ method accepts:
    # - a reference to the simulator object
    # - the value for this entity (EntityType.A or EntityType.B)
    # - the interval for this entity's timer
    # - the size of the window used for the Go-Back-N algorithm
    def __init__(self, simulator, entity, timer_interval, window_size):
        
        # These are important state values that you will need to use in your code
        self.simulator = simulator
        self.entity = entity
        
        # Sender properties
        self.timer_interval = timer_interval        # The duration the timer lasts before triggering
        self.window_size = window_size              # The size of the seq/ack window
        self.window_base = 0                        # The last ACKed packet. This starts at 0 because no packets 
                                                    # have been ACKed
        self.next_seq_num = 0                       # The SEQ number that will be used next
        self.unACKed_buffer = {}
        self.app_layer_buffer = []

        # Reciever properties
        self.expected_seq_num = 0
        self.last_ack = self.make_pkt(-1, '')
   

    ###########################################################################################################
    ## Core Interface functions that are called by Simulator

    # This function implements the SENDING functionality. It should implement retransmit-on-timeout. 
    # Refer to the GBN sender flowchart for details about how this function should be implemented
    def receive_from_application_layer(self, payload):
        if (self.next_seq_num < self.window_base + self.window_size):
            pkt = self.make_pkt(self.next_seq_num, payload)
            self.unACKed_buffer[self.next_seq_num] = pkt

            self.simulator.pass_to_network_layer(self.entity, pkt, False)

            if (self.window_base == self.next_seq_num):
                self.simulator.start_timer(self.entity, self.timer_interval)
            
            self.next_seq_num += 1
        else:
            self.app_layer_buffer.append(payload)
            


    # This function implements the RECEIVING functionality. This function will be more complex that
    # receive_from_application_layer(), it includes functionality from both the GBN Sender and GBN receiver
    # FSM's (both of these have events that trigger on receive_from_network_layer). You will need to handle 
    # data differently depending on if it is a packet containing data, or if it is an ACK.
    # Refer to the GBN receiver flowchart for details about how to implement responding to data pkts, and
    # refer to the GBN sender flowchart for details about how to implement responidng to ACKs
    def receive_from_network_layer(self, byte_data):
        is_corrupt = self.is_corrupt(byte_data)
        
        pkt_type = unpack("!H", byte_data[:2])[0]

        if pkt_type != 0:
            if is_corrupt:
                self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)
            else:
                seq_num = unpack("!i", byte_data[2:6])[0]
                if seq_num != self.expected_seq_num:
                    self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)
                else:
                    payload_len = unpack("!I", byte_data[8:12])[0]
                    payload = unpack("!" + str(payload_len) + "s", byte_data[12:])[0]
                    self.simulator.pass_to_application_layer(self.entity, payload.decode())
                    self.last_ack = self.make_pkt(seq_num, '')
                    self.simulator.pass_to_network_layer(self.entity, self.last_ack, True)
                    self.expected_seq_num += 1

        else:
            if not is_corrupt:
                ack_num = unpack("!i", byte_data[2:6])[0]
                if ack_num >= self.window_base:
                    self.window_base = ack_num + 1
                    self.simulator.stop_timer(self.entity)
                    if self.window_base != self.next_seq_num:
                        self.simulator.start_timer(self.entity, self.timer_interval)
                    
                    while (len(self.app_layer_buffer) > 0 and self.next_seq_num < self.window_base + self.window_size):
                        payload = self.app_layer_buffer.pop()
                        pkt = self.make_pkt(self.next_seq_num, payload)
                        self.unACKed_buffer[self.next_seq_num] = pkt
                        self.simulator.pass_to_network_layer(self.entity, pkt, False)

                        if self.window_base == self.next_seq_num:
                            self.simulator.start_timer(self.entity, self.timer_interval)
                        
                        self.next_seq_num += 1

            
    # This function is called by the simulator when a timer interrupt is triggered due to an ACK not being 
    # received in the expected time frame. All unACKed data should be resent, and the timer restarted
    def timer_interrupt(self):
        self.simulator.start_timer(self.entity, self.timer_interval)

        for i in range(self.window_base, self.next_seq_num):
            pkt = self.unACKed_buffer[i]

            self.simulator.pass_to_network_layer(self.entity, pkt, False)

    # This function should check to determine if a given packet is corrupt. The packet parameter accepted
    # by this function should contain a byte array
    def is_corrupt(self, packet):
        if type(packet) != bytes:
            return True

        if len(packet) % 2 == 1:
            packet = packet + bytes(1)
        
        word_sum = 0
        for i in range(0, len(packet), 2):
            word = packet[i] << 8 | packet[i + 1]
            word_sum += word
            word_sum = (word_sum & 0xffff) + (word_sum >> 16)

        return word_sum != 65535

    def make_pkt(self, seq_num, payload):
        payload_len = len(payload)

        if payload_len == 0:
            pkt_type = 0
        else:
            pkt_type = 128
            
        temp_pkt = pack("!HiHI" + str(payload_len) + "s", pkt_type, seq_num, 0, payload_len, payload.encode())
        checksum = self.compute_checksum(temp_pkt)

        pkt = pack("!HiHI" + str(payload_len) + "s", pkt_type, seq_num, checksum, payload_len, payload.encode())

        return pkt


    def compute_checksum(self, pkt):
        if len(pkt) % 2 == 1:
            pkt = pkt + bytes(1)

        word_sum = 0
        for i in range(0, len(pkt), 2):
            word = (pkt[i] << 8 | pkt[i + 1])
            word_sum += word
            word_sum = (word_sum & 0xffff) + (word_sum >> 16)

        checksum = ~word_sum & 0xffff

        return checksum