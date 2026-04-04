#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <string>
#include <vector>

#include "inimigo.hpp"
#include "jogador.hpp"
#include "som.hpp"
#include "tempo.hpp"
#include "texto.hpp"

class Game {
public:
    void start();

private:
    using Clock = std::chrono::steady_clock;

    void resetState();
    bool carregarCascade();
    bool abrirCamera();
    bool processarFrame();
    void atualizarTimers();
    void atualizarColisoes(cv::Mat& smallFrame);
    void atualizarHud(cv::Mat& smallFrame);
    void tratarGameOver();
    void encerrarJanela();

    cv::VideoCapture capture;
    cv::Mat frame;
    cv::CascadeClassifier cascade;
    double scale = 1.0;
    bool tryflip = true;
    char key = 0;

    std::vector<Inimigo> inimigos;
    Jogador jogador;
    int vidas = 3;
    const std::chrono::milliseconds duracaoInvulnerabilidade = std::chrono::milliseconds(2000);
    Clock::time_point fimInvulnerabilidade = Clock::time_point::min();
    Tempo tempoJogo;
    Som somDano{"assets/fahh_sound_effect.mp3"};
    Som somGameOver{"assets/FAH-with-shotgun.mp3"};

    double recordeInicial = 0.0;
    double recordeAtual = 0.0;
    bool novoRecordeAtingido = false;
    bool mostrarAvisoNovoRecorde = false;
    bool jogoEncerrado = false;
    Clock::time_point fimAvisoNovoRecorde = Clock::time_point::min();

    Texto tempoHudSombra{"", cv::Point(21, 41), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 0), 4, cv::LINE_AA};
    Texto tempoHud{"", cv::Point(20, 40), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 255, 255), 2, cv::LINE_AA};
    Texto vidasHudSombra{"", cv::Point(21, 76), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 0), 4, cv::LINE_AA};
    Texto vidasHud{"", cv::Point(20, 75), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 255, 255), 2, cv::LINE_AA};
    Texto recordeHudSombra{"", cv::Point(21, 111), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 0, 0), 4, cv::LINE_AA};
    Texto recordeHud{"", cv::Point(20, 110), cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(0, 255, 0), 2, cv::LINE_AA};
    Texto novoRecordeHudSombra{"", cv::Point(320, 111), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 0, 0), 4, cv::LINE_AA};
    Texto novoRecordeHud{"", cv::Point(319, 110), cv::FONT_HERSHEY_DUPLEX, 0.8, cv::Scalar(0, 255, 0), 2, cv::LINE_AA};
};

#endif // GAME_HPP