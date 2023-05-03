#ifndef MATRIZ_H
#define MATRIZ_H

class PontoMatriz {
public:

    double matriz[3][1];

    PontoMatriz();
    PontoMatriz(double x, double y);

    PontoMatriz operator*(double M[3][3]);

};

#endif
