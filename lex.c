#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"
#define MAX_NUMBER_TOKENS 1000
#define MAX_IDENT_LEN 11
#define MAX_NUMBER_LEN 5

//Felipe Romero && Naomi Palm's Code

lexeme *list;
int lex_index;

void printlexerror(int type);
int reservedwords();
void symbols(char buffer[]);
void printtokens();

lexeme *lexanalyzer(char *input, int printFlag){
	list = malloc(sizeof(lexeme) * MAX_NUMBER_TOKENS);
	char buffer[MAX_NUMBER_TOKENS] = {'\0'};
	lex_index = 1;
	int i = 0;
	int j = 0;
	int y = 0;
	int comments = 0;

	while(input[i] != '\0'){//This while loops is going to check what the input is a letter,symbol,space,cntrl
			y = 0;
			j = 0;

			while (buffer[y] != '\0'){//Is going to conatain part of the inputs so we canm hold multiple things at once/
				buffer[y] = '\0';
				y++;
			}
			if(isspace(input[i]) != 0 || iscntrl(input[i]) != 0){//If there is a space token
				i++;
			}else if(isalpha(input[i]) != 0){//If there is a letter token
				printf("%c\n",input[i]);
				if(comments == 0){
					buffer[j] = input[i];
					j++;
					i++;
					while(isalpha(input[i]) != 0 || isdigit(input[i]) != 0){
						buffer[j] = input[i];
						j++;
						i++;
					}
					if(j > MAX_IDENT_LEN){// Max Identifier Length Error
						printlexerror(3);
					}
					else{
						if(reservedwords(buffer) != 0){ //The reservcewords function is going to add the certain comands to the list and this if stament is to handle variables and their values.
							strcpy(list[lex_index].name, buffer);
							list[lex_index].type = identsym;
							lex_index++;
						}
					}
				}else if(comments == 1){
					i++;
				}	
			}else if(ispunct(input[i]) != 0){ //If there is a symbol token 
				printf("%c\n",input[i]);
				if(comments == 0){
					if(input[i] == '/' && input[i+1] == '*'){
						comments = 1;
					}else{
						buffer[j] = input[i];
					}
					if(input[i] == '=' && input[i+1] == '='){
						j++;
						buffer[j] = input[i+1];
						i++;
					}else if(input[i] == '!' && input[i+1] == '='){
						j++;
						buffer[j] = input[i+1];
						i++;
					}else if(input[i] == '<' && input[i+1] == '='){
						j++;
						buffer[j] = input[i+1];
						i++;
					}else if(input[i] == '>' && input[i+1] == '='){
						j++;
						buffer[j] = input[i+1];
						i++;
					}else if(input[i] == ':' && input[i+1] == '='){
						j++;
						buffer[j] = input[i+1];
						i++;
					}
					symbols(buffer);
					i++;
				}else if(comments == 1){
					char temp = input[i];
					if(input[i] == '*' && input[i+1] == '/'){
						comments = 0;
						i++;
						buffer[j] = temp;
					}
					i++;
				}
			}else if (isdigit(input[i]) != 0){ //If there is a digit token.
				printf("%c\n",input[i]);
				buffer[j] = input[i];
				j++;
				i++;
				while(isdigit(input[i]) != 0){
					buffer[j] = input[i];
					j++;
					i++;
				}
				if(j >= MAX_NUMBER_LEN){// Max Number Length Error
					printlexerror(2);
				}
				else if(isalpha(input[i]) != 0){
					printlexerror(1);
				}
				else{
					sscanf(buffer, "%d", &list[lex_index].value);
					list[lex_index].type = numbersym;
					lex_index++;
				}
			}
		}

	if(printFlag)
		printtokens();
	// these last two lines are really important for the rest of the package to run
	list[lex_index].type = -1;
	return list;
}

int reservedwords(char buffer[]){//This function adds the reserve words to the list with their type and value.
	int result = 1;

	if(strcmp("const", buffer) == 0){
		list[lex_index].value = 2;
		list[lex_index].type = constsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("var", buffer) == 0){
		list[lex_index].value = 4;
		list[lex_index].type = varsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("procedure", buffer) == 0){
		list[lex_index].value = 6;
		list[lex_index].type = procsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("call", buffer) == 0){
		list[lex_index].value = 8;
		list[lex_index].type = callsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("if", buffer) == 0){
		list[lex_index].value = 10;
		list[lex_index].type = ifsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("then", buffer) == 0){
		list[lex_index].value = 12;
		list[lex_index].type = thensym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("else", buffer) == 0){
		list[lex_index].value = 14;
		list[lex_index].type = elsesym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("while", buffer) == 0){
		list[lex_index].value = 16;
		list[lex_index].type = whilesym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("do", buffer) == 0){
		list[lex_index].value = 18;
		list[lex_index].type = dosym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("begin", buffer) == 0){
		list[lex_index].value = 20;
		list[lex_index].type = beginsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("end", buffer) == 0){
		list[lex_index].value = 22;
		list[lex_index].type = endsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("read", buffer) == 0){
		list[lex_index].value = 24;
		list[lex_index].type = readsym;
		lex_index++;
		result = 0;
	}
	else if(strcmp("write", buffer) == 0){
		list[lex_index].value = 26;
		list[lex_index].type = writesym;
		lex_index++;
		result = 0;
	}
	return result;
}

void symbols(char buffer[]){//This function adds symbols to the list with their type and value.
	if(strcmp("==", buffer) == 0){
		list[lex_index].value = 1;
		list[lex_index].type = eqlsym;
		lex_index++;
	}
	else if(strcmp("!=", buffer) == 0){
		list[lex_index].value = 3;
		list[lex_index].type = neqsym;
		lex_index++;
	}
	else if(strcmp("<", buffer) == 0){
		list[lex_index].value = 5;
		list[lex_index].type = lsssym;
		lex_index++;
	}
	else if(strcmp("<=", buffer) == 0){
		list[lex_index].value = 7;
		list[lex_index].type = leqsym;
		lex_index++;
	}
	else if(strcmp(">", buffer) == 0){
		list[lex_index].value = 9;
		list[lex_index].type = gtrsym;
		lex_index++;
	}
	else if(strcmp(">=", buffer) == 0){
		list[lex_index].value = 11;
		list[lex_index].type = geqsym;
		lex_index++;
	}
	else if(strcmp("*", buffer) == 0){
		list[lex_index].value = 13;
		list[lex_index].type = multsym;
		lex_index++;
	}
	else if(strcmp("/", buffer) == 0){
		list[lex_index].value = 15;
		list[lex_index].type = divsym;
		lex_index++;
	}
	else if(strcmp("+", buffer) == 0){
		list[lex_index].value = 17;
		list[lex_index].type = plussym;
		lex_index++;
	}
	else if(strcmp("-", buffer) == 0){
		list[lex_index].value = 19;
		list[lex_index].type = minussym;
		lex_index++;
	}
	else if(strcmp("(", buffer) == 0){
		list[lex_index].value = 21;
		list[lex_index].type = lparensym;
		lex_index++;
	}
	else if(strcmp(")", buffer) == 0){
		list[lex_index].value = 23;
		list[lex_index].type = rparensym;
		lex_index++;
	}
	else if(strcmp(",", buffer) == 0){
		list[lex_index].value = 25;
		list[lex_index].type = commasym;
		lex_index++;
	}
	else if(strcmp(".", buffer) == 0){
		list[lex_index].value = 27;
		list[lex_index].type = periodsym;
		lex_index++;
	}
	else if(strcmp(";", buffer) == 0){
		list[lex_index].value = 29;
		list[lex_index].type = semicolonsym;
		lex_index++;
	}
	else if(strcmp(":=", buffer) == 0){
		list[lex_index].value = 31;
		list[lex_index].type = assignsym;
		lex_index++;
	}
	else{
		printf("%s\n",buffer);
		printlexerror(4);
	}
}

void printtokens(){ //This function prints out the tokens in a specific format 
	int i = 0;
	printf("Lexeme Table:\n");
	printf("lexeme\t\ttoken type\n");
	for (i = 0; i < lex_index; i++){
		switch (list[i].type){
			case eqlsym:
				printf("%11s\t%d", "==", eqlsym);
				break;
			case neqsym:
				printf("%11s\t%d", "!=", neqsym);
				break;
			case lsssym:
				printf("%11s\t%d", "<", lsssym);
				break;
			case leqsym:
				printf("%11s\t%d", "<=", leqsym);
				break;
			case gtrsym:
				printf("%11s\t%d", ">", gtrsym);
				break;
			case geqsym:
				printf("%11s\t%d", ">=", geqsym);
				break;
			case multsym:
				printf("%11s\t%d", "*", multsym);
				break;
			case divsym:
				printf("%11s\t%d", "/", divsym);
				break;
			case plussym:
				printf("%11s\t%d", "+", plussym);
				break;
			case minussym:
				printf("%11s\t%d", "-", minussym);
				break;
			case lparensym:
				printf("%11s\t%d", "(", lparensym);
				break;
			case rparensym:
				printf("%11s\t%d", ")", rparensym);
				break;
			case commasym:
				printf("%11s\t%d", ",", commasym);
				break;
			case periodsym:
				printf("%11s\t%d", ".", periodsym);
				break;
			case semicolonsym:
				printf("%11s\t%d", ";", semicolonsym);
				break;
			case assignsym:
				printf("%11s\t%d", ":=", assignsym);
				break;
			case beginsym:
				printf("%11s\t%d", "begin", beginsym);
				break;
			case endsym:
				printf("%11s\t%d", "end", endsym);
				break;
			case ifsym:
				printf("%11s\t%d", "if", ifsym);
				break;
			case thensym:
				printf("%11s\t%d", "then", thensym);
				break;
			case elsesym:
				printf("%11s\t%d", "else", elsesym);
				break;
			case whilesym:
				printf("%11s\t%d", "while", whilesym);
				break;
			case dosym:
				printf("%11s\t%d", "do", dosym);
				break;
			case callsym:
				printf("%11s\t%d", "call", callsym);
				break;
			case writesym:
				printf("%11s\t%d", "write", writesym);
				break;
			case readsym:
				printf("%11s\t%d", "read", readsym);
				break;
			case constsym:
				printf("%11s\t%d", "const", constsym);
				break;
			case varsym:
				printf("%11s\t%d", "var", varsym);
				break;
			case procsym:
				printf("%11s\t%d", "procedure", procsym);
				break;
			case identsym:
				printf("%11s\t%d", list[i].name, identsym);
				break;
			case numbersym:
				printf("%11d\t%d", list[i].value, numbersym);
				break;
		}
		printf("\n");
	}
	printf("\n");
	printf("Token List:\n");
	for (i = 1; i < lex_index; i++){
		if (list[i].type == numbersym)
			printf("%d %d ", numbersym, list[i].value);
		else if (list[i].type == identsym)
			printf("%d %s ", identsym, list[i].name);
		else
			printf("%d ", list[i].type);
	}
	printf("\n");
}

void printlexerror(int type){//Prints the erros if there is a wrong token or breaks one fo the rules.
	if (type == 1)
		printf("Lexical Analyzer Error: Invalid Identifier\n");
	else if (type == 2)
		printf("Lexical Analyzer Error: Number Length\n");
	else if (type == 3)
		printf("Lexical Analyzer Error: Identifier Length\n");
	else if (type == 4)
		printf("Lexical Analyzer Error: Invalid Symbol\n");
	else if (type == 5)
		printf("Lexical Analyzer Error: Never-ending comment\n");
	else
		printf("Implementation Error: Unrecognized Error Type\n");
	
	free(list);
	return;
}