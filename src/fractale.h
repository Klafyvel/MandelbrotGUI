#ifndef Fractale_H
#define Fractale_H

#include <iostream>

#include <cmath>

#include <QApplication>
#include <QWidget>
#include <QtGui>
#include <QColorDialog>
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QCheckBox>
#include <QMouseEvent>
#include <QFileDialog>

class ImageDisplay : public QLabel
{
    Q_OBJECT
public:
    ImageDisplay(QWidget *parent = 0);

protected:
     void mousePressEvent(QMouseEvent *ev);

signals:
     void clicked(QPoint pos);
};
class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QWidget *parent=0);
    QColor getColor();
private:
    QColor color;
public slots:
    void chooseColor();
};

enum
{
    TOP = 1,
    LEFT = 2,
    RIGHT = 3,
    BOTTOM = 4,
};

class Fractale : public QWidget
{
    Q_OBJECT
public:
    Fractale(QWidget *parent = 0);
    Fractale(bool isJulia, QWidget *parent = 0);/*
    static Fractale& Mandelbrot(QWidget *parent = 0);
    static Fractale& Julia(QWidget *parent = 0);*/
    void draw();
    void move(int mv);

signals:

public slots:
    void changeSettingsAndRedraw();
    void manageClick(QPoint p);
    void beginZoom();
    void setZoomMinus();
    void setZoomPlus();
    void moveLeft();
    void moveRight();
    void moveTop();
    void moveBottom();
    void switchJulia(int isJulia);
    void switchInteractiveJulia();
    void save();
    void resetFigure();

private:
    QImage* image;
    ImageDisplay* label;
    QHBoxLayout* layout;

    QFormLayout* settingsArea;
    QVBoxLayout* controlArea;
    QPushButton* redraw;
    QPushButton* reset;
    QPushButton* saveImage;
    QPushButton* zoom;
    QPushButton* zoomPlus;
    QPushButton* zoomMinus;

    QPushButton* left;
    QPushButton* right;
    QPushButton* top;
    QPushButton* bottom;

    QDoubleSpinBox* inputOrigDrawX;
    QDoubleSpinBox* inputOrigDrawY;

    QCheckBox* julia;
    QDoubleSpinBox* inputOrigJuliaX;
    QDoubleSpinBox* inputOrigJuliaY;
    QPushButton* activeInteractiveJulia;
    bool activeJulia;

    QSpinBox* inputColorRatio;
    QSpinBox* inputMaxIter;
    QSpinBox* inputPixelSize;
    QDoubleSpinBox* inputRealSize;
    ColorButton* color;

    bool settingZoom1;
    bool settingZoom2;
    QPoint firstPointZoom;

    int maxIter;
    int colorRatio;
    double beginX;
    double beginY;
    double sizeReal;
    int sizePx;

    bool isJulia;
    double orig_x;
    double orig_y;

};

#endif // Fractale_H
