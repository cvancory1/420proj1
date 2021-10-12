
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <crypt.h> - mac doesn't know what this is

// crypt function - returns char* of crypted input word
char* crypting(char *password) {
  //printf("In crypt function -- pasword to be encrypted is %s\n", password);
  char* result = crypt(password, "$1$ab$"); // hardcode the salt - on Mac salt is second!!
  //char* result = crypt("$1$ab$", password); // linux version

  //printf("In crypt function - your encrypted password is: %s\n", result);
  return result;
}

// checks if hashed word is same as hashed password
// returns 0 if they are the same and 1 if not
int compare(char *password, char *crypted) {
  int ret = 1;
  if (strcmp(password, crypted) == 0) {
        printf("Password has been cracked. The password is: \n");
        for (int i = 0; i < strlen(password); i++)
          printf("%c", password[i]);
        printf("\nThe crypted version is: \n");
        for (int i = 0; i < strlen(crypted); i++)
          printf("%c", crypted[i]);
        ret = 0; // set to 0 since its found
      } else {
        //printf("Password has not been cracked :(\n");
        ret = 1;
      }
  return ret;
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

  sprintf(buf, "%s", word); // puts null terminator after inserting word

  puts("");
  // test printing
  for (int i = 0; i < buflen; i++){
    printf("%c", buf[i]);
  }
  puts("\n");

  // ------ CREATING EVERY POSSIBLE SUFFIX ------
  // TODO: need 0's!!! & change 4s to 10s...

  // initalize to 0 so ++ incrementing works properly
  buf[wordlen] = '0'; 
  for (int i = 1; i < 4; i++) {
    printf("buf = %s\n", buf);
    buf[wordlen + 4] = '\0'; // place null terminator after suffix number prior to crypting
    char *crypted = crypting(buf); // crypt
    int same = compare(password, crypted); // compare

    // no need to keep checking if the password has been found
    if (same == 0)
      break;

    buf[wordlen + 1] = '0'; 
    for (int j = 1; j < 4; j++) {
      buf[wordlen + 1]++; // word1100
      printf("buf = %s\n", buf);
      buf[wordlen + 4] = '\0'; 
      char *crypted = crypting(buf);
      int same = compare(password, crypted);

      if (same == 0)
        break;

      buf[wordlen + 2] = '0';
      for (int k = 1; k < 4; k++) {
        buf[wordlen + 2]++; // word1110
        printf("buf = %s\n", buf);
        buf[wordlen + 4] = '\0'; 
        char *crypted = crypting(buf); 
        int same = compare(password, crypted);

        // no need to keep checking if the password has been found
        if (same == 0)
          break;

        buf[wordlen + 3] = '0';
        for (int l = 1; l < 4; l++) {
          buf[wordlen + 3]++; // word1111
          printf("buf = %s\n", buf);
          buf[wordlen + 4] = '\0'; // place null terminator after suffix number prior to crypting
          char *crypted = crypting(buf); // crypt
          int same = compare(password, crypted); // compare

          // no need to keep checking if the password has been found
          if (same == 0)
            break;

        } // end of word1111
      } // end of word1110
    } // end of word1100
  } // end of word1000
  return 0;
}

int main () {
  char *word = "aardvark";
  char *password = "inputhashhere";

  // testing purposes:
  // char *crypted = crypting(password);
  // printf("Crypted word: ");
  // for (int i = 0; i < strlen(crypted); i++)
  //   printf("%c", crypted[i]);

  checkWord(password, word);

  return 0;
}