from network_config import config

class RoundTripLatencyCalculator:

    def __init__(self, config):
        self.packet_len = config['packet_length']
        self.num_links = config['num_links']
        self.bandwidths = config['bandwidths']
        self.distances = config['distances']
        self.transm_spd = config['transmission_speeds']
        self.p_delays = config['processing_delays']
        self.avg_packet_ar = config['average_packet_arrival_rates']
        

    def calculate_total_RTT(self):
        delay = 0

        for i in range(self.num_links):
            delay += self.calculate_link_contribution(i)

        return 2 * delay
        

    def calculate_link_contribution(self, hop_number):
        return (self.calculate_transmission_delay(hop_number) + 
                self.calculate_propagation_delay(hop_number) + 
                self.calculate_processing_delay(hop_number) + 
                self.calculate_queuing_delay(hop_number))
        

    def calculate_transmission_delay(self, hop_number):
        return self.packet_len / self.bandwidths[hop_number]


    def calculate_propagation_delay(self, hop_number):
        return self.distances[hop_number] / self.transm_spd[hop_number]


    def calculate_processing_delay(self, hop_number):
        return self.p_delays[hop_number]


    def calculate_queuing_delay(self, hop_number):
        delay_factor = (self.packet_len * self.avg_packet_ar[hop_number]) / self.bandwidths[hop_number]

        return (0.1 / (1.0 - delay_factor) - 0.1)


if __name__ == "__main__":
    calc = RoundTripLatencyCalculator(config)

    latency = calc.calculate_total_RTT()
    print(latency)