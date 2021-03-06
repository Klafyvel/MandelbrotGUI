/*
MandelbrotGUI - Explore Mandelbrot/Julia Set
Copyright (C) 2015  Hugo LEVY-FALK

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
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
#include <QTabWidget>
#include <QShortcut>

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
    void drawAndShow();
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

protected:
    void keyPressEvent(QKeyEvent *ev);

private:
    QImage* image;
    ImageDisplay* label;
    QHBoxLayout* layout;
    QTabWidget* tabs;


    QWidget* settings;
    QWidget* controls;
    QFormLayout* settingsArea;
    QVBoxLayout* controlArea;
    QPushButton* redraw;
    QPushButton* reset;
    QPushButton* saveImage;
    QPushButton* zoom;
    QPushButton* zoomPlus;
    QPushButton* zoomMinus;
    QCheckBox* displayResult;

    QGridLayout* pad;
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
    QSpinBox* inputPixelSizeX;
    QSpinBox* inputPixelSizeY;
    QDoubleSpinBox* inputRealSizeX;
    QDoubleSpinBox* inputRealSizeY;
    ColorButton* color;

    bool settingZoom1;
    bool settingZoom2;
    QPoint firstPointZoom;

    int maxIter;
    int colorRatio;
    double beginX;
    double beginY;
    double sizeRealX;
    double sizeRealY;
    int sizePxX;
    int sizePxY;

    bool isJulia;
    double orig_x;
    double orig_y;

};

#endif // Fractale_H
