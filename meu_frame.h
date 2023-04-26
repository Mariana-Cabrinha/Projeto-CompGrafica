#ifndef MY_FRAME_H
#define MY_FRAME_H

#include <QFrame>
#include <QPaintEvent>
#include <QPoint>
#include <QLine>
#include <QPolygon>

class MyFrame : public QFrame
{
    Q_OBJECT

public:
    explicit MyFrame(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;

private:
    bool m_colorir{false};
    float m_dx{0.0};
    float m_dy{0.0};
    float m_escala{1.0};
    float m_angulo{0.0};

public slots:
    void colorir();
    void translacao();
    void escala();
    void rotacao();

signals:
};

#endif // MY_FRAME_H
