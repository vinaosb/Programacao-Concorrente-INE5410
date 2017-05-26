/*
* Universidade Federal de Santa Catarina
*
* Professor: Christian Koliver
* Alunos: 	Vinícius Schwinden Berkenbrock		16100751
* 			Valdivino Morais da Silva Junior	16100749
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <sys/sysinfo.h>
typedef unsigned char cell_t;
int MAX_THREADS = 1;
int threadsLinha, threadsColuna;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//struct para passar parametros para thread abaixo
typedef struct {
	cell_t ** board;
	int size;
	int k;
} line;
//thread para alocar linhas do tabuleiro
void * allocate_line (void * args){
	line *arg = args;
	int	i;
	
	int temp = arg->size%MAX_THREADS;
	
	int threadLines = arg->size/MAX_THREADS;
	
	for (i=arg->k*threadLines; i<(arg->k+1)*threadLines + temp; i++){
		arg->board[i] = (cell_t *) malloc(sizeof(cell_t)*arg->size);
	}
	pthread_exit(NULL);
}
//alocador de tabuleiro
cell_t ** allocate_board (int size) {
	cell_t ** board = (cell_t **) malloc(sizeof(cell_t*)*size);
	pthread_t thread[MAX_THREADS];
	line temp[MAX_THREADS];
	int k;
	for (k = 0; k < MAX_THREADS; k++){
		temp[k].board = board;
		temp[k].size = size;
		temp[k].k = k;
		pthread_create(&thread[k], NULL, allocate_line, (void*) &temp[k]);
	}
	for (k = 0; k < MAX_THREADS; k++)
			pthread_join(thread[k],NULL);
	return board;
}

//desalocador de tabuleiro (original)
void free_board (cell_t ** board, int size) {
	int     i;
	for (i=0; i<size; i++)
		free(board[i]);
	free(board);
}

//printador de tabuleiro (original)
void print (cell_t ** board, int size) {
	int	i, j;
	/* for each row */
	for (j=0; j<size; j++) {
		/* print each column position... */
		for (i=0; i<size; i++)
			printf ("%c", board[i][j] ? 'x' : ' ');
		/* followed by a carriage return */
		printf ("\n");
	}
}

//struct para passar de parametro para thread abaixo
typedef struct {
	cell_t ** board;
	int size;
	char ** s;
	int k;
	int l;
} lineread;
//thread para copiar linhas e colunas da string para o tabuleiro
void * read_line (void * args) {
	lineread *arg = args;
	int	i, j;
	
	int temp = arg->size%threadsLinha;
	int temp2 = arg->size%threadsColuna;
	
	int threadLines = arg->size/threadsLinha;
	int threadColumns = arg->size/threadsColuna;
	
	for (i=arg->k*threadLines; i<(arg->k+1)*threadLines + temp; i++){
		for (j=arg->l*threadColumns; j<(arg->l+1)*threadColumns + temp2; j++) {
			arg->board[i][j] = arg->s[i][j] == 'x';
		}
	}
	pthread_exit(NULL);
}
//funcao para passar a string para o tabuleiro
void string_to_board (cell_t ** board, int size, char ** s) {
	pthread_t thread[MAX_THREADS];
	lineread temp[MAX_THREADS];
	int k, l, t = 0;
	for (k = 0; k < threadsLinha; k++){
		for (l = 0; l < threadsColuna; l++) {
			temp[t].s = s;
			temp[t].board = board;
			temp[t].size = size;
			temp[t].k = k;
			temp[t].l = l;
			pthread_create(&thread[t], NULL, read_line, (void*) &temp[t]);
			t++;
		}
	}
	for (k = 0; k < MAX_THREADS; k++)
			pthread_join(thread[k],NULL);
}
//funcao para passar do arquivo para a string (apenas para facilitar visao)
void file_to_string (FILE * f, char ** s, int size) {
	int j;
	/* read the first new line (it will be ignored) */
	fgets (s[0], size+10,f);

	/* read the life board */
	for (j=0; j<size; j++) {
		/* get a string */
		fgets (s[j], size+10,f);
		/* copy the string to the life board */
	}
}
//le arquivo para o tabuleiro
void read_file (FILE * f, cell_t ** board, int size) {
	int	j;
	char *s[size];
	for (j = 0; j < size; j++)
		s[j]= (char*) malloc(size+10);

	file_to_string(f,s,size);
	string_to_board(board,size,s);
}

//seta o tabuleiro novo apois contar numero de celulas adjacentes a celula i,j do tabuleiro antigo
void adjacent_to (cell_t ** board, cell_t ** newboard, int size, int i, int j) {
	int	k, l, count=0;

	int sk = (i>0) ? i-1 : i;
	int ek = (i+1 < size) ? i+1 : i;
	int sl = (j>0) ? j-1 : j;
	int el = (j+1 < size) ? j+1 : j;

	if ((i>0)&&(i+1 < size)&&(j > 0)&&(j+1 < size)) {
		count+= board[i-1][j-1];
		count+= board[i-1][j]; 
		count+= board[i-1][j+1]; 
		count+= board[i][j-1];
		count+= board[i][j+1];
		count+= board[i+1][j-1];
		count+= board[i+1][j];
		count+= board[i+1][j+1];
	} else {
  		for (k=sk; k<=ek; k++)
  			for (l=sl; l<=el; l++)
  				count+=board[k][l];
  		count-=board[i][j];
	}
	
	if (count == 2) newboard[i][j] = board[i][j];
	if (count == 3) newboard[i][j] = 1;
	if (count < 2) newboard[i][j] = 0;
	if (count > 3) newboard[i][j] = 0;
}
//struct para passar de parametro para thread abaixo
typedef struct {
	cell_t ** board;
	cell_t ** newboard;
	int size;
	int k;
	int l;
} boards;
//thread para chamar a funcao adjacent_to
void * player (void* args) {
	boards *arg = args;
	int	i, j;
	
	int temp = arg->size%threadsLinha;
	int temp2 = arg->size%threadsColuna;
	
	int threadLines = arg->size/threadsLinha;
	int threadColumns = arg->size/threadsColuna;
	
	for (i=arg->k*threadLines; i<(arg->k+1)*threadLines + temp; i++)
		for (j=arg->l*threadColumns; j<(arg->l+1)*threadColumns + temp2; j++) {
			adjacent_to (arg->board, arg->newboard, arg->size, i, j);
		}
	pthread_exit(NULL);
}
//funcao geradora de threads para o jogo
void play (cell_t ** board, cell_t ** newboard, int size) {
	/* for each cell, apply the rules of Life */
	pthread_t thread[MAX_THREADS];
	boards temp[MAX_THREADS];
	int k, l, t = 0;
	for (k = 0; k < threadsLinha; k++){
		for (l = 0; l < threadsColuna; l++) {
			temp[t].board = board;
			temp[t].newboard = newboard;
			temp[t].size = size;
			temp[t].k = k;
			temp[t].l = l;
			pthread_create(&thread[t], NULL, player, (void*) &temp[t]);
			t++;
		}
	}
	for (k = 0; k < MAX_THREADS; k++)
			pthread_join(thread[k],NULL);
}

int main (int argc, char** argv) {
	//seta o numero maximo de threads
	MAX_THREADS = argc > 1? atoi(argv[1]) : get_nprocs ();
	if(MAX_THREADS <= 0)
		MAX_THREADS = 1;

	//declara variaveis
	int size, steps, i;
	cell_t ** tmp;
	FILE    *f;

	//inicializa variaveis;
	f = stdin;
	fscanf(f,"%d %d", &size, &steps);
	
	//divisor de threads por linha e coluna
	int temp = sqrt(MAX_THREADS);
	int temp2 = temp;
	if (MAX_THREADS % temp != 0)
		for (int i = 1; i < temp; i++)
			if (MAX_THREADS%i == 0)
				temp2 = i;
	threadsColuna = temp2;
	threadsLinha = MAX_THREADS/temp2;
	
	//aloca tabuleiro
	cell_t ** prev = allocate_board (size);
	//seta tabuleiro
	read_file (f, prev,size);
	fclose(f);
	
	//aloca proximo tabuleiro
	cell_t ** next = allocate_board (size);

	#ifdef DEBUG
		printf("Initial:\n");
		print(prev,size);
	#endif

	for (i=0; i<steps; i++) {
		//seta o proximo tabuleiro
		play (prev,next,size);

		#ifdef DEBUG
			printf("%d ----------\n", i + 1);
			print (next,size);
		#endif

		//realiza troca triangular de tabuleiros
		tmp = next;
		next = prev;
		prev = tmp;
	}

	#ifdef RESULT
	printf("Final:\n");
	print (prev,size);
	#endif

	//libera memoria
	free_board(prev,size);
	free_board(next,size);
}
