#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "algorithms.h"

char* placeholder1 = "placeholder1";
char* placeholder2 = "placeholder2";
char* placeholder3 = "placeholder3";

// Recibe numero (n) a revisar, returna "true" o "false"
char* armstrong(long n){
  // return placeholder1;
  char* armstrongTrue = "true";
  char* armstrongFalse = "false";
  // Si el número es mayor o igual a 0.
  if(n >= 0)
  {
    // Se crean variables y se inicializan para obtener nro de digitos de n
    long limiteLoop  = 1;
    long modulo      = 10;
    long divisor     = 10;
    long digitos     = 0;
    long result      = 0;

    // Se obtiene la cantidad de dígitos del número a ser verificado
    while(limiteLoop != 0)
    {
      limiteLoop = n/divisor;
      divisor   *= 10;
      digitos++;
    }

    //  A las variables se les asignan valores para verificar si n es número de armstrong o no
    divisor     = 1;
    limiteLoop  = digitos;

    // Se calcula si es armstrong o no, ejecutando el loop el mismo número de veces que los dígitos
    // de "n".
    do
    {
      result  = result + pow((n%modulo)/divisor, digitos);
      divisor = divisor * 10;
      modulo  = modulo * 10;
      limiteLoop --;
    }while(limiteLoop != 0);

    //  Si el resultado es igual al número de entrada, es número de Armstrong
    if(result == n)
    {
      return armstrongTrue;
    }
    return armstrongFalse;
  }
  return armstrongFalse;
}

// Recibe n, retorna un string con el numero en double
char* euler(long n){
    // return placeholder2;
    char *eulerChar = malloc(sizeof(char)*50);
    double euler    = 0;
    euler           = pow(1.0+1.0/n, n);
    sprintf(eulerChar, "%.30f", euler);
    return eulerChar;
}

//  Función empleada para determinar el numero de electrones por nivel de energia.
int energyLevel(char level)
{
    switch(level)
    {
        case 's':
            return 2;
        case 'p':
            return 6;
        case 'd':
            return 10;
        case 'f':
            return 14;
    }
}

// Recibe el numero atomico de un elemento, retorna un string con la configuracion electronica
char* electronConfiguration(int atomicNumber){
  // return placeholder3;
  int table[4]    = {'s', 'p', 'd', 'f'};

  int i           = 0;                        //  Índice para recorrer table
  int aux         = 0;                        //  Variable para recorrer table desde la posición i a 0.
  int level       = 0;                        //  Variable empleada para repetir cada iteración de aux 2 veces.
  int counter     = 0;                        //  Variable empleada para indicar el número del nivel de energía  
  int auxElem     = atomicNumber;             //  Copia del número de electrones de un elemento químico.
  char *result    = malloc(sizeof(char)*300); //  Arreglo de caracteres que contiene el resultado final.
  char auxResult[10];                         //  Arreglo parcial que guarda un nivel de energía (ej: 1s2, 4f11)

  for(i = 0; i < 4; i++)
  {
    level   = 0;
    counter = i+1;
    while(level < 2)
    {
      aux = i;
      while(aux >= 0)
      {
        if(auxElem > energyLevel(table[aux]))
        {
          sprintf(auxResult, "%d%c%d ",  counter, table[aux], energyLevel(table[aux]));
          strcat(result, auxResult);
          auxElem -= energyLevel(table[aux]);
        }
        else if(auxElem == energyLevel(table[aux]))
        {
          sprintf(auxResult, "%d%c%d ",  counter, table[aux], energyLevel(table[aux]));
          strcat(result, auxResult);
          return result;
        }
        else
        {
          sprintf(auxResult, "%d%c%d ",  counter, table[aux], auxElem);
          strcat(result, auxResult);
          return result;
        }
        aux--;
        counter++;
      }
      level++;
      counter = i+2;
    }
  }
}
