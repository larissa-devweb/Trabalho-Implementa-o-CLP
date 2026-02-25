#include <Python.h>  // Biblioteca principal para criar extensoes em C para o Python
#include "mandelbrot.h"

/*  FUNCAO: py_calcular_mandelbrot
 - Esta funcao atua como o "tradutor" entre o Python e o C.
 - Ela recebe objetos Python, converte para tipos nativos do C, 
 - executa o calculo e converte o resultado de volta para Python.
 */
static PyObject* py_calcular_mandelbrot(PyObject* self, PyObject* args) {
    int largura, altura, max_iteracoes;
    double x_min, x_max, y_min, y_max;
    
    // Parse dos argumentos Python
    // PyArg_ParseTuple: Converte os argumentos vindos do Python (objetos) para variaveis C. 
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
    /*  PyBytes_FromStringAndSize: Pega o buffer de pixels bruto (unsigned char*) e o transforma em
    um objeto  de bytes do Python. Isso permite que o Python "leia" a imagem calculada.
     */
    PyObject* pixels = PyBytes_FromStringAndSize((char*)dados->pixels, 
                                                 largura * altura * 3);
//Py_BuildValue: Empacota o resultado em uma Tupla Python (i=int, N=objeto sem incrementar refcount).
//xria uma estrutura que o Python le como: (largura, altura, iterações, status_pixel, dados_binarios_da_imagem)
    PyObject* resultado = Py_BuildValue("(iiiiN)", 
                                    largura, altura,
                                    dados->max_iteracoes,
                                    dados->pixels ? 1 : 0,
                                    pixels);
    
    liberar_dados_mandelbrot(dados);
    
    return resultado;
}

// Metodos do modulo
/*  TABELA DE METODOS:  Define quais funcoes deste arquivo estarao visiveis quando voce der 
 'import mandelbrot' no Python.
 */
static PyMethodDef MetodosMandelbrot[] = {
    {"calcular", py_calcular_mandelbrot, METH_VARARGS, 
     "Calcula o fractal de Mandelbrot"},
    {NULL, NULL, 0, NULL}
};

// Estrutura que descreve o modulo para o interpretador Python.
static struct PyModuleDef modulomandelbrot = {
    PyModuleDef_HEAD_INIT,
    "mandelbrot",
    "Modulo para calculo do fractal de Mandelbrot",
    -1,
    MetodosMandelbrot
};

/*  INICIALIZACAO DO MODULO:  Funcao chamada uma unica vez quando o modulo e carregado.
 * O nome deve ser obrigatoriamente PyInit_<nome_do_modulo>.
 */
PyMODINIT_FUNC PyInit_mandelbrot(void) {
    return PyModule_Create(&modulomandelbrot);
}

