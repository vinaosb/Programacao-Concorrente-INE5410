/*
 * exercicio1-3.c
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


#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>



void func(int idpai){  // funcao para o exercicio 3
	int temp = fork();
	if(temp != 0){
		fork();
	}
	if(getpid() != idpai){
		int temp2 = fork();
		if (temp2 != 0)
			fork();
	}
}

int main(int argc, char **argv)
{
	int a;
	int idpai = getpid();
	printf("Digite o exercício requisitado (1,2 ou 3) \n");  // entrada para qual exercicio quer
	scanf("%d",&a);
	
	if(a == 1){  // exercicio 1
		fork();
		puts("Novo processo criado");
	}else
	
	if(a == 2){  // exercicio 2
		for(int i = 0; i < 4; i++){
			if(getpid() == idpai){
				int temp = fork();
				if(temp != 0)
					printf("Processo pai %d criou %d \n",idpai,temp);
			}
		}
	}else
	
	if(a == 3){  // exercicio 3
		func(idpai);  // chama funcao para ficar mais limpo
		if(getpid() != idpai)
			printf("Processo %d filho de %d \n",getpid(),getppid());
	}
	
	int c;
	scanf("%d",&c);
	return 0;
}

