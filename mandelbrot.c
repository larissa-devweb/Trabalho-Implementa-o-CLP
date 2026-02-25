#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "mandelbrot.h"

/* FUNÇÃO: aplicar_cores_avancadas
  OBJETIVO: Implementar coloração por histograma (Histogram Coloring).
- Esta técnica requer três iterações sobre todos os pixels  da imagem. Em Python, isso causaria um gargalo absurdo.
Em C, aproveitamos a localidade de cache para processar milhões de operações por segundo.
 */
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
    
    // Segundo passo: calcular histograma acumulado/Normalização do total de pontos fora do conjunto.
    double total = 0;
    for(int i = 0; i < max_iter; i++) {
        total += histograma[i];
    }
    
    // Terceiro passo: criar mapa de cores baseado no histograma
  // hue = Cálculo do HUE (matiz) acumulado.
  // Isso cria uma transição suave de cores, evitando o efeito de "degraus" 
    double* hue = (double*)malloc(max_iter * sizeof(double));
    double acumulado = 0;
    for(int i = 0; i < max_iter; i++) {
        acumulado += histograma[i] / total;
        hue[i] = acumulado;
    }
    
    // Quarto passo: aplicar cores nos pixels/ Renderização final no buffer RGB
    for(int i = 0; i < largura * altura; i++) {
        int idx = i * 3;
        if(iteracoes[i] == max_iter) {
            // // Pontos que não escaparam (dentro do conjunto) são pintados de preto.
            pixels[idx] = pixels[idx+1] = pixels[idx+2] = 0;
        } else {
            // Fora do conjunto - cores baseadas no histograma
            double t = hue[iteracoes[i]];
            
            //formulas para o Gradiente (azul -> verde -> vermelho)
            pixels[idx] = (uint8_t)(9 * (1-t) * t * t * t * 255);     // Vermelho
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
  // Alocação do buffer de pixels: Será lido pelo Python como uma imagem RGB.
    dados->pixels = (uint8_t*)malloc(largura * altura * 3);
    if (!dados->pixels) {
        free(dados);
        return NULL;
    }
    
    //Array temporário para guardar o número de iterações de cada coordenada.
    int* iteracoes = (int*)malloc(largura * altura * sizeof(int));
    if (!iteracoes) {
        free(dados->pixels);
        free(dados);
        return NULL;
    }
    
    // Cálculo do fractal / calculo delta para mapear os pixels da tela ao plano complexo
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
          // Verificamos se a magnitude excede 2 (zx² + zy² < 4)
            while (zx * zx + zy * zy < 4.0 && iteracao < max_iteracoes) {
                double temp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = temp;
                iteracao++;
            }
            
        
    //Armazena o resultado bruto para o pós-processamento de cores.
            iteracoes[y * largura + x] = iteracao;
        }
    }
    
    // Aplica cores usando histograma
    aplicar_cores_avancadas(dados->pixels, iteracoes, largura, altura, max_iteracoes);
    
    // Libera array de iterações
    free(iteracoes);
    
    return dados;   // Retorna o ponteiro para o Python (via ctypes ou Python.h).
}

void liberar_dados_mandelbrot(DadosMandelbrot* dados) {
    if (dados) {
        free(dados->pixels);
        free(dados);
    }
}
