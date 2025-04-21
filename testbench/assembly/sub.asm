.data       ; begin .data section. .data section must exist and
            ; come before the .text section

a = 215     ; variable a assigned to 215
            ; variables are to be 8-bit long, between 0 and 255

b = 79      ; variable b assigned to 79

.text       ; begin .text section

_start:     ; entry point must exist with the label _start

    LDA a   ; load variable 'a' to the accumulator register

    SUB b   ; subtract from the accumulator's content the variable 'b'
            ; store the result in the accumulator register

    OUT     ; show the accumulator's content in the display port

    HLT     ; halt the CPU
