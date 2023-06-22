#include "meu_frame.h"
#include "matriz.h"
#include "elemento.h"
#include "slider.h"
#include "pokemon.h"
#include <QVector>
#include <cmath>
#include <QPainter>
#include <QPolygon>
#include <QRandomGenerator>
#include <iostream>
#include <QSlider>
#include <QTextStream>

MyFrame::MyFrame(QWidget *parent) : QFrame(parent) {
    setMinimumSize(200, 200);

    ElementoPonto P1(75, 75);
    Linha L1(25, 25, 50);
    Retangulo R1(10, 10, 50, 50);
    Circulo C1(50, 50, 50, 10);

    elementos.append(P1);
    elementos.append(L1);
    elementos.append(R1);
    elementos.append(C1);
}

void MyFrame::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);

    QPainter painter(this);

    painter.setViewport(0, 0, 200, 200);
    painter.setWindow(0, 0, zoom, zoom);

    int left = borda;
    int top = borda;
    int right = zoom - borda;
    int bottom = zoom - borda;

    for(auto& elemento : elementos) {
        QPen pen(elemento.cor, 2);
        painter.setPen(pen);

        int x, y, x1, y1, w, h, raio;

        switch (elemento.tipo) {
        case 'p':
            x = elemento.pontos[0].matriz.matriz[0][0];
            y = elemento.pontos[0].matriz.matriz[1][0];
            if (clippingPonto(x, y, left, top, right, bottom)) {
                painter.drawPoint(x, y);
            }
            break;
            break;
        case 'l':
            x = elemento.pontos[0].matriz.matriz[0][0];
            y = elemento.pontos[0].matriz.matriz[1][0];
            x1 = elemento.pontos[1].matriz.matriz[0][0];
            y1 = elemento.pontos[1].matriz.matriz[1][0];
            if (clippingLinha(x, y, x1, y1, left, top, right, bottom)) {
                painter.drawLine(x, y, x1, y1);
            }
            break;
        case 'r':
            x = std::fmin(elemento.pontos[0].matriz.matriz[0][0], elemento.pontos[1].matriz.matriz[0][0]);
            y = std::fmin(elemento.pontos[0].matriz.matriz[1][0], elemento.pontos[1].matriz.matriz[1][0]);
            w = std::abs(elemento.pontos[0].matriz.matriz[0][0] - elemento.pontos[1].matriz.matriz[0][0]);
            h = std::abs(elemento.pontos[0].matriz.matriz[1][0] - elemento.pontos[2].matriz.matriz[1][0]);
            if (clippingRetangulo(x, y, w, h, left, top, right, bottom)) {
                painter.drawRect(x, y, w, h);
            }
            break;
        case 'c':
            x = elemento.pontos[0].matriz.matriz[0][0];
            y = elemento.pontos[0].matriz.matriz[1][0];
            raio = std::sqrt(std::pow(elemento.pontos[1].matriz.matriz[0][0] - x, 2) + std::pow(elemento.pontos[1].matriz.matriz[1][0] - y, 2));
            if (clippingElipse(x, y, raio, left, top, right, bottom)) {
                painter.drawEllipse(QPoint(x, y), raio, raio);
            }
            break;
        default:
            std::cout << "Tipo de elemento desconhecido: " << elemento.tipo << std::endl;
            break;
        }
    }

    // Desenha a moldura interna
    QPen borderPen(Qt::black, borda);
    painter.setPen(borderPen);
    painter.drawRect(borda, borda, 200, 200);
}

void MyFrame::colorir() {
    for (auto& elemento : elementos) {
        // Gerando valores aleatórios para os componentes RGB
        int r = QRandomGenerator::global()->bounded(256);
        int g = QRandomGenerator::global()->bounded(256);
        int b = QRandomGenerator::global()->bounded(256);

        // Criando uma nova QColor com os valores aleatórios
        QColor cor_aleatoria(r, g, b);

        elemento.cor = cor_aleatoria;
    }

    update();
}

void MyFrame::translacao() {
    for (auto& elemento : elementos) {
        QVector<Ponto> novos_pontos;
        double T[3][3] = {{1, 0, 3}, {0, 1, 3}, {0, 0, 1}};
        for (auto& ponto : elemento.pontos) {
            PontoMatriz novo = ponto.matriz * T;
            novos_pontos.push_back(Ponto(novo.matriz[0][0], novo.matriz[1][0]));
        }
        elemento.pontos = novos_pontos;
    }

    update();
}

void MyFrame::escala() {
    for (auto& elemento : elementos) {
        QVector<Ponto> novos_pontos;
        double S[3][3] = {{1.1, 0, 0}, {0, 1.1, 0}, {0, 0, 1}};
        for (auto& ponto : elemento.pontos) {
            PontoMatriz novo = ponto.matriz * S;
            novos_pontos.push_back(Ponto(novo.matriz[0][0], novo.matriz[1][0]));
        }
        elemento.pontos = novos_pontos;
    }

    update();
}

void MyFrame::rotacao() {
    for (auto& elemento : elementos) {
        double rad = 10 * M_PI / 180.0;
        double c = cos(rad);
        double s = sin(rad);
        QVector<Ponto> novos_pontos;
        double R[3][3] = {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
        for (auto& ponto : elemento.pontos) {
            PontoMatriz novo = ponto.matriz * R;
            novos_pontos.push_back(Ponto(novo.matriz[0][0], novo.matriz[1][0]));
        }
        elemento.pontos = novos_pontos;
    }

    update();
}

void MyFrame::darZoom(int zoom) {
    this->zoom = zoom;
    update();
}

// Função para verificar a região do ponto em relação à janela de recorte
int MyFrame::codigoArea(int x, int y, int left, int top, int right, int bottom) {
    int code = 0;

    if (x < left)
        code |= 1;    // à esquerda da janela de recorte
    else if (x > right)
        code |= 2;    // à direita da janela de recorte

    if (y < top)
        code |= 4;    // acima da janela de recorte
    else if (y > bottom)
        code |= 8;    // abaixo da janela de recorte

    return code;
}

// Função para recortar um ponto
bool MyFrame::clippingPonto(int& x, int& y, int left, int top, int right, int bottom) {
    int code = codigoArea(x, y, left, top, right, bottom);
    if (code == 0) {
        // O ponto está completamente dentro da janela de recorte
        return true;
    } else {
        // O ponto está completamente fora da janela de recorte
        return false;
    }
}

// Função para recortar uma linha
bool MyFrame::clippingLinha(int& x0, int& y0, int& x1, int& y1, int left, int top, int right, int bottom) {
    int codigo0 = codigoArea(x0, y0, left, top, right, bottom);
    int codigo1 = codigoArea(x1, y1, left, top, right, bottom);
    bool aceito = false;

    while (true) {
        if ((codigo0 | codigo1) == 0) {
            // Ambos os pontos estão completamente dentro da janela de recorte
            aceito = true;
            break;
        } else if (codigo0 & codigo1) {
            // Ambos os pontos estão completamente fora da janela de recorte em uma mesma região
            break;
        } else {
            // Pelo menos um ponto está parcialmente dentro da janela de recorte
            int x, y;
            int codigoFora = codigo0 ? codigo0 : codigo1;

            // Encontra o ponto de interseção
            if (codigoFora & 1) {           // Ponto à esquerda da janela de recorte
                x = left;
                y = y0 + (y1 - y0) * (left - x0) / (x1 - x0);
            } else if (codigoFora & 2) {    // Ponto à direita da janela de recorte
                x = right;
                y = y0 + (y1 - y0) * (right - x0) / (x1 - x0);
            } else if (codigoFora & 4) {    // Ponto acima da janela de recorte
                y = top;
                x = x0 + (x1 - x0) * (top - y0) / (y1 - y0);
            } else {                        // Ponto abaixo da janela de recorte
                y = bottom;
                x = x0 + (x1 - x0) * (bottom - y0) / (y1 - y0);
            }

            if (codigoFora == codigo0) {
                x0 = x;
                y0 = y;
                codigo0 = codigoArea(x0, y0, left, top, right, bottom);
            } else {
                x1 = x;
                y1 = y;
                codigo1 = codigoArea(x1, y1, left, top, right, bottom);
            }
        }
    }

    return aceito;
}

// Função para recortar um retângulo
bool MyFrame::clippingRetangulo(int& x, int& y, int& width, int& height, int left, int top, int right, int bottom) {
    int x1 = x + width;
    int y1 = y + height;
    bool aceito = clippingLinha(x, y, x1, y, left, top, right, bottom);
    aceito &= clippingLinha(x1, y, x1, y1, left, top, right, bottom);
    aceito &= clippingLinha(x1, y1, x, y1, left, top, right, bottom);
    aceito &= clippingLinha(x, y1, x, y, left, top, right, bottom);

    x = std::min(x, x1);
    y = std::min(y, y1);
    width = std::abs(x1 - x);
    height = std::abs(y1 - y);

    return aceito;
}

// Função para recortar uma elipse
bool MyFrame::clippingElipse(int& x, int& y, int& radius, int left, int top, int right, int bottom) {
    int x1 = x + radius;
    int y1 = y + radius;
    bool aceito = clippingLinha(x, y, x1, y, left, top, right, bottom);
    aceito &= clippingLinha(x1, y, x1, y1, left, top, right, bottom);
    aceito &= clippingLinha(x1, y1, x, y1, left, top, right, bottom);
    aceito &= clippingLinha(x, y1, x, y, left, top, right, bottom);

    x = std::min(x, x1);
    y = std::min(y, y1);
    radius = std::abs(x1 - x);

    return aceito;
}
