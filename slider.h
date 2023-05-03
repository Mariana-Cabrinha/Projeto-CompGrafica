#ifndef SLIDER_H
#define SLIDER_H

#include <QWidget>
#include <QSlider>

class Slider : public QSlider
{
    Q_OBJECT

public:
    explicit Slider(QWidget *parent = nullptr);

private:
    QSlider *sld_zoom;
};

#endif
