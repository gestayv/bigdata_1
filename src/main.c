// Author: Wes Kendall
// Copyright 2011 www.mpitutorial.com
// This code is provided freely with the tutorials on mpitutorial.com. Feel
// free to modify it for your own use. Any distribution of the code must
// either provide a link to www.mpitutorial.com or keep this header intact.
//
// An intro MPI hello world program that uses MPI_Init, MPI_Comm_size,
// MPI_Comm_rank, MPI_Finalize, and MPI_Get_processor_name.
//
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "io.h"

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
    Query** queries;
    queries = readQueries("Entrada.in");

    // por cada tarea
    /*
      Buscar resultado en cache
    */

    /*
      Enviar tareas a workers si no existe en cache
    */

    /* 
      Recopilar resultados
    */

    writeResults("Salida.out", queries);
  }

  // Workers
  else{ 
    while(1){
      /*
        Esperar tarea
      */

      /* 
        Resolver tarea
      */

      /*
        Enviar respuesta a dispatcher
      */
    }
  }


  // Finalize the MPI environment. No more MPI calls can be made after this
  MPI_Finalize();
}
