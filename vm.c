#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define MAX_REG_LENGTH 10
#define MAX_DATA_LENGTH 50
#define MAX_PROGRAM_LENGTH 150

void print_execution(int line, char *opname, instruction IR, int PC, int BP, int SP, int RP, int *data_stack, int *register_stack){
	int i;
	// print out instruction and registers
	printf("%2d\t%s\t%d\t%d\t%d\t%d\t%d\t%d\t\t", line, opname, IR.l, IR.m, PC, BP, SP, RP);
	
	// print register stack
	for (i = MAX_REG_LENGTH - 1; i >= RP; i--)
		printf("%d ", register_stack[i]);
	printf("\n");
	
	// print data stack
	printf("\tdata stack : ");
	for (i = 0; i <= SP; i++)
		printf("%d ", data_stack[i]);
	printf("\n");
}

int base(int L, int BP, int *data_stack){
	int ctr = L;
	int rtn = BP;
	while (ctr > 0){
		rtn = data_stack[rtn];
		ctr--;
	}
	return rtn;
}

void execute_program(instruction *code, int printFlag){
	int PC = 0;
	int BP = 0;
	int SP = BP-1;
	int RP = MAX_REG_LENGTH;
	int Line = 0;
	int Halt = 1;
	int *temp = calloc(0,MAX_DATA_LENGTH*sizeof(int));
	instruction IR;
	int input = 0;

	int *data_stack = calloc(0,MAX_DATA_LENGTH*sizeof(int));
	
	int *register_stack = malloc(MAX_PROGRAM_LENGTH*sizeof(int));

	if (printFlag){
		printf("\t\t\t\tPC\tBP\tSP\tRP\n");
		printf("Initial values:\t\t\t%d\t%d\t%d\t%d\n", PC, BP, SP, RP);
	}

	while(Halt != 0){

		/*---------------------------Fetch Cycle-----------------------------------------------------------*/
		if(code != NULL){

			IR.opcode = code[PC].opcode;
			IR.l = code[PC].l;
			IR.m = code[PC].m;
			PC++;
		}
		/*--------------------------------------------------------------------------------------------------*/
		
		switch (IR.opcode){
			case 1://LIT 
				RP--;
				register_stack[RP] = IR.m;
				
				Line++;
				char nameofOpcode[4] = {"LIT"};
				print_execution(Line,nameofOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;
		case 2: // OPM 
				switch (IR.m){
					case 0: //RET
						SP = BP - 1;
						BP = register_stack[RP - 2];
						PC = register_stack[RP - 3];
						Line++;
						char RETOpcode[4] = {"RET"};
						print_execution(Line,RETOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
					break;

					case 1: //NEG
						register_stack[RP] = -1 * register_stack[RP];
						Line++;
						char NEGOpcode[4] = {"NEG"};
						print_execution(Line,NEGOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
					break;

					case 2: //ADD
						RP++;
						register_stack[RP] = register_stack[RP] + register_stack[RP - 1];
						Line++;
						char ADDOpcode[4] = {"ADD"};
						print_execution(Line,ADDOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
						
					break;

					case 3: //SUB
						RP++;
						register_stack[RP] = register_stack[RP] - register_stack[RP - 1];
						Line++;
						char SUBOpcode[4] = {"SUB"};
						print_execution(Line,SUBOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
						
					break;

					case 4: //MUL
						RP++;
						register_stack[RP] = register_stack[RP] * register_stack[RP - 1];
						Line++;
						char MULOpcode[4] = {"MUL"};
						print_execution(Line,MULOpcode,IR,PC, BP, SP,RP,data_stack,register_stack);
						
					break;

					case 5: //Div
						RP++;
						register_stack[RP] = register_stack[RP] / register_stack[RP - 1];
						Line++;
						char DIVOpcode[4] = {"DIV"};
						print_execution(Line,DIVOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			
					break;

					case 6: //EQl
						RP++;
						if(register_stack[RP] == register_stack[RP - 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}
						Line++;
						char EQLOpcode[4] = {"EQL"};
						print_execution(Line,EQLOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);	
						
					break;

					case 7://NEQ
						RP++;
						if(register_stack[RP] != register_stack[RP - 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}						
						Line++;
						char NEQOpcode[4] = {"NEQ"};
						print_execution(Line,NEQOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
					break;

					case 8://LSS
						RP++;
						if(register_stack[RP] < register_stack[RP - 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}
						Line++;
						char LSSOpcode[4] = {"LSS"};
						print_execution(Line,LSSOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);	
						
					break;

					case 9://LEQ
						RP++;
						if(register_stack[RP] <= register_stack[RP - 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}
						Line++;
						char LEQOpcode[4] = {"LEQ"};
						print_execution(Line,LEQOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
						
					break;

					case 10://GTR
						RP++;
						if( register_stack[RP] > register_stack[RP + 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}
						Line++;
						char GTROpcode[4] = {"GTR"};
						print_execution(Line,GTROpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);	
						
					break;

					case 11://GEQ
						RP++;
						if( register_stack[RP] >= register_stack[RP + 1]){
							register_stack[RP] = 1;
						}else{
							register_stack[RP] = 0;
						}
					
						Line++;
						char GEQOpcode[4] = {"GEQ"};
						print_execution(Line,GEQOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
						
					break;
				}
			break;

			case 3: // LOD 
				RP--;
				register_stack[RP] = data_stack[base(IR.l,BP,data_stack) + IR.m]; //Review this.
        		Line++;
        		char LODOpcode[4] = {"LOD"};
        		print_execution(Line,LODOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 4: //STO 
				data_stack[base(IR.l,BP,data_stack) + IR.m] = register_stack[RP];// Review this.
				RP++;
        		Line++;
        		char STOOpcode[4] = {"STO"};
       			print_execution(Line,STOOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 5: //CAL 
				register_stack[SP-1] = 0;
				register_stack[SP-2] = BP;
				register_stack[SP-3] = PC;
				BP = SP-1;
				PC = IR.m;
        		Line++;
        		char CALOpcode[4] = {"CAL"};
        		print_execution(Line,CALOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 6: //INC 
				SP = SP + IR.m;
        		Line++;

				temp = data_stack;
				data_stack = calloc(0,IR.m*sizeof(int));

				for(int i=0;i<IR.m;i++){
					data_stack[i] = temp[i];
				}

        		char INCOpcode[4] = {"INC"};
        		print_execution(Line,INCOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 7: //JUMP
				PC = IR.m;
        		Line++;
        		char JMPOpcode[4] = {"JMP"};
        		print_execution(Line,JMPOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 8: //JPC 
				RP++;
				if(register_stack[RP] == 0){
					register_stack[IR.opcode];
				}
        		Line++;
        		char JPCOpcode[4] = {"JPC"};
        		print_execution(Line,JPCOpcode,IR,PC,  BP, SP,RP,data_stack,register_stack);
			break;

			case 9: //WRT,RED,HAL
				switch (IR.m){
          			case 1://WRT
					  	printf("Top of Stack Value: %d\n",data_stack[SP]);
					  	Line++;
						char WRTOpcode[4] = {"WRT"};
					  	RP++;
						print_execution(Line,WRTOpcode,IR,PC,BP, SP,RP,data_stack,register_stack);
          			break;

          			case 2://RED
						scanf("%d",&input);
						RP--;
						register_stack[RP] = input;
						Line++;
						char REDOpcode[4] = {"RED"};
						print_execution(Line,REDOpcode,IR,PC,BP, SP,RP,data_stack,register_stack);
          			break;

          			case 3: //HAL
						Halt = 0;
						Line++;
						char HALOpcode[4] = {"HAL"};
						print_execution(Line,HALOpcode,IR,PC,BP, SP,RP,data_stack,register_stack);
          			break;
				}
			break;
		}
	}
}
