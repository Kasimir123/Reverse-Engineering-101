; Show jump if zero

MOV AH, 1
MOV AL, 1

SUB AH, AL

JZ true
JMP false

true:
MOV AX, 1
JMP done

false:
MOV AX, 0

done:
