#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <iostream>
#include "utils.hpp"
#include "inimigo.hpp"
#include "jogador.hpp"
using namespace std;
using namespace cv;


string cascadeName;
string wName = "Game";
int xi =10;
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
    cascadeName = "haarcascade_frontalface_default.xml";
    scale = 1; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    if (!cascade.load(cascadeName)) {
        cout << "ERROR: Could not load classifier cascade: " << cascadeName << endl;
        return -1;
    }
    //FACES FINAL


    //Teste da webcam
    if(!capture.open(0)) 
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    

    //Lógica da captura de vídeo
    if(capture.isOpened() ) {
        cout << "Video capturing has been started ..." << endl;
        namedWindow(wName, WINDOW_KEEPRATIO);

        vector<Inimigo> inimigos(3); // cria 5 inimigos
        Jogador jogador;
        while (1) {
            capture >> frame;
            if (frame.empty()) break;

            // prepara smallFrame uma vez
            Mat smallFrame;
            double fx = 1 / scale;
            resize(frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT);
            if (tryflip) flip(smallFrame, smallFrame, 1);

            for(Inimigo& inimigo : inimigos){
                inimigo.draw_inimigo(smallFrame);
                jogador.draw_jogador_rectangle(smallFrame, cascade, inimigo.get_rect());
                inimigo.move(jogador.pos);
            }

            putText(smallFrame, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, Scalar(0,0,255));
            imshow(wName, smallFrame);

            

            key = (char)waitKey(10);
            if (key == 27 || key == 'q' || key == 'Q') break;
        }
            }

    return 0;
}
