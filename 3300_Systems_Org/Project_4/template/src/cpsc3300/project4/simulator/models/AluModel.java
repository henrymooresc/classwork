package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Instruction;

public class AluModel {

	private boolean zeroFlag = false;
	private int result;

	public int calculate(int data1, int data2, int operation) {
		// TODO: Return the result of ALU operation based on operation
		return 0;
	}

	public static byte getOperation(byte funct, int AluOp) {
		// TODO: Return ALU operation based on function and OpCode
		return -1;
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
