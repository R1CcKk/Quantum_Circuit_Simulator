import pandas as pd
import matplotlib.pyplot as plt
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
csv_path = os.path.join(script_dir, "..", "build", "benchmark_results.csv")

if not os.path.exists(csv_path):
    print(f"ERROR: Benchmark results file not found at {csv_path}")
    exit()

df = pd.read_csv(csv_path)

plt.figure(figsize=(10, 6))
plt.style.use('seaborn-v0_8-darkgrid')

plt.plot(df['qubits'], df['time_ms'], 'o-', color='#00ffcc', label='simulator')
plt.yscale('log')

plt.title("Performance Scaling: The Exponential Wall", fontsize=14)
plt.xlabel("Number of Qubits (n)", fontsize=12)
plt.ylabel("Time (ms) - Log Scale", fontsize=12)
plt.grid(True, which="both", ls="-", alpha=0.5)
plt.legend()

plt.show()