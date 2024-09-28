#include"../includes/metadata.h"
#include"../includes/tablaHash.h"

int fnHash(char* clave){
    int i = 0;
    int contador = 0;
    while (clave[i]){
        contador += clave[i];
        i++;
    }
    return contador % NRO_PRIMO;
}

bool seleccionarPK(Metadata* metadata){
    bool valido = false;
    int opcion = 0;

    printf("Seleccione alguno de los siguiente campos como clave primaria.\n");
    printf("Solo se presentaran los valores validos como clave primaria\n");
    
    while (!valido){
        for (int i = 1; i < metadata->nCampos - 1; i++){
            if(metadata->campos[i].longitud >= MIN_LEN_PK){
                printf("Campo %i : %s \n",i,metadata->campos[i].nombre);
            }
        }
        printf("Ingrese la opcion elegida [Numero de campo]: ");
        opcion = ingresoEntero();
        if(opcion >= 1 && opcion <= metadata->nCampos-2 && metadata->campos[opcion].longitud >= MIN_LEN_PK){
            metadata->campos[opcion].pk = 1;    //Valor de clave primaria establecido
            valido = true;
            limpiarBuffer();
        }
        else
        {
            printf("La opcion seleccionada no es valida\n");
        }
    }
    printf("\n");
    return valido;
}

int buscarPK(Metadata* metadata){
    int i = 0, pos = 0;
    bool encontrada = false;
    while (i < metadata->nCampos && !encontrada){
        if(metadata->campos[i].pk){
            encontrada = true;
            pos = i;
        }
        i++;
    }
    return pos;
}

void* cargarMetadata(char* fileName, char* fileNameMetadata){
    TablaHash th = NULL;
    
    char estado[] = "estado";
    char siguiente[] = "siguiente";
    char file[MAX_LEN_FILE] = {0};
    char pathFile[MAX_LEN_FILE] = "files/";
    #ifdef _WIN32  // Windows (32-bit or 64-bit)
        // No es necesario codigo extra para Windows 
    #else  // Cualquier otro sistema operativo
        char pathFileMeta[MAX_LEN_FILE] = PATH_METADATA;
    #endif
    bool Pkvalid = false;
    
    Metadata* metadata = (Metadata*)calloc(1,sizeof(Metadata));
    
    int cantidadCampos = 0, lenEstado = 2, lenSiguiente = 5;
    int i = 0;
    bool pkSelecionada = false;
    printf("Ingrese la cantidad de campos que tendra el registro: ");
    cantidadCampos = ingresoEntero();
    metadata->nCampos = cantidadCampos + 2; //sumamos dos para estado y puntero a siguiente

    //Reservamos espacio para el arreglo de campos, luego de previamente definir la cantidad 
    metadata->campos = (Campos*)calloc(metadata->nCampos,sizeof(Campos));


    strcpy(metadata->campos[i].nombre, estado);
    metadata->campos[i].longitud = lenEstado;
    metadata->longitudRegistro += metadata->campos[i].longitud;
       
    //Inicio i en 1 y recorremos cantidad -1
    //*primer campo 0 reservado para estado
    //*ultimo campo (extra) reservado para puntero
    
    for (i = 1; i < metadata->nCampos - 1; i++){
        printf("Ingrese el nombre del campo %i :",i);
        fgets(metadata->campos[i].nombre,MAX_LEN_CAMPO,stdin);
        if(!quitarFinDeLinea(metadata->campos[i].nombre)){
            limpiarBuffer();
        }
        
        
        printf("Ingrese la longitud del campo %i :", i);
        metadata->campos[i].longitud = ingresoEntero();
        if (metadata->campos[i].longitud >= MIN_LEN_PK)
        {
            Pkvalid = true;
        }
        

        if ((metadata->nCampos - 2) == i && Pkvalid == false)
        {
            while (metadata->campos[i].longitud < MIN_LEN_PK)
            {
                printf("Por favor, la longitud ingresada debe ser mayor a %i digitos\n", MIN_LEN_PK);
                metadata->campos[i].longitud = ingresoEntero();
            }
        }
        
        
        metadata->campos[i].longitud++;

        metadata->campos[i].pk = 0; //Valor de 'false' para clave primaria
    
        metadata->longitudRegistro += metadata->campos[i].longitud;
    }
    
    strcpy(metadata->campos[i].nombre, siguiente);
    metadata->campos[i].longitud = lenSiguiente;
    metadata->longitudRegistro += metadata->campos[i].longitud;
    pkSelecionada = seleccionarPK(metadata);
    if(!pkSelecionada){
        printf("Error al seleccionar la PK\n");
    }

    printf("Ingrese el nombre que tendra el archivo (Incluyendo el '.dat' final): ");
    fgets(file,MAX_LEN_FILE - 1,stdin);
    if(!quitarFinDeLinea(file)){
        limpiarBuffer();
    }
    strcat(pathFile, file);

    #ifdef _WIN32  // Windows (32-bit or 64-bit)
        strcpy(fileName,file);
        strcpy(fileNameMetadata,"Meta");
        strcat(fileNameMetadata,file);
    #else  // Cualquier otro sistema operativo
        strcpy(fileName,pathFile);
        strcat(pathFileMeta,file);
        strcpy(fileNameMetadata,pathFileMeta);
    #endif

    //Creamos archivo de metadata 
    #ifdef _WIN32  // Windows (32-bit or 64-bit)
        FILE* arch;
        arch = fopen(fileNameMetadata, "w+b");
        if(arch == NULL)
        {
            printf("No se pudo crear el archivo metadata...\n");
            return NULL;
        }
        else
        {
            printf("Archivo metadata creado con exito!\n");
            fwrite(&metadata->nCampos,sizeof(int),1,arch);
            fwrite(&metadata->longitudRegistro,sizeof(int),1,arch);
            fwrite(metadata->campos,sizeof(Campos)*metadata->nCampos,1,arch);
        }
        fclose(arch);
    #else  // Cualquier otro sistema operativo
        FILE* fp = fopen(fileNameMetadata, MODO_W);
        if(fp == NULL)
        {
            printf("No se pudo crear el archivo metadata...\n");
            return NULL;
        }
        printf("Archivo metadata creado con exito!\n");
        fwrite(&metadata->nCampos,sizeof(int),1,fp);
        fwrite(&metadata->longitudRegistro,sizeof(int),1,fp);
        fwrite(metadata->campos,sizeof(Campos)*metadata->nCampos,1,fp);
        fclose(fp);
    #endif
   
    th = th_crear(MAX_CAPACIDAD_TABLA,&fnHash,fileName,metadata->longitudRegistro,metadata);

    return th;  //*La tabla es la crea el archivo con la info de la estructura que se cargo en la metadata
}

void* lecturaMetadata(char* fileName, char* fileNameMetadata)
{ 
    #ifdef _WIN32  // Windows (32-bit or 64-bit)
        char file[MAX_LEN_FILE];
        char pathFile[MAX_LEN_FILE];
        strcpy(fileNameMetadata,"Meta");

        printf("Ingrese el nombre del archivo (Incluyendo el '.dat' final): ");
        fgets(file,MAX_LEN_FILE - 1,stdin);
        if (file[strlen(file)-1] == '\n')
        {
            file[strlen(file)-1] = '\0';
        }
        fflush(stdin);
        strcpy(pathFile, file);
        strcpy(fileName,pathFile);
        strcat(fileNameMetadata,file);

        FILE* fp = fopen(fileNameMetadata, "r+b");
        if (fp == NULL) {
            printf("No se pudo abrir el archivo de metadata %s\n", fileNameMetadata);
            return NULL;
        }
        Metadata* metadata = (Metadata*)calloc(1, sizeof(Metadata));
        fread(&metadata->nCampos, sizeof(int), 1, fp);         //esto me sirve para determinar cuanta memoria reservar y cuantos campos debera leer el archivo
        fread(&metadata->longitudRegistro, sizeof(int), 1, fp);   //esto me sirve para detetminar la longitud del registro completo y pasarsela a la tabla hash

        metadata->campos = (Campos*)calloc(metadata->nCampos, sizeof(Campos));  //reservo espacio en memoria para el arreglo campos y poder leer la metadata

        fread(metadata->campos, sizeof(Campos), metadata->nCampos, fp); //leo y cargo la metadata
        fclose(fp);

        TablaHash th = th_crear(MAX_CAPACIDAD_TABLA, &fnHash, fileName, metadata->longitudRegistro, metadata);
    #else  // Cualquier otro sistema operativo
        char file[MAX_LEN_FILE] = {0};
        char pathFile[MAX_LEN_FILE] = "files/";
        char pathFileMeta[MAX_LEN_FILE] = PATH_METADATA;

        printf("Ingrese el nombre que tendra el archivo: ");
        fgets(file,MAX_LEN_FILE - 1,stdin);
        if(!quitarFinDeLinea(file))
        {
            limpiarBuffer();
        }
        strcat(pathFile, file);
        strcpy(fileName,pathFile);
        strcat(pathFileMeta,file);
        strcpy(fileNameMetadata,pathFileMeta);

        FILE* fp = fopen(fileNameMetadata, "r+b");
        if (fp == NULL) {
            printf("No se pudo abrir el archivo de metadata %s\n", fileNameMetadata);
            return NULL;
        }
        Metadata* metadata = (Metadata*)calloc(1, sizeof(Metadata));
        fread(&metadata->nCampos, sizeof(int), 1, fp);         //esto me sirve para determinar cuanta memoria reservar y cuantos campos debera leer el archivo
        fread(&metadata->longitudRegistro, sizeof(int), 1, fp);   //esto me sirve para detetminar la longitud del registro completo y pasarsela a la tabla hash

        metadata->campos = (Campos*)calloc(metadata->nCampos, sizeof(Campos));  //reservo espacio en memoria para el arreglo campos y poder leer la metadata

        fread(metadata->campos, sizeof(Campos), metadata->nCampos, fp); //leo y cargo la metadata
        fclose(fp);

        TablaHash th = th_crear(MAX_CAPACIDAD_TABLA, &fnHash, fileName, metadata->longitudRegistro, metadata);
        return th;  
    #endif
}

char** lectorDeCampos(Metadata* metadata,char* registro){

    int offset = 0;
    char** campos = (char**)calloc(metadata->nCampos, sizeof(char*));
    for (int i = 0; i < metadata->nCampos; i++) {
        // Reservamos un byte extra para el carácter nulo
        campos[i] = (char*)calloc(metadata->campos[i].longitud + 1, sizeof(char));
        // Copiamos los datos desde el registro
        memcpy(campos[i], registro + offset, metadata->campos[i].longitud);
        // Nos aseguramos de que la cadena esté terminada con '\0'
        campos[i][metadata->campos[i].longitud] = '\0';
        offset += metadata->campos[i].longitud;
    }
    return campos;
}

char* escritorDeCampos(Metadata* metadata, char** campos){
    
    int offset = 0;
    char* registro = (char*)calloc(metadata->longitudRegistro, sizeof(char));
    for (int i = 0; i < metadata->nCampos; i++) {
        memcpy(registro + offset, campos[i], metadata->campos[i].longitud);
        offset += metadata->campos[i].longitud;
    }
    return registro;
}