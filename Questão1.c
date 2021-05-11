#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define size 5  
pthread_mutex_t mymutex = PTHREAD_MUTEX_INITIALIZER;
int contador = 0;

// A função contando utiliza o mecanismo de exclusão mutua 
// antes de acessar a região crítica, que é o contador.
void *contando(void *b){   
  int ind = *((int *)b);   
  while(contador < 1000000){
    pthread_mutex_lock(&mymutex); 
    contador++; 
    if(contador == 1000000){
      printf("A thread #%d chegou no número 1000000!\n", ind); 
    }
    pthread_mutex_unlock(&mymutex); 
  }   
  pthread_exit(NULL);
}

// Na função main ocorre a criação das threads
// O mecanismo de sincronização é necessário para que a main não finalize antes das outras
// threads finalizarem a contagem. 
int main (int argc, char *argv[]){   
  pthread_t threads[size];
  int *indice[size];

  int rc;   int t;   
  for(t=0; t<size; t++){      
   indice[t] = (int *) malloc(sizeof(int)); *indice[t] = t;
    rc = pthread_create(&threads[t], NULL, contando, (void *) indice[t]);      
    if (rc){         
      printf(" o código de retorno é %d\n", rc);         
      exit(-1);      
    }   
  }
  for(t=0;t<size;t++){
        pthread_join(threads[t],NULL);
    }
  return 0;   
}