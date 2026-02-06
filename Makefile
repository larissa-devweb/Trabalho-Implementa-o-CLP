# como compilar tudo: make executar
#Para apenas compilar o código C: make compilar
#Lembre-se que no Makefile, os comandos abaixo dos nomes (como all:, compilar:) precisa começar com um Tab, não espaços.
#Se copiar e der erro de "missing separator", verificar se editor não converteu os Tabs em espaços.

# Variáveis de Compilação
CC = gcc
CFLAGS = -O3 -march=native -fPIC -shared
TARGET_LIB = mandelbrot.so
SRC = mandelbrot.c
PYTHON = python3

# Detectar Sistema Operacional para extensão da biblioteca
ifeq ($(OS), Windows_NT)
    TARGET_LIB = mandelbrot.dll
    PYTHON = python 
endif

# Alvo padrão: Compila tudo
all: compilar

# Compila a biblioteca C
compilar: $(SRC)
	@echo " Compilando a biblioteca compartilhada..."
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET_LIB)
	@echo " Compilação concluída: $(TARGET_LIB)"

# Instala dependências do Python
instalar:
	@echo "Instalando dependências (Pillow)..."
	$(PYTHON) -m pip install pillow

# Executa a aplicação
executar: compilar
	@echo " Iniciando a interface..."
	$(PYTHON) main.py

# Limpa arquivos gerados
clean:
	@echo " Limpando arquivos binários..."
	rm -f $(TARGET_LIB)
