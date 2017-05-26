/*
 * exercicio4.c
 * 
 * Copyright 2017 Vinicius S Berkenbrock
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define SIZE 50000

#define NRA 8                 /* number of rows in matrix A */
#define NCA 8                 /* number of columns in matrix A */
#define NCB 8                 /* number of columns in matrix B */


void mm () {

  int i, j, k;

  /* matrix A to be multiplied */
  double **a = (double **) malloc(sizeof(double *) * NRA);
  /* matrix B to be multiplied */
  double **b = (double **) malloc(sizeof(double *) * NCA);
  /* result matrix C */
  double **c = (double **) malloc(sizeof(double *) * NRA);
  
	#pragma omp parallel private (i,j,k) shared (a,b,c) 
	{
		#pragma omp for
			for (i = 0; i < NRA; i ++)
				a[i] = (double *) malloc(sizeof(double) * NCA);
		#pragma omp for
			for (i = 0; i < NCA; i ++)
				b[i] = (double *) malloc(sizeof(double) * NCB);
		#pragma omp for
			for (i = 0; i < NRA; i ++)
				c[i] = (double *) malloc(sizeof(double) * NCB);

  //printf("Initializing matrices...\n");

	#pragma omp barrier
  /*** Initialize matrices ***/
	#pragma omp for
		for (i=0; i<NRA; i++)
			for (j=0; j<NCA; j++)
				a[i][j]= i+j;
	#pragma omp for
		for (i=0; i<NCA; i++)
			for (j=0; j<NCB; j++)
				b[i][j]= i*j;
	#pragma omp for
		for (i=0; i<NCA; i++)
			for (j=0; j<NCB; j++)
				c[i][j]= 0;
	
	#pragma omp barrier
  /*** Do the matrix-matrix multiplication ***/
	#pragma omp for
		for (i=0; i<NRA; i++)
			for(j=0; j<NCB; j++)
				for (k=0; k<NCA; k++)
					c[i][j] += a[i][k] * b[k][j];

	#pragma omp barrier
  /*** Print results ***/
  //printf("******************************************************\n");
  //printf("Result Matrix:\n");
  /*for (i=0; i<NRA; i++)
  {
    for (j=0; j<NCB; j++)
      printf("%10.2f  ", c[i][j]);
    printf("\n");
  }
  printf("******************************************************\n");
  printf ("Done.\n");
  */

	#pragma omp sections
	{
	#pragma omp section 
	{
		for (i = 0; i < NRA; i ++)
			free(a[i]);
		free(a);
	}

	#pragma omp section 
	{
		for (i = 0; i < NCA; i ++)
			free(b[i]);
		free(b);
	}

	#pragma omp section 
	{
		for (i = 0; i < NRA; i ++)
			free(c[i]);
		free(c);
	}
	}
}
}

void calculo() {
	int i;

	double *c = (double *) malloc (sizeof(double) * SIZE);
	#pragma omp parallel
	{
	#pragma omp for
	for (i = 0; i < SIZE; i++) {
		c[i] = sqrt(i * 32) + sqrt(i * 16 + i * 8) + sqrt(i * 4 + i * 2 + i);
		c[i] -= sqrt(i * 32 * i * 16 + i * 4 + i * 2 + i);
		c[i] += pow(i * 32, 8) + pow(i * 16, 12);
	}
	}

  free(c);

}


int main(int argc, char *argv[]) {
	int a = 2;
	while(1){
		//printf("Digite o exercício requisitado: \n1 2 para exercicios \n3 para sair \n");  // entrada para qual exercicio quer
		//scanf("%d",&a);
		if(a == 1){
			calculo();
			break;
		}
		if(a == 2) {
			mm();
			break;
		}
		if (a == 3) {
			break;
		}
	}
	
  return 0;
}
