#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define size 27  
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int matriz[9][9];
int sudokuValido = 1;

void preencheSudoku(){
    int i;
    int j;
	for(int i=0; i<9; i++){
		for(int j=0; j<9; j++){
			scanf("%d", &matriz[i][j]);
		}
	}
}

void *checarLinhas(void *b){
    
    int tid = *((int *)b); 
    int i, j, k;

	i=tid;    // define qual é a linha

	for(j = 0; j < 9 && sudokuValido; j++)     // percorre as colunas
	{
		for(k = 0; k < 9 && sudokuValido; k++) // tambem percorre as colunas
		{
			// Verifica se naquela linha algum numero se repete
			// caso se repita, sudokuValido = 0 e para todas as operações da
			// thread restantes

			if((j != k) && (matriz[i][j] == matriz[i][k]))
			{
				pthread_mutex_lock(&lock);
				sudokuValido = 0;
				pthread_mutex_unlock(&lock);
				break;
			}
		}

	}      
    pthread_exit(NULL); 
}

void *checarColunas(void *b){

    int tid = *((int *)b); 
    int i, j, k;
	
  	j = tid - 9;   // define a coluna, como os tid responsaveis por verifica as
	  			   // colunas são de 9 a 17, temos que subtrair 9 pra encontrar
				   // o idetificador correto da matriz
				 
		
	for(i = 0; i < 9 && sudokuValido; i++)       // percorre as linhas
	{
		for(k = 0; k < 9 && sudokuValido; k++)   //tambem percorre as linhas
		{
			// Se um mesmo numero for encontrado duas vezes, sudokuValido = 0
			if((i != k) && (matriz[i][j] == matriz[k][j]))
			{
				pthread_mutex_lock(&lock);
				sudokuValido = 0; 
				pthread_mutex_unlock(&lock);
				break;
			}
		}     
	}             

    pthread_exit(NULL); 
}

void *checarQuadrante(void *b){
    
    int tid = *((int *)b); 
    int i, j;           	// variaveis auxiliares
	int N,M;				// representarao a linha e a coluna inicial
	int num, existe = 0;	

	int threadId = tid - 18;	// é subtraído em 18 porque recebera tid de 18 a 
								//27 torna mais facil vizualizar os quadrantes 
								//de 0 a 8
	
	switch (threadId)     
	{
		case 0:
			N=0;
			M=0;
			break;
		case 1:
			N=0;
			M=3;
			break;
		case 2:
			N=0;
			M=6;
			break;
		case 3:
			N=3;
			M=0;
			break;
		case 4:
			N=3;
			M=3;
			break;
		case 5:
			N=3;
			M=6;
			break;
		case 6:
			N=6;
			M=0;
			break;
		case 7:
			N=6;
			M=3;
			break;
		case 8:
			N=6;
			M=6;
			break;
		default:
			break;
	}
	
	for(num = 1; num <= 9 && sudokuValido; num++){	// verifica se um mesmo numero aparece												//numero apareceu mais de 
	                                                //mis de uma vez

		existe = 0;
		for(i = 0; i < 3 && sudokuValido; i++)
		{
			for(j = 0; j < 3 && sudokuValido; j++)
			{
				if(matriz[N+i][M+j] == num){
					 existe++;
				}	
			}
		}

		if(existe>1){		            // se um numero aparecer mais de uma vez, 
			pthread_mutex_lock(&lock);  //sudokuValido = 0
			sudokuValido = 0; 
			pthread_mutex_unlock(&lock);
			break ;
		}

	
	}

    pthread_exit(NULL); 
}




// Na função main ocorre a criação das threads
// O mecanismo de sincronização é necessário para que a main não finalize antes das outras
// threads finalizarem a contagem. 
int main (int argc, char *argv[]){   
    pthread_t threads[size];
    int *indice[size];
    int rc;   
    int t;

    preencheSudoku();
  
  
  
    for(t=0; t<size; t++){      
        indice[t] = (int *) malloc(sizeof(int)); *indice[t] = t;
        
        if(t<10){
            rc = pthread_create(&threads[t], NULL, checarColunas, (void *) indice[t]);      
            if (rc){         
            printf(" o código de retorno é %d\n", rc);         
            exit(-1);      
            }      
        } 
        else if(t>10 && t<19){
            rc = pthread_create(&threads[t], NULL, checarColunas, (void *) indice[t]);      
            if (rc){         
            printf(" o código de retorno é %d\n", rc);         
            exit(-1);      
            }      
        }
        else{   
            rc = pthread_create(&threads[t], NULL, checarQuadrante, (void *) indice[t]);      
            if (rc){         
            printf(" o código de retorno é %d\n", rc);         
            exit(-1);      
            }   
        }
    }
  
    for(t=0;t<size;t++){
        pthread_join(threads[t],NULL);
    }

    if (sudokuValido){
        printf("VALIDO.\n");
    } 
    else {
        printf("INVALIDO\n");
    }  

  
  
  
  return 0;   
}