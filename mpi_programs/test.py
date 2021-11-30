import subprocess
import sys
import os

if os.path.isfile("mpi_programs/build/main/Debug/main.exe"):
	argv = sys.argv
	com = ["mpiexec"]
	# [-n count]
	k = 1
	if (argv[k] == "-n"):
		com += argv[k:k+2]
		k += 2
	# main.exe
	com += ["mpi_programs/build/main/Debug/main.exe"]
	
	subprocess.run(com+argv[k:])
else:
	print("Not found file")