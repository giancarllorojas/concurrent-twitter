// an echo client 
#include "mysocket.h"
#define SERVER_ADDR "192.168.1.109"
#define SERVER_PORT 4018
#define 

void registerUser(char* userName, TSocket srvSock){
  char registerMessage[100];
  char registerResponse[10];

  printf("User: %s\n", userName);
  sprintf(registerMessage, "1 %s \n", userName);
  printf("%s", registerMessage);
  WriteN(srvSock, registerMessage, strlen(registerMessage));
  ReadLine(srvSock, registerResponse, 3);

  printf("%s", registerResponse);
}

int main(int argc, char *argv[]) {
  char userName[30];
  TSocket srvSock;

  // Handle connection
  srvSock = ConnectToServer(SERVER_ADDR, SERVER_PORT);

  // 1- Apresenta para o usuario a opcao de cadastrar-se como publicador de mensagens’’ (informar login)
  printf("Digite seu nome de usuário como publicador\n");
  scanf("%99[^\n]%*c", userName);

  // 2- Interage com o despachante para cadastrar o usuario
  registerUser(userName, srvSock);
  return 0;
}
