.data

a = 65
b = 98
c = 17
d = 107
e = 144
f = 54
g = 78

.text

_start:
	LDA a
	ADD b
	SUB c
	ADD d
	SUB e
	ADD f
	SUB g
	OUT
	HLT
