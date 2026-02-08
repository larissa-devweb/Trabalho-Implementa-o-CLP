#include <Python.h>
#include "mandelbrot.h"

// Funcao para Python
static PyObject* py_calcular_mandelbrot(PyObject* self, PyObject* args) {
    int largura, altura, max_iteracoes;
    double x_min, x_max, y_min, y_max;
    
    // Parse dos argumentos Python
    if (!PyArg_ParseTuple(args, "iiddddi", 
                         &largura, &altura,
                         &x_min, &x_max,
                         &y_min, &y_max,
                         &max_iteracoes)) {
        return NULL;
    }
    
    // Chamando funcao C
    DadosMandelbrot* dados = calcular_mandelbrot(largura, altura, 
                                               x_min, x_max, 
                                               y_min, y_max, 
                                               max_iteracoes);
    
    if (!dados) {
        PyErr_SetString(PyExc_MemoryError, "Falha ao alocar memoria");
        return NULL;
    }
    
    // Cria objeto Python bytes com os pixels
    PyObject* pixels = PyBytes_FromStringAndSize((char*)dados->pixels, 
                                                 largura * altura * 3);

    PyObject* resultado = Py_BuildValue("(iiiiN)", 
                                    largura, altura,
                                    dados->max_iteracoes,
                                    dados->pixels ? 1 : 0,
                                    pixels);
    
    liberar_dados_mandelbrot(dados);
    
    return resultado;
}

// Metodos do modulo
static PyMethodDef MetodosMandelbrot[] = {
    {"calcular", py_calcular_mandelbrot, METH_VARARGS, 
     "Calcula o fractal de Mandelbrot"},
    {NULL, NULL, 0, NULL}
};

// Definicao do modulo
static struct PyModuleDef modulomandelbrot = {
    PyModuleDef_HEAD_INIT,
    "mandelbrot",
    "Modulo para calculo do fractal de Mandelbrot",
    -1,
    MetodosMandelbrot
};

// Inicializacao de modulo
PyMODINIT_FUNC PyInit_mandelbrot(void) {
    return PyModule_Create(&modulomandelbrot);
}

