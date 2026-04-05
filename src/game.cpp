#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>

#include <format>
#include <iostream>

#include "game.hpp"
#include "recorde.hpp"

using namespace std;
using namespace cv;

namespace {
    const char* kCascadeName = "assets/haarcascade_frontalface_default.xml";
    const char* kWindowName = "Game";
}

void Game::resetState() {
    scale = 1.0;
    tryflip = true;
    key = 0;
    frame.release();

    inimigos.clear();
    if (modoDificil) {
        inimigos.emplace_back();
        inimigos.back().perseguicao = true;
    } else {
        inimigos.emplace_back();
        inimigos.back().speed = 6.0f;
        inimigos.emplace_back();
        inimigos.back().speed = 6.0f;
        inimigos.emplace_back();
        inimigos.back().speed = 6.0f;
    }

    jogador = Jogador{};
    vidas = 3;
    fimInvulnerabilidade = Clock::time_point::min();
    tempoJogo.reiniciar();

    recordeInicial = Recorde::ler();
    recordeAtual = recordeInicial;
    novoRecordeAtingido = false;
    mostrarAvisoNovoRecorde = false;
    jogoEncerrado = false;
    fimAvisoNovoRecorde = Clock::time_point::min();

    tempoHudSombra.setConteudo("");
    tempoHud.setConteudo("");
    vidasHudSombra.setConteudo("");
    vidasHud.setConteudo("");
    recordeHudSombra.setConteudo("");
    recordeHud.setConteudo("");
    novoRecordeHudSombra.setConteudo("");
    novoRecordeHud.setConteudo("");
}

bool Game::carregarCascade() {
    if (!cascade.load(kCascadeName)) {
        cout << "ERROR: Could not load classifier cascade: " << kCascadeName << endl;
        return false;
    }

    return true;
}

bool Game::abrirCamera() {
    if (!capture.open(0)) {
        cout << "Capture from camera #0 didn't work" << endl;
        return false;
    }

    return capture.isOpened();
}

void Game::atualizarTimers() {
    const auto agora = Clock::now();
    const double tempoAtual = tempoJogo.emSegundos();

    if (!novoRecordeAtingido && tempoAtual > recordeInicial) {
        novoRecordeAtingido = true;
        mostrarAvisoNovoRecorde = true;
        fimAvisoNovoRecorde = agora + chrono::seconds(4);
    }

    if (novoRecordeAtingido) {
        recordeAtual = tempoAtual;
    }

    if (mostrarAvisoNovoRecorde && agora >= fimAvisoNovoRecorde) {
        mostrarAvisoNovoRecorde = false;
    }
}

void Game::atualizarColisoes(Mat& smallFrame) {
    bool houveContatoNoFrame = false;
    const auto agora = Clock::now();

    for (Inimigo& inimigo : inimigos) {
        inimigo.draw_inimigo(smallFrame);
        if (jogador.draw_jogador_rectangle(smallFrame, cascade, inimigo.get_rect())) {
            houveContatoNoFrame = true;
        }
        inimigo.move(jogador.pos);
    }

    const bool jogadorInvulneravel = agora < fimInvulnerabilidade;
    if (!houveContatoNoFrame || jogadorInvulneravel || vidas <= 0) {
        return;
    }

    vidas--;
    fimInvulnerabilidade = agora + duracaoInvulnerabilidade;

    if (vidas > 0) {
        somDano.tocar();
        return;
    }

    somGameOver.tocar();
    tratarGameOver();
    jogoEncerrado = true;
}

void Game::atualizarHud(Mat& smallFrame) {
    const bool jogadorInvulneravel = Clock::now() < fimInvulnerabilidade;

    const string textoTempo = "Tempo vivo: " + tempoJogo.formatadoSegundos(2) + "s";
    string textoVidas = "Vidas: " + to_string(vidas);
    if (jogadorInvulneravel) {
        textoVidas += " (imortal)";
    }

    const string textoRecorde = "Recorde: " + std::format("{:.2f}", recordeAtual) + "s";

    tempoHudSombra.setConteudo(textoTempo);
    tempoHud.setConteudo(textoTempo);
    vidasHudSombra.setConteudo(textoVidas);
    vidasHud.setConteudo(textoVidas);
    recordeHudSombra.setConteudo(textoRecorde);
    recordeHud.setConteudo(textoRecorde);

    if (mostrarAvisoNovoRecorde) {
        novoRecordeHudSombra.setConteudo("(Novo Recorde!)");
        novoRecordeHud.setConteudo("(Novo Recorde!)");
    } else {
        novoRecordeHudSombra.setConteudo("");
        novoRecordeHud.setConteudo("");
    }

    tempoHudSombra.desenhar(smallFrame);
    tempoHud.desenhar(smallFrame);
    vidasHudSombra.desenhar(smallFrame);
    vidasHud.desenhar(smallFrame);
    recordeHudSombra.desenhar(smallFrame);
    recordeHud.desenhar(smallFrame);
    novoRecordeHudSombra.desenhar(smallFrame);
    novoRecordeHud.desenhar(smallFrame);
}

void Game::tratarGameOver() {
    const double tempoFinal = tempoJogo.emSegundos();
    cout << "Game over! Tempo vivo: " << tempoJogo.formatadoSegundos(2) << "s" << endl;

    if (novoRecordeAtingido) {
        recordeAtual = tempoFinal;
        cout << "NOVO RECORDE ALCANCADO! (" << tempoJogo.formatadoSegundos(2) << "s)" << endl;
        Recorde::salvar(recordeAtual);
    }
}

void Game::encerrarJanela() {
    if (capture.isOpened()) {
        capture.release();
    }
    destroyWindow(kWindowName);
    destroyAllWindows();
    waitKey(1);
}

bool Game::processarFrame() {
    capture >> frame;
    if (frame.empty()) {
        return true; // frame perdido, continua rodando
    }

    atualizarTimers();

    Mat smallFrame;
    const double fx = 1 / scale;
    resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);
    if (tryflip) {
        flip(smallFrame, smallFrame, 1);
    }

    atualizarColisoes(smallFrame);
    atualizarHud(smallFrame);
    imshow(kWindowName, smallFrame);

    key = (char)waitKey(10);
    if (key == 27 || key == 'q' || key == 'Q') {
        return false;
    }

    return true;
}

void Game::start(bool modoDificil) {
    this->modoDificil = modoDificil;
    resetState();

    if (scale < 1) {
        scale = 1;
    }

    if (!carregarCascade()) {
        return;
    }

    if (!abrirCamera()) {
        return;
    }

    cout << "Video capturing has been started ..." << endl;
    cout << "Resolution: " << capture.get(CAP_PROP_FRAME_WIDTH) << " x "
         << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;

    namedWindow(kWindowName, WINDOW_FULLSCREEN);

    while (processarFrame()) {
        if (jogoEncerrado) {
            break;
        }
    }

    encerrarJanela();
}