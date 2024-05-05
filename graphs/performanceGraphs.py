import pandas as pd
import matplotlib.pyplot as plt
import os

script_dir = os.path.dirname(__file__)
graph_folder = os.path.join(script_dir, 'graphs')
if not os.path.exists(graph_folder):
    os.makedirs(graph_folder)

# Dados
data = {
    "Computer": ["mac", "mac", "mac", "mac", "mac", "windows", "windows", "windows", "windows", "windows"],
    "File": ["ft_03", "ft_06", "ft_20", "ft_80", "ft_1000", "ft_03", "ft_06", "ft_20", "ft_80", "ft_1000"],
    "SEQ": [0.00124, 0.00604, 0.01472, 0.19112, 70.64145, 0.00196, 0.00760, 0.01981, 0.45939, 267.47008],
    "PC1": [0.00133, 0.00511, 0.01508, 0.20712, 70.85157, 0.00287, 0.00896, 0.02613, 0.44095, 266.53309],
    "PC2": [0.00153, 0.00290, 0.00968, 0.10778, 36.49682, 0.00212, 0.00605, 0.01350, 0.23632, 138.53338],
    "PC4": [0.00051, 0.00397, 0.00707, 0.06314, 18.94992, 0.00115, 0.00336, 0.00777, 0.13144, 94.37913],
    "PC8": [0.00056, 0.00158, 0.00305, 0.04863, 15.47075, 0.00122, 0.00178, 0.00416, 0.07491, 67.32317],
    "PC16": [0.00046, 0.00144, 0.00328, 0.04771, 15.34645, 0.00146, 0.00199, 0.00389, 0.08647, 47.65842],
    "PC32": [0.00047, 0.00155, 0.00357, 0.05022, 15.16807, 0.00116, 0.00218, 0.00439, 0.06439, 47.56434]
}

# Criar um DataFrame
df = pd.DataFrame(data)
print(df)

speedup_data = {
    "File": df["File"],
    "Computer": df["Computer"]
}

# Calculando o Speedup (S)
for i in [1, 2, 4, 8, 16, 32]:
    if f"PC{i}" in df.columns:
        speedup_data[f"Speedup_PC{i}"] = df["SEQ"] / df[f"PC{i}"]
speedup_df = pd.DataFrame(speedup_data)
print(speedup_df)

mac_speedup_df = speedup_df[speedup_df["Computer"] == "mac"]
windows_speedup_df = speedup_df[speedup_df["Computer"] == "windows"]

# Gráfico para Mac
plt.figure(figsize=(10, 6))

for file, group in mac_speedup_df.groupby("File"):
    plt.plot([1, 2, 4, 8, 16, 32], group.iloc[0, 2:], marker='o', label=file)

plt.xlabel('Número de threads (p)')
plt.ylabel('Speedup (S)')
plt.title('Evolução do Speedup (S) em função de p para Mac')
plt.legend(title='Ficheiro')
plt.grid(True)
plt.ylim(0, 6) 
plt.xticks([1, 2, 4, 8, 16, 32])
plt.tight_layout()
plt.savefig('graphs/Mac_Speedup.png')

# Gráfico para Windows
plt.figure(figsize=(10, 6))

for file, group in windows_speedup_df.groupby("File"):
    plt.plot([1, 2, 4, 8, 16, 32], group.iloc[0, 2:], marker='o', label=file)

plt.xlabel('Número de threads (p)')
plt.ylabel('Speedup (S)')
plt.title('Evolução do Speedup (S) em função de p para Windows')
plt.legend(title='Ficheiro')
plt.grid(True)
plt.ylim(0, 8) 
plt.xticks([1, 2, 4, 8, 16, 32])
plt.tight_layout()
plt.savefig('graphs/Windows_Speedup.png')

# Separar os dados para computadores Mac e Windows
mac_df = df[df["Computer"] == "mac"]
windows_df = df[df["Computer"] == "windows"]

# Gráfico para Mac
plt.figure(figsize=(12, 6))
for file, group in mac_df.groupby("File"):
    if file != "ft_1000":
        plt.plot([1, 2, 4, 8, 16, 32], group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Mac em função do número de threads')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 0.25) 
plt.savefig('graphs/Mac_Performance.png')

# Gráfico para Mac (ft_1000)
plt.figure(figsize=(12, 6))
for file, group in mac_df.groupby("File"):
    if file == "ft_1000":
        plt.plot([1, 2, 4, 8, 16, 32], group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Mac em função do número de threads para ft_1000')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 80)
plt.savefig('graphs/Mac_Performance_ft1000.png')

# Gráfico para Windows
plt.figure(figsize=(12, 6))
for file, group in windows_df.groupby("File"):
    if file != "ft_1000":
        plt.plot([1, 2, 4, 8, 16, 32], group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Windows em função do número de threads')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 0.5)  # Definindo o limite do eixo y para o zoom
plt.savefig('graphs/Windows_Performance.png')

# Gráfico para Windows (ft_1000)
plt.figure(figsize=(12, 6))
for file, group in windows_df.groupby("File"):
    if file == "ft_1000":
        plt.plot([1, 2, 4, 8, 16, 32], group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Windows em função do número de threads para ft_1000')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 300)
plt.savefig('graphs/Windows_Performance_ft1000.png')

# Gráfico para o tempo de execução sequencial
plt.figure(figsize=(16, 6))

# Gráfico para Mac
plt.subplot(1, 2, 1)
plt.plot(df[df['Computer'] == 'mac']["File"], df[df['Computer'] == 'mac']["SEQ"], marker='o', label="SEQ mac", color='black', linestyle='dashed')

plt.xlabel("Ficheiro")
plt.ylabel("Tempo de Execução (Segundos)")
plt.title("Mac: Tempo de Execução Sequencial")
plt.xticks(rotation=45)
plt.yscale('log')
plt.legend()
plt.grid(True)

# Gráfico para Windows
plt.subplot(1, 2, 2)
plt.plot(df[df['Computer'] == 'windows']["File"], df[df['Computer'] == 'windows']["SEQ"], marker='o', label="SEQ windows", color='black', linestyle='dashed')

plt.xlabel("Ficheiro")
plt.ylabel("Tempo de Execução (Segundos)")
plt.title("Windows: Tempo de Execução Sequencial")
plt.xticks(rotation=45)
plt.yscale('log')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('graphs/Tempo_Execucao_Sequencial.png')
