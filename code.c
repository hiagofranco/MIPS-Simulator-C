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
    case ativa_nor:
      *result_ula = ~(a | b);
      break;
    default:
      printf("Comando invalido!\n");
      break;
  }

  if(*result_ula == 0) *zero = 1;
  return 0;
}

void control_unit(int IR, short int *sc)
{

}


void instruction_fetch(short int sc, int PC, int ALUOUT, int IR, int* PCnew, int* IRnew, int* MDRnew)
{
  IR = memory[PC];
  PC += 4;
  control_unit(IR, &sc);

}


void decode_register(short int sc, int IR, int PC, int A, int B, int *Anew, int *Bnew, int *ALUOUTnew)
{
}


void exec_calc_end_branch(short int sc, int A, int B, int IR, int PC, int ALUOUT, int *ALUOUTnew, int *PCnew)
{
}


void write_r_access_memory(short int sc, int IR, int MDR, int AMUOUT, int PC, int *MDRnew, int *IRnew)
{
}


void write_ref_mem(short int sc, int IR, int MDR, int ALUOUT)
{
}
