package cpsc3300.project4.simulator.views;

import cpsc3300.project4.simulator.models.DataMemoryModel;
import cpsc3300.project4.simulator.models.InstructionMemoryModel;
import cpsc3300.project4.simulator.models.RegistersModel;

public interface ICpuView {

	/**
	 * Update view with the given data
	 */
	void updateView(InstructionMemoryModel im, DataMemoryModel dm, RegistersModel regs, int pc);
}
