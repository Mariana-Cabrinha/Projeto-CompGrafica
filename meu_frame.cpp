#include "meu_frame.h"
#include "matriz.h"
#include "elemento.h"
#include "slider.h"
#include <QVector>
#include <cmath>
#include <QPainter>
#include <QPolygon>
#include <QRandomGenerator>
#include <iostream>
#include <QSlider>

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

    painter.begin(this);

    painter.setViewport(0, 0, 200, 200);
    painter.setWindow(0, 0, 400, 400);

    for(auto& elemento : elementos) {
        QPen pen(elemento.cor, 2);
        painter.setPen(pen);

        int x, y, w, h, raio;

        switch (elemento.tipo) {
        case 'p':
            painter.drawPoint(elemento.pontos[0].matriz.matriz[0][0], elemento.pontos[0].matriz.matriz[1][0]);
            break;
        case 'l':
            painter.drawLine(elemento.pontos[0].matriz.matriz[0][0], elemento.pontos[0].matriz.matriz[1][0], elemento.pontos[1].matriz.matriz[0][0], elemento.pontos[1].matriz.matriz[1][0]);
            break;
        case 'r':
            x = std::fmin(elemento.pontos[0].matriz.matriz[0][0], elemento.pontos[1].matriz.matriz[0][0]);
            y = std::fmin(elemento.pontos[0].matriz.matriz[1][0], elemento.pontos[1].matriz.matriz[1][0]);
            w = std::abs(elemento.pontos[0].matriz.matriz[0][0] - elemento.pontos[1].matriz.matriz[0][0]);
            h = std::abs(elemento.pontos[0].matriz.matriz[1][0] - elemento.pontos[2].matriz.matriz[1][0]);
            painter.drawRect(x, y, w, h);
            break;
        case 'c':
            x = elemento.pontos[0].matriz.matriz[0][0];
            y = elemento.pontos[0].matriz.matriz[1][0];
            raio = std::sqrt(std::pow(elemento.pontos[1].matriz.matriz[0][0] - x, 2) + std::pow(elemento.pontos[1].matriz.matriz[1][0] - y, 2));
            painter.drawEllipse(QPoint(x, y), raio, raio);
            break;
        default:
            std::cout << "Tipo de elemento desconhecido: " << elemento.tipo << std::endl;
            break;
        }
    }

    painter.end();
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
    painter.setWindow(0, 0, zoom, zoom);
    update();
}
