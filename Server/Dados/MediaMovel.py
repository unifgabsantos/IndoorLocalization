import json
import numpy as np
import matplotlib.pyplot as plt

# Carrega os dados do arquivo 0.txt
dados = []
file = open("1.txt","r")
for line in file.readlines():
    dados.append(json.loads(line)["distance"])

# Aplica o filtro de média móvel com janela de tamanho 5
janela = 5
filtro = np.ones(janela)/janela
dados_suavizados = np.convolve(dados, filtro, mode='valid')

# Plota os resultados
plt.plot(dados, 'b-', label='Dados originais')
plt.plot(dados_suavizados, 'r-', label='Dados suavizados')
plt.legend()
plt.show()
