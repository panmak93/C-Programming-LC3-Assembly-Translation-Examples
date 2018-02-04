#include <stdio.h>
#include <string.h>
#include <stdbool.h>

unsigned short operation(char operator[]) {
	unsigned short instruction;
	if(strcmp(operator, "add") ==0)instruction = 0x1000;
	else if(strcmp(operator, "jmp") ==0)instruction = 0xc000;
	else if(strcmp(operator, "ld") ==0)instruction = 0x2000;
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

short compare(char file2[],char *operand2,short add){
	FILE *fp2 = fopen(file2, "r");
		do{
			char str[100];
			if (fgets(str, 100, fp2) == NULL) break;
			char delimiter[2]=" ";
			char *token;
			short item_add, offset, neg, pos;
			neg = 0x01ff;
			pos = 0x00ff;
			token=strtok(str,delimiter);
			if((strncmp(token, operand2, strlen(token)) == 0)&&(strncmp(token, operand2, strlen(operand2)-2) == 0)){
				token = strtok (NULL, delimiter);
				item_add = strtol(token, NULL ,16);
				offset = item_add - add;
				if (add > item_add){
					offset = offset & neg;
				}
				else{
					offset = offset & pos;
				}
				return offset;
			}
		} while(true);

	fclose(fp2);
}

int main(int argc,char *argv[]) { 
	if ( argc != 4 ) {
		printf( "usage: %s file_name symboltable_file_name starting_address \n", argv[0]);
		}
	else{
		char str[100], *operator, *operand1, *operand2, *dest;
		char* startadd_str = argv[3];
		char delimiter[3]=" ,";
		short add;
		unsigned short instruction, shift;
		shift = 0;
		FILE *fp = fopen(argv[1], "r");
		add = strtol(startadd_str, NULL ,16);
		do{
			add++;
			if (fgets(str, 100, fp) == NULL) break;
			operator = strtok(str,delimiter);	
			*operator = tolower(*operator);	
			instruction = operation(operator);
			if(strcmp(operator, "jmp") ==0){
				operand1 = strtok(NULL,delimiter);
				shift = reg(operand1);
				shift = shift << 6;
				instruction = instruction | shift;				
			}
			else if(strcmp(operator, "ld") ==0){	
				operand1 = strtok (NULL, delimiter);
				shift = reg(operand1);
				shift = shift << 9;
				instruction = instruction | shift;
				operand2 = strtok (NULL, delimiter);
				shift = compare(argv[2], operand2, add);
				instruction = instruction | shift;
			}
			else{
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
					short op2;
					op2 = strtol(operand2, NULL ,10);
					shift = imm(op2);
					instruction = instruction | 0x0020;
				}
				instruction = instruction | shift;
			}
			printf("%04x\n", instruction);		
			
		} while(true);
		fclose(fp);
	};
}
