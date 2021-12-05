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
		switch (opcode) {
			case 0:
				regDst = true;
				aluSrc = false;
				memToReg = false;
				regWrite = true;
				memRead = false;
				memWrite = false;
				branch = false;
				aluOp = 0x2;
				break;
			case 4:
				regDst = false;
				aluSrc = false;
				memToReg = false;
				regWrite = false;
				memRead = false;
				memWrite = false;
				branch = true;
				aluOp = 0x1;
				break;
			case 35:
				regDst = false;
				aluSrc = true;
				memToReg = true;
				regWrite = true;
				memRead = true;
				memWrite = false;
				branch = false;
				aluOp = 0x0;
				break;
			case 43:
				regDst = false;
				aluSrc = true;
				memToReg = false;
				regWrite = false;
				memRead = false;
				memWrite = true;
				branch = false;
				aluOp = 0x0;
				break;
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
