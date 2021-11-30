import os
from sys import argv

files=["mpi_programs", "thread_programs"]

def build_file(file):
	build="cd build && cmake .. && cmake --build ."
	cd="cd "
	if not os.path.isdir(file+"/build"):
		os.system(cd+file+" && mkdir build")
	os.system(cd+file+" && "+build)

if len(argv) == 2 and (argv[1] in ["1", "2"]):
	build_file(files[int(argv[1])-1])
else:
	for file in files:
		build_file(file)
