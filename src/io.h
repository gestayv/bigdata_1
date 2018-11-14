#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "structs.h"

Query** readQueries(char* filename);
void writeResult(char* filename, Query** queries); // agregar resultado 1 a 1 o completo???


#endif // IO_H
