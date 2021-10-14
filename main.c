
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
 #include<sys/stat.h>

#define ROOT 0
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

  // typedef struct Users {
  //   char *  username;
  //   char *  id;
  //   char *  alg;
  //   char *  pwd;
  // } Users;

  int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm world = MPI_COMM_WORLD;

    char name[MPI_MAX_PROCESSOR_NAME];
    int worldSize, rank, nameLen;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &rank);
    MPI_Get_processor_name(name, &nameLen);
    // MPI_File shadowfh, wordsfh;
    // MPI_Errhandler errorCheck;  // used to ensure the file has opened correctly



    /*
        read and parse the shadow txt file to get the username,id,salt, password

    */
    // FILE * shadowPtr;
    // shadowPtr = fopen ("shadow.txt", "r");
    // char * line;

    // Users shadowUsers; // TODO ask abotu this 

    // while(fscanf(shadowPtr,"%s", line )!=EOF){
      // char * username = strtok(line, ":" );
      // char * alg = strtok(NULL, "$" );
      // char * id = strtok(NULL, "$" );
      // char * pwd = strtok(NULL, "$" );

      // shadowUsers[0].username = username;
      // shadowUsers.alg = alg;
      // shadowUsers.id = id;
      // shadowUsers.pwd = pwd;
      
      // printf("token=%s\n",username);
      // printf("alg=%s\n",alg);
      // printf("id=%s\n",id);
      // printf("id=%s\n",pwd);


    // }
    // fclose(shadowPtr);


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
                printf("sendcnt[%d]=%d\n",index ,sendcnt[index]);

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


    /* ROOT Calculated how much every node needs to read into their local dictionary 
       use scatter to send this amount 
    */

    if (rank != ROOT) {
      sendcnt = malloc(worldSize * sizeof(int));
    }

   MPI_Bcast(sendcnt, worldSize, MPI_INT, ROOT, world);
  //  printf("rank = %d sendcnt= %d\n",rank ,sendcnt[rank]);


   /*
       use lseek to position file pointers and then read into then in a portion of the words.txt into a local dictionary 
   */ 

    int * displc = malloc(worldSize * sizeof(int));
    displc[0] = 0;
    for(int i =1; i < worldSize ; i++){
      displc[i] = sendcnt[i-1] +displc[i-1];
      if(rank ==0)
        printf("i =%d displc= %d\n",i, displc[i]);

    }

    char * localDict = malloc ( sendcnt[rank] * sizeof(char));
    lseek(fd ,displc[rank], SEEK_SET);
    read(fd , localDict ,sendcnt[rank] );

    // if(rank == 4){
    //   printf("rank = %d \nstring= %s END\n",rank ,localDict);
    //   // TODO: why is it reading in like this at the ery end 
    // }


    
  /*
    Parse every words from every nodes local dictionary to crpyt and test ater 
  */


    MPI_Finalize();
    return 0;
  }
