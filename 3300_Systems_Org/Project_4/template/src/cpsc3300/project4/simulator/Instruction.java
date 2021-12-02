package cpsc3300.project4.simulator;

public class Instruction {

	private int   instr;
	private byte  opcode;
	private byte  rs;
	private byte  rt;
	private short tail;

	public Instruction(int instr) {
		this.instr = instr;
		// TODO: Set opcode, rs, rt, and tail based on binary data in instr
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
	}

	public byte getShamt() {
		// TODO: Isolate Shamt value from instruction
	}

	public byte getFunct() {
		// TODO: Isolate Funct value from instruction
	}

	public int getJumpAddress() {
		// TODO: Isolate address value from jump instruction
	}

	public int getAluOp() {
		switch (opcode) {
			// TODO: Make cases for various types based on opcode, returning the appropriate ALU Op
			default:
				return 0;
		}
	}

	public String decode() {
		return ""; //TODO convert instruction back to assembly code
	}

	@Override
	public String toString() {
		String bin = Integer.toBinaryString(instr);
		//left pad with zeros to be full 32 bit width
		return "00000000000000000000000000000000".substring(bin.length()) + bin;
	}
}
