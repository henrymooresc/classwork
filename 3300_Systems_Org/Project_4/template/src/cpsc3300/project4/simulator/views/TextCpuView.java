package cpsc3300.project4.simulator.views;

import cpsc3300.project4.simulator.models.DataMemoryModel;
import cpsc3300.project4.simulator.models.InstructionMemoryModel;
import cpsc3300.project4.simulator.models.RegistersModel;

public class TextCpuView implements ICpuView {

	@Override
	public void updateView(InstructionMemoryModel IM, DataMemoryModel DM, RegistersModel regs, int pc) {

		// TODO: Display PC information

		System.out.println("===========Registers===========");
		// TODO: Display register information

		// TODO: Display memory information
		System.out.println("---Address---|-------Data--------|-Instruction-");

		// TODO: Display logic block statistics
		/* You should track # cycles for a given program.
		   Also, you should track ALU arithmetic operations (how many add, sub, etc ops).
		   Note: Some instructions besides "add" use add. example is beq; this counts as an ALU arithmetic op, incrementing the PC does not.
		   You should track # of memory reads/writes too.
		   The control should track the # of each individual instruction. */
	}

	//TESTING
	public static void main(String[] args) {
		// OPTIONAL TODO: Update constructors and initialization as needed -- only for testing
		InstructionMemoryModel im   = new InstructionMemoryModel("input.asm");
		//DataMemoryModel        dm   = new DataMemoryModel();
		RegistersModel         regs = new RegistersModel();

		TextCpuView v = new TextCpuView();
	}
}
