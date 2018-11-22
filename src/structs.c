#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"

// QUERY

// Crea nueva query
Query* newQuery(char* uuid, int algorithm, long param){
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

// QUERYLIST

QueryList* newQueryList(Query* data){
  QueryList* list = (QueryList*)malloc(sizeof(QueryList));
  QueryListNode* node = (QueryListNode*)malloc(sizeof(QueryListNode));
	node->data = data;
	node->next = NULL;
  list->first = node;
  list->last = node;
	return list;
}

QueryList* AddQueryToList(QueryList* list, Query* data){
  QueryListNode* node = (QueryListNode*)malloc(sizeof(QueryListNode));
	node->data = data;
	node->next = NULL;
  list->last->next = node;
  list->last = node;
  return list;
}

void freeQueryList(QueryList* list){
  QueryListNode *aux, *next;
	if(list == NULL)return;
	aux = list->first;
	while(aux != NULL){
		next = aux->next;
		freeQuery(aux->data);
    free(aux);
		aux = next;
	}
	free(list);
}

Query* getUnassignedQuery(QueryList* list){
  QueryListNode *aux;
  Query *candidate = NULL;
  double age;
	if(list == NULL) return NULL;

	aux = list->first;
	while(aux != NULL){
    if(aux->data->result != NULL){
      aux = aux->next;
      continue;
    }

    age = getQueryAge(aux->data);

    if(age==-1){
      return aux->data;
    }else if(candidate == NULL || getQueryAge(candidate) < age){
      candidate = aux->data;
    }
		aux = aux->next;
	}
  return candidate;
}


Query* getQueryByUUID(QueryList* list, char* uuid){
  QueryListNode *aux;
	if(list == NULL) return NULL;

	aux = list->first;
	while(aux != NULL){
    if( strcmp(aux->data->uuid, uuid) == 0 ){
      return aux->data;
    }
		aux = aux->next;
	}
  return NULL;
}

void setQueriesResult(QueryList* list, long param, char* result){
  QueryListNode *aux;
	if(list == NULL) return;

	aux = list->first;
	while(aux != NULL){
    if( aux->data->param == param && aux->data->result == NULL){
      aux->data->result = result;
      return;
    }
		aux = aux->next;
	}
  return;
}

void printQueries(QueryList* list){
  QueryListNode *aux;
	if(list == NULL) return;

	aux = list->first;
	while(aux != NULL){
    printf("%s %i %ld %s\n",
    aux->data->uuid,
    aux->data->algorithm,
    aux->data->param,
    aux->data->result);

    aux=aux->next;
	}
  return;
}


// CACHE
Cache* newCache(int size, char* eviction){
  Cache* cache = (Cache*) malloc(sizeof(Cache));
  cache->size = size;
  cache->eviction = eviction;
  cache->entries = (CacheEntry*) malloc(size * sizeof(CacheEntry));
  cache->hits = 0l;
  cache->misses = 0l;
  
  int i;
  for (i=0; i<size; i++){
    cache->entries[i].algorithm = -1;
    cache->entries[i].query = -1;
    cache->entries[i].result = NULL;
    cache->entries[i].hits = 0;
    cache->entries[i].last_hit_at = 0;
  }
  return cache;
}

char* searchQuery(Cache* cache, int algorithm, long query){
  if(cache == NULL) return NULL;

  int size = cache->size;
  CacheEntry entry;

  int i;
  for (i=0; i<size; i++){
    entry = cache->entries[i]; 
    if(entry.algorithm == algorithm && entry.query == query){
      entry.last_hit_at = time(NULL);
      entry.hits++;
      cache->hits++;
      return entry.result;
    }
  }
  cache->misses++;
  return NULL;
}

Cache* saveResult(Cache* cache, int algorithm, long query, char* result){
  if(cache == NULL) return NULL;

  int size = cache->size;
  int lfu_index; 
  int lfu_hits = -1; 
  int lru_index;
  time_t lru_time = -1;
  int mru_index;
  time_t mru_time = -1;

  CacheEntry entry;
  int i;
  for (i=0; i<size; i++){
    entry = cache->entries[i];
    if(entry.algorithm == algorithm && entry.query == query){
      return cache;
    }
    if(entry.query == -1){
      cache->entries[i].algorithm = algorithm;
      cache->entries[i].query = query;
      cache->entries[i].result = result;
      cache->entries[i].hits = 1;
      cache->entries[i].last_hit_at = time(NULL);
      return cache;
    }

    if(lfu_hits == -1 || entry.hits < lfu_hits){
      lfu_index = i;
      lfu_hits = entry.hits;
    }

    if(lru_time == -1 || entry.last_hit_at < lru_time){
      lru_index = i;
      lru_time = entry.last_hit_at;
    }

    if(mru_time == -1 || entry.last_hit_at > mru_time){
      mru_index = i;
      mru_time = entry.last_hit_at;
    }
  }

  char* eviction = cache->eviction;
  if(!strcmp(eviction, "lfu")){
    i = lfu_index;
  }else if(!strcmp(eviction, "lru")){
    i = lru_index;
  }else if(!strcmp(eviction, "mru")){
    i = mru_index;
  }else{
    i = lru_index;
  }

  cache->entries[i].algorithm = algorithm;
  cache->entries[i].query = query;
  cache->entries[i].result = result;
  cache->entries[i].hits = 1;
  cache->entries[i].last_hit_at = time(NULL);

  return cache;  
}

void printCache(Cache* cache){
  int size = cache->size;
  
  int i;
  CacheEntry entry;
  printf("---***CACHE***---\n");
  for(i=0;i<size;i++){
    entry = cache->entries[i];
    printf("alg:%i query:%ld result:%s hits:%i\n",entry.algorithm,entry.query,entry.result,entry.hits);
  }
  long total = cache->hits+cache->misses;
  printf("Politica:%s Hits:%ld Total:%ld PERFORMANCE:%lf\n",cache->eviction,cache->hits,total,((double)cache->hits)/total);
  printf("---***END CACHE***---\n");
}

void printCachePerformance(Cache* cache){
  long total = cache->hits+cache->misses;
  printf("Politica:%s Hits:%ld Total:%ld PERFORMANCE:%lf\%\n",cache->eviction,cache->hits,total,((double)cache->hits*100)/total);
}