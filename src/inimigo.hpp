#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <iostream>
#include<random>
#include<chrono>
#include<cmath>
#include"utils.hpp"

using namespace std;
using namespace cv;



class Inimigo{
    public:
        Mat img;
        Mat dst;
        Size s;
        pair<int,int>pos;
        float speed;
        float angle;        // direção atual do movimento (radianos)
        float angle_step;   // quanto gira por frame (define o zigzag)
        int step;           // contador de frames
        bool perseguicao;   // true = modo difícil (perseguição), false = zigzag
        float chase_speed;  // velocidade acumulada no modo perseguição

        Inimigo(){
            auto seed = chrono::steady_clock::now().time_since_epoch().count();
            mt19937 gen(seed);
            uniform_int_distribution<> distrib(1,540);
            uniform_real_distribution<float> distrib_angle(0.0f, 2.0f * M_PI);
            uniform_real_distribution<float> distrib_step(0.04f, 0.10f);

            img  = imread("assets/inimigo.png",IMREAD_UNCHANGED);
            resize(img,dst,Size(),0.15,0.15,INTER_LINEAR);
            s = dst.size();
            pos = {distrib(gen),distrib(gen)};
            speed = 3.0f;
            angle = distrib_angle(gen);
            angle_step = distrib_step(gen);
            step = 0;
            perseguicao = false;
            chase_speed = 4.0f;
        };

        void draw_inimigo(Mat& smallFrame){
            if (!dst.empty())
                drawImage(smallFrame, dst, pos.first, pos.second);
        }

        Rect get_rect(){
            return Rect(pos.first - dst.cols/2, pos.second - dst.rows/2, dst.cols, dst.rows);
        }

        void move(pair<int,int> pos_jogador){
            if (perseguicao) {
                // modo difícil: persegue o jogador e acelera
                float dx = pos_jogador.first  - pos.first;
                float dy = pos_jogador.second - pos.second;
                float norma = sqrt(dx*dx + dy*dy);
                if (norma == 0) return;
                pos.first  += (int)roundf(dx / norma * chase_speed);
                pos.second += (int)roundf(dy / norma * chase_speed);
                chase_speed = max(8.0f, chase_speed + 0.002f);
            } else {
                // modo fácil: zigzag, rebate nas bordas
                angle += angle_step * sinf(step * 0.05f);
                step++;
                pos.first  += (int)roundf(cosf(angle) * speed);
                pos.second += (int)roundf(sinf(angle) * speed);

                if (pos.first  < 0)   { pos.first  = 0;   angle = M_PI - angle; }
                if (pos.first  > 640) { pos.first  = 640; angle = M_PI - angle; }
                if (pos.second < 0)   { pos.second = 0;   angle = -angle; }
                if (pos.second > 480) { pos.second = 480; angle = -angle; }
            }
        }



};
