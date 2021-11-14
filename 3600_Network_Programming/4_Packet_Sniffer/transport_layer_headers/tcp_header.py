from layer_header import LayerHeader
from struct import pack, unpack

class TCPHeader(LayerHeader):
    def __init__(self, pkt):
        # TODO: Replace the value of header_length with the length of an Ethernet header
        header_length = 20
        
        # TODO: If this header can be variable length, you will need to update the contents of 
        #       self.header_bytes once you know the full length of the header in order to ensure
        #       that all of the bytes associated with this header are saved. 
        #       You can leave it as is for now.
        self.header_bytes = pkt[:header_length]

        self.source_port = None
        self.dest_port = None
        self.SEQ = None
        self.ACK = None
        self.data_offset = None
        self.NS_flag = None
        self.CWR_flag = None
        self.ECE_flag = None
        self.URG_flag = None
        self.ACK_flag = None
        self.PSH_flag = None
        self.RST_flag = None
        self.SYN_flag = None
        self.FIN_flag = None
        self.window_size = None
        self.checksum = None
        self.urg_pointer = None
        self.options_bytes = None

        # TODO: Unpack the header and assign the values to the above variables
        self.source_port = unpack('>H', self.header_bytes[:2])[0]
        self.dest_port = unpack('>H', self.header_bytes[2:4])[0]
        self.SEQ = unpack('>I', self.header_bytes[4:8])[0]
        self.ACK = unpack('>I', self.header_bytes[8:12])[0]

        data = unpack('>H', self.header_bytes[12:14])[0]

        self.data_offset = (data >> 12) & 15
        self.NS_flag = data & 256
        self.CWR_flag = data & 128
        self.ECE_flag = data & 64
        self.URG_flag = data & 32
        self.ACK_flag = data & 16
        self.PSH_flag = data & 8
        self.RST_flag = data & 4
        self.SYN_flag = data & 2
        self.FIN_flag = data & 1

        self.window_size = unpack('>H', self.header_bytes[14:16])[0]
        self.checksum = unpack('>H', self.header_bytes[16:18])[0]
        self.urg_pointer = unpack('>H', self.header_bytes[18:20])[0]


        # TODO: You do not need to unpack any options, if they are present in the header. However, if options 
        #       are present, store the bytes associated with them in self.options_bytes.

        if self.data_offset > 5:
            header_length = 4 * self.data_offset
            self.header_bytes = pkt[:header_length]
            self.options_bytes = pkt[20:header_length]

    def protocol(self):
        return "TCP"

    def header_bytes(self):
        return self.header_bytes

    def print_header(self):
        print("")
        print("TCP HEADER: ")
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
        version_str = "SEQ NUMBER: " + str(self.SEQ)
        white_space = (96 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)


        ####################################################################
        # Print first line
        print("-"*line_width)
        
        # Compose the contents of the first row of the header
        version_str = "ACK NUMBER: " + str(self.ACK)
        white_space = (96 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)


        ####################################################################
        # Print first line
        print("-"*line_width)
        
        # Compose the contents of the first row of the header
        version_str = "OFFSET: " + str(self.data_offset)
        white_space = (16 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        ihl_str = "  ---  |"
        first_row_str += ihl_str

        ihl_str = "NS:" + ("1" if self.NS_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "CWR:" + ("1" if self.CWR_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "ECE:" + ("1" if self.ECE_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "URG:" + ("1" if self.URG_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "ACK:" + ("1" if self.ACK_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "PSH:" + ("1" if self.PSH_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "RST:" + ("1" if self.RST_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "SYN:" + ("1" if self.SYN_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "FIN:" + ("1" if self.FIN_flag else "0")
        white_space = (5 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        ihl_str = "WIN:" + str(self.window_size)
        white_space = (20 - len(ihl_str))//2
        first_row_str += " "*white_space + ihl_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)



        ####################################################################
        # Print first line
        print("-"*line_width)
        
        # Compose the contents of the first row of the header
        version_str = "CHECKSUM: " + str(self.checksum)
        white_space = (48 - len(version_str))//2
        first_row_str = "|" + " "*white_space + version_str + " "*white_space + "|"

        # Compose the contents of the first row of the header
        version_str = "URG POINTER: " + str(self.urg_pointer)
        white_space = (48 - len(version_str))//2
        first_row_str += " "*white_space + version_str + " "*white_space + "|"

        # Print the first row of the header
        print(first_row_str)

        ####################################################################
        # Print a line divider
        print("-"*line_width)

        return super().print_header()