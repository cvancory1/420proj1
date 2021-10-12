
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
char *bufArr(int *arr, int n) {
  char *buf = malloc(n * (4 + 1) + 1);
  buf[0] = '\0';

  const char *fmt = "%4d ";
  char tmp[4 + 1 + 1];  // same width as fmt plus a null term

  for (int i = 0; i < n; i++) {
    sprintf(tmp, fmt, arr[i]);
    strcat(buf, tmp);
  }

  return buf;
}
  int   checkWord(char* pswd, char* word) {
    // TODO : Kayln hard code the salt and we will take care of that later

    int N = 10;  // delete later
    // int N= 9999;
    for (int i = 1; i <= N; i++) {
      char* prefixWord = malloc(4 * sizeof(char) + sizeof(word));  // check this
      char* suffixWord = malloc(4 * sizeof(char) + sizeof(word));  // check this
      // sprintf(prefixWord,"%d%s", N, word); // takes N + word and puts into
      // string prefixword sprintf(suffixWord,"%s%d", word, N );
      // printf("prefixWord = %s\n", prefixWord);
      // printf("suffixWord = %s\n", suffixWord);

      // TODO : still need to hash the word and compare
      // TODO : dont use sprintf since it is some string maniuplation which is
      // costly

      if (strncmp(prefixWord, pswd, strlen(pswd)) == 0) {
        printf("found word = %s\n", prefixWord);
        return 1;
      } else if (strncmp(prefixWord, pswd, strlen(pswd)) == 0) {
        printf("found word = %s\n", prefixWord);
        return 1;
      }
    }

    return 0;  // dict word was not used in the password
  }

  int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    MPI_Comm world = MPI_COMM_WORLD;

    char name[MPI_MAX_PROCESSOR_NAME];
    int worldSize, rank, nameLen;

    MPI_Comm_size(world, &worldSize);
    MPI_Comm_rank(world, &rank);
    MPI_Get_processor_name(name, &nameLen);
    MPI_File shadowfh, wordsfh;
    MPI_Errhandler errorCheck;  // used to ensure the file has opened correctly

    /*****  READING FROM BOTH FILES- SHADOW + WORDS   **********/

    // ALL NODES - read in the shadow file with username and pswds
    // const char* fname = "shadow.txt";
    const char* fname = "testShadow.txt";
    errorCheck = MPI_File_open(world,            // comm
                               fname,            // filename
                               MPI_MODE_RDONLY,  // mode
                               MPI_INFO_NULL,    // info structure
                               &shadowfh);

    // checks to see if the file exits and opens correctly
    if (errorCheck != 0) {
      if (rank == ROOT) {
        printf("\n%s did not open correctly \n", fname);
      }
      MPI_Finalize();  // all nodes need to call this
      return 1;
    }

    // calc how many bytes the shadowfile is to malloc into a buffer
    MPI_Offset filesize;
    MPI_File_get_size(shadowfh, &filesize);
    char* shadowBuf = malloc(filesize + 1 * sizeof(char));

    MPI_File_read_all(shadowfh, shadowBuf, filesize + 1, MPI_CHAR,
                      MPI_STATUS_IGNORE);
    if (rank == ROOT)
      printf("\n---- PRINTING SHADOW FILE----\n%s\n", shadowBuf);











    /******** ALL NODES - read a porition of the dict words file *****/

    // const char* dict_fname = "words.txt";
    const char* dict_fname = "testWords.txt";
    errorCheck = MPI_File_open(world,            // comm
                               dict_fname,       // filename
                               MPI_MODE_RDONLY,  // mode
                               MPI_INFO_NULL,    // info structure
                               &wordsfh);

    // checks to see if words.txt opened correctly
    if (errorCheck != 0) {
      if (rank == ROOT) {
        printf("\n%s did not open correctly \n\n", dict_fname);
      }
      MPI_Finalize();
      return 1;
    }



    MPI_File_get_size(wordsfh, &filesize);
    // printf("%d \n", filesize/worldSize);

    // char* dictBuff = malloc(filesize + 1 * sizeof(char));
    // MPI_File_read(wordsfh, dictBuff, filesize + 1, MPI_CHAR,
    //               MPI_STATUS_IGNORE);  // TODO Do a read at
    // printf("----PRINTING WORDS FILE---- \n%s\n", dictBuff);

    int WORDCOUNT = 100;  // for the temporary word count
                          //  int WORDCOUNT =235888;
    int * sendcnt = malloc(worldSize * sizeof(int));
    int numBytes;
    int lineCount=0;
    int * offset;
    int buffersize=1;
    char buff[buffersize];
    // int lineCount = 
    // sendcnt = +10; // TODO neds to be in bytes , calc lines then how many bytes are in that number of lines 

      printf("HERE =");

    if (rank == ROOT) {
      int fd=-1;
      fd= open("testWords.txt",O_RDONLY);
      if(fd ==-1 ){
        printf("Error opening the testwords.txt\n");

            while((numBytes=read(fd,buff,buffersize))>0){
              if(buff[0] == '\0'){
                  printf("HERE =");

                lineCount ++;
                if(lineCount == (WORDCOUNT / worldSize)){
                  printf("line count =%d",lineCount);
                  lineCount = 0;
                }
              }

               // printf(" %c",buff[0]);
            }


      }

    }
      

  //   // printf("send=%d\n", sendcnt);
  //   offset = malloc(worldSize * sizeof(int));  // holds the index of where each node
  //                                         // will read in from the file
  //   offset[0] = 0;
  //   for (int i = 1; i < worldSize; i++) {
  //     offset[i] = offset[i - 1] + sendcnt;
  //     // printf("i = %d offset = %d \n", i , offset[i]);
  //   }


  //   char* localDict = malloc(sendcnt * 255 * sizeof(char));
  // // // is read at blocking? 
  //   MPI_File_read_at(wordsfh,       // file handle
  //                    offset[rank],  // file offset is this in bytes ? 
  //                    localDict,     // buffer
  //                    sendcnt,       // elements to read in buffer
  //                    MPI_CHAR,      // MPI data tupe
  //                    MPI_STATUS_IGNORE);

  //   if(rank ==ROOT)
  //     printf("Rank %d received %s\n", rank, localDict);

  //   if(rank == 1)
  //     printf("\nRank %d received %s\n", rank, localDict); 

    // printf("R = %d buf =%s\n", rank, localDict);






























    /***** TESTING THE CHECKWORD FUNCTION WILL BE USED WHERE EVERY NODE CALLS
     * THIS ON THEIR OWN SUB ARRAY **********/

    // char * pswd = malloc(10 *sizeof(char));
    // char * word = malloc(10* sizeof(char));
    // strcpy(pswd, "10test");
    // strcpy(word, "test");
    // checkWord(pswd, word);

    // what is the difference between doing read by c system calls and then the
    // mpi read

    // printf("count=%d\n",count);

    MPI_Finalize();
    return 0;
  }
