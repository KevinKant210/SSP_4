/*
Authors:
    Caitlin Fabian
    Kevin Kant
Lexical Analyzer
Due: 06/25/2022
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//include the compil.er headerfile
#include "compiler.h"


#define MAX_LEXEME_LENGTH 500
#define false 0
#define true 1
#define iscomment -1

\

//different type of errors we can run into
typedef enum error_type{
    none = -2, no_letter_on_start = -3,long_number = -4, invalid_symbol = -5, comment_error = -6,too_long_identifier = -7 } error_type;









int isSpecialSymbol(char c,char next,lexeme* token){
    //function that determines the logic of tokenizing symbols
    if(c == '+'){
        
            token->name[0] = '+';
            
            return plussym;
        
    }

    
    if(c == '-'){

        
            token->name[0] = '-';
            return minussym;
        
    }
    

    if(c == '*'){
        
            token->name[0] = '*';
            return multsym;
        
    }

    if(c == '/'){
        if(next == '*'){
            return iscomment;
        }else{
            token->name[0] = '/';
            return slashsym;
        }

    }

    if(c == '%'){
        
            token->name[0] = '%';
            return modsym;
        
    }

    if(c == '='){
        
            token->name[0] = '=';
            return eqlsym;
        
    }


    if(c == '!'){
        if(next == '='){
            token->name[0] = '!';
            token->name[1] = '=';
            return neqsym;
        }
        
    }

    

    if(c == '<'){
        if(next == '='){
            token->name[0] = '<';
            token->name[1] = '=';
            return leqsym;
            
        }else{
             token->name[0] = '<';
            return lessym;
        }
           
        
        
    }

    if(c == '>'){
        if(next == '='){
            token->name[0] = '>';
            token->name[1] = '=';
            return geqsym;
        }else{
            token->name[0] = '>';
            return gtrsym;
        }
    }


    if(c == '('){
        
            token->name[0] = '(';
            return lparentsym;
        
    }

    if(c == ')'){
        
            token->name[0] = ')';
            return rparentsym;
        
    }

    if(c == ','){
        
            token->name[0] = ',';
            return commasym;
        
    }

    if(c == ';'){
        
            token->name[0] = ';';
            return semicolonsym;
        
    }

    if(c == '.'){
        
            token->name[0] = '.';
            return periodsym;
        
    }

    if(c == ':'){
        if(next == '='){
            token->name[0] = ':';
            token->name[1] = '=';
            return becomessym;
        }
    }

    token->error_type = invalid_symbol;
    return invalid_symbol;

    
}

void printName(lexeme token){
    //function that just prints the char array name of the token
    for(int i = 0 ; i < strlen(token.name); i++){
        if(isspace(token.name[i]) == false ){
            printf("%c",token.name[i]);
        }
        
        
    }
}

 



lexeme* lexical_analyzer(char* input){

    //need to change for reading in char* input

    lexeme* tokenTable = (lexeme*) malloc(sizeof(lexeme)*MAX_LEXEME_LENGTH);

    

    int inputIndex = 0;

    int halt = false;
    int tokenCounter = 0;
    while(halt == false){

        
        char c,next;

        lexeme newLex;

        for(int i = 0; i < 12; i++){
            newLex.name[i] = 0;
        }
        newLex.error_type = none;
        newLex.type = 0;


        //c = fgetc(fp);
        c = input[inputIndex];
        inputIndex++;

        //change from EOF to end of string
        if(c == '\0'){
            //end the file
            halt = true;
        }else if(isspace(c)){   
            //ignroe this character
            continue;
        }else if(isalpha(c)){
           
            //is an identifier or a reserved word
            newLex.name[0] = c;
            int i = 1;

            //need to buffer in the substring
            int alphaHalt = false;
            while(alphaHalt == false){
                
                //c = fgetc(fp);
                c = input[inputIndex];
                input++;

                if(newLex.error_type != none){
                    //error so we keep reading until symbol or whitespace
                    if(isalpha(c) != false || isdigit(c) != false){
                        //do nothing
                    }else{
                        //ran into symbol or blank character so put it back for the main to handle
                        //ungetc(c,fp);
                        input--;
                        alphaHalt = true;
                    }
                
                }else if(isalpha(c) != false || isdigit(c) != false){
                    //case where our substring hasnt ended
                    if(i == 11){
                        //we have a too long identifier
                        newLex.error_type = too_long_identifier;
                    }else{
                        newLex.name[i] = c;
                        i++;
                    }
                }else{
                    //isnt a letter or digit so must be a symbol or a space
                    
                    //ungetc(c,fp);
                    inputIndex--;

                    //finalize and store token
                    if(strcmp(newLex.name,"else") == 0){
                        newLex.type = elsesym;
                    }else if(strcmp(newLex.name,"write") == 0){
                        newLex.type = writesym;
                    }else if(strcmp(newLex.name,"read") == 0){
                        newLex.type = readsym;
                    }else if(strcmp(newLex.name,"begin") == 0){
                        newLex.type = beginsym;
                    }else if(strcmp(newLex.name,"end") == 0){
                        newLex.type = endsym;
                    }else if(strcmp(newLex.name,"if") == 0){
                        newLex.type = ifsym;
                    }else if(strcmp(newLex.name,"then") == 0){
                        newLex.type = thensym;
                    }else if(strcmp(newLex.name,"while") == 0){
                        newLex.type = whilesym;
                    }else if(strcmp(newLex.name,"do") == 0){
                        newLex.type = dosym;
                    }else if(strcmp(newLex.name,"call") == 0){
                        newLex.type = callsym;
                    }else if(strcmp(newLex.name,"const") == 0){
                        newLex.type = constsym;
                    }else if(strcmp(newLex.name,"var") == 0){
                        newLex.type = varsym;
                    }else if(strcmp(newLex.name,"procedure") == 0){
                        newLex.type = procsym;
                    }else{
                        newLex.type = identsym;
                    }
                    alphaHalt = true;
                }

                


            }


        }else if(isdigit(c)){

            //must be a number
            
            int i = 0;

            newLex.name[i] = c;
            i++;
            
            int numHalt = false;

            //grab substring of numbers
            while(numHalt == false){

                //c = fgetc(fp);
                c = input[inputIndex];
                input++;

                if(newLex.error_type != none){
                    //we ran into an error at some point so read until we get to a new symbol or blankspace
                    if(isalpha(c) == false && isdigit(c) == false){
                        //ungetc(c,fp);
                        input--;

                        numHalt = true;
                    }

                }else if(isdigit(c)){
                    if(i == 4){
                        //this means we already have five numbers in our array
                        

                        newLex.error_type = long_number;

                    }else{
                        newLex.name[i] = c;
                        i++;
                    }
                }else if(isalpha(c)){
                    //is a letter so we run into an error

                    //run through the invalid comment
                   

                    newLex.error_type = no_letter_on_start;
                    
                }else{
                    //must be some kind of symbol or whitespace so we put it back and end this DFA
                    //ungetc(c,fp);
                    inputIndex--;

                    newLex.type = numbersym;

                    sscanf(newLex.name,"%d",&(newLex.value));
                    
                    numHalt = true;

                }
            }
            

        }else{
            //we ran into a symbol
            
            //next = fgetc(fp);
            next = input[inputIndex];
            input++;

            int output = isSpecialSymbol(c,next,&newLex);

            if(output < 0){
                //then error must have occured or we ran into a comment
                if(output == iscomment){

                    int commentHalt = false;

                    while(commentHalt == false){
                        //char temp = fgetc(fp);
                        char temp = input[inputIndex];
                        inputIndex++;

                        if(temp == EOF){
                            newLex.error_type = comment_error;
                            commentHalt = true;
                        }else if(temp == '*'){

                            //temp = fgetc(fp);
                            temp = input[inputIndex];
                            inputIndex++;

                            if(temp == '/'){
                                commentHalt = true;
                            }
                        }

                    }
                }

                
            }else if(newLex.error_type == none){
                //we didnt get an error so we can just tokenize the symbol
                if(output == neqsym || output == leqsym || output == geqsym || output == becomessym){
                    
                }else{
                    //ungetc(next,fp);
                    inputIndex--;
                }

                newLex.type = output;
            }
            }

            //add things to the token table
            tokenTable[tokenCounter] = newLex;
            tokenCounter++;


        }

        
    
    //we can close the filepointer we are no longer reading
    //fclose(fp);

    //now we just print out the token table
    printf("Lexeme Table:\n");
    printf("lexeme \t\ttoken type\n");
    for(int i = 0 ; i < tokenCounter; i++){
        if(tokenTable[i].type == 0 && tokenTable[i].error_type == none){
            continue;
        }else if(tokenTable[i].error_type != none){
            if(tokenTable[i].error_type == no_letter_on_start){
                printf("Lexical Analyzer Error: Invalid Identifier\n");
            }else if(tokenTable[i].error_type == long_number){
                printf("Lexical Analyzer Error: Number Length\n");
            }else if(tokenTable[i].error_type == invalid_symbol){
                printf("Lexical Analyzer Error: Invalid Symbol\n");
            }else if(tokenTable[i].error_type == comment_error){
                printf("Lexical Analyzer Error: Never-ending comment\n");
            }else if(tokenTable[i].error_type == too_long_identifier){
                printf("Lexical Analyzer Error: Identifier Length\n");
            }
        }else{
            printf("%11s\t%d\n", tokenTable[i].name, tokenTable[i].type);
        }
        
    }
    
    int hasError = false;
    //we now print out lex table output
    printf("\nToken List:\n");
        for(int i = 0; i < tokenCounter; i++){

            
            if(tokenTable[i].error_type != none){
                hasError = true;
            }else{
                if(tokenTable[i].type == 0){
                    //this must be some whitespace or EOF that slipped through the cracks

                }else if(tokenTable[i].type == identsym){
                    //print an identifier
                    
                    printf("%d ", tokenTable[i].type);
                    printName(tokenTable[i]);
                    printf(" ");
                }else if(tokenTable[i].type == numbersym){
                    //print a number
                    
                    //printName(tokenTable[i]);
                    printf("%d %d ", tokenTable[i].type,tokenTable[i].value);
                }else{
                    //reserved word or a symbol
                    //printName(tokenTable[i]);
                    printf("%d ",tokenTable[i].type);
                }
                
                
            }
        }

        //free our token table and slap on a whitespace
        //free(tokenTable);   

        printf("\n");
        if(hasError == true){
            return NULL;
        }else{
            return tokenTable;
        }
        
    }

    