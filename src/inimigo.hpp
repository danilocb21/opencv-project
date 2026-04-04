#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <iostream>
#include<random>
#include<chrono>
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

        Inimigo(){
            auto seed = chrono::steady_clock::now().time_since_epoch().count();
            mt19937 gen(seed);
            uniform_int_distribution<> distrib(1,540);

            img  = imread("assets/inimigo.png",IMREAD_UNCHANGED);
            resize(img,dst,Size(),0.15,0.15,INTER_LINEAR);
            s = dst.size();
            pos = {distrib(gen),distrib(gen)};
            speed = 4.0f;
        };

        void draw_inimigo(Mat& smallFrame){
            if (!dst.empty())
                drawImage(smallFrame, dst, pos.first, pos.second);
        }

        Rect get_rect(){
            return Rect(pos.first - dst.cols/2, pos.second - dst.rows/2, dst.cols, dst.rows);
        }

        void move(pair<int,int> pos_jogador){
            float dx = pos_jogador.first - pos.first;
            float dy = pos_jogador.second - pos.second;
            float norma = sqrt(dx*dx + dy*dy);
            if (norma == 0) return;
            pos.first  += (int)roundf(dx / norma * speed);
            pos.second += (int)roundf(dy / norma * speed);
            speed  = max(8.0f,speed+0.002f); // acelera gradualmente
        }



};
