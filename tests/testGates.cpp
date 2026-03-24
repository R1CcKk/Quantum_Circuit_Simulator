#include "../include/QubitRegister.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <cmath>
#include <vector>

// Helper to format results in the terminal
void report(std::string name, int ok, int total, std::string target)
{
    double perc = (double)ok / total * 100.0;
    std::cout << std::left << std::setw(30) << name
              << " | " << std::setw(4) << ok << "/" << total
              << " (" << std::fixed << std::setprecision(1) << perc << "%)"
              << " | Target: " << target << std::endl;
}

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    const int TRIALS = 1000;

    std::cout << "==========================================================" << std::endl;
    std::cout << "       ULTIMATE QUANTUM SIMULATOR MASTER TESTS            " << std::endl;
    std::cout << "==========================================================" << std::endl;

    // 1. CLASSICAL LOGIC: Pauli-X (NOT)
    int xOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyPauliX(0);
        if (reg.measure(0) == 1)
            xOk++;
    }
    report("1. Pauli-X (NOT)", xOk, TRIALS, "100%");

    // 2. SUPERPOSITION: Hadamard (Coin Flip)
    int hOne = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        if (reg.measure(0) == 1)
            hOne++;
    }
    report("2. Hadamard (Randomness)", hOne, TRIALS, "~50%");

    // 3. INTERFERENCE: Double Hadamard (Identity)
    int h2Ok = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        reg.applyHadamard(0);
        if (reg.measure(0) == 0)
            h2Ok++;
    }
    report("3. H-Interference (H*H=I)", h2Ok, TRIALS, "100%");

    // 4. PHASE KICKBACK: H -> Z -> H
    int zOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        reg.applyPauliZ(0);
        reg.applyHadamard(0);
        if (reg.measure(0) == 1)
            zOk++;
    }
    report("4. Pauli-Z Phase Flip", zOk, TRIALS, "100%");

    // 5. ARBITRARY PHASE: PhaseShift(PI)
    int psOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        reg.applyPhaseShift(0, M_PI); // Equivalent to Z
        reg.applyHadamard(0);
        if (reg.measure(0) == 1)
            psOk++;
    }
    report("5. PhaseShift (PI)", psOk, TRIALS, "100%");

    // 6. CONTINUOUS ROTATION X: RX(PI)
    int rxOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyRotationX(0, M_PI); // Should act like Pauli-X
        if (reg.measure(0) == 1)
            rxOk++;
    }
    report("6. Rotation-X (PI)", rxOk, TRIALS, "100%");

    // 7. CONTINUOUS ROTATION Y: RY(PI/2)
    int ryOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyRotationY(0, M_PI / 2.0);
        if (reg.measure(0) == 1)
            ryOk++;
    }
    report("7. Rotation-Y (PI/2)", ryOk, TRIALS, "~50%");

    // 8. CONTINUOUS ROTATION Z: RZ(PI)
    int rzOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        reg.applyRotationZ(0, M_PI);
        reg.applyHadamard(0);
        if (reg.measure(0) == 1)
            rzOk++;
    }
    report("8. Rotation-Z Interference", rzOk, TRIALS, "100%");

    // 9. ENTANGLEMENT: CNOT (Bell State)
    int bellOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(2);
        reg.applyHadamard(0);
        reg.applyCNOT(0, 1);
        if (reg.measure(0) == reg.measure(1))
            bellOk++;
    }
    report("9. CNOT (Bell Correlation)", bellOk, TRIALS, "100%");

    // 10. MULTI-QUBIT: GHZ State (3 Qubits)
    int ghzOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(3);
        reg.applyHadamard(0);
        reg.applyCNOT(0, 1);
        reg.applyCNOT(1, 2);
        int m0 = reg.measure(0);
        int m1 = reg.measure(1);
        int m2 = reg.measure(2);
        if (m0 == m1 && m1 == m2)
            ghzOk++;
    }
    report("10. GHZ State (3-Qubit)", ghzOk, TRIALS, "100%");

    // 11. UNIVERSAL LOGIC: Toffoli (CCNOT)
    int tofOk = 0;
    for (int i = 0; i < TRIALS; ++i)
    {
        QubitRegister reg(3);
        reg.applyPauliX(0);
        reg.applyPauliX(1);
        reg.applyToffoli(0, 1, 2);
        if (reg.measure(2) == 1)
            tofOk++;
    }
    report("11. Toffoli (AND Logic)", tofOk, TRIALS, "100%");

    std::cout << "==========================================================" << std::endl;
    std::cout << "TEST SUMMARY: If all 100% (except 50% ones), the engine is PERFECT." << std::endl;
    std::cout << "==========================================================" << std::endl;

    return 0;
}