#include "matriz.h"
#include <cmath>
#include <iostream>

PontoMatriz::PontoMatriz() {
    matriz[0][0] = 0;
    matriz[1][0] = 0;
    matriz[2][0] = 1;
}

PontoMatriz::PontoMatriz(double x, double y) {
    matriz[0][0] = x;
    matriz[1][0] = y;
    matriz[2][0] = 1;
}

PontoMatriz PontoMatriz::operator*(double M[3][3]) {
    PontoMatriz resultado;
    resultado.matriz[0][0] = (matriz[0][0] * M[0][0]) + (matriz[1][0] * M[0][1]) + (matriz[2][0] * M[0][2]);
    resultado.matriz[1][0] = (matriz[0][0] * M[1][0]) + (matriz[1][0] * M[1][1]) + (matriz[2][0] * M[1][2]);
    resultado.matriz[2][0] = (matriz[0][0] * M[2][0]) + (matriz[1][0] * M[2][1]) + (matriz[2][0] * M[2][2]);
    return resultado;
}
