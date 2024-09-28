TP 1 - ejercicio 3

grupo:
-Juan Cruz Rodríguez (195341)
-Gonzalo Echeverria Crenna (195155)
-Tobias Avila (195327)
-Salvador Baez (195157)

//============================================================================
//============================================================================
    Forma de ejecutar el programa
//============================================================================
//============================================================================

El código ha sido diseñado para ejecutarse de manera predeterminada, 
sin necesidad de modificar ningún parámetro, siendo compatible tanto 
con sistemas operativos Windows como Linux.

*NECESARIO*  
    Para la ejecución del programa, se ha optado por utilizar la 
    herramienta make, ya que permite su ejecución en cualquier sistema 
    operativo sin la necesidad de modificar valores en los archivos de 
    configuración de lanzamiento. Esta elección facilitó un desarrollo más ágil 
    y permitió realizar pruebas de manera eficiente. Por lo tanto, se 
    recomienda instalar make en caso de no contar con dicha herramienta, a 
    fin de garantizar la correcta ejecución del programa.

*POSIBLE DUDA*
(No tengo Make instalado, ¿que hago?)

    Existen dos posibles soluciones para ejecutar el programa:

    Instalar Make en el sistema:
        Recomendamos el siguiente video tutorial, que explica detalladamente cómo 
        instalar "make" en Windows. Este recurso fue utilizado por dos miembros 
        de nuestro grupo para configurar correctamente el entorno de trabajo y 
        así poder utilizar "make" de manera efectiva.

        https://youtu.be/PIW8QG8q4v4?si=teaWpoPFMOiDtBR1
    
    Ejecución manual:
        Alternativamente, es posible ejecutar el programa de manera manual. 
        Sin embargo, para esta opción será necesario modificar los archivos 
        de configuración de lanzamiento (*launch*) en Visual Studio o en el 
        motor utilizado para la ejecución del proyecto.

*COMO EJECUTAR*
    Abra una terminal (CMD en windows) y navegue hasta el directorio donde se encuentra este TP.
    cd "ruta al tp"
    
    Una vez ubicado en el directorio correcto, ejecute el comando "make" para compilar el proyecto
    
    Nota: Es posible que durante el proceso de compilación aparezcan algunos warnings. Sin embargo, 
    estos no afectan la funcionalidad del programa, por lo que puede continuar con la ejecución normalmente.

    Finalmente, una vez compilado el proyecto, ejecute el comando "make run" 
    y se ejecutara el programa

*POSIBLES DUDAS CON LA EJECUCION*
(el programa falla al tratar de ejecutar)

    Hemos encontrado un inconveniente durante la ejecución del comando 
    "make run", relacionado con la carpeta "bin". Existen dos posibles causas para este problema:

    1. Carpeta "bin" inexistente: 
        Es posible que la carpeta "bin" no exista en el directorio 
        "ejercicio3". En tal caso, se debe crear manualmente dicha carpeta para que el
        programa pueda ejecutarse correctamente.

    2. Conflicto con ejecutables en "bin": 
        Otra posibilidad es que ya exista un archivo ejecutable 
        en la carpeta **bin** que esté impidiendo la correcta ejecución 
        del programa en el sistema operativo. Para resolver esto, recomendamos 
        eliminar todos los archivos dentro de la carpeta "bin". 
        Al ejecutar nuevamente el comando "make", los ejecutables serán regenerados 
        automáticamente, lo que permitirá la ejecución del programa sin problemas.
