.data

a = 145
b = 78
c = 92
d = 103

.text

_start:
    LDA a
    ADD b
    SUB c
    ADD d
    OUT
    HLT
