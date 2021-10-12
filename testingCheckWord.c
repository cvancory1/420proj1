
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <crypt.h> - mac doesn't know what this is

// crypt function - returns char* of crypted input word
char* crypting(char *password) {
  printf("In crypt function -- pasword to be encrypted is %s\n", password);
  char* result = crypt(password, "$1$ab$"); // hardcode the salt - on Mac salt is second!!
  //char* result = crypt("$1$ab$", password); // linux version

  printf("In crypt function - your encrypted password is: %s\n", result);
  return result;
}

// takes in the crypted password & the current dictionary word
// going through every prefix & suffix, hash the prefixWord & suffixWord to determine if it matches the password
// (use string compare, == compares pointers)
int checkWord(char *password , char *word) {

  int maxLen = 40; // maximum word length
  int buflen = maxLen * sizeof(word) * sizeof(char); // buffer length = max * array length * char size
  char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
  memset(buf, '#', buflen); // fills with null terminators

  int wordlen = strlen(word); // store dict word length

  // traverse through the buf by character index to change
  // ex) word[3]++ NOT string word2 = word + 1

  // puts null terminator after inserting word
  sprintf(buf, "%s", word);

  puts("");
  // test printing
  for (int i = 0; i < buflen; i++){
    printf("%c", buf[i]);
  }
  puts("\n");

  // adding suffix numbers - first just 1-9
  buf[wordlen] = '0'; // initalize to 0 so ++ incrementing works properly
  buf[wordlen+1] = '\0';
  for (int i = 1; i < 10; i++) {
    buf[wordlen]++; 
    printf("buf = %s\n", buf);

    // crypt
    char *crypted = crypting(buf); // do we need to send in only the part of buf that is used?

    // printf("Test1: %s\n", crypting("abcd"));
    // printf("Test2: %s\n", crypting("bbcd"));
    // printf("Test2: %s\n", crypting("cbcd"));
    // printf("Test2: %s\n", crypting("dbcd"));
    // printf("Test2: %s\n", crypting("ebcd"));
    // printf("Test2: %s\n", crypting("fbcd"));
  
    // compare
    if (strcmp(password, crypted) == 0) {
      printf("Password has been cracked. The password is: \n");
      for (int i = 0; i < strlen(password); i++)
        printf("%c", password[i]);
      printf("\nThe crypted version is: \n");
      for (int i = 0; i < strlen(crypted); i++)
        printf("%c", crypted[i]);
    } else {
      printf("Password has not been cracked :(\n");
    }

  }
  return 0;
}

int main () {
  char *word = "aardvark";
  char *password = "aardvark123";
  char *crypted = crypting(password);
  printf("Crypted word: ");
  for (int i = 0; i < strlen(crypted); i++)
    printf("%c", crypted[i]);
  // why is this showing a different hash than the function??? ******

  checkWord(password, word);

  return 0;
}