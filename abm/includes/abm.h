#ifndef ABM_H
#define ABM_H
#define MAX_LEN_FILE 101
#define MAX_LEN_FIELD 31
#define ERROR -1
#define EXITO 0
#define LIBRE "0"
#define OCUPADO "1"
#define BORRADO "2"
#include<stdio.h>
#include<string.h>
#include<stdbool.h>
#include"utils.h"
#include"tablaHash.h"

/**
 * Realiza la alta de un nuevo registro en la tabla hash.
 * Solicita al usuario los valores para cada campo del registro, asegurando que
 * el valor del campo de clave primaria (PK) sea único en el sistema. El primer campo
 * se establece como "OCUPADO". La función valida la entrada del usuario y maneja
 * los posibles errores de entrada.
 *
 * Se utiliza `buscarPK` para obtener la posición del campo de clave primaria. Se crea
 * un arreglo de cadenas `campos` para almacenar los valores ingresados. Para cada campo,
 * se solicita al usuario que ingrese un valor, con una validación especial para el campo
 * PK, donde se verifica que no haya valores duplicados utilizando `th_recuperar`.
 *
 * Una vez que se han ingresado todos los datos, se convierte el arreglo de campos en un
 * registro con `escritorDeCampos`, y se inserta en la tabla hash mediante `th_insertar`.
 *
 * La función devuelve un valor booleano que indica si la operación de alta fue exitosa.
 */
bool abmAlta(TablaHash th);

/**
 * Realiza la baja de un registro en la tabla hash mediante la clave primaria (PK).
 * Solicita al usuario el valor de la PK para identificar el registro que se desea eliminar.
 * Se utiliza `buscarPK` para determinar la posición del campo de clave primaria en la
 * tabla de metadatos. La función asigna memoria para almacenar la clave ingresada y
 * verifica si se han introducido caracteres adicionales, limpiando el buffer si es necesario.
 *
 * La baja se lleva a cabo utilizando la función `th_eliminar`, que busca y elimina el
 * registro correspondiente a la clave proporcionada. 
 *
 * La función devuelve un valor booleano que indica si la operación de baja fue exitosa.
 */
bool abmBaja(TablaHash th);

/**
 * Permite modificar un registro existente en la tabla hash a partir de la clave primaria (PK).
 * Solicita al usuario el valor de la PK para identificar el registro que se desea modificar.
 * Utiliza `buscarPK` para obtener la posición del campo de clave primaria en la tabla de metadatos.
 *
 * Si el registro existe, se muestran los campos disponibles para modificación (excluyendo la PK),
 * y el usuario selecciona cuál desea modificar. Se valida la opción ingresada para asegurar que
 * esté dentro del rango permitido y que no corresponda a la clave primaria.
 *
 * El usuario ingresa el nuevo valor para el campo seleccionado, el cual reemplaza el valor anterior.
 * La modificación se lleva a cabo mediante la función `th_reescribir`, que actualiza el registro
 * en la tabla hash. La función devuelve un valor booleano que indica si la operación de modificación
 * fue exitosa.
 */
bool abmModificaciones(TablaHash th);

/**
 * Muestra un registro de la tabla hash en un formato tabular.
 *
 * @param th TablaHash que contiene los metadatos del registro.
 * @param registro Cadena que representa el registro a mostrar.
 * @param cabezera Booleano que indica si se debe imprimir la cabecera de la tabla.
 * @param pos Posición del registro en la tabla, utilizada para determinar si es la primera fila.
 *
 * La función construye una representación visual del registro, mostrando los nombres de los campos
 * y sus valores en columnas. Si el parámetro `cabezera` es verdadero o `pos` es cero, se imprime
 * una línea separadora y la cabecera de la tabla. Los valores de los campos se alinean y se
 * muestran en formato de tabla, asegurando que cada campo ocupe el espacio adecuado.
 *
 * Se utiliza `lectorDeCampos` para obtener los valores de los campos del registro y se gestiona
 * la memoria dinámica para los buffers necesarios. La función imprime la tabla en la consola.
 */
static void mostrar(TablaHash th,char* registro, bool cabezera, int pos);

/**
 * Muestra un registro específico de la tabla hash según la clave primaria proporcionada por el usuario.
 *
 * @param th TablaHash que contiene los registros y sus metadatos.
 *
 * La función solicita al usuario que ingrese el valor de la clave primaria correspondiente al registro
 * que desea mostrar. Utiliza `th_recuperar` para buscar el registro en la tabla hash. Si el registro
 * existe, se invoca la función `mostrar` para imprimirlo en formato tabular, incluyendo una cabecera.
 * Si el registro no se encuentra, se notifica al usuario que el cliente es inexistente.
 *
 * La función maneja la entrada del usuario y se asegura de que se eliminen caracteres de nueva línea
 * que pudieran interferir con la búsqueda del registro.
 */
void abmMostrarRegistro(TablaHash th);

/**
 * Muestra registros de la tabla hash desde un archivo, permitiendo al usuario elegir entre dos opciones.
 *
 * @param th TablaHash que contiene los registros y sus metadatos.
 *
 * La función solicita al usuario seleccionar una opción: 
 * 1. Ver el listado de clientes existentes (es decir, aquellos que están marcados como ocupados).
 * 2. Ver el archivo completo, que incluye todos los registros, independientemente de su estado.
 *
 * Dependiendo de la opción seleccionada, la función lee registros del archivo correspondiente 
 * y los muestra utilizando la función `mostrar`. Si se selecciona la opción 1, solo se mostrarán 
 * los registros ocupados. Si se selecciona la opción 2, se mostrarán todos los registros.
 * 
 * La función asegura que se manejen adecuadamente los errores al abrir el archivo y también 
 * gestiona la entrada del usuario para garantizar que se seleccionen opciones válidas.
 */
void abmMostrarArchivo(TablaHash th);

#endif