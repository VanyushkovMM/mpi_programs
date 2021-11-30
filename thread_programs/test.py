# On the cmd:
# python test.py [-n count] *argv
# For example:
# python test.py -n 4 100000 10000000
# python test.py 100000 10000000

import subprocess
import sys
import os

if os.path.isfile("build/main/Debug/main.exe"):
	argv = sys.argv
	proc = "4"
	# [-n count]
	k = 1
	if (argv[k] == "-n"):
		proc = argv[k + 1]
		k += 2
	# fileMPI.exe
	com = ["build/main/Debug/main.exe"]
	# *argv
	argv = argv[k:]
	
	k = 1
	for variable in argv:
		print("-------------", k, "-------------")
		print("--- Variable:", variable)
		k += 1
		subprocess.run(com+[variable, proc])
else:
	print("Not found file")