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
  int lfu_index; 
  int lfu_hits = -1; 
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
      lfu_hits = cache->entries[i].hits;
    }

    if(lru_time == -1 || cache->entries[i].last_hit_at < lru_time){
      lru_index = i;
      lru_time = cache->entries[i].last_hit_at;
    }
  }

  i = lfu_index; // cambiar por lfu_index para comparar performance
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
    printf("query:%ld result:%s hits:%i\n",entry.query,entry.result,entry.hits);
  }
  printf("---***END CACHE***---\n");

}