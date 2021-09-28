; Empty out AX
XOR AX, AX

; 8 bit division - AH: 01 AL: 21
MOV AX, 64h
MOV CL, 3
IDIV CL

; 16 bit division - AX: 07AF DX: 0008
XOR CX, CX
XOR DX, DX
MOV AX, 64h
MOV DX, 3
MOV CX, 64h
IDIV CX

MOV AX, DX