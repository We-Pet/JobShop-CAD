import pandas as pd
import matplotlib.pyplot as plt

# Dados
data = {
    "Computer": ["mac", "mac", "mac", "mac", "mac", "windows", "windows", "windows", "windows", "windows"],
    "File": ["ft_03", "ft_06", "ft_20", "ft_80", "ft_1000", "ft_03", "ft_06", "ft_20", "ft_80", "ft_1000"],
    "SEQ": [0.00124, 0.00604, 0.01472, 0.20115, 70.64145, 0.00196, 0.00760, 0.01981, 0.45939, 257.13035],
    "PC1": [0.00133, 0.00511, 0.01508, 0.20712, 70.85157, 0.00287, 0.00896, 0.02613, 0.44095, None],
    "PC2": [None, None, None, None, 36.49682, 0.00212, 0.00605, 0.01350, 0.23632, None],
    "PC4": [None, None, None, None, 18.94992, 0.00115, 0.00336, 0.00777, 0.13144, None],
    "PC8": [None, None, None, None, 15.47075, 0.00122, 0.00178, 0.00416, 0.07491, None],
    "PC16": [None, None, None, None, 15.34645, 0.00146, 0.00199, 0.00389, 0.08647, None],
    "PC32": [None, None, None, None, 15.16807, 0.00116, 0.00218, 0.00439, 0.06439, None]
}

# Create a DataFrame
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

mac_speedup_df = speedup_df[speedup_df["Computer"] == "mac"]
windows_speedup_df = speedup_df[speedup_df["Computer"] == "windows"]

# Gráfico para Mac
plt.figure(figsize=(10, 6))

for file, group in mac_speedup_df.groupby("File"):
    plt.plot([1, 2, 4, 8, 16, 32], group.iloc[0, 2:], marker='o', label=file)

plt.xlabel('Número de threads (p)')
plt.ylabel('Speedup (S)')
plt.title('Evolução do Speedup (S) em função de p para Mac')
plt.legend(title='Arquivo')
plt.grid(True)
plt.xticks([1, 2, 4, 8, 16, 32])
plt.tight_layout()
plt.show()

# Gráfico para Windows
plt.figure(figsize=(10, 6))

for file, group in windows_speedup_df.groupby("File"):
    plt.plot([1, 2, 4, 8, 16, 32], group.iloc[0, 2:], marker='o', label=file)

plt.xlabel('Número de threads (p)')
plt.ylabel('Speedup (S)')
plt.title('Evolução do Speedup (S) em função de p para Windows')
plt.legend(title='Arquivo')
plt.grid(True)
plt.xticks([1, 2, 4, 8, 16, 32])
plt.tight_layout()
plt.show()

# Separate data for Mac and Windows computers
mac_df = df[df["Computer"] == "mac"]
windows_df = df[df["Computer"] == "windows"]

# Plot for Mac
plt.figure(figsize=(12, 6))
for file, group in mac_df.groupby("File"):
    if file != "ft_1000":
        plt.plot(range(1, 7), group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Mac em função do número de threads')
plt.xticks(range(1, 7), ["PC1", "PC2", "PC4", "PC8", "PC16", "PC32"])
plt.grid(True)
plt.legend()
plt.ylim(0, 0.5)  # Definindo o limite do eixo y para o zoom
plt.show()

# Plot for Mac
plt.figure(figsize=(12, 6))
for file, group in mac_df.groupby("File"):
    if file == "ft_1000":
        plt.plot(range(1, 7), group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Mac em função do número de threads para ft_1000')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 100)  # Definindo o limite do eixo y para o zoom
plt.show()

# Plot for Windows
plt.figure(figsize=(12, 6))
for file, group in windows_df.groupby("File"):
    if file != "ft_1000":
        plt.plot(range(1, 7), group.loc[:, "PC1":"PC32"].values.T, marker='o', label=file)

plt.xlabel('Número de Threads')
plt.ylabel('Tempo de Execução')
plt.title('Desempenho no Windows em função do número de threads')
plt.xticks([1, 2, 4, 8, 16, 32])
plt.grid(True)
plt.legend()
plt.ylim(0, 0.5)  # Definindo o limite do eixo y para o zoom
plt.show()

# Plot SEQ
plt.figure(figsize=(16, 6))

# Plot for mac
plt.subplot(1, 2, 1)
plt.plot(df[df['Computer'] == 'mac']["File"], df[df['Computer'] == 'mac']["SEQ"], marker='o', label="SEQ mac", color='black', linestyle='dashed')

plt.xlabel("File")
plt.ylabel("Execution Time (Seconds)")
plt.title("Mac: Sequential Execution Time")
plt.xticks(rotation=45)
plt.yscale('log')
plt.legend()
plt.grid(True)

# Plot for windows
plt.subplot(1, 2, 2)
plt.plot(df[df['Computer'] == 'windows']["File"], df[df['Computer'] == 'windows']["SEQ"], marker='o', label="SEQ windows", color='black', linestyle='dashed')

plt.xlabel("File")
plt.ylabel("Execution Time (Seconds)")
plt.title("Windows: Sequential Execution Time")
plt.xticks(rotation=45)
plt.yscale('log')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
