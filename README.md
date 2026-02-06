# Trabalho-Implementacao-CLP

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

ou GCC: gcc -O3 -march=native -fPIC -shared mandelbrot.c -o mandelbrot.so

Rodar a aplicação : python3 main.py 
# Executar interface: 
python3 interface.py

A aplicação abrirá uma janela exibindo o fractal. Você pode interagir da seguinte forma:

    Zoom: Clique e arraste o mouse para selecionar uma área específica. O fractal será recalculado para essa nova região.

    Slider de Iterações: Ajusta a precisão do cálculo (mais iterações = mais detalhe, porém mais processamento).

    Botão "Redefinir": Retorna à visualização inicial (coordenadas padrão).

    Botão "Calcular": Força a atualização com os parâmetros atuais do slider.

Integracao Python-C:

    O código C é compilado como uma Shared Library (.so no Linux ou .dll no Windows).

    O Python utiliza a biblioteca ctypes para carregar esse binário.

    Os parâmetros de coordenadas e iterações são passados do Python para structs em C, onde o cálculo pesado ocorre, devolvendo os dados de cores para a interface.



