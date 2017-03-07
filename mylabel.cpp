#include "mylabel.h"

MyLabel::MyLabel(QObject *parent)
{
      //this->setAlignment(Qt::AlignCenter);
      //Default Label Value
      //this->setText("No Value");
      //set MouseTracking true to capture mouse event even its key is not pressed
      setMouseTracking(true);
}
MyLabel::~MyLabel()
{

}
void MyLabel::mouseMoveEvent(QMouseEvent* event)
{
    //x = event->x();
    //y = event->y();
}
void MyLabel::mouseReleaseEvent(QMouseEvent* event)
{
        xRelease = event->x();
        yRelease = event->y();
        //this->setText("X:"+QString::number(x)+"-- Y:"+QString::number(y));
        emit mySignalLabel();
}
void MyLabel::mousePressEvent(QMouseEvent *event)
{
    xPress = event->x();
    yPress = event->y();
}


