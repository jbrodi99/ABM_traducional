#include"../includes/utils.h"

void limpiar_pantalla()
{
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
}

bool quitarFinDeLinea(char* cad){
    if(cad[strlen(cad)-1] == '\n'){
        cad[strlen(cad)-1] = '\0';
        return true;
    }
    return false;
}

void limpiarBuffer(){
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausa(){
    #ifdef _WIN32  // Windows (32-bit or 64-bit)
        system("pause");
        limpiar_pantalla();
    #else  // Cualquier otro sistema operativo
        printf("Ingrese ENTER para continuar...\n");
        getchar();
        limpiarBuffer();
        limpiar_pantalla();
    #endif
}

char** split(char* cadena,char* delimitador){

    char** cadenas; //arreglo de cadenas
    int subCadenas = 1;
    cadenas = (char**) malloc ( subCadenas * sizeof(char*));    
    char *token = strtok(cadena, delimitador);
    if(token != NULL){
        while(token != NULL){
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            // printf("Token: %s\n", token);
            *(cadenas + subCadenas - 1) = token;  
            subCadenas++;
            cadenas = (char**) realloc (cadenas, subCadenas * sizeof(char*));  
            token = strtok(NULL, delimitador);
        }
    }
    *(cadenas + subCadenas - 1) = NULL; //identificamos el final con NULL
    return cadenas;
}

int ingresoEntero()
{
    char input[100];
    fgets(input, sizeof(input), stdin);
    if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
    }
    while(strspn(input,".-0123456789")!= strlen(input) || esflotante(input) || strcmp(input,".") == 0)
    {
        printf("Ingrese un caracter valido: ");
        fgets(input, sizeof(input), stdin);
        if (input[strlen(input) - 1] == '\n') {
        input[strlen(input) - 1] = '\0';
        }
    }
    int valor = atoi(input);
    return valor;
}

bool esflotante(char val[])
{
    bool flotante = false;
    int longitud = strlen(val);
    for (int i = 0; i < longitud; i++) 
    {
        if ( val[i] == ',' || val[i] == ' ' || val[i] == '.') 
        {
            flotante = true;
        }
    }
    return flotante;
}

int longitudCampo(int longitudNombre, int longitudCampo){

    int resultado = 0;

    if(longitudCampo < longitudNombre){
        resultado = longitudNombre;
    }else{
        resultado = longitudNombre + abs(longitudCampo - longitudNombre);
    }

    return resultado;
}

char* intToString(int siguiente){
    char* codificado = (char*)calloc(5,sizeof(char));
    sprintf(codificado,"%d",siguiente);
    return codificado;
}

int parseInt(char* siguiente){
    return atoi(siguiente);
}