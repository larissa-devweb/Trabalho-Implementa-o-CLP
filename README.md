# Projeto de Fractal de Mandelbrot usando Python e C

- Python: Interface gráfica
- C: Cálculos matemáticos do fractal


## Arquivos no repositório

- `mandelbrot.h`: cabeçalho em C.
- `mandelbrot.c`: implementação em C do cálculo do fractal.
- `main.py`: interface gráfica em Python usando Tkinter e PIL.
- `Makefile`: para compilar a biblioteca compartilhada.
- `README.md`

## Requisitos

### Dependências
- Python 3.8 ou superior
- GCC (Linux/Mac) ou MinGW (Windows)
- Biblioteca Pillow (Python Imaging Library)
- python compativel com GCC (ambos 64 bits ou 32 bits, o meu ambos sao 32 bits)

### Instalação de Dependências

#### Ubuntu/Debian: sudo apt-get update
sudo apt-get install python3-tk python3-pil python3-pil.imagetk build-essential python3-dev

### Windows: Instale o Python 3.

Instale o MinGW ou as Ferramentas de Compilação do Visual Studio.

No terminal, instale a biblioteca de imagem: pip install pillow

Compile e execulte:

1. cd " caminho.."
2. gcc -O3 -march=native -fPIC -shared mandelbrot.c -o mandelbrot.so
3. python main.py

Uma dica, se for recompilar e der erro, faça:  Apaga a DLL antiga se ela existir para não haver conflito -> del mandelbrot.dll
gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
 python main.py 

A aplicação abrirá uma janela exibindo o fractal. Você pode interagir da seguinte forma:

    Zoom: Clique e arraste o mouse para selecionar uma área específica. O fractal será recalculado para essa nova região.
    Como usar agora:
    Clique no botão Esquerdo do MOUSE : Centraliza e aproxima.
    Clique no botão Direito do MOUSE: Afasta a câmera (Zoom Out).
    Dica de Performance: Quando você entrar muito fundo no fractal (Zoom alto), a imagem pode começar a ficar "lisa" ou com poucos detalhes. Quando isso acontecer, aumente o Slider de Iterações e clique novamente. Isso faz o C trabalhar mais para encontrar os detalhes escondidos
    
    Slider de Iterações: Ajusta a precisão do cálculo (mais iterações = mais detalhe, porém mais processamento).
    Botão "Redefinir": Retorna à visualização inicial (coordenadas padrão).
    Botão "Calcular": Força a atualização com os parâmetros atuais do slider.

Integracao Python-C:

    O código C é compilado como uma Shared Library (.so no Linux ou .dll no Windows).
    O Python utiliza a biblioteca ctypes para carregar esse binário.
    Os parâmetros de coordenadas e iterações são passados do Python para structs em C, onde o cálculo pesado ocorre, devolvendo os dados de cores para a interface.


OBSERVAÇÃO: 

Se tiver problema de não reconhecer caminho cd e/ou main.py, tente forçar a leitura de 64 bits ou 32 bits abaixo:

gcc -m64 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

OU 

gcc -m32 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

Se , ainda sim, Se o GCC disser que não reconhece -m64 ou -m32, significa que o seu instalador do MinGW só suporta uma arquitetura. Nesse caso, você terá que baixar a versão do Python que combine com seu GCC: https://www.python.org/downloads/windows/

DICA: para saber se seu pyhon é 64 ou 32: python -c "import struct; print(struct.calcsize('P') * 8)"


Não esqueça que, ao baixar novo python, precisa fazer no terminal :

1. python -m pip install Pillow
2. cd "caminho"
3. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
4. python main.py

Imagens da miha compilação:

Interface :


Se modificar o zoom:
