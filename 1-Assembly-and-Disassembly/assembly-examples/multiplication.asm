; Empty out AX
XOR AX, AX

; 8 bit unsigned multiplication - should get 36BC
MOV AL, 0E2h
MOV CL, 03Eh
MUL CL

; 8 bit multiplication - should get F8BC
MOV AL, 0E2h
MOV CL, 03Eh
IMUL CL

; 16 bit multiplication -  should get F50DB07 DB07 in AX and 0F50 in DX
MOV AX, 07EE1h
MOV CX, 01EE7h
IMUL CX

MOV AX, DX