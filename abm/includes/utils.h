#ifndef UTILS_H
#define UTILS_H
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#ifdef _WIN32
  #include<windows.h>
#endif  

/**
 * Elimina el carácter de nueva línea al final de una cadena si está presente.
 *
 * @param cad - un puntero a la cadena de caracteres de la cual se eliminará el carácter de nueva línea.
 * @return bool - verdadero si se eliminó el carácter de nueva línea, falso en caso contrario.
 */
bool quitarFinDeLinea(char* cad);

/**
 * Limpia el búfer de entrada estándar hasta encontrar un salto de línea o el final del archivo.
 *
 * @return void - Esta función no devuelve ningún valor.
 */
void limpiarBuffer();

/**
 * Limpia la pantalla de la consola.
 *
 * @return void - Esta función no devuelve ningún valor.
 *
 * Esta función utiliza el comando adecuado para limpiar la pantalla en Windows o en otros sistemas operativos.
 */
void limpiar_pantalla();

/**
 * Pausa la ejecución del programa y espera la entrada del usuario para continuar.
 *
 * Esta función utiliza un comando del sistema para pausar en Windows y solicita al usuario que presione ENTER en otros sistemas operativos.
 */
void pausa();

/**
 * Divide una cadena en subcadenas utilizando un delimitador especificado.
 *
 * @param cadena - un puntero a la cadena que se desea dividir.
 * @param delimitador - un puntero a una cadena que contiene los delimitadores utilizados para dividir la cadena.
 * @return char** - un puntero a un arreglo de punteros a cadenas, que contiene las subcadenas resultantes, finalizando con un puntero NULL.
 */
char** split(char* cadena,char* delimitador);

/**
 * Solicita al usuario un número entero y verifica que la entrada sea válida.
 *
 * @return int - el número entero ingresado por el usuario.
 */
int ingresoEntero();

/**
 * Verifica si una cadena de caracteres representa un número flotante.
 *
 * @param val un puntero a una cadena de caracteres que se evaluará para determinar si es un número flotante.
 * @return bool - verdadero si la cadena contiene caracteres que indican un número flotante, falso en caso contrario.
 */
bool esflotante(char val[]);

/**
 * Calcula la longitud del campo en función de la longitud del nombre y la longitud del campo.
 *
 * @param longitudNombre - la longitud del nombre que se utilizará para calcular el resultado.
 * @param longitudCampo - la longitud del campo que se comparará con la longitud del nombre.
 * @return int - la longitud calculada del campo.
 */
int longitudCampo(int longitudNombre, int longitudCampo);

/**
 * Convierte un número entero en una cadena de caracteres.
 *
 * @param siguiente el número entero que se desea convertir a cadena.
 * @return char* - un puntero a la cadena de caracteres que representa el número entero.
 */
char* intToString(int siguiente);

/**
 * Convierte una cadena de caracteres en un número entero.
 *
 * @param siguiente un puntero a una cadena de caracteres que representa el número entero a convertir.
 * @return int - el número entero correspondiente a la cadena de caracteres.
 */
int parseInt(char* siguiente);

#endif