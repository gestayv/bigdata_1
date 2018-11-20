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

// Lista de queries
typedef struct ProtoList{
	Query* data;
	struct ProtoList* next;
}QueryListNode;

typedef struct{
  QueryListNode* first;
  QueryListNode* last;
}QueryList;

QueryList* newQueryList(Query* data);
QueryList* AddQueryToList(QueryList* list, Query* data);
void freeQueryList(QueryList* list);
Query* getUnassignedQuery(QueryList* list);
Query* getQueryByUUID(QueryList* list, char* uuid);
void setQueriesResult(QueryList*, long param, char* result);


// CACHE
typedef struct{
  long query;
  char* result;
  int hits;
  time_t last_hit_at;
}CacheEntry;

typedef struct{
  CacheEntry* entries;
  int size;
}Cache;

Cache* newCache(int size);
char* searchQuery(Cache* cache, long query);
Cache* saveResult(Cache* cache, long query, char* result);

#endif