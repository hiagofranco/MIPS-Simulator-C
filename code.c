#include <stdio.h>
#include <stdlib.h>
#include "mask.h"
#include "cpu.h"

//variaveis globais
char zero;
char overflow;

int alu( int a, 
          int b, 
          char alu_op, 
          int *result_alu, 
          char *zero, 
          char *overflow)
{
    switch(alu_op)
    {
        case ativa_soma:
            *result_alu = a + b;
            break;
        case ativa_subtracao:
            *result_alu = a - b;
            break;
        case ativa_or:
            *result_alu = a | b;
            break;
        case ativa_and:
            *result_alu = a & b;
            break;
        case ativa_slt:
            if(a<b){
                *result_alu = 1;
            }
            else{
                *result_alu = 0;
            }
            break;
    }
    if(*result_alu == 0) *zero = ativa_bit_zero;
    else *zero = desativa_bit_zero;

    return 0;
}

char controle_alu(int IR, int sc)
{
    switch(((sc & separa_ALUOp0) | (sc & separa_ALUOp1)) >> 5){
        //LW e SW
        case 0x0:
            return 0b0010;
            break;
        //Branch
        case 0x1:
            return 0b0110;
            break;
        //Tipo-R
        case 0x2:
            //Tipo-R depende de function
            //Vamos realizar as diferentes combinações de function
            //function são os ultimos 4 bits de IR
            switch(IR & 0b1111){
                //ADD
                case 0b0000:
                    return 0b0010;
                    break;

                //SUB
                case 0b0010:
                    return 0b0110;
                    break;

                //AND
                case 0b0100:
                    return 0b0000;
                    break;

                //OR
                case 0b0101:
                    return 0b0001;
                    break;

                //SLT
                case 0b1010:
                    return 0b0111;
                    break;

                default:
                    break;
            }
            break;
        //Tipo-R
        case 0x3:
            //Tipo-R depende de function
            //Vamos realizar as diferentes combinações de function
            switch(IR & 0b1111){
                //SUB
                case 0b0010:
                    return 0b0110;  //retorn o alu_op responsavel pela SUB
                    break;

                //SLT
                case 0b1010:
                    return 0b0111;  //retorn o alu_op responsavel pela SLT
                    break;

                default:
                    break;
            }
            break;
    }
}

void control_unit(int IR, short int *sc)
{
    //Aqui temos o primeiro estado
    if(IR == -1) *sc = 0b1001010000001000;
    else{
        char operation = (IR & separa_cop) >> 26;

        if(*sc == ((short int) 0b1001010000001000)){ //se estado 0, vai para o 1
            *sc = 0b0000000000011000; 
        }
        else{
            switch(operation){
                case ((short int) 0x00):
                    //TIPO R
                    switch(*sc)
                    {
                        case ((short int)0b0000000000011000): //se estado 1, vai para o 6
                            *sc = 0b0000000001000100;
                            break;
                        case ((short int)0b0000000001000100): //se estado 6, vai para o 7
                            *sc = 0b0000000000000011;
                            break;
                        case ((short int)0b0000000000000011): //se estado 7, vai para o 0
                            *sc = 0b1001010000001000;
                            break;
                    }
                    break;
                case ((short int) 0x23):
                    //LW
                    switch(*sc)
                    {
                        case ((short int) 0b0000000000011000): //se estado 1, vai para o 2
                            *sc = 0b0000000000010100;
                            break;
                        case ((short int) 0b0000000000010100): //se estado 2, vai para o 3
                            *sc = 0b0001100000000000;
                            break;
                        case ((short int) 0b0001100000000000): //se estado 3, vai para o 4
                            *sc = 0b0100000000000010;
                            break;
                        case ((short int) 0b0100000000000010): //se estado 4, vai para o 0
                            *sc = 0b1001010000001000;
                            break;
                    }
                    break;
                case ((short int) 0x2b):
                    //SW
                    switch(*sc)
                    {
                        case ((short int) 0b0000000000011000): //se estado 1, vai para o 2
                            *sc = 0b0000000000010100;
                            break;
                        case ((short int) 0b0000000000010100): //se estado 2, vai para o 5
                            *sc = 0b0000100000000010;
                            break;
                        case ((short int) 0b0000100000000010): //se estado 5, vai para o 0
                            *sc = 0b1001010000001000; 
                            break;
                    }
                    break;
                case ((short int) 0x04):
                    //BEQ
                    switch(*sc)
                    {
                        case ((short int) 0b0000000000011000): //se estado 1, vai para o 8
                            *sc = 0b0000001010100100;
                            break;
                        case ((short int) 0b0000001010100100): //se estado 8, vai para o 0
                            *sc = 0b1001010000001000;
                            break;
                    }
                    break;
                case ((short int) 0x02):
                    //JUMP
                    switch(*sc)
                    {
                        case ((short int) 0b0000000000011000): //se estado 1, vai para o 9
                            *sc = 0b0000010010000000;
                            break;
                        case ((short int) 0b0000010010000000): //se estado 9, vai para o 0
                            *sc = 0b1001010000001000;
                            break;
                    }
                    break;
            }
        }
    }
}


void instruction_fetch(short int sc, 
                        int PC, 
                        int ALUOUT, 
                        int IR, 
                        int* PCnew, 
                        int* IRnew, 
                        int* MDRnew)
{
    if(sc == ((short int) 0b1001010000001000))
    {
        *IRnew = memory[PC];

        char aluop = controle_alu(IR, sc);

        alu(PC, 1, aluop, &ALUOUT, &zero, &overflow);
        *PCnew = ALUOUT;
        *MDRnew = memory[PC];
        if(*IRnew == 0)
        {
          loop = 0;
        }
    }
}


void decode_register(short int sc,
                      int IR, 
                      int PC, 
                      int A, 
                      int B, 
                      int *Anew, 
                      int *Bnew, 
                      int *ALUOUTnew)
{
    if(sc == ((short int) 0b0000000000011000))
    {
        *Anew = reg[(separa_rs & IR) >> 21]; 
        *Bnew = reg[(separa_rt & IR) >> 16]; 

        char aluop = controle_alu(IR, sc);

        alu(PC, (separa_imediato & IR), aluop, ALUOUTnew, &zero, &overflow);
    }
}


void exec_calc_end_branch(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
    char aluop = controle_alu(IR, sc);

    if (sc == (short int) 0b0000000000010100)
    {
      alu(A, IR & separa_imediato, aluop, ALUOUTnew, &zero, &overflow);
    }
    if(sc == (short int) 0b0000000001000100)
    {
      alu(A, B, aluop, ALUOUTnew, &zero, &overflow);
    }
    if(sc == (short int) 0b0000001010100100)
    {
      alu(A, B, aluop, ALUOUTnew, &zero, &overflow);
      if(zero == 1)
      {
          *PCnew = ALUOUT;
      }
    }
    if(sc == (short int) 0b0000010010000000)
    {
      alu((PC & separa_4bits_PC),(IR & separa_endereco_jump),ativa_or,PCnew,&zero,&overflow);
    }
}


void write_r_access_memory(short int sc, int B, int IR, int ALUOUT, int PC, int *MDRnew, int *IRnew)
{
  if(sc == (short int) 0b0001100000000000)
    {
         *MDRnew = memory[ALUOUT];
    }
    if (sc == (short int) 0b0000100000000010)
    {
        memory[ALUOUT] = reg[(IR & separa_rt) >> 16];
    }
    if(sc == (short int) 0b0000000000000011)
    {
        reg[(IR & separa_rd) >> 11] = ALUOUT;
    }
}

void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT){
    if(sc == ((short int) 0b0100000000000010))
    {
        reg[(IR & separa_rt) >> 16] = MDR;
    }
}
