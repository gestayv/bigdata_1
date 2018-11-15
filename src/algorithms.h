#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// Recibe numero a revisar, returna "true" o "false"
char* armstrong(long n);

// Recibe n, retorna un string con el numero en double
char* euler(long n);

// Recibe el numero atomico de un elemento, retorna un string con la configuracion electronica
char* electronConfiguration(int atomicNumber);

// Utils
int getAtomicNumber(char* element);

#endif