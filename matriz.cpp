#include "matriz.h"
#include <QVector>
#include <cmath>

Matriz::Matriz(int nLinhas, int nColunas)
{
    // construir a matriz de tamanho linhas x colunas e inicializá-lo com zeros?.
}

Matriz::operator*(Matriz outraMatriz)
{
    // retornar uma nova matriz que é o produto desta matriz com outra matriz passada como argumento.
    // número de colunas desta matriz tem que ser igual ao número de linhas da outra matriz.
}

void Matriz::translacao(double dx, double dy)
{
    matriz[0][2] += dx;
    matriz[1][2] += dy;
}

void Matriz::escala(double sx, double sy)
{
    matriz[0][0] *= sx;
    matriz[1][1] *= sy;
}

void Matriz::rotacao(double angulo)
{
    // nem ideia.
}

