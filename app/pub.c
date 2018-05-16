// an echo client 
#include "mysocket.h"
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 8000
#define BUFFER_SIZE 100

char userName[30];

void registerUser(TSocket srvSock){
  char registerMessage[100];
  char registerResponse[10];
  int intRes;
  
  // 1- Apresenta para o usuario a opcao de cadastrar-se como publicador de mensagens’’ (informar login)
  printf("Digite seu nome de usuário como publicador\n");
  scanf("%99[^\n]%*c", userName);

  //printf("User: %s\n", userName);
  sprintf(registerMessage, "1 %s \n", userName);
  //printf("%s", registerMessage);
  WriteN(srvSock, registerMessage, strlen(registerMessage) + 1);
  ReadLine(srvSock, registerResponse, BUFFER_SIZE-1);

  printf("%s", registerResponse);
  intRes = atoi(registerResponse);

  if(intRes){
    printf("Usuário registrado\n");
  }else{
    printf("Usuário já registrado, tente outro nome\n");
    registerUser(srvSock);
  }
}

void unregisterUser(srvSock){
  char unregisterMsg[100];
  char response[10];
  char intResponse;

  sprintf(unregisterMsg, "5 %s \n", userName);

  WriteN(srvSock, unregisterMsg, strlen(registerMessage) + 1);
  ReadLine(srvSock, response, BUFFER_SIZE-1);

  intResponse = atoi(response);

  if(intResponse){
    printf("Usuário %s desregistrado\n", userName);
  }else{
    printf("Erro ao desregistrar um usuario");
  }
}

void publishLoop(TSocket srvSock){
  char message[140];
  char pubResponse[10];
  char pubMessage[1000];
  int intResponse;

  for(;;){
    printf("Digite a mensagem que quer publicar\nou digite \"FIM\" para sair\n");
    scanf("%99[^\n]%*c", message);
    
    sprintf(pubMessage, "4 %s %s \n", userName, message);

    if(strcmp(message, "FIM") == 0){
      unregisterUser(srvSock);
      break;
    }

    WriteN(srvSock, pubMessage, strlen(pubMessage) + 1);
    ReadLine(srvSock, pubResponse, BUFFER_SIZE-1);

    intResponse = atoi(pubResponse);

    if(intResponse){
      printf("Mensagem publicada\n");
    }else{
      printf("Erro ao publicar mensagem\n");
    }
  }
  
}

int main(int argc, char *argv[]) {
  TSocket srvSock;

  // Handle connection
  srvSock = ConnectToServer(SERVER_ADDR, SERVER_PORT);

  // 2- Interage com o despachante para cadastrar o usuario
  registerUser(srvSock);
  publishLoop(srvSock);
  close(srvSock);
  return 0;
}
