# Projeto de Fractal de Mandelbrot usando Python e C

- Python: Interface gráfica
- C: Cálculos matemáticos intensivos do fractal


## Arquivos no repositório

- `mandelbrot.h`: cabeçalho em C.
- `mandelbrot.c`: implementação em C do cálculo do fractal.
- `main.py`: interface gráfica em Python usando Tkinter e PIL.
- `Makefile`: para compilar a biblioteca compartilhada.
- `documentacao.pdf`: documentação do projeto.
- `README.md`

## Requisitos

### Dependências
- Python 3.8 ou superior
- GCC (Linux/Mac) ou MinGW (Windows)
- Biblioteca Pillow (Python Imaging Library)

### Instalação de Dependências

#### Ubuntu/Debian: sudo apt-get update
sudo apt-get install python3-tk python3-pil python3-pil.imagetk build-essential python3-dev

### Windows: Instale o Python 3.

Instale o MinGW ou as Ferramentas de Compilação do Visual Studio.

No terminal, instale a biblioteca de imagem: pip install pillow

Compile e execute:

make all        # Compila a biblioteca C e prepara o ambiente
make executar   # Inicia a aplicação

Ou use os comandos individuais:

make compilar    # Compila modulo C
make instalar    # Instala dependencias
make executar    # Executa aplicacao

ou , COMO EU FAÇO, GCC: gcc -O3 -march=native -fPIC -shared mandelbrot.c -o mandelbrot.so

Eu coloquei no powershell o cd (caminho) e entao compilei o gcc e depois o python main.py
Uma dica, se for recompilar, faça:

# Apaga a DLL antiga se ela existir para não haver conflito
del mandelbrot.dll

# Compila a nova DLL (O motor do projeto)
gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll

Rodar a aplicação : python main.py 


A aplicação abrirá uma janela exibindo o fractal. Você pode interagir da seguinte forma:

    Zoom: Clique e arraste o mouse para selecionar uma área específica. O fractal será recalculado para essa nova região.
    Slider de Iterações: Ajusta a precisão do cálculo (mais iterações = mais detalhe, porém mais processamento).
    Botão "Redefinir": Retorna à visualização inicial (coordenadas padrão).
    Botão "Calcular": Força a atualização com os parâmetros atuais do slider.

Integracao Python-C:

    O código C é compilado como uma Shared Library (.so no Linux ou .dll no Windows).
    O Python utiliza a biblioteca ctypes para carregar esse binário.
    Os parâmetros de coordenadas e iterações são passados do Python para structs em C, onde o cálculo pesado ocorre, devolvendo os dados de cores para a interface.


OBSERVAÇÃO: 

Se tiver problema de não reconhecer caminho cd e/ou main.py, tente forçar a leitura de 64 bits ou 32 bits abaixo.
Se , ainda sim, Se o GCC no power shell disser que não reconhece -m64 ou -m32, significa que o seu instalador do MinGW só suporta uma arquitetura. Nesse caso, você terá que baixar a versão do Python que combine com seu GCC: https://www.python.org/downloads/windows/

DICA: para saber se seu pyhon é 64 ou 32: python -c "import struct; print(struct.calcsize('P') * 8)"

forçar compilação: 

gcc -m64 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

OU 

gcc -m32 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

Não esqueça que, ao baixar novo python, precisa fazer no terminal :

1. python -m pip install Pillow
2. cd "caminho"
3. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
4. python main.py
   
<img width="1000" height="670" alt="image" src="https://github.com/user-attachments/assets/797e5fdc-21fc-4c59-bd3d-54aeaa58ca2a" />


E, se aumentar a iteração:

<img width="600" height="560" alt="image" src="https://github.com/user-attachments/assets/38ff30b4-23dc-420e-b5ad-abbb0e6c2ea1" />
