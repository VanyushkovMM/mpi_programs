import shutil
import os
from sys import argv

files=["mpi_programs", "thread_programs"]

def del_build(file):
	path=file+"/build"
	if os.path.isdir(path):
		shutil.rmtree(path)
		print("Success del", path)
	else:
		print("Not found", path)

if len(argv) == 2 and (argv[1] in ["1", "2"]):
	del_build(files[int(argv[1])-1])
else:
	for file in files:
		del_build(file)