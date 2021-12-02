package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Instruction;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class InstructionMemoryModel {

	private List<Instruction> instructions = new ArrayList<>();


	public InstructionMemoryModel(String instructionFile) {
		try {
			DataInputStream dat = new DataInputStream(new FileInputStream(instructionFile));

			while (dat.available() > 0) {
				instructions.add(new Instruction(dat.readInt()));
			}
		} catch (IOException e) {
			System.out.println("Unable to read instruction binary file");
			System.exit(1);
		}
	}

	public InstructionMemoryModel(Instruction[] d) {
		instructions.addAll(Arrays.asList(d));
	}

	public InstructionMemoryModel(int[] d) {
		for (int i : d) {
			instructions.add(new Instruction(i));
		}
	}

	public Instruction readInstruction(int addr) {
		try {
			return instructions.get(addr / 4);
		} catch (IndexOutOfBoundsException e) {
			return new Instruction(0);
		}
	}

	public int getMaxAddress(){
		return instructions.size() * 4;
	}
}
