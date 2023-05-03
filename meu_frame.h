#ifndef MEU_FRAME_H
#define MEU_FRAME_H

#include <QFrame>
#include "elemento.h"

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
    QPainter painter;
};

#endif // MEU_FRAME_H
