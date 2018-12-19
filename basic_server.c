#include "pipe_networking.h"

int main() {

  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );

  char msg[BUFFER_SIZE];
  char data[BUFFER_SIZE];
  while(1){

    read(from_client, msg, sizeof(msg));
    printf("The client says: %s\n", msg);
    strcpy(data, msg);
    strcat(data, " processed");
    printf("%s", data);
    write(to_client, data, sizeof(data));
  }
}
