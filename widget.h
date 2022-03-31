#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtDebug>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPoint>
#include <QLine>
#include <QImage>
#include <QPixmap>
#include <QGraphicsScene>
#include <algorithm>
#include <QGridLayout>
#include <QLabel>
#include <QTime>
#include <stdlib.h>
#include <stdio.h>
#include <complex.h>
#include <cmath>
#include <emmintrin.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

struct coord{
    double lgr, rgr, bgr, tgr;
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void moveEvent(QMoveEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void re_draw();
    void keyReleaseEvent(QKeyEvent *event);
    QColor smooth_color(double n, double num);
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    const int iter_max = 255;
    int cur_it = -1;
    float lft_bar = -2,
          rht_bar =  1,
          top_bar =  1,
          bot_bar = -1;
    float move_dx = 0.1;
    float move_dy = 0.1;
    
    float centerX = -1.325f, centerY = 0;
    const float scaleUp = 1.25f, scaleDown = 0.8f;
    QTime m_time;
    int m_frameCount = 0;

    const QSize render_size = {800, 600};
    QPixmap* pix = nullptr;

    Ui::Widget *ui;
    QLabel* label;
    QGridLayout* box;
    QVector<QColor> palette;
    QVector<QPixmap*> opers;
    QVector<coord> coords;

};
#endif // WIDGET_H
