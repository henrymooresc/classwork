package cpsc3300.project4.simulator;

import cpsc3300.project4.simulator.models.AluModel;
import cpsc3300.project4.simulator.models.ControlModel;
import cpsc3300.project4.simulator.models.DataMemoryModel;
import cpsc3300.project4.simulator.models.InstructionMemoryModel;
import cpsc3300.project4.simulator.models.MuxModel;
import cpsc3300.project4.simulator.models.RegistersModel;
import cpsc3300.project4.simulator.views.ICpuView;
import cpsc3300.project4.simulator.views.TextCpuView;

import java.util.Scanner;

public class Controller {
	public static final int    MEM_SIZE   = 102400;
	public static final double CLOCK_RATE = 0.25; // in seconds

	private InstructionMemoryModel IM;
	private DataMemoryModel        DM;
	private RegistersModel         regs;
	private int                    pc = 0;

	private ICpuView view;

	public Controller(String instructionFile, String dataFile) {
		this.IM = new InstructionMemoryModel(instructionFile);
		this.DM = new DataMemoryModel(dataFile);
		this.regs = new RegistersModel();
		this.view = new TextCpuView();
	}

	/**
	 * Runs the simulator for all instructions
	 */
	public void runAll() {

		// keeps running until out of instructions
		while (pc < IM.getMaxAddress()) {
			view.updateView(IM, DM, regs, pc);
			singleStep();

			try {
				Thread.sleep((long) (1000 * CLOCK_RATE));
			} catch (InterruptedException e) {
				System.out.println("Error pausing between instructions");
			}
		}
		// display the final state of the CPU
		view.updateView(IM, DM, regs, pc);
	}

	/**
	 * Runs the simulator for all instructions stopping after all
	 */
	public void runSingleStep() {
		System.out.println("Press enter to step processor");
		Scanner s = new Scanner(System.in);
		String c;
		while ((c = s.nextLine()) != ""){
			view.updateView(IM, DM, regs, pc);
			singleStep();
			if (pc > IM.getMaxAddress()) break;
		}
	}

	/**
	 * run a single instruction (reads instruction based on <code>pc</code> and updates <code>pc</code>
	 */
	private void singleStep() {
		Instruction  instr   = IM.readInstruction(pc);
		ControlModel control = new ControlModel(instr);
		AluModel     alu     = new AluModel();

		// prepare ALU inputs
		byte operation = alu.getOperation(instr);
		int  data1     = regs.readRegister(instr.getRs());
		// select input based on control signal
		// TODO: Use Mux to select correct input based on control signal
		int data2 = MuxModel.mux(...);
		int result = alu.calculate(data1, data2, operation);

		// memory reads and writes
		int memVal = 0;
		// TODO: Perform memory writes or reads as needed
		// TODO: Prepare writeback to register as needed
		int output = MuxModel.mux(...);

		if (control.isRegWrite()) {
			// TODO: Identify register to write to
			byte reg = MuxModel.mux(...);
			regs.writeRegister(reg, output);
		}
		// update pc to next instruction
		this.pc += 4;
		// TODO: Add branch control for beq instruction
	}
}
