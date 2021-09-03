# TODO: Import the config variable from the network_config module
from network_config import config

class RoundTripLatencyCalculator:

    def __init__(self, config):
        # TODO: Perform any required initialization. Config should be a dictionary that contains
        #       all of the parameters necessary to calculate the latency for a given path through a network
        #       The config variable should be defined in a separate file named 'network_config.py'
        #       Delete "pass" once you have added code to this function.
        self.packet_len = config['packet_length']
        self.num_links = config['num_links']
        self.bandwidths = config['bandwidths']
        self.distances = config['distances']
        self.transm_spd = config['transmission_speeds']
        self.p_delays = config['processing_delays']
        self.avg_packet_ar = config['average_packet_arrival_rate']
        

    def calculate_total_RTT(self):
        # TODO: Compute the total round trip latency. You should use the below helper functions to break 
        #       the computation into its multiple component parts.
        #       Return the total round trip latency after computing it
        #       Delete "pass" once you have added code to this function.
        delay = 0

        for i in range(self.num_links):
            delay += self.calculate_link_contribution(i)

        return delay
        

    def calculate_link_contribution(self, hop_number):
        # TODO: Compute the total latency associated with a crossing a specific hop one-way. 
        #       You should use the below helper functions to break the computation into its multiple component parts.
        #       Return the total round trip latency after computing it
        #       Delete "pass" once you have added code to this function.
        return self.calculate_transmission_delay(hop_number) + self.calculate_propagation_delay(hop_number) + self.calculate_processing_delay(hop_number) + self.calculate_queuing_delay(hop_number)
        

    def calculate_transmission_delay(self, hop_number):
        # TODO: Compute the transmission delay associated with a given hop
        #       Delete "pass" once you have added code to this function.
        return self.packet_len / self.bandwidths[hop_number]


    def calculate_propagation_delay(self, hop_number):
        # TODO: Compute the propagation delay associated with a given hop
        #       Delete "pass" once you have added code to this function.
        return self.distances[hop_number] / self.transm_spd[hop_number]


    def calculate_processing_delay(self, hop_number):
        # TODO: Compute the processing delay associated with a given hop
        #       Delete "pass" once you have added code to this function.
        return self.p_delays[hop_number]


    def calculate_queuing_delay(self, hop_number):
        # TODO: Compute the queuing delay associated with a given hop using the equations 
        #       delay = (0.1) / (1-delay_factor) - .1
        #       delay_factor = packet_length * average_packet_arrival_rate / bandwidth
        #       IMPORTANT: In real life, you can't predict exactly what the queueing delay will be. 
        #                  These equations roughly model what the size of the delay could be like in proportion
        #                  to how many packets are trying to move through a router at a given time
        #       Delete "pass" once you have added code to this function.
        return (0.1) / (1 - ((self.packet_len * self.avg_packet_ar[hop_number]) / self.bandwidths[hop_number])) - 0.1



# You do not need to change anything in the main method. It will not be called by the testing suite, so anything
# you implement here will not register when you submit your code. It is intended for your own personal testing only
if __name__ == "__main__":
    calc = RoundTripLatencyCalculator(config)

    latency = calc.calculate_total_RTT()
    print(latency)