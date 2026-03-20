#include<cmath>
#include "../include/QubitRegister.hpp"


//The general idea is that I'm allocating a contiguous block of memory to store the state vector of the qubit register.
//for example, let's work with a 2-qubit register. The state vector will have 4 complex coefficients corresponding to the basis states |00>, |01>, |10>, and |11>.
// I initialize the state vector to represent the |00> state, which means that the coefficient for |00> is set to 1 (fully occupied) and all other coefficients are set to 0 (unoccupied). 
//This way, I can easily manipulate the state of the qubits by applying quantum gates, which will modify these coefficients according to the rules of quantum mechanics.
QubitRegister::QubitRegister(int n): numQubits(n){
    //pow(2, n) gives the total number of states in the register, which is 2^n for n qubits
    long long size = std::pow(2 , n);
    stateVector = Eigen::VectorXcd::Zero(size);  //allocated in a contiguous block of memory and initialized to zero
    stateVector[0] = std::complex<double>(1,0); //initialize to |00...0>
}


//The Hadamard gate creates superposition by transforming |0> to (|0> + |1>)/sqrt(2) and |1> to (|0> - |1>)/sqrt(2).
void QubitRegister::applyHadamard(int qubitIndex) {
    long long mask = 1LL << qubitIndex;
    long long size = stateVector.size();

    #pragma omp parallel for
    for(long long i = 0; i < size; ++i){
        
        if(!(i & mask)){
            long long i0 = i;   
            long long i1 = i | mask;
            
            std::complex<double> temp0 = stateVector[i0];
            std::complex<double> temp1 = stateVector[i1];
            
            // Quantum Hadamard gate: alpha|0> + beta|1> -> (alpha + beta)/sqrt(2)|0> + (alpha - beta)/sqrt(2)|1>
            stateVector[i0] = (temp0 + temp1) / std::sqrt(2.0); //superposition of the two states
            stateVector[i1] = (temp0 - temp1) / std::sqrt(2.0); //superposition of the two states
        }
    }
}

//The Pauli-X gate (also known as the NOT gate) flips the state of a qubit: it transforms |0> to |1> and |1> to |0>.

//The idea is that I'm working with qubit, which means I use amplitudes to represent the state of the qubit.
//This fact implies that in the position of the vector, I'll find the amplitude of the state. For example, if I have a 2-qubit register, the state vector will have 4 complex coefficients corresponding
//to the basis states |00>, |01>, |10>, and |11>. If I want to apply a Pauli-X gate to the first qubit, I need to swap the amplitudes of the states that differ in the first qubit.
// Specifically, I would swap the amplitudes of |00> and |10>, and also swap the amplitudes of |01> and |11>. This way, I effectively flip the state of the first qubit while keeping the second qubit unchanged.
void QubitRegister::applyPauliX(int qubitIndex) {
    //imagine the vector as a coridor with 2ˆn doors, each door corresponds to a basis state (index i).
    //If I want to join the rooms where only the "second switch" is off (state 0), I use a mask to identify those states.
    long long mask = 1LL << qubitIndex;
    //remember, since I'm working with qubit, the qubit 0 represent the value 2^0 = 1, the qubit 1 represent the value 2^1 = 2, the qubit 2 represent the value 2^2 = 4, and so on.
    //so, If I'm targeting the qubit in position 0, 2^0 = 1, so the mask will be 1 (binary 01), if I'm targeting the qubit in position 1, 2^1 = 2, so the mask will be 2 (binary 10),
    // if I'm targeting the qubit in position 2, 2^2 = 4, so the mask will be 4 (binary 100), and so on.
    //by writing 1LL << qubitIndex, I'm calculating 2ˆqubitIndex, which gives me a mask that has a 1 in the position corresponding to the qubit I want to manipulate and 0s elsewhere. 
    //For example, if qubitIndex is 1, the mask will be 2 (binary 10), which means I'm targeting the second qubit.
    long long size = stateVector.size();

    #pragma omp parallel for
    for(long long i = 0; i < size; ++i){
        //I apply the gate only to a couple at time, so I only check if the "second switch" is off (state 0) and if it's on (state 1).
        //If it's off, I swap it with the corresponding state where the "second switch" is on, and vice versa.
        if(!(i & mask)){
            // To avoid redundant swaps (which would nullify the gate), we only process
            //indices where the target bit is 0. * 'i & mask' checks if the bit at targetQubit position is 1. * /
            long long i0 = i;    // state where the "second switch" is off
            long long i1 = i | mask; //state where the "second switch" is on
            
            std::complex<double> temp = stateVector[i0];
            stateVector[i0] = stateVector[i1]; //swap the amplitudes
            stateVector[i1] = temp; //swap the amplitudes
        }
    }
}

//The Pauli-Z gate applies a phase flip to the |1> state, transforming |0> to |0> and |1> to -|1>.
void QubitRegister::applyPauliZ(int qubitIndex) {
        long long mask = 1LL << qubitIndex;
        long long size = stateVector.size();
    
        #pragma omp parallel for
        for(long long i = 0; i < size; ++i){
            //I apply the gate only to a couple at time, so I only check if the "second switch" is on (state 1).
            //If it's on, I apply a phase flip by multiplying the amplitude by -1.
            if(i & mask){
                // Quantum Z-gate: alpha|0> + beta|1> -> alpha|0> - beta|1>
                stateVector[i] = -stateVector[i]; //apply phase flip
            }
        }
}