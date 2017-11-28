/* code-c - Student's code for mmcpu

   Copyright 2017  Monaco F. J.   <monaco@icmc.usp.br>

   This file is part of Muticlycle Mips CPU (MMCPU)

   MMCPU is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


/* After you have implemented your functions, you may safely remove these lines. */
#include <stdio.h>
#include <stdlib.h>
#include "mask.h"		/* Useful masks; customize at you please. */
#include "cpu.h"
#define not_implemented() fprintf(stderr, "Not implemented\n"); exit(EXIT_FAILURE)

/* Students, you are required to implemented the functions bellow.
   Please, refere to cpu.h for further information. */
char zero, overflow;

int alu (int a, int b, char alu_op, int *result_alu, char *zero, char *overflow)
{

  switch (alu_op) {
    case ativa_soma:
      *result_alu = a + b;
      break;
    case ativa_subtracao:
      *result_alu = a - b;
      break;
    case ativa_and:
      *result_alu = a & b;
      break;
    case ativa_or:
      *result_alu = a | b;
      break;
    case ativa_slt:
      if(a < b) *result_alu = 1;
      else *result_alu = 0;
      break;
  }

  if(*result_alu == 0) *zero = ativa_bit_zero;
  else *zero = desativa_bit_zero;

  return 0;
}

void control_unit(int IR, short int *sc)
{
  // Aqui temos o primeiro estado.
  if(IR == -1) *sc = 0b1001010000001000; //vai para o estado 0
  else{
    char operation = IR & separa_cop;
    operation >>= 26;

    if(*sc == (short int)0b1001010000001000){ //se estado 0, vai para o 1
      *sc = 0b0000000000011000;
    }
    else{
      switch(operation){
        case ((short int)0x00): //TIPO - R;
        printf("Tipo R");
          switch(*sc){
            case((short int)0b0000000000011000): //se estado 1, vai para o 6
              *sc = 0b0000000001000100;
              break;
            case((short int)0b0000000001000100): //se estado 6, vai para o 7
              *sc = 0b0000000000000011;
              break;
            case((short int)0b0000000000000011): //se estado 7, vai para o 0
              *sc = 0b1001010000001000;
              break;
          }
          break;
        case ((short int) 0x23): //LW
        printf("Tipo LW");
          switch(*sc){
            case((short int)0b0000000000011000): //se estado 1, vai para o 2
              *sc = 0b0000000000010100;
              break;
            case((short int)0b0000000000010100): //se estado 2, vai para o 3
              *sc = 0b0001100000000000;
              break;
            case((short int)0b0001100000000000): //se estado 3, vai para o 4
              *sc = 0b0100000000000010;
              break;
            case((short int)0b0100000000000010): //se estado 4, vai para o 0
              *sc = 0b1001010000001000;
              break;
          }
          break;
          case ((short int)0x2b): //SW
          printf("Tipo SW");
            switch(*sc){
              case((short int)0b0000000000011000): //se estado 1, vai para o 2
                *sc = 0b0000000000010100;
                break;
              case((short int)0b0000000000010100): //se estado 2, vai para o 5
                *sc = 0b0000100000000010;
                break;
              case((short int)0b0000100000000010): //se estado 5, vai para o 0
                *sc = 0b1001010000001000;
                break;
            }
            break;
          case ((short int)0x04): //BEQ
          printf("Tipo BEQ");
            switch(*sc){
              case((short int)0b0000000000011000): //se estado 1, vai para o 8
                *sc = 0b0000001010100100;
                break;
              case((short int)0b0000001010100100): //se estado 8, vai para o 0
                *sc = 0b1001010000001000;
                break;
            }
            break;
          case ((short int)0x02): //Jump
          printf("Tipo Jump");
            switch(*sc){
              case((short int)0b0000000000011000): //se estado 1, vai para o 9
                *sc = 0b0000010010000000;
                break;
              case((short int)0b0000010010000000): //se estado 9, vai para o 0
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
  if(sc == ((short int)0b1001010000001000)){
    *IRnew = memory[PC];
    alu(PC, 1, (char)ativa_soma, &ALUOUT, &zero, &overflow);
    *PCnew = ALUOUT;
    *MDRnew = memory[PC];
    if(*IRnew == 0) {
      loop = 0;
      printf("Cheguei aqui!\n");
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
  if(sc ==(short int) 0b0000000000011000){
    *Anew = reg[(separa_rs & IR)>>21];
    *Bnew = reg[(separa_rt & IR)>>16];
    //*ALUOUTnew = PC + (IR & separa_imediato) << 2;
    int aux;
    if((IR & separa_imediato) & 0x00008000) {
      aux = (IR & separa_imediato) | 0xffff0000;
    }
    alu(PC, aux << 2, (char)ativa_soma, ALUOUTnew, &zero, &overflow);
  }
}

void exec_calc_end_branch(short int sc,
                          int A,
                          int B,
                          int IR,
                          int PC,
                          int ALUOUT,
                          int *ALUOUTnew,
                          int *PCnew)
{
  /* Separa apenas os 6 bits mais significativos e depois joga como eles como
  os menos significativos */
  char operation = IR & separa_cop;
  operation >>= 26;

  //Switch para decidir qual instrucao.
  switch (operation) {
    case ((short int)0x00): //TIPO - R;
      if(sc ==(short int) 0b0000000001000100)
      alu(A, B, (char)(IR & separa_cfuncao), ALUOUTnew, &zero, &overflow);
      break;
    case ((short int)0x23): //LW
      if(sc == (short int)0b0000000000010100)
      alu(A, (IR & separa_imediato), (char)ativa_soma, ALUOUTnew, &zero, &overflow);
      break;
    case ((short int)0x2b): //SW
      if(sc == (short int) 0b0000000000010100)
      alu(A, (IR & separa_imediato), (char)ativa_soma, ALUOUTnew, &zero, &overflow);
      break;
    case ((short int)0x04): //BEQ
      if(sc == (short int)0b0000001010100100){
        alu(A, B, (char)ativa_subtracao, ALUOUTnew, &zero, &overflow);
        if(zero) *PCnew = *ALUOUTnew;
      }
      break;
    case ((short int)0x02): //Jump
      if(sc == (short int) 0b0000010010000000)
      alu(PC & separa_4bits_PC, (char)(IR & separa_endereco_jump) << 2, ativa_or, PCnew, &zero, &overflow);
      break;
  }
}

void write_r_access_memory(short int sc,
                           int IR,
                           int MDR,
                           int ALUOUT,
                           int PC,
                           int *MDRnew,
                           int *IRnew)
{
  char operation = IR & separa_cop;
  operation >>= 26;

  //Switch para decidir qual instrucao.
  switch (operation) {
    case ((short int)0x00): //TIPO - R;
      if(sc == (short int)0b0000000000000011)
      reg[(separa_rd & IR)>>11]  = ALUOUT;
      break;
    case ((short int)0x23): //LW
      if(sc == (short int)0b0001100000000000)
      *MDRnew = memory[ALUOUT];
      break;
    case ((short int)0x2b): //SW
      if(sc == (short int)0b0000100000000010)
      memory[ALUOUT] = reg[(separa_rt & IR)>>16];
      break;
    }
}


void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
  char operation = IR & separa_cop;
  operation >>= 26;
    if(operation ==  ((short int)0x23)) { //LW
      if(sc == (short int) 0b01000000000000010)
        reg[(IR & separa_rt)>>16] = MDR;
    }
}
