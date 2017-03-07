#ifndef MYCAMERA_H
#define MYCAMERA_H

#include <QMainWindow>
#include <QCamera>
#include <QDialog>
#include <QCameraImageCapture>
#include <QCameraViewfinder>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QString>
#include <QPushButton>


class MyCamera : public QDialog
{
    Q_OBJECT

public:
    explicit MyCamera(QWidget *parent, QString);
    ~MyCamera();
    QString photoPath();


private slots:
    void myGetPhoto();

signals:
    myCloseSignal();


private:
    QCamera* mCamera;
    QCameraImageCapture* imageCapture;
    QCameraViewfinder* mCameraViewfinder;
    QCameraImageCapture* mCameraImageCapture;
    QVBoxLayout* mLayout;
    QWidget* widget;
    QPushButton* getPhotoButton;
    QString fromCameraPath;
    QString path2;
};

#endif // MAINWINDOW_H
