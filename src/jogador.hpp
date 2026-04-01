#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <iostream>
#include"utils.hpp"

using namespace std;
using namespace cv;


class Jogador{
    public:
        pair<int,int>pos;

        void draw_jogador_rectangle(Mat& smallFrame, CascadeClassifier& cascade, Rect recor){
            vector<Rect> faces;
            Mat grayFrame;
            Scalar color = Scalar(255,0,0);

            cvtColor(smallFrame, grayFrame, COLOR_BGR2GRAY);
            equalizeHist(grayFrame, grayFrame);

            cascade.detectMultiScale(grayFrame, faces,
                1.3, 2, 0
                |CASCADE_SCALE_IMAGE,
                Size(40, 40));

            for (Rect r : faces){
                if (recor.area() > 0 && (r & recor).area() > 0)
                    color = Scalar(0,0,255);
                else
                    color = Scalar(255,0,0);

                rectangle(smallFrame,
                    Point(cvRound(r.x), cvRound(r.y)),
                    Point(cvRound(r.x + r.width - 1), cvRound(r.y + r.height - 1)),
                    color, 3);

                pos = {r.x + r.width/2, r.y + r.height/2};
            }
        }
};
