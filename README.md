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
- GCC (Linux/Mac) ou MinGW (Windows)
- Biblioteca Pillow (Python Imaging Library)

- python compativel com compilador C (ambos 64 bits ou 32 bits, o meu ambos sao 32 bits e deu certo assim). Isso é fundamental para rodar, Já que eles se complementam no codigo.
  Seu compilador C provavelmente é de 32bits e python 64 bits e é mais facil baixar o python 32 bits (deixei os links de dowload e explicações abaixo)

### Instalação de Dependências

#### Ubuntu/Debian: sudo apt-get update
sudo apt-get install python3-tk python3-pil python3-pil.imagetk build-essential python3-dev

### Windows: Instale o Python 3.

- Instale o MinGW
- No terminal, instale a biblioteca de imagem: pip install pillow

# Compile e execulte (uso windows nesse caso, então só posso dizer os erros e como resolver nele):

1. cd " caminho.."
  
2. gcc -O3 -shared mandelbrot.c -o mandelbrot.dll
outra opçao seria:  gcc -O3 -march=native -fPIC -shared mandelbrot.c -o mandelbrot.so

3. python main.py

# dica: coloque o zip do projeto no disco C e use caminho da pasta MAIS INTERNA para pôr no cd , como eu fiz: "C:\Trabalho-Implementa-o-CLP-main\Trabalho-Implementa-o-CLP-main"

## Se for compilar e der erro, Apague a DLL antiga se ela existir para não haver conflito . 
-FAÇA:

1. del mandelbrot.dll
2. gcc -O3 -shared -fPIC mandelbrot.c -o mandelbrot.dll
3. python main.py

## OBSERVAÇÃO : 

Se ainda tiver erro de não reconhecer caminho cd e/ou main.py, tente forçar a leitura de 64 bits ou 32 bits abaixo(seu problema será uso de 64 bits + 32 bits e não pode):

gcc -m64 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

## OU 

gcc -m32 -shared mandelbrot.c -o mandelbrot.dll
python teste_lib.py

## Se , ainda sim, o GCC disser que não reconhece -m64 ou -m32, significa que o seu instalador do MinGW só suporta uma arquitetura. 
Nesse caso, você terá que baixar a versão do Python que combine com seu GCC, provavelmente o seu python é de 64bits e precisa ser o de 32bits: https://www.python.org/downloads/windows/

## PRECISA SER AMBOS DA MESMA VERSÃO e normalmente nosso gcc é 32 bits e o python de 64 bits. Então, se apareceu erros desse tipo, apenas baixando python de 32 bits, no link acima, resolve.

* DICA para saber se seu pyhon é 64 ou 32: python -c "import struct; print(struct.calcsize('P') * 8)"

## Não esqueça que, ao baixar NOVO PYTHON, precisa fazer no terminal :

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

    Tem opçao de resetar zoom, se quiser.
    
    Dica de Performance: Quando você entrar muito fundo no fractal (Zoom alto), a imagem pode começar a ficar "lisa" ou com poucos detalhes. Quando isso acontecer, aumente o Slider de Iterações e clique novamente. Isso faz o C trabalhar mais para encontrar os detalhes escondidos

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

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/7e43a627-8d3d-4e10-a310-a9ed5d882d70" />

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/32a2e2cf-9e35-479e-b6c1-35a2372c1397" />




## Se modificar o zoom:

Ao clica no botão esquerdo do mouse, se amplia/ da zoom:

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/42ef7391-a536-4ff0-b75b-9e2ade0fdee6" />


Ao clica no botão direito do mouse, recua/ diminue zoom:

<img width="1366" height="768" alt="image" src="https://github.com/user-attachments/assets/bdc5889d-5be2-4862-911c-58abdd38f63a" />



