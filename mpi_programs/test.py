import subprocess
import sys
import os

path = "build/main/Debug/main.exe"
if os.path.isdir("mpi_programs"):
	path = "mpi_programs/" + path

if os.path.isfile(path):
	argv = sys.argv
	com = ["mpiexec"]
	# [-n count]
	k = 1
	if (argv[k] == "-n"):
		com += argv[k:k+2]
		k += 2
	
	subprocess.run(com+[path]+argv[k:])
else:
	print("Not found file")