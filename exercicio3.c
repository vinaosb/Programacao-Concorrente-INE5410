/*
 * exercicio3.c
 * 
 * Copyright 2017 Vinicius S Berkenbrock <vinicius@ViniNote>
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
#include <semaphore.h>
#include <pthread.h>

#define MAX_THREADS 1000
#define N 1000

sem_t semaphore, prodr, consr, conta;
int contador_global = 0;
int buffer[N];


void *exercicio4() {
	sem_wait(&semaphore);
	
	for(int i = 0; i < 100; i++)
		contador_global++;
		
	sem_post(&semaphore);
	return NULL;
}

void *produ() {
	int valp = 0;
	int i;
	while(1){
		sem_getvalue(&conta,&i);
		if(i < N){
			sem_wait(&prodr);
			valp = (valp+1)%N;
			buffer[valp] = 0;
			sem_post(&conta);
			sem_post(&consr);
			if(valp == N-1)
				printf("%d\n",++contador_global);
		}
	}
}

void *consu() {
	int valp = 0;
	while(1){  //  nao precisa de if para acessar semaphoro contador pois ele so inicializa quando um produtor der um valor
		sem_wait(&consr);
		valp = (valp+1)%N;
		buffer[valp] = contador_global;
		contador_global++;
		sem_post(&prodr);
		sem_wait(&conta);
	}
}


int main(int argc, char **argv) {
	while(1){
		int a;
		printf("Digite o exercício requisitado: \n1 ou 2, para exercicios \n5 para sair \n");  // entrada para qual exercicio quer
		scanf("%d",&a);
		if(a == 2){
			sem_init(&prodr,0,1);
			sem_init(&consr,0,0);
			sem_init(&conta,0,0);
			pthread_t prod;
			pthread_t cons;
			pthread_create(&prod,NULL,produ,NULL);
			pthread_create(&cons,NULL,consu,NULL);
			pthread_join(prod,NULL);
			pthread_join(cons,NULL);
			sem_destroy(&prodr);
			sem_destroy(&consr);
			sem_destroy(&conta);
		}
		if(a == 1){
			sem_init(&semaphore,0,1);
			pthread_t thread[MAX_THREADS];
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_create(&thread[i],NULL,exercicio4,NULL);
			}
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_join(thread[i],NULL);
			}
			sem_destroy(&semaphore);
			printf("Contador: %d\n", contador_global);
		}
	}
	return 0;
}

