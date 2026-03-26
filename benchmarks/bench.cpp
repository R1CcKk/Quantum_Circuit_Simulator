#include "../include/QubitRegister.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <fstream>

int main(){
    std::ofstream benchFile("benchmark_results.csv");
    benchFile << "qubits,time_ms\n";

    std::cout << "--- PERFORMANCE BENCHMARK ---" << std::endl;
    for(int n = 2; n <= 30; ++n){
        QubitRegister reg(n);

        auto start = std::chrono::high_resolution_clock::now();

        //applied on every qubit
        for(int i = 0; i < n; ++i){
            reg.applyHadamard(i);
        }

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        std::cout << "Qubits: " << n << " | Time: " << elapsed.count() << " ms" << std::endl;
        benchFile << n << "," << elapsed.count() << "\n";
    }
    benchFile.close();
    return 0;
}