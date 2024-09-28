#ifndef METADATA_H
#define METADATA_H
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"utils.h"
#define MAX_LEN_CAMPO 32
#define MIN_LEN_PK 4
#define MAX_CAPACIDAD_TABLA 1000
#define NRO_PRIMO 997
#define MODO_RW "r+b"
#define MODO_W "w+b"
#define PATH_METADATA "files/meta-";
#define LIBRE "0"
#define OCUPADO "1"
#define BORRADO "2"
#define NULO -1


typedef struct{
    char nombre[MAX_LEN_CAMPO];
    int longitud;
    char pk;
}Campos;

typedef struct{
    int nCampos;
    int longitudRegistro;
    Campos * campos;
}Metadata;

/**
 * Calcula el valor hash de una clave utilizando la suma de los caracteres de la cadena.
 *
 * @param clave - un puntero a la cadena de caracteres cuya clave se desea hashear.
 * @return int - el valor hash resultante de la clave, obtenido como la suma de sus caracteres módulo un número primo.
 */
int fnHash(char* clave);

/**
 * Permite al usuario seleccionar un campo de metadatos como clave primaria.
 *
 * @param metadata - un puntero a la estructura Metadata que contiene los campos disponibles para la selección.
 * @return bool - verdadero si se ha seleccionado una clave primaria válida, falso en caso contrario.
 */
bool seleccionarPK(Metadata* metadata);

/**
 * Busca el índice de la clave primaria en la estructura de metadatos.
 *
 * @param metadata - un puntero a la estructura Metadata que contiene los campos a evaluar.
 * @return int - el índice del campo que se ha definido como clave primaria; devuelve la posición del primer campo encontrado o 0 si no se encuentra ninguna clave primaria.
 */
int buscarPK(Metadata* metadata);

/**
 * Carga la metadata de un archivo, solicita información al usuario sobre los campos del registro,
 * y crea un archivo de metadata que almacena la información de la estructura de datos.
 *
 * @param fileName - un puntero a la cadena que recibirá el nombre del archivo de datos.
 * @param fileNameMetadata - un puntero a la cadena que recibirá el nombre del archivo de metadata.
 * @return void* - un puntero a la tabla hash creada con la metadata cargada, o NULL si hubo un error.
 */
void* cargarMetadata(char* fileName,char* fileMetadata);

/**
 * Lee la metadata de un archivo y crea una tabla hash utilizando la información cargada.
 *
 * @param fileName - un puntero a la cadena que recibirá el nombre del archivo de datos.
 * @param fileNameMetadata - un puntero a la cadena que recibirá el nombre del archivo de metadata.
 * @return void* - un puntero a la tabla hash creada con la metadata leída, o NULL si hubo un error al abrir el archivo.
 */
void* lecturaMetadata(char* fileName,char* fileMetadata);

/**
 * Lee los campos de un registro y los almacena en un arreglo de cadenas.
 *
 * @param metadata - un puntero a la estructura Metadata que define la longitud de cada campo.
 * @param registro - un puntero a la cadena que contiene el registro del cual se extraerán los campos.
 * @return char** - un puntero a un arreglo de cadenas que contiene los campos leídos del registro.
 */
char** lectorDeCampos(Metadata* metadata,char* registro);

/**
 * Crea un registro a partir de un arreglo de campos, concatenando los valores de cada campo en una sola cadena.
 *
 * @param metadata - un puntero a la estructura Metadata que define la longitud de cada campo.
 * @param campos - un puntero a un arreglo de cadenas que contiene los campos que se concatenarán en el registro.
 * @return char* - un puntero a la cadena que representa el registro concatenado de todos los campos.
 */
char* escritorDeCampos(Metadata* metadata, char** campos);

#endif