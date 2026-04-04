#ifndef UTILS_HPP
#define UTILS_HPP

#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace cv;

// Variaveis globais legadas mantidas no header por solicitacao.
inline string wName = "Game";
inline int xi = 10;
inline int yi = 150;
inline bool sentido = true;

/**
 * @brief Draws a transparent image over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
inline void drawImage(Mat frame, Mat img, int xPos, int yPos) {
    if (frame.empty() || img.empty())
        return;

    Rect frameRect(0, 0, frame.cols, frame.rows);
    Rect targetRect(xPos, yPos, img.cols, img.rows);
    Rect clippedTarget = targetRect & frameRect;
    if (clippedTarget.width <= 0 || clippedTarget.height <= 0)
        return;

    Rect sourceRect(clippedTarget.x - targetRect.x,
                    clippedTarget.y - targetRect.y,
                    clippedTarget.width,
                    clippedTarget.height);

    Mat mask;
    vector<Mat> layers;

    split(img, layers); // seperate channels
    if (layers.size() == 4) { // img com transparencia.
        Mat rgb[3] = { layers[0],layers[1],layers[2] };
        mask = layers[3]; // png's alpha channel used as mask
        merge(rgb, 3, img);  // put together the RGB channels, now transp insn't transparent 
        img(sourceRect).copyTo(frame(clippedTarget), mask(sourceRect));
    } else {
        img(sourceRect).copyTo(frame(clippedTarget));
    }
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 * 
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
inline void drawTransRect(Mat frame, Scalar color, double alpha, Rect region) {
    Rect frameRect(0, 0, frame.cols, frame.rows);
    Rect clipped = region & frameRect;
    if (clipped.width <= 0 || clipped.height <= 0)
        return;

    Mat roi = frame(clipped);
    Mat rectImg(roi.size(), CV_8UC3, color); 
    addWeighted(rectImg, alpha, roi, 1.0 - alpha , 0, roi); 
}

inline void detectAndDraw(Mat& frame, CascadeClassifier& cascade, double scale, bool tryflip) {
    vector<Rect> faces;
    Mat grayFrame, smallFrame;
    Scalar color = Scalar(255,0,0);

    double fx = 1 / scale;
    resize( frame, smallFrame, Size(), fx, fx, INTER_LINEAR_EXACT );
    if( tryflip )
        flip(smallFrame, smallFrame, 1);
    cvtColor( smallFrame, grayFrame, COLOR_BGR2GRAY );
    equalizeHist( grayFrame, grayFrame );


    cascade.detectMultiScale( grayFrame, faces,
        1.3, 2, 0
        //|CASCADE_FIND_BIGGEST_OBJECT
        //|CASCADE_DO_ROUGH_SEARCH
        |CASCADE_SCALE_IMAGE,
        Size(40, 40) );

    
    
    // Desenha uma imagem
    Mat img = imread("inimigo.png", IMREAD_UNCHANGED);
    Rect recor;
    if (!img.empty()) {
        if (img.rows > 200 || img.cols > 200)
            resize(img, img, Size(200, 200));


        int maxX = std::max(0, smallFrame.cols - img.cols);
        if (sentido) {
            xi += 5;
            if (xi >= maxX) {
                xi = maxX;
                sentido = false;
            }
        } else {
            xi -= 5;
            if (xi <= 0) {
                xi = 0;
                sentido = true;
            }
        }

        drawImage(smallFrame, img, xi, yi);
        recor = Rect(Point(xi, yi), Point(xi + img.cols, yi + img.rows));
    }

    // Desenha quadrados com transparencia
    // double alpha = 0.3; // entre 0 e 1
    // int rectW = std::min(200, smallFrame.cols);
    // int rectH = std::min(200, smallFrame.rows);
    // drawTransRect(smallFrame, Scalar(0,255,0), alpha, Rect(0, 0, rectW, rectH));

    // int secondX = rectW;
    // int secondW = std::min(200, std::max(0, smallFrame.cols - secondX));
    // if (secondW > 0)
    //     drawTransRect(smallFrame, Scalar(255,0,0), alpha, Rect(secondX, 0, secondW, rectH));

    // Desenha um texto
    color = Scalar(0,0,255);
    putText	(smallFrame, "Placar:", Point(300, 50), FONT_HERSHEY_PLAIN, 2, color); // fonte

    // PERCORRE AS FACES ENCONTRADAS
    for (Rect r : faces)
    {
        if(recor.area() > 0 && (r & recor).area()>0){
            color = Scalar(0,0,255);
        }else{
            color = Scalar(255,0,0);
        }

        rectangle( smallFrame, Point(cvRound(r.x), cvRound(r.y)),
                    Point(cvRound((r.x + r.width-1)), cvRound((r.y + r.height-1))),
                    color, 3);
    }


    // Desenha o frame na tela
    imshow(wName, smallFrame);
}

#endif // UTILS_HPP
