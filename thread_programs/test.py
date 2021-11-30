import subprocess
import sys
import os

if os.path.isfile("thread_programs/build/main/Debug/main.exe"):
	argv = sys.argv
	proc = "4"
	# [-n count]
	k = 1
	if (argv[k] == "-n"):
		proc = argv[k + 1]
		k += 2
	# fileMPI.exe
	com = ["thread_programs/build/main/Debug/main.exe"]
	# *argv
	subprocess.run(com+argv[k:]+[proc])
else:
	print("Not found file")