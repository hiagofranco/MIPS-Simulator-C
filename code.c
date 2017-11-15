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
#define not_implemented() fprintf(stderr, "Not implemented\n"); exit(EXIT_FAILURE)

/* Students, you are required to implemented the functions bellow.
   Please, refere to cpu.h for further information. */

int alu (int a, int b, char alu_op, int *result_alu, char *zero, char *overflow)
{
  /* Operacao usada para separa os 4 ultimos bits de alu_op */
  char C_ULA = alu_op & separa_controle_ula;

  switch (C_ULA) {
    case ativa_soma:
      *result_alu = a + b;
      break;
    case ativa_subtracao:
      *result_ula = a - b;
      break;
    case ativa_and:
      *result_ula = a & b;
      break;
    case ativa_or:
      *result_ula = a | b;
      break;
    case ativa_slt:
      if(a < b) *result_ula = 1;
      else *result_ula = 0;
      break;
  }

  if(*result_ula == 0) *zero = 1;
  return 0;
}

void control_unit(int IR, short int *sc)
{

}

void instruction_fetch(short int sc,
                       int PC,
                       int ALUOUT,
                       int IR,
                       int* PCnew,
                       int* IRnew,
                       int* MDRnew)
{
  char zero, overflow;
  control_unit(IR, &sc);
  *IRnew = memory[PC];
  alu(PC, 4, ativa_soma, &ALUOUT, &zero, &overflow);
  *PCnew = ALUOUT;
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
  char zero, overflow;
  control_unit(IR, &sc);
  *Anew = reg[separa_rs & IR];
  *Bnew = reg[separa_rt & IR];
  //*ALUOUTnew = PC + (IR & separa_imediato) << 2;
  if((IR & separa_imediato) & 0x00008000) {
    int aux = (IR & separa_imediato) | 0xffff0000;
  }
  alu(PC, aux << 2, ativa_soma, ALUOUTnew, &zero, &overflow);
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
  char zero, overflow;
  int operation = IR & separa_cop;
  operation = operation >> 26;

  //Switch para decidir qual instrucao.
  /*Nintendo*/switch (operation) {
    case 0x0: //TIPO - R;
      alu(A, B, (IR & separa_cfuncao), ALUOUTnew, &zero, &overflow);
      break;
    case 0x23: //LW
      alu(A, (IR & separa_imediato), ativa_soma, ALUOUTnew, &zero, &overflow);
      break;
    case 0x2b: //SW
      alu(A, (IR & separa_imediato), ativa_soma, ALUOUTnew, &zero, &overflow);
      break;
    case 0x4: //BEQ
      if(A == B) *PCnew = *ALUOUTnew;
      break;
    case 0x2: //Jump
      alu(PC & separa_4bits_PC, (IR & separa_endereco_jump) << 2, ativa_or, PCNew, &zero, &overflow);
      break;
  }
}

void write_r_access_memory(short int sc,
                           int IR,
                           int MDR,
                           int AMUOUT,
                           int PC,
                           int *MDRnew,
                           int *IRnew)
{
  int operation = IR & separa_cop;
  operation = operation >> 26;

  //Switch para decidir qual instrucao.
  /*Nintendo*/switch (operation) {
    case 0x0: //TIPO - R;
      reg[separa_rd & IR] = AMUOUT;
      break;
    case 0x23: //LW
      MDRnew = memory[AMUOUT];
      break;
    case 0x2b: //SW
      memory[AMUOUT] = reg[separa_rt & IR];
      break;
}


void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
  int operation = IR & separa_cop;
  operation = operation >> 26;
    if(operation ==  0x23){ //LW
    reg[IR & separa_rt] = MDR;
    }
}
