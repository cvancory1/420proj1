#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
//#include<crypt.h>

#define _DEFAULT_SOURCE

int main(int argc, char** argv){
  if( argc < 3 ){
    printf("Usage: ./cryptest [salt] [password]\n");
    return 1;
  }

  char* result = crypt(argv[2], argv[1]);

  printf("Your encrypted password is: %s\n", result);
  return 0;
}