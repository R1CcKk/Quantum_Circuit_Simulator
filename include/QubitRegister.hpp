#ifndef QUBIT_REGISTER_HPP
#define QUBIT_REGISTER_HPP

#include<iostream>
#include<vector>
#include<Eigen/Dense>

class QubitRegister {
    private:
        Eigen::VectorXcd stateVector; // State of qubit register
        int numQubits; // Number of qubits in the register

    public:
        
        QubitRegister(int n);
        
        void applyHadamard(int qubitIndex);

        void applyPauliX(int qubitIndex);
        void applyPauliZ(int qubitIndex);

        void applyCNOT(int controlQubit, int targetQubit);

        void applyToffoli(int controlQubit1, int controlQubit2, int targetQubit);

        void applyPhaseShift(int qubitIndex, double angle);

        void applyRotationX(int qubitIndex, double angle);
        void applyRotationY(int qubitIndex, double angle);
        void applyRotationZ(int qubitIndex, double angle);

        int measure(int qubitIndex);

        std::complex<double> getAmplitude(int index) const;

        void printState() const;
};





#endif // QUBIT_REGISTER_HPP