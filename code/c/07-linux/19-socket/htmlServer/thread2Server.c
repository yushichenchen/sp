#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <err.h>
#include <pthread.h>

#define THREAD_MAX 10
#define STR_MAX  1000
#define TEXT_MAX 10000

pthread_t threads[THREAD_MAX];
int client_fds[THREAD_MAX];

void readHeader(int client_fd, char *header) {
  int len = read(client_fd, header, TEXT_MAX);
  header[len] = '\0';
}

void parseHeader(char *header, char *path) {
  sscanf(header, "GET %s HTTP/", path);
}

void responseFile(int client_fd, char *path) {
  char text[TEXT_MAX], response[TEXT_MAX], fpath[STR_MAX];
  sprintf(fpath, "./web/%s", path);
  printf("responseFile:fpath=%s\n", fpath);
  FILE *file = fopen(fpath, "r");
  printf("file=%d\n", file);
  int len = fread(text, 1, TEXT_MAX, file);
  text[len] = '\0';
  printf("text=%s\n", text);
  sprintf(response, "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html; charset=UTF-8\r\n"
                    "Content-Length: %d\r\n\r\n%s", len, text);
  write(client_fd, response, strlen(response));
}

void* handleRequest(void *client_fdp) {
  char header[TEXT_MAX], path[STR_MAX];
  int client_fd = *(int*) client_fdp;
  if (client_fd == -1) {
    printf("Can't accept");
    return NULL;
  }
  readHeader(client_fd, header);
  printf("===========header=============\n%s\n", header);
  parseHeader(header, path);
  printf("path=%s\n", path);
  if (strstr(path, ".htm") != NULL) {
    printf("path contain .htm\n");
    responseFile(client_fd, path);
  } else {
    printf("not html => no response!\n");
  }
  close(client_fd);
  *(int*)client_fdp = -1;
}

int main()
{
  int one = 1, client_fd;
  struct sockaddr_in svr_addr, cli_addr;
  socklen_t sin_len = sizeof(cli_addr);
 
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) err(1, "can't open socket");
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
  int port = 8080;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);
 
  if (bind(sock, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(sock);
    err(1, "Can't bind");
  }
 
  listen(sock, 5);
  memset(client_fds, -1, THREAD_MAX);
  while (1) {
    int ti;
    for (ti=0; ti<THREAD_MAX; ti++) {
      if (client_fds[ti] <= 0) break;
    }
    if (ti < THREAD_MAX) {
      client_fds[ti] = accept(sock, (struct sockaddr *) &cli_addr, &sin_len);
      pthread_create(&threads[ti], NULL, handleRequest, (void*) &client_fds[ti]);
    } else {
      sleep(1);
    }
  }
}
