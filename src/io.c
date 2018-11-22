#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "structs.h"
#include "utils.h"

QueryList* readQueries(char* filename){
  FILE* file;
  char* uuid; // 37?
  char* rawParam; // long 12 chars
  int algorithm;

  file = fopen(filename,"r");

  if (file==NULL){
    printf( "No se puede abrir el fichero.\n" );
    return NULL;
  }

  QueryList* list = NULL;
  Query* query;
  long param;
  rawParam = (char*) malloc(12*sizeof(char));
  while(!feof(file)){
    uuid = (char*) malloc(37*sizeof(char));
    fscanf(file,"%36s,%i,%s\n",uuid,&algorithm,rawParam);

    if(algorithm == 3){
      param = getAtomicNumber(rawParam);
    }else{
      param = getLongFromString(rawParam);
    }

    query = newQuery(uuid,algorithm,param);
    
    if(list == NULL){
      list = newQueryList(query);
    }else{
      list = AddQueryToList(list, query);
    }
  }

  free(rawParam);
  fclose(file);
  return list;
}

void writeResults(char* filename, QueryList* queries){ 
  FILE* file = fopen(filename,"w");
  QueryListNode *aux;
	if(queries == NULL) return;

  char* param = (char*) malloc(12*sizeof(char));
	aux = queries->first;
	while(aux != NULL){
    if(aux->data->algorithm == 3){
      param = getElement(aux->data->param);
    }else{
      sprintf(param, "%ld", aux->data->param);
    }

    fprintf(file,"UUID:%s ALGORITHM:%i PARAM:%s RESULT:%s\n",
    aux->data->uuid,
    aux->data->algorithm,
    param,
    aux->data->result);

    aux = aux->next;
	}

  fclose(file);
  return;
}