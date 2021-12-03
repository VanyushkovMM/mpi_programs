import subprocess
import sys
import re
import os

countRepeat = "1"
if len(sys.argv) > 1: countRepeat=sys.argv[1]

print("Wait...")
text = subprocess.run(["python", "others/run_tests.py", countRepeat], stdout=subprocess.PIPE, text=True)
os.system("cls")

result = re.split(r"-{29}\n Name: {6}", text.stdout)[1:]
count = len(result) // 2
for i in range(len(result)):
	result[i] = re.split(r"-{29}\n", result[i])
	result[i][0] = re.split(r"\n \w+:\s+", result[i][0])
	result[i][0][2] = result[i][0][2][:-1]
	result[i][1] = re.split(r"[*]{13} \d+ [*]{13}\n", result[i][1])[1:]
	for j in range(len(result[i][1])):
		result[i][1][j] = re.split(r"Success\n\n", result[i][1][j])[:-1]
		for k in range(len(result[i][1][j])):
			result[i][1][j][k] = re.split(r"\n", result[i][1][j][k])[:-1]
			result[i][1][j][k][0] = result[i][1][j][k][0][:-1]
			for l in range(1, len(result[i][1][0][k])): # 3
				result[i][1][j][k][l] = re.split(r":\s+", result[i][1][j][k][l])
				result[i][1][j][k][l][1] = float(result[i][1][j][k][l][1])

for i in range(len(result)):
	for j in range(1, len(result[i][1])):
		for k in range(len(result[i][1][j])): # 4
			for l in range(1, len(result[i][1][0][k])): # 3
				result[i][1][0][k][l][1] += result[i][1][j][k][l][1]
	for k in range(len(result[i][1][j])): # 4
		for l in range(1, len(result[i][1][0][k])): # 3
			result[i][1][0][k][l][1] /= len(result[i][1])
	result[i][1] = result[i][1][0]

class Line():
	value = "Value"
	processes = "Processes"
	task = ["Task"]
	sequential = ["Sequential"]
	mpi = ["MPI"]
	thread = ["Thread"]

	def __init__(self, line1=None, line2=None):
		if line1 != None:
			self.value = line1[0][1]
			self.processes = line1[0][2]
			self.task = []
			self.sequential = []
			self.mpi = []
			self.thread = []
			for j in range(len(line1[1])):
				self.task += [line1[1][j][0]]
				self.sequential += [str(round(line1[1][j][1][1], 2))]
				self.mpi += [str(round(line1[1][j][2][1], 2))]
				self.thread += [str(round(line2[1][j][2][1], 2))]

	def __text(self, value, processes, task, sequential, mpi, thread):
		return value+"|"+processes+"|"+task+"|"+sequential+"|"+mpi+"|"+thread+"\n"

	def setLength(self, value, processes, task, sequential, mpi, thread):
		self.value += " " * (value - len(self.value))
		self.processes += " " * (processes - len(self.processes))
		for i in range(len(self.task)):
			self.task[i] += " " * (task - len(self.task[i]))
			self.sequential[i] = " " * (sequential - len(self.sequential[i])) + self.sequential[i]
			self.mpi[i] = " " * (mpi - len(self.mpi[i])) + self.mpi[i]
			self.thread[i] = " " * (thread - len(self.thread[i])) + self.thread[i]

	def getLength(self):
		value = len(self.value)
		processes = len(self.processes)
		task = len(self.task[0])
		sequential = len(self.sequential[0])
		mpi = len(self.mpi[0])
		thread = len(self.thread[0])
		for i in range(1, len(self.task)):
			if task < len(self.task[i]): task = len(self.task[i])
			if sequential < len(self.sequential[i]): sequential = len(self.sequential[i])
			if mpi < len(self.mpi[i]): mpi = len(self.mpi[i])
			if thread < len(self.thread[i]): thread = len(self.thread[i])
		return [value, processes, task, sequential, mpi, thread]

	def __repr__(self):
		text = ""
		line = (len(self.task) + 1) // 2
		for i in range(len(self.task)):
			if i+1 == line:
				text += self.__text(self.value, self.processes, self.task[i], self.sequential[i], self.mpi[i], self.thread[i])
			else:
				text += self.__text(" "*len(self.value), " "*len(self.processes), self.task[i], self.sequential[i], self.mpi[i], self.thread[i])
		return text + "-"*(len(text)//len(self.task) - 1)+"\n"

class Table():
	table = [Line()]
	def __init__(self, result):
		for i in range(len(result) // 2):
			self.table += [Line(result[2*i], result[2*i+1])]
		self.__setTableSize()

	def __setTableSize(self):
		size = [0,0,0,0,0,0]
		for i in range(len(self.table)):
			tmp = self.table[i].getLength()
			for j in range(6):
				if size[j] < tmp[j]: size[j] = tmp[j]
		for i in range(len(self.table)):
			self.table[i].setLength(size[0], size[1], size[2], size[3], size[4], size[5])

	def __repr__(self):
		text = ""
		for i in range(len(self.table)):
			text += str(self.table[i])
		return text
		
table = Table(result)
print(table)
f = open("others/results.txt", "w")
f.write(str(table))
f.close()
