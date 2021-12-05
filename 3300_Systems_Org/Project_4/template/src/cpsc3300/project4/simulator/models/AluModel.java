package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Instruction;

public class AluModel {

	private boolean zeroFlag = false;
	private int result;

	public int calculate(int data1, int data2, int operation) {
		if (operation == 2) {
			result = data1 + data2;
			return result;
		} else if (operation == 6) {
			result = data1 - data2;
			return result;
		} else if (operation == 0) {
			result = data1 & data2;
			return result;
		} else if (operation == 1) {
			result = data1 | data2;
			return result;
		} else {
			if (data1 < data2) {
				result = 1;
				return result;
			} else {
				result = 0;
				return result;
			}
		}
	}

	public static byte getOperation(byte funct, int AluOp) {
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
