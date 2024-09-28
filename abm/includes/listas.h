#ifndef LISTAS_H
#define LISTAS_H

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include"metadata.h"

struct ListaRep;
typedef struct ListaRep *Lista;

struct IteradorRep;
typedef struct IteradorRep *Iterador;

/**
 * Crea una nueva lista y la inicializa con los parámetros proporcionados.
 *
 * @param fileName - un puntero a la cadena que contiene el nombre del archivo asociado a la lista.
 * @param sizeData - el tamaño de los datos que contendrá cada elemento de la lista.
 * @param inicio - el índice del primer elemento en la lista.
 * @param metadata - un puntero a la estructura Metadata que define la estructura de los datos en la lista.
 * @return Lista - un puntero a la nueva lista creada.
 */
Lista l_crear(char* fileName,size_t sizeData, int inicio, Metadata* metadata);

/**
 * Verifica si la lista está vacía.
 *
 * @param lista - un puntero a la lista que se va a verificar.
 * @return bool - devuelve true si la lista está vacía (su inicio es NULO), de lo contrario devuelve false.
 */
bool l_es_vacia(Lista lista);

/**
 * Verifica si la lista está llena.
 *
 * @param lista - un puntero a la lista que se va a verificar.
 * @return bool - devuelve true si la lista está llena, de lo contrario devuelve false.
 */
bool l_es_llena(Lista lista);

/**
 * Calcula la longitud de la lista, es decir, cuenta el número de registros ocupados.
 *
 * @param lista - un puntero a la lista de la cual se quiere conocer la longitud.
 * @return int - devuelve la cantidad de registros ocupados en la lista, o -1 si no se pudo abrir el archivo.
 */
int l_longitud(Lista lista);

/**
 * Agrega un nuevo elemento a la lista, reutilizando registros eliminados si es posible.
 *
 * @param lista - un puntero a la lista donde se desea agregar el elemento.
 * @param elemento - un puntero al elemento que se desea agregar a la lista.
 * @return bool - devuelve true si el elemento fue agregado exitosamente, de lo contrario, devuelve false.
 */
bool l_agregar(Lista lista, void* elemento);

/**
 * Elimina lógicamente un elemento de la lista, marcándolo como borrado.
 *
 * @param lista - un puntero a la lista de la cual se desea eliminar el elemento.
 * @param clave - una cadena que representa la clave del elemento que se desea eliminar.
 * @return bool - devuelve true si el elemento fue eliminado exitosamente, de lo contrario, devuelve false.
 */
bool l_borrar(Lista lista, char* clave);

/**
 * Busca un elemento en la lista utilizando la clave proporcionada.
 *
 * @param lista - un puntero a la lista en la que se desea buscar el elemento.
 * @param clave - una cadena que representa la clave del elemento que se busca.
 * @return void* - devuelve un puntero al registro encontrado si se encuentra, o NULL si no se encuentra o si la lista está vacía.
 */
void* l_buscar(Lista lista, char* clave);

//No Usada
bool l_insertar(Lista lista, void* elemento, int pos);

//No Usada
bool l_eliminar(Lista lista, int pos);

/**
 * Recupera un registro de la lista en la posición especificada.
 *
 * @param lista - un puntero a la lista de la que se desea recuperar el registro.
 * @param pos - un entero que representa la posición del registro que se desea recuperar (1 para el primer registro).
 * @return void* - devuelve un puntero al registro encontrado si se encuentra, o NULL si no se encuentra o si la lista está vacía.
 */
void* l_recuperar(Lista lista, int pos);

//No Usada
void l_mostrar(Lista lista);

// Funciones de iterador de la lista
//No Usada
Iterador iterador(Lista lista);

//No Usada
bool hay_siguiente(Iterador iterador);

//No Usada
void* siguiente(Iterador iterador);

#endif // LISTAS_H