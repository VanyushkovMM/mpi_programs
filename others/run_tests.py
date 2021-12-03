import subprocess
import sys
import os

def printTextFile(file, param, i, result):
	if i <= 1:
		print("-"*29)
		print(" Name:     ", file)
		print(" Values:   ", param[1])
		print(" Processes:", param[0])
		print("-"*29)
	print("*"*13, i, "*"*13)
	print(result, end="")

count = 1
argv = sys.argv
if len(argv) > 1: count = int(argv[1])

path = "test_arguments.txt"
if not os.path.isfile(path):
	path = "others/" + path

f = open(path, "r")
text = f.read().split('\n')
f.close()

files = ["mpi_programs", "thread_programs"]
for line in text:
	for file in files:
		for i in range(count):
			param = line.split()
			com = ["python", file + "/test.py"]
			if len(param) > 1: com += ["-n"]
			com += param
			if len(param) == 1: param = ["4"] + param
			result = subprocess.run(com, stdout=subprocess.PIPE, text=True)
			printTextFile(file, param, i+1, result.stdout)

