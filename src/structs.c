#include <stdlib.h>
#include "structs.h"

// Crea nueva query
Query* newQuery(char* uuid, int algorithm, char* param){
  Query* query;

  query = (Query*) malloc(sizeof(Query));
  query->uuid = uuid;
  query->algorithm = algorithm;
  query->param = param;
  query->dispatched_at = 0;
  query->result = NULL;

  return query;  
}

// Libera query
void freeQuery(Query* query){
  free(query->uuid);
  free(query->param);
  free(query->result);
  free(query);
  return;
}

// Segundos desde que fue despachado, -1 si esta en cola
double getQueryAge(Query* query){
  if(query->dispatched_at == 0) return -1;
  return difftime(time(NULL),query->dispatched_at);  
}

// Setear query como despachada
void setQueryDispatch(Query* query){
  query->dispatched_at = time(NULL);
}