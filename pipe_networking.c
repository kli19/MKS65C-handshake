#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  //printf("Server making WKP\n");
  mkfifo("wkp", 0644);
  //printf("Server made WKP\n");

  //printf("Server opening WKP\n");
  int from_client = open("wkp", O_RDONLY);
  //printf("Server opened WKP\n");

  //printf("Server reading WKP\n");
  read(from_client, pid, sizeof(pid));
  printf("Server read \"%s\"\n", pid);

  //printf("Server opening private pipe\n");
  *to_client = open(pid, O_WRONLY);
  //printf("Server opened private pipe\n");

  //printf("Server removing WKP\n");
  remove("wkp");
  //printf("Server removed WKP\n");

  //printf("Server writing to private pipe\n");
  write(*to_client, ACK, sizeof(ACK));
  printf("Server wrote \"%s\"\n", ACK);

  //printf("Server reading from WKP\n";)
  read(from_client, msg, sizeof(msg));
  printf("The server read \"%s\"\n", msg);
  printf("Handshake complete!\n=================\n");

  while(1){
    read(from_client, msg, sizeof(msg));
    printf("The client says: %s\n", msg);
    char data[BUFFER_SIZE];
    int len = strlen(msg);
    //sprintf(data, "Length: %d\n", len);
    printf("%s", data);
    write(*to_client, msg, sizeof(msg));
  }
  
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  char pid[HANDSHAKE_BUFFER_SIZE];
  char msg[HANDSHAKE_BUFFER_SIZE];

  sprintf(pid, "%d", getpid());

  //printf("Client creating private pipe\n");
  mkfifo(pid, 0644);
  //printf("Client created private pipe\n");

  //printf("Client opening private pipe\n");
  *to_server = open("wkp", O_WRONLY);
  //printf("Client opened private pipe\n");

  //printf("Client writing to WKP\n");
  write(*to_server, pid, sizeof(pid));
  printf("Client wrote\"%s\"\n", pid);

  //printf("Client opening private pipe\n");
  int from_server = open(pid, O_RDONLY);
  //printf("Client opening private pipe\n");

  //printf("Client reading private pipe\n");
  read(from_server, msg, sizeof(msg));
  printf("Client read \"%s\"\n", msg);

  //printf("Client removing private pipe\n");
  remove(pid);
  //printf("Client removed private pipe\n");

  //printf("Client writing to WKP\n");
  write(*to_server, ACK, sizeof(ACK));
  printf("Client wrote \"%s\"\n", ACK);

  printf("Handshake complete!\n==================\n");

  while(1){
    printf("Send something to the server:\n");
    char * line = malloc(100 * sizeof(char));
    fgets(line, 100, stdin);
    write(*to_server, line, sizeof(line));
    read(from_server, msg, sizeof(msg));
    printf("Server says: %s\n", msg);
    free(line);
  }
  
  return from_server;
}
