#ifndef TEMPO_HPP
#define TEMPO_HPP

#include <chrono>
#include <format>
#include <string>

using namespace std;

class Tempo {
public:
    Tempo() {
        reiniciar();
    }

    void reiniciar() {
        inicio = Clock::now();
    }

    long long emMilissegundos() const {
        return chrono::duration_cast<chrono::milliseconds>(Clock::now() - inicio).count();
    }

    double emSegundos() const {
        return emMilissegundos() / 1000.0;
    }

    string formatadoSegundos(int casasDecimais = 2) const {
        return std::format("{:.{}f}", emSegundos(), casasDecimais);
    }

private:
    using Clock = chrono::steady_clock;
    Clock::time_point inicio;
};

#endif // TEMPO_HPP