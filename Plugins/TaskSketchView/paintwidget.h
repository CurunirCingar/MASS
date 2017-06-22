#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QImage>
#include <QPicture>
#include <QWidget>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QDebug>
//! \addtogroup TaskSketchView_imp
//! \{
class PaintWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PaintWidget(QWidget *parent = 0);
    QImage image;

private:
    bool isPressed;
    QPoint mousePos, prevMousePos;
    QPen pen;

public slots:
    void Clean();

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
};
//! \}
#endif // PAINTWIDGET_H
