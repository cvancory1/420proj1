
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h> //- mac doesn't know what this is

// crypt function - returns char* of crypted input word
char* crypting(char *password) {
  //printf("In crypt function -- pasword to be encrypted is %s\n", password);
  char* result = crypt(password, "$1$ab$"); // hardcode the salt - on Mac salt is second!!
  //printf("In crypt function - your encrypted password is: %s\n", result);
  return result;
}

// checks if hashed word is same as hashed password
// returns 1 if they are the same and 0 if not
int compare(char *password, char *crypted, char *nonhashedpass) {
  int ret = 0;
  if (strcmp(password, crypted) == 0) {
    puts("****** PASSWORD HAS BEEN CRACKED!******");
    printf("The password is: %s\n", nonhashedpass);
    printf("The crypted version is: %s\n", crypted);
    ret = 1; // set to 1 since its found
  } else {
    //printf("Password has not been cracked :(\n");
    ret = 0;
  }
  return ret;
}

// checks password to dictionary word with suffixes 0-10
int onesCheck(char *password, char *word) {
  int wordlen = strlen(word); // store dict word length
  int buflen = (2 + wordlen * sizeof(char)); // buffer length = 'tens' place + word length
  char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
  sprintf(buf, "%s", word); // puts null terminator after inserting word

  buf[wordlen] = '0';
  for (int i = 0; i < 10; i++) {
    //printf("(ones) buf = %s\n", buf);

    char *crypted = crypting(buf); // crypt
    int same = compare(password, crypted, buf); // compare
    if (same == 1) {
      return 1; 
    }
    buf[wordlen]++;
  }
  return 0;
}

// checks password to dictionary word with suffixes 10-99
int tensCheck (char *password, char *word) {
  int wordlen = strlen(word); // store dict word length
  int buflen = (2 + wordlen * sizeof(char)); // buffer length = 'tens' place + word length
  char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
  sprintf(buf, "%s", word); // puts null terminator after inserting word

  buf[wordlen] = '0';
  buf[wordlen + 1] = '0'; 

  for (int i = 9; i <= 100; i++) {
      //printf("(tens) buf = %s\n", buf);
      char *crypted = crypting(buf); // crypt
      int same = compare(password, crypted, buf); // compare
      if (same == 1){
        return 1; 
      }
      
      buf[wordlen+1]++;
      if (i % 10 == 0) {
        buf[wordlen]++; // tens 
        buf[wordlen+1]= '0' ; // ones 
      }
  }
}

// checks password to dictionary word with suffixes 100-999
int hundredsCheck (char *password, char *word) {
  int wordlen = strlen(word); // store dict word length
  int buflen = (3 + wordlen * sizeof(char)); // buffer length = 'hundreds' place + word length
  char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
  sprintf(buf, "%s", word); // puts null terminator after inserting word

  buf[wordlen] = '1';
  buf[wordlen+1] = '0'; 
  buf[wordlen+2] = '0'; 

  for(int i = 0; i < 10; i++) {
    for(int j = 0; j < 10; j++) {
      for(int k = 0; k < 10; k++){ 
        //printf("(hundreds) buf = %s\n", buf);
        char *crypted = crypting(buf); // crypt
        //printf("crypted buf (hundreds): %s\n", crypted);
        int same = compare(password, crypted, buf); // compare
        //printf("SAME: %d", same);
        if (same == 1) { // compare returns 1 if found, not 0
          return 1; 
        }
        
        buf[wordlen+2]++; // ones place 

        // hacking - stops the loop early 
        if (buf[wordlen+2]==':' && buf[wordlen+1]=='9' && buf[wordlen]=='9') {
          return 0;
        }
      
      }
      buf[wordlen+2]= '0'; // ones
      buf[wordlen+1]++; // tens
    }
    buf[wordlen+2]= '0'; // ones
    buf[wordlen+1]='0';// tens
    buf[wordlen]++; // hundreads
  }
}

// checks password to dictionary word with suffixes 1000-9999
int thousandsCheck (char *password, char *word) {
  int wordlen = strlen(word); // store dict word length
  int buflen = (4 + wordlen * sizeof(char)); // buffer length = 'hundreds' place + word length
  char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
  sprintf(buf, "%s", word); // puts null terminator after inserting word

  buf[wordlen] = '1';
  buf[wordlen+1] = '0'; 
  buf[wordlen+2] = '0'; 
  buf[wordlen+3] = '0'; 

  for(int i = 0; i < 9; i++) {
    for(int j = 0; j < 10; j++) {
      for(int k = 0; k < 10; k++) { 
        for (int l = 0; l < 10; l++) {
          //printf("(thousands) buf = %s\n", buf);
          char *crypted = crypting(buf); // crypt
          int same = compare(password, crypted, buf); // compare
          if (same == 1){
            return 1; 
          }
          
          buf[wordlen+3]++; // ones place 

          // hacking - stops the loop early 
          if(buf[wordlen]==':' && buf[wordlen+1]=='0' && buf[wordlen+2]=='0' && buf[wordlen+3]=='0') {
            return 0;
          }
        }
        buf[wordlen+3] = '0'; // ones
        buf[wordlen+2]++; //tens
      }
      buf[wordlen+3]= '0'; // ones
      buf[wordlen+2]= '0';// tens
      buf[wordlen+1]++; // hundreads
    }
    buf[wordlen+3] = '0'; // ones
    buf[wordlen+2]= '0'; // tens
    buf[wordlen+1]= '0';// hundreds
    buf[wordlen]++; // thousands
  }
}

// checks dictionary word for each suffix size
int checkWord(char *password , char *word) {

  if (onesCheck(password, word) == 1)
    return 1;
  if (tensCheck(password, word ) == 1)
    return 1;
  if (hundredsCheck(password, word) == 1)
    return 1;
  if (thousandsCheck(password, word) == 1)
    return 1;
  
  return 0; // false
}

int main () {
  char *word = "abandon";
  //char *testing = "aardvark123";
  //char *password = "$1$ab$koTc2TaVJm9d6HbDymlZO/"; // aardvark123
  char *password = "$1$ab$EN4HdMyFfvk9VYCCEIIJs0"; //abandon9365

  //int res = hundredsCheck(password, word);
  //printf("Result: %d", res);

  checkWord(password, word);

  return 0;
}