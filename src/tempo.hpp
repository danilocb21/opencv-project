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
        inicio_ = Clock::now();
    }

    long long emMilissegundos() const {
        return chrono::duration_cast<chrono::milliseconds>(Clock::now() - inicio_).count();
    }

    double emSegundos() const {
        return emMilissegundos() / 1000.0;
    }

    string formatadoSegundos(int casasDecimais = 2) const {
        return std::format("{:.{}f}", emSegundos(), casasDecimais);
    }

private:
    using Clock = chrono::steady_clock;
    Clock::time_point inicio_;
};

#endif // TEMPO_HPP