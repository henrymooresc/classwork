package cpsc3300.project4.simulator;

import cpsc3300.project4.simulator.models.RegistersModel;

public class Instruction {

	private int   instr;
	private byte  opcode;
	private byte  rs;
	private byte  rt;
	private short tail;

	public Instruction(int instr) {
		this.instr = instr;
		// TODO: Set opcode, rs, rt, and tail based on binary data in instr
		this.opcode = (byte) (instr >> 26);
		this.rs = (byte) ((instr >> 21) & 31);
		this.rt = (byte) ((instr >> 16) & 31);
		this.tail = (short) (instr & 32767);
	}

	public int getInstr() {
		return instr;
	}

	public byte getOpcode() {
		return opcode;
	}

	public byte getRs() {
		return rs;
	}

	public byte getRt() {
		return rt;
	}

	public short getAddress() {
		return tail;
	}

	public int getExtendedAddress() {
		return tail;
	}

	public byte getRd() {
		// TODO: Isolate Rd value from instruction
		return (byte) ((tail >> 11) & 31);
	}

	public byte getShamt() {
		// TODO: Isolate Shamt value from instruction
		return (byte) ((tail >> 6) & 31);
	}

	public byte getFunct() {
		// TODO: Isolate Funct value from instruction
		return (byte) (tail & 63);
	}

	public int getJumpAddress() {
		// TODO: Isolate address value from jump instruction
		return tail;
	}

	public int getAluOp() {
		switch (opcode) {
			// TODO: Make cases for various types based on opcode, returning the appropriate ALU Op
			case 0:
				return 2;
			case 4:
				return 1;
			default:
				return 0;
		}
	}

	public String decode() {
		String instr_s;
		if (opcode == 35)
		{
			instr_s.concat("")
		}

		return ""; //TODO convert instruction back to assembly code
	}

	@Override
	public String toString() {
		String bin = Integer.toBinaryString(instr);
		//left pad with zeros to be full 32 bit width
		return "00000000000000000000000000000000".substring(bin.length()) + bin;
	}
}
