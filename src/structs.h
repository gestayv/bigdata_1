#ifndef STRUCT_H
#define STRUCT_H

#include <time.h>

typedef struct{
  char* uuid;           // ID
  int algorithm;        // ID algoritmo
  long param;          // Parametros
  time_t dispatched_at; // Tiempo de despacho para evaluar retry
  char* result;         // Resultado de la query
} Query;

Query* newQuery(char* uuid, int algorithm, long param); // Crea nueva query
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
void printQueries(QueryList* list);


// CACHE
typedef struct{
  int algorithm;
  long query;
  char* result;
  int hits;
  time_t last_hit_at;
}CacheEntry;

typedef struct{
  CacheEntry* entries;
  int size;
  char* eviction;
  long hits;
  long misses;
}Cache;

Cache* newCache(int size, char* eviction);
char* searchQuery(Cache* cache, int algorithm, long query);
Cache* saveResult(Cache* cache, int algorithm, long query, char* result);
void printCachePerformance(Cache* cache);
void printCache(Cache* cache);


#endif