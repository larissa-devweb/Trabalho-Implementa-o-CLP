# Variáveis
CC = gcc
CFLAGS = -O3 -march=native -fPIC -shared -fopenmp
SRC = mandelbrot.c
PYTHON = python

# Ajuste de Extensão (Windows vs Linux)
ifeq ($(OS), Windows_NT)
    LIB = mandelbrot.dll
    RM = del /f
else
    LIB = mandelbrot.so
    RM = rm -f
    PYTHON = python3
endif

# 1. Entrada para geração do executável (Compilação)
compilar:
	$(CC) $(CFLAGS) $(SRC) -o $(LIB) -lm

# 2. Entrada para execução (Caso de Estudo)
executar: compilar
	$(PYTHON) main.py

# Limpeza
clean:
	$(RM) $(LIB)
