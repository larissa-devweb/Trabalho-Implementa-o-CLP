# Fractal de Mandelbrot

- Python: Interface gráfica
- C: Cálculos matemáticos do fractal


## Arquivos no repositório

- `mandelbrot.h`: cabeçalho em C.
- `mandelbrot.c`: implementação em C do cálculo do fractal.
- `main.py`: interface gráfica em Python usando Tkinter e PIL.
- `Makefile`: para compilar a biblioteca compartilhada.
- `README.md`

## Requisitos

- Python 3.8 (32 bits, se seu gcc for 32bits) 
- GCC (Linux/Mac) ou MinGW (Windows) em 32bits.
- Biblioteca Pillow (Python Imaging Library)

- python compativel com compilador C (ambos 32 bits ou 64bits, o meu deu certo ambos de 32bits). Isso é fundamental para rodar e é mais facil baixar o python 32 bits (deixei os links de dowload e explicações abaixo).


### Instalação de Dependências

- Primeiro passo é saber se seu seu pyhon é 64 ou 32: python -c "import struct; print(struct.calcsize('P') * 8)"

 -baixar PYTHON 32bits: https://www.python.org/downloads/windows/
-  biblioteca de imagem: pip install pillow

## MAKEFILE:

- make compilar
- make executar

-Dica: No Makefile, o espaço antes dos comandos TEM QUE SER UM TAB. Se você copiar e colar e der erro de missing separator, 
apague os espaços antes de $(CC), $(PYTHON) e $(RM) e aperte a tecla Tab

# SE MakeFile não funcionar: Compile e execulte (uso windows nesse caso, então só posso dizer os erros e como resolver nele):

- execute os seguintes comandos na raiz do projeto:

1. cd " caminho/mais/interno/da/pasta"
2. del mandelbrot.dll
3.  gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
4. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
5. python main.py

## Se for compilar e der o primeiro erro, refaça:
1. del mandelbrot.dll
2. gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
3. python main.py

## OBSERVAÇÃO : 

- Se o GCC disser que não reconhece -m64 ou -m32, significa que o seu instalador do MinGW é 32bits e python continua sendo 64bits -> PYTHON 32bits: https://www.python.org/downloads/windows/

## Não esqueça que, ao baixar NOVO PYTHON, precisa fazer no terminal :

1. python -m pip install Pillow
2. cd "caminho"
3. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
4. python main.py

## EXPLICAÇÃO:

A aplicação abrirá uma janela exibindo o fractal. Você pode interagir da seguinte forma:

    Zoom:
    Botão Direito: Centraliza e aproxima (Zoom In).
    Botão Esquerdo: Afasta a câmera (Zoom Out).
    Redefinir zoom
    
    ABA 2: 
    
    Slider de Iterações: Ajusta a precisão do cálculo (mais iterações = mais detalhe, porém mais processamento).
    Botão "Redefinir": Retorna à visualização inicial (coordenadas padrão).
    Botão "Calcular": Força a atualização com os parâmetros atuais do slider.

    ABA 3: 
      - caracteristicas do fractal

Integracao Python-C:

    O código C é compilado como uma Shared Library (.so no Linux ou .dll no Windows).
    O Python utiliza a biblioteca ctypes para carregar esse binário.
    Os parâmetros de coordenadas e iterações são passados do Python para structs em C, onde o cálculo pesado ocorre, devolvendo os dados de cores para a interface.

 ## Link do vídeo em drive da MINHA compilação: 

https://drive.google.com/file/d/1eZ-SX1NbjV-S79sieSz53obLsCR6rk1w/view?usp=sharing

 obs: tomei liberdade de colocar o link, para caso por algum motivo não funcionar no seu PC. É minha prova que segui os passos e deu certo. 

## Imagens da minha compilação:

##  Interface :

<img width="1300" height="768" alt="image" src="https://github.com/user-attachments/assets/022ee03a-41f4-4896-af5d-96752eade8f2" />


## Ao clicar no botão direito do mouse:

<img width="1000" height="768" alt="image" src="https://github.com/user-attachments/assets/a8f5886d-56df-49b1-bbb6-f8df7fc1d1ac" />

<img width="1000" height="768" alt="image" src="https://github.com/user-attachments/assets/438ae9bd-c855-4d14-8fd8-948f0ea336b8" />

<img width="1000" height="768" alt="image" src="https://github.com/user-attachments/assets/e2e32a1a-2600-495c-9c66-cdc88f1440a3" />

