package cpsc3300.project4.simulator.views;

import cpsc3300.project4.simulator.models.AluModel;
import cpsc3300.project4.simulator.models.DataMemoryModel;
import cpsc3300.project4.simulator.models.InstructionMemoryModel;
import cpsc3300.project4.simulator.models.RegistersModel;

public class TextCpuView implements ICpuView {

	private int cycles = 0;
	private int adds = 0;
	private int subs = 0;
	private int ands = 0;
	private int ors = 0;
	private int slts = 0;
	private int memR = 0;
	private int memW = 0;

	@Override
	public void updateView(InstructionMemoryModel IM, DataMemoryModel DM, RegistersModel regs, int pc) {

		if (pc < IM.getMaxAddress()) {
			System.out.println("\n\n~~~~PC -> " + pc + "~~~~");
			cycles++;
			int oper = AluModel.getOperation(IM.readInstruction(pc));

			if (oper == 2) {
				adds++;
				if (IM.readInstruction(pc).getOpcode() == 35) {
					memR++;
				} else if (IM.readInstruction(pc).getOpcode() == 43) {
					memW++;
				}
			} else if (oper == 6) {
				subs++;
			} else if (oper == 0) {
				ands++;
			} else if (oper == 1) {
				ors++;
			} else {
				slts++;
			}
		} else {
			System.out.println("\n\n~~~~Final View~~~~");
		}

		System.out.println("\n=============================================");
		System.out.println("\tRegisters");
		System.out.println("=============================================");
		byte index = 0;
		for (String reg_name : RegistersModel.registerNames) {
			int data = regs.readRegister(index);
			System.out.print(reg_name + ": " + Integer.toString(data) + "  |  ");

			if ((index + 1) % 6 == 0) {
				System.out.print("\n  ");
			}
			index++;
		}

		System.out.print("\n=============================================\n");

		System.out.println("\tAddress\t| Data");
		System.out.println("=============================================");
		for (int i = 0; i <= DM.getMaxAddress(); i+=4) {
			System.out.println("\t0x" + Integer.toHexString(i) + "\t| " + DM.readData(i));
		}

		// TODO: Display logic block statistics
		/* You should track # cycles for a given program.
		   Also, you should track ALU arithmetic operations (how many add, sub, etc ops).
		   Note: Some instructions besides "add" use add. example is beq; this counts as an ALU arithmetic op, incrementing the PC does not.
		   You should track # of memory reads/writes too.
		   The control should track the # of each individual instruction. */
		System.out.println("\n=============================================");
		System.out.println("\tLogic Block Stats");
		System.out.println("=============================================");

		

		System.out.println("Cycles: " + cycles + "\t| MemReads: " + memR + "\t| Memwrites: " + memW);
		System.out.println("ADDs: " + adds + "\t\t| SUBs: " + subs + "\t| SLTs: " + slts);
		System.out.println("ANDs: " + ands + "\t\t| ORs: " + ors);
	}
}
