#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>


// verificar se uso int ou char
typedef struct {
    int vermelho;
    int verde;
    int azul;
    int vermelhoNovo;
    int verdeNovo;
    int azulNovo;
    int tonsDeCinza;
    
} Pixel;

int numThreads=0;

int linhas, colunas, valorMaximo, quantPixels;
Pixel *pixels;

void obterDimensoes(FILE *arquivo);
int verificarFormato(FILE *arquivo);
void *converterTonsDeCinza(void *threadid); 
void obterValoresPixels(FILE *arquivo);
void imprimirVetorPixel();
void criarArquivoFinal(FILE *arquivo);

int main(int argc, char *argv[]){

    FILE *arq, *arqFinal;
   
    char nomeArquivo[50];

    printf("Digite o numero de Threads: \n");
    scanf("%d", &numThreads);

    pthread_t threads[numThreads];
    int *taskids[numThreads];

    printf("\nDigite o nome do arquivo que deseja abrir: \n");
    printf("Obs.: Nao esqueça da extensao do arquivo \n");
    scanf("%s", nomeArquivo);
       
    arq = fopen(nomeArquivo, "rt");                

    if(arq == NULL){
        printf("Problemas na criação do arquivo\n");
        return 1;
    }
    
    if(verificarFormato(arq)){       //  Verifica se o arquivo tem o formato certo, P3
        return 1;
    }
        
    obterDimensoes(arq);           // obtem a quantidade de colunas, linhas e o valor maximo
    quantPixels=colunas*linhas;
    
    pixels = (Pixel *)malloc( quantPixels * sizeof( Pixel));
    
    obterValoresPixels(arq);

    for(int t=0; t<numThreads; t++){
        printf("Criando a thread : %d\n", t);
        taskids[t] = (int *) malloc(sizeof(int));
        *taskids[t] = t;
        pthread_create(&threads[t], NULL, converterTonsDeCinza,(void *)taskids[t]);
    }
    
    for(int u=0; u<numThreads;u++) {
       
        pthread_join(threads[u], NULL);
    } 
    
    imprimirVetorPixel();

    printf("\nDigite o nome do arquivo onde deseja salvar o resultado: \n");
    printf("Obs.: Nao esqueça da extensao do arquivo \n");
    scanf("%s", nomeArquivo);
    arqFinal = fopen(nomeArquivo, "w");
    
    if(arqFinal == NULL){
        printf("Problemas na criação do arquivo\n");
        return 1;
    }
   
    criarArquivoFinal(arqFinal);

    pthread_exit(NULL);
        
    fclose(arq);
    fclose(arqFinal);
    free(pixels);
   
   
    
    return 0; 
}

//  Verifica se o arquivo tem o formato certo, P3
int verificarFormato(FILE *arquivo){
    
    char formato[5];
    fgets(formato, 3, arquivo);
    printf("%s \n", formato);

 
    if(strcmp(formato, "P3")){
        printf("Formato invalido !\n");
        return 1;
    }
    return 0;
}

// Obtem os valores das colunas, linhas e valorMaximo
void obterDimensoes(FILE *arquivo){
    char l[10], c[10], valMaximo[10];

    fscanf(arquivo, "%s%s%s", c,l,valMaximo);
    
    colunas = atoi(c);
   
    linhas = atoi(l);
   
    valorMaximo= atoi(valMaximo);
   
   printf("Numero de Colunas: %d \n", colunas);
   printf("Numero de Linhas: %d \n", linhas);
   printf("Valor Maximo: %d \n", valorMaximo);

    
}

// Armazena no vetor os valores de cada um dos pixels
void obterValoresPixels(FILE *arquivo){
   
    char red[6], blue[6], green[6];
    
    printf("\n Valor original de todos os pixel\n\n");
    for(int i=0; i<quantPixels; i++){             
        fscanf(arquivo, "%s%s%s", red, green, blue);
        pixels[i].vermelho = atoi(red);
        pixels[i].verde = atoi(green);
        pixels[i].azul = atoi(blue);

        printf("Vermelho = %3d, Verde = %3d  Azul= %3d \n",pixels[i].vermelho, pixels[i].verde,  pixels[i].azul);
    }
    
}

void *converterTonsDeCinza(void *threadid){ 

    int tid = *((int *)threadid);

    for(int i=tid; i<quantPixels; i= i+numThreads){
        printf("thread %d fazendo o pixel %d\n", tid, i);
        pixels[i].vermelhoNovo =  pixels[i].vermelho * 0.30;
        pixels[i].verdeNovo =  pixels[i].verde * 0.59;
        pixels[i].azulNovo =  pixels[i].azul * 0.11;

        pixels[i].tonsDeCinza = pixels[i].vermelhoNovo + pixels[i].verdeNovo + pixels[i].azulNovo ;
        
    }
    
    pthread_exit(NULL);
}

void imprimirVetorPixel(){
    
    printf("\nImagem em tons de cinza\n\n");

    for(int i=0;i< quantPixels; i++){
        printf("Vermelho = %3d Verde = %3d  Azul = %3d\n",pixels[i].vermelhoNovo ,pixels[i].verdeNovo,pixels[i].azulNovo );
    }
}

void criarArquivoFinal(FILE *arquivo){
    
    fprintf(arquivo, "%s \n%d %d \n%d \n", "P3", colunas, linhas, valorMaximo);
    for(int i=0; i<quantPixels; i++){
       fprintf(arquivo,"%3d %3d %3d \n",pixels[i].tonsDeCinza ,pixels[i].tonsDeCinza,pixels[i].tonsDeCinza );
    }
}