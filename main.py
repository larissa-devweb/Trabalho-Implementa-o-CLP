import ctypes #Permite chamar funções em C
import os
import platform
import threading #Mantém a interface (GUI) rodando enquanto o C calcula em paralelo.
import time
import tkinter as tk
from tkinter import ttk, messagebox
from PIL import Image, ImageTk #Faz a ponte entre os bytes brutos do C e uma imagem visível.

# DEFINIÇÃO DA ESTRUTURA (INTERFACE COM O C) -> espelhamento exato da 'struct' no arquivo C.
class DadosMandelbrot(ctypes.Structure):
    _fields_ = [
        ("largura", ctypes.c_int),
        ("altura", ctypes.c_int),
        ("max_iteracoes", ctypes.c_int),
        ("x_min", ctypes.c_double),
        ("x_max", ctypes.c_double),
        ("y_min", ctypes.c_double),
        ("y_max", ctypes.c_double),
        # Ponteiro para o array de pixels. Sem o POINTER, o Python não sabe
        # onde termina a struct e onde começa a imagem na memória RAM.
        ("pixels", ctypes.POINTER(ctypes.c_uint8))
    ]

# CARREGAMENTO DA BIBLIOTECA (DLL) ->(PONTE DE EXECUÇÃO)
#definir restype e argtypes, pois, Por padrão, o ctypes acha que tudo é inteiro. Se não definirmos que
# 'calcular_mandelbrot' retorna um PONTEIRO para a nossa struct, 
# o Python vai tentar ler o endereço de memória como um número comum e travar
def carregar_biblioteca():
    diretorio_atual = os.path.dirname(os.path.abspath(__file__))
    nome_dll = "mandelbrot.dll" if platform.system() == "Windows" else "mandelbrot.so"
    caminho_lib = os.path.join(diretorio_atual, nome_dll)
    
    if not os.path.exists(caminho_lib):
        caminho_lib = os.path.join(os.getcwd(), nome_dll)

    try:
        biblioteca = ctypes.CDLL(caminho_lib)
        biblioteca.calcular_mandelbrot.restype = ctypes.POINTER(DadosMandelbrot)

        # Define os tipos de entrada: Garante que o Python converta os dados 
        # corretamente antes de enviar para os registradores do processador.
        biblioteca.calcular_mandelbrot.argtypes = [
            ctypes.c_int, ctypes.c_int,
            ctypes.c_double, ctypes.c_double,
            ctypes.c_double, ctypes.c_double,
            ctypes.c_int
        ]
        biblioteca.liberar_dados_mandelbrot.argtypes = [ctypes.POINTER(DadosMandelbrot)]
        return biblioteca, caminho_lib
    except Exception as e:
        return None, str(e)

lib, info_lib = carregar_biblioteca()

# CLASSE PRINCIPAL DA INTERFACE
class AplicativoMandelbrot:
    def __init__(self, raiz):
        self.raiz = raiz
        self.raiz.title("Mandelbrot Explorer - C + Python")
        self.raiz.geometry("900x750")
        
        #Coordenadas que serão enviadas ao motor C
        self.largura, self.altura = 800, 600
        self.x_min_inicial, self.x_max_inicial = -2.0, 1.0  # Salva valores iniciais
        self.y_min_inicial, self.y_max_inicial = -1.2, 1.2
        self.x_min, self.x_max = self.x_min_inicial, self.x_max_inicial
        self.y_min, self.y_max = self.y_min_inicial, self.y_max_inicial
        self.foto = None
        self.ultimo_tempo = 0
        
        self.configurar_ui()
        
        if lib:
            self.desenhar_fractal()
        else:
            messagebox.showerror("Erro", f"DLL não encontrada!\n{info_lib}")

    def configurar_ui(self):
        
        #ABAS da interface
        self.notebook = ttk.Notebook(self.raiz)
        self.notebook.pack(fill=tk.BOTH, expand=True, padx=5, pady=5)
        
        # Aba 1: Visualização do Fractal
        self.aba_visual = ttk.Frame(self.notebook)
        self.notebook.add(self.aba_visual, text="🌀 Fractal")
        
        # Aba 2: Controles
        self.aba_controles = ttk.Frame(self.notebook)
        self.notebook.add(self.aba_controles, text="⚙️ Controles")
        
        # Aba 3: Informações
        self.aba_info = ttk.Frame(self.notebook)
        self.notebook.add(self.aba_info, text="ℹ️ Info")
 
        # ABA VISUAL - Canvas do Fractal
        # Frame para centralizar o canvas
        frame_canvas = ttk.Frame(self.aba_visual)
        frame_canvas.pack(expand=True, fill=tk.BOTH, padx=10, pady=10)
        
        self.canvas = tk.Canvas(frame_canvas, 
                               width=self.largura, 
                               height=self.altura, 
                               bg="black", 
                               cursor="cross",
                               highlightthickness=1,
                               highlightbackground='gray')
        self.canvas.pack(expand=True)
        
        # COMANDOS DO MOUSE PARA NAVEGAÇÃO
        self.canvas.bind("<Button-1>", lambda e: self.ajustar_zoom(e, 0.5))  # Esq: Zoom In
        self.canvas.bind("<Button-3>", lambda e: self.ajustar_zoom(e, 2.0))  # Dir: Zoom Out
        
        # BARRA DE FERRAMENTAS NA ABA VISUAL
        toolbar = ttk.Frame(self.aba_visual)
        toolbar.pack(fill=tk.X, pady=5)
        
        # Botão de reset rápido na toolbar
        ttk.Button(toolbar, text="↺ Resetar Zoom", 
                  command=self.reset_zoom,
                  width=15).pack(side=tk.LEFT, padx=5)
        
        ttk.Button(toolbar, text="↻ Reset Completo", 
                  command=self.reset_completo,
                  width=15).pack(side=tk.LEFT, padx=5)
        
        # Legenda
        ttk.Label(toolbar, 
                 text="Botão Esq: Zoom In | Botão Dir: Zoom Out", 
                 foreground="#f122be", 
                 font=("Arial", 10, "bold")).pack(side=tk.RIGHT, padx=10)
        
        # ABA CONTROLES - Parâmetros
        frame_controles = ttk.Frame(self.aba_controles, padding="30")
        frame_controles.pack(expand=True, fill=tk.BOTH)
        
        # Título
        ttk.Label(frame_controles, text="PARÂMETROS DO FRACTAL", 
                 font=('Arial', 14, 'bold')).pack(pady=(0,20))
        
        # Controle de Iterações
        frame_iter = ttk.Frame(frame_controles)
        frame_iter.pack(fill=tk.X, pady=10)
        
        ttk.Label(frame_iter, text="Número de Iterações:", 
                 font=('Arial', 11)).pack(anchor=tk.W)
        
        self.slider = ttk.Scale(frame_iter, from_=50, to=10000, 
                               orient=tk.HORIZONTAL)
        self.slider.set(256)
        self.slider.pack(fill=tk.X, pady=5)
        
        # Label para mostrar valor atual
        self.label_iteracoes = ttk.Label(frame_iter, 
                                        text=f"Valor atual: 256",
                                        font=('Arial', 10))
        self.label_iteracoes.pack(anchor=tk.E)
        
        # Atualiza label quando slider muda
        self.slider.configure(command=self.atualizar_label_iteracoes)
        
        # Separador
        ttk.Separator(frame_controles, orient='horizontal').pack(fill=tk.X, pady=20)
        
        # Frame para botões de controle
        frame_botoes_controle = ttk.Frame(frame_controles)
        frame_botoes_controle.pack(pady=10)
        
        # Botões de reset na aba controles
        ttk.Button(frame_botoes_controle, text="↺ Resetar Zoom Apenas", 
                  command=self.reset_zoom,
                  width=20).pack(pady=5)
        
        ttk.Button(frame_botoes_controle, text="↻ Reset Completo (Zoom + Iterações)", 
                  command=self.reset_completo,
                  width=30).pack(pady=5)
        
        ttk.Button(frame_botoes_controle, text="⚡ Recalcular Agora", 
                  command=self.desenhar_fractal,
                  width=20).pack(pady=5)
        
        # Status
        self.status_var = tk.StringVar(value="✅ Pronto")
        self.status_label = ttk.Label(frame_controles, textvariable=self.status_var,
                                      font=('Arial', 11, 'bold'),
                                      foreground='green')
        self.status_label.pack(pady=20)
        
        # ABA INFO - Estatísticas
        frame_stats = ttk.Frame(self.aba_info, padding="40")
        frame_stats.pack(expand=True, fill=tk.BOTH)
        
        # Título
        ttk.Label(frame_stats, text="📊 ESTATÍSTICAS EM TEMPO REAL", 
                 font=('Arial', 14, 'bold')).pack(pady=(0,30))
        
        # Frame com borda para as estatísticas
        stats_box = ttk.Frame(frame_stats, relief='solid', borderwidth=2, padding="25")
        stats_box.pack(fill=tk.BOTH, expand=True)
        
        # Texto das estatísticas
        self.stats_text = tk.StringVar()
        stats_label = ttk.Label(stats_box, textvariable=self.stats_text,
                               justify=tk.LEFT, 
                               font=('Courier', 12),
                               background='#f0f0f0',
                               relief='sunken',
                               padding=20)
        stats_label.pack(expand=True, fill=tk.BOTH)
        
        # Separador
        ttk.Separator(frame_stats, orient='horizontal').pack(fill='x', pady=30)
        
        # Informações do Projeto
        frame_projeto = ttk.Frame(frame_stats)
        frame_projeto.pack(fill=tk.X)
        
        ttk.Label(frame_projeto, text="SOBRE O PROJETO", 
                 font=('Arial', 12, 'bold')).pack(pady=10)
        
        info_text = """
        • Python: Interface gráfica e controle do usuário
        • C: Cálculos matemáticos de alta performance
        • Integração: ctypes (biblioteca compartilhada)
        • Otimizações: Histograma de cores
        • Disciplina: Conceitos de Linguagens de Programação
        • 5º Período - Ciência da Computação
        """
        
        ttk.Label(frame_projeto, text=info_text, 
                 justify=tk.LEFT,
                 font=('Arial', 10)).pack()
        
        # Inicializa estatísticas
        self.atualizar_stats()

    def atualizar_label_iteracoes(self, valor):
        """Atualiza o label com o valor do slider"""
        self.label_iteracoes.config(text=f"Valor atual: {int(float(valor))}")

    def atualizar_stats(self):
        """Atualiza as informações na aba Info"""
        centro_x = self.x_min + (self.x_max - self.x_min) / 2
        centro_y = self.y_min + (self.y_max - self.y_min) / 2
        zoom = 1.0 / (self.x_max - self.x_min)
        largura_range = self.x_max - self.x_min
        altura_range = self.y_max - self.y_min
        
        stats = f"""
╔════════════════════════════════════════════╗
║     DADOS DO FRACTAL ATUAL                 ║
╠════════════════════════════════════════════╣
║                                            ║
║  Zoom: {zoom:<15.2e}x                      ║
║  Centro X: {centro_x:<15.6f}               ║
║  Centro Y: {centro_y:<15.6f}               ║
║                                            ║
║  X: [{self.x_min:.6f}, {self.x_max:.6f}]   ║
║  Y: [{self.y_min:.6f}, {self.y_max:.6f}]   ║
║                                            ║
║  Largura: {largura_range:<15.6f}           ║
║  Altura:  {altura_range:<15.6f}            ║
║                                            ║
║  Iterações: {int(self.slider.get()):<12}   ║
║  Tempo: {self.ultimo_tempo:<12.4f}s        ║
║  Resolução: {self.largura}x{self.altura:<6}║
║                                            ║
╚════════════════════════════════════════════╝
"""
        self.stats_text.set(stats)

    def ajustar_zoom(self, evento, fator):
        """Altera as coordenadas baseadas no ponto clicado e recalcula"""
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

    def reset_zoom(self):
        """Reset APENAS o zoom, mantém iterações"""
        self.x_min, self.x_max = self.x_min_inicial, self.x_max_inicial
        self.y_min, self.y_max = self.y_min_inicial, self.y_max_inicial
        self.desenhar_fractal()
        self.status_var.set("✅ Zoom resetado")

    def reset_completo(self):
        """Reset COMPLETO: zoom + iterações volta ao padrão"""
        self.x_min, self.x_max = self.x_min_inicial, self.x_max_inicial
        self.y_min, self.y_max = self.y_min_inicial, self.y_max_inicial
        self.slider.set(256)  # Volta iterações para 256
        self.atualizar_label_iteracoes(256)
        self.desenhar_fractal()
        self.status_var.set("✅ Reset completo realizado")

    def desenhar_fractal(self):
        if not lib: return
        self.status_var.set("🔄 Calculando no motor C...")
        self.status_label.config(foreground='orange')
        threading.Thread(target=self._processar_calculo, daemon=True).start()

    def _processar_calculo(self):
        inicio = time.time()
        max_it = int(self.slider.get())
# Chamada da função C: Retorna um ponteiro para a memória alocada no C
        ponteiro = lib.calcular_mandelbrot(
            self.largura, self.altura,
            self.x_min, self.x_max,
            self.y_min, self.y_max,
            max_it
        )

        if ponteiro:
            dados = ponteiro.contents # Acessa a estrutura de dados via ponteiro
            tamanho_buffer = dados.largura * dados.altura * 3

            # MEMÓRIA COMPARTILHADA: 
            # 'string_at' lê os bytes diretamente do endereço de RAM alocado pelo C.
            # É o jeito mais rápido de transferir milhões de pixels entre linguagens.
            pixels_brutos = ctypes.string_at(dados.pixels, tamanho_buffer)
            imagem = Image.frombytes("RGB", (dados.largura, dados.altura), pixels_brutos) # O Pillow (Image) reconstrói a imagem a partir dos bytes brutos.
            lib.liberar_dados_mandelbrot(ponteiro)
            
            self.ultimo_tempo = time.time() - inicio
            self.raiz.after(0, self._renderizar_na_tela, imagem)

    def _renderizar_na_tela(self, imagem):
        self.foto = ImageTk.PhotoImage(imagem)
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, anchor=tk.NW, image=self.foto)
        
        zoom_nivel = 1.0 / (self.x_max - self.x_min)
        self.status_var.set(f"✅ Pronto - Tempo: {self.ultimo_tempo:.4f}s")
        self.status_label.config(foreground='green')
        
        # Atualiza estatísticas
        self.atualizar_stats()
        
        # Mostra informações rápidas no título
        self.raiz.title(f"Mandelbrot Explorer - Zoom: {zoom_nivel:.2e} | {int(self.slider.get())} it")

if __name__ == "__main__":
    root = tk.Tk()
    app = AplicativoMandelbrot(root)
    root.mainloop()
