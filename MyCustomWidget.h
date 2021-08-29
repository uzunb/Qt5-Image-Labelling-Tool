#pragma once

#include <QPainter>
#include <QWidget>

namespace Ui {
    class MyCustomWidget;
}

class MyCustomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyCustomWidget(QWidget *parent = 0);
    MyCustomWidget(QSize mdiSize);
    MyCustomWidget(QString imagePath);
    bool mousePressed;
    bool drawStarted;
    int selectedTool;
    void setPenColor(const QColor &color);
    void setPenWidth(int w);
    QColor penColor() const {return myPenColor;}
    int penWidth() const {return myPenWidth;}
    ~MyCustomWidget();

    bool loadFile(const QString &fileName);

    void setCurrentFile(const QString &fileName);
    QSize sizeHint() const;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

public slots:
    void setSelectedTool(int t);
    void setSelectedTool2();

private:
    Ui::MyCustomWidget *ui;
    QPainter painter;
    QPixmap mPix;
    QLine mLine;
    QRect mRect;
    QRect mEllipse;
    QString curFile;
    bool isUntitled;

    QColor myPenColor;
    int myPenWidth;

};
