#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mandelbrot.h"

// Função interna para coloração suave (Smooth Coloring)
// DEPENDE do cálculo de logaritmos complexos que só o C processa com eficiência.

void aplicar_cores_avancadas(uint8_t* pixels, int* iteracoes, double* radios, 
                             int largura, int altura, int max_iter) {
    // Histogram Coloring aprimorado com interpolação linear
    double* histograma = (double*)calloc(max_iter, sizeof(double));
    if (!histograma) return;

    for (int i = 0; i < largura * altura; i++) {
        if (iteracoes[i] < max_iter) {
            histograma[iteracoes[i]] += 1.0;
        }
    }

    double total = 0;
    for (int i = 0; i < max_iter; i++) total += histograma[i];

    // Mapeamento dinâmico de cores
    for (int i = 0; i < largura * altura; i++) {
        int idx = i * 3;
        if (iteracoes[i] >= max_iter) {
            pixels[idx] = pixels[idx+1] = pixels[idx+2] = 0; // Interior
        } else {
            // LÓGICA DINÂMICA: Usa a parte fracionária da iteração (radios) 
            // para criar um degradê infinito e suave.
            double t = 0;
            for (int j = 0; j <= iteracoes[i]; j++) t += histograma[j] / total;


              // FÓRMULA DE COR SENOIDAL (Dinâmica e Profissional)
            // A dependência aqui é a precisão de ponto flutuante do C.
            // Paleta "Electric Night" (Azul Profundo -> Magenta -> Laranja)
            pixels[idx]     = (uint8_t)(255 * pow(t, 0.5) * sin(t * M_PI));         // R
            pixels[idx + 1] = (uint8_t)(255 * pow(t, 1.5) * cos(t * M_PI * 0.5));  // G
            pixels[idx + 2] = (uint8_t)(255 * pow(t, 0.2));                        // B
        }
    }
    free(histograma);
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
            pixels[idx] = (uint8_t)(9 * (1-t) * t * t * t * 255);     // Vermelho
            pixels[idx+1] = (uint8_t)(15 * (1-t) * (1-t) * t * t * 255); // Verde
            pixels[idx+2] = (uint8_t)(8.5 * (1-t) * (1-t) * (1-t) * t * 255); // Azul
        }
    }
    
    // Libera memória
    free(histograma);
    free(hue);
}

/* FUNÇÃO PRINCIPAL: calcular_mandelbrot
 OBJETIVO: O motor de cálculo intensivo (CPU Bound).
 * INTEROPERABILIDADE: Esta função é exportada para ser chamada pelo Python. 
 * Ela aloca a memória que o Python irá consumir posteriormente.
 */
DadosMandelbrot* calcular_mandelbrot(int largura, int altura, 
                                     double x_min, double x_max, 
                                     double y_min, double y_max, 
                                     int max_iteracoes) {
    
   // Alocação de Memória Gerenciada pelo C 
//(Dependência: Python precisa que o C libere isso depois)
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
          // FÓRMULA DE ESCAPE: Z = Z² + C
          // Verificamos se a magnitude excede 2 (zx² + zy² < 16)
            // Raio de escape maior (4.0 -> 16.0) para melhorar a suavização das cores

            while (zx * zx + zy * zy < 4.0 && iteracao < max_iteracoes) {
                double temp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = temp;
                iteracao++;
            }
            
            // Salva iteração em array separado
       
            iteracoes[y * largura + x] = iteracao;
        }
    }

    aplicar_cores_avancadas(dados->pixels, iteracoes, largura, altura, max_iteracoes);
    
    free(iteracoes);
    
    return dados;
}

void liberar_dados_mandelbrot(DadosMandelbrot* dados) {
    if (dados) {
        free(dados->pixels);
        free(dados);
    }
}
