// Program that does a bit of disassembly to show how the process works
// Works on linux, may not work on other operating systems due to some weirdness I do


// Reference links

// http://ref.x86asm.net/coder32.html
// http://ref.x86asm.net/coder32.html#modrm_byte_16
// http://www.c-jump.com/CIS77/CPU/x86/X77_0060_mod_reg_r_m_byte.htm


// Includes
#include <iostream>
#include <cstring>

// Enum for certain values
enum {
		AL, EAX, ES, CS, SS, DS, ONE, CL, XMM0, BND0, BAD, MM0,
		IMM8, IMM16, IMM32, REL8, REL32, PTR1632, R, RM,
		BYTE, WORD, DWORD, QWORD, FWORD, XMMWORD, BUILTIN, VAL
	};

// Instruction struct
typedef struct
{
    // Register size
    char regSize;

    // Mnemonic of the instruction
    const char * name;

    // Argument count
    char argument_count;

    // Arguments
    unsigned char arguments[4];
} INSTRUCTION;


// String cnostants for several registers and instructions
const char * R8[] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};
const char * R16[] = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
const char * MOD2[] = {"BX + SI ", "BX + DI ", "BP + SI ", "BP + DI ", "SI ", "DI ", "BP ", "BX "};
const char * ROSH[] = {"ROL", "ROR", "RCL", "RCR", "SHL", "SHR", "SAL", "SHR", "SAL", "SAR"};
const char * ADD_TO_CMP[] = {"ADD", "OR", "ADC", "SBB", "AND", "SUB", "XOR", "CMP"};

// Constants for a few instructions
#define INT "INT"
#define JMP "JMP"
#define JZ "JZ"
#define MOV "MOV"
#define SUB "SUB"

// Prints the instruction
// instruction = instruction to print
// bytes = all bytes to be processed
// ip = pointer to current instruction pointer
void printInstruction(INSTRUCTION instruction, unsigned char * bytes, int * ip)
{
    // Initialize Output
    char OUTPUT[0xFF] = "";

    // Print the address of the instruction pointer
    sprintf(OUTPUT + strlen(OUTPUT), "%08X: ", *ip);

    // Concatenate the instruction mnemonic
    strcat(OUTPUT, instruction.name);

    // Loop through instruction arguments
    for (int i = 0; i < instruction.argument_count; i++)
    {
        // If the first one add a tab
        if (!i) strcat(OUTPUT, "\t");
        // Otherwise add a comma
        else strcat(OUTPUT, ", ");

        // Switch based on the argument
        switch(instruction.arguments[i])
        {
            // If immediate byte memory
            case IMM8:
                sprintf(OUTPUT + strlen(OUTPUT), "%02Xh", bytes[(*ip)++]);
                break;
            // If immediate word memory
            case IMM16:
                sprintf(OUTPUT + strlen(OUTPUT), "%04Xh", (bytes[(*ip)++] + (bytes[(*ip)++] << 8)));
                break;
            // If the register is built into the opcode (B0 and B8)
            case BUILTIN:
                if (instruction.regSize == BYTE)
                    strcat(OUTPUT, R8[instruction.arguments[++i]]);
                else if (instruction.regSize == DWORD)
                    strcat(OUTPUT, R16[instruction.arguments[++i]]);
                break;
            // If normal register
            case R:
            {
                int reg = (bytes[*ip] & 0x38) >> 3;
                if (instruction.regSize == BYTE)
                    strcat(OUTPUT, R8[reg]);
                else if (instruction.regSize == DWORD)
                    strcat(OUTPUT, R16[reg]);
            } break;
            // If mod register
            case RM:
            {
                int mod = (bytes[*ip] & 0xC0) >> 6;
                int rm = (bytes[*ip] & 0x7);
                switch (mod)
                {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        (*ip)++;
                        strcat(OUTPUT, MOD2[rm]);
                        sprintf(OUTPUT + strlen(OUTPUT), "+ %04Xh", (bytes[(*ip)++] + (bytes[(*ip)] << 8)));
                        break;
                    case 3:
                        if (instruction.regSize == BYTE)
                            strcat(OUTPUT, R8[rm]);
                        else if (instruction.regSize == DWORD)
                            strcat(OUTPUT, R16[rm]);
                        break;
                }
                (*ip)++;
                
            } break;
            // If relative byte offset
            case REL8:
                sprintf(OUTPUT + strlen(OUTPUT), "%08X", (*ip + (signed char)(bytes[(*ip)++]) + 2));
                break;
            // If SS
            case SS:
                strcat(OUTPUT, "SS");
                break;
            // If DS
            case DS:
                strcat(OUTPUT, "DS");
                break;
            // If hard coded value
            case VAL:
                sprintf(OUTPUT + strlen(OUTPUT), "%d", instruction.arguments[++i]);
                break;
        }
    }

    // Add string terminator to output
    strcat(OUTPUT, "\0");

    // Print output
    printf("%s\n", OUTPUT);
}

void bytesToAsssembly(unsigned char *bytes, int size)
{

    std::cout << "\nInstructions:" << std::endl;

    int ip = 0;
    unsigned char op = bytes[ip++];

    while (ip < size)
    {
        switch (op)
        {
            unsigned char reg;


            // 2A		r						SUB	r8	r/m8					o..szapc	o..szapc			Subtract
            case 0x2a:
                printInstruction((INSTRUCTION) {BYTE, SUB, 2, R, RM}, bytes, &ip);
                break;


            // 74								JZ	rel8					....z...					Jump short if zero/equal (ZF=1)
            case 0x74:
                printInstruction((INSTRUCTION) {BYTE, JZ, 1, REL8}, bytes, &ip);
                break;

            // 83		0					L	ADD	r/m16/32	imm8					o..szapc	o..szapc			                Add
            // 83		1	03+				L	OR	r/m16/32	imm8					o..szapc	o..sz.pc	.....a..	o......c	Logical Inclusive OR
            // 83		2					L	ADC	r/m16/32	imm8				    .......c	o..szapc	o..szapc			    Add with Carry
            // 83		3					L	SBB	r/m16/32	imm8				    .......c	o..szapc	o..szapc			    Integer Subtraction with Borrow
            // 83		4	03+				L	AND	r/m16/32	imm8					o..szapc	o..sz.pc	.....a..	o......c	Logical AND
            // 83		5					L	SUB	r/m16/32	imm8					o..szapc	o..szapc			                Subtract
            // 83		6	03+				L	XOR	r/m16/32	imm8					o..szapc	o..sz.pc	.....a..	o......c	Logical Exclusive OR
            case 0x83:
                printInstruction((INSTRUCTION) {DWORD, ADD_TO_CMP[(int)(bytes[ip] & 0x38) >> 3], 2, RM, IMM8}, bytes, &ip);
                break;


            // 8A		r						MOV	r8	r/m8									Move
            case 0x8a:
                printInstruction((INSTRUCTION) {BYTE, MOV, 2, R, RM}, bytes, &ip);
                break;

            // 	8B		r						MOV	r16/32	r/m16/32									Move
            case 0x8b:
                printInstruction((INSTRUCTION) {DWORD, MOV, 2, R, RM}, bytes, &ip);
                break;


            // 8E		r						MOV	Sreg	r/m16									Move
            case 0x8e:
                printInstruction((INSTRUCTION) {DWORD, MOV, 2, DS, RM}, bytes, &ip);
                break;


            // 	B0+r							MOV	r8	imm8									Move
            case 0xb0:
            case 0xb1:
            case 0xb2:
            case 0xb3:
            case 0xb4:
            case 0xb5:
            case 0xb6:
            case 0xb7:
                reg = op - 0xb0;
                printInstruction((INSTRUCTION) {BYTE, MOV, 3, BUILTIN, reg, IMM8}, bytes, &ip);
                break;

            // 	B8+r							MOV	r16/32	imm16/32									Move
            case 0xb8:
            case 0xb9:
            case 0xba:
            case 0xbb:
            case 0xbc:
            case 0xbd:
            case 0xbe:
            case 0xbf:
                reg = op - 0xb8;
                printInstruction((INSTRUCTION) {DWORD, MOV, 3, BUILTIN, reg, IMM16}, bytes, &ip);
                break;


            // CD						f		INT	imm8	eFlags					..i.....	..i.....		..i.....	Call to Interrupt Procedure
            case 0xcd:
                printInstruction((INSTRUCTION) {BYTE, INT, 1, IMM8}, bytes, &ip);
                break;

            // D1		0						ROL	r/m16/32	1					o..szapc	o..szapc			        Rotate
            // D1		1						ROR	r/m16/32	1					o..szapc	o..szapc			        Rotate
            // D1		2						RCL	r/m16/32	1				    .......c	o..szapc	o..szapc		Rotate
            // D1		3						RCR	r/m16/32	1				    .......c	o..szapc	o..szapc		Rotate
            // D1		4						SHL	r/m16/32	1					o..szapc	o..sz.pc	.....a..		Shift
            //                                  SAL	r/m16/32	1		
            // D1		5						SHR	r/m16/32	1					o..szapc	o..sz.pc	.....a..		Shift
            // D1		6		U2				SAL	r/m16/32	1					o..szapc	o..sz.pc	.....a..		Shift
            //                                  SHL	r/m16/32	1		
            // D1		7						SAR	r/m16/32	1					o..szapc	o..sz.pc	.....a..		Shift
            case 0xd1:
                printInstruction((INSTRUCTION) {DWORD, ROSH[(int)(bytes[ip] & 0x38) >> 3], 3, RM, VAL, 1}, bytes, &ip);
                break;

            // 	EB								JMP	rel8										Jump
            case 0xeb:
                printInstruction((INSTRUCTION) {BYTE, JMP, 1, REL8}, bytes, &ip);
                break;

            default:
                std::cout << "Error parsing" << std::endl;
                exit(1);
        }
        op = bytes[ip++];
    }
}



// Bytes from the skeleton assembly file

// 	\xBB\x00\x00        MOV   BX, 0000h
//  \xB4\x02            MOV   AH, 02h
//  \xB0\x01            MOV   AL, 01h
//  \x2A\xE0            SUB   AH, AL
//  \x74\x02            JZ    00000005
// 	\xEB\x05
//  \xB8\x01\x00
//  \xEB\x03
//  \xB8\x00\x00
//  \x8B\xC8
// 	\xB8\x06\x00
//  \x8E\xD8
//  \xBB\x00\x00
//  \x8A\xDD
//  \xD1\xEB
//  \xD1\xEB
//  \xD1\xEB
//  \xD1\xEB
//  \x8A\x97\x02\x00
//  \xB4\x02
//  \xCD\x21
//  \x8A\xDD
//  \x83\xE3\x0F
//  \x8A\x97\x02\x00
//  \xB4\x02
// 	\xCD\x21
//  \x8A\xD9
//  \xD1\xEB
//  \xD1\xEB
//  \xD1\xEB
//  \xD1\xEB
// 	\x8A\x97\x02\x00
//  \xB4\x02
//  \xCD\x21
//  \x8A\xD9
//  \x83\xE3\x0F
//  \x8A\x97\x02\x00
//  \xB4\x02
//  \xCD\x21
//  \xB8\x00\x4C
// 	\xCD\x21


int main()
{
    unsigned char * bytes = (unsigned char *)"\xBB\x00\x00";

    bytesToAsssembly(bytes, 3);
}