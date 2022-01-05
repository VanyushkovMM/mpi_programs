import subprocess
import sys
import os

path = "build/main/"
if sys.platform == 'win32':
	path = path + "Debug/main.exe"
elif sys.platform == 'linux':
	path = path + 'main'
if os.path.isdir("thread_programs"):
	path = "thread_programs/" + path

if os.path.isfile(path):
	argv = sys.argv
	proc = "4"
	# [-n count]
	k = 1
	if (argv[k] in ['-n', '-np']):
		proc = argv[k + 1]
		k += 2

	subprocess.run([path]+argv[k:]+[proc])
else:
	print("Not found file")