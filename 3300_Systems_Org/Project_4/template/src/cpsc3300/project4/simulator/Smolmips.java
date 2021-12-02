package cpsc3300.project4.simulator;

import java.util.Scanner;

public class Smolmips {
	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("Usage:\n\tjava smolmips <instruction binary file> <data binary file>");
			System.exit(1);
		}

		Controller c = new Controller(args[0], args[1]);

		Scanner input = new Scanner(System.in);
		boolean OK = false;
		do {
			System.out.println("Run full program (r) or single-step instructions(s)?");
			char mode = input.nextLine().charAt(0);
			if (mode == 'r') {
				c.runAll();
				OK = true;
			} else if (mode == 's') {
				c.runSingleStep();
				OK = true;
			}
			else {
				System.out.println("Unrecognized input.");
			}
		} while (!OK);
	}
}
