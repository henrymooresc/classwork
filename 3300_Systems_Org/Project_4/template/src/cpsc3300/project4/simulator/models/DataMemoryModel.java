package cpsc3300.project4.simulator.models;

import cpsc3300.project4.simulator.Controller;

import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public class DataMemoryModel {

	private List<Integer> data       = new ArrayList<Integer>(Controller.MEM_SIZE);
	private int           maxAddress = 0;

	public DataMemoryModel(String dataFile) {
		try {
			DataInputStream dat = new DataInputStream(new FileInputStream(dataFile));

			while (dat.available() > 0) {
				data.add(dat.readInt());
			}
		}
		catch (IOException e) {
			System.out.println("Unable to read data binary file");
			System.exit(1);
		}
		maxAddress = data.size() * 4;
	}

	public DataMemoryModel(int[] d) {
		for (Integer i : d) {
			data.add(i);
		}
		maxAddress = data.size() * 4;
	}

	public int readData(int addr) {
		try{return data.get(addr / 4);}
		catch (IndexOutOfBoundsException e){
			return 0;
		}
	}

	public void writeData(int addr, int word) {
		data.add(addr / 4, word);
		maxAddress = Math.max(maxAddress, addr);
	}

	public int getMaxAddress() {
		return maxAddress;
	}
}
