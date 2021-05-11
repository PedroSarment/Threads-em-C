#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define size 10 
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
char display[40][151];

void *preenche(void *k){   
    FILE *file = ((FILE *)k);
    int a, i, b,j;            
    int inicio1, inicio2;
    int fim1, fim2;
    
    char temporario[40][150];
    

    i=0;
    while(fgets(temporario[i] ,150, file) != NULL){
        i++;
        }
 
    fclose(file);    
 
    //transforma extremos em inteiro 
 
    inicio1 = atoi(temporario[0]);
    fim1 = atoi(temporario[1]);
 
    for(j=0; j<2; j++){
        for(i=0; temporario[j][i] != ' '; i++){
            temporario[j][i] = ' ';
 
        }
    }
 
    inicio2 = atoi(temporario[0]);
    fim2 = atoi(temporario[1]);
 
    // fim transforma extremos em inteiro

    // preenche display 
    a = 2;
    b = 0;
    for (i = inicio2; i <(inicio2+fim2) ; i++){    
        for(j = inicio1; j <(inicio1 + fim1); j++){
            pthread_mutex_unlock(&mymutex); 
            display[i][j] = temporario[a][b];
            pthread_mutex_unlock(&mymutex); 
            b++;
            }
        b=0;
        a++;
    }
    // fim Preenche display

    pthread_exit(NULL);
}   


int main (int argc, char *argv[]){   
    pthread_t threads[size];
    int i, j;
    int rc;   
    int t=0;
    char nome[10];


    FILE *asset0;
    asset0 = fopen("asset0.txt", "r");

    pthread_create(&threads[0], NULL, preenche, (void *) asset0);

    FILE *asset1;
    asset1 = fopen("asset1.txt", "r");

    pthread_create(&threads[1], NULL, preenche, (void *) asset1);

    FILE *asset2;
    asset2 = fopen("asset2.txt", "r");

    pthread_create(&threads[2], NULL, preenche, (void *) asset2);

    FILE *asset3;
    asset3 = fopen("asset3.txt", "r");

    pthread_create(&threads[3], NULL, preenche, (void *) asset3);

    FILE *asset4;
    asset4 = fopen("asset4.txt", "r");

    pthread_create(&threads[4], NULL, preenche, (void *) asset4);

    FILE *asset5;
    asset5 = fopen("asset5.txt", "r");

    pthread_create(&threads[5], NULL, preenche, (void *) asset5);

    FILE *asset6;
    asset6 = fopen("asset6.txt", "r");

    pthread_create(&threads[6], NULL, preenche, (void *) asset6);

    FILE *asset7;
    asset7 = fopen("asset7.txt", "r");

    pthread_create(&threads[7], NULL, preenche, (void *) asset7);

    FILE *asset8;
    asset8 = fopen("asset8.txt", "r");

    pthread_create(&threads[8], NULL, preenche, (void *) asset8);

    FILE *asset9;
    asset9 = fopen("asset9.txt", "r");

    pthread_create(&threads[9], NULL, preenche, (void *) asset9); 
  
    for(t=0;t<size;t++){
        pthread_join(threads[t],NULL);
    }


    for (i = 0; i < 40; i++){    
        for(j = 0; j < 150; j++){
 
            printf("%c", display[i][j]);
        }
                printf("\n");
    }

 

  
  
  return 0;   
}