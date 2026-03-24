#include "../include/QubitRegister.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>

void printResult(const std::string &testName, int success, int total, std::string expected)
{
    double percentage = (double)success / total * 100.0;
    std::cout << std::left << std::setw(20) << testName
              << " | Success: " << std::setw(4) << success << "/" << total
              << " (" << std::fixed << std::setprecision(1) << percentage << "%)"
              << " | Expected: " << expected << std::endl;
}

int main()
{
    //useful to have different random sequences each time we run the tests, especially for the Hadamard test
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "====================================================" << std::endl;
    std::cout << "       TEST SESSION QUANTUM SIMULATOR     " << std::endl;
    std::cout << "====================================================" << std::endl;

    // --- TEST 1: PAULI-X (NOT) ---
    // Verify if the state |0> is inverted to |1> at 100%
    int notSuccess = 0;
    int trials = 100;
    for (int i = 0; i < trials; ++i)
    {
        QubitRegister reg(1);
        reg.applyPauliX(0);
        if (reg.measure(0) == 1)
        {
            notSuccess++;
        }
    }
    printResult("Test Pauli-X", notSuccess, trials, "100%");

    // --- TEST 2: HADAMARD (MONETA) ---
    // Verify if the superposition generates approximately 50% 0s and 50% 1s
    int hOneSuccess = 0;
    int hTrials = 1000;
    for (int i = 0; i < hTrials; ++i)
    {
        QubitRegister reg(1);
        reg.applyHadamard(0);
        if (reg.measure(0) == 1)
        {
            hOneSuccess++;
        }
    }
    printResult("Test Hadamard", hOneSuccess, hTrials, "~50%");

    // --- TEST 3: CNOT DETERMINISTICA ---
    // Verify if the control |1> correctly inverts the target
    int cnotSuccess = 0;
    for (int i = 0; i < trials; ++i)
    {
        QubitRegister reg(2);
        reg.applyPauliX(1);  // Q1 = 1 (Controllo)
        reg.applyCNOT(1, 0); // Se Q1=1, inverti Q0
        if (reg.measure(1) == 1 && reg.measure(0) == 1)
        {
            cnotSuccess++;
        }
    }
    printResult("Test CNOT (1->1)", cnotSuccess, trials, "100%");

    // --- TEST 4: STATO DI BELL (ENTANGLEMENT) ---
    // Verify the correlation: if Q0 is 0, Q1 must be 0. If Q0 is 1, Q1 must be 1.
    int bellSuccess = 0;
    for (int i = 0; i < trials; ++i)
    {
        QubitRegister reg(2);
        reg.applyHadamard(0);
        reg.applyCNOT(0, 1);
        int m0 = reg.measure(0);
        int m1 = reg.measure(1);
        if (m0 == m1)
        { // Correlazione perfetta
            bellSuccess++;
        }
    }
    printResult("Test Bell (Corr.)", bellSuccess, trials, "100%");

    std::cout << "====================================================" << std::endl;

    if (notSuccess == trials && bellSuccess == trials && cnotSuccess == trials)
    {
        std::cout << "FINAL RESULT: THE QUANTUM SIMULATOR IS PHYSICALLY COHERENT!" << std::endl;
    }
    else
    {
        std::cout << "FINAL OUTCOME: LOGICAL ERRORS DETECTED. PLEASE CHECK THE GATES." << std::endl;
    }

    return 0;
}