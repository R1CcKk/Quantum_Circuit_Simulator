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

    if num_qubits  <= 3:
        print(f"Detected {num_qubits} Qubit: Drawing Bloch Sphere...")
        draw_bloch(amplitudes, num_qubits)

    else:
        print(f"Detected {num_qubits} Qubits: Drawing Probability Histogram...")
        draw_histogram(indices, probs, num_qubits)
"""
FUNCTION: draw_bloch
--------------------
This function visualizes a multi-qubit state vector by projecting each individual qubit 
onto its respective Bloch Sphere.

RATIONALE & DESIGN CHOICES:
1. DATA CONVERSION: Converts JSON-structured real/imaginary amplitudes into 
   Pythonic complex numbers to build the full computational basis state vector.

2. QUANTUM OBJECT (Qobj): I explicitly define the 'dims' parameter. For an n-qubit 
   system, QuTiP needs to know the internal structure (tensor product of n 2-level systems) 
   rather than treating it as a generic 2^n vector.

3. REDUCED DENSITY MATRIX (ptrace): In multi-qubit systems, individual qubits may 
   be entangled. I use the Partial Trace operator (.ptrace(i)) to isolate the i-th 
   qubit.

4. VISUALIZATION: 
   - Uses a dynamic subplot layout to display all qubits in a single row.
   - Utilizes the QuTiP Bloch class for high-fidelity 3D rendering of the 
     state vector or density matrix.
"""

def draw_bloch(amplitudes, num_qubits):
    full_state_vector = []
    for amp in amplitudes:
        full_state_vector.append(complex(amp['real'], amp['imag']))

    dims = [[2] * num_qubits, [1] * num_qubits]
    state = Qobj(full_state_vector, dims)

    fig = plt.figure(figsize=(5*num_qubits, 5))

    for i in range(num_qubits):
        ax = fig.add_subplot(1, num_qubits, i + 1, projection='3d')
        b = Bloch(fig=fig, axes=ax)

        rho_i = state.ptrace(i)

        b.add_states(rho_i)
        b.render()
        ax.set_title(f"Qubit {i}")

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