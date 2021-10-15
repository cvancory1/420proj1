
/*
Chloe VanCory and Kalyn Howes
420 Project 1
10.17.21
*/

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


 #include <fcntl.h>
 #include <ctype.h>
 #include <sys/types.h>
 #include <sys/stat.h>

#define ROOT 0
#include "testingCheckWord.c"
// #define WORDCOUNT 235888 TODO : change later
// char *bufArr(int *arr, int n) {
//   char *buf = malloc(n * (4 + 1) + 1);
//   buf[0] = '\0';

//   const char *fmt = "%4d ";
//   char tmp[4 + 1 + 1];  // same width as fmt plus a null term

//   for (int i = 0; i < n; i++) {
//     sprintf(tmp, fmt, arr[i]);
//     strcat(buf, tmp);
//   }

//   return buf;
// }





  typedef struct Users {
    char *  username;
    char *  id;
    char *  alg;
    char *  pwd;
  } Users;

  int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm world = MPI_COMM_WORLD;

    char name[MPI_MAX_PROCESSOR_NAME];
    int worldSize, rank, nameLen;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &rank);
    MPI_Get_processor_name(name, &nameLen);

    /*
        read and parse the shadow txt file to get the username,id,salt, password

    */

  //  malloc the actual pointers then malloc each of hte arrays 
    FILE * shadowPtr;
    shadowPtr = fopen ("testShadow.txt", "r");
    char * line= malloc(255* sizeof(char));
    int numUsers = 11;
    Users shadowUsers[numUsers]; // 48 bytes 
    int index = 0;
 
      // printf("here=\n");
    // if(rank ==ROOT){
    //   while(fscanf(shadowPtr,"%s", line )!=EOF){
    //   // printf("here1=\n");

      // printf("line=%s\n",line);
      // char * username = strtok(line, ":" );
      // char * alg = strtok(NULL, "$" );
      // char * id = strtok(NULL, "$" );
      // char * pwd = strtok(NULL, "$" );

      // shadowUsers[index].username = username;
      // shadowUsers[index].alg = alg;
      // shadowUsers[index].id = id;
      // shadowUsers[index].pwd = pwd;

    //   shadowUsers[index].username = strtok(line, ":" );
    //   printf("line=%s\n",line);
    //   printf("shadowUsers[index].username=%s\n",shadowUsers[index].username);


    //   shadowUsers[index].alg  = strtok(NULL, "$" );
    //   shadowUsers[index].id = strtok(NULL, "$" );
    //   shadowUsers[index].pwd  = strtok(NULL, "$" );
    
    //   index++;
      
    //   // printf("username=%s\n",username);
    //   // printf("alg=%s\n",alg);
    //   // printf("id=%s\n",id);
    //   // printf("pwd=%s\n\n",pwd);


    //   }
      
    // }
    
    // fclose(shadowPtr);
    // printf("checkpt 1 ");

    
    /* ERROR CHECKING */
    // if(rank == ROOT){
    //   for(int i =0; i < numUsers ;i++){
    //   puts("-------");

    //     printf("i= %d username=%s\n",i, shadowUsers[i].username);
    //     printf("alg=%s\n", shadowUsers[i].alg);
    //     printf("id=%s\n", shadowUsers[i].id);
    //     printf("pwd=%s\n\n", shadowUsers[i].pwd);

    //   }

    // }



    /*  
      READING THE WORDS.TXT file 
    */


    int * sendcnt;
    if(rank==ROOT){
     sendcnt = malloc(worldSize * sizeof(int));
       for(int i =0; i< worldSize ;i++){
         sendcnt[i] =0;
        // printf("sendcnt[%d]=%d\n",i ,sendcnt[i]);
      }

    }

    int WORDCOUNT =100;
    // int WORDCOUNT = 235888;
    int * offset;
    int fd;
    fd= open("testWords.txt",O_RDONLY);
    if (rank == ROOT) {

      int numBytes;
      int index =0;
      int lineCount=0;
      int buffersize=1;
      char buff[buffersize];

      if(fd !=-1 ){
            while((numBytes=read(fd,buff,buffersize))>0){
                // printf("%c",buff[0]);
                sendcnt[index]++;
              if(buff[0] == '\n'){
                lineCount ++;
                // printf("linecount= =%d\n",lineCount);

                if(lineCount == (WORDCOUNT / worldSize)){
                  // printf("here - line count =%d\n",lineCount);
                // printf("sendcnt[%d]=%d\n",index ,sendcnt[index]);

                  lineCount = 0;
                  index++;

                }
              }
               // printf(" %c",buff[0]);
            }

      }else{
        printf("Error opening the testwords.txt\n");
      }
    }

    // printf("checkpt 2\n ");

    /* ROOT Calculated how much every node needs to read into their local dictionary 
       use scatter to send this amount 
    */

    if (rank != ROOT) {
      sendcnt = malloc(worldSize * sizeof(int));
    }

   MPI_Bcast(sendcnt, worldSize, MPI_INT, ROOT, world);
  //  printf("rank = %d sendcnt= %d\n",rank ,sendcnt[rank]);




  /*
      calcs the displacement for each proccessor to lseek ( move file pointer ) to a specific place
      in the file
  */

    int * displc = malloc(worldSize * sizeof(int));
    displc[0] = 0;
    for(int i =1; i < worldSize ; i++){
      displc[i] = sendcnt[i-1] + displc[i-1];
      // if(rank ==0)
        // printf("i =%d displc= %d\n",i, displc[i]);

    }



    // printf("checkpt 3 ");

  //  /*
  //      use lseek to position file pointers and then read into then in a portion of the words.txt into a local dictionary 
  //  */ 
    char * localDict = malloc ( sendcnt[rank]+1 * sizeof(char));
    lseek(fd ,displc[rank], SEEK_SET);
    int numRead = read(fd , localDict ,sendcnt[rank] );
    // localDict[strlen(localDict)]= 0; // places the NULL term @ the end
    // printf("HERE rank = %d \nstring= %sEND\n",rank ,localDict);




    //  ERROR CHECKING
    // if(rank == 3){
      // printf("rank = %d \nstring= %sEND\n",rank ,localDict);
    // printf("Rank = %d numRead = %d   MALLOC  = %d  strlen(localDict)  =%lu\n",rank, numRead ,sendcnt[rank] , strlen(localDict ) );

    // }

    // if(rank == ROOT){
    //     for(int i =0; i < worldSize ;i++){
    //   // printf("rank = %d \nstring= %c%c%c END\n",rank ,localDict[displc[i-1]], localDict[displc[i]] , localDict[displc[i+1]]);
    //     }
      
    // }
  //   
   


  // printf("checkpt 4 ");

  // /*
  //   set up "shared array " that will indicate whether we have found a users password or not
  // */

  int * usrPwd = malloc(numUsers * sizeof(int)); 
  for(int i=0; i< numUsers ;i++){
    usrPwd[i] = 0;
  }
  int pswdIndex = 0; // index of the current users paswds all nodes are trying to find 



  // printf("checkpt 5 ");
  
  // /*
  //  *ALL NODES - Parse every words from the nodes local dictionary to crpyt and test ater 
  // * Do this for every username we have

  // */

    fscanf(shadowPtr,"%s", line );
    char * username = strtok(line, ":" );
    char * alg = strtok(NULL, "$" );
    char * salt = strtok(NULL, "\n" );
    // char * pwd = strtok(NULL, "$" );
      printf("salt=%d\n", salt);
      printf("line=%d\n", line);




  // for(int i =0 ;i<numUsers ;i++){
    /* ALL NODES - parse their first word in the localdict */ 
      // int check;
      // char * currentWord = strtok(localDict, "\n" );
      // if(rank ==ROOT) printf("rank = %d currentWord = %s pass=%s  pswdIndex =%d \n", rank, currentWord, pwd, pswdIndex);

      // if(rank ==ROOT){
      // check = checkWord(salt , currentWord);
      // printf("IN MAIN");

      // }
      // printf("RANK = %d BACK IN MAIN\n", rank);
      // if( check == 1 ){
      //   usrPwd[pswdIndex]=1; // this node found a password
      // }

      int wordCounter = 1; // how many words of the local dict we have checked
      // while(currentWord != NULL ){
      // while(currentWord != NULL && usrPwd[pswdIndex] == 0 ){
      //   // if(rank == ROOT){
      // //   //   printf("rank = %d currentWord = %s\n", rank, currentWord);
      //   currentWord = strtok(NULL, "\n" );
      //   if( currentWord !=NULL ){
      //     check = checkWord(pwd , currentWord);
      //   }
      //   // }
      
      // //   // printf("rank = %d currentWord = %s\n", rank, currentWord);

      //   if( check == 1 ){
      //     // usrPwd[pswdIndex]= 1; // this node found a password
      //      printf("rank = %d FOUND = %s\n", rank);


      //   }

  //       wordCounter++;
          
       
  //       if(wordCounter == 2000){
        
  //         wordCounter = 0; 
  //         MPI_Allreduce(
  //             &check,       // send buffer, an array
  //             &usrPwd[pswdIndex],       // recv buffer
  //             1,          // count
  //             MPI_INT,    // datatype
  //             MPI_MAX,    // operation handle
  //             world       // comm
  //         );
  //           //DO Reduce all - then all other nodes will see that they have a 1 there 

  //         if( usrPwd[pswdIndex] == 1 ){
  //           pswdIndex++;
  //         }
  //       }

      // }
        
      // }
  // }

  // printf("FINAL");
    // close(fd);
  // fclose(pswdfd);
  MPI_Finalize();
  return 0;
}
