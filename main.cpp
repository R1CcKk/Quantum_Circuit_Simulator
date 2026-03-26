#include "../include/QubitRegister.hpp"
#include <iostream>

int main()
{
    std::cout << "Main Simulation" << std::endl;

    //bell state
    QubitRegister reg(3);

    reg.applyRotationY(0, M_PI / 4.0);
    reg.applyPauliX(1);
    reg.applyHadamard(2);

    
    reg.exportToJson("simulation_result.json", "Test 3 states");

    std::cout << "Data saved to build/simulation_result.json" << std::endl;

    reg.exportToJson("simulation_result.json", "Bell State");

    std::cout << "File 'simulation_result.json' generated successfully!" << std::endl;
    return 0;
}