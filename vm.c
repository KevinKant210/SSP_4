/*
Authors:
    Caitlin Fabian
    Kevin Kant
virtual machine for p-code isa
Due: 06/05/2022
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#define MAX_PAS_LENGTH 500

struct IR{
    int op;
    int l;
    int m;
}IR;

int base(int L,int BP, int pas[]){
    int arb = BP; // arb = activation record base
    while ( L > 0)     //find base L levels down
    {
        arb = pas[arb];
        L--;
    }

    return arb;
}

void virtual_machine(instruction* code){

    // This is the only pointer used, its simply to communicate with the text file and does not have any impact on the vm
   // FILE *fp;
    
    
    //fp = fopen(argv[1], "r");
    
    int codeIndex = 0;

    int pas[MAX_PAS_LENGTH];
    int arhold[MAX_PAS_LENGTH/5];
    int arcounter = 0;

    for(int i = 0; i< MAX_PAS_LENGTH; i++){
        pas[i] = 0;
    }

    // reading in the input.txt
    int halt = 0;
    int sp = 0;

    while(!halt){
        int op = 0, l = 0, m = 0;

        //fscanf(fp,"%d %d %d", &op, &l, &m);
        op = code[codeIndex].op;
        l = code[codeIndex].l;
        m = code[codeIndex].m;
        codeIndex++;
        
        pas[sp] = op;
        pas[sp+1] = l;
        pas[sp+2] = m;
        sp += 3;
        
        //change from op == 9 && m ==3 to op == -1
        if(op == -1){
            halt = 1;
            sp -= 1;
        }
    }

    //closing out the file stream
    //fclose(fp);

    //generating out initial vars
    struct IR ir;
    int bp = sp+1;
    int start = bp;
    int pc = 0;

    printf("\t\tPC  BP  SP  stack\n");
    printf("intial values: \t%-3d %-3d %-3d\n", pc, bp, sp);

    //entering the fetch execute cycle
    while(halt){
        ir.op = pas[pc];
        ir.l = pas[pc+1];
        ir.m = pas[pc+2];
        pc = pc+3;
        
        char opCode[4];

        switch(ir.op){
            // LIT
            case 1: 
                sp = sp+1;
                pas[sp] = ir.m; 
                strcpy(opCode,"LIT");
                break;
            // OPR
            case 2: {
                switch(ir.m){
                    //RTN
                    case 0:{
                        strcpy(opCode,"RTN");
                        sp = bp -1;
                        bp = pas[sp+2];
                        pc = pas[sp+3];
                        break;
                    }
                    // NEG
                    case 1:{
                        strcpy(opCode,"NEG");
                        sp = sp-1;
                        pas[sp] = -1*pas[sp];
                        break;
                    }
                    // ADD
                    case 2:{
                        strcpy(opCode,"ADD");
                        sp = sp-1;
                        pas[sp] = pas[sp] + pas[sp+1];
                        break;
                    }
                    // SUB
                    case 3 :{
                        strcpy(opCode,"SUB");
                        sp = sp-1;
                        pas[sp] = pas[sp] - pas[sp+1];
                        break;
                    }
                    // MUL
                    case 4 :{
                        strcpy(opCode,"MUL");
                        sp = sp-1;
                        pas[sp] = pas[sp] * pas[sp+1];
                        break;
                    }
                    // DIV
                    case 5 :{
                        strcpy(opCode,"DIV");
                        sp = sp-1;
                        pas[sp] = pas[sp] / pas[sp+1];
                        break;
                    }
                    // MOD
                    case 6 :{
                        strcpy(opCode,"MOD");
                        sp = sp-1;
                        pas[sp] = pas[sp] % pas[sp+1];
                        break;
                    }
                    // EQL
                    case 7 :{
                        strcpy(opCode,"EQL");
                        sp = sp-1;
                        pas[sp] = pas[sp] == pas[sp+1];
                        break;
                    }
                    // NEQ
                    case 8 :{
                        strcpy(opCode,"NEQ");
                        sp = sp-1;
                        pas[sp] = pas[sp] != pas[sp+1];
                        break;
                    }
                    // LSS
                    case 9 :{
                        strcpy(opCode,"LSS");
                        sp = sp-1;
                        pas[sp] = pas[sp] < pas[sp+1];
                        break;
                    }
                    //LEQ
                    case 10 :{
                        strcpy(opCode,"LEQ");
                        sp = sp-1;
                        pas[sp] = pas[sp] <= pas[sp+1];
                        break;
                    }
                    // GTR
                    case 11 :{
                        strcpy(opCode,"GTR");
                        sp = sp-1;
                        pas[sp] = pas[sp] > pas[sp+1];
                        break;
                    }
                    // GEQ
                    case 12 :{
                        strcpy(opCode,"GEQ");
                        sp = sp-1;
                        pas[sp] = pas[sp] >= pas[sp+1];
                        break;
                    }
                }
                break;
            }

            // LOD
            case 3: {
                strcpy(opCode,"LOD");
                sp = sp+1;
                pas[sp] = pas[base(ir.l,bp,pas)+ir.m];
                break;
            }
            // STO
            case 4: {
                strcpy(opCode,"STO");
                pas[base(ir.l,bp,pas)+ir.m]= pas[sp];
                sp -= 1;
                break;
            }
            // CAL
            case 5: {
                strcpy(opCode,"CAL");

                
                pas[sp+1] = base(ir.l,bp,pas);
                pas[sp+2] = bp;
                pas[sp+3] = pc;
                bp = sp+1;
                pc = ir.m;
                arhold[arcounter] = bp;
                arcounter++;
                break;
            }
            // INC
            case 6: {
                strcpy(opCode,"INC");
                sp = sp + ir.m;
                break;
            }
            // JMP
            case 7: {
                strcpy(opCode,"JMP");
                pc = ir.m;
                break;
            }
            // JPC
            case 8: {
                strcpy(opCode,"JPC");
                if(pas[sp] == 0)
                    pc = ir.m;
                sp = sp - 1;
                break;
            }
            // SYS
            case 9: {
                strcpy(opCode,"SYS");
                switch(ir.m){
                    case 1: {
                        printf("Output result is: %d\n", pas[sp]);
                        sp = sp -1;
                        break;
                    }
                    case 2:{
                         sp = sp+1;
                         
                         printf("Please Enter an Integer: ");
                         scanf(" %d", &pas[sp]);
                         
                        
                         break;
                    }
                    case 3:{
                        halt = 0;
                        break;
                    }
                }
                break;
            }

        }
        //printing out op, l , m , pc , b p , and sp
        printf("  %s  %-2d %-2d\t%-2d  %-2d  %-2d\t", opCode, ir.l, ir.m, pc, bp, sp );
        int jp = 0;

        //printing out the stack trace
        for(int i = start; i <= sp; i++){
            //the logic for controlling where the | goes for activation records
            char c = '|';

            
            for(int j = 0 ; j < arcounter; j++){    
                if(i == arhold[j]){
                    if((arhold[j] - jp >= 5) || jp == 0){
                        jp = arhold[j];
                        
                        printf("|");
                    }
                    
                    
                    
                }
            }
            
            printf("%-3d", pas[i]);
        }

        
        printf("\n");
    }
    
}


