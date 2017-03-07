#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include <QtWidgets> // potrzebne do menu
#include <QTextEdit>
#include <QPixmap>
#include <QtGui>
#include <iostream>
#include <QGroupBox>
#include <QSlider>
#include <mylabel.h>
#include <QFileDialog> // zapis odczyt dialog
#include <QTextStream>

#include <mycamera.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void saveFile();

private slots:
    void transformPicture1();
    void transformPicture2();
    void monochrom();
    void loadPictureFromPath();
    void binaryzacja();
    void segmentacja();
    void changeSliderLable();
    void clickTransform();
    void savePictureAndRaport();
    void mechanika();
    void maxAndMinWspSlot();
    void areaSlot();
    void coloreOne();
    void clearRaportSlot();
    void mySlot_getPathFromCamera();

private:
    //Groupbox na operacje:
    QGroupBox* operationGroupBox;
    QGroupBox* createOperationGroupBox();
    QGroupBox* loadPictureGroupBox;
    QGroupBox* createLoadPictureGroupBox();
    QGroupBox* raportGroupBox;
    QGroupBox* createRaportBox();

    //Pola na obrazki:
    MyLabel* label1;
    MyLabel* label2;

    //Pola tekstowe
    QLabel* textLabelWczytaj;

    //Obrazki
    QImage image1;
    QImage image2;
    QImage* wzor;

    //Layout
    QGridLayout* myGridLayout;

    //Widget:
    QWidget* widget;

    //Raport;
    QTextEdit* raport;
    QString raportString;

    //Przyciski:
    QPushButton* button1; // button ----->
    QPushButton* button2; // button <-----
    QPushButton* getPathButton; // pobranie ścieżki do obrazka
    QPushButton* monochromButton; // ->Monochromatyczny->
    QPushButton* binaryzacjaButton; // ->Binaryzacja->
    QPushButton* segmentacjaButton; // ->Segmentacja->
    QPushButton* srodekCiezkosciButton; // ->Środek ciężkości->
    QPushButton* maxAndMinWspButton; // ->Współrzędne max i min->
    QPushButton* areaButton; // ->Pole powierzchni->
    QPushButton* indeksacjaOneButton; // ->Indeksacja 1->
    QPushButton* clearRaportButton; // Wyczyść raport

    //Rozmiar obrazka:
    int sizeX, sizeY;

    //Scieżka do wczytania obrazka pierwszego:
    QLineEdit* image1Path;
    QString fromCameraPath;

    //Funkcje:
    void my_set_label_function(MyLabel* label, QImage &image); // Ustawia obrazek w ramce oraz ustawia wielkosc obrazka żeby pasowała do ramki
    QRgb zwroc_mono(QRgb color);
    void allConnection();

    //Zmienne:
    const long maskaR = 0xff ,maskaG = 0xff00 , maskaB = 0xff00;
    long maska;

    //
    QSlider* sliderBinaryzacja;
    QLabel* sliderBinaryzacjaLabel;

    //Pomocnoczy label na dole ramki zadan
    QLabel* helpLabel;

    //Lookup table:
    int lut[255];
    void setUpLookTable();

    //Menu
    QMenu* fileMenu;
    QAction* action_fileSave;
    QAction* action_quitMyApp;
    QAction* action_getPhotoFromCamera;
    void createMenu();

    int xmax, ymax, xmin, ymin, area;

    // indeksacja i kolorowanie
    void fill_obszar(int x,int y,int kol_w,int kol_b);
    int pictureAmount;

    //
    MyCamera* mCamera;

};

#endif // MAINWINDOW_H
