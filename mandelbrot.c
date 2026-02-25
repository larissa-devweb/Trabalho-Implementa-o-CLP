#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mandelbrot.h"

// Função interna para coloração suave (Smooth Coloring)
// DEPENDE do cálculo de logaritmos complexos que só o C processa com eficiência.
void calcular_cores_suaves(uint8_t* pixels, int largura, int altura, int max_iter, int* iters, double* radii) {
    // 1. Criar Histograma para balanceamento de cores (Evita áreas "mortas")
    double* histograma = (double*)calloc(max_iter, sizeof(double));
    for (int i = 0; i < largura * altura; i++) {
        if (iters[i] < max_iter) histograma[iters[i]]++;
    }

    double total = 0;
    for (int i = 0; i < max_iter; i++) total += histograma[i];

    // 2. Mapeamento de Cores Dinâmico
    for (int i = 0; i < largura * altura; i++) {
        int idx = i * 3;
        if (iters[i] >= max_iter) {
            pixels[idx] = pixels[idx+1] = pixels[idx+2] = 0; // Interior (Preto)
        } else {
            // Mapeamento baseado em densidade (Histograma)
            double t = 0;
            for (int j = 0; j <= iters[i]; j++) t += histograma[j] / total;

            // FÓRMULA DE COR SENOIDAL (Dinâmica e Profissional)
            // A dependência aqui é a precisão de ponto flutuante do C.
            pixels[idx]     = (uint8_t)(127.5 * (1.0 + sin(t * 6.28 + 0.0))); // R
            pixels[idx + 1] = (uint8_t)(127.5 * (1.0 + sin(t * 6.28 + 2.1))); // G
            pixels[idx + 2] = (uint8_t)(127.5 * (1.0 + sin(t * 6.28 + 4.2))); // B
        }
    }
    free(histograma);
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
    
    // Alocação de Memória Gerenciada pelo C (Dependência: Python precisa que o C libere isso depois)
    DadosMandelbrot* dados = (DadosMandelbrot*)malloc(sizeof(DadosMandelbrot));
    if (!dados) return NULL;
    
    dados->largura = largura;
    dados->altura = altura;

     // Aloca memória para pixels (3 bytes por pixel: RGB)
  // Alocação do buffer de pixels: Será lido pelo Python como uma imagem RGB.
    dados->pixels = (uint8_t*)malloc(largura * altura * 3);
    
    // Arrays auxiliares para o cálculo de suavização
    int* iters = (int*)malloc(largura * altura * sizeof(int));
    
    if (!dados->pixels || !iters) {
        if (dados->pixels) free(dados->pixels);
        free(dados);
        return NULL;
    }

    double passo_x = (x_max - x_min) / largura;
    double passo_y = (y_max - y_min) / altura;

    // Loop de Cálculo (Cores Suaves e Escape de Mandelbrot)
    for (int y = 0; y < altura; y++) {
        for (int x = 0; x < largura; x++) {
            double zx = 0.0, zy = 0.0;
            double cx = x_min + x * passo_x;
            double cy = y_min + y * passo_y;
            int i = 0;

             // Algoritmo de Mandelbrot
          // FÓRMULA DE ESCAPE: Z = Z² + C
          // Verificamos se a magnitude excede 2 (zx² + zy² < 16)
            // Raio de escape maior (4.0 -> 16.0) para melhorar a suavização das cores
            while (zx * zx + zy * zy < 16.0 && i < max_iteracoes) {
                double temp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = temp;
                i++;
            }
            iters[y * largura + x] = i;
        }
    }

    // Aplica a coloração avançada antes de retornar para o Python
    calcular_cores_suaves(dados->pixels, largura, altura, max_iteracoes, iters, NULL);

    free(iters);
    return dados;
}

void liberar_dados_mandelbrot(DadosMandelbrot* dados) {
    if (dados) {
        free(dados->pixels);
        free(dados);
    }
}
