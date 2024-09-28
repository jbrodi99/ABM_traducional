#include "../includes/tablaHash.h"

TablaHash th_crear(int capacidad, int (*hash_function)(char*), char* fileName, size_t size, Metadata* metadata) {
    TablaHash nuevaTablaHash = (TablaHash) malloc(sizeof(struct TablaHashRep));
    nuevaTablaHash->capacidad = capacidad;
    nuevaTablaHash->hash_function = hash_function;
    strcpy(nuevaTablaHash->fileName, fileName);
    nuevaTablaHash->size_data = size;
    nuevaTablaHash->metadata = metadata;

    char* espacio = (char*)calloc(1,size);
    //Seteamos byte de estado en LIBRE
    memcpy(espacio, LIBRE, metadata->campos[0].longitud);
    //Grabamos los ultimos 4 bytes con el puntero a NULO
    memcpy(espacio + (metadata->longitudRegistro - metadata->campos[metadata->nCampos - 1].longitud), intToString(NULO), metadata->campos[metadata->nCampos - 1].longitud);

    #ifdef _WIN32  // Windows (32-bit or 64-bit)
            FILE* arch;
            arch = fopen(nuevaTablaHash->fileName, MODO_RW);
        if(arch == NULL){
            arch = fopen(nuevaTablaHash->fileName, MODO_W);
            if(arch == NULL){
                printf("No se pudo crear el archivo...\n");
                return NULL;
            }
            printf("Archivo creado con exito!\n");
            //Escribimos el espacio necesario para cada registro completo
            for (int i = 0; i < capacidad; i++){
                fwrite(espacio,size,1,arch);
            }
        }
        fclose(arch);
    #else  // Cualquier otro sistema operativo
            FILE* fp = fopen(nuevaTablaHash->fileName, MODO_RW);
        if(fp == NULL){
            fp = fopen(nuevaTablaHash->fileName, MODO_W);
            if(fp == NULL){
                printf("No se pudo crear el archivo...\n");
                return NULL;
            }
            printf("Archivo creado con exito!\n");
            //Escribimos el espacio necesario para cada registro completo
            for (int i = 0; i < capacidad; i++){
                fwrite(espacio,size,1,fp);
            }
        }
        fclose(fp);
    #endif
    
    return nuevaTablaHash;
}

bool th_insertar(TablaHash th, void* elemento, char* clave) {
    
    int posPK = buscarPK(th->metadata);
    int end;
    int pos = th->hash_function(clave);
    char* siguiente = NULL;
    int siguienteDecodificado = 0;
    char* registro = (char*)calloc(1,th->size_data);
    char* registroEscritura = NULL;
    char* byteEstado = (char*)calloc(1,sizeof(char));
    char** campos = NULL;
    char** camposNuevos = NULL;

    Lista lista;
    bool agregado = false;

    FILE* fp = fopen(th->fileName, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }
    
    fseek(fp,0,SEEK_END);
    end = ftell(fp);
    end = end/th->size_data;
    //*Clave codificada para poder almacenarla como cadena
    siguiente = intToString(end);

    //traer nodo del disco y verificar estado
    fseek(fp,th->size_data*pos,SEEK_SET);

    //lee la longitud del registro segun metadata
    fread(registro,th->size_data,1,fp);
    //Validamos que el registro exista
    campos = lectorDeCampos(th->metadata,registro);
    camposNuevos = lectorDeCampos(th->metadata,(char*)elemento);

    if (strcmp(campos[0], LIBRE) == 0 || strcmp(campos[0], BORRADO) == 0){
        //posicion libre, escribir registro
        sprintf(byteEstado,"%s",OCUPADO);
        strcpy(campos[0], byteEstado);
        strcpy(camposNuevos[th->metadata->nCampos - 1],campos[th->metadata->nCampos - 1]);
        fseek(fp,-th->size_data,SEEK_CUR);
        registroEscritura = escritorDeCampos(th->metadata,camposNuevos);
        printf("Direccion relativa %li \n",th->size_data*pos );
        fwrite(registroEscritura,th->size_data,1,fp);
        agregado = true;
    }else{  //espacio ocupado, inserto en la lista
        siguienteDecodificado = parseInt(campos[th->metadata->nCampos - 1]);
        if(siguienteDecodificado != NULO){  //Si campo siguiente != NULO (-1)
            //lista existente, creamos instancia de lista dinamica y seteamos el inicio de la lista.
            lista = l_crear(th->fileName,th->size_data,siguienteDecodificado, th->metadata);
            
            if ( strcmp(clave, campos[posPK]) != 0 &&
                    l_buscar(lista,clave) == NULL) {   //verificamos que no exista el registro
                agregado = l_agregar(lista, elemento);
            }
        }else{
            //creamos lista y agregamos como primer elemento
            strcpy(campos[th->metadata->nCampos - 1],siguiente);
            fseek(fp,-th->size_data,SEEK_CUR);
            registroEscritura = escritorDeCampos(th->metadata,campos);
            fwrite(registroEscritura,th->size_data,1,fp);

            lista = l_crear(th->fileName,th->size_data,NULO, th->metadata);
            agregado = l_agregar(lista,elemento);
        }
    }
    fclose(fp);
    return agregado;
}


bool th_eliminar(TablaHash th, char* clave) {
    int pos = th->hash_function(clave);
    int posPK = buscarPK(th->metadata);
    int siguiente = 0;
    char* registroLectura = (char*)calloc(1,th->size_data);
    char* registroEscritura = NULL;
    char* byteEstado = (char*)calloc(1,sizeof(char));
    char** campos = NULL;
    bool borre = false;
    Lista lista;

    FILE* fp = fopen(th->fileName, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }
    //traer nodo del disco y verificar estado
    fseek(fp,th->size_data*pos,SEEK_SET);
    fread(registroLectura,th->size_data,1,fp);
    campos = lectorDeCampos(th->metadata,registroLectura);
    siguiente = parseInt(campos[th->metadata->nCampos - 1]);

    if (strcmp(campos[0], OCUPADO) == 0) {
        if (strcmp(campos[posPK], clave) == 0) {
            //es el elemento de la tabla
            sprintf(byteEstado,"%s",BORRADO);
            strcpy(campos[0], byteEstado);
            fseek(fp,-th->size_data,SEEK_CUR);
            registroEscritura = escritorDeCampos(th->metadata,campos);
            fwrite(registroEscritura,th->size_data,1,fp);
            borre = true;
        }else{
            //borrado logico del registro en la lista
            if(siguiente != NULO){
                lista = l_crear(th->fileName,th->size_data,siguiente,th->metadata);
                borre = l_borrar(lista, clave);
            }
        }
    }
    fclose(fp);
    return borre;
}

void* th_recuperar(TablaHash th, char* clave){
    int pos = th->hash_function(clave);
    int posPK = buscarPK(th->metadata);
    int siguiente = 0;
    char* registroLectura = (char*)calloc(1,th->size_data);
    char* registroResultado = NULL;
    char** campos = NULL;

    Lista lista;

    FILE* fp = fopen(th->fileName, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }

    //traer nodo del disco y verificar estado
    fseek(fp,th->size_data*pos,SEEK_SET);
    fread(registroLectura,th->size_data,1,fp);
    
    campos = lectorDeCampos(th->metadata,registroLectura);
    siguiente = parseInt(campos[th->metadata->nCampos - 1]);

    if (strcmp(campos[0], OCUPADO) == 0 && strcmp(campos[posPK], clave) == 0){

        registroResultado = registroLectura;
    } else if(siguiente != NULO) {
        lista = l_crear(th->fileName,th->size_data,siguiente,th->metadata);
        registroResultado = (char*)l_buscar(lista,clave);
    }
    fclose(fp);
    return registroResultado;
}

bool th_reescribir(TablaHash th, char* clave, char* estructuraModificada){

    int pos = th->hash_function(clave);
    bool reescrito = false;
    Lista lista;
    int posPK = buscarPK(th->metadata);
    int siguiente = 0;
    char* registroLectura = (char*)calloc(1,th->size_data);
    char* registroReescrito = estructuraModificada;
    char** campos = NULL;

    FILE* fp = fopen(th->fileName, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }

    //traer nodo del disco y verificar estado
    fseek(fp,th->size_data*pos,SEEK_SET);
    fread(registroLectura,th->size_data,1,fp);
    campos = lectorDeCampos(th->metadata,registroLectura);
    siguiente = parseInt(campos[th->metadata->nCampos - 1]);
    if (strcmp(campos[0], OCUPADO) == 0 && strcmp(campos[posPK],clave) == 0){
        fseek(fp,-th->size_data,SEEK_CUR);
        fwrite(registroReescrito,th->size_data,1,fp);
        reescrito = true;
    }else if (siguiente != NULO){
        lista = l_crear(th->fileName,th->size_data,siguiente,th->metadata);
        reescrito = l_borrar(lista,clave);
        reescrito = l_agregar(lista,registroReescrito);
    }

    fclose(fp);
    
    return reescrito;
}
