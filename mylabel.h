#ifndef MYLABEL_H
#define MYLABEL_H

#include <QObject>
#include <QLabel>
#include <QMessageBox>
#include <QEvent>
#include <QMouseEvent>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QObject *parent = 0);
    ~MyLabel();
    void mouseMoveEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent *eventPress);
    int xPress, yPress, xRelease, yRelease;

signals:
    void mySignalLabel();
public slots:

};

#endif // MYLABEL_H
