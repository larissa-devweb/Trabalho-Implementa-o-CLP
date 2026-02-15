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
OU , SE DER ERRO:
gcc -O3 -shared mandelbrot.c -o mandelbrot.dll

4. python main.py

# dica: coloque o zip do projeto no disco C e use caminho da pasta MAIS INTERNA para pôr no cd "C:\Trabalho-Implementa-o-CLP-main\Trabalho-Implementa-o-CLP-main"

## Se for compilar e der erro, faça:  Apaga a DLL antiga se ela existir para não haver conflito 

del mandelbrot.dll
gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
python main.py

## OBSERVAÇÃO : 

Se ainda tiver erro de não reconhecer caminho cd e/ou main.py, tente forçar a leitura de 64 bits ou 32 bits abaixo:

gcc -m64 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

## OU 

gcc -m32 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

## Se , ainda sim, o GCC disser que não reconhece -m64 ou -m32, significa que o seu instalador do MinGW só suporta uma arquitetura. 
Nesse caso, você terá que baixar a versão do Python que combine com seu GCC, provavelmente o seu é de 64bits e precisa ser o de 32bits: https://www.python.org/downloads/windows/

DICA para saber se seu pyhon é 64 ou 32: python -c "import struct; print(struct.calcsize('P') * 8)"


## Não esqueça que, ao baixar novo python, precisa fazer no terminal :

1. python -m pip install Pillow
2. cd "caminho"
3. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
4. python main.py

## EXPLICAÇÃO:

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

## Imagens da minha compilação:

##  Interface :

<img width="1390" height="736" alt="Interface" src="https://github.com/user-attachments/assets/3e1dc3ad-cb80-4a32-bf58-6d67b0ea242d" />



## Se modificar o zoom:

Ao clica no botão esquerdo do mouse, se amplia/ da zoom:

<img width="764" height="588" alt="FotoBotaoEsquerdoZoom" src="https://github.com/user-attachments/assets/4eca9f03-c379-4a6e-a944-a831bb3eaf50" />

Ao clica no botão direito do mouse, recua/ diminue zoom:

<img width="1366" height="768" alt="botaoDireito" src="https://github.com/user-attachments/assets/306c685a-0ff0-4d9e-b6e7-638218ff0928" />


