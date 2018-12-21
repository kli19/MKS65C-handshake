#include "pipe_networking.h"

static void sighandler(int signo){
  
  if(signo == SIGINT){
    //printf("removing WKP\n");
    remove("wkp");
    exit(0);
  }
}

int main() {
  signal(SIGINT, sighandler);
  
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  char msg[BUFFER_SIZE];
  char data[BUFFER_SIZE];
    
  while(1){
    memset(data, 0, BUFFER_SIZE);
    read(from_client, msg, sizeof(msg));
    printf("The client says: %s", msg);
    strncpy(data, msg, strlen(msg)-1);
    strcat(data, " processed\0");
    //printf("%s", data);
    write(to_client, data, sizeof(data));
    
  }
}
