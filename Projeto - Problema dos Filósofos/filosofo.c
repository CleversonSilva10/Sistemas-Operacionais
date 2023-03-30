// Gustavo Teixeira dos Santos  TIA: 32197020
// Cleverson Pereira da Silva  TIA: 32198531
// Pedro Augusto Lima  TIA: 32136374

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define quant_filosofos 5 // Define constante de quantidade de fil�sofos
#define filosofo_aguardando 0 
//identificador de filosofo_aguardando
#define filosofo_com_fome 1 
//identificador de filosofo_com_fome
#define filosofo_comendo 2 
//identificador de filosofo_comendo

sem_t sem_filosofo[quant_filosofos];// inicia semaforo
sem_t saleiro; // Variável semáforo para controlar a permiss�o de alteração de estado_filosofo
int estado_filosofo [quant_filosofos];// atual posi��o que o filosofo se encontra
int num_filosofo[quant_filosofos]= {0,1,2,3,4};// inicializa filosofos

//Fun��es//
void *filosofo(void *n);
void pegaGarfo(int);
void soltaGarfo(int);
void teste(int);

//Fun��o pega garfo//

void pegaGarfo(int num_filosofo)
{
  sem_wait(&saleiro); // Espera o semáforo que controla as mudanças de estado_filosofo ficar livre
  estado_filosofo[num_filosofo]= filosofo_com_fome; // Atualiza o estado_filosofo do fil�sofo para "Com fome"
  printf("Filosofo de numero %d esta com fome \n",num_filosofo+1);// Exibe mensagem na tela de que o fil�sofo está com fome
  teste(num_filosofo); // Tenta fazer com que o fil�sofo pegue os garfos. 
  sem_post(&saleiro); // Destrava o semáforo saleiro
  sem_wait(&sem_filosofo[num_filosofo]); // Espera até o semáforo referente ao fil�sofo estar liberado
  sleep(1);
}

//Fun��o solta garfo//

void soltaGarfo(int num_filosofo)
{
  sem_wait(&saleiro); // Espera o semáforo que controla as mudanças de estado_filosofo ficar livre
  estado_filosofo[num_filosofo]= filosofo_aguardando; // Atualiza o estado_filosofo do fil�sofo para "Aguardando"
  printf("Filosofo de numero %d terminou de comer e deixou os garfos %d e %d \n",num_filosofo+1,(num_filosofo+4)%quant_filosofos+1,num_filosofo+1); // Exibe mensagem na tela avisando que o fil�sofo pegou o garfo
  printf("Filosofo de numero %d agora esta aguardando\n\n",num_filosofo+1); // Exibe mensagem na tela avisando que o fil�sofo está aguardando
  teste((num_filosofo+4)%quant_filosofos); // Tenta fazer com que o fil�sofo "A esquerda" comer
  teste((num_filosofo+1)%quant_filosofos); // Tenta fazer com que o fil�sofo "A direita" comer
  sem_post(&saleiro); // Destrava o semáforo saleiro
}

//Fun��o teste//

void teste(int num_filosofo)
{
  if(estado_filosofo[num_filosofo]==filosofo_com_fome&& estado_filosofo[(num_filosofo+4)%quant_filosofos]!=filosofo_comendo&& estado_filosofo[(num_filosofo+1)%quant_filosofos]!=filosofo_comendo) // Checa se o fil�sofo pode comer
  {
    estado_filosofo[num_filosofo]=filosofo_comendo; // Coloca o estado_filosofo do fil�sofo como "Comendo"
    sleep(1); 
    printf("Filosofo de numero %d pegou os garfos %d e %d \n",num_filosofo+1,(num_filosofo+4)%quant_filosofos+1,num_filosofo+1); // Exibe mensagem avisando que o fil�sofo pegou os garfos
    printf("Filosofo de numero %d esta comendo \n",num_filosofo+1); // Exibe mensagem avisando que o fil�sofo está comendo
    sem_post(&sem_filosofo[num_filosofo]); // Libera o semáforo do fil�sofo
  }
}

//Fun��o filosofos//

void *filosofo(void *n) // Fun��o que demonstra o comportamento dos fil�sofos
{
  while(1) // Laço
  {
   int *i = n;
   sleep(1);
   pegaGarfo(*i); // Chama Fun��o que pega o garfo
   sleep(1);
   soltaGarfo(*i); // Chama a Fun��o que solta o garfo
  }
}

//Fun��o main//

int main()
{
  int i;
  pthread_t ident_tr[quant_filosofos];// identificador de thread
  sem_init(&saleiro,0,1);
  for(i=0; i<quant_filosofos; i++)
    sem_init(&sem_filosofo[i],0,0);
  for(i=0;i<quant_filosofos;i++)
  {
    pthread_create(&ident_tr[i], NULL, filosofo, &num_filosofo[i]);// criador de threads necessarias
    printf("Filosofo de numero %d esta aguardando.\n",i+1);// imprime filosofo que esta aguardando
  }
  printf("\n");
  for(i=0;i<quant_filosofos;i++)
  {
    pthread_join(ident_tr[i],NULL);// thread principal aguarda as outras threads
  }
  return(0);
}


