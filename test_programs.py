import subprocess
import sys

def printTextFile(file, param):
	k = 0
	if len(param) > 1: k += 1
	print("-"*29)
	print("------", file)
	print("------ Var:", param[k])
	print("-"*29)

count = 1
argv = sys.argv
if len(argv) > 1: count = int(argv[1])

f = open("test_arguments.txt", "r")
text = f.read().split('\n')
f.close()

files = ["mpi_programs", "thread_programs"]
for i in range(count):
	if count > 1: print("*"*20, i+1, "*"*20)
	for line in text:
		for file in files:
			param = line.split()
			printTextFile(file, param)
		
			com = ["python", file + "/test.py"]
			if len(param) > 1: com += ["-n"]
			com += param
			result = subprocess.run(com, stdout=subprocess.PIPE, text=True)
			print(result.stdout)
