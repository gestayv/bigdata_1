#include <stdlib.h>
#include "structs.h"

// QUERY

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
  list->last->next = data;
  list->last = data;
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
    age = getQueryAge(aux->data);

    if(age==-1){
      return aux->data;
    }else if(age > 3 && (candidate == NULL || getQueryAge(candidate) < age)){
      candidate = aux->data;
    }
		aux = aux->next;
	}
  return candidate;
}


// CACHE
Cache* newCache(int size){
  Cache* cache = (Cache*) malloc(sizeof(Cache));
  cache->size = size;
  cache->entries = (CacheEntry*) malloc(size * sizeof(CacheEntry));
  
  int i;
  for (i=0; i<size; i++){
    cache->entries[i].query = -1;
    cache->entries[i].result = NULL;
    cache->entries[i].hits = 0;
    cache->entries[i].last_hit_at = 0;
  }
  return cache;
}

char* searchQuery(Cache* cache, long query){
  if(cache == NULL) return NULL;

  int size = cache->size;

  int i;
  for (i=0; i<size; i++){
    if(cache->entries[i].query == query){
      cache->entries[i].last_hit_at = time(NULL);
      cache->entries[i].hits++;
      return cache->entries[i].result;
    }
  }
  return NULL;
}

Cache* saveResult(Cache* cache, long query, char* result){
  if(cache == NULL) return NULL;

  int size = cache->size;
  int lfu_index, lfu_hits = -1; 
  int lru_index;
  time_t lru_time = -1;

  int i;
  for (i=0; i<size; i++){
    if(cache->entries[i].query == query){
      return cache;
    }
    if(cache->entries[i].query == -1){
      cache->entries[i].query = query;
      cache->entries[i].result = result;
      cache->entries[i].hits = 1;
      cache->entries[i].last_hit_at = time(NULL);
      return cache;
    }

    if(lfu_hits == -1 || cache->entries[i].hits < lfu_hits){
      lfu_index = i;
    }

    if(lru_time == -1 || cache->entries[i].last_hit_at < lru_time){
      lru_index = i;
    }

  }

  i = lru_index; // cambiar por lfu_index para comparar performance
  cache->entries[i].query = query;
  cache->entries[i].result = result;
  cache->entries[i].hits = 1;
  cache->entries[i].last_hit_at = time(NULL);

  return cache;  
}