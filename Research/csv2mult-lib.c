/*csv2mult-lib.c*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "csv2mult-lib.h"
#include "getline.h"
#include <string.h>

typedef struct ModelDat
{
  int InputNo;
  int OutputNo;
  char ** InputNames;
  char ** OutputNames;
}ModelDat;

char * convert(char * input, char * change)
{
  char helper[255];
  strcpy(helper,input);
  free(input);
  input = malloc(strlen(helper)+3);
  strcpy(input,helper);
  strcpy(helper,change);
  strcat(input,helper);
  return input;
}

/* takes the file, takes out the number of infiles, number of outfiles
 name of infiles, name of outfiles, and all the information*/
void  loadFile( char * infileName, char ***dictionary, int *diclen,int*count,void *incoming,int dFlag)
{

  FILE *infile;
  char *buffer;
  ModelDat *data = incoming;
  int i=0;
  infile = fopen(infileName,"rt");
if (NULL == infile)
{
printf("Can't open %s for output.\n", infileName);
exit(EXIT_FAILURE);
}
printf("opened file\n");
/*initialise inputNo and output No */
while (fgetline_123(&buffer,infile))
{ 
  parse(dictionary, buffer, count,diclen,data,dFlag);
}
fclose(infile);
printf("loaded file\n");
printData(data);

for (i=0;i<(*count);i++)
{
  printf("%s\n",(*dictionary)[i]);

}
data = workData(dictionary,data,count); 
}

void freeData(void *data)
{
  ModelDat *m = data;
  int i = 0;
  int j = 0;
  printf("freeing data\n");
  for (;i<m->InputNo;i++)
    free((m->InputNames)[i]);
  for (;j<m->OutputNo;j++)
    free((m->OutputNames)[j]);
  free(m->InputNames);
  free(m->OutputNames);
}

void printData(void * data)
{
  ModelDat *m = (ModelDat*)data;
  int i = 0;
  int j =0;
  printf("printing data...\n");
  printf("Inputs = %d Outputs =%d \n", m->InputNo,m->OutputNo);
  for (;i<(m->InputNo);i++)
    printf("%s ",m->InputNames[i]);
  printf("\n");
  for(;j<(m->OutputNo);j++)
    printf("%s ",m->OutputNames[j]);
  printf("\n");
}


/* parses each line and extracts stuff like inputnames, outputnames,
 * dictionary values for output values */
int parse(char ***dictionary, char *word, int * count,int*diclen,void*data,int dFlag)
{
  ModelDat *m = data;
	char * token;
  int counter;
  counter = 0;
  token = strtok(word,",");
  /* first input is the output name, the rest are the input names */
  while (token != NULL)
  {
    counter+=1;
    /*test if the thing taken is a 0, 1 or 2, or -, if it is it is stored in
     * dictionary, test if the thing taken is a 0d,1d or 2d*/
    if ((!strcmp(token,"0")) || (!strcmp(token,"1")) || (!strcmp(token,"-")))
    {
      if (counter != 1)
      insertToDictionary(dictionary,token,count,diclen,dFlag);

    }
    else if(!strcmp(token,"2") || !strcmp(token,"0d") || !strcmp(token,"1d") || !strcmp(token,"2d") || !strcmp(token,"0dd") || !strcmp(token,"1dd") || !strcmp(token,"2dd"))
    {
      if (counter != 1)
      insertToDictionary(dictionary,token,count,diclen,dFlag);
    }
    else
      /*if not, it is placed either in output or input*/
    {
      char * helper;
      helper = malloc(strlen(token)+1);
      strcpy(helper,token);
      /*if outputname not found yet, we have an output!*/
      if (m->OutputNames[0] == NULL)
      {
      m->OutputNames[0] = malloc(strlen(helper)+1);
      strcpy(m->OutputNames[0],helper);
      }
      /*else if inputname not full, we have input!*/
      else
      {
        int ii =0;
        for (;ii<m->InputNo;ii++)
        {
          if(m->InputNames[ii] == NULL)
          {
            m->InputNames[ii] = malloc(strlen(helper)+1);
            strcpy(m->InputNames[ii],helper);
            break;
          }
        }
        if (ii == m->InputNo-1)
        {free(helper);
         return 1;
        }
      }
    free(helper);
    }
    token = strtok(NULL,",");
}
  return 1;
}


/* takes the 0,1,2,0d,1d,2d and makes it into a 0,1,2,etc to be put in the
 * dictionary. */
void insertToDictionary(char *** dictionary, char * token, int *count, int *diclen,int dFlag)
{
  int j = 0;
  int helperInt = 0;
  char c;
  char helper[10];
  /* doubling dictionary if not big enough */
      if(*count == *diclen)
      {
        char ** buffer;
        *diclen = *diclen*2;
        buffer = malloc((*diclen)*(sizeof(char*)));
        for(;j<=*count;j++)
          buffer[j] = (*dictionary)[j];
        free(*dictionary);
        *dictionary = buffer;
      }
  if (dFlag)
  {
    c = token[0];
    if (c == '-')
      sprintf(helper,"--");
    else{
      helperInt = strlen(token)-1;
      sprintf(helper,"%c%d",c,helperInt);
    }
    token = helper;
  }
  (*dictionary)[(*count)] = malloc(strlen(token)+1);
  strcpy((*dictionary)[(*count)],token);
  (*count)++; 
}

/* converts double data into new data.  */
void * workData(char ***dictionary,void *data,int *count)
{
  ModelDat *m = data;
  int marker =0;
  int i;
  char **buf;
  char *final;
  int *mod;
  int power=1;
  printf("working data\n");
  mod = malloc(m->InputNo*sizeof(int));
  buf = malloc(m->InputNo*sizeof(char *));
  printf("count  = %d\n",*count);
  /* goes through all the markers, and changes the numbers to output */
  
  for(;marker<(*count);marker++)
  {
    power = 1;
    for (i=0;i<m->InputNo;i++)
    {
      if (i!= 0)
        power = power*3;
    mod[i] =(marker/power)%3;
    if (mod[i] ==1)
      buf[i] = "1";
    if (mod[i] == 2)
      buf[i] = "2";
    if (mod[i] ==0)
      buf[i] = "0";
  }
    final = malloc(m->InputNo + strlen(" \t") + strlen((*dictionary)[marker]) + 1);
    strcpy(final,"\0");
    for (i=m->InputNo-1;i>-1;i--)
    {
      strcat(final,buf[i]);
    }
    strcat(final," \t");
    strcat(final,(*dictionary)[marker]);
    printf("%s\n",final);
    free((*dictionary)[marker]);
    (*dictionary)[marker] = final;
  }
  free(mod);
  free(buf);
  return m;
}

void printFile( char **dictionary, int *count, FILE *stream,void *data)
{
  ModelDat *m = data;
  int i;
  int marker = 0;
  fprintf(stream,".lib");
  for (i=0;i<m->InputNo;i++)
    fprintf(stream," %s",m->InputNames[i]);
  fprintf(stream,"\n");
  fprintf(stream,".i %d\n",m->InputNo);
  fprintf(stream,".o %d\n",m->OutputNo);
  fprintf(stream,".ob");
  for (i=0;i<m->OutputNo;i++)
    fprintf(stream," %s",m->OutputNames[i]);
  fprintf(stream,"\n");
  fprintf(stream,".p %d\n",*count);
  while (marker < *count)
  {
    fprintf(stream,"%s\n",dictionary[marker]);
    marker ++;
  }
  fprintf(stream,".e\n");
}
