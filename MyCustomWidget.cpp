#include "MyCustomWidget.h"

#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QTextStream>
#include <QtWidgets>
#include "ui_mainwindow.h"

MyCustomWidget::MyCustomWidget(QWidget *parent) :
    QWidget(parent)//,
    //ui(new Ui::MyCustomWidget)
{
    //ui->setupUi(this);

    mPix = QPixmap(QWidget::size());
    mPix.fill(Qt::white);

    QScrollBar(this);

    //set everything to false as nothing has started yet
    mousePressed = false;
    drawStarted = false;

    //default is line
    selectedTool = 2;

    myPenColor = Qt::blue;
    myPenWidth = 1;

    //connect(ui.)
}

MyCustomWidget::MyCustomWidget(QSize mdiSize)
{
    mPix = QPixmap(mdiSize);
    mPix.fill(Qt::white);

    //set everything to false as nothing has started yet
    mousePressed = false;
    drawStarted = false;

    //default is line
    selectedTool = 2;

    myPenColor = Qt::blue;
    myPenWidth = 1;

    //connect(ui.)
}

MyCustomWidget::MyCustomWidget(QString imagePath)
{
    mPix = QPixmap(imagePath);
    //mPix.scaled(mPix.width() / 4, mPix.height() / 4, Qt::IgnoreAspectRatio);

    QImage tempImage;
    tempImage.load(imagePath);
    QSize imageSize = tempImage.size();
    this->setMinimumSize(imageSize);

    //set everything to false as nothing has started yet
    mousePressed = false;
    drawStarted = false;

    //default is line
    selectedTool = 2;

    myPenColor = Qt::blue;
    myPenWidth = 1;

    //connect(ui.)
}

void MyCustomWidget::setPenColor(const QColor &color)
{
    myPenColor = color;
}

void MyCustomWidget::setPenWidth(int w)
{
    myPenWidth = w;
}


void MyCustomWidget::mousePressEvent(QMouseEvent* event){
    //Mouse is pressed for the first time
    mousePressed = true;

    //set the initial line points, both are same
    if(selectedTool == 1){
        mRect.setTopLeft(event->pos());
        mRect.setBottomRight(event->pos());
    }
    else if (selectedTool == 2){
        mLine.setP1(event->pos());
        mLine.setP2(event->pos());
    }
    else if(selectedTool == 3)
    {
        mEllipse.setTopLeft(event->pos());
        mEllipse.setBottomRight(event->pos());
    }
}

void MyCustomWidget::mouseMoveEvent(QMouseEvent* event){

    //As mouse is moving set the second point again and again
    // and update continuously
    if(event->type() == QEvent::MouseMove){
        if(selectedTool == 1){
            mRect.setBottomRight(event->pos());
        }
        else if (selectedTool == 2){
            mLine.setP2(event->pos());
        }
        else if (selectedTool == 3){
            mEllipse.setBottomRight(event->pos());
        }
    }

    //it calls the paintEven() function continuously
    update();
}

void MyCustomWidget::mouseReleaseEvent(QMouseEvent *event){

    //When mouse is released update for the one last time
    mousePressed = false;
    update();
}

void MyCustomWidget::paintEvent(QPaintEvent *event){


    painter.begin(this);

    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                        Qt::RoundJoin));
    //When the mouse is pressed
    if(mousePressed){
        // we are taking QPixmap reference again and again
        //on mouse move and drawing a line again and again
        //hence the painter view has a feeling of dynamic drawing
        painter.drawPixmap(0,0,mPix);
        if(selectedTool == 1)
            painter.drawRect(mRect);
        else if(selectedTool == 2)
            painter.drawLine(mLine);
        else if(selectedTool ==3)
            painter.drawEllipse(mEllipse);

        drawStarted = true;
    }
    else if (drawStarted){
        // It created a QPainter object by taking  a reference
        // to the QPixmap object created earlier, then draws a line
        // using that object, then sets the earlier painter object
        // with the newly modified QPixmap object
        QPainter tempPainter(&mPix);
        tempPainter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap,
                            Qt::RoundJoin));
        if(selectedTool == 1)
            tempPainter.drawRect(mRect);
        else if(selectedTool == 2)
            tempPainter.drawLine(mLine);
        else if(selectedTool == 3)
            tempPainter.drawEllipse(mEllipse);

        painter.drawPixmap(0,0,mPix);
    }

    painter.end();
}

MyCustomWidget::~MyCustomWidget()
{
    //delete ui;
}

void MyCustomWidget::setSelectedTool(int t)
{
    selectedTool = t;
}

void MyCustomWidget::setSelectedTool2()
{
    selectedTool = 3;
}


bool MyCustomWidget::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("MDI"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    mPix.load(fileName,0,Qt::AutoColor);

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    //setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);

//    connect(document(), SIGNAL(contentsChanged()),
//            this, SLOT(documentWasModified()));

    return true;
}

void MyCustomWidget::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    isUntitled = false;
    //document()->setModified(false);
    setWindowModified(false);

    //setWindowTitle(userFriendlyCurrentFile() + "[*]");
}

QSize MyCustomWidget::sizeHint() const
{
    return QSize(72 * fontMetrics().width('x'),
            25 * fontMetrics().lineSpacing());

}
