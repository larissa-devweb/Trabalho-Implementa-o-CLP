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

-Para a ponte entre o C e o Python ser construida, pela Biblioteca de Vínculo Dinâmico (DLL):

2.  gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
3. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
4. python main.py

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

    Zoom:Botão Esquerdo: Centraliza e aproxima (Zoom In).
    Botão Direito: Afasta a câmera (Zoom Out).
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

 ## LINK DO VIDEO em drive DA MINHA COMPILAÇAO: 

https://drive.google.com/file/d/1dp-CMi_KCztcE87JhbOF8lMMpRK8VPL9/view?usp=sharing

 obs: tomei liberdade de colocar o link, para caso por algum motivo não funcionar no seu PC. É minha prova que segui os passos e deu certo. 

## Imagens da minha compilação:

##  Interface :

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/4198e9f7-793a-4d85-84cb-00fe02c06ee1" />

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/b87a08d5-797d-4fa8-a7c4-cbc92afa04cd" />


### Se modificar o zoom:

## Ao clica no botão esquerdo do mouse, se amplia/ da zoom:

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/000469c6-f4e6-47af-babe-828806194247" />

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/52cb03e1-9a56-4466-a52b-14ee101a4d05" />

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/379c6db6-1bae-412d-9ec9-d56786099942" />


## Ao clica no botão direito do mouse, recua/ diminue zoom:

<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/c61589e9-60a0-4279-b1a8-9447a4446323" />


<img width="1366" height="770" alt="image" src="https://github.com/user-attachments/assets/e47b9dee-2894-4d91-901d-b3833b991e6c" />





