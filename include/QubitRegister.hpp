#ifndef QUBIT_REGISTER_HPP
#define QUBIT_REGISTER_HPP

#include<iostream>
#include<vector>
#include<Eigen/Dense>

class QubitRegister {
    private:
        std::vector<double> stateReal; 
        std::vector<double> stateImag;
        int numQubits; // Number of qubits in the register

    public:
        
        QubitRegister(int n);
        std::vector<double> getStateReal() const;
        std::vector<double> getStateImag() const;

        void applyHadamard(int qubitIndex);

        void applyPauliX(int qubitIndex);
        void applyPauliZ(int qubitIndex);

        void applyCNOT(int controlQubit, int targetQubit);

        void applyToffoli(int controlQubit1, int controlQubit2, int targetQubit);

        void applyPhaseShift(int qubitIndex, double angle);

        void applyRotationX(int qubitIndex, double angle);
        void applyRotationY(int qubitIndex, double angle);
        void applyRotationZ(int qubitIndex, double angle);

        void applyUnitary(int qubit, const Eigen::Matrix2cd &gate);
        void applySingleQubitGateTiled(int qubitIndex, const Eigen::Matrix2cd &gate);

        int measure(int qubitIndex);

        std::complex<double> getAmplitude(int index) const;

        void printState() const;

        void exportToJson(const std::string& filename, const std::string& circuitName) const;
};





#endif // QUBIT_REGISTER_HPP