#ifndef TABLA_HASH_H
#define TABLA_HASH_H
#define MAX_LEN_FILE 101

#include <stdbool.h>
#include <stdlib.h>
#include"listas.h"
#include"metadata.h"

struct TablaHashRep{
    int (*hash_function)(char*);
    int capacidad;
    char fileName[MAX_LEN_FILE];
    size_t size_data;
    Metadata* metadata;
};
typedef struct TablaHashRep *TablaHash;

/**
 * Crea una nueva tabla hash con un tamaño inicial y una función hash especificada.
 *
 * @param tamano - un valor entero que indica el tamaño inicial de la tabla hash.
 * @param hash_function - un puntero a una función que se utilizará para calcular el hash de las claves.
 * @return TablaHash - una nueva instancia de TablaHash con el tamaño y función hash especificados.
 */
TablaHash th_crear(int tamano, int (*hash_function)(char*), char* fileNamea, size_t size_data, Metadata* metadata);

/**
 * Inserta un TipoElemento en la tabla hash especificada.
 *
 * @param th - la tabla hash donde se insertará el elemento.
 * @param te - un TipoElemento que se insertará en la tabla hash.
 */
bool th_insertar(TablaHash th, void* elemento, char* clave);

/**
 * Elimina un TipoElemento de la tabla hash especificada.
 *
 * @param th - la tabla hash de donde se eliminará el elemento.
 * @param clave - un valor entero que se utilizará como clave del elemento que se eliminará.
 */
bool th_eliminar(TablaHash th, char* clave);

/**
 * Recupera el TipoElemento con la clave especificada de la tabla hash.
 *
 * @param th - la tabla hash donde se buscará el elemento.
 * @param clave - un valor entero que se utilizará como clave del elemento que se buscará.
 * @return TipoElemento - un puntero al TipoElemento con la clave especificada o NULL si no se encuentra.
 */
void* th_recuperar(TablaHash th, char* clave);

/**
 * Reescribe un registro en la tabla hash o en una lista asociada si el registro no está ocupado.
 *
 * @param th - la tabla hash donde se buscará y reescribirá el registro.
 * @param clave - un puntero a la cadena que representa la clave del registro a modificar.
 * @param estructuraModificada - un puntero a la cadena que contiene los nuevos datos del registro.
 * @return bool - verdadero si el registro fue reescrito exitosamente, falso en caso contrario.
 */
bool th_reescribir(TablaHash th, char* clave, char* estrucuturaModificada);

#endif //TABLA_HASH_H