#ifndef INTERFACE_H
#define INTERFACE_H
#include<stdbool.h>
#include"abm.h"
#include"utils.h"
#include"tablaHash.h"

/**
 * Muestra un menú de opciones para la gestión de archivos y registros.
 * Permite al usuario seleccionar entre diferentes opciones, como crear/cargar archivos,
 * realizar altas, bajas, modificaciones, y mostrar registros o archivos.
 *
 * La función gestiona la entrada del usuario y valida que la opción seleccionada sea válida.
 * Si se selecciona una opción que requiere una tabla hash, verifica que esta no sea NULL
 * antes de proceder. Al finalizar, se ofrece la opción de salir del programa.
 */
void interfaces();

/**
 * Muestra un menú para gestionar la creación y carga de archivos.
 * Permite al usuario elegir entre crear un nuevo archivo, cargar un archivo existente,
 * o salir del menú. La función valida la opción ingresada y asegura que esté dentro
 * del rango permitido.
 *
 * Si se selecciona la opción para crear un archivo nuevo, se llama a `cargarMetadata`
 * para cargar la metadata del archivo. Si se elige la opción para cargar un archivo
 * existente, se llama a `lecturaMetadata`. En ambos casos, si se logra cargar la metadata
 * correctamente, se establece el indicador `creadoCargado` en verdadero.
 *
 * La función devuelve un valor booleano que indica si la operación de carga o creación
 * fue exitosa.
 */
bool menuCarga(char* filename, char* fileNameMetadata, TablaHash* estructura);

#endif