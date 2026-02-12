import ctypes
import os
import platform
import threading
import time
import tkinter as tk
from tkinter import ttk, messagebox
from PIL import Image, ImageTk

# DEFINIÇÃO DA ESTRUTURA (IDÊNTICA AO C)
class DadosMandelbrot(ctypes.Structure):
    _fields_ = [
        ("largura", ctypes.c_int),
        ("altura", ctypes.c_int),
        ("max_iteracoes", ctypes.c_int),
        ("x_min", ctypes.c_double),
        ("x_max", ctypes.c_double),
        ("y_min", ctypes.c_double),
        ("y_max", ctypes.c_double),
        ("pixels", ctypes.POINTER(ctypes.c_uint8))
    ]

#CARREGANDO DA BIBLIOTECA
def carregar_biblioteca():
    # Busca a DLL na mesma pasta deste arquivo .py
    diretorio_atual = os.path.dirname(os.path.abspath(__file__))
    nome_dll = "mandelbrot.dll" if platform.system() == "Windows" else "mandelbrot.so"
    caminho_lib = os.path.join(diretorio_atual, nome_dll)
    
    if not os.path.exists(caminho_lib):
        return None, caminho_lib

    try:
        # Carrega a DLL
        biblioteca = ctypes.CDLL(caminho_lib)
        
        # Configura as funções da biblioteca
        biblioteca.calcular_mandelbrot.restype = ctypes.POINTER(DadosMandelbrot)
        biblioteca.calcular_mandelbrot.argtypes = [
            ctypes.c_int, ctypes.c_int,       # largura, altura
            ctypes.c_double, ctypes.c_double, # x_min, x_max
            ctypes.c_double, ctypes.c_double, # y_min, y_max
            ctypes.c_int                      # max_iteracoes
        ]
        biblioteca.liberar_dados_mandelbrot.argtypes = [ctypes.POINTER(DadosMandelbrot)]
        
        return biblioteca, caminho_lib
    except Exception as e:
        return None, str(e)

# Tenta carregar a biblioteca globalmente
lib, info_lib = carregar_biblioteca()

#INTERFACE
class AplicativoMandelbrot:
    def __init__(self, raiz):
        self.raiz = raiz
       self.raiz.title("Mandelbrot Explorer - C + Python ")
        
        # Dimensões e Coordenadas Iniciais
        self.largura, self.altura = 800, 600
        self.x_min, self.x_max = -2.0, 1.0
        self.y_min, self.y_max = -1.2, 1.2
        self.foto = None
        
        self.configurar_ui()

        # COMANDOS DO MOUSE PARA NAVEGAÇÃO
        self.canvas.bind("<Button-1>", lambda e: self.ajustar_zoom(e, 0.5))  # Esq: Zoom In
        self.canvas.bind("<Button-3>", lambda e: self.ajustar_zoom(e, 2.0))  # Dir: Zoom Out
        
        
        if lib:
            self.desenhar_fractal()
        else:
            messagebox.showerror("Erro de Carregamento", 
                f"Não foi possível carregar a DLL.\nCaminho: {info_lib}\n\n"
                "Verifique se a arquitetura (32/64 bits) do Python e da DLL são iguais.")

    def configurar_ui(self):
        # Painel de Controles
        painel = ttk.Frame(self.raiz, padding="5")
        painel.pack(side=tk.TOP, fill=tk.X)

 ttk.Label(painel, text="Iterações:").pack(side=tk.LEFT, padx=5)
        self.slider = ttk.Scale(painel, from_=50, to=10000, orient=tk.HORIZONTAL)
        self.slider.set(256)
        self.slider.pack(side=tk.LEFT, fill=tk.X, expand=True, padx=5)

        ttk.Button(painel, text="Calcular", command=self.desenhar_fractal).pack(side=tk.LEFT, padx=2)
        ttk.Button(painel, text="Redefinir", command=self.reset).pack(side=tk.LEFT, padx=2)
 # Legenda corrigida (alinhada dentro da função)
        ttk.Label(painel, text=" Botão Esq: Zoom In | Botão Dir: Zoom Out", 
                foreground="#f122be", font=("Arial", 9, "bold")).pack(side=tk.LEFT, padx=10)
    
        # Área do Desenho
        self.canvas = tk.Canvas(self.raiz, width=self.largura, height=self.altura, bg="black", cursor="cross")
        self.canvas.pack()

        # Barra de Status
        self.status_var = tk.StringVar(value="Pronto")
        ttk.Label(self.raiz, textvariable=self.status_var, relief=tk.SUNKEN, anchor=tk.W).pack(side=tk.BOTTOM, fill=tk.X)

 # Barra de Status
        self.status_var = tk.StringVar(value="Pronto")
        ttk.Label(self.raiz, textvariable=self.status_var, relief=tk.SUNKEN, anchor=tk.W).pack(side=tk.BOTTOM, fill=tk.X)

    def ajustar_zoom(self, evento, fator):
        """ Altera as coordenadas baseadas no ponto clicado e recalcula """
        largura_real = self.x_max - self.x_min
        altura_real = self.y_max - self.y_min
        
        clique_x = self.x_min + (evento.x / self.largura) * largura_real
        clique_y = self.y_min + (evento.y / self.altura) * altura_real

        nova_largura = largura_real * fator
        nova_altura = altura_real * fator

        self.x_min = clique_x - nova_largura / 2
        self.x_max = clique_x + nova_largura / 2
        self.y_min = clique_y - nova_altura / 2
        self.y_max = clique_y + nova_altura / 2

        self.desenhar_fractal()

    def desenhar_fractal(self):
        if not lib: return
        self.status_var.set("Calculando no motor em C...")
        # Roda o cálculo em uma thread para não travar a janela
        threading.Thread(target=self._processar_calculo, daemon=True).start()

    def _processar_calculo(self):
        inicio = time.time()
        max_it = int(self.slider.get())

        # Chama a função do C
        ponteiro = lib.calcular_mandelbrot(
            self.largura, self.altura,
            self.x_min, self.x_max,
            self.y_min, self.y_max,
            max_it
        )
 if ponteiro:
            dados = ponteiro.contents
            tamanho_buffer = dados.largura * dados.altura * 3
            pixels_brutos = ctypes.string_at(dados.pixels, tamanho_buffer)
            imagem = Image.frombytes("RGB", (dados.largura, dados.altura), pixels_brutos)
            lib.liberar_dados_mandelbrot(ponteiro)
            
            tempo_total = time.time() - inicio
            self.raiz.after(0, self._renderizar_na_tela, imagem, tempo_total)


    def _renderizar_na_tela(self, imagem, tempo):
        self.foto = ImageTk.PhotoImage(imagem)
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.foto)
        self.status_var.set(f"Calculado em {tempo:.4f} segundos")

    def reset(self):
        self.x_min, self.x_max = -2.0, 1.0
        self.y_min, self.y_max = -1.2, 1.2
        self.slider.set(256)
        self.desenhar_fractal()

if __name__ == "__main__":
    root = tk.Tk()
    app = AplicativoMandelbrot(root)
    root.mainloop()
