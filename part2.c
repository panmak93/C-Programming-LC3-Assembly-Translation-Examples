#include <stdio.h>
#include <string.h>
#include <stdbool.h>

unsigned short operation(char operator[]) {
	unsigned short instruction;
	if(strcmp(operator, "add") ==0)instruction = 0x1000;
	else {instruction = 0x5000;};
	return(instruction);
}

unsigned short reg(char reg_s[]) {
	char reg_char[2];
	short reg_int;
	unsigned short shift;
	memcpy(reg_char, &reg_s[1], 1);
	reg_int = strtol(reg_char, NULL ,10);
	shift = reg_int & 0x0007;
	return(shift);
}
		
unsigned short imm(int value) {
	unsigned short shift;
	shift = value & 0x001f;
	return(shift);
}			

int main(int argc,char *argv[]) { 
	if ( argc != 2 ) {
		printf( "usage: %s file_name \n", argv[0]);
		}
	else{
		char str[100], *operand1, *operand2, *dest, *operator;
		char delimiter[3]=" ,";
		unsigned short instruction, shift;
		shift = 0;
		FILE *fp = fopen(argv[1], "r");		
		do{
			if (fgets(str, 100, fp) == NULL) break;
			operator = strtok(str,delimiter);	
			*operator = tolower(*operator);	
			dest = strtok(NULL,delimiter);
			operand1 = strtok(NULL,delimiter);
			operand2 = strtok(NULL,delimiter);		
			
			instruction = operation(operator);
			shift = reg(dest);
			shift = shift << 9;
			instruction = instruction | shift;
			shift = reg(operand1);
			shift = shift << 6;
			instruction = instruction | shift;
			if(operand2[0]=='r'){
				shift = reg(operand2);
			}
			else{
				long op2;
				op2 = strtol(operand2, NULL ,10);
				shift = imm(op2);
				instruction = instruction | 0x0020;
			}
			instruction = instruction | shift;
			printf("%04x\n", instruction);		
			
		} while(true);
		fclose(fp);
	};
}

