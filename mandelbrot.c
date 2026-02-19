#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mandelbrot.h"


void aplicar_cores_avancadas(uint8_t* pixels, int* iteracoes, 
                             int largura, int altura, int max_iter) {
  
    // Primeiro passo: calcular histograma
    int* histograma = (int*)calloc(max_iter, sizeof(int));
    if (!histograma) return;
    
    // Conta frequência de cada iteração
    for(int i = 0; i < largura * altura; i++) {
        if(iteracoes[i] < max_iter) {
            histograma[iteracoes[i]]++;
        }
    }
    
    // Segundo passo: calcular histograma acumulado
    double total = 0;
    for(int i = 0; i < max_iter; i++) {
        total += histograma[i];
    }
    
    // Terceiro passo: criar mapa de cores baseado no histograma
    double* hue = (double*)malloc(max_iter * sizeof(double));
    double acumulado = 0;
    for(int i = 0; i < max_iter; i++) {
        acumulado += histograma[i] / total;
        hue[i] = acumulado;
    }
    
    // Quarto passo: aplicar cores nos pixels
    for(int i = 0; i < largura * altura; i++) {
        int idx = i * 3;
        if(iteracoes[i] == max_iter) {
            // Dentro do conjunto - preto
            pixels[idx] = pixels[idx+1] = pixels[idx+2] = 0;
        } else {
            // Fora do conjunto - cores baseadas no histograma
            double t = hue[iteracoes[i]];
            
            // Gradiente suave (azul -> verde -> vermelho)
            pixels[idx] = (uint8_t)(9 * (1-t) * t * t * t * 255);     // Vermelho
            pixels[idx+1] = (uint8_t)(15 * (1-t) * (1-t) * t * t * 255); // Verde
            pixels[idx+2] = (uint8_t)(8.5 * (1-t) * (1-t) * (1-t) * t * 255); // Azul
        }
    }
    
    // Libera memória
    free(histograma);
    free(hue);
}

DadosMandelbrot* calcular_mandelbrot(int largura, int altura, 
                                     double x_min, double x_max, 
                                     double y_min, double y_max, 
                                     int max_iteracoes) {
    
    // Aloca estrutura de dados
    DadosMandelbrot* dados = (DadosMandelbrot*)malloc(sizeof(DadosMandelbrot));
    if (!dados) return NULL;
    
    // Inicializa parâmetros
    dados->largura = largura;
    dados->altura = altura;
    dados->max_iteracoes = max_iteracoes;
    dados->x_min = x_min;
    dados->x_max = x_max;
    dados->y_min = y_min;
    dados->y_max = y_max;
    
    // Aloca memória para pixels (3 bytes por pixel: RGB)
    dados->pixels = (uint8_t*)malloc(largura * altura * 3);
    if (!dados->pixels) {
        free(dados);
        return NULL;
    }
    
    //  Aloca array para armazenar iterações
    int* iteracoes = (int*)malloc(largura * altura * sizeof(int));
    if (!iteracoes) {
        free(dados->pixels);
        free(dados);
        return NULL;
    }
    
    // Cálculo do fractal
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
            
        
    // MODIFICADO: Salva iteração em array separado
            iteracoes[y * largura + x] = iteracao;
        }
    }
    
    // Aplica cores usando histograma
    aplicar_cores_avancadas(dados->pixels, iteracoes, largura, altura, max_iteracoes);
    
    // Libera array de iterações
    free(iteracoes);
    
    return dados;
}

void liberar_dados_mandelbrot(DadosMandelbrot* dados) {
    if (dados) {
        free(dados->pixels);
        free(dados);
    }
}
