#ifndef SOM_HPP
#define SOM_HPP

#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

class Som {
public:
    explicit Som(string caminhoArquivo)
        : caminhoArquivo(std::move(caminhoArquivo)), playerSelecionado(detectarPlayer()) {}

    void tocar() const {
        tocarAssincrono(caminhoArquivo, playerSelecionado);
    }

private:
    string caminhoArquivo;
    string playerSelecionado;

    static string escaparAspasSimples(const string& texto) {
        string escaped;
        escaped.reserve(texto.size() + 8);
        for (char c : texto) {
            if (c == '\'') {
                escaped += "'\\''";
            } else {
                escaped += c;
            }
        }
        return escaped;
    }

    static int executarComando(const string& comando) {
        return std::system(comando.c_str());
    }

    static bool comandoDisponivel(const string& comando) {
        return executarComando("command -v " + comando + " >/dev/null 2>&1") == 0;
    }

    static string detectarPlayer() {
        if (comandoDisponivel("mpg123")) {
            return "mpg123";
        }
        if (comandoDisponivel("ffplay")) {
            return "ffplay";
        }
        if (comandoDisponivel("paplay")) {
            return "paplay";
        }
        if (comandoDisponivel("cvlc")) {
            return "cvlc";
        }
        cerr << "Aviso: nenhum player de audio encontrado (mpg123/ffplay/paplay/cvlc)." << endl;
        return "";
    }

    static void tocarAssincrono(const string& caminho, const string& player) {
        if (player.empty()) {
            return;
        }

        const string caminhoSeguro = "'" + escaparAspasSimples(caminho) + "'";
        string cmd;

        if (player == "mpg123") {
            cmd = "mpg123 -q " + caminhoSeguro + " >/dev/null 2>&1 &";
        } else if (player == "ffplay") {
            cmd = "ffplay -nodisp -autoexit -loglevel quiet " + caminhoSeguro + " >/dev/null 2>&1 &";
        } else if (player == "paplay") {
            cmd = "paplay " + caminhoSeguro + " >/dev/null 2>&1 &";
        } else if (player == "cvlc") {
            cmd = "cvlc --play-and-exit " + caminhoSeguro + " >/dev/null 2>&1 &";
        }

        if (!cmd.empty()) {
            executarComando(cmd);
        }
    }
};

#endif // SOM_HPP