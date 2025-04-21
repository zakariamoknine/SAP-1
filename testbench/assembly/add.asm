.data       ; begin .data section. .data section must exist and
            ; come before the .text section

a = 19      ; variable a assigned to 19
            ; variables are to be 8-bit long, between 0 and 255

b = 43      ; variable b assigned to 43

.text       ; begin .text section

_start:     ; entry point must exist with the label _start

    LDA a   ; load the variable 'a' to the accumulator register

    ADD b   ; add the accumulator's content to the variable 'b'
            ; store the result in the accumulator register

    OUT     ; show the accumulator's content in the display port

    HLT     ; halt the CPU
