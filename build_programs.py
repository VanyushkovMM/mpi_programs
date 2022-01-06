import os
import sys

if sys.platform == 'win32':
    import ctypes
    kernel32 = ctypes.windll.kernel32
    kernel32.SetConsoleMode(kernel32.GetStdHandle(-11), 7)

files=["mpi_programs", "thread_programs"]

def checkBuild(path):
	path += '/build/main/'
	if sys.platform == 'win32':
		path += 'Debug/main.exe'
	else:
		path += 'main'
	if os.path.isfile(path):
		print('\033[35m{}\033[37m'.format('Successfully build!')) # PURPLE
	else:
		print('\033[31m{}\033[37m'.format('Not build!')) # RED
	print()

def build_file(file):
	build="/build && cmake .. && cmake --build ."
	cd="cd "
	if not os.path.isdir(file+"/build"):
		os.system(cd+file+" && mkdir build")
	com = cd+file+build
	print('\033[33m{}\033[37m'.format('>>> '+com)) # YELLOW
	os.system(com)
	checkBuild(file)

if len(sys.argv) == 2 and (sys.argv[1] in ["1", "2"]):
	build_file(files[int(sys.argv[1])-1])
elif len(sys.argv) == 2 and (sys.argv[1] in files):
	build_file(sys.argv[1])
else:
	for file in files:
		build_file(file)
