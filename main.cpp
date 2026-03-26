#include "../include/QubitRegister.hpp"
#include <iostream>

int main()
{
    std::cout << "Main Simulation" << std::endl;

    //bell state
    QubitRegister reg(2);
    reg.applyHadamard(0);
    reg.applyCNOT(0, 1);

    
    reg.exportToJson("simulation_result.json", "Bell State");

    std::cout << "Data saved to build/simulation_result.json" << std::endl;

    reg.exportToJson("simulation_result.json", "Bell State");

    std::cout << "File 'simulation_result.json' generated successfully!" << std::endl;
    return 0;
}