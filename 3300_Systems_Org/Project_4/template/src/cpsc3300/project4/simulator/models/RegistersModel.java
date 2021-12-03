package cpsc3300.project4.simulator.models;

public class RegistersModel {
	private             int[]    registers = new int[28]; // TODO: Identify number of registers to support
	public static final String[] registerNames = {"zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
												  "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
												  "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7"}; // TODO: Identify register names in order

	public int readRegister(byte regAddr) {
		// TODO: return value in register given by index
		return registers[regAddr];
	}

	public void writeRegister(byte regAddr, int word) {
		// TODO: store value word in register given by index
		registers[regAddr] = word;
	}
}
