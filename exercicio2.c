/*
 * exercicio2.c
 * 
 * Copyright 2017 Vinicius S Berkenbrock e Valdivino Morais
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
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>

#define MAX_THREADS 512


int contador_global = 0;
pthread_mutex_t lock;

void *exercicio1e2(){
	pthread_t tid = pthread_self();
	printf("Nova thread criada. TID = %d!\n",(unsigned int)tid);
			pthread_exit(NULL);
}

void *exercicio3(){
	for(int i = 0; i < 100; i++)
		contador_global++;
			pthread_exit(NULL);
}

void *exercicio4(){
	pthread_mutex_lock(&lock);
	
	for(int i = 0; i < 100; i++)
		contador_global++;
		
	pthread_mutex_unlock(&lock);
			pthread_exit(NULL);
}

int main(int argc, char **argv)
{
	int a = 0;
	while(1){
		printf("Digite o exercício requisitado: \n1,2, 3 ou 4 para exercicios \n5 para sair \n");  // entrada para qual exercicio quer
		scanf("%d",&a);
		if(a == 1){
			pthread_t thread;
			pthread_create(&thread,NULL,exercicio1e2,NULL);
			pthread_join(thread,NULL);
		}
		if(a == 2){
			pthread_t thread[MAX_THREADS];
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_create(&thread[i],NULL,exercicio1e2,NULL);
			}
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_join(thread[i],NULL);
			}
		}
		if(a == 3){
			pthread_t thread[MAX_THREADS];
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_create(&thread[i],NULL,exercicio3,NULL);
			}
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_join(thread[i],NULL);
			}
			printf("Contador: %d\n", contador_global);
		}
		if(a == 4){
			pthread_t thread[MAX_THREADS];
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_create(&thread[i],NULL,exercicio4,NULL);
			}
			for(int i = 0; i<MAX_THREADS; i++){
				pthread_join(thread[i],NULL);
			}
			pthread_mutex_destroy(&lock);
			printf("Contador: %d\n", contador_global);
		}
		if(a == 5){
			return 0;
		}
	}
}

