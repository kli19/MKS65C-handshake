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

  char msg[BUFFER_SIZE];
  char data[BUFFER_SIZE];
    
  while(1){
    server_handshake_part1(&from_client);
    int client_pid = fork();
    if(client_pid){
      close(from_client);
      printf("removing wkp\n");
      remove("wkp");
      printf("removed wkp\n");
    }
    else{
      server_handshake_part2(&to_client, &from_client);     
      while(read(from_client, msg, sizeof(msg))){
	memset(data, 0, BUFFER_SIZE);
	printf("The client says: %s", msg);
	strncpy(data, msg, strlen(msg)-1);
	strcat(data, " processed\0");
	//printf("%s", data);
	write(to_client, data, sizeof(data));
      }
    }
  }
  return 0;
}
