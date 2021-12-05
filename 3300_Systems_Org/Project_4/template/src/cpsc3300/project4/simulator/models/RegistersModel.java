package cpsc3300.project4.simulator.models;

public class RegistersModel {
	private             int[]    registers = new int[24];
	public static final String[] registerNames = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
												  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
												  "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7"};

	public int readRegister(byte regAddr) {
		return registers[regAddr];
	}

	public void writeRegister(byte regAddr, int word) {
		registers[regAddr] = word;
	}
}
