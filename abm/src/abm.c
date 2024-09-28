#include"../includes/abm.h"
#define NRO_PRIMO 997
#define MAX_SIZE_TABLE 1000

bool abmAlta(TablaHash th){

    bool alta = false;
    bool repetido = true;

    int posPK = buscarPK(th->metadata);
    char* registroLectura = NULL;
    char** campos = (char**)calloc(th->metadata->nCampos,sizeof(char*));
    for (int i = 0; i < th->metadata->nCampos; i++){
        campos[i] = (char*)calloc(th->metadata->campos[i].longitud,sizeof(char));
    }
    //primer campo Estado --> OCUPADO
    strcpy(campos[0], OCUPADO);

    //Ir desde campo 1 a n-1 e ingresar datos (user entry)
    for (int i = 1; i < th->metadata->nCampos - 1; i++){
        if(i != posPK){
            printf("Ingrese el valor del campo \"%s\": ", th->metadata->campos[i].nombre);
            fgets(campos[i],th->metadata->campos[i].longitud, stdin);
            if(!quitarFinDeLinea(campos[i])){
                limpiarBuffer();
            }
        }else{  //Campo clave primaria
            do{
                printf("Ingrese el valor del campo \"%s\" (PK) :", th->metadata->campos[i].nombre);
                //campo especial, se ingresa por separado para validar la no repeticion del mismo, evitando inconsistencia
                fgets(campos[i],th->metadata->campos[i].longitud, stdin);
                if(!quitarFinDeLinea(campos[i])){
                    limpiarBuffer();
                }
                registroLectura = th_recuperar(th,campos[i]);
                if(registroLectura == NULL){
                    repetido = false;
                }else{
                    printf("El valor del campo ingresado ya existe en el sistema. Vuelva a intentarlo con una clave valida.\n");
                }
            } while (repetido);
        }
    }
    campos[th->metadata->nCampos - 1] = intToString(NULO);
    char* registroEscritura = escritorDeCampos(th->metadata,campos);

    alta = th_insertar(th,registroEscritura,campos[posPK]);

    return alta;
}

bool abmBaja(TablaHash th){
    
    bool borre = false;
    int posPK = buscarPK(th->metadata);
    char* clave = (char*)calloc(th->metadata->campos[posPK].longitud +1, sizeof(char));  
    
    printf("Ingrese el valor de \"%s\" (PK) :",th->metadata->campos[posPK].nombre);
    fgets(clave,th->metadata->campos[posPK].longitud + 1, stdin);
    if (!quitarFinDeLinea(clave)) {
        limpiarBuffer();  // Si hay datos extra en el buffer, los limpiamos
    }
    
    borre = th_eliminar(th,clave);
    
    return borre;
}

bool abmModificaciones(TablaHash th){
    int nroCampo = 0;

    bool estado = false;
    int posPK = buscarPK(th->metadata);
    char* clave = (char*)calloc(1,th->metadata->campos[posPK].longitud * sizeof(char));  
    char* campoModificado = NULL;
    char** campos = NULL;
    //int campo = 0, clave = 0;
    printf("Ingrese el valor de \"%s\" (PK) :",th->metadata->campos[posPK].nombre);
    fgets(clave,th->metadata->campos[posPK].longitud, stdin);
    if(!quitarFinDeLinea(clave)){
        limpiarBuffer();
    }
    //recuperar cliente -> si existe, modificamos registro -> llamamos a th_reescribir

    char* registroLectura = NULL;
    char* registroEscritura = NULL;
    registroLectura = th_recuperar(th,clave);

    if(registroLectura == NULL){
        printf("Registro inexistente\n");
    }else{
        campos = lectorDeCampos(th->metadata,registroLectura);

        printf("Seleccione el campo a modificar [1-%i]: \n",th->metadata->nCampos - 3);
        for (int i = 1; i < th->metadata->nCampos - 1; i++){
            if(i != posPK){
                printf("%i. \"%s\".\n",i,th->metadata->campos[i].nombre);
            }
        }
        
        nroCampo = ingresoEntero();
        while (nroCampo < 1 || nroCampo > th->metadata->nCampos-1 || posPK == nroCampo)
        {
            printf("Valor no valido, por favor ingrese un numero entre:\n");
            printf("1 - %i\n", th->metadata->nCampos - 1);
            printf("Tambien debe ser distinto a: %i\n", posPK);
            nroCampo = ingresoEntero();
        }
        
        printf("Ingrese el valor a modificar del campo \"%s\": ",th->metadata->campos[nroCampo].nombre);

        campoModificado = (char*)calloc(1,th->metadata->campos[nroCampo].longitud);  

        fgets(campoModificado,th->metadata->campos[nroCampo].longitud, stdin);
        if(!quitarFinDeLinea(campoModificado))
        {
            limpiarBuffer();
        }

        printf("Modificando \"%s\" por \"%s\"...\n",campos[nroCampo], campoModificado);

        campos[nroCampo] = campoModificado;
        
        registroEscritura = escritorDeCampos(th->metadata, campos);

        estado = th_reescribir(th,clave, registroEscritura);

        if(estado){
            printf("Registro modificado con exito\n");
        }else{
            printf("Ocurrio un error al reescribir la informacion\n");
        }

    }

    return estado;
}

static void mostrar(TablaHash th,char* registro, bool cabezera, int pos){
    
    int lenTabla = (th->metadata->longitudRegistro + th->metadata->nCampos + 1);
    char* tupla = NULL;
    char** campos = lectorDeCampos(th->metadata,registro);
    char* separador = (char*)calloc(lenTabla + 2,sizeof(char));
    memset(separador,'-',lenTabla);
    separador[lenTabla - 2] = '\n';
    separador[lenTabla - 1] = '\0';
    tupla = (char*)calloc(lenTabla,sizeof(char));
    
    for (int i = 1; i < th->metadata->nCampos - 1; i++){
        strcat(tupla,"|");
        strncat(tupla,th->metadata->campos[i].nombre,longitudCampo(MAX_LEN_CAMPO, th->metadata->campos[i].longitud));
        memset(tupla + strlen(tupla),' ',th->metadata->campos[i].longitud - strlen(campos[i]));
    }
    strcat(tupla,"|");

    if(cabezera || pos ==  0){
        printf("%s",separador);
        printf("%s\n",tupla); 
        printf("%s",separador);
    }

    memset(tupla,0,strlen(tupla));

    for (int i = 1; i < th->metadata->nCampos -1; i++){
        strcat(tupla,"|");
        strncat(tupla,campos[i],longitudCampo(MAX_LEN_CAMPO, th->metadata->campos[i].longitud));
        memset(tupla + strlen(tupla),' ',th->metadata->campos[i].longitud - strlen(campos[i]));
    }
    strcat(tupla,"|");

    printf("%s\n", tupla);
    printf("%s",separador);
}

void abmMostrarRegistro(TablaHash th){
    
    int posPK = buscarPK(th->metadata);
    char* clave = (char*)calloc(1,th->metadata->campos[posPK].longitud * sizeof(char));  
    char* registroLectura = NULL;
    printf("Ingrese el valor de \"%s\" (PK) :",th->metadata->campos[posPK].nombre);
    fgets(clave,th->metadata->campos[posPK].longitud, stdin);
    if(!quitarFinDeLinea(clave)){
        limpiarBuffer();
    }

    registroLectura = (char*)th_recuperar(th,clave);

    if(registroLectura != NULL){
        mostrar(th,registroLectura, true, 0);

    }else{
        printf("Cliente inexistente.\n");
    }
}

void abmMostrarArchivo(TablaHash th){
    int opcion = 0;
    int count = 0;
    char * registroLectura = (char*)calloc(1,th->metadata->longitudRegistro*sizeof(char));
    char** campos = NULL;
    printf("1.Ver listado de clientes(solo existentes)\n");
    printf("2.Ver archivo completo\n");

    opcion = ingresoEntero();
    while (opcion < 1 || opcion > 2)
    {
        printf("Por favor, ingrese un valor valido(1-2): ");
        opcion = ingresoEntero();
    }
    
    FILE* fp = fopen(th->fileName, "r+b");
    if (fp == NULL){
        perror("No se pudo abrir el archivo...");
        return;
    }

    while (fread(registroLectura,th->metadata->longitudRegistro,1,fp) != 0){
        
        if(opcion == 1){
            campos = lectorDeCampos(th->metadata,registroLectura);
            if(strcmp(campos[0], OCUPADO) == 0){
                mostrar(th,registroLectura, false, count);
                count++;
            }
        }else{
            mostrar(th,registroLectura,false, count);
            count++;
        }
        
    }

}