import subprocess
import sys
import os

path = "build/main/"
if sys.platform == 'win32':
	path = path + "Debug/main.exe"
elif sys.platform == 'linux':
	path = path + 'main'
if os.path.isdir("mpi_programs"):
	path = "mpi_programs/" + path

if os.path.isfile(path):
	argv = sys.argv
	com = ["mpiexec"]
	# [-n count]
	k = 1
	if (argv[k] in ['-n', '-np']):
		com += argv[k:k+2]
		k += 2
	
	subprocess.run(com+[path]+argv[k:])
else:
	print("Not found file")