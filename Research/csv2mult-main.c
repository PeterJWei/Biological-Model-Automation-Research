#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "csv2mult-lib.h"
#include "getline.h"

#define true 1
#define false 0


typedef struct ModelDat
{
  int InputNo;
  int OutputNo;
  char **InputNames;
  char ** OutputNames;
}ModelDat;

int main(int argc, char *argv[])
{
  ModelDat *data;

  int count = 0;
  int diclen = 50;
  char **dictionary = calloc(diclen,(sizeof(char *)));
  int i = 0;
  int k = 0;
  char c;
  int dFlag = 0;
  FILE *outfile;
  for(;k<diclen;k++)
    dictionary[k] = "\0";

  if (argc >1 && argv[1][0] == '-' )
  {
    for(i=1; (c=argv[1][i]) != '\0';i++)
    {
      if(c=='d')
        dFlag++;
    }
    --argc;
    ++argv;
  }
  if (argc <4)
  {
    printf("usage: ./%s <inputNo> <infileName> <outFilename>\n",argv[0]);
    return EXIT_FAILURE;
  }
  printf("loading dictionary\n");
  /* doing as much initialisation of data as possible */
  data = malloc(sizeof(ModelDat));
  data->InputNames = calloc(atoi(argv[1]),sizeof(char *));
  data->InputNo = atoi(argv[1]);
  
  /* if delays are enabled, outputs are 2 */
  data->OutputNo = 1;
  if (dFlag)
    data->OutputNo = 2;
  data->OutputNames = calloc(data->OutputNo,sizeof(char *));
  data->OutputNames[0] = NULL;
  if (dFlag)
  {
    data->OutputNames[1] = malloc(1+strlen("delay"));
    strcpy(data->OutputNames[1],"delay");
  }
    for (i=0;i<data->InputNo;i++)
    data->InputNames[i] = NULL;
  loadFile(argv[2],&dictionary, &diclen, &count, data,dFlag);
  printf("Finished load of %s\n",argv[2]);
  
  /*opening the output file file */  
  outfile = fopen(argv[3],"wt");
  if (NULL == outfile)
  {
    printf("Can't open %s for output.\n",argv[3]);
    exit(EXIT_FAILURE);
  }
  printFile(dictionary, &count, outfile,data);
  freeData(data);
  fclose(outfile);
  for (i=0;i<count;i++)
  {
    free(dictionary[i]);
  }
    free(dictionary);
  printf("success\n");
  return (EXIT_SUCCESS);
}
