/*
 * tokenizer.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */
enum type{Mal, Zero, Oct, Dec, Hex, Float, Invalid}; //delcares enum type to differeniate the types of tokens

struct TokenizerT_ { //definition of TokenizerT
	char *fullString; //contains the fullString input
	enum type token; //type of the token
};

typedef struct TokenizerT_ TokenizerT;

/*
 * TKCreate creates a new TokenizerT object for a given token stream
 * (given as a string).
 * 
 * TKCreate should copy the arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 *
 * You need to fill in this function as part of your implementation.
 */

TokenizerT *TKCreate( char * ts ) { 
	 TokenizerT *tp = malloc(sizeof(TokenizerT)); //allocates memory to Tokenizer *tp
	 tp->fullString = ts; //sets the value of fullString to the char * ts that was passed in 
	 return tp; //returns the Tokenizer pointer that was created
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy( TokenizerT * tk ) {
	free(tk); //frees the memory allocated to the tokenizer
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT * tk ){
  int i; //creates a counter
  char *temp = malloc(strlen((*tk).fullString) + 1); //largest possible size for the string + 1 for '\0'
  int num = 0; //sets the default case to 0
  	for(i = 0; strlen(tk->fullString) > 0; i++){
  		char c = (*tk).fullString[0];
  		//printf("%d\n", num);
  		switch(num){ //switch statement based on the previous state
			case 0: //first character only
				if(isblank(c)){
					num = 0;
				}else if(c == '0'){ //if zero Token could be a Octal, the Decimal '0', or Hexadecimal
					tk->token = Zero; //sets token to type Zero
					num = 1;
				}else if(c >= '1' && c <= '9'){	//if first digit is 1-9, Token is a decimal
					tk->token = Dec; //sets token to type Dec
					num = 2;
				}else{ //if Token is not Octal, Decimal, Hexadecimal, or Float it is an error
					tk->token = Invalid; //sets token to type Invalid
					num = 8; //Returns Invalid type
				}
				break;
			
			case 1: //means the first character was '0'
				if(isblank(c)){
					temp[i] = '\0';
					return temp;
				}else if(c >= '0' && c <= '7'){ //tests if the Token is Octal
					tk->token = Oct;
					num = 3;
				}else if(c == 'x' || c == 'X'){ //tests if the Token is Hexadecimal
					num = 4;
				}else if(c == '.'){	//tests if the Token is a Float
					tk->token = Mal;
					num = 5;
				}else{ //else Token is invalid
					temp[i] = '\0';
					return temp;
				}
				break;

			case 2:
				if(isblank(c)){
					temp[i] = '\0';
					return temp;
				}else if(isdigit(c)){ //tests if the Token is still Decimal
					//do nothing
				}else if(c == '.'){ //tests if the Token is a Float
					tk->token = Mal;
					num = 5;
				}else if(c == 'e' || c == 'E'){ //tests if Token is Float
					tk->token = Mal;
					num = 6;
				}else{//else return error
					temp[i] = '\0';
					return temp;
				}
				break;

			case 3:
				if(!(c >= '0' && c <= '7')){ //tests if Token is still Octal
					temp[i] = '\0';
					return temp;
				}
				break;

			case 4:
				if(!isxdigit(c)){ //tests if Token is still Hexadecimal
					temp[i] = '\0';
					return temp;
				}
				tk->token = Hex;
				break;

			case 5:
				if(!isdigit(c) && tk->token != Float){ //tests if Token is still Float
					temp[i] = '\0';
					return temp;
				}
				tk->token = Float;
				if(tk->token == Float && (c == 'e' || c == 'E')){ //tests if Token is still Float
					tk->token = Mal;
					num = 6;
				}
				break;

			case 6:
				if(c == '+' || c == '-'){ //tests if Token is still Float
					num = 7;
				}else if(isdigit(c)){
					tk->token = Float;
				}
				else{//else return error
					temp[i] = '\0';
					return temp;
				}
				break;

			 case 7:
			 	if(!isdigit(c)){//tests if Token is still Float
					temp[i] = '\0';
					return temp;
			 	}
			 	tk->token = Float;
			 	break;
			 case 8:
			 	tk->token = Invalid;
			 	//printf("it got here");
			 	if(isdigit(c) || isblank(c)){
			 		temp[i] = '\0';
			 		return temp; //Returns Invalid type
			 	}
			 	break;
		}
		if(!isblank(c)){
			temp[i] = (*tk).fullString[0];
		}else{
			i--;
		}
		tk->fullString++; //moves to next char
	}
	if(strlen(temp) == 0){
	  	free(temp); //frees the memory that is no longer needed
	  	return 0; //returns zero meaning that the function is finished
	}
	temp[i] = '\0'; //adds '\0' delimiter
	return temp; //returns the value of temp
}

/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	  char *var;
	  if(argv[1] != NULL){ //tests if there is an input at the beginning
		  	TokenizerT *t = TKCreate(argv[1]); //creates a new Token
		  
		  for(var = TKGetNextToken(t); var != 0 && strlen(var) != 0; var = TKGetNextToken(t)){ //for loop for Tokens
		  	//printf("%s\n", var);
		  	if(t->token == Zero){
		  		printf("%s is of type Zero\n", var); //prints out type Zero
		  	}else if(t->token == Oct){ 
		  		printf("%s is of type Octal\n", var); //prints out Octal
		  	}else if(t->token == Dec){
		  		printf("%s is of type Decimal\n", var); //prints out Decimal
		  	}else if(t->token == Hex){
		  		printf("%s is of type Hexademical\n", var); //prints out Hexadecimal
		  	}else if(t->token == Float){
		  		printf("%s is of type Float\n", var); //prints out Float
		  	}else if(t->token == Invalid){
		  		int i;
		  		printf("[");
		  		for(i = 0; i < strlen(var)-1 && var[i] != '\0'; i++){
		  			printf("0x%0x ", var[i]);
		  		}
		  		printf("0x%0x] is invalid input!\n", var[i]);
		  	}else{
		  		printf("%s is Malformed\n", var);
		  	}
		  }
		TKDestroy(t);  //frees memory allocated to Tokenizer
	}else{
		printf("You must provide an input!\n"); //returns error if no input
	}
  return 0;
}
