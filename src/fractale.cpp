#include "fractale.h"


ImageDisplay::ImageDisplay(QWidget *parent) : QLabel(parent)
{}
void ImageDisplay::mousePressEvent(QMouseEvent *ev){
    emit clicked(ev->pos());
}
ColorButton::ColorButton(QWidget * parent) : QPushButton(parent)
{
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(chooseColor()));
    this->color = qRgb(0,255,0);
    this->setStyleSheet(QString("ColorButton {background-color: %1;} ColorButton:focus {outline:0;}").arg(this->color.name()));
}
QColor ColorButton::getColor()
{
    return this->color;
}
void ColorButton::chooseColor()
{
    this->color = QColorDialog::getColor(Qt::white);
    this->setStyleSheet(QString("ColorButton {background-color: %1;} ColorButton:focus {outline:0;}").arg(this->color.name()));
}


Fractale::Fractale(bool isJulia, QWidget *parent) :
    QWidget(parent)
{
    this->maxIter = 1000;
    this->colorRatio = 25;
    this->sizePxX = 512;
    this->sizePxY = 512;
    this->sizeRealX = 4;
    this->sizeRealY = 4;
    this->isJulia = isJulia;
    this->orig_x = 0.3;
    this->orig_y = 0.5;
    this->beginX = -2;
    this->beginY = 2;
    this->settingZoom1 = false;
    this->settingZoom2 = false;

    this->image = new QImage(this->sizePxX, this->sizePxY, QImage::Format_RGB32);
    this->label = new ImageDisplay(this);
    this->label->setPixmap(QPixmap::fromImage(*(this->image)));
    this->layout = new QHBoxLayout(this);
    this->layout->addWidget(this->label);
    this->tabs = new QTabWidget;
    this->settings = new QWidget;
    this->controls = new QWidget;


    this->settingsArea = new QFormLayout;
    this->controlArea = new QVBoxLayout;

    this->zoom = new QPushButton(QString("Zoom boite (sélection de deux points)"));
    this->redraw = new QPushButton(QString("(Re)Dessiner"));
    this->saveImage = new QPushButton(QString("Enregistrer l'image"));
    this->zoomMinus = new QPushButton(QString("Zommer en arrière"));
    this->zoomPlus = new QPushButton(QString("Zommer en avant"));
    this->pad = new QGridLayout;
    this->left = new QPushButton(QString("<"));
    this->right = new QPushButton(QString(">"));
    this->top = new QPushButton(QString("^"));
    this->bottom = new QPushButton(QString("v"));
    this->reset = new QPushButton(QString("Réinitialiser"));
    this->displayResult = new QCheckBox();

    this->displayResult->setChecked(true);

//    this->left->setMaximumWidth(32);
//    this->right->setMaximumWidth(32);
//    this->top->setMaximumWidth(32);
//    this->bottom->setMaximumWidth(32);

    this->zoomMinus->setShortcut(Qt::Key_Minus);
    this->zoomPlus->setShortcut(Qt::Key_Plus);
    this->left->setShortcut(Qt::Key_Left);
    this->right->setShortcut(Qt::Key_Right);
    this->top->setShortcut(Qt::Key_Up);
    this->bottom->setShortcut(Qt::Key_Down);
    this->saveImage->setShortcut(Qt::Key_S);
    this->redraw->setShortcut(Qt::Key_F5);

    this->inputOrigDrawX = new QDoubleSpinBox;
    this->inputOrigDrawY = new QDoubleSpinBox;

    this->julia = new QCheckBox;
    this->inputOrigJuliaX = new QDoubleSpinBox;
    this->inputOrigJuliaY = new QDoubleSpinBox;
    this->activeInteractiveJulia = new QPushButton("Activer la vue interactive");
    this->julia->setChecked(false);
    this->inputOrigJuliaX->setEnabled(false);
    this->inputOrigJuliaY->setEnabled(false);
    this->activeInteractiveJulia->setEnabled(false);
    this->activeJulia = false;

    this->inputColorRatio = new QSpinBox;
    this->inputMaxIter = new QSpinBox;
    this->inputPixelSizeX = new QSpinBox;
    this->inputPixelSizeY = new QSpinBox;
    this->inputRealSizeX = new QDoubleSpinBox;
    this->inputRealSizeY = new QDoubleSpinBox;
    this->color = new ColorButton;

    this->settingsArea->addRow(QString("Afficher le résultat des calculs :"), this->displayResult);
    this->settingsArea->addRow(QString("Origine du dessin en X :"), this->inputOrigDrawX);
    this->settingsArea->addRow(QString("Origine du dessin en Y :"), this->inputOrigDrawY);
    this->settingsArea->addRow(QString("Diviseur de coloration :"), this->inputColorRatio);
    this->settingsArea->addRow(QString("Nombre maximum d'itérations :"), this->inputMaxIter);
    this->settingsArea->addRow(QString("Taille (en pixels) de la figure en X :"), this->inputPixelSizeX);
    this->settingsArea->addRow(QString("Taille (en pixels) de la figure en Y :"), this->inputPixelSizeY);
    this->settingsArea->addRow(QString("Taille équivalente sur la figure en X :"), this->inputRealSizeX);
    this->settingsArea->addRow(QString("Taille équivalente sur la figure en Y :"), this->inputRealSizeY);
    this->settingsArea->addRow(QString("Couleur pour la figure :"), this->color);

    this->settingsArea->addRow(QString("Dessiner un ensemble de Julia :"), this->julia);
    this->settingsArea->addRow(QString("Origine de l'ensemble de Julia en X :"), this->inputOrigJuliaX);
    this->settingsArea->addRow(QString("Origine de l'ensemble de Julia en Y :"), this->inputOrigJuliaY);
    this->settingsArea->addRow(QString(), this->activeInteractiveJulia);

    this->settings->setLayout(this->settingsArea);
    this->tabs->addTab(this->settings, "Réglages");

    this->pad->addWidget(this->top, 0, 1, 1, 1);
    this->pad->addWidget(this->left, 1, 0, 1, 1);
    this->pad->addWidget(this->right, 1, 2, 1, 1);
    this->pad->addWidget(this->bottom, 1, 1, 1, 1);
//    this->controlArea->addWidget(this->top);
//    this->controlArea->addWidget(this->left);
//    this->controlArea->addWidget(this->right);
//    this->controlArea->addWidget(this->bottom);
    this->controlArea->addLayout(this->pad);
    this->controlArea->addWidget(this->zoomPlus);
    this->controlArea->addWidget(this->zoomMinus);
    this->controlArea->addWidget(this->zoom);
    this->controlArea->addWidget(this->saveImage);
    this->controlArea->addWidget(this->reset);
    this->controlArea->addWidget(this->redraw);

    this->controls->setLayout(this->controlArea);
    this->tabs->addTab(this->controls, "Contrôles");

    this->layout->addWidget(this->tabs);

    this->setLayout(this->layout);

    QObject::connect(this->activeInteractiveJulia, SIGNAL(clicked()), this, SLOT(switchInteractiveJulia()));
    QObject::connect(this->julia, SIGNAL(stateChanged(int)), this, SLOT(switchJulia(int)));
    QObject::connect(this->left, SIGNAL(clicked()), this, SLOT(moveLeft()));
    QObject::connect(this->right, SIGNAL(clicked()), this, SLOT(moveRight()));
    QObject::connect(this->top, SIGNAL(clicked()), this, SLOT(moveTop()));
    QObject::connect(this->bottom, SIGNAL(clicked()), this, SLOT(moveBottom()));
    QObject::connect(this->zoomPlus, SIGNAL(clicked()), this, SLOT(setZoomPlus()));
    QObject::connect(this->zoomMinus, SIGNAL(clicked()), this, SLOT(setZoomMinus()));
    QObject::connect(this->zoom, SIGNAL(clicked()), this, SLOT(beginZoom()));
    QObject::connect(this->saveImage, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(this->reset, SIGNAL(clicked()), this, SLOT(resetFigure()));
    QObject::connect(this->redraw, SIGNAL(clicked()), this, SLOT(changeSettingsAndRedraw()));
    QObject::connect(this->label, SIGNAL(clicked(QPoint)), this, SLOT(manageClick(QPoint)));

    this->inputRealSizeX->setDecimals(100);
    this->inputRealSizeY->setDecimals(100);
    this->inputOrigDrawX->setDecimals(100);
    this->inputOrigDrawY->setDecimals(100);
    this->inputOrigJuliaX->setDecimals(100);
    this->inputOrigJuliaY->setDecimals(100);
    this->inputRealSizeX->setMaximumWidth(200);
    this->inputRealSizeY->setMaximumWidth(200);
    this->inputOrigDrawX->setMaximumWidth(200);
    this->inputOrigDrawY->setMaximumWidth(200);
    this->inputOrigJuliaX->setMaximumWidth(200);
    this->inputOrigJuliaY->setMaximumWidth(200);

    this->inputOrigDrawX->setMaximum(16);
    this->inputOrigDrawX->setMinimum(-16);
    this->inputOrigDrawY->setMaximum(16);
    this->inputOrigDrawY->setMinimum(-16);
    this->inputOrigJuliaX->setMaximum(16);
    this->inputOrigJuliaX->setMinimum(-16);
    this->inputOrigJuliaY->setMaximum(16);
    this->inputOrigJuliaY->setMinimum(-16);
    this->inputColorRatio->setMaximum(1000);
    this->inputColorRatio->setMinimum(1);
    this->inputMaxIter->setMaximum(100000);
    this->inputMaxIter->setMinimum(100);
    this->inputPixelSizeX->setMaximum(2048);
    this->inputPixelSizeY->setMaximum(2048);
    this->inputPixelSizeX->setMinimum(256);
    this->inputPixelSizeY->setMinimum(256);
    this->inputRealSizeX->setMaximum(16);
    this->inputRealSizeY->setMaximum(16);
    this->inputRealSizeX->setMinimum(0.0000001);
    this->inputRealSizeY->setMinimum(0.0000001);

    this->inputRealSizeX->setSingleStep(0.01);
    this->inputRealSizeY->setSingleStep(0.01);
    this->inputOrigDrawX->setSingleStep(0.01);
    this->inputOrigDrawY->setSingleStep(0.01);
    this->inputOrigJuliaX->setSingleStep(0.01);
    this->inputOrigJuliaY->setSingleStep(0.01);

    this->inputOrigDrawX->setValue(this->beginX);
    this->inputOrigDrawY->setValue(this->beginY);
    this->inputOrigJuliaX->setValue(this->orig_x);
    this->inputOrigJuliaY->setValue(this->orig_y);
    this->inputColorRatio->setValue(this->colorRatio);
    this->inputMaxIter->setValue(this->maxIter);
    this->inputPixelSizeX->setValue(this->sizePxX);
    this->inputPixelSizeY->setValue(this->sizePxY);
    this->inputRealSizeX->setValue(this->sizeRealX);
    this->inputRealSizeY->setValue(this->sizeRealY);


}
void Fractale::changeSettingsAndRedraw()
{
    this->maxIter = this->inputMaxIter->value();
    this->colorRatio = this->inputColorRatio->value();
    this->sizeRealX = this->inputRealSizeX->value();
    this->sizeRealY = this->inputRealSizeY->value();
    this->beginX = this->inputOrigDrawX->value();
    this->beginY = this->inputOrigDrawY->value();
    this->orig_x = this->inputOrigJuliaX->value();
    this->orig_y = this->inputOrigJuliaY->value();

    if(this->sizePxX != this->inputPixelSizeX->value() && this->displayResult->isChecked())
    {
        double factor = double(this->inputPixelSizeX->value())/this->sizePxX;
        this->beginX += (this->sizeRealX - this->sizeRealX*factor)/2;
        this->sizeRealX *= factor;
        this->sizePxX = this->inputPixelSizeX->value();
        this->inputRealSizeX->setValue(this->sizeRealX);
        this->inputOrigDrawX->setValue(this->beginX);
    }
    if(this->sizePxY != this->inputPixelSizeY->value() && this->displayResult->isChecked())
    {
        double factor = double(this->inputPixelSizeY->value())/this->sizePxY;
        this->beginY -= (this->sizeRealY - this->sizeRealY/factor)/2;
        this->sizeRealY /= factor;
        this->sizePxY = this->inputPixelSizeY->value();
        this->inputRealSizeY->setValue(this->sizeRealY);
        this->inputOrigDrawY->setValue(this->beginY);
    }

    delete this->image;
    this->image = new QImage(this->sizePxX, this->sizePxY, QImage::Format_RGB32);
    this->drawAndShow();
}
void Fractale::setZoomPlus()
{
    this->beginX += (this->sizeRealX - this->sizeRealX/1.25)/2;
    this->beginY -= (this->sizeRealY - this->sizeRealY/1.25)/2;
    this->sizeRealX /= 1.25;
    this->sizeRealY /= 1.25;

    this->inputRealSizeX->setValue(this->sizeRealX);
    this->inputRealSizeY->setValue(this->sizeRealY);
    this->inputOrigDrawX->setValue(this->beginX);
    this->inputOrigDrawY->setValue(this->beginY);

    this->drawAndShow();
}
void Fractale::setZoomMinus()
{
    this->beginX += (this->sizeRealX - this->sizeRealX*1.25)/2;
    this->beginY -= (this->sizeRealY - this->sizeRealY*1.25)/2;
    this->sizeRealX *= 1.25;
    this->sizeRealY *= 1.25;

    this->inputRealSizeX->setValue(this->sizeRealX);
    this->inputRealSizeY->setValue(this->sizeRealY);
    this->inputOrigDrawX->setValue(this->beginX);
    this->inputOrigDrawY->setValue(this->beginY);

    this->drawAndShow();
}

void Fractale::beginZoom()
{
    this->settingZoom1 = true;
}

void Fractale::manageClick(QPoint p)
{
    if(this->settingZoom1)
    {
        this->firstPointZoom.setX(p.x());
        this->firstPointZoom.setY(p.y());
        this->settingZoom1 = false;
        this->settingZoom2 = true;
    }
    else if(this->settingZoom2)
    {
        this->beginX = double(this->firstPointZoom.x())/this->sizePxX*this->sizeRealX+this->beginX;
        this->beginY = (double(this->firstPointZoom.y())/this->sizePxY)*this->sizeRealY+this->beginY;

        double delta_x = abs(p.x()-this->firstPointZoom.x());
        double delta_y = abs(p.y()-this->firstPointZoom.y());
        if(delta_x > delta_y)
        {
            this->sizeRealX = double(delta_x)/this->sizePxX*this->sizeRealX;
            this->sizeRealY = double(delta_x)/this->sizePxX*this->sizeRealX;
        }
        else
        {
            this->sizeRealX = delta_y/this->sizePxY*this->sizeRealY;
            this->sizeRealY = delta_y/this->sizePxY*this->sizeRealY;
        }

        this->inputRealSizeX->setValue(this->sizeRealX);
        this->inputRealSizeY->setValue(this->sizeRealY);
        this->inputOrigDrawX->setValue(this->beginX);
        this->inputOrigDrawY->setValue(this->beginY);

        this->drawAndShow();

        this->settingZoom2 = false;
    }
    else if (this->activeJulia)
    {
        this->orig_x = double(p.x())/this->sizePxX*this->sizeRealX+this->beginX;
        this->orig_y = double(p.y())/this->sizePxY*this->sizeRealY-this->beginY;
        this->inputOrigJuliaX->setValue(this->orig_x);
        this->inputOrigJuliaY->setValue(this->orig_y);
        this->drawAndShow();
    }
}
void Fractale::drawAndShow()
{
    this->draw();
    if(this->displayResult->isChecked())
        this->label->setPixmap(QPixmap::fromImage(*(this->image)));
}

void Fractale::draw()
{
    double c_b, z[2], c[2], t[2];
    int l;
    int r_base = this->color->getColor().red();
    int g_base = this->color->getColor().green();
    int b_base = this->color->getColor().blue();
    for(long y=0;y<this->sizePxY;y++){
        c_b = double(y)/double(this->sizePxY)*this->sizeRealY-this->beginY;
        for(long x=0; x<this->sizePxX; x++){
            if(this->isJulia) {
                z[0] = double(x)/double(this->sizePxX)*this->sizeRealX+this->beginX; z[1]= c_b;
                c[0] = this->orig_x; c[1] = this->orig_y;
            }
            else {
                c[0] = double(x)/double(this->sizePxX)*this->sizeRealX+this->beginX; c[1]= c_b;
                z[0] = 0; z[1] = 0;
            }
            l = 0;
            while(z[0]*z[0]+z[1]*z[1]<4 && l < this->maxIter)
            {
                l += 1;
                t[0] = z[0]; t[1] = z[1];
                z[0] = t[0]*t[0]-t[1]*t[1]+c[0];
                z[1] = 2*t[0]*t[1]+c[1];
            }
            int v_r = int(double(l)/double(this->colorRatio)*r_base);
            int v_g = int(double(l)/double(this->colorRatio)*g_base);
            int v_b = int(double(l)/double(this->colorRatio)*b_base);
            if (v_r > 255)
                v_r = 255;
            if (v_g > 255)
                v_g = 255;
            if (v_b > 255)
                v_g = 255;
            if(l < 1000)
                this->image->setPixel(x, y, qRgb(v_r,v_g,v_b));
            else
                this->image->setPixel(x, y, qRgb(0,0,0));
            }
    }
}
void Fractale::move(int mv)
{
    if(mv==TOP) {this->beginY += this->sizeRealY*10/100;}
    else if(mv == BOTTOM) {this->beginY -= this->sizeRealY*10/100;}
    else if(mv == LEFT) {this->beginX -= this->sizeRealX*10/100;}
    else if(mv == RIGHT) {this->beginX += this->sizeRealX*10/100;}

    this->inputOrigDrawX->setValue(this->beginX);
    this->inputOrigDrawY->setValue(this->beginY);

    this->drawAndShow();
}
void Fractale::moveLeft()
{
    this->move(LEFT);
}
void Fractale::moveRight()
{
    this->move(RIGHT);
}
void Fractale::moveTop()
{
    this->move(TOP);
}
void Fractale::moveBottom()
{
    this->move(BOTTOM);
}
void Fractale::switchJulia(int isJulia)
{
    this->inputOrigJuliaX->setEnabled(isJulia==2);
    this->inputOrigJuliaY->setEnabled(isJulia==2);
    this->isJulia = isJulia==2;
    this->activeInteractiveJulia->setEnabled(isJulia==2);
}
void Fractale::save()
{
    QString title;
    if(this->isJulia) {
        title = QString("JULIA_x_%1_y_%2.jpg").arg(this->orig_x).arg(this->orig_y);
    }
    else {
        title = QString("MANDELBROT_begX_%1_begY_%2_width_%3_height_%4.jpg").arg(QString::number(this->beginX)).arg(QString::number(this->beginY)).arg(QString::number(this->sizeRealX)).arg(QString::number(this->sizeRealY));
    }
    this->image->save(QFileDialog::getSaveFileName(this, "Enregistrer l'image", title, "Images (*.jpg *.jpeg)"), "JPEG", 100);
}
void Fractale::switchInteractiveJulia()
{
    if(this->activeJulia){
        this->activeJulia = false;
        this->activeInteractiveJulia->setText("Activer la vue interactive");
    }
    else {
        this->activeJulia = true;
        this->activeInteractiveJulia->setText("Désativer la vue interactive");
    }

}
void Fractale::resetFigure()
{
    this->maxIter = 1000;
    this->colorRatio = 25;
    this->sizePxX = 512;
    this->sizePxY = 512;
    this->sizeRealX = 4;
    this->sizeRealY = 4;
    this->isJulia = isJulia;
    this->orig_x = 0.3;
    this->orig_y = 0.5;
    this->beginX = -2;
    this->beginY = 2;
    this->inputOrigDrawX->setValue(this->beginX);
    this->inputOrigDrawY->setValue(this->beginY);
    this->inputOrigJuliaX->setValue(this->orig_x);
    this->inputOrigJuliaY->setValue(this->orig_y);
    this->inputColorRatio->setValue(this->colorRatio);
    this->inputMaxIter->setValue(this->maxIter);
    this->inputPixelSizeX->setValue(this->sizePxX);
    this->inputPixelSizeY->setValue(this->sizePxY);
    this->inputRealSizeX->setValue(this->sizeRealX);
    this->inputRealSizeY->setValue(this->sizeRealY);

    delete this->image;
    this->image = new QImage(this->sizePxX, this->sizePxY, QImage::Format_RGB32);
    this->drawAndShow();
}
