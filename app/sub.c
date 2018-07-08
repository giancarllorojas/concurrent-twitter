#include "mysocket.h"  
#include <pthread.h>
#include <stdbool.h>

#define BUFFER_SIZE 140
#define NTHREADS 100
#define MAX_INSCRIPTIONS 30

int inscriptionsNumber = 0;
char *serverAddr;
int serverPort, localPort;

struct inscription {
  char userName[50];
  int id;
  bool canceled;
};

struct inscription inscriptions[MAX_INSCRIPTIONS];

void cancel_subinscription(int userNumber){
  TSocket srvSock;
  char request[60];
  char response[60];
  int responseCode;

  srvSock = ConnectToServer(serverAddr, serverPort);
   
  sprintf(request, "3 %d\n", inscriptions[userNumber].id);

  printf("%s",request);

  WriteN(srvSock, request, strlen(request));
  ReadLine(srvSock, response, 59);

  responseCode = atoi(response);

  if(responseCode == 1){
    inscriptions[userNumber].canceled = true;
    printf("Desinscrição feita com sucesso!\n");
  }else{
    printf("Não foi possível cancelar a inscrição\n");
  }
}

/**
 * Não da pra implementar porque não tem na especificação do servidor
 */ 
void list_publishers(){

}

bool list_inscriptions(){
  if(inscriptionsNumber == 0){
    printf("Você não tem nenhuma inscrição no momento para desinscrever\n");
    return false;
  }

  printf("Lista de publicadores que você está inscrito:\n");

  for(int i = 0; i < inscriptionsNumber; i++){
    if(!inscriptions[i].canceled) printf("%d: %s\n", i, inscriptions[i].userName);
  }

  return true;
}

void subscribe(){
  TSocket srvSock;
  char userName[30];
  char request[60];
  char response[60];
  int responseCode, userCode;

  printf("Digite o nome de usuário que deseja se inscrever ou RET para voltar:\n");
  
  list_publishers();

  scanf("%99[^\n]%*c", userName);

  srvSock = ConnectToServer(serverAddr, serverPort);

  if (strncmp(userName, "RET", 3) == 0) {
    return;
  };

  sprintf(request, "2 %s %d\n", userName, localPort);
  WriteN(srvSock, request, strlen(request));

  ReadLine(srvSock, response, 59);

  printf("%s", response);
  responseCode = atoi(strtok(response, " "));
  userCode     = atoi(strtok(NULL, " "));

  if(responseCode == 0){
    printf("Erro ao se inscrever\n");
  }else{
    printf("Inscrição feita! Código da inscrição: %d \n", userCode);

    inscriptions[inscriptionsNumber].id = userCode;
    inscriptions[inscriptionsNumber].canceled = false;
    sprintf(inscriptions[inscriptionsNumber].userName, "%s", userName);
    inscriptionsNumber++;
  }
}

void unsubscribe(){
  char userNumber[30];
  int n;

  if(!list_inscriptions()) return;
  printf("Digite o número da inscrição que deseja cancelar a inscrição ou RET para voltar:\n");

  scanf("%99[^\n]%*c", userNumber);

  if (strncmp(userNumber, "RET", 3) == 0) {
    return;
  };

  n = atoi(userNumber);

  cancel_subinscription(n);
}

void cancel_all_subscriptions(){
  for(int i = 0; i < inscriptionsNumber; i++){
    cancel_subinscription(i);
  }
}

void main_loop(){
  TSocket localServer;
  fd_set set;
  int ret;
  char str[30];

  localServer = CreateServer(localPort);

  /* Run until FIM */
  for (;;) { 
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    FD_SET(localServer, &set);

    printf("\nDigite: \n 1: Ver lista de publicadores(Não implementado) \n 2: Inscreve em um publicador \n 3: Desfazer uma inscrição \n FIM: Sair\n\n");

    /* Select returns 1 if input available, -1 if error */
    ret = select (FD_SETSIZE, &set, NULL, NULL, NULL);
    if (ret<0) {
       WriteError("select() failed"); 
       break;
    }

    /* Read from stdin */
    if (FD_ISSET(STDIN_FILENO, &set)) {
      scanf("%99[^\n]%*c", str);
      if (strncmp(str, "FIM", 3) == 0){
        cancel_all_subscriptions();
        break;
      };

      if (strncmp(str, "1", 1) == 0) {
        list_publishers();
      };

      if (strncmp(str, "2", 2) == 0) {
        subscribe();
      };

      if (strncmp(str, "3", 3) == 0) {
        unsubscribe();
      };
    }

    /* Read from localServer */
    if (FD_ISSET(localServer, &set)) {
      char serverResponse[BUFFER_SIZE];
      printf("Mensagem recebida:\n");
      TSocket srvSock = AcceptConnection(localServer);

      ReadLine(srvSock, serverResponse, BUFFER_SIZE-1);
      printf("%s", serverResponse);
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 4) {
    ExitWithError("Usage: sub <porta local> <endereco server> <porta server>");
  }
  else {
    localPort = atoi(argv[1]);
    serverAddr = argv[2];
    serverPort = atoi(argv[3]);
  }

  main_loop();
  return 0;
}