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
enum type{Oct, Dec, Hex, Float, Invalid};

struct TokenizerT_ {
	char *fullString;
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

char *TKGetNextToken(TokenizerT * tk ) {
  int count = 0;
  char *temp = malloc(strlen((*tk).fullString) + 1); //largest possible size for the string + 1 for '\0'
  while(strlen((*tk).fullString) != 0 && !(isblank((*tk).fullString[0]) == 0)){ //if it is a blank space it keeps going
  	tk->fullString++; //moves on to next char
  }
  while(strlen((*tk).fullString) != 0 && isblank((*tk).fullString[0]) == 0){ //if it is not black it keeps going
  	temp[count] = (*tk).fullString[0]; //replaces the value at temp[count] with the first char of fullString
  	tk->fullString++; //moves on to next char
  	count++; //adds one to the counter for the temp
  }
  if(strlen(temp) == 0){ //if the temp does not have anything stored, free it
  	free(temp); //frees the memory that is no longer needed
  	return 0; //returns zero meaning that the function is finished
  }
  temp[count] = '\0'; //delimited by '\0'
  return temp; //returns value of temp
}

enum type getType(char * token){
	int num = 0;
	int i;
	for(i = 0; i < strlen(token); i++){
		char c = token[i];
		//printf("\nThe char is: %c and the num is %d", c, num);
		switch(num){

			case 0:
				if(c == '0'){
					num = 1;
				}else if(c >= '1' && c <= '9'){
					num = 2;
				}else{
					return Invalid; //Returns Invalid type
				}
				break;
			
			case 1:
				if(c >= '0' && c <= '7'){
					num = 3;
				}else if(c == 'x' || c == 'X'){
					num = 4;
				}else if(c == '.'){
					num = 5;
				}else{
					return Invalid; //Returns Invalid type
				}
				break;

			case 2:
				if(isdigit(c)){
					//do nothing
				}else if(c == '.'){
					num = 5;
				}else if(c == 'e' || c == 'E'){
					num = 6;
				}else{
					return Invalid; //Returns Invalid type
				}
				break;

			case 3:
				if(!(c >= '0' && c <= '7')){
					return Invalid; //Returns Invalid type
				}
				break;

			case 4:
				if(!isxdigit(c)){
					return Invalid; //Returns Invalid type
				}
				break;

			case 5:
				if(isdigit(c)){
					//do nothing
				}else if(c == 'e' || c == 'E'){
					num = 6;
				}else{
					return Invalid; //Returns Invalid type
				}
				break;

			case 6:
				if(c == '+' || c == '-' || isdigit(c)){
					num = 7;
				}else{
					return Invalid; //Returns Invalid type
				}
				break;

			 case 7:
			 	if(!isdigit(c)){
			 		return Invalid; //Returns Invalid type
			 	}
			 	break;
		}
	}
	if(num == 3)
		return Oct; //Returns Octal type

	if(num == 4)
		return Hex; //Returns Hexidemical type

	if(num == 5 || num == 2 || num == 1)
		return Dec; //Returns Decimal type

	if(num == 7)
		return Float; //Returns Float type

	return Invalid; //Returns Invalid type
}
/*
 * main will have a string argument (in argv[1]).
 * The string argument contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	  char *var;
	  if(argv[1] != NULL){
		  	TokenizerT *t = TKCreate(argv[1]); //creates a new Token
		  
		  for(var = TKGetNextToken(t); var != 0 && strlen(var) != 0; var = TKGetNextToken(t)){
		  	//printf("%s\n", var);
		  	enum type token = getType(var);
		  	if(token == Oct){
		  		printf("%s is of type Octal\n", var);
		  	}else if(token == Dec){
		  		printf("%s is of type Decimal\n", var);
		  	}else if(token == Hex){
		  		printf("%s is of type Hexademical\n", var);
		  	}else if(token == Float){
		  		printf("%s is of type Float\n", var);
		  	}else{
		  		printf("[%s] is malformed!\n", var);
		  	}
		  }
		TKDestroy(t);  
	}else{
		printf("You must provide an input!\n");
	}
  return 0;
}
