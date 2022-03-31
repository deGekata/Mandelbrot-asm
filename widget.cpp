#include "ui_widget.h"
#include "widget.h"

//=================================================================================================

float ROI_X = -1.325f, ROI_Y = 0;

//=================================================================================================

union mmxi_t;

union mmxf_t {
    __m128 m;
    float f[4];

    mmxf_t(__m128 val);
    mmxf_t(float val);
    mmxf_t(float val1, float val2, float val3, float val4);
    mmxf_t(const mmxi_t& val);

    operator __m128() const;
    operator mmxi_t() const;

    float& operator[](size_t n);
};

//-------------------------------------------------------------------------------------------------

union mmxi_t {
    __m128i m;
    int i[4];

    mmxi_t();
    mmxi_t(__m128i val);
    mmxi_t(int val);
    mmxi_t(int val1, int val2, int val3, int val4);

    operator __m128i() const;

    int& operator[](size_t n);
};

//-------------------------------------------------------------------------------------------------

inline mmxf_t::mmxf_t(__m128 val) : m(val) {}
inline mmxf_t::mmxf_t(float val) : m(_mm_set_ps1(val)) {}
inline mmxf_t::mmxf_t(float val1, float val2, float val3, float val4)
    : m(_mm_set_ps(val1, val2, val3, val4)) {}
inline mmxf_t::mmxf_t(const mmxi_t& val) : m(_mm_cvtepi32_ps(val)) {}

inline mmxf_t::operator __m128() const { return m; }
inline mmxf_t::operator mmxi_t() const { return mmxi_t(_mm_castps_si128(m)); }
inline float& mmxf_t::operator[](size_t n) { return f[n]; }

//-------------------------------------------------------------------------------------------------
inline mmxi_t::mmxi_t() {}
inline mmxi_t::mmxi_t(__m128i val) : m(val) {}
inline mmxi_t::mmxi_t(int val) : m(_mm_set1_epi32(val)) {}
inline mmxi_t::mmxi_t(int val1, int val2, int val3, int val4)
    : m(_mm_set_epi32(val1, val2, val3, val4)) {}

inline mmxi_t::operator __m128i() const { return m; }
inline int& mmxi_t::operator[](size_t n) { return i[n]; }

//-------------------------------------------------------------------------------------------------

inline mmxf_t operator+(const mmxf_t& a, const mmxf_t& b) {
    return _mm_add_ps(a, b);
}
inline mmxf_t operator-(const mmxf_t& a, const mmxf_t& b) {
    return _mm_sub_ps(a, b);
}
inline mmxf_t operator*(const mmxf_t& a, const mmxf_t& b) {
    return _mm_mul_ps(a, b);
}
inline mmxf_t operator/(const mmxf_t& a, const mmxf_t& b) {
    return _mm_div_ps(a, b);
}
inline mmxf_t operator<=(const mmxf_t& a, const mmxf_t& b) {
    return _mm_cmple_ps(a, b);
}
inline int operator+(const mmxf_t& cmp) { return _mm_movemask_ps(cmp); }

inline mmxf_t sqrt(const mmxf_t& x) { return _mm_sqrt_ps(x); }

inline mmxi_t operator+(const mmxi_t& a, const mmxi_t& b) {
    return _mm_add_epi32(a, b);
}
inline mmxi_t operator-(const mmxi_t& a, const mmxi_t& b) {
    return _mm_sub_epi32(a, b);
}

double lerp(double v0, double v1, double t) { return v0 + t * (v1 - v0); }

Widget::Widget(QWidget* parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);

    label = new QLabel(this);
    label->setMinimumSize(1, 1);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    pix = new QPixmap(this->width(), this->height());
    pix->fill(QColor("red"));
    label->setPixmap(*pix);
    qDebug() << opers.size();

    box = new QGridLayout;
    box->setGeometry(this->rect());
    box->addWidget(label);
    box->setContentsMargins(0, 0, 0, 0);
    box->setSpacing(0);
    this->setLayout(box);
    QColor BUFF;
    palette.resize(iter_max + 2);
    for (int it = 0; it < iter_max + 2; ++it) {
        BUFF.setHsv(360 * (static_cast<double>(it) / iter_max), 180, 240);
        palette[it] = BUFF;
    }

    //    re_draw();
    qDebug() << "a;s;df";
    //    delete opers[0];
    qDebug() << cur_it << "   " << opers.size() << "inti";
}

Widget::~Widget() { delete ui; }

void Widget::keyReleaseEvent(QKeyEvent* event) {
    switch (event->key()) {
        case Qt::Key_Left: {
            qDebug() << "left";
            if (event->modifiers() & Qt::ControlModifier)
                lft_bar -= move_dx * 5, rht_bar -= move_dx * 5;
            else
                lft_bar -= move_dx, rht_bar -= move_dx;
            break;
        }

        case Qt::Key_Right: {
            if (event->modifiers() & Qt::ControlModifier)
                lft_bar += move_dx * 5, rht_bar += move_dx * 5;
            else
                lft_bar += move_dx, rht_bar += move_dx;
            break;
        }

        case Qt::Key_Down: {
            if (event->modifiers() & Qt::ControlModifier)
                top_bar -= move_dx * 5, bot_bar -= move_dx * 5;
            else
                top_bar -= move_dx, bot_bar -= move_dx;
            break;
        }

        case Qt::Key_Up: {
            if (event->modifiers() & Qt::ControlModifier)
                top_bar += move_dx * 5, bot_bar += move_dx * 5;
            else
                top_bar += move_dx, bot_bar += move_dx;
            break;
        }

        case Qt::Key_Minus: {
            float delta_x = (rht_bar - lft_bar) * (scaleUp - 1.f);
            rht_bar += delta_x, lft_bar -= delta_x;
            float delta_y = (top_bar - bot_bar) * (scaleUp - 1.f);
            top_bar += delta_y, bot_bar -= delta_y;
            move_dx *= scaleUp;
            move_dy *= scaleUp;
            break;
        }

        case Qt::Key_Plus: {
            float delta_x = (rht_bar - lft_bar) * (1.f - scaleDown);
            rht_bar -= delta_x, lft_bar += delta_x;
            float delta_y = (top_bar - bot_bar) * (1.f - scaleDown);
            top_bar -= delta_y, bot_bar += delta_y;
            move_dx *= scaleDown;
            move_dy *= scaleDown;
            qDebug() << "move " << move_dx << "  " << move_dy << "\n";
            break;
        }

        default:
            break;
    }

    update();
}

void Widget::mouseMoveEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    return;
}

void Widget::mousePressEvent(QMouseEvent* event) {
    Q_UNUSED(event);
    this->setMouseTracking(true);
    lft_bar += 0.2;
    QWidget::update();
}

void Widget::mouseReleaseEvent(QMouseEvent* event) {}

void Widget::re_draw() {
    double delta_x =
        qAbs((rht_bar - lft_bar)) / (1.0 * (double)(render_size.width()));
    double delta_y =
        qAbs((top_bar - bot_bar)) / (1.0 * (double)(render_size.height()));

    QImage* img = new QImage(render_size, QImage::Format_ARGB32_Premultiplied);
    QPainter* painter = new QPainter(img);
    QPen pen;

    const mmxf_t rSMax = 16;
    const mmxf_t _255 = 255.f;
    const mmxf_t _3210(3.f, 2.f, 1.f, 0.f);
    const mmxf_t nmax = iter_max;

    for (int iy = 0; iy < render_size.height(); iy++) {
        float x0 = lft_bar, y0 = bot_bar + iy * delta_y;

        for (int ix = 0; ix < render_size.width(); ix += 4, x0 += delta_x * 4) {
            mmxf_t X0 = x0 + delta_x * _3210;
            mmxf_t Y0 = y0;

            mmxf_t X = X0, Y = Y0;
            mmxi_t N = 0;

            for (int n = 0; n < iter_max; n++) {
                mmxf_t x2 = X * X, y2 = Y * Y;
                mmxf_t r2 = x2 + y2;

                mmxf_t cmp = (r2 <= rSMax);
                if (!+cmp) break;
                mmxi_t conv = mmxi_t(_mm_castps_si128(cmp.m));

                N = N - conv;

                mmxf_t xy = X * Y;

                X = x2 - y2 + X0;
                Y = xy + xy + Y0;
            }

            //            mmxf_t I = sqrt (sqrt (N/nmax)) * _255;

            for (int i = 0; i < 4; i++) {
                unsigned char c = (unsigned char)(N[i]);
                //                qDebug() << c << "sym \n";
//                pen.setColor(
//                    (N[i] < iter_max)
//                        ? smooth_color(c, len_sqr(ComplexConstructorTmp(X[i], Y[i])))
//                        : QColor(0, 0, 0));

                pen.setColor(
                    (N[i] < iter_max)
                        ? QColor(255, 255, 255)
                        : QColor(0, 0, 0));

                painter->setPen(pen);
                painter->drawPoint(ix + i, iy);
                //                qDebug() << ix + i << "- ix   " << iy << "\n";
                //                RGBQUAD color = (N[i] < nMax)? RGBQUAD {
                //                (BYTE) (255-c), (BYTE) (c%2 * 64), c } :
                //                RGBQUAD {};
            }
        }
    }

    img->save(QString("c:\\name\\lol") + QString::number(m_frameCount) +
                  QString(".jpg"),
              "JPG");

    pix = new QPixmap(QPixmap::fromImage(*img));
    delete img;
    //    painter->end();
    //    delete painter;
    //    buff = new QPixmap(*pix);
    //    is_swaped = false;

    label->setPixmap(pix->scaled(this->width(), this->height()));
}

void Widget::moveEvent(QMoveEvent* event) { Q_UNUSED(event); }
void Widget::resizeEvent(QResizeEvent* event) {
    Q_UNUSED(event);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    label->setPixmap(pix->scaled(this->width(), this->height()));
}

void Widget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);
    re_draw();
    if (m_frameCount == 0) {
        m_time.start();
        qDebug() << "lol \n";
    } else {
        qDebug() << "FPS is" << 1000 / (m_time.elapsed() / float(m_frameCount))
                 << ", frame count:" << m_frameCount << "\n";
        if (m_frameCount == 50) {
            m_time.start();
            m_frameCount = 0;
        }
    }
    ROI_X += 0.05;
    m_frameCount++;
}

QColor Widget::smooth_color(double n, double radius) {
    double lzn = std::log(radius) / 2;
    double nu = std::log(lzn / std::log(2)) / std::log(2);
    n = n + 1 - nu;
    QColor fc, sc, res;

    if (int(n) < 0 || int(n) > 255) {
        n = 100;
    }

    fc = palette[int(n)];
    sc = palette[int(n) + 1];
    res.setRgb(lerp(fc.red(), sc.red(), n - (int)n),
               lerp(fc.green(), sc.green(), n - (int)n),
               lerp(fc.blue(), sc.blue(), n - (int)n));
    return res;
}
