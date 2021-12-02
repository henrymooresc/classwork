import sys

# Global variables
valid_instr = ['lw', 'sw', 'add', 'sub', 'and', 'or', 'slt', 'beq', 'j']

opcode_dict = {'lw': 35, 'sw': 43, 'beq': 4, 'j': 2}

func_dict = {'add': 32, 'sub': 34, 'and': 36, 'or': 37, 'slt': 42}

# Helper functions for translating instruction into binary and writing to output
def jType(opcode, addr, o_file):
    seq = f'{opcode:06b}{addr:026b}'
    o_file.write((int(seq, 2)).to_bytes(4, 'big'))

def iType(opcode, rs, rt, offset, o_file):
    if offset < 0:
        offset = 65535 + offset + 1

    seq = f'{opcode:06b}{rs:05b}{rt:05b}{offset:016b}'
    o_file.write((int(seq, 2)).to_bytes(4, 'big'))

def rType(instr, rd, rs, rt, o_file):
    opcode = 0
    shamt = 0
    funct = func_dict[instr]
    
    seq = f'{opcode:06b}{rs:05b}{rt:05b}{rd:05b}{shamt:05b}{funct:06b}'
    o_file.write((int(seq, 2)).to_bytes(4, 'big'))

# Main function
if __name__ == "__main__":

    # Open files for reading and writing
    i_file = open(sys.argv[1], 'r')
    o_file = open(sys.argv[2], 'wb')

    line_num = 0
    has_error = False

    # Loops through each line of input file.
    # If any error is found, writing to the output file will stop
    # but the loop will continue to look for other errors
    for line in i_file:
        line_num += 1

        # Cleans up and separates instruction elements
        line = line.replace(',','')
        line = line.replace('$','')
        line = line.replace('(',' ')
        line = line.replace(')','')
        line_arr = line.split()

        instr = line_arr[0]
        
        # Checks if instruction given is valid
        if instr not in valid_instr:
            print(f'Error on line {line_num}: invalid instruction')
            has_error = True

        else:

            # Checks which instruction is given, runs appropriate helper function
            if instr == 'j':
                opcode = opcode_dict[instr]
                addr = int(line_arr[1])

                if not((0 <= addr <= 67108863)):
                    print(f'Error on line {line_num}: address out of range')
                    has_error = True
                else:
                    if not has_error:
                        jType(opcode, addr, o_file)

            elif instr == 'beq':
                opcode = opcode_dict[instr]
                rs = int(line_arr[1])
                rt = int(line_arr[2])
                offset = int(line_arr[3])

                if not((0 < rs < 32) and (0 < rt < 32) and (-65535 <= offset <= 65535)):
                    print(f'Error on line {line_num}: register or offset out of range')
                    has_error = True
                else:
                    if not has_error:
                        iType(opcode, rs, rt, offset, o_file)

            elif instr == 'lw' or instr == 'sw':
                opcode = opcode_dict[instr]
                rt = int(line_arr[1])
                offset = int(line_arr[2])
                rs = int(line_arr[3])

                if not((0 < rs < 32) and (0 < rt < 32) and (-65535 <= offset <= 65535)):
                    print(f'Error on line {line_num}: register or offset out of range')
                    has_error = True
                else:
                    if not has_error:
                        iType(opcode, rs, rt, offset, o_file)

            else:
                rd = int(line_arr[1])
                rs = int(line_arr[2])
                rt = int(line_arr[3])

                if not((0 < rd < 32) and (0 < rs < 32) and (0 < rt < 32)):
                    print(f'Error on line {line_num}: register out of range')
                    has_error = True
                else:
                    if not has_error:
                        rType(instr, rd, rs, rt, o_file)


    i_file.close()
    o_file.close()

    # I used the code below to quickly print what was written to the output file.
    # Figured I should keep it in the file just in case
    '''
    t_file = open(sys.argv[2], 'rb')

    data = t_file.read(1)
    i = 0

    while data:
        i += 1
        d_data = int.from_bytes(data, 'big')
        print(f'{d_data:08b} ', end='')
        if i % 4 == 0:
            print('\n')
        data = t_file.read(1)

    t_file.close()
    '''