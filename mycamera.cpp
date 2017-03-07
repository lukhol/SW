#include "mycamera.h"


MyCamera::MyCamera(QWidget *parent, QString path) : QDialog(parent), path2(path)
{
    mCamera = new QCamera(this);
    setWindowTitle("Obraz z kamery");
    mCameraViewfinder = new QCameraViewfinder(this);
    mCameraImageCapture = new QCameraImageCapture(mCamera, this);
    mLayout = new QVBoxLayout;
    getPhotoButton = new QPushButton("Zrób zdjęcie");
    //connect(this, SIGNAL(myCloseSignal()), this, SLOT(close()));
    mLayout->addWidget(mCameraViewfinder);
    mLayout->addWidget(getPhotoButton);
    mCamera->setViewfinder(mCameraViewfinder);
    mLayout->setMargin(0);

    connect(getPhotoButton, SIGNAL(clicked(bool)), this, SLOT(myGetPhoto()));

    mCamera->start();
    mCamera->searchAndLock();
    mCamera->unlock();

    this->setLayout(mLayout);
    //widget = new QWidget;
    //widget->setLayout(mLayout);
    //this->setCentralWidget(widget);
}

MyCamera::~MyCamera()
{

}

void MyCamera::myGetPhoto()
{
    mCameraImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
    QImageEncoderSettings imageEncoderSettings;
    imageEncoderSettings.setCodec("image/jpeg");
    imageEncoderSettings.setResolution(400,400);
    mCameraImageCapture->setEncodingSettings(imageEncoderSettings);
    mCamera->setCaptureMode(QCamera::CaptureStillImage);

    mCamera->searchAndLock();
    mCameraImageCapture->capture(path2);
    fromCameraPath = path2;
    emit myCloseSignal();
}

QString MyCamera::photoPath()
{
    return fromCameraPath;
}
