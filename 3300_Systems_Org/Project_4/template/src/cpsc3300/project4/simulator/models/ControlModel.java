package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Instruction;

public class ControlModel {

	private boolean regDst;
	private boolean branch;
	private boolean memRead;
	private boolean memToReg;
	private byte    aluOp; //2 bits
	private boolean memWrite;
	private boolean aluSrc;
	private boolean regWrite;

	public ControlModel(byte opcode) {
		// TODO: Switch on OpCode to set controls for the private variables for each kind of instruction
		switch (opcode) {
			default:
				regDst = false;
				aluSrc = false;
				memToReg = false;
				regWrite = false;
				memRead = false;
				memWrite = false;
				branch = false;
				aluOp = 0x0;
				break;
		}
	}

	public ControlModel(Instruction instr) {
		this(instr.getOpcode());
	}

	public boolean isRegDst() {
		return regDst;
	}

	public boolean isBranch() {
		return branch;
	}

	public boolean isMemRead() {
		return memRead;
	}

	public boolean isMemToReg() {
		return memToReg;
	}

	public byte getAluOp() {
		return aluOp;
	}

	public boolean isMemWrite() {
		return memWrite;
	}

	public boolean isAluSrc() {
		return aluSrc;
	}

	public boolean isRegWrite() {
		return regWrite;
	}
}
