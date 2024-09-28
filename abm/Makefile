OUT = bin/
OBJS = ${OUT}main.o ${OUT}interface.o ${OUT}utils.o ${OUT}abm.o ${OUT}lista_disco.o ${OUT}metadata.o ${OUT}tablaHash.o
CFLAGS = -g -Wall
BINERY = ejercicio3
CC = gcc

# Detectar el sistema operativo
ifeq ($(OS),Windows_NT)   # Windows
    RM = del 
    EXE = .exe
else                      # Linux/Unix
    RM = rm -f
    EXE = 
endif

# Compilar el programa principal
all: ${OUT}${BINERY}

${OUT}${BINERY}: ${OBJS}
	${CC} -o ${OUT}${BINERY}${EXE} ${OBJS}

# Reglas para compilar cada archivo fuente
${OUT}main.o: src/main.c includes/interface.h
	${CC} ${CFLAGS} -c src/main.c -o ${OUT}main.o

${OUT}interface.o: src/interface.c includes/interface.h includes/abm.h includes/utils.h
	${CC} ${CFLAGS} -c src/interface.c -o ${OUT}interface.o

${OUT}utils.o: src/utils.c includes/utils.h
	${CC} ${CFLAGS} -c src/utils.c -o ${OUT}utils.o

${OUT}abm.o: src/abm.c includes/abm.h
	${CC} ${CFLAGS} -c src/abm.c -o ${OUT}abm.o

${OUT}lista_disco.o: src/lista_disco.c includes/listas.h includes/metadata.h
	${CC} ${CFLAGS} -c src/lista_disco.c -o ${OUT}lista_disco.o

${OUT}metadata.o: src/metadata.c includes/metadata.h includes/tablaHash.h includes/utils.h
	${CC} ${CFLAGS} -c src/metadata.c -o ${OUT}metadata.o

${OUT}tablaHash.o: src/tablaHash.c includes/tablaHash.h
	${CC} ${CFLAGS} -c src/tablaHash.c -o ${OUT}tablaHash.o

# Limpiar archivos generados
clean:
	${RM} ${OUT}${BINERY}${EXE} ${OBJS}

# Ejecutar el programa
run: ${OUT}${BINERY}
	./${OUT}${BINERY}${EXE}

# Depuración
debug: ${OUT}${BINERY}
	gdb ./${OUT}${BINERY}${EXE}

# Otros
.PHONY: all clean run debug