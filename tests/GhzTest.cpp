#include "../include/QubitRegister.hpp"
#include <iostream>

int main()
{
    int numQubits = 3;
    QubitRegister reg(numQubits);

    std::cout << "Generating GHZ state: (|000> + |111>) / sqrt(2)..." << std::endl;

    // |000> -> (|0> + |1>)|00>
    reg.applyHadamard(0);

    // (|00> + |11>)|0>
    reg.applyCNOT(0, 1);

    // (|000> + |111>)
    reg.applyCNOT(1, 2);

    reg.exportToJson("simulation_result.json", "GHZ State Validation");

    std::cout << "Success! Data exported to simulation_result.json" << std::endl;
    

    return 0;
}