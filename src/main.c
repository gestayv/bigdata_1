#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "io.h"
#include "algorithms.h"

int main(int argc, char** argv) {
  // Initialize the MPI environment. The two arguments to MPI Init are not
  // currently used by MPI implementations, but are there in case future
  // implementations might need the arguments.
  MPI_Init(NULL, NULL);

  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);


  // READER/MASTER procedure
  if(world_rank == 0){
    QueryList* queries;
    queries = readQueries("entrada.in");

    // Generar 3 caches
    Cache* cachePool[3];
    cachePool[0] = newCache(20); // Algoritmo 1: Armstrong
    cachePool[1] = newCache(20); // Algoritmo 2: Euler
    cachePool[2] = newCache(20); // Algoritmo 3: Configuracion electronica

    int i;
    Query* query;
    char* result;
    int noQueriesLeft = 0;


    for(i=1;i<world_size;){
      query = getUnassignedQuery(queries);
      if(query == NULL){
        noQueriesLeft = 1;
        break; // dejar de hacer dispatch
      }
      result = searchQuery(cachePool[query->algorithm-1],query->param);

      // Cache hit
      if(result != NULL){
        printf("Resultado en cache: %ld->%s\n",query->param,result);
        query->result = result;
      }

      // Cache miss
      else{
        printf("Enviando tarea param:%ld worker:%i algo:%i\n",query->param,i,query->algorithm);
        MPI_Send(&query->param,1,MPI_LONG,i,query->algorithm,MPI_COMM_WORLD);
        setQueryDispatch(query);
        i++; // subir contador solo al despachar
      }
    }

    // Recopilar resultados y reasignar tareas
    MPI_Status status;
    int charCount;
    long queryParam;
    while(noQueriesLeft == 0){
      // esperar mensaje
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      // Obtener la cantidad de caracteres de la respuesta
      MPI_Get_count(&status, MPI_CHAR, &charCount);

      // Obtener resultado desde worker
      result = (char*) malloc(charCount*sizeof(char));
      MPI_Recv(
        result, 
        charCount, 
        MPI_CHAR, 
        status.MPI_SOURCE, 
        status.MPI_TAG,
        MPI_COMM_WORLD, 
        MPI_STATUS_IGNORE);

      // Obtener la consulta originar al worker
      MPI_Recv(
        &queryParam, 
        1, // largo de una uuid
        MPI_LONG,
        status.MPI_SOURCE, 
        0, // Manda parametro de la query original por el tag 0
        MPI_COMM_WORLD, 
        MPI_STATUS_IGNORE);

      // Guardar resultado en query
      setQueriesResult(queries, queryParam, result);

      // guardar resultado en cache
      saveResult(cachePool[status.MPI_TAG-1], queryParam, result);
      
      // Despachar la siguiente tarea
      while(noQueriesLeft == 0){ // solo si quedan queries sin respuesta
        query = getUnassignedQuery(queries);
        // terminar si no quedan mas tareas
        if (query == NULL){
          noQueriesLeft = 1;
          break;
        }

        //buscar en cache
        result = searchQuery(cachePool[query->algorithm-1],query->param);
        
        // Cache hit
        if(result != NULL){
          printf("Resultado en cache: %ld->%s\n",query->param,result);
          query->result = result;
        }

        // Cache miss
        else{
          printf("Enviando tarea param:%ld worker:%i algo:%i\n",query->param,i,query->algorithm);
          MPI_Send(&query->param,1,MPI_LONG,status.MPI_SOURCE,query->algorithm,MPI_COMM_WORLD);
          setQueryDispatch(query);
          break; // solo despachar una query a la vez
        }
      }
    }

    printCache(cachePool[0]);
    printCache(cachePool[1]);
    printCache(cachePool[2]);
    //writeResults("salida.out", queries);

    // Parar workers
    for(i=1;i<world_size;i++){
      MPI_Send(&i,1,MPI_INT,i,0,MPI_COMM_WORLD);
    }
  }

  // Workers
  else{ 
    long queryParam;
    char* result;
    int done = 0;
    while(done == 0){
      // Esperar tarea
      MPI_Status status;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      // Recibir mensaje
      MPI_Recv(
        &queryParam, 
        1, 
        MPI_LONG, 
        status.MPI_SOURCE, 
        status.MPI_TAG,
        MPI_COMM_WORLD, 
        MPI_STATUS_IGNORE);
 
      //Resolver tarea
      switch(status.MPI_TAG){
        case 0:
          done = 1;
          break;
        case 1:
          result = armstrong(queryParam);
          break;
        case 2:
          result = euler(queryParam);
          break;
        case 3:
          result = electronConfiguration(queryParam);
          break;
      }

      if(done) break;

      // enviar respuesta
      MPI_Send(result,strlen(result)+1,MPI_CHAR,status.MPI_SOURCE,status.MPI_TAG,MPI_COMM_WORLD);
      // enviar query original
      MPI_Send(&queryParam,1,MPI_LONG,status.MPI_SOURCE,0,MPI_COMM_WORLD);
    }
  }

  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
