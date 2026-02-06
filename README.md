# Trabalho-Implementacao-CLP

# Projeto de Fractal de Mandelbrot usando Python e C

Usando C e python para gerar e exibir o fractal de Mandelbrot.

## Arquivos no repositório

- `mandelbrot.h`: cabeçalho em C.
- `mandelbrot.c`: implementação em C do cálculo do fractal.
- `main.py`: interface gráfica em Python usando Tkinter e PIL.
- `Makefile`: para compilar a biblioteca compartilhada.
- `documentacao.pdf`: documentação do projeto.
   `README.md`.

## Requisitos

- Python 3.x
- PIL (Pillow) - para manipulação de imagens no Python
- GCC (para compilar o código C)

## Instalação das dependências

No Ubuntu/Debian:

```bash
sudo apt-get install python3-tk python3-pil python3-pil.imagetk

Obs: No Windows, é possível instalar o Python e depois o Pillow via pip.

Compilação e execução
Compilar a biblioteca C: make
Isso gerará o arquivo libmandelbrot.so (no Linux) ou equivalente.
Executar a aplicação: make run

Ou, manualmente: python3 main.py

Como funciona

    O código em C (mandelbrot.c) contém a função mandelbrot que calcula o fractal e retorna um array de inteiros com o número de iterações para cada pixel.

    O código em Python (main.py) usa ctypes para carregar a biblioteca compartilhada e chamar a função C.

    A interface gráfica em Tkinter exibe a imagem gerada a partir dos dados retornados pela função C.

Documentação: Consulte o arquivo documentacao.pdf para mais detalhes sobre a implementação e a interface entre as linguagens.
