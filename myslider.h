#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QWidget>
#include <QSlider>


class mySlider : public QSlider
{
    Q_OBJECT

public:
    explicit mySlider(QWidget *parent = 0);
    ~mySlider();

private:
    Ui::mySlider *ui;
};

#endif // MYSLIDER_H
