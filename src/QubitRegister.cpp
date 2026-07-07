#include <cmath>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <vector>
#include "../include/QubitRegister.hpp"

QubitRegister::QubitRegister(int n) : numQubits(n)
{
    long long size = 1LL << n;
    stateReal.assign(size, 0.0);
    stateImag.assign(size, 0.0);
    stateReal[0] = 1.0;
}

std::vector<double> QubitRegister::getStateReal() const { return stateReal; }
std::vector<double> QubitRegister::getStateImag() const { return stateImag; }

void QubitRegister::applyHadamard(int q)
{
    Eigen::Matrix2cd H;
    H << 1, 1, 1, -1;
    H /= std::sqrt(2.0);
    applySingleQubitGateTiled(q, H);
}

void QubitRegister::applyPauliX(int q)
{
    Eigen::Matrix2cd X;
    X << 0, 1, 1, 0;
    applyUnitary(q, X);
}

void QubitRegister::applyPauliZ(int q)
{
    Eigen::Matrix2cd Z;
    Z << 1, 0, 0, -1;
    applySingleQubitGateTiled(q, Z);
}

void QubitRegister::applyCNOT(int control, int target)
{
    long long maskC = 1LL << control;
    long long maskT = 1LL << target;
    long long size = stateReal.size();

#pragma omp parallel for if (size > 1024)
    for (long long i = 0; i < size; ++i)
    {
        if ((i & maskC) && !(i & maskT))
        {
            std::swap(stateReal[i], stateReal[i | maskT]);
            std::swap(stateImag[i], stateImag[i | maskT]);
        }
    }
}

void QubitRegister::applyToffoli(int c1, int c2, int target)
{
    long long maskC1 = 1LL << c1;
    long long maskC2 = 1LL << c2;
    long long maskT = 1LL << target;
    long long size = stateReal.size();

#pragma omp parallel for if (size > 1024)
    for (long long i = 0; i < size; ++i)
    {
        if ((i & maskC1) && (i & maskC2) && !(i & maskT))
        {
            std::swap(stateReal[i], stateReal[i | maskT]);
            std::swap(stateImag[i], stateImag[i | maskT]);
        }
    }
}

void QubitRegister::applyPhaseShift(int q, double phi)
{
    long long mask = 1LL << q;
    double cosP = std::cos(phi);
    double sinP = std::sin(phi);

#pragma omp parallel for if (stateReal.size() > 1024)
    for (long long i = 0; i < (long long)stateReal.size(); ++i)
    {
        if (i & mask)
        {
            double r = stateReal[i], im = stateImag[i];
            stateReal[i] = r * cosP - im * sinP;
            stateImag[i] = r * sinP + im * cosP;
        }
    }
}

void QubitRegister::applyRotationX(int q, double theta)
{
    Eigen::Matrix2cd gate;
    double c = std::cos(theta / 2.0);
    double s = std::sin(theta / 2.0);
    gate << c, std::complex<double>(0, -s), std::complex<double>(0, -s), c;
    applyUnitary(q, gate);
}

void QubitRegister::applyRotationY(int q, double theta)
{
    Eigen::Matrix2cd gate;
    double c = std::cos(theta / 2.0);
    double s = std::sin(theta / 2.0);
    gate << c, -s, s, c;
    applySingleQubitGateTiled(q, gate);
}

void QubitRegister::applyRotationZ(int q, double theta)
{
    long long mask = 1LL << q;
    double p0 = -theta / 2.0;
    double p1 = theta / 2.0;
    double c0 = std::cos(p0), s0 = std::sin(p0);
    double c1 = std::cos(p1), s1 = std::sin(p1);

#pragma omp parallel for if (stateReal.size() > 1024)
    for (long long i = 0; i < (long long)stateReal.size(); ++i)
    {
        double r = stateReal[i], im = stateImag[i];
        double c = (i & mask) ? c1 : c0;
        double s = (i & mask) ? s1 : s0;
        stateReal[i] = r * c - im * s;
        stateImag[i] = r * s + im * c;
    }
}

void QubitRegister::applyUnitary(int qubit, const Eigen::Matrix2cd &gate)
{
    long long mask = 1LL << qubit;
    long long half_size = stateReal.size() >> 1;
    double r00 = gate(0, 0).real(), i00 = gate(0, 0).imag();
    double r01 = gate(0, 1).real(), i01 = gate(0, 1).imag();
    double r10 = gate(1, 0).real(), i10 = gate(1, 0).imag();
    double r11 = gate(1, 1).real(), i11 = gate(1, 1).imag();

#pragma omp parallel for if (half_size > 1024)
    for (long long i = 0; i < half_size; ++i)
    {
        long long i0 = ((i & ~(mask - 1)) << 1) | (i & (mask - 1));
        long long i1 = i0 | mask;
        double tr0 = stateReal[i0], ti0 = stateImag[i0];
        double tr1 = stateReal[i1], ti1 = stateImag[i1];
        stateReal[i0] = r00 * tr0 - i00 * ti0 + r01 * tr1 - i01 * ti1;
        stateImag[i0] = r00 * ti0 + i00 * tr0 + r01 * ti1 + i01 * tr1;
        stateReal[i1] = r10 * tr0 - i10 * ti0 + r11 * tr1 - i11 * ti1;
        stateImag[i1] = r10 * ti0 + i10 * tr0 + r11 * ti1 + i11 * tr1;
    }
}

void QubitRegister::applySingleQubitGateTiled(int q, const Eigen::Matrix2cd &gate)
{
    long long mask = 1LL << q;
    long long half_size = stateReal.size() >> 1;
    const long long TILE_SIZE = 16384;
    double r00 = gate(0, 0).real(), i00 = gate(0, 0).imag();
    double r01 = gate(0, 1).real(), i01 = gate(0, 1).imag();
    double r10 = gate(1, 0).real(), i10 = gate(1, 0).imag();
    double r11 = gate(1, 1).real(), i11 = gate(1, 1).imag();

#pragma omp parallel for if (half_size > 1024)
    for (long long ts = 0; ts < half_size; ts += TILE_SIZE)
    {
        long long te = std::min(ts + TILE_SIZE, half_size);
        for (long long i = ts; i < te; ++i)
        {
            long long i0 = ((i & ~(mask - 1)) << 1) | (i & (mask - 1));
            long long i1 = i0 | mask;
            double tr0 = stateReal[i0], ti0 = stateImag[i0];
            double tr1 = stateReal[i1], ti1 = stateImag[i1];
            stateReal[i0] = r00 * tr0 - i00 * ti0 + r01 * tr1 - i01 * ti1;
            stateImag[i0] = r00 * ti0 + i00 * tr0 + r01 * ti1 + i01 * tr1;
            stateReal[i1] = r10 * tr0 - i10 * ti0 + r11 * tr1 - i11 * ti1;
            stateImag[i1] = r10 * ti0 + i10 * tr0 + r11 * ti1 + i11 * tr1;
        }
    }
}

int QubitRegister::measure(int q)
{
    long long mask = 1LL << q;
    double prob1 = 0.0;
    for (long long i = 0; i < (long long)stateReal.size(); ++i)
    {
        if (i & mask)
            prob1 += (stateReal[i] * stateReal[i] + stateImag[i] * stateImag[i]);
    }
    int outcome = ((double)rand() / RAND_MAX < prob1) ? 1 : 0;
    double p = (outcome == 1) ? prob1 : (1.0 - prob1);
    double invNorm = 1.0 / std::sqrt(std::max(p, 1e-15));

#pragma omp parallel for if (stateReal.size() > 1024)
    for (long long i = 0; i < (long long)stateReal.size(); ++i)
    {
        if (((i & mask) && outcome == 0) || (!(i & mask) && outcome == 1))
        {
            stateReal[i] = 0;
            stateImag[i] = 0;
        }
        else
        {
            stateReal[i] *= invNorm;
            stateImag[i] *= invNorm;
        }
    }
    return outcome;
}

void QubitRegister::exportToJson(const std::string &f, const std::string &name) const
{
    std::ofstream file(f);
    file << "{\n  \"metadata\": {\"circuitName\": \"" << name << "\", \"numQubits\": " << numQubits << "},\n  \"amplitudes\": [\n";
    for (long long i = 0; i < (long long)stateReal.size(); ++i)
    {
        double prob = stateReal[i] * stateReal[i] + stateImag[i] * stateImag[i];
        file << "    {\"index\": " << i << ", \"real\": " << stateReal[i] << ", \"imag\": " << stateImag[i] << ", \"prob\": " << prob << "}";
        if (i < (long long)stateReal.size() - 1)
            file << ",";
        file << "\n";
    }
    file << "  ]\n}";
}