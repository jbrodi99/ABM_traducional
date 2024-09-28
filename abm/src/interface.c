#include"../includes/interface.h"


void interfaces()
{
    bool valido;
    int opcion;
    char fileName[MAX_LEN_FILE];
    char fileNameMetadata[MAX_LEN_FILE];
    TablaHash th = NULL;

    do  //MENU/////////////////////////////////////////////////
    {
        printf("\t\t\t                      +------------------------+\n");
        printf("\t\t\t                      |        Menu ABM        |\n");
        printf("\t\t\t                      +------------------------+\n\n\n");
        printf("\t||>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>||\n");
        printf("\t||OPCION 1 - Crear/Cargar archivo.                                                                    ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 2 - Alta                                                                                     ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 3 - Baja                                                                                     ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 4 - Modificaciones                                                                           ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 5 - Mostrar cliente                                                                          ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 6 - Mostrar archivo                                                                          ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 0 - Salir                                                                                    ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");        
        printf("\t||                                                                                                    ||\n");
        printf("\t||Ingrese una opcion entre 0 y 6                                                                      ||\n");
        printf("\t||                                                                                                    ||\n");
        printf("\t||<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<||\n");
        
        //VALIDACION OPCION DEL MENU////////////////////////////////////////////////////////////
        valido = false;
        while (valido != true) 
        {   
            opcion = ingresoEntero(); //Validacion de entero
            
            if (opcion < 0 || opcion > 6 )
            {
                printf("Valor invalido, reintente el ingreso...(Opciones validas entre 0 y 6)\n");
            }
            else
            {
                valido = true;
            }
        } 
        //OPCION SELECCIONADA/////////////////////////////////////////////////////////////////
        limpiar_pantalla();

        switch (opcion){
        case 1:
            menuCarga(fileName, fileNameMetadata, &th);
            pausa();
            break;
        case 2:
            if(th == NULL){
                interfaces();
            }
            abmAlta(th);
            pausa();
            break;
        case 3:
            if(th == NULL){
                interfaces();
            }
            abmBaja(th);
            pausa();
            break;
        case 4:
            if(th == NULL){
                interfaces();
            }
            abmModificaciones(th);
            pausa();
            break;    
        case 5:
            if(th == NULL){
                interfaces();
            }
            abmMostrarRegistro(th);
            pausa();
            break;
        case 6:
            if(th == NULL){
                interfaces();
            }
            abmMostrarArchivo(th);
            pausa();
            break;
        default:
            printf("Saliendo...\n");
            break;
        }
    }while (opcion != 0);
}

bool menuCarga(char* fileName, char* fileNameMetadata, TablaHash* th){
    bool valido, creadoCargado = false;
    int opcion = 0;

    do  //MENU/////////////////////////////////////////////////
    {
        printf("\t\t\t                      +------------------------+\n");
        printf("\t\t\t                      |       Menu Archivo     |\n");
        printf("\t\t\t                      +------------------------+\n\n\n");
        printf("\t||>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>||\n");
        printf("\t||OPCION 1 - Crear archivo nuevo.                                                                     ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 2 - Cargar archivo existente.                                                                ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");
        printf("\t||OPCION 0 - Salir                                                                                    ||\n");
        printf("\t||----------------------------------------------------------------------------------------------------||\n");        
        printf("\t||                                                                                                    ||\n");
        printf("\t||Ingrese una opcion entre 0 y 2                                                                      ||\n");
        printf("\t||                                                                                                    ||\n");
        printf("\t||<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<||\n");
        do //VALIDO OPCION DEL MENU////////////////////////////////////////////////////////////
        {
            valido = true;
            opcion = ingresoEntero();
            if (opcion < 0 || opcion > 2 )
            {
                printf("Valor invalido, reintente el ingreso...(Opciones validas entre 0 y 2)\n");
                valido = false;
            }
        } while (!valido);
        //OPCION SELECCIONADA/////////////////////////////////////////////////////////////////
        limpiar_pantalla();

        switch (opcion){
        case 1:
            *th = (TablaHash)cargarMetadata(fileName,fileNameMetadata);
            if(*th != NULL){
                creadoCargado = true;
            }
            pausa();
            break;
        case 2:
            *th = (TablaHash)lecturaMetadata(fileName,fileNameMetadata);
            if(*th != NULL){
                creadoCargado = true;
            }
            pausa();
            break;
        default:
            printf("Saliendo...\n");
            break;
        }
    }while (opcion != 0 && !creadoCargado);
    return creadoCargado;
}