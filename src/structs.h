#ifndef STRUCT_H
#define STRUCT_H

#include <time.h>

typedef struct{
  char* uuid;           // ID
  int algorithm;        // ID algoritmo
  char* param;          // Parametros
  time_t dispatched_at; // Tiempo de despacho para evaluar retry
  char* result;         // Resultado de la query
} Query;

Query* newQuery(char* uuid, int algorithm, char* param); // Crea nueva query
void freeQuery(Query* query); // Libera query
double getQueryAge(Query* query); // Segundos desde que fue despachado, -1 si esta en cola
void setQueryDispatch(Query* query); // Setear query como despachada



#endif