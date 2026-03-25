import json
import numpy as np
import matplotlib.pyplot as plt
from qutip import Bloch, Qobj
import seaborn as sns

def visualize_quantum_data(json_path):
    with open(json_path, 'r') as f:
        data = json.load(f)

    num_qubits = data['metadata']['numQubits']
    amplitudes = data['amplitudes']

    indices = [item['index'] for item in amplitudes]
    probs = [item['prob'] for item in amplitudes]

    if num_qubits == 1:
        print("Detected 1 Qubit: Drawing Bloch Sphere...")
        draw_bloch(amplitudes)
    else:
        print(f"Detected {num_qubits} Qubits: Drawing Probability Histogram...")
        draw_histogram(indices, probs, num_qubits)


def draw_bloch(amplitudes):
    alpha = complex(amplitudes[0]['real'], amplitudes[0]['imag'] )
    beta = complex(amplitudes[1]['real'], amplitudes[1]['imag'] )

    state = Qobj([[alpha],[beta]])

    b = Bloch()
    b.add_states(state)
    b.show()
    plt.show()

def draw_histogram(indices, probs, n):
    plt.style.use('dark_background')
    plt.figure(figsize=(10, 6))

    binary_labels = [format(i, f'0{n}b') for i in indices]

    colors = sns.color_palette("viridis", len(indices))
    plt.bar(binary_labels, probs, color=colors, edgecolor='white', alpha=0.8)

    plt.title(f"Probability Distribution - {n} Qubits", color='cyan', fontsize=14)
    plt.xlabel("Basis States |q_n...q_0>")
    plt.ylabel("Probability")
    plt.ylim(0, 1.1)
    plt.grid(axis='y', linestyle='--', alpha=0.3)
    plt.show()


if __name__ == '__main__':
    visualize_quantum_data("../build/simulation_result.json")