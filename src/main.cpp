#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <format>

#include "utils.hpp"
#include "inimigo.hpp"
#include "jogador.hpp"
#include "tempo.hpp"
#include "texto.hpp"
#include "som.hpp"
#include "recorde.hpp"

using namespace std;
using namespace cv;


string cascadeName;
string wName = "Game";
int xi = 10;
int yi = 150;
bool sentido = true;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;
    char key = 0;


    //FACES INICIO
    cascadeName = "assets/haarcascade_frontalface_default.xml";
    scale = 1; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return 1;
    }
    //FACES FINAL


    //Teste da webcam
    if(!capture.open(0)) {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    //Lógica da captura de vídeo
    if (capture.isOpened()) {
        cout << "Video capturing has been started ..." << endl;
        cout << "Resolution: " << capture.get(CAP_PROP_FRAME_WIDTH) << " x "
        << capture.get(CAP_PROP_FRAME_HEIGHT) << endl;
        namedWindow(wName, WINDOW_KEEPRATIO);

        vector<Inimigo> inimigos(1); // cria 5 inimigos
        Jogador jogador;
        int vidas = 3;
        const chrono::milliseconds duracaoInvulnerabilidade(2000);
        auto fimInvulnerabilidade = chrono::steady_clock::time_point::min();
        Tempo tempoJogo;
        Som somDano("assets/fahh_sound_effect.mp3");
        Som somGameOver("assets/FAH-with-shotgun.mp3");

        double recordeAtual = Recorde::ler();

        // Sombra preta + texto amarelo para manter contraste em fundos claros e escuros.
        Texto tempoHudSombra("", Point(21, 41), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 0), 4, LINE_AA);
        Texto tempoHud("", Point(20, 40), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 255, 255), 2, LINE_AA);
        Texto vidasHudSombra("", Point(21, 76), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 0), 4, LINE_AA);
        Texto vidasHud("", Point(20, 75), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 255, 255), 2, LINE_AA);
        
        //Recorde
        Texto recordeHudSombra("", Point(21, 111), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 0, 0), 4, LINE_AA);
        Texto recordeHud("", Point(20, 110), FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 255, 0), 2, LINE_AA); // Cor Verde para destaque
        
        while (1) {
            capture >> frame;
            if (frame.empty()) break;

            const auto agora = chrono::steady_clock::now();
            bool jogadorInvulneravel = agora < fimInvulnerabilidade;
            bool houveContatoNoFrame = false;

            // prepara smallFrame uma vez
            Mat smallFrame;
            double fx = 1 / scale;
            resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);
            if (tryflip) flip(smallFrame, smallFrame, 1);

            for(Inimigo& inimigo : inimigos){
                inimigo.draw_inimigo(smallFrame);
                if (jogador.draw_jogador_rectangle(smallFrame, cascade, inimigo.get_rect())) {
                    houveContatoNoFrame = true;
                }
                inimigo.move(jogador.pos);
            }

            if (houveContatoNoFrame && !jogadorInvulneravel && vidas > 0) {
                vidas -= 1;
                fimInvulnerabilidade = agora + duracaoInvulnerabilidade;
                jogadorInvulneravel = true;
                
                if (vidas > 0) {
                    somDano.tocar();
                }

                if (vidas == 0) {
                    somGameOver.tocar();
                    double tempoFinal = tempoJogo.emSegundos();
                    cout << "Game over! Tempo vivo: " << tempoJogo.formatadoSegundos(2) << "s" << endl;
                    
                    // Lógica do recorde aplicada antes de dar o break
                    if (tempoFinal > recordeAtual) {
                        cout << "NOVO RECORDE ALCANCADO! (" << tempoJogo.formatadoSegundos(2) << "s)" << endl;
                        Recorde::salvar(tempoFinal);
                    }
                    
                    break; // Agora sai do loop corretamente após salvar
                }
}

            string textoTempo = "Tempo vivo: " + tempoJogo.formatadoSegundos(2) + "s";
            string textoVidas = "Vidas: " + to_string(vidas);
            if (jogadorInvulneravel) {
                textoVidas += " (imortal)";
            }

            string textoRecorde = "Recorde: " + std::format("{:.2f}", recordeAtual) + "s";

            tempoHudSombra.setConteudo(textoTempo);
            tempoHud.setConteudo(textoTempo);
            vidasHudSombra.setConteudo(textoVidas);
            vidasHud.setConteudo(textoVidas);

            recordeHudSombra.setConteudo(textoRecorde);
            recordeHud.setConteudo(textoRecorde);

            tempoHudSombra.desenhar(smallFrame);
            tempoHud.desenhar(smallFrame);
            vidasHudSombra.desenhar(smallFrame);
            vidasHud.desenhar(smallFrame);

            recordeHudSombra.desenhar(smallFrame);
            recordeHud.desenhar(smallFrame);

            imshow(wName, smallFrame);

            key = (char)waitKey(10);
            if (key == 27 || key == 'q' || key == 'Q') break;

        }
    }

    return 0;
}
