#ifndef TEXTO_HPP
#define TEXTO_HPP

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <string>

using namespace cv;
using namespace std;

class Texto {
public:
    Texto(const string& conteudo = "",
          Point posicao = Point(0, 0),
          int fonte = FONT_HERSHEY_PLAIN,
          double escala = 1.0,
          Scalar cor = Scalar(255, 255, 255),
          int espessura = 1,
          int tipoLinha = LINE_AA)
        : conteudo_(conteudo),
          posicao_(posicao),
          fonte_(fonte),
          escala_(escala),
          cor_(cor),
          espessura_(espessura),
          tipoLinha_(tipoLinha) {}

    void setConteudo(const string& conteudo) { conteudo_ = conteudo; }
    void setPosicao(Point posicao) { posicao_ = posicao; }
    void setFonte(int fonte) { fonte_ = fonte; }
    void setEscala(double escala) { escala_ = escala; }
    void setCor(Scalar cor) { cor_ = cor; }
    void setEspessura(int espessura) { espessura_ = espessura; }
    void setTipoLinha(int tipoLinha) { tipoLinha_ = tipoLinha; }

    const string& getConteudo() const { return conteudo_; }
    Point getPosicao() const { return posicao_; }
    int getFonte() const { return fonte_; }
    double getEscala() const { return escala_; }
    Scalar getCor() const { return cor_; }
    int getEspessura() const { return espessura_; }
    int getTipoLinha() const { return tipoLinha_; }

    void desenhar(Mat& frame) const {
        if (frame.empty() || conteudo_.empty())
            return;

        putText(frame, conteudo_, posicao_, fonte_, escala_, cor_, espessura_, tipoLinha_);
    }

private:
    string conteudo_;
    Point posicao_;
    int fonte_;
    double escala_;
    Scalar cor_;
    int espessura_;
    int tipoLinha_;
};

#endif // TEXTO_HPP