#include "mainwindow.h"
#include <QApplication>
#include <ctime>
#include <cstdlib>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)//, maskaR(0xff), maskaG(0xff00), maskaB(0xff0000)
{
    pictureAmount = 0;
    mCamera = NULL;
    //this->setStyleSheet("background-color: blue;");
    //setStyleSheet("QMainWindow { background-image: url(C:/Users/Łukasz/Desktop/backgroundOrange.jpg); }");
    setStyleSheet("QPushButton { color: white; background-color: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #88d, stop: 0.1 #99e, stop: 0.49 #77c, stop: 0.5 #66b, stop: 1 #77c); border-width: 1px; border-color: #339; border-style: solid; border-radius: 7; solid; padding: 3px;font-size: 10px; padding-left: 5px;  padding-right: 5px;}  }");
    setWindowTitle("Systemy wizyjne - projekt zaliczeniowy");
    raportString = "";
    //Rozmiar okna:
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    setSizePolicy(sizePolicy);

    //Rozmiar obrazkow:
    sizeX = 400; sizeY = 400;

    setUpLookTable();

    //Tworzenie obrazków:
    image1 = QImage(sizeX, sizeY, QImage::Format_ARGB32); //ok
    image2 = QImage(sizeX, sizeY, QImage::Format_ARGB32);

    for(int l = 0 ; l<sizeX ; l++)
    {
        for(int c = 0 ; c<sizeY ; c++)
        {
            if(image1.pixel(l,c)>16777215*0.2)
            image1.setPixel(l, c, 16777215);
            else
                image1.setPixel(l,c,0);
        }
    }


    //Ustawianie ramki pierwszej:
    label1 = new MyLabel;
    my_set_label_function(label1, image1);

    //Ustawienie ramki drugiej:
    label2 = new MyLabel;
    my_set_label_function(label2, image2);

    //GroupBox przycisków operacji:
    operationGroupBox = createOperationGroupBox();
    //GroupBox wczytania obrazka pierwszego:
    loadPictureGroupBox = createLoadPictureGroupBox();
    //GroupBox raportu:
    raportGroupBox = createRaportBox();

    //Tworzenie layoutu:
    myGridLayout = new QGridLayout;
    myGridLayout->addWidget(label1, 1, 0);
    myGridLayout->addWidget(label2, 1, 2);
    myGridLayout->addWidget(loadPictureGroupBox,0,0);
    myGridLayout->addWidget(operationGroupBox,1,1);
    myGridLayout->addWidget(raportGroupBox,2,0,1,3);

    //Połączenia
    allConnection();

    //Tworzenie widgetu:
    widget = new QWidget;
    widget->setLayout(myGridLayout);
    this->setCentralWidget(widget);
    createMenu();
}
MainWindow::~MainWindow()
{
    //TRZEBA DODAĆ USUWANIE OBIEKTÓW!!!!!!!
   // delete operationGroupBox;
    //delete createOperationGroupBox();
   // delete loadPictureGroupBox;
    //delete createLoadPictureGroupBox();
   // delete raportGroupBox;
    //delete createRaportBox();

    delete label1;
    delete label2;
    delete button1;
    delete button2;
    delete monochromButton;
    delete binaryzacjaButton;
    delete segmentacjaButton;
    delete sliderBinaryzacja;
    delete sliderBinaryzacjaLabel;
    delete helpLabel;
    delete srodekCiezkosciButton;
    delete maxAndMinWspButton;
    delete areaButton;
    delete indeksacjaOneButton;
    delete clearRaportButton;
    delete getPathButton; // pobranie ścieżki do obraz
    delete textLabelWczytaj;
    delete image1Path;
    delete raport;
    delete myGridLayout;
    delete widget;
    delete fileMenu;
    delete action_fileSave;
    delete action_quitMyApp;
    delete wzor;

    if(mCamera != NULL) delete mCamera;


}
void MainWindow::allConnection()
{
    connect(button1,SIGNAL(clicked()), this, SLOT(transformPicture1()));
    connect(button2,SIGNAL(clicked()), this, SLOT(transformPicture2()));
    connect(getPathButton,SIGNAL(clicked()), this, SLOT(loadPictureFromPath()));
    connect(monochromButton, SIGNAL(clicked()), this, SLOT(monochrom()));
    connect(binaryzacjaButton, SIGNAL(clicked()), this, SLOT(binaryzacja()));
    connect(segmentacjaButton, SIGNAL(clicked()), this, SLOT(segmentacja()));
    connect(sliderBinaryzacja, SIGNAL(valueChanged(int)),this, SLOT(changeSliderLable()));
    connect(label1,SIGNAL(mySignalLabel()), this, SLOT(clickTransform()));
    connect(srodekCiezkosciButton, SIGNAL(clicked()), this, SLOT(mechanika()));
    connect(maxAndMinWspButton, SIGNAL(clicked()), this, SLOT(maxAndMinWspSlot()));
    connect(areaButton, SIGNAL(clicked()), this, SLOT(areaSlot()));
    connect(indeksacjaOneButton, SIGNAL(clicked()), this, SLOT(coloreOne()));
    connect(clearRaportButton, SIGNAL(clicked()), this, SLOT(clearRaportSlot()));
}
void MainWindow::binaryzacja()
{
    image2 = QImage(image1.width(), image1.height(), QImage::Format_ARGB32);
    for(int l = 0 ; l<image1.width() ; l++)
    {
        for(int c = 0 ; c<image1.height() ; c++)
        {
            if(-image1.pixel(l,c)<sliderBinaryzacja->value())
                image2.setPixel(l, c, -1);
            else
                image2.setPixel(l,c, -16777215);
        }
    }
    int progProcentowy;
    progProcentowy = (sliderBinaryzacja->value()*100)/sliderBinaryzacja->maximum();
    raportString += "Binaryzacja z progiem:  ";
    raportString += QString::number(progProcentowy);
    raportString += "% \n";
    raport->setText(raportString);

    image2 = image2.convertToFormat(QImage::Format_ARGB32);
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);

    //Ustawienie na dole tekstu.
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::segmentacja()
{
    image2 = QImage(image1.width(), image1.height(), QImage::Format_ARGB32);
    int number;
    for(int l = 0 ; l<image1.width() ; l++)
    {
        for(int c = 0 ; c<image1.height() ; c++)
        {
            number = (image1.pixel(l,c)*99)/sliderBinaryzacja->maximum();
            image2.setPixel(l,c, lut[number]);
        }
    }
    raportString += "Segmentacja\n";
    raport->setText(raportString);

    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);

    //Ustawienie na dole tekstu.
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::transformPicture1()
{

    image2 = QImage(image1.width(), image1.height(), QImage::Format_ARGB32);
    for(int l = 0 ; l<image1.width() ; l++)
    {
        for(int c = 0 ; c<image1.height() ; c++)
        {
            image2.setPixel(l, c, image1.pixel(l,c));
        }
    }
    image2 = image2.convertToFormat(QImage::Format_ARGB32);
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);

}
void MainWindow::transformPicture2()
{
    /*
    image1 = QImage(image2.width(), image2.height(), QImage::Format_ARGB32);
    for(int l = 0 ; l<image2.width() ; l++)
    {
        for(int c = 0 ; c<image2.height() ; c++)
        {
              image1.setPixel(l, c, image2.pixel(l,c));
        }
    }
    QPixmap help1 = QPixmap::fromImage(image1);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label1->setPixmap(help);
    */
    image1 = image2.convertToFormat(QImage::Format_ARGB32);;
    QPixmap help = QPixmap::fromImage(image1);
    label1->setPixmap(help.scaled(QSize(sizeX, sizeY)));
}
void MainWindow::loadPictureFromPath()
{
    image1 = QImage(image1Path->text());
    image1.convertToFormat(QImage::Format_ARGB32);
    QPixmap help1 = QPixmap::fromImage(image1);
    label1->setPixmap(help1.scaled(QSize(sizeX, sizeY)));
}
void MainWindow::my_set_label_function(MyLabel* label, QImage &image)
{
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    label->setSizePolicy(sizePolicy);
    label->setMaximumSize(QSize(sizeX,sizeY));
    label->setMinimumSize(QSize(sizeX,sizeY));
    QPixmap help1 = QPixmap::fromImage(image);
    label->setPixmap(help1.scaled(QSize(sizeX, sizeY)));
}
QRgb MainWindow::zwroc_mono(QRgb color)
{
    QRgb r, g, b, rr, gg, bb;
    r = color & maskaR;
    g = (color & maskaG) / 0x100;
    b = (color & maskaB) / 0x10000;

    rr = 0.299*r + 0.587*g +0.114*b;
    gg = 0.299*r + 0.587*g +0.114*b;
    bb = 0.299*r + 0.587*g +0.114*b;
    return  qRgb(rr,gg,bb);
}
void MainWindow::monochrom()
{
    image2 = QImage(image1.width(), image1.height(), QImage::Format_ARGB32);
    for(int l = 0 ; l<image1.width() ; l++)
    {
        for(int c = 0 ; c<image1.height() ; c++)
        {
            image2.setPixel(l, c, zwroc_mono(image1.pixel(l,c)));
        }
    }
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);

    raportString += "Monochromatyczny  ";
    raportString += "\n";
    raport->setText(raportString);
    raport->moveCursor(QTextCursor::End);
}
QGroupBox* MainWindow::createOperationGroupBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Operacje:"));
    button1 = new QPushButton("----->");
    button2 = new QPushButton("<-----");
    monochromButton = new QPushButton("->Monochromatyczny->");
    binaryzacjaButton = new QPushButton("->Binaryzacja->");
    segmentacjaButton = new QPushButton("->Segmentacja->");
    sliderBinaryzacja = new QSlider(Qt::Horizontal);
    sliderBinaryzacjaLabel = new QLabel(" 0%");
    helpLabel = new QLabel;
    helpLabel->setFrameShadow(QFrame::Raised);

    srodekCiezkosciButton = new QPushButton("->Środek ciężkości->");
    maxAndMinWspButton = new QPushButton("->Współrzędne max i min->");
    areaButton = new QPushButton("->Pole powierzchni->");
    indeksacjaOneButton = new QPushButton("->Indeksacja 1->");
    clearRaportButton = new QPushButton("Wyczyść raport");


    sliderBinaryzacja->setMaximum(16777215);
    sliderBinaryzacja->setMinimum(0);

    QVBoxLayout *vbox = new QVBoxLayout;
    QGridLayout *hbox = new QGridLayout;
    vbox->addWidget(button1);
    vbox->addWidget(button2);
    vbox->addWidget(monochromButton);

    hbox->addWidget(sliderBinaryzacja,0,0);
    hbox->addWidget(sliderBinaryzacjaLabel,0,1);
    vbox->addLayout(hbox);
    vbox->addWidget(binaryzacjaButton);
    //vbox->addWidget(sliderBinaryzacjaLabel);
    //vbox->addWidget(sliderBinaryzacja);
    vbox->addWidget(indeksacjaOneButton);
    vbox->addWidget(segmentacjaButton);
    vbox->addWidget(srodekCiezkosciButton);
    vbox->addWidget(maxAndMinWspButton);
    vbox->addWidget(areaButton);
    vbox->addWidget(helpLabel);
    vbox->addWidget(clearRaportButton);


    groupBox->setLayout(vbox);
    groupBox->setStyleSheet("QGroupBox { font-weight: bold; } ");
    return groupBox;
}
QGroupBox* MainWindow::createLoadPictureGroupBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Wczytaj obrazek pierwszy:"));
    QVBoxLayout *vbox = new QVBoxLayout;

    //Przycisk wczytania ścieżki
    getPathButton = new QPushButton("Wczytaj");
    //Ramki informacyjne:
    textLabelWczytaj = new QLabel("Podaj adres obrazka pierwszego:");
    //TextLineEdit - tworzenie wczytywania lokalizacji pliku:
    image1Path = new QLineEdit;

    vbox->addWidget(textLabelWczytaj);
    vbox->addWidget(image1Path);
    vbox->addWidget(getPathButton);

    groupBox->setLayout(vbox);
    groupBox->setStyleSheet("QGroupBox { font-weight: bold; } ");
    return groupBox;
}
QGroupBox* MainWindow::createRaportBox()
{
    QGroupBox *groupBox = new QGroupBox(tr("Raport:"));
    QVBoxLayout *vbox = new QVBoxLayout;

    raport = new QTextEdit;
    raport->setReadOnly(true);
    vbox->addWidget(raport);
    groupBox->setLayout(vbox);
    groupBox->setStyleSheet("QGroupBox { font-weight: bold; } ");
    return groupBox;
}
void MainWindow::changeSliderLable()
{
    int progProcentowy;
    progProcentowy = (sliderBinaryzacja->value()*100)/sliderBinaryzacja->maximum();
    QString procent = QString::number(progProcentowy);
    procent += "%";
    sliderBinaryzacjaLabel->setText(procent);
}
void MainWindow::setUpLookTable()
{
    for(int i = 0 ; i<100 ; i++)
    {
       lut[i] = qRgb(2.5*i,2.5*i,2.5*i);
    }
}
void MainWindow::clickTransform()
{
    image2 = QImage(image1.width(), image1.height(), QImage::Format_ARGB32);
    for(int l = label1->xPress ; l<label1->xRelease ; l++)
    {
        for(int c = label1->yPress ; c<label1->yRelease ; c++)
        {
            image2.setPixel(l, c, image1.pixel(l,c));
        }
    }
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);
}
void MainWindow::createMenu()
{
    fileMenu = menuBar()->addMenu(tr("&Plik"));

    action_fileSave = new QAction(tr("Zapisz"), this);
    fileMenu->addAction(action_fileSave);

    action_getPhotoFromCamera = new QAction(tr("Pobierz obraz z kamery"), this);
    fileMenu->addAction(action_getPhotoFromCamera);

    fileMenu->addSeparator();

    action_quitMyApp = new QAction(tr("Zamknij"), this);
    fileMenu->addAction(action_quitMyApp);

    connect(action_fileSave, SIGNAL(triggered()), this, SLOT(savePictureAndRaport()));
    connect(action_quitMyApp, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(action_getPhotoFromCamera, SIGNAL(triggered()), this, SLOT(mySlot_getPathFromCamera()));

    //1. quitAction = new QAction(tr("&Wyjście"), this);
    //2. quitAction->setStatusTip(tr("Wyjdź z programu."));
    //3. connect(getName, SIGNAL(triggered()), this, SLOT(openGetNameDialog()));
    //4. fileMenu->addAction(maximalize);
}
void MainWindow::savePictureAndRaport()
{
    saveFile();
}
void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,tr("Zapisz plik jako..."),tr("/home/"),tr("Pliki tekstowe (*.txt)"));
    if(fileName.isEmpty())
            return;
    QFile plik(fileName);
    plik.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    QTextStream stream(&plik);
    stream<<raportString;

    plik.close();

    image1.save("C://Users//Łukasz//Desktop//Bez tytułu.jpg");
}
void MainWindow::mechanika()
{
    int x,y;
    double x0, y0, F, Sx, Sy;
    double Jx0, Jy0, Jx0y0, Jx, Jy, Jxy, Je_0, Jt_0;
    double alfa_e, alfa_t, alfa_e_deg, alfa_t_deg, dlugosc;

    int x_s1, y_s1, x_s2, y_s2,  x_s3, y_s3, x_s4, y_s4;
    double wektor_cz[2];
    double wektor_nieb[2];
    int i, zakres_szukania;
    bool czarny;

    int pictureAmount2 = 0;
    while(pictureAmount2 != pictureAmount)
    {
        F = 0;
        Sx = 0;
        Sy = 0;
        Jx = 0;
        Jy = 0;
        Jxy = 0;
        czarny = false;

        for(int X = 0 ; X<image1.width() ; X++)
        {
            for(int Y = 0 ; Y<image1.height() ; Y++)
            {
                if(image1.pixel(X,Y) == (-pictureAmount2-2))
                {
                    F = F + 1;
                    Sx = Sx + Y;
                    Sy = Sy + X;
                    Jx = Jx + Y*Y;
                    Jy = Jy + X*X;
                    Jxy = Jxy + X * Y;

                    if(X > xmax) xmax = X;
                    if(X < xmin) xmin = X;
                    if(Y > ymax) ymax = Y;
                    if(Y < ymin) ymin = Y;
                }
            }
        }

        if(F==0) {}
        else
        {
            x0 = Sy / F;
            y0 = Sx / F;

            for(int i = x0 - 2 ; i< x0 + 2 ; i++)
            {
                for( int j = y0 - 2 ; j<y0+2 ; j++)
                {
                    image2.setPixel(i, j, qRgb(255,0,0));
                }
            }
        }

        raportString = raportString + "Element nr " + QString::number(pictureAmount2+1) + " -> ";
        raportString = raportString + "Środek ciężkości x: "+ QString::number(x0) +", y: "+QString::number(y0) +"\n";
        pictureAmount2++;
    }
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);
    raport->setText(raportString);
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::maxAndMinWspSlot()
{
    int pictureAmount2 = 0;
    while(pictureAmount2 != pictureAmount)
    {
        xmax = 0; ymax = 0; xmin = 1000; ymin = 1000;
        for(int X = 0 ; X<image1.width() ; X++)
        {
            for(int Y = 0 ; Y<image1.height() ; Y++)
            {
                if(image1.pixel(X,Y) == (-pictureAmount2-2))
                {
                    if(X > xmax) xmax = X;
                    if(X < xmin) xmin = X;
                    if(Y > ymax) ymax = Y;
                    if(Y < ymin) ymin = Y;
                }
            }
        }
        pictureAmount2++;
        raportString = raportString + "Element nr " + QString::number(pictureAmount2)+" -> " + "Xmax: "+QString::number(xmax)+", Xmin: "+QString::number(xmin) + ", Ymax: "+QString::number(ymax)+", Ymin: "+QString::number(ymin)+"\n";
    }
    raport->setText(raportString);
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::areaSlot()
{
    int pictureAmount2 = 0;
    while(pictureAmount2 != pictureAmount)
    {
        area = 0;
        for(int X = 0 ; X<image1.width() ; X++)
        {
            for(int Y = 0 ; Y<image1.height() ; Y++)
            {
                if(image1.pixel(X,Y) == (-pictureAmount2-2))
                {
                    area +=1;
                }
            }
        }
        pictureAmount2++;
        raportString = raportString +"Element nr" + QString::number(pictureAmount2)+" -> "+ "Pole powierzchni: " + QString::number(area) + "\n";
    }

    raport->setText(raportString);
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::fill_obszar(int x,int y,int kol_w,int kol_b)
{
    if(x<0 || y<0 || x >= wzor->width() || y >= wzor->height()) return;

    int c = wzor->pixel(x, y);
    if(c != -1) return;
    if ((c!=kol_w) && (c!=kol_b))
    {
        area += 1;
        wzor->setPixel(x, y, kol_b);
        fill_obszar(x, y-1, kol_w, kol_b);
        fill_obszar(x, y+1, kol_w, kol_b);
        fill_obszar(x-1, y, kol_w, kol_b);
        fill_obszar(x+1, y, kol_w, kol_b);
     }
}
void MainWindow::coloreOne()
{
    bool czy_juz = true;
    pictureAmount = 0;

    //wzor = image1;//.convertToFormat(QImage::Format_ARGB32);
    //wzor = wzor.scaled(QSize(sizeX, sizeY));

    wzor = new QImage(sizeX, sizeY, QImage::Format_ARGB32);
    *wzor = image1;

    for(int i = 0 ; i<wzor->width() ; i++)
    {
        wzor->setPixel(i, 0, -16777215);
        wzor->setPixel(i, wzor->height()-1, -16777215);
    }

    for(int i = 0 ; i<wzor->height() ; i++)
    {
        wzor->setPixel(0, i, -16777215);
        wzor->setPixel(wzor->width()-1, i, -16777215);
    }

    while(czy_juz && pictureAmount<100)
    {
        area = 0;
        for(int l = 0 ; l<wzor->width() ; l++)
        {
            for(int c = 0 ; c<wzor->height() ; c++)
            {
                if(wzor->pixel(l,c) == - 1)
                {
                    pictureAmount++;
                    fill_obszar(l, c, -16777215,-1-pictureAmount);
                    l = wzor->width()-1;
                    break;
                }
                if(l == (wzor->width()-1)  && c == (wzor->height()-1)) czy_juz = false;
            }
        }

        if(area < 100 && czy_juz == true)
        {
            if(pictureAmount >= 1)
                pictureAmount--;
        }
        if(pictureAmount == -1) break;

    }

    raportString += "Indeksacja, ilość elementów: ";
    raportString += QString::number(pictureAmount);
    raportString += "\n";
    raport->setText(raportString);

    image2 = *wzor;
    QPixmap help1 = QPixmap::fromImage(image2);
    QPixmap help = help1.scaled(QSize(sizeX, sizeY));
    label2->setPixmap(help);
    //Ustawienie na dole tekstu.
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::clearRaportSlot()
{
    raportString = " ";
    raport->setText(raportString);
    raport->moveCursor(QTextCursor::End);
}
void MainWindow::mySlot_getPathFromCamera()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"),
                                  "untitled.jpg",
                                  tr("Images (.jpg)"));
     if(filename.isEmpty()) return;

    mCamera = new MyCamera(this, filename);
    //mCamera->activateWindow();
    if(!mCamera->exec())
    {
        fromCameraPath = mCamera->photoPath();
        image1 = QImage(fromCameraPath);
        QPixmap help1 = QPixmap::fromImage(image1);
        label1->setPixmap(help1.scaled(QSize(sizeX, sizeY)));
        delete mCamera;
    }
}
