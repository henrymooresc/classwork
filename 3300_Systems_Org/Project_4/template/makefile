
run: runSimulator

compile: compileSimulator

compileSimulator :
	cd src/cpsc3300/project4/simulator && javac *.java models/*.java views/*.java

runSimulator: compileSimulator
	echo "place bin files in src/"
	cd src/ && java cpsc3300.project4.simulator.Smolmips output.bin jmemory.bin

clean :
	find src/ -name "*.class" -type f -delete
