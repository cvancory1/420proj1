
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

// ********************* NEVER SAYING THE PASSWORD HAS BEEN CRACKED ************************
// checks if hashed word is same as hashed password
// returns 1 if they are the same and 0 if not
int compare(char *password, char *crypted) {
  int ret = 0;
  if (strcmp(password, crypted) == 0) {
        printf("Password has been cracked. The password is: \n");
        for (int i = 0; i < strlen(password); i++)
          printf("%c", password[i]);
        printf("\nThe crypted version is: \n");
        for (int i = 0; i < strlen(crypted); i++)
          printf("%c", crypted[i]);
        ret = 1; // set to 0 since its found
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
    int same = compare(password, crypted); // compare
    if(same == 1) {
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
      int same = compare(password, crypted); // compare
      if(same == 1){
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
        int same = compare(password, crypted); // compare
        if (same == 1){
          return 1; 
        }
        
        buf[wordlen+2]++; // ones place 

        // hacking - stops the loop early 
        if(buf[wordlen+2]==':' && buf[wordlen+1]=='9' && buf[wordlen]=='9') {
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
          int same = compare(password, crypted); // compare
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

// ----- old hundreds calculations ------
    // for (int i = 99; i <= 1000  ; i++) {
    //   printf(" buf = %s\n", buf);
    //   char *crypted = crypting(buf); // crypt
    //   int same = compare(password, crypted); // compare
    //     if(same == 1){
    //       return 1; 
    //     }
      
    //     buf[wordlen]++;
    //     // if (i % 10 == 0) {
          
    //     //   buf[wordlen+1]++; // tens 
    //     //   buf[wordlen+2]= '0' ; // ones 
    //       if(i%100 == 0 ){
    //         buf[wordlen]++; // hundreds 
    //         buf[wordlen+1]= '0'; // tens =0
    //         buf[wordlen+2]= '0'; // ones =0

    //       }
    //     //}
        
    //     // buf[wordlen+2]++;
    //     // else if (i%100 ==  0){
    //         // increment the 100's place 
    //         // set the tens place = 0 
    //         // set the ones place =0
    //     // }
    //   }

// ----- attempted optimization -----
// int checkSuffix(char *password , char *word, int suffixSize ) {
//   int wordlen = strlen(word); // store dict word length
//   int buflen = (suffixSize + wordlen * sizeof(char)); // buffer length = 'ones place + word length
//   char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
//   sprintf(buf, "%s", word); // puts null terminator after inserting word
//   buf[wordlen] = '0'; 
//   if (suffixSize == 1) {
//     for (int i = 0; i < 10; i++) {
//       printf("buf = %s\n", buf);
//       char *crypted = crypting(buf); // crypt
//       int same = compare(password, crypted); // compare
//        if(same == 1){
//           return 1; 
//         }
//       buf[wordlen]++;
//     }
//   } else if (suffixSize == 2) {
//     buf[wordlen] = '0';
//     buf[wordlen+1] = '0'; 
//       for (int i = 10; i <= 99  ; i++) {
//         printf("buf = %s\n", buf);
//         char *crypted = crypting(buf); // crypt
//         int same = compare(password, crypted); // compare
//          if(same == 1){
//           return 1; 
//         }  
//         buf[wordlen+1]++; // += i?
//         if(i%10 ==0){
//           buf[wordlen] = '0';
//           buf[wordlen+1]++;
//         }
//       }
//   } else if (suffixSize == 3) {
//      buf[wordlen] = '0';
//      buf[wordlen+1] = '0'; 
//      buf[wordlen+2] = '0';
//       for (int i = 100; i <= 999  ; i++) {
//         printf("buf = %s\n", buf);
//         char *crypted = crypting(buf); // crypt
//         int same = compare(password, crypted); // compare
//          if(same == 1){
//           return 1; 
//         }    
//         buf[wordlen+2]++;
//       }
//   } else if (suffixSize == 4){
//      buf[wordlen] = '0';
//      buf[wordlen+1] = '0'; 
//      buf[wordlen+2] = '0';
//      buf[wordlen+3] = '0';
//      for (int i = 1000 ;i <= 9999; i++) {
//         printf("buf = %s\n", buf);
//         char *crypted = crypting(buf); // crypt
//         int same = compare(password, crypted); // compare
//         if(same == 1){
//           return 1; 
//         }
//         buf[wordlen+3]++;
//       }
//   }
// }
// // (use string compare, == compares pointers)

// ------ unused for the moment (original function by Kalyn) ------
// int checkWord(char *password , char *word) {
//   int maxLen = 40; // maximum word length
//   int buflen = maxLen * sizeof(word) * sizeof(char); // buffer length = max * array length * char size
//   char *buf = malloc(buflen); // buffer to hold the dictionary word and the prefix/suffix
//   memset(buf, '#', buflen); // fills with null terminators

//   int wordlen = strlen(word); // store dict word length

//   sprintf(buf, "%s", word); // puts null terminator after inserting word

//   puts("");
//   // test printing
//   for (int i = 0; i < buflen; i++){
//     printf("%c", buf[i]);
//   }
//   puts("\n");

//   // ------ CREATING EVERY POSSIBLE SUFFIX ------
//   // TODO: change 4s to 10s...
//   // TODO: figure out how to find suffixes & prefixes that are less than 4 digits
//   // TODO: add prefixes

//   // initalize to 0 so ++ incrementing works properly
//   buf[wordlen] = '0'; 
//   buf[wordlen + 1] = '0'; // dont want to initalize them all in the beginning since aardvark123 & aardvark1230 are different
//   buf[wordlen + 2] = '0';
//   buf[wordlen + 3] = '0';

//   buf[wordlen + 4] = '\0'; // place null terminator after suffix number prior to crypting
//   for (int i = 1; i < 4; i++) {
//     printf("buf = %s\n", buf);
//     char *crypted = crypting(buf); // crypt
//     int same = compare(password, crypted); // compare

//     // no need to keep checking if the password has been found
//     if (same == 0)
//       break;

//     buf[wordlen]++;
//     buf[wordlen + 1] = '0'; 
//     for (int j = 1; j < 4; j++) {
//       printf("buf = %s\n", buf);
//       buf[wordlen + 4] = '\0'; 
//       char *crypted = crypting(buf);
//       int same = compare(password, crypted);

//       if (same == 0)
//         break;

//       buf[wordlen + 1]++; // word1100
//       buf[wordlen + 2] = '0';
//       for (int k = 1; k < 4; k++) {
//         printf("buf = %s\n", buf);
//         buf[wordlen + 4] = '\0'; 
//         char *crypted = crypting(buf); 
//         int same = compare(password, crypted);

//         // no need to keep checking if the password has been found
//         if (same == 0)
//           break;

//         buf[wordlen + 2]++; // word1110
//         buf[wordlen + 3] = '0';
//         for (int l = 1; l < 4; l++) {
//           printf("buf = %s\n", buf);
//           buf[wordlen + 4] = '\0'; // place null terminator after suffix number prior to crypting
//           char *crypted = crypting(buf); // crypt
//           int same = compare(password, crypted); // compare

//           // no need to keep checking if the password has been found
//           if (same == 0)
//             break;

//           buf[wordlen + 3]++; // word1111
//         } // end of word1111
//       } // end of word1110
//     } // end of word1100
//   } // end of word1000
//   return 0;
// }

// checks dictionary word for each suffix size
int checkWord(char *password , char *word) {

  if (onesCheck(password, word) == 1) {
    return 1;
  }

  if (tensCheck(password, word ) == 1) {
    return 1;
  }

  if (hundredsCheck(password, word) == 1) {
    return 1;
  }

  if (thousandsCheck(password, word) == 1) {
    return 1;
  }
  
  return 0; // false
}

int main () {
  char *word = "aardvark";
  char *testing = "aardvark123";
  char *password = "$1$ab$koTc2TaVJm9d6HbDym1ZO"; // aardvark123

  // testing purposes:
  // char *crypted = crypting(password);
  // printf("Crypted word: ");
  // for (int i = 0; i < strlen(crypted); i++)
  //   printf("%c", crypted[i]);
  
  checkWord(password, word);

  return 0;
}