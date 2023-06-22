#ifndef MEU_FRAME_H
#define MEU_FRAME_H

#include <QFrame>
#include "elemento.h"
#include "pokemon.h"

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    MyFrame(QWidget *parent = nullptr);

public slots:
    void colorir();
    void translacao();
    void escala();
    void rotacao();
    void darZoom(int zoom);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QList<Elemento> elementos;
    int zoom = 600;
    int borda = 1;
    int codigoArea(int x, int y, int left, int top, int right, int bottom);
    bool clippingPonto(int& x, int& y, int left, int top, int right, int bottom);
    bool clippingLinha(int& x0, int& y0, int& x1, int& y1, int left, int top, int right, int bottom);
    bool clippingRetangulo(int& x, int& y, int& width, int& height, int left, int top, int right, int bottom);
    bool clippingElipse(int& x, int& y, int& radius, int left, int top, int right, int bottom);
};

#endif // MEU_FRAME_H
