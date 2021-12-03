package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Instruction;

public class AluModel {

	private boolean zeroFlag = false;
	private int result;

	public int calculate(int data1, int data2, int operation) {
		// TODO: Return the result of ALU operation based on operation
		if (operation == 2) {
			return data1 + data2;
		} else if (operation == 6) {
			return data1 - data2;
		} else if (operation == 0) {
			return data1 & data2;
		} else if (operation == 1) {
			return data1 | data2;
		} else {
			if (data1 < data2) {
				return 1;
			} else {
				return 0;
			}
		}
	}

	public static byte getOperation(byte funct, int AluOp) {
		// TODO: Return ALU operation based on function and OpCode
		if (AluOp == 0) {
			return 2;
		} else if (AluOp == 1) {
			return 6;
		} else {
			if (funct == 32) {
				return 2;
			} else if (funct == 34) {
				return 6;
			} else if (funct == 38) {
				return 0;
			} else if (funct == 39) {
				return 1;
			} else {
				return 7;
			}
		}
	}

	public static byte getOperation(Instruction instr) {
		return getOperation(instr.getFunct(), instr.getAluOp());
	}

	public boolean isZeroFlag() {
		return zeroFlag;
	}

	public int getResult(){
		return result;
	}
}
