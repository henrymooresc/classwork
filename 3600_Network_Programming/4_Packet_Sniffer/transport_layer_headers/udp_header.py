from layer_header import LayerHeader
from struct import pack, unpack

class UDPHeader(LayerHeader):
    def __init__(self, pkt):
        # TODO: Replace the value of header_length with the length of an Ethernet header
        header_length = 8
        
        # TODO: If this header can be variable length, you will need to update the contents of 
        #       self.header_bytes once you know the full length of the header in order to ensure
        #       that all of the bytes associated with this header are saved. 
        #       You can leave it as is for now.
        self.header_bytes = pkt[:header_length]

        self.source_port = None
        self.dest_port = None
        self.length =None
        self.checksum = None

        # TODO: Unpack the header and assign the values to the above variables
        self.source_port = unpack('>H', self.header_bytes[:2])[0]
        self.dest_port = unpack('>H', self.header_bytes[2:4])[0]
        self.length = unpack('>H', self.header_bytes[4:6])[0]
        self.checksum = unpack('>H', self.header_bytes[6:])[0]


    def protocol(self):
        return "UDP"

    def header_bytes(self):
        return self.header_bytes

    def print_header(self):
        print("")
        print("UDP HEADER: ")
        line_width = (96+4)

        ####################################################################
        # Print first line
        print("-"*line_width)
        
        # Compose the contents of the first row of the header
        version_str = "SOURCE PORT: " + str(self.source_port)
        white_space = (48 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        ihl_str = "DEST PORT: " + str(self.dest_port)
        white_space = (48 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)
        


        ####################################################################
        # Print first line
        print("-"*line_width)
        
        # Compose the contents of the first row of the header
        version_str = "LENGTH: " + str(self.length)
        white_space = (48 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        # Compose the contents of the first row of the header
        version_str = "CHECKSUM: " + str(self.checksum)
        white_space = (48 - len(version_str))//2
        first_row_str += " "*white_space + version_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)


        ####################################################################
        # Print a line divider
        print("-"*line_width)

        return super().print_header()