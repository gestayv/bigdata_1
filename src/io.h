#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "structs.h"

QueryList* readQueries(char* filename);
void writeResult(char* filename, QueryList* queries); // agregar resultado 1 a 1 o completo???


#endif // IO_H
