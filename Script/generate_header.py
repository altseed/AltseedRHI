import re
import codecs
import os
import os.path

def cd(path):
	os.chdir(path)

def cdToScript():
	cd(os.path.dirname(os.path.abspath(__file__)))

class CreateHeader:
	def __init__(self):
		self.lines = []

	def append(self,s):
		self.lines.append(s)

	def readLines(self,path):
		f = codecs.open(path, 'r','utf-8_sig')
		line = f.readline()
		while line:
			if re.search('include( ?)\"', line) != None:
				line = f.readline()
				continue;

			if re.search('#pragma once', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <asd.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Graphics/asd.', line) != None:
				line = f.readline()
				continue;

			if re.search('#include <Math/asd.', line) != None:
				line = f.readline()
				continue;

			self.lines.append(line)
			line = f.readline()
		self.lines.append('\r\n')
		f.close

	def output(self,path):
		f =  codecs.open(path, 'w','utf-8_sig')
		for line in self.lines:
			f.write(line)
		f.close()

cdToScript()

engineHeader = CreateHeader()
engineHeader.append('#pragma once')
engineHeader.readLines("../src/ar.Base.h")
engineHeader.readLines("../src/ar.ImageHelper.h")

engineHeader.output('../include/AltseedRHI.h')
