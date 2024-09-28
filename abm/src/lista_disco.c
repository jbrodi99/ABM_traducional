#include"../includes/listas.h"


struct ListaRep{
    int inicio; 
    size_t size_data; 
    char file[100]; 
    Metadata* metadata;
};

struct IteradorRep{
    Lista lista;
    int posActual;
};

Lista l_crear(char* fileName,size_t sizeData, int inicio, Metadata* metadata){
    Lista nuevaLista = (Lista)malloc(sizeof( struct ListaRep));
    nuevaLista->inicio = inicio;
    strcpy(nuevaLista->file,fileName);
    nuevaLista->size_data = sizeData;
    nuevaLista->metadata = metadata;
    return nuevaLista;
}

bool l_es_vacia(Lista lista){
    return lista->inicio == NULO;
}

bool l_es_llena(Lista lista);   
//sin implementar

//*Modificada para metadata
int l_longitud(Lista lista){
    if(l_es_vacia(lista)){
        return 0;
    }
    FILE* fp = fopen(lista->file, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return -1;
    }

    char* registro = (char*)calloc(1,lista->size_data);
    char** campos = NULL;
    int cont = 0;
    int siguiente = 0;

    fseek(fp,lista->size_data*lista->inicio,SEEK_SET);

    //lee la longitud del registro segun metadata
    fread(registro,lista->size_data,1,fp);
    campos = lectorDeCampos(lista->metadata,registro);
    
    if( strcmp(campos[0], OCUPADO) == 0){
        cont++;
    }
    
    siguiente = parseInt(campos[lista->metadata->nCampos - 1]);

    while(siguiente != NULO){

        fseek(fp,(lista->size_data*siguiente),SEEK_SET);

        fread(registro,lista->size_data,1,fp);
        campos = lectorDeCampos(lista->metadata, registro);

        if(strcmp(campos[0], OCUPADO) == 0){
            cont++;
        }

        siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
    }
    fclose(fp);
    return cont;
}

bool l_agregar(Lista lista, void* elemento){
    bool escrito = false;
    int siguiente = 0, end = 0;
    char** campos = NULL;
    char** camposNuevos = NULL;
    char* registroAGrabar = (char*) elemento;
    char* registroLectura = (char*)calloc(1,lista->size_data);
    camposNuevos = lectorDeCampos(lista->metadata,(char*)elemento);
    
    FILE* fp = fopen(lista->file, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }

    fseek(fp,0, SEEK_END);
    end = ftell(fp);
    end = end/lista->size_data;

    
    if(l_es_vacia(lista)){
        fwrite(registroAGrabar,lista->size_data,1,fp);
        lista->inicio = end;
        escrito = true;
    }else{
        //nuevo codigo donde corroboro registros reutilizables
        fseek(fp,lista->size_data*lista->inicio,SEEK_SET); //me posiciono al inicio
        fread(registroLectura,lista->size_data,1,fp);
    
        campos = lectorDeCampos(lista->metadata,registroLectura);

        if(strcmp(campos[0], BORRADO) == 0){
            strcpy(camposNuevos[lista->metadata->nCampos - 1],campos[lista->metadata->nCampos - 1]);    //actualizo puntero para reutilizar registro
            fseek(fp,-lista->size_data,SEEK_CUR);   //reescribo registro con nNodo
            registroAGrabar = escritorDeCampos(lista->metadata,camposNuevos);
            fwrite(registroAGrabar,lista->size_data,1,fp);
            escrito = true; 
        }
        //?Verificar si se puede tener variable siguiente por fuera y quitar el calculo de la expresion
        while(parseInt((campos[lista->metadata->nCampos - 1])) != NULO && !escrito){
            siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
            fseek(fp,lista->size_data*siguiente,SEEK_SET);
            fread(registroLectura,lista->size_data,1,fp);
            campos = lectorDeCampos(lista->metadata,registroLectura);
            if(strcmp(campos[0], BORRADO) == 0){
                strcpy(camposNuevos[lista->metadata->nCampos - 1],campos[lista->metadata->nCampos - 1]);    //actualizo puntero para reutilizar registro
                fseek(fp,-lista->size_data,SEEK_CUR);   //reescribo registro con nNodo
                registroAGrabar = escritorDeCampos(lista->metadata, camposNuevos);
                fwrite(registroAGrabar,lista->size_data,1,fp);
                escrito = true;
            }
        }
        if(!escrito){
            //Si estoy posicionado al final de la lista, todos estaban ocupados
            siguiente = end;
            fseek(fp,-lista->size_data,SEEK_CUR);   //actualizo puntero del anteultimo
            strcpy(campos[lista->metadata->nCampos - 1],intToString(siguiente));
            registroAGrabar = escritorDeCampos(lista->metadata, campos);
            fwrite(registroAGrabar,lista->size_data,1,fp);
            
            fseek(fp,0, SEEK_END);  //agrego registro dinamicamente 
            registroAGrabar = escritorDeCampos(lista->metadata, camposNuevos);
            fwrite(registroAGrabar,lista->size_data,1,fp);
            escrito = true;
        }
    }

    fclose(fp);
    return escrito;
}
bool l_borrar(Lista lista, char* clave){
    char** campos = NULL;
    char* registroAGrabar = NULL;
    char* registroLectura = (char*)calloc(1,lista->size_data);
    int siguiente;
    if(l_es_vacia(lista)){
        return false;
    }
    bool borre = false;

    FILE* fp = fopen(lista->file, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }
    
    fseek(fp,lista->size_data*lista->inicio,SEEK_SET);
    fread(registroLectura,lista->size_data,1,fp);
    campos = lectorDeCampos(lista->metadata,registroLectura);
    int posPK = buscarPK(lista->metadata);
    
    if(strcmp(campos[posPK], clave) == 0){
        //borrado logico
        strcpy(campos[0], BORRADO);
        fseek(fp,-lista->size_data,SEEK_CUR);
        registroAGrabar = escritorDeCampos(lista->metadata, campos);
        fwrite(registroAGrabar,lista->size_data,1,fp);       
        borre = true;
    }
    siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
    while (siguiente != NULO && !borre){
        fseek(fp,lista->size_data*siguiente,SEEK_SET);
        fread(registroLectura,lista->size_data,1,fp);
        campos = lectorDeCampos(lista->metadata, registroLectura);
        siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
        if(strcmp(campos[posPK], clave) == 0){
            //borrado logico
            strcpy(campos[0], BORRADO);
            fseek(fp,-lista->size_data,SEEK_CUR);
            registroAGrabar = escritorDeCampos(lista->metadata, campos);
            fwrite(registroAGrabar,lista->size_data,1,fp);       
            borre = true;
        }
    }
    fclose(fp);
    return borre;
}

void* l_buscar(Lista lista, char* clave){
    if(l_es_vacia(lista)){
        return NULL;
    }

    int posPK = 0, siguiente = 0;
    char* resultado = NULL;
    char* registroLectura = (char*)calloc(1,lista->size_data);
    char** campos;
    bool encontrado = false;

    FILE* fp = fopen(lista->file, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return false;
    }
    
    fseek(fp,lista->size_data*lista->inicio,SEEK_SET);
    fread(registroLectura,lista->size_data,1,fp);
    campos = lectorDeCampos(lista->metadata,registroLectura);
    posPK = buscarPK(lista->metadata);

    if(strcmp(campos[posPK],clave) == 0){
        resultado  = registroLectura;
    }else{
        siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
        while (siguiente != NULO && !encontrado){
            fseek(fp,lista->size_data*siguiente,SEEK_SET);
            fread(registroLectura,lista->size_data,1,fp);
            campos = lectorDeCampos(lista->metadata,registroLectura);
            siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
            if(strcmp(campos[0], OCUPADO) == 0 && strcmp(campos[posPK],clave) == 0){
                encontrado = true;
                resultado = registroLectura;
            }
        }
    }
    
    fclose(fp);
    return resultado;
}

void* l_recuperar(Lista lista, int pos){
    if(l_es_vacia(lista)){
        return NULL;
    }

    bool encontrado = false;
    int posRelativa = 1, siguiente = 0;
    char* registroLectura = (char*)calloc(1,lista->size_data);
    char** campos;



    FILE* fp = fopen(lista->file, "r+b");
    if(fp == NULL){
        perror("No fue posible abrir el archivo\n");
        return NULL;
    }
    
    fseek(fp,lista->size_data*lista->inicio,SEEK_SET);
    fread(registroLectura,lista->size_data,1,fp);
    campos = lectorDeCampos(lista->metadata,registroLectura);

    if(pos == 1){
        encontrado = true;
    }
    siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
    while(siguiente != NULO && !encontrado){
        posRelativa++;
        fseek(fp,lista->size_data*siguiente,SEEK_SET);
        fread(registroLectura,lista->size_data,1,fp);
        if(posRelativa == pos){
            encontrado = true;
            break;
        }
        campos = lectorDeCampos(lista->metadata,registroLectura);
        siguiente = parseInt(campos[lista->metadata->nCampos - 1]);
    }
    fclose(fp);
    
    if(encontrado)  return registroLectura;
    return NULL;
}
