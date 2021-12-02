package cpsc3300.project4.simulator.models;

public class MuxModel {

	/**
	 * @param a    the object returned when <code>flag</code> is true
	 * @param b    the object returned when <code>flag</code> is false
	 * @param flag selects input
	 * @return selected object
	 */
	public static <T> T mux(boolean flag, T a, T b) {
		return flag ? a : b;
	}
}
