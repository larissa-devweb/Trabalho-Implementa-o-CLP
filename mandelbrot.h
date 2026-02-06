#ifndef MANDELBROT_H
#define MANDELBROT_H

#include <stdint.h>

typedef struct {
    int largura;
    int altura;
    int max_iteracoes;
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    uint8_t* pixels; // array de pixels

} DadosMandelbrot;

//calculo

DadosMandelbrot* calcular_mandelbrot (  int largura, int altura,  double x_min, double x_max,double y_min,
                    double y_max; int max_iteracoes);

// liberar memória
void liberar_dados_mandelbrot(DadosMandelbrot* dados);

#endif
