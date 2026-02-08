
#include <stdlib.h>
#include <math.h>
#include "mandelbrot.h"

DadosMandelbrot* calcular_mandelbrot(int largura, int altura, 
                                     double x_min, double x_max, 
                                     double y_min, double y_max, 
                                     int max_iteracoes) {


                        //aloca estrutura de dados
    DadosMandelbrot* dados = (DadosMandelbrot*)malloc(sizeof(DadosMandelbrot));
    if (!dados) return NULL;
                        //inicializa parametros
    dados->largura = largura;
    dados->altura = altura;
    dados->max_iteracoes = max_iteracoes;
    dados->x_min = x_min;
    dados->x_max = x_max;   
    dados->y_min = y_min;
    dados->y_max = y_max;
                        //aloca memoria para pixels (3 bytes por pixel: RGB)
    dados->pixels = (uint8_t*)malloc(largura * altura * 3);
    if (!dados->pixels) {
        free(dados);
        return NULL;

                                     }
              }

//calcula o fractal de Mandelbrot 
double passo_x = (x_max - x_min) / largura;
double passo_y = (y_max - y_min) / altura;

for (int y = 0; y < altura; y++) {
    for (int x = 0; x < largura; x++) {
        double zx = 0.0;
        double zy = 0.0;
        double cx = x_min + x * passo_x;
        double cy = y_min + y * passo_y;

        int iteracao = 0;

 // Algoritmo de Mandelbrot
        while (zx * zx + zy * zy < 4.0 && iteracao < max_iteracoes) {
            double temp = zx * zx - zy * zy + cx;
            zy = 2.0 * zx * zy + cy;
            zx = temp;
            iteracao++;
        }
        // Mapeia iteracoes para cores
        int indice = (y * largura + x) * 3;
       
       if( iteracao == max_iteracoes) {
            // Ponto dentro do conjunto (preto)
            dados->pixels[indice] = 0;
            dados->pixels[indice + 1] = 0;
            dados->pixels[indice + 2] = 0;
        } else {
            // Ponto fora do conjunto (cores baseadas nas iteracoes)
            double t = (double)iteracao / max_iteracoes;
            dados->pixels[indice] = (uint8_t)(9 * (1 - t) * t * t * t * 255);
            dados->pixels[indice + 1] = (uint8_t)(15 * (1 - t) * (1 - t) * t * t * 255);
            dados->pixels[indice + 2] = (uint8_t)(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
        }
    
        
    }
    
    return dados;
}

void liberar_dados_mandelbrot(DadosMandelbrot* dados) {
    if (dados) {
        free(dados->pixels);
        free(dados);
    }
}
