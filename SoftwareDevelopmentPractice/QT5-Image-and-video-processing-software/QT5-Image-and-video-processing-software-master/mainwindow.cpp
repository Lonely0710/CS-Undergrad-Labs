#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QToolButton>
#include <QApplication>
#include <QSpinBox>
#include <QTextEdit>
#include <QMdiSubWindow>
#include <QLabel>
#include <string>
using namespace std;
#include <iostream>
#include <sstream>
#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QTranslator>
#include <QDebug>
#include <QPaintDevice>
#include <QPainter>
#include <QImage>
#include <QtCore/qmath.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
#include <QTimer>
#include "video.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/imgproc/imgproc_c.h"///for cvSmooth
#include <QProgressDialog>
#include <QThread>

//视频处理对象
VideoProcessor::VideoProcessor(MainWindow *mainWindow, QObject *parent)
    : QThread(parent),
      mainWindow(mainWindow),
      running(false),
      type(0),
      speedFactor(1.0) // 初始倍速因子为1.0
{
}

void VideoProcessor::run()
{
    Mat frame;
    while (running && capture.read(frame)) {
        // 根据 type 进行处理
        switch (type) {
            case 1:
                cvtColor(frame, frame, CV_BGR2GRAY);
                break;
            case 2:
                cvtColor(frame, frame, CV_BGR2GRAY);
                GaussianBlur(frame, frame, Size(3, 3), 0, 0, BORDER_DEFAULT);
                Canny(frame, frame, 100, 300, 3);
                break;
            case 3:
                GaussianBlur(frame, frame, Size(3, 3), 0, 0);
                break;
            case 4:
                cvtColor(frame, frame, CV_BGR2GRAY);
                threshold(frame, frame, 96, 255, THRESH_BINARY);
                break;
            case 0:
            default:
                break;
        }

        QImage image = mainWindow->MatToQImage(frame);
        emit frameReady(image);

        // 根据倍速因子调整等待时间
        msleep(static_cast<unsigned long>(1000 / (30 * speedFactor))); // 30帧每秒，调整后等待时间
    }
}

void VideoProcessor::stop()
{
    running = false;
    wait();
}

void VideoProcessor::setType(int newType)
{
    type = newType;
}

void VideoProcessor::setSpeedFactor(double factor)
{
    speedFactor = factor;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);

    connect(&timer, &QTimer::timeout, this, &MainWindow::onTimeout);
    connect(&timer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

    videoProcessor = new VideoProcessor(this);
    connect(videoProcessor, SIGNAL(frameReady(const QImage &)), this, SLOT(onFrameReady(const QImage &)));
    videoProcessor->start();

    connect(ui->pushButton_origin_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_origin_2_clicked);
    connect(ui->pushButton_gray, &QPushButton::clicked, this, &MainWindow::on_pushButton_gray_clicked);
    connect(ui->pushButton_binary, &QPushButton::clicked, this, &MainWindow::on_pushButton_binary_clicked);
    connect(ui->pushButton_smooth, &QPushButton::clicked, this, &MainWindow::on_pushButton_smooth_clicked);
    connect(ui->pushButton_edge, &QPushButton::clicked, this, &MainWindow::on_pushButton_edge_clicked);

    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    // setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    ui->statusBar->showMessage(tr("欢迎使用多功能浏览器"),2000);
    QLabel *permanent = new QLabel(this);
    permanent->setObjectName("status");
    QFont font("Microsoft YaHei", 8);  // 设置字体为微软雅黑
    permanent->setFont(font);
    permanent->setAlignment(Qt::AlignCenter);  // 设置文本居中对齐
    permanent->setStyleSheet("QLabel { color: black; background: rgba(255, 255, 255, 150); border: 1px solid rgba(255, 255, 255, 150); border-radius: 10px; }");
    permanent->setText("欢迎使用");
    ui->statusBar->addPermanentWidget(permanent);
    ui->tabWidget->setStyleSheet("QTabWidget:pane {border-top:0px;background:  transparent; }");
}

MainWindow::~MainWindow()
{
    if (capture.isOpened()) {
            capture.release();
    }
    videoProcessor->stop();
    delete videoProcessor;
    delete ui;
}

void MainWindow::on_action_Open_triggered()
{
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/images", tr("图像文件(*.jpg *.png *.bmp)"));
    if(srcDirPathListS.size()>0)
    {
        ui->tabWidget->setCurrentIndex(0);
    }
    if(srcDirPathListS.size()>=3){
        srcDirPathList =srcDirPathListS;
        srcDirPathListS.clear();
        index =0;
        QString srcDirPath = srcDirPathList.at(index);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;
        QImage images=ImageCenter(image,ui->label_other);
        ui->label_other->setPixmap(QPixmap::fromImage(images));
        ui->label_other->setAlignment(Qt::AlignCenter);
        //状态栏显示图片路径
       QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        label->setText(srcDirPath);

        QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
        QImage image1(src1);

        QString src2 = srcDirPathList.at((index+2)%srcDirPathList.size());
        QImage image2(src2);
    }else if(srcDirPathListS.size()==1){
        srcDirPathList =srcDirPathListS;
        srcDirPathListS.clear();
        index =0;
        QString srcDirPath = srcDirPathList.at(index);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;
        QImage images=ImageCenter(image,ui->label_other);
        ui->label_other->setPixmap(QPixmap::fromImage(images));
        ui->label_other->setAlignment(Qt::AlignCenter);
        //状态栏显示图片路径
        QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        label->setText(srcDirPath);
     }
    else if(srcDirPathListS.size()==2){
            srcDirPathList =srcDirPathListS;
            srcDirPathListS.clear();
            index =0;
            QString srcDirPath = srcDirPathList.at(index);
            QImage image(srcDirPath);
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            origin_path=srcDirPath;
            QImage images=ImageCenter(image,ui->label_other);
            ui->label_other->setPixmap(QPixmap::fromImage(images));
            ui->label_other->setAlignment(Qt::AlignCenter);
            //状态栏显示图片路径
            QLabel *label=ui->statusBar->findChild<QLabel *>("status");
            label->setText(srcDirPath);
            QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
            QImage image1(src1);
           }
}

void split(const string& s,vector<int>& sv,const char flag = ' ') {
    sv.clear();
    istringstream iss(s);
    string temp;

    while (getline(iss, temp, flag)) {
        sv.push_back(stoi(temp));
    }
    return;
}

//图片居中显示,图片大小与label大小相适应
QImage MainWindow::ImageCenter(QImage  qimage,QLabel *qLabel)
{
    QImage image;
    QSize imageSize = qimage.size();
    QSize labelSize = qLabel->size();

    double dWidthRatio = 1.0*imageSize.width() / labelSize.width();
    double dHeightRatio = 1.0*imageSize.height() / labelSize.height();
            if (dWidthRatio>dHeightRatio)
            {
                image = qimage.scaledToWidth(labelSize.width());
            }
            else
            {
                image = qimage.scaledToHeight(labelSize.height());
            }
            return image;

}

//选择图片
void MainWindow::on_pushButton_clicked()
{
    QStringList srcDirPathListS = QFileDialog::getOpenFileNames(this, tr("选择图片"), "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/images/", tr("图像文件(*.jpg *.png *.bmp)"));
        if(srcDirPathListS.size()>0)
        {
            ui->tabWidget->setCurrentIndex(0);
        }
        if(srcDirPathListS.size()>=3){
            srcDirPathList =srcDirPathListS;
            srcDirPathListS.clear();
            index =0;
            QString srcDirPath = srcDirPathList.at(index);
            QImage image(srcDirPath);
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            origin_path=srcDirPath;
            QImage images=ImageCenter(image,ui->label_other);
            ui->label_other->setPixmap(QPixmap::fromImage(images));
            ui->label_other->setAlignment(Qt::AlignCenter);
            //状态栏显示图片路径
           QLabel *label=ui->statusBar->findChild<QLabel *>("status");
            label->setText(srcDirPath);

            QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
            QImage image1(src1);
            QImage Image1 = ImageCenter(image1,ui->label_other_1);
            ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
            ui->label_other_1->setAlignment(Qt::AlignCenter);

            QString src2 = srcDirPathList.at((index+2)%srcDirPathList.size());
            QImage image2(src2);
            QImage Image2 = ImageCenter(image2,ui->label_other_3);
            ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
            ui->label_other_3->setAlignment(Qt::AlignCenter);
            ui->pushButton_3->setDisabled(false);
            ui->pushButton_4->setDisabled(false);
            ui->label_other_1->setVisible(true);
            ui->label_other_3->setVisible(true);
        }else if(srcDirPathListS.size()==1){
            srcDirPathList =srcDirPathListS;
            srcDirPathListS.clear();
            index =0;
            QString srcDirPath = srcDirPathList.at(index);
            QImage image(srcDirPath);
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            origin_path=srcDirPath;
            QImage images=ImageCenter(image,ui->label_other);
            ui->label_other->setPixmap(QPixmap::fromImage(images));
            ui->label_other->setAlignment(Qt::AlignCenter);
            //状态栏显示图片路径
            QLabel *label=ui->statusBar->findChild<QLabel *>("status");
            label->setText(srcDirPath);
            //有图片触发事件
            //isImage=true;
            //qDebug("%d",srcDirPathList.size());
            ui->pushButton_3->setDisabled(true);
            ui->pushButton_4->setDisabled(true);
            ui->label_other_3->setVisible(false);
            ui->label_other_1->setVisible(false);
         }
        else if(srcDirPathListS.size()==2){
                srcDirPathList =srcDirPathListS;
                srcDirPathListS.clear();
                index =0;
                QString srcDirPath = srcDirPathList.at(index);
                QImage image(srcDirPath);
                QImage Image=ImageCenter(image,ui->label_show);
                ui->label_show->setPixmap(QPixmap::fromImage(Image));
                ui->label_show->setAlignment(Qt::AlignCenter);
                origin_path=srcDirPath;
                QImage images=ImageCenter(image,ui->label_other);
                ui->label_other->setPixmap(QPixmap::fromImage(images));
                ui->label_other->setAlignment(Qt::AlignCenter);
                //状态栏显示图片路径
                QLabel *label=ui->statusBar->findChild<QLabel *>("status");
                label->setText(srcDirPath);
                //有图片触发事件
                //isImage=true;
                //qDebug("%d",srcDirPathList.size());
                QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
                QImage image1(src1);
                QImage Image1 = ImageCenter(image1,ui->label_other_1);
                ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
                ui->label_other_1->setAlignment(Qt::AlignCenter);
                ui->pushButton_3->setDisabled(false);
                ui->pushButton_4->setDisabled(false);
                ui->label_other_1->setVisible(true);
                ui->label_other_3->setVisible(false);
               }
 }

//保存
void MainWindow::on_action_Save_triggered()
{
            if(ui->label_show->pixmap()!=nullptr){
                QString filename = QFileDialog::getSaveFileName(this,
                tr("保存图片"),
                "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/",
                tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
            if (filename.isEmpty())
            {
                return;
            }
            else
            {
                if (!(ui->label_show->pixmap()->toImage().save(filename))) //保存图像
                {

                    QMessageBox::information(this,
                        tr("图片保存成功！"),
                        tr("图片保存失败！"));
                    return;
                }
                ui->statusBar->showMessage("图片保存成功！");
            }

        }else{
            QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
}

//灰度化
QImage MainWindow::gray(QImage image){
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
    QColor oldColor;

        for(int y = 0; y < newImage.height(); y++)
        {
            for(int x = 0; x < newImage.width(); x++)
            {
                oldColor = QColor(image.pixel(x,y));
                int average = (oldColor.red() + oldColor.green() + oldColor.blue()) / 3;
                newImage.setPixel(x, y, qRgb(average, average, average));
            }
        }
        return newImage;
}

//灰度化按钮
void MainWindow::on_pushButton_gray_clicked()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);

    QImage images=gray(image);

    QImage Image=ImageCenter(images,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//QImage转Mat
Mat MainWindow::QImageToMat(QImage image)
{
    Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}

//Mat转QImage
QImage MainWindow::MatToQImage(const cv::Mat& mat)
{
    static QImage lastImage;  // 静态变量用于保存上一帧图像

    // 8-bits unsigned, NO. OF CHANNELS = 1
    if (mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for (int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for (int row = 0; row < mat.rows; row++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }

        // 使用双缓冲技术显示图像
        QImage doubleBuffered = image.copy();
        lastImage = doubleBuffered;  // 更新上一帧图像
        return doubleBuffered;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if (mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);

        // 使用双缓冲技术显示图像
        QImage doubleBuffered = image.rgbSwapped().copy();
        lastImage = doubleBuffered;  // 更新上一帧图像
        return doubleBuffered;
    }
    else if (mat.type() == CV_8UC4)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);

        // 使用双缓冲技术显示图像
        QImage doubleBuffered = image.copy();
        lastImage = doubleBuffered;  // 更新上一帧图像
        return doubleBuffered;
    }
    else
    {
        return QImage();
    }
}

//均值滤波按钮
void MainWindow::on_pushButton_junzhi_clicked()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    Mat srcimage=QImageToMat(image);
    Mat images;
    Size ksize(5,5);
        cv::blur(srcimage,images,ksize);
        image=MatToQImage(images);
             QImage Image=ImageCenter(image,ui->label_show);
             ui->label_show->setPixmap(QPixmap::fromImage(Image));
             ui->label_show->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//保存
void MainWindow::on_pushButton_save_clicked()
{
            if(ui->label_show->pixmap()!=nullptr){
                QString filename = QFileDialog::getSaveFileName(this,
                tr("保存图片"),
                "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/",
                tr("*.png;; *.jpg;; *.bmp;; *.tif;; *.GIF")); //选择路径
            if (filename.isEmpty())
            {
                return;
            }
            else
            {
                if (!(ui->label_show->pixmap()->toImage().save(filename))) //保存图像
                {

                    QMessageBox::information(this,
                        tr("图片保存成功！"),
                        tr("图片保存失败！"));
                    return;
                }
                ui->statusBar->showMessage("图片保存成功！");
            }

        }else{
            QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
        }

//显示原图按钮
void MainWindow::on_pushButton_origin_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//伽马变换
QImage MainWindow::gamma(QImage image){
    double d=1.2;
    QColor color;
    int height = image.height();
    int width = image.width();
    for (int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            color = QColor(image.pixel(i,j));
            double r = color.red();
            double g = color.green();
            double b = color.blue();
            int R = qBound(0,(int)qPow(r,d),255);
            int G = qBound(0,(int)qPow(g,d),255);
            int B = qBound(0,(int)qPow(b,d),255);
            image.setPixel(i,j,qRgb(R,G,B));
        }
    }
    return image;
}

//伽马变换按钮
void MainWindow::on_pushButton_gamma_clicked()
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        image=gamma(image);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }

}

//左转
void MainWindow::on_pushButton_turnleft_clicked()
{
    if(ui->label_show->pixmap()!=nullptr){
        QImage images(ui->label_show->pixmap()->toImage());
        QMatrix matrix;
        matrix.rotate(-90.0);//逆时针旋转90度
        images= images.transformed(matrix,Qt::FastTransformation);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//右转
void MainWindow::on_pushButton_turnright_clicked()
{
    if(ui->label_show->pixmap()!=nullptr){
        QImage images(ui->label_show->pixmap()->toImage());
        QMatrix matrix;
        matrix.rotate(90.0);//逆时针旋转90度
        images= images.transformed(matrix,Qt::FastTransformation);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//左右翻转
void MainWindow::on_pushButton_turnleft_2_clicked()
{
    if(ui->label_show->pixmap()!=nullptr){
        QImage images(ui->label_show->pixmap()->toImage());
        images = images.mirrored(true, false);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//上下翻转
void MainWindow::on_pushButton_turnleft_3_clicked()
{
    if(ui->label_show->pixmap()!=nullptr){
        QImage images(ui->label_show->pixmap()->toImage());
        images = images.mirrored(false, true);
        //QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(images));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//边缘检测
QImage MainWindow::bianyuan(QImage image)
{
    QImage newImage =image.convertToFormat(QImage::Format_ARGB32);
        QColor color0;
        QColor color1;
        QColor color2;
        QColor color3;
        int  r = 0;
        int g = 0;
        int b = 0;
        int rgb = 0;
        int r1 = 0;
        int g1 = 0;
        int b1 = 0;
        int rgb1 = 0;
        int a = 0;
        for( int y = 0; y < image.height() - 1; y++)
        {
            for(int x = 0; x < image.width() - 1; x++)
            {
                color0 =   QColor ( image.pixel(x,y));
                color1 =   QColor ( image.pixel(x + 1,y));
                color2 =   QColor ( image.pixel(x,y + 1));
                color3 =   QColor ( image.pixel(x + 1,y + 1));
                r = abs(color0.red() - color3.red());
                g = abs(color0.green() - color3.green());
                b = abs(color0.blue() - color3.blue());
                rgb = r + g + b;

                r1 = abs(color1.red() - color2.red());
                g1= abs(color1.green() - color2.green());
                b1 = abs(color1.blue() - color2.blue());
                rgb1 = r1 + g1 + b1;

                a = rgb + rgb1;
                a = a>255?255:a;

                newImage.setPixel(x,y,qRgb(a,a,a));
            }
        }
        return newImage;
}

//边缘检测按钮
void MainWindow::on_pushButton_bianyuan_clicked()
{
    if(origin_path!=nullptr){
    QImage image(origin_path);
    QImage newImage =bianyuan(image);
    QImage Image=ImageCenter(newImage,ui->label_show);
    ui->label_show->setPixmap(QPixmap::fromImage(Image));
    ui->label_show->setAlignment(Qt::AlignCenter);
}
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//原图+边缘滤波复合调用函数
QImage MainWindow::fuhe(QImage images){
    QImage image2 =images.convertToFormat(QImage::Format_ARGB32);
    QColor color0;
    QColor color1;
    QColor color2;
    QColor color3;
    int  r = 0;
    int g = 0;
    int b = 0;
    int rgb = 0;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int rgb1 = 0;
    int a = 0;
    for( int y = 0; y < images.height() - 1; y++)
    {
        for(int x = 0; x < images.width() - 1; x++)
        {
            color0 =   QColor ( images.pixel(x,y));
            color1 =   QColor ( images.pixel(x + 1,y));
            color2 =   QColor ( images.pixel(x,y + 1));
            color3 =   QColor ( images.pixel(x + 1,y + 1));
            r = abs(color0.red() - color3.red());
            g = abs(color0.green() - color3.green());
            b = abs(color0.blue() - color3.blue());
            rgb = r + g + b;

            r1 = abs(color1.red() - color2.red());
            g1= abs(color1.green() - color2.green());
            b1 = abs(color1.blue() - color2.blue());
            rgb1 = r1 + g1 + b1;

            a = rgb + rgb1;
            a = a>255?255:a;

            image2.setPixel(x,y,qRgb(a,a,a));
        }
    }

QImage image(origin_path);
int red, green, blue;
int red2,green2,blue2;
    int pixels = image.width() * image.height();

    unsigned int *data = (unsigned int *)image.bits();
    unsigned int *data2 = (unsigned int *)image2.bits();

    for (int i = 0; i < pixels; ++i)
    {
        red= qRed(data[i]);
        red2=qRed(data2[i])*0.5+red*0.5;
        red2 = (red2 < 0x00) ? 0x00 : (red2 > 0xff) ? 0xff : red2;

        green= qGreen(data[i]);
        green2= qGreen(data2[i])*0.5+green*0.5;
        green2 = (green2 < 0x00) ? 0x00 : (green2 > 0xff) ? 0xff : green2;

        blue= qBlue(data[i]);
        blue2= qBlue(data2[i])*0.5+blue*0.5;
        blue2 =  (blue2  < 0x00) ? 0x00 : (blue2  > 0xff) ? 0xff : blue2 ;

        data2[i] = qRgba(red2, green2, blue2, qAlpha(data2[i]));
    }
    return image2;
}

//原图+边缘滤波复合调用函数按钮
void MainWindow::on_pushButton_bianyuan_2_clicked()
{
    if(origin_path!=nullptr){
        QImage images(origin_path);
        QImage image2 =fuhe(images);
        QImage Image=ImageCenter(image2,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//亮度调节滑块
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        int red, green, blue;
            int pixels = image.width() * image.height();
            unsigned int *data = (unsigned int *)image.bits();

            for (int i = 0; i < pixels; ++i)
            {
                red= qRed(data[i])+ value;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green= qGreen(data[i]) + value;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue= qBlue(data[i]) + value;
                blue =  (blue  < 0x00) ? 0x00 : (blue  > 0xff) ? 0xff : blue ;
                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            ui->label_light->setText(QString::number(value));
        }
        else{
            QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }

}

//更改比例滑块
void MainWindow::on_horizontalSlider_2_valueChanged(int value1)
{
    float value=(float)value1/100;
    if(origin_path!=nullptr){
        QImage images(origin_path);
        QImage image2 =images.convertToFormat(QImage::Format_ARGB32);
        QColor color0;
        QColor color1;
        QColor color2;
        QColor color3;
        int  r = 0;
        int g = 0;
        int b = 0;
        int rgb = 0;
        int r1 = 0;
        int g1 = 0;
        int b1 = 0;
        int rgb1 = 0;
        int a = 0;
        for( int y = 0; y < images.height() - 1; y++)
        {
            for(int x = 0; x < images.width() - 1; x++)
            {
                color0 =   QColor ( images.pixel(x,y));
                color1 =   QColor ( images.pixel(x + 1,y));
                color2 =   QColor ( images.pixel(x,y + 1));
                color3 =   QColor ( images.pixel(x + 1,y + 1));
                r = abs(color0.red() - color3.red());
                g = abs(color0.green() - color3.green());
                b = abs(color0.blue() - color3.blue());
                rgb = r + g + b;

                r1 = abs(color1.red() - color2.red());
                g1= abs(color1.green() - color2.green());
                b1 = abs(color1.blue() - color2.blue());
                rgb1 = r1 + g1 + b1;

                a = rgb + rgb1;
                a = a>255?255:a;

                image2.setPixel(x,y,qRgb(a,a,a));
            }
        }
    QImage image(origin_path);
//    QImage image2(label->pixmap()->toImage());
    int red, green, blue;
    int red2,green2,blue2;
        int pixels = image.width() * image.height();

        unsigned int *data = (unsigned int *)image.bits();
        unsigned int *data2 = (unsigned int *)image2.bits();

        for (int i = 0; i < pixels; ++i)
        {
            red= qRed(data[i]);
            red2=qRed(data2[i])*value+red*(1-value);
            red2 = (red2 < 0x00) ? 0x00 : (red2 > 0xff) ? 0xff : red2;

            green= qGreen(data[i]);
            green2= qGreen(data2[i])*value+green*(1-value);
            green2 = (green2 < 0x00) ? 0x00 : (green2 > 0xff) ? 0xff : green2;

            blue= qBlue(data[i]);
            blue2= qBlue(data2[i])*value+blue*(1-value);
            blue2 =  (blue2  < 0x00) ? 0x00 : (blue2  > 0xff) ? 0xff : blue2 ;

            data2[i] = qRgba(red2, green2, blue2, qAlpha(data2[i]));
        }
        QImage Image=ImageCenter(image2,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        ui->label_fuhe->setText(QString::number(value).append(":").append(QString::number(1-value)));
    }
    else{
        QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//改变r值滑动条
void MainWindow::on_horizontalSlider_R_valueChanged(int value)
{
    if(ui->label_show->pixmap()!=nullptr){
            int value_r=value;
            int value_g=ui->horizontalSlider_G->value();
            int value_b=ui->horizontalSlider_B->value();
            QImage image(origin_path);
            QImage images=setRGB(image,value_r,value_g,value_b);
            QImage Image=ImageCenter(images,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        }else{
            QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
}

//调整rgb函数调用
QImage MainWindow::setRGB(QImage image,int value_r, int value_g, int value_b){
    int r,g,b;
    QColor oldColor;
    int height=image.height();
    int width=image.width();
    for (int i = 0; i < height; ++i)
    {
        for(int j=0;j<width;++j){
            oldColor = QColor(image.pixel(j,i));
            r=oldColor.red()+value_r;
            if(r>255)
                r=255;
            g=oldColor.green()+value_g;
            if(g>255)
                g=255;
            b=oldColor.blue()+value_b;
            if(b>255)
                b=255;
            image.setPixel(j,i, qRgb(r, g, b));
        }
    }
    return image;
}

//改变g值滑动条
void MainWindow::on_horizontalSlider_G_valueChanged(int value)
{
    if(ui->label_show->pixmap()!=nullptr){
        int value_r=ui->horizontalSlider_R->value();
        int value_g=value;
        int value_b=ui->horizontalSlider_B->value();
        QImage image(origin_path);
        QImage images=setRGB(image,value_r,value_g,value_b);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//改变b值滑动条
void MainWindow::on_horizontalSlider_B_valueChanged(int value)
{
    if(ui->label_show->pixmap()!=nullptr){
        int value_r=ui->horizontalSlider_R->value();
        int value_g=ui->horizontalSlider_G->value();
        int value_b=value;
        QImage image(origin_path);
        QImage images=setRGB(image,value_r,value_g,value_b);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
    }else{
        QMessageBox::warning(nullptr, "提示", "请先打开图片！", QMessageBox::Yes |  QMessageBox::Yes);
    }
}

//二值化滑动条
void MainWindow::on_horizontalSlider_erzhi_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage images=gray(image);
        int height=images.height();
        int width=images.width();
                int bt;
                QColor oldColor;
                for (int i = 0; i < height; ++i)
                {
                    for(int j=0;j<width;++j){
                        oldColor = QColor(images.pixel(j,i));
                        bt = oldColor.red();
                    if(bt<value){
                        bt=0;
                    }else{
                        bt=255;
                    }
                    images.setPixel(j,i, qRgb(bt, bt, bt));

                    }
                }
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        ui->label_yuzhi->setText(QString::number(value));
        }
        else{
            QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
}

//调整对比度
QImage MainWindow::AdjustContrast(QImage image, int value)
{
    int pixels = image.width() * image.height();
        unsigned int *data = (unsigned int *)image.bits();

        int red, green, blue, nRed, nGreen, nBlue;

        if (value > 0 && value < 256)
        {
            float param = 1 / (1 - value / 256.0) - 1;

            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * param;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * param;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * param;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }
        else
        {
            for (int i = 0; i < pixels; ++i)
            {
                nRed = qRed(data[i]);
                nGreen = qGreen(data[i]);
                nBlue = qBlue(data[i]);

                red = nRed + (nRed - 127) * value / 100.0;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - 127) * value / 100.0;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - 127) * value / 100.0;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;

                data[i] = qRgba(red, green, blue, qAlpha(data[i]));
            }
        }

        return image;
}

//对比度滑动条
void MainWindow::on_horizontalSlider_duibi_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage images=AdjustContrast(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        }
        else{
            QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
}

//饱和度函数调用
QImage MainWindow::AdjustSaturation(QImage Img, int iSaturateValue)
{
    int red, green, blue, nRed, nGreen, nBlue;
        int pixels = Img.width() * Img.height();
        unsigned int *data = (unsigned int *)Img.bits();

        float Increment = iSaturateValue/100.0;

        float delta = 0;
        float minVal, maxVal;
        float L, S;
        float alpha;

        for (int i = 0; i < pixels; ++i)
        {
            nRed = qRed(data[i]);
            nGreen = qGreen(data[i]);
            nBlue = qBlue(data[i]);

            minVal = std::min(std::min(nRed, nGreen), nBlue);
            maxVal = std::max(std::max(nRed, nGreen), nBlue);
            delta = (maxVal - minVal) / 255.0;
            L = 0.5*(maxVal + minVal) / 255.0;
            S = std::max(0.5*delta / L, 0.5*delta / (1 - L));

            if (Increment > 0)
            {
                alpha = std::max(S, 1 - Increment);
                alpha = 1.0 / alpha - 1;
                red = nRed + (nRed - L*255.0)*alpha;
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = nGreen + (nGreen - L*255.0)*alpha;
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = nBlue + (nBlue - L*255.0)*alpha;
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            }
            else
            {
                alpha = Increment;
                red = L*255.0 + (nRed - L * 255.0)*(1+alpha);
                red = (red < 0x00) ? 0x00 : (red > 0xff) ? 0xff : red;
                green = L*255.0 + (nGreen - L * 255.0)*(1+alpha);
                green = (green < 0x00) ? 0x00 : (green > 0xff) ? 0xff : green;
                blue = L*255.0 + (nBlue - L * 255.0)*(1+alpha);
                blue = (blue < 0x00) ? 0x00 : (blue > 0xff) ? 0xff : blue;
            }

            data[i] = qRgba(red, green, blue, qAlpha(data[i]));
        }

        return Img;
}

//饱和度函数滑动条
void MainWindow::on_horizontalSlider_baohe_valueChanged(int value)
{
    if(origin_path!=nullptr){
        QImage image(origin_path);
        QImage images=AdjustSaturation(image,value);
        QImage Image=ImageCenter(images,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        }
        else{
            QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }
}

//图像锐化
QImage * MainWindow::sharpen(QImage * origin){

    QImage * newImage = new QImage(* origin);

    int kernel [3][3]= {{0,-1,0},
                        {-1,5,-1},
                        {0,-1,0}};
    int kernelSize = 3;
    int sumKernel = 1;
    int r,g,b;
    QColor color;

    for(int x=kernelSize/2; x<newImage->width()-(kernelSize/2); x++){
        for(int y=kernelSize/2; y<newImage->height()-(kernelSize/2); y++){

            r = 0;
            g = 0;
            b = 0;

            for(int i = -kernelSize/2; i<= kernelSize/2; i++){
                for(int j = -kernelSize/2; j<= kernelSize/2; j++){
                    color = QColor(origin->pixel(x+i, y+j));
                    r += color.red()*kernel[kernelSize/2+i][kernelSize/2+j];
                    g += color.green()*kernel[kernelSize/2+i][kernelSize/2+j];
                    b += color.blue()*kernel[kernelSize/2+i][kernelSize/2+j];
                }
            }

            r = qBound(0, r/sumKernel, 255);
            g = qBound(0, g/sumKernel, 255);
            b = qBound(0, b/sumKernel, 255);

            newImage->setPixel(x,y, qRgb(r,g,b));

        }
    }
    return newImage;
}

//图像锐化按钮
void MainWindow::on_pushButton_bianyuan_5_clicked()
{
    if(origin_path!=nullptr){
            QImage images(origin_path);
            QImage image2 =fuhe(images);
            QImage Image=ImageCenter(image2,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
        }

        else{
            QMessageBox::warning(nullptr, "提示", "请先选择一张图片！", QMessageBox::Yes |  QMessageBox::Yes);
        }

}

//水平拼接
QPixmap MainWindow::opencvMosaic(const Mat &mat1, const Mat &mat2)
{
    // 检查图像是否有效
    if(mat1.empty() || mat2.empty())
    {
        return QPixmap();
    }

    // 判断两张图像的高度是否相同，不相同则调整mat2的高度以匹配mat1
    cv::Mat resizedMat2;
    if(mat1.rows != mat2.rows)
    {
        cv::resize(mat2, resizedMat2, cv::Size(mat2.cols, mat1.rows));
    }
    else
    {
        resizedMat2 = mat2;
    }

    // 水平拼接图像
    cv::Mat resultMat;
    cv::hconcat(mat1, resizedMat2, resultMat);

    // 将 OpenCV 的 Mat 转换为 QImage
    if(resultMat.channels() == 3)
    {
        cv::cvtColor(resultMat, resultMat, cv::COLOR_BGR2RGB);
        QImage newImage((const unsigned char*)(resultMat.data), resultMat.cols, resultMat.rows, resultMat.step, QImage::Format_RGB888);
        // 保存拼接图像
        cv::imwrite("C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/opencvMosaic.jpg", resultMat);
        // 返回拼接图像
        return QPixmap::fromImage(newImage);
    }
    else if(resultMat.channels() == 1)
    {
        QImage newImage((const unsigned char*)(resultMat.data), resultMat.cols, resultMat.rows, resultMat.step, QImage::Format_Grayscale8);
        // 保存拼接图像
        cv::imwrite("C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/opencvMosaic.jpg", resultMat);
        // 返回拼接图像
        return QPixmap::fromImage(newImage);
    }
    else
    {
        return QPixmap();
    }
}

//图像融合
QPixmap MainWindow::opencvMerge(const cv::Mat &mat1, const cv::Mat &mat2)
{
    // 检查图像是否有效
    if(mat1.empty() || mat2.empty())
    {
        return QPixmap();
    }

    // 调整图像大小使其相同
    cv::Mat resizedMat1, resizedMat2;
    if(mat1.size() != mat2.size())
    {
        cv::resize(mat1, resizedMat1, mat2.size());
        resizedMat2 = mat2;
    }
    else
    {
        resizedMat1 = mat1;
        resizedMat2 = mat2;
    }

    // 检查调整大小后的图像类型是否相同
    if(resizedMat1.type() != resizedMat2.type())
    {
        return QPixmap();
    }

    // 图像融合（这里只是一个简单的示例, 使用加权平均）
    cv::Mat resultMat;
    double alpha = 0.5;   // 融合系数, 可以根据需要调整
    cv::addWeighted(resizedMat1, alpha, resizedMat2, 1.0 - alpha, 0.0, resultMat);

    // 将 OpenCV 的 Mat 转换为 QImage
    QImage newImage;
    if(resultMat.channels() == 3)
    {
        cv::cvtColor(resultMat, resultMat, cv::COLOR_BGR2RGB);
        newImage = QImage((const unsigned char*)(resultMat.data), resultMat.cols, resultMat.rows, resultMat.step, QImage::Format_RGB888);
    }
    else if(resultMat.channels() == 1)
    {
        newImage = QImage((const unsigned char*)(resultMat.data), resultMat.cols, resultMat.rows, resultMat.step, QImage::Format_Grayscale8);
    }
    else
    {
        return QPixmap();
    }

    // OpenCV 的 imwrite 保存拼接图像
    cv::imwrite("C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/opencvMerge.jpg", resultMat);

    // 返回拼接图像
    return QPixmap::fromImage(newImage.rgbSwapped());
}

//多图处理图像展示
void MainWindow::setShowImage(int index)
{
    QPixmap showPixmap;

    if(index == 1)
    {
        //使用OpenCV水平拼接
        showPixmap = opencvMosaic(m_mat_1,m_mat_2);
    }
    else if(index == 2)
    {
        //使用OpenCV图像融合
        showPixmap = opencvMerge(m_mat_1,m_mat_2);
    }

    //更新显示
    m_showPixmap = showPixmap;
    if(!m_showPixmap.isNull())
    {
        ui->lb_imageShow->setPixmap(showPixmap.scaled(ui->lb_imageShow->size(),Qt::KeepAspectRatio));   //保持比例
        ui->lb_imageShow->setAlignment(Qt::AlignCenter);
    }
    else
    {
        QMessageBox::warning(this,"警告","图像显示失败!");
    }
}

//纹理迁移实现
Mat MainWindow::performTextureTransfer(const Mat &mat1, const Mat &mat2, int cutCols, QProgressDialog* progressDialog)
{
    // 1. 检查图像是否有效
        if (mat1.empty() || mat2.empty()) {
            qDebug() << "图像为空.";
            return cv::Mat();
        }

        // 2. 调整图像大小使其相同
        cv::Mat resizedMat1, resizedMat2;
        if (mat1.size() != mat2.size()) {
            cv::resize(mat1, resizedMat1, mat2.size());
            resizedMat2 = mat2;
        } else {
            resizedMat1 = mat1;
            resizedMat2 = mat2;
        }

        // 3. 检查调整大小后的图像类型是否相同
        if (resizedMat1.type() != resizedMat2.type()) {
            qDebug() << "调整大小后的图像类型不同.";
            return cv::Mat();
        }

        // 4. 纹理迁移处理
        cv::Mat transferPic;
        int patchSize = 50; // 设置块大小，可以根据实际情况调整
        int overlappingSize = 10; // 设置重叠大小，可以根据实际情况调整

        int totalPatches = ((resizedMat1.rows + patchSize - 1) / patchSize) * ((resizedMat1.cols + patchSize - 1) / patchSize);
        int processedPatches = 0;

        for (int w = 0; w < resizedMat1.rows; w += patchSize - overlappingSize) {
            int rangeRow = std::min(patchSize, resizedMat1.rows - w);

            cv::Mat colsPatch, colsTempPatch;
            for (int i = 0; i < resizedMat1.cols; i += patchSize - overlappingSize) {
                int rangeCol = std::min(patchSize, resizedMat1.cols - i);

                // 创建当前块
                cv::Mat patch = resizedMat1(cv::Rect(i, w, rangeCol, rangeRow));

                // 在纹理图片中搜索相似区域
                cv::Mat similarPatch = SearchForSimilarAreas(patch, resizedMat2);

                // 水平拼接
                if (i == 0) {
                    colsPatch = similarPatch.clone();
                } else {
                    if (rangeCol > overlappingSize)
                        colsTempPatch = Horizontal(colsPatch, similarPatch, overlappingSize).clone();
                    else
                        break;
                    colsPatch = colsTempPatch.clone();
                }

                // 更新进度条
                processedPatches++;
                    progressDialog->setValue(static_cast<int>((processedPatches / static_cast<double>(totalPatches)) * 100));
                    QCoreApplication::processEvents();

                    if (progressDialog->wasCanceled()) {
                        return cv::Mat();
                    }
            }

            // 垂直拼接
            if (w == 0) {
                transferPic = colsPatch.clone();
            } else {
                if (rangeRow > overlappingSize)
                    transferPic = Vertical(transferPic, colsPatch, overlappingSize).clone();
                else
                    break;
            }
        }

        return transferPic;
}


//上一张
void MainWindow::on_pushButton_3_clicked()
{
    if(srcDirPathList.size()>=3)
        {
        index=qAbs(index+srcDirPathList.size()-1);
        int i = index%srcDirPathList.size();
        //qDebug("%d",i);
        QString srcDirPath = srcDirPathList.at(i);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;
        QImage images3=ImageCenter(image,ui->label_other);
        ui->label_other->setPixmap(QPixmap::fromImage(images3));
        ui->label_other->setAlignment(Qt::AlignCenter);
        //状态栏显示图片路径
        QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        label->setText(srcDirPath);

        QString src1 = srcDirPathList.at(qAbs(index+srcDirPathList.size()-1)%srcDirPathList.size());
        QImage image1(src1);
        QImage Image1 = ImageCenter(image1,ui->label_other_1);
        ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
        ui->label_other_1->setAlignment(Qt::AlignCenter);

        QString src2 = srcDirPathList.at(qAbs(index+srcDirPathList.size()-2)%srcDirPathList.size());
        QImage image2(src2);
        QImage Image2 = ImageCenter(image2,ui->label_other_3);
        ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
        ui->label_other_3->setAlignment(Qt::AlignCenter);
    }
        else if(srcDirPathList.size()==2){
            index=qAbs(index+srcDirPathList.size()-1);
            int i = index%srcDirPathList.size();
            //qDebug("%d",i);
            QString srcDirPath = srcDirPathList.at(i);
            QImage image(srcDirPath);
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            origin_path=srcDirPath;
            QImage images3=ImageCenter(image,ui->label_other);
            ui->label_other->setPixmap(QPixmap::fromImage(images3));
            ui->label_other->setAlignment(Qt::AlignCenter);
            //状态栏显示图片路径
            QLabel *label=ui->statusBar->findChild<QLabel *>("status");
            label->setText(srcDirPath);

            QString src1 = srcDirPathList.at(qAbs(index+srcDirPathList.size()-1)%srcDirPathList.size());
            QImage image1(src1);
            QImage Image1 = ImageCenter(image1,ui->label_other_1);
            ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
            ui->label_other_1->setAlignment(Qt::AlignCenter);
        }
}

//下一张
void MainWindow::on_pushButton_4_clicked()
{
    if(srcDirPathList.size()>=3)
        {
        index=qAbs(index+1);
        int i = index%srcDirPathList.size();
       // qDebug("%d",i);
        QString srcDirPath = srcDirPathList.at(i);
        QImage image(srcDirPath);
        QImage Image=ImageCenter(image,ui->label_show);
        ui->label_show->setPixmap(QPixmap::fromImage(Image));
        ui->label_show->setAlignment(Qt::AlignCenter);
        origin_path=srcDirPath;
        QImage images1=ImageCenter(image,ui->label_other);
        ui->label_other->setPixmap(QPixmap::fromImage(images1));
        ui->label_other->setAlignment(Qt::AlignCenter);
        //状态栏显示图片路径
        QLabel *label=ui->statusBar->findChild<QLabel *>("status");
        label->setText(srcDirPath);


        QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
        QImage image1(src1);
        QImage Image1 = ImageCenter(image1,ui->label_other_1);
        ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
        ui->label_other_1->setAlignment(Qt::AlignCenter);

        QString src2 = srcDirPathList.at((index+2)%srcDirPathList.size());
        QImage image2(src2);
        QImage Image2 = ImageCenter(image2,ui->label_other_3);
        ui->label_other_3->setPixmap(QPixmap::fromImage(Image2));
        ui->label_other_3->setAlignment(Qt::AlignCenter);
    }
        else if(srcDirPathList.size()==2){
            index=qAbs(index+1);
            int i = index%srcDirPathList.size();
            QString srcDirPath = srcDirPathList.at(i);
            QImage image(srcDirPath);
            QImage Image=ImageCenter(image,ui->label_show);
            ui->label_show->setPixmap(QPixmap::fromImage(Image));
            ui->label_show->setAlignment(Qt::AlignCenter);
            origin_path=srcDirPath;
            QImage images1=ImageCenter(image,ui->label_other);
            ui->label_other->setPixmap(QPixmap::fromImage(images1));
            ui->label_other->setAlignment(Qt::AlignCenter);
            //状态栏显示图片路径
            QLabel *label=ui->statusBar->findChild<QLabel *>("status");
            label->setText(srcDirPath);

            QString src1 = srcDirPathList.at((index+1)%srcDirPathList.size());
            QImage image1(src1);
            QImage Image1 = ImageCenter(image1,ui->label_other_1);
            ui->label_other_1->setPixmap(QPixmap::fromImage(Image1));
            ui->label_other_1->setAlignment(Qt::AlignCenter);
        }
}

//选择第一张图片
void MainWindow::on_pick_img_1_clicked()
{
    //打开文件对话框,选择图像文件
        QString fileName = QFileDialog::getOpenFileName(this,"选择图像文件","C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/images/","Image Files(*.png *.jpg)");
        if(!fileName.isEmpty())
        {
            //赋值Mat图像
            m_mat_1 = cv::imread(fileName.toStdString(),cv::IMREAD_COLOR);

            //读取图像文件
            QImage image(fileName);
            if(!image.isNull())
            {
                //赋值QImage图像
                m_image_1 = image;

                //将QImage转换为QPixmap以在QLabel上显示
                QPixmap pixmap = QPixmap::fromImage(m_image_1);
                ui->lb_image_1->setPixmap(pixmap.scaled(ui->lb_image_1->size(),Qt::KeepAspectRatio));   //保持比例
                ui->lb_image_1->setAlignment(Qt::AlignCenter);
            }
            else
            {
                //图像文件无效或无法读取
                QMessageBox::warning(this,"警告","图像文件打开失败!");
            }
        }
}

//选择第二张图片
void MainWindow::on_pick_img_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"选择图像文件","C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/images/","Image Files(*.png *.jpg)");
    if(!fileName.isEmpty())
    {
        m_mat_2 = cv::imread(fileName.toStdString(),cv::IMREAD_COLOR);
        QImage image(fileName);
        if(!image.isNull())
        {
            m_image_2 = image;
            QPixmap pixmap = QPixmap::fromImage(m_image_2);
            ui->lb_image_2->setPixmap(pixmap.scaled(ui->lb_image_2->size(),Qt::KeepAspectRatio));   //保持比例
            ui->lb_image_2->setAlignment(Qt::AlignCenter);
        }
        else
        {
            QMessageBox::warning(this,"警告","图像文件打开失败!");
        }
    }

}

//纹理迁移：寻找相同区域
typedef struct minimumDifferenceLocation {
    int row_num = 0;
    int col_num = 0;
    int pixelDifference = 0;
} MinimumDifferenceLocation;

bool compare_minimumDifferenceLocationArray(MinimumDifferenceLocation const struct1, MinimumDifferenceLocation const struct2) {
    return struct1.pixelDifference < struct2.pixelDifference;
}

Mat MainWindow::SearchForSimilarAreas(Mat currentPatch, Mat texturePic) {
    int patchCols = currentPatch.cols;
    int patchRows = currentPatch.rows;
    int textureCols = texturePic.cols;
    int textureRows = texturePic.rows;
    int rows_need_be_compared = textureRows - patchRows;
    int cols_need_be_compared = textureCols - patchCols;

    int **gray_scale_difference_sum;
    gray_scale_difference_sum = (int**)calloc(rows_need_be_compared, sizeof(int*));
    for (int i = 0; i < rows_need_be_compared; i++) {
        gray_scale_difference_sum[i] = (int*)calloc(cols_need_be_compared, sizeof(int));
    }

    for (int i = 0; i < rows_need_be_compared; i++) {
        for (int j = 0; j < cols_need_be_compared; j++)
            gray_scale_difference_sum[i][j] = 0;
    }

    Mat grayCurrentPatch, grayTexturePic;
    cvtColor(currentPatch, grayCurrentPatch, CV_BGR2GRAY, 0);
    cvtColor(texturePic, grayTexturePic, CV_BGR2GRAY, 0);

    int minPixelRowPositon = 0;
    int minPixelColPosition = 0;
    int minimumDifferenceSum = INT_MAX;
    int speedupValue = 3;
    MinimumDifferenceLocation* minimumDifferenceLocationArray = (MinimumDifferenceLocation*)calloc(cols_need_be_compared, sizeof(MinimumDifferenceLocation));
    for (int i = 0; i < rows_need_be_compared; i += speedupValue) {
        for (int j = 0; j < cols_need_be_compared; j++) {
            for (int k = 0; k < patchRows; k += speedupValue) {
                for (int l = 0; l < patchCols; l += speedupValue) {
                    gray_scale_difference_sum[i][j] += abs(grayCurrentPatch.at<uchar>(k, l) - grayTexturePic.at<uchar>(i + k, j + l));
                }
            }
            minimumDifferenceLocationArray[j].row_num = i;
            minimumDifferenceLocationArray[j].col_num = j;
            minimumDifferenceLocationArray[j].pixelDifference = gray_scale_difference_sum[i][j];
        }

        sort(minimumDifferenceLocationArray, minimumDifferenceLocationArray + cols_need_be_compared, compare_minimumDifferenceLocationArray);
        if (minimumDifferenceLocationArray[0].pixelDifference < minimumDifferenceSum) {
            minPixelRowPositon = minimumDifferenceLocationArray[0].row_num;
            minPixelColPosition = minimumDifferenceLocationArray[0].col_num;
            minimumDifferenceSum = minimumDifferenceLocationArray[0].pixelDifference;
        }
    }

    Mat theSimilarPath;
    theSimilarPath.create(patchRows, patchCols, CV_8UC3);
    for (int i = 0; i < patchRows; i++) {
        for (int j = 0; j < patchCols; j++) {
            Vec3b RGB = texturePic.at<Vec3b>(minPixelRowPositon + i, minPixelColPosition + j);
            theSimilarPath.at<Vec3b>(i, j) = RGB;
        }
    }

    return theSimilarPath;
}

//纹理迁移：水平处理
Mat MainWindow::Horizontal(Mat rock1_, Mat rock2_, int cutCols)
{
    Mat rock1 = rock1_;
        Mat rock2 = rock2_;
        int i, j;
        double** Assembly_actually = NULL;
        Mat kimCreate;
        Vec3b** leftPicture = NULL;
        Vec3b** rightPicture = NULL;
        int rows = rock1.rows;
        int cols = rock2.cols;

        leftPicture = (Vec3b**)calloc(cutCols, sizeof(Vec3b*));
        rightPicture = (Vec3b**)calloc(cutCols, sizeof(Vec3b*));

        for (i = 0; i < cutCols; i++)
        {
            leftPicture[i] = (Vec3b*)calloc(rows, sizeof(Vec3b));
            rightPicture[i] = (Vec3b*)calloc(rows, sizeof(Vec3b));
        }

        // initialize left picture array, from left to right boundary
        for (i = (rock1.cols - cutCols); i < rock1.cols; i++) {
            for (j = 0; j < rows; j++) {
                Vec3b rock1RGB = rock1.at<Vec3b>(j, i);
                leftPicture[i - (rock1.cols - cutCols)][j] = rock1RGB;
            }
        }

        // initialize right picture array, from left boundary to right
        for (i = 0; i < cutCols; i++) {
            for (j = 0; j < rows; j++) {
                Vec3b rock2RGB = rock2.at<Vec3b>(j, i);
                rightPicture[i][j] = rock2RGB;
            }
        }

        // initialize assembly line array, assembly line array contains each pixel weight in overlapping area
        Assembly_actually = (double**)calloc(cutCols, sizeof(double*));
        for (i = 0; i < cutCols; i++)
        {
            Assembly_actually[i] = (double*)calloc(rows, sizeof(double));
        }


        for (i = 0; i < cutCols; i++) {
            for (j = 0; j < rows; j++) {
                // d ==  ( (R1 - R2)^2 + (B1 - B2)^2 + (G1 - G2)^2 )
                Assembly_actually[i][j] = sqrt(pow((leftPicture[i][j].val[0] - rightPicture[i][j].val[0]), 2) + pow((leftPicture[i][j].val[1] - rightPicture[i][j].val[1]), 2) + pow((leftPicture[i][j].val[2] - rightPicture[i][j].val[2]), 2));
            }
        }

        double** f = NULL; // record the value of the shorest distance from starting point to current pixel
        int** l = NULL; // record assembly line path

        f = (double**)calloc(cutCols, sizeof(double*));
        for (i = 0; i < cutCols; i++) {
            f[i] = (double*)calloc(rows, sizeof(double));
        }
        l = (int**)calloc(cutCols, sizeof(int*));
        for (i = 0; i < cutCols; i++) {
            l[i] = (int*)calloc(rows, sizeof(int));
        }

        for (int i = 0; i < cutCols; i++)
        {
            l[i][0] = i; // initialize first step as itself
        }
        for (i = 0; i < cutCols; i++) {
            f[i][0] = Assembly_actually[i][0]; // initialize first step distance
        }

        for (i = 1; i < rows; i++) {
            for (j = 0; j < cutCols; j++) {
                // if there are three paths can be choosed
                if (j != 0 && j != (cutCols - 1)) {
                    // if middle path is the shortest path
                    if ((f[j][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i]) && (f[j][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i])) {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                    // if upper path is the shortest path
                    else if ((f[j - 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) && (f[j - 1][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i])) {
                        f[j][i] = f[j - 1][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j - 1;
                    }
                    // if lower path is the shortest path
                    else if ((f[j + 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) && (f[j + 1][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i])) {
                        f[j][i] = f[j + 1][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j + 1;
                    }
                    else {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                }
                // if there are two paths can be choosed
                else {
                    // left boundary
                    if (j == 0 && cutCols > 1) {
                        // if middle path is the shortest path
                        if (f[j][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i]) {
                            f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j;
                        }
                        // if upper path is the shortest path
                        else if (f[j + 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) {
                            f[j][i] = f[j + 1][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j + 1;
                        }
                        else {
                            f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j;
                        }

                    }
                    // right boundary
                    else if(j == (cutCols - 1) && cutCols > 1){
                        if (f[j][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i]) {
                            f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j;
                        }
                        else if (f[j - 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) {
                            f[j][i] = f[j - 1][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j - 1;
                        }
                        else {
                            f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                            l[j][i] = j;
                        }
                    }
                    // if there is only one path can be choose
                    else if (cutCols == 1) {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                    else {
                        exit(-1);
                    }
                }
            }
        }

        double minDistance = DBL_MAX;
        int position = -1;
        int lineNum = -1;
        int* boundaryPosition;
        boundaryPosition = (int*)calloc(rows, sizeof(int));

        // find the shortest path from left boundary to right boundary
        for (i = 0; i < cutCols; i++) {
            if (f[i][rows - 1] < minDistance) {
                minDistance = f[i][rows - 1];
                position = i;
            }
            else
                continue;
        }

        //lineNum = l[rows - 1][position];
        lineNum = l[position][rows - 1];
        for (i = rows - 1; i >= 0; i--) {
            lineNum = l[lineNum][i];
            boundaryPosition[i] = lineNum;
        }

        /*
        The codes below are not in line with human thinking habits,
        it's difficult to understand them
        */

        // create a picture , note the paras order --  rows, cols, type
        //kimCreate.create(rows, 2 * cols - cutCols, CV_8UC3);
        kimCreate.create(rows, rock1.cols + rock2.cols - cutCols, CV_8UC3);
        // assign the new picture from left to right and up to down
        for (i = 0; i < rows; i++) {
            for (j = 0; j < (rock1.cols + rock2.cols - cutCols); j++) {
                if (j <= boundaryPosition[i] + (rock1.cols - cutCols)) {  // at the left of the boundary edge
                    kimCreate.at<Vec3b>(i, j) = rock1.at<Vec3b>(i, j);
                }
                else {
                    kimCreate.at<Vec3b>(i, j) = rock2.at<Vec3b>(i, (j - (rock1.cols - cutCols)));
                }

            }
        }

        return kimCreate;
}

//纹理迁移：垂直处理
Mat MainWindow::Vertical(Mat rock1_, Mat rock2_, int cutRows)
{

    Mat rock1 = rock1_;
    Mat rock2 = rock2_;
    int i, j;
    double** Assembly_actually = NULL;
    Mat kimCreate;
    Vec3b** upPicture = NULL;
    Vec3b** downPicture = NULL;
    int rows = rock1.rows;
    int cols = rock2.cols;
    upPicture = (Vec3b**)calloc(cutRows, sizeof(Vec3b*));
    downPicture = (Vec3b**)calloc(cutRows, sizeof(Vec3b*));

for (i = 0; i < cutRows; i++)  // calloc space for pixels from up to down
    {
        upPicture[i] = (Vec3b*)calloc(cols, sizeof(Vec3b));
        downPicture[i] = (Vec3b*)calloc(cols, sizeof(Vec3b));
    }

    Assembly_actually = (double**)calloc(cutRows, sizeof(double*)); // assembly line array contains each pixel weight calculated by RGB
    for (i = 0; i < cutRows; i++)
    {
        Assembly_actually[i] = (double*)calloc(cols, sizeof(double));
    }

    for (i = (rock1.rows - cutRows); i < rock1.rows; i++) {     // initialize upper part overlap pixel
        for (j = 0; j < rock1.cols; j++) {
            Vec3b rock1RGB = rock1.at<Vec3b>(i, j); // get pixel from upper picture
            upPicture[i - (rock1.rows - cutRows)][j] = rock1RGB; // assign pixel to upPicture pixel array
        }
    }

    for (i = 0; i < cutRows; i++) {  // initialize lower part overlap pixel
        for (j = 0; j < rock2.cols; j++) {
            Vec3b rock2RGB = rock2.at<Vec3b>(i, j); // get pixel from lower picture
            downPicture[i][j] = rock2RGB; // assign pixel to downPicture pixel array
        }
    }

    for (i = 0; i < cutRows; i++) {
        for (j = 0; j < cols; j++) {
            // d ==  ( (R1 - R2)^2 + (B1 - B2)^2 + (G1 - G2)^2 )
            Assembly_actually[i][j] = sqrt(pow((upPicture[i][j].val[0] - downPicture[i][j].val[0]), 2) + pow((upPicture[i][j].val[1] - downPicture[i][j].val[1]), 2) + pow((upPicture[i][j].val[2] - downPicture[i][j].val[2]), 2));
        }
    }

    double** f = NULL; // record the value of the shorest distance from starting point to current pixel
    int** l = NULL; // record assembly line path

    f = (double**)calloc(cutRows, sizeof(double*));
    for (i = 0; i < cutRows; i++) {
        f[i] = (double*)calloc(cols, sizeof(double));
    }

    // record assembly line path
    l = (int**)calloc(cols, sizeof(int*));
    for (i = 0; i < cutRows; i++) {
        l[i] = (int*)calloc(cols, sizeof(int));
    }

    for (int i = 0; i < cutRows; i++)
    {
        l[i][0] = i; // initialize first step as itself
    }
    // initialize the pixel value of the first col
    for (i = 0; i < cutRows; i++) {
        f[i][0] = Assembly_actually[i][0]; // initialize first step distance
    }

    for (i = 1; i < cols; i++) {
        for (j = 0; j < cutRows; j++) {
            // if there are three path can be choosed
            if (j != 0 && j != (cutRows - 1)) {
                // if middle path is shortest
                if ((f[j][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i]) && (f[j][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i])) {
                    f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                    l[j][i] = j;
                }
                // if upper path is shortest
                else if ((f[j - 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) && (f[j - 1][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i])) {
                    f[j][i] = f[j - 1][i - 1] + Assembly_actually[j][i];
                    l[j][i] = j - 1;
                }
                // if lower path is shortest
                else if ((f[j + 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) && (f[j + 1][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i])) {
                    f[j][i] = f[j + 1][i - 1] + Assembly_actually[j][i];
                    l[j][i] = j + 1;
                }
                // if there are two path distance are equal, choose middle path by default.  I am a lazy man...
                else {
                    f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                    l[j][i] = j;
                }
            }
            // if there are two path can be choosed, the top line or the bottom line
            else {
                // the top line
                if (j == 0 && cutRows > 1) {
                    // if middle path is shortest
                    if (f[j][i - 1] + Assembly_actually[j][i] < f[j + 1][i - 1] + Assembly_actually[j][i]) {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                    // if lower path is shorest
                    else if (f[j + 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) {
                        f[j][i] = f[j + 1][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j + 1;
                    }
                    // if the middle and lower path are equal ,choose the middle path by default
                    else {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }

                }
                // the bottom line
                else if(j == (cutRows - 1) && cutRows > 1){
                    // if middle path is shortest
                    if (f[j][i - 1] + Assembly_actually[j][i] < f[j - 1][i - 1] + Assembly_actually[j][i]) {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                    // if upper path is shortest
                    else if (f[j - 1][i - 1] + Assembly_actually[j][i] < f[j][i - 1] + Assembly_actually[j][i]) {
                        f[j][i] = f[j - 1][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j - 1;
                    }
                    // if middle and upper path are euqal, choose the middle path by default
                    else {
                        f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                        l[j][i] = j;
                    }
                }
                // if there is only one path can be choose
                else if (cutRows == 1)
                {
                    f[j][i] = f[j][i - 1] + Assembly_actually[j][i];
                    l[j][i] = j;
                }
                else {
                    exit(-1);
                }
            }
        }
    }

    double minDistance = DBL_MAX;
    int position = -1;
    int lineNum = -1;
    int* boundaryPosition;
    boundaryPosition = (int*)calloc(cols, sizeof(int));  // record the shortest distance path

                                                         // find the shortest distance from left boundary to right boundary
    for (i = 0; i < cutRows; i++) {
        if (f[i][cols - 1] < minDistance) {
            minDistance = f[i][cols - 1];
            position = i;
        }
        else
            continue;
    }

    // starting from the shortest distance path last pixel
    lineNum = l[position][cols - 1];

    for (i = cols - 1; i >= 0; i--) {
        lineNum = l[lineNum][i];  // from which row to here, upper, middle or lower
        boundaryPosition[i] = lineNum; // record the shortest distance path each step
    }

    kimCreate.create(rock1.rows + rock2.rows - cutRows, cols, CV_8UC3);  // synthesis upper picture and lower picture for one picture
    for (i = 0; i < (rock1.rows + rock2.rows - cutRows); i++) {
        for (j = 0; j < cols; j++) {
            if (i <= boundaryPosition[j] + (rock1.rows - cutRows)) {  // above the boundary edge
                kimCreate.at<Vec3b>(i, j) = rock1.at<Vec3b>(i, j);
            }
            else {
                kimCreate.at<Vec3b>(i, j) = rock2.at<Vec3b>((i - (rock1.rows - cutRows)), j);
            }
        }
    }
    return kimCreate;
}

//等比例显示按钮
void MainWindow::on_img_keep_clicked()
{
    if(!m_showPixmap.isNull())
        {
            ui->lb_imageShow->setPixmap(m_showPixmap.scaled(ui->lb_imageShow->size(),Qt::KeepAspectRatio));   //保持比例
        }
        else
        {
            QMessageBox::warning(this,"警告","图像显示失败!");
        }
}

//填充显示按钮
void MainWindow::on_img_fill_clicked()
{
    if(!m_showPixmap.isNull())
    {
        ui->lb_imageShow->setPixmap(m_showPixmap.scaled(ui->lb_imageShow->size(),Qt::IgnoreAspectRatio));   //自由缩放
    }
    else
    {
        QMessageBox::warning(this,"警告","图像显示失败!");
    }
}

//opencv拼接按钮
void MainWindow::on_opencv_mosaic_clicked()
{
      setShowImage(1);
}

//opencv融合按钮
void MainWindow::on_opencv_merge_clicked()
{
      setShowImage(2);
}

//opencv纹理迁移按钮
void MainWindow::on_opencv_texture_clicked()
{
    // 创建进度对话框
    QProgressDialog progressDialog("Processing texture transfer...", "Cancel", 0, 100, this);
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(0);

    // 调用纹理迁移处理函数
    cv::Mat transferredMat = performTextureTransfer(m_mat_1, m_mat_2, 100, &progressDialog);

        // 将处理后的图像显示在界面上
        if (!transferredMat.empty()) {
            QImage newImage;
            if (transferredMat.channels() == 3) {
                cv::cvtColor(transferredMat, transferredMat, cv::COLOR_BGR2RGB);
                newImage = QImage((const unsigned char*)(transferredMat.data), transferredMat.cols, transferredMat.rows, transferredMat.step, QImage::Format_RGB888);
            } else if (transferredMat.channels() == 1) {
                newImage = QImage((const unsigned char*)(transferredMat.data), transferredMat.cols, transferredMat.rows, transferredMat.step, QImage::Format_Grayscale8);
            }

            // 保存处理后的图像
            QString savePath = QString("C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/test_texture_")+ QString::number(textureImageCounter++)+ ".png";
            cv::imwrite(savePath.toStdString(), transferredMat);

            // 在界面上显示处理后的图像
            QPixmap pixmap = QPixmap::fromImage(newImage.rgbSwapped());
            ui->lb_imageShow->setPixmap(pixmap.scaled(ui->lb_imageShow->size(), Qt::KeepAspectRatio));
            ui->lb_imageShow->setAlignment(Qt::AlignCenter);

            qDebug() << "Texture transfer completed and saved to:" << savePath;

            // 弹出保存成功的提示窗口
            QMessageBox::information(this, "提示", "纹理迁移处理完成并保存！", QMessageBox::Ok);
        } else {
            qDebug() << "Texture transfer failed.";
        }

        progressDialog.setValue(100);
}

//打开视频
void MainWindow::on_pushButton_open_clicked()
{
    QString video_path = QFileDialog::getOpenFileName(
        this, tr("选择视频"),
        "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/videos/",
        tr("Video (*.WMV *.mp4 *.rmvb *.flv)")
    );

    if (!video_path.isEmpty()) { // 使用 !video_path.isEmpty() 替代 nullptr 检查
        capture.open(video_path.toStdString());

        if (!capture.isOpened()) {
            QMessageBox::warning(nullptr, "提示", "打开视频失败！", QMessageBox::Yes);
            return;
        }

        QFileInfo fileInfo(video_path);
        videoName = fileInfo.baseName();
        frameCount = 0;

        ui->tabWidget->setCurrentIndex(1);
        ui->pushButton_6->setEnabled(true);

        long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
        double rate = capture.get(CAP_PROP_FRAME_COUNT);
        delay = 1000 / rate;

        // 确保定时器停止，避免重复启动
        timer.stop();

        // 初始化定时器并设置间隔
        timer.start(static_cast<int>(delay));

        // 连接定时器信号到更新帧的槽函数
        connect(&timer, &QTimer::timeout, this, &MainWindow::updateFrame);

        // 更新播放按钮样式
        ui->pushButton_6->setStyleSheet(
            "border-radius:32px;"
            "background-image: url(:/myImage/images/stop.png);"
            "border:none;"
        );

        isstart = true; // 假设 isstart 用于控制播放状态
    } else {
        // 处理用户取消选择文件的情况
    }
}

//暂停-播放按钮
void MainWindow::on_pushButton_6_clicked()
{
    if(isstart)
        {
            timer.stop();
            isstart=false;
            ui->pushButton_6->setStyleSheet("border-radius:32px;"
                                                     "background-image: url(:/myImage/images/start.png);border:none;") ;
        }else {
            updateSpeed(currentSpeedFactor); // 确保每次启动定时器时，使用当前的速度因子
            timer.start(timer.interval());  // 使用当前设置的间隔启动定时器
            isstart=true;
            ui->pushButton_6->setStyleSheet("border-radius:32px;"
                                                     "background-image: url(:/myImage/images/stop.png);border:none;") ;
    }
}

//更新视频帧
void MainWindow::updateFrame()
{
    if (capture.isOpened()) {
        capture >> currentFrame;
        if (!currentFrame.empty()) {
            // 将当前帧转换为 QImage
            QImage qimg = QImage((const unsigned char*)(currentFrame.data),
                                 currentFrame.cols, currentFrame.rows,
                                 currentFrame.step, QImage::Format_RGB888).rgbSwapped();

            QPixmap pixmap = QPixmap::fromImage(qimg);
            double scaleFactor = ui->horizontalSlider_suofang->value() / 100.0;
            int newWidth = qRound(pixmap.width() * scaleFactor);
            int newHeight = qRound(pixmap.height() * scaleFactor);
            pixmap = pixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            // 只有在缩放比例变化时才更新UI
            if (pixmap.size() != ui->label_13->pixmap()->size()) {
                ui->label_13->setPixmap(pixmap);
                ui->label_13->setAlignment(Qt::AlignCenter);
            }
        }
    }
}

//进度条随视频移动
void MainWindow::updatePosition(){
    long totalFrameNumber = capture.get(CAP_PROP_FRAME_COUNT);
    ui->VideohorizontalSlider_2->setMaximum(totalFrameNumber);
    long frame=capture.get(CAP_PROP_POS_FRAMES );
    ui->VideohorizontalSlider_2->setValue(frame);
}

//秒转分函数
QString MainWindow::stom(int s){
    QString m;
    if(s/60==0){
        m=QString::number (s%60);
    }else{
        m=QString::number (s/60)+":"+QString::number (s%60);
    }
    return m;
}

//timer触发函数
void MainWindow::onTimeout()
{
    Mat frame;
    if (!capture.read(frame)) {
        timer.stop();
        return;
    }

    // 根据 type 进行处理
    switch (type) {
        case 1:
            cvtColor(frame, frame, CV_BGR2GRAY);
            break;
        case 2:
            cvtColor(frame, frame, CV_BGR2GRAY);
            GaussianBlur(frame, frame, Size(3, 3), 0, 0, BORDER_DEFAULT);
            Canny(frame, frame, 100, 300, 3);
            break;
        case 3:
            GaussianBlur(frame, frame, Size(3, 3), 0, 0);
            break;
        case 4:
            cvtColor(frame, frame, CV_BGR2GRAY);
            threshold(frame, frame, 96, 255, THRESH_BINARY);
            break;
        case 0:
        default:
            break;
    }

    QImage image = MatToQImage(frame);

    // 检查是否需要更新界面
    if (!lastImage.isNull() && image != lastImage) {
        double scale = ui->horizontalSlider_suofang->value() / 100.0;
        QSize qs = ui->label_13->rect().size() * scale;

        ui->label_13->setPixmap(QPixmap::fromImage(image).scaled(qs, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        ui->label_13->setAlignment(Qt::AlignCenter);
        ui->label_13->repaint();
    }

    lastImage = image;  // 更新上一帧图像
}

//缩放
void MainWindow::on_horizontalSlider_suofang_valueChanged(int value)
{
    // 更新缩放值标签
    ui->label_suofangvalue->setText(QString::number(value / 100.0));

    // 读取当前显示的 QPixmap
    QPixmap pixmap = *ui->label_13->pixmap();
    if (!pixmap.isNull()) {
        // 计算新的缩放尺寸
        int newWidth = pixmap.width() * value / 100;
        int newHeight = pixmap.height() * value / 100;

        // 重新设置缩放后的 QPixmap
        QPixmap scaledPixmap = pixmap.scaled(newWidth, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_13->setPixmap(scaledPixmap);
        ui->label_13->setAlignment(Qt::AlignCenter); // 确保居中显示
    }
}

//进度条
void MainWindow::on_VideohorizontalSlider_2_valueChanged(int value)
{
    capture.set(CAP_PROP_POS_FRAMES, value);
}

//保存视频帧
void MainWindow::on_pushButton_saveframes_clicked()
{
    if (!currentFrame.empty())
        {
            // 构建保存路径
            QString savePath = "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res/";
            QString filePath = savePath + videoName + "_" + QString::number(frameCount++) + ".png";

            // 使用OpenCV保存帧到文件
            cv::imwrite(filePath.toStdString(), currentFrame);
            QMessageBox::information(this, tr("提示"), tr("帧保存成功！"));
        }
        else
        {
            QMessageBox::warning(this, tr("警告"), tr("当前没有视频帧可保存！"));
        }
}

//原图 type=0
void MainWindow::on_pushButton_origin_2_clicked()
{
    type = 0;
    if (timer.isActive()) {
        timer.stop();
    }
    timer.start(delay);
}

//灰度 type=1
void MainWindow::on_pushButton_grey_clicked()
{
    type = 1;
    if (timer.isActive()) {
            timer.stop();
        }
    timer.start(delay);
}

//边缘化 type=2
void MainWindow::on_pushButton_edge_clicked()
{
    type = 2;
    if (timer.isActive()) {
        timer.stop();
    }
    timer.start(delay);
}

//平滑 type=3
void MainWindow::on_pushButton_smooth_clicked()
{
    type = 3;
    if (timer.isActive()) {
        timer.stop();
    }
    timer.start(delay);
}

//二值化 type=4
void MainWindow::on_pushButton_binary_clicked()
{
    type = 4;
    if (timer.isActive()) {
        timer.stop();
    }
    timer.start(delay);
}

void MainWindow::onFrameReady(const QImage &image)
{
    double scale = ui->horizontalSlider_suofang->value() / 100.0;
    QSize qs = ui->label_13->rect().size() * scale;
    ui->label_13->setPixmap(QPixmap::fromImage(image).scaled(qs, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    ui->label_13->setAlignment(Qt::AlignCenter);
    ui->label_13->repaint();
}

// 更新倍速函数
void MainWindow::updateSpeed(double speedFactor)
{
    if (speedFactor > 0) {  // 避免速度因子为零
        currentSpeedFactor = speedFactor;
        int newDelay = static_cast<int>(delay / speedFactor);
        timer.setInterval(newDelay);
    }
}


void MainWindow::on_pushButton_play1_clicked()
{
    updateSpeed(0.5);
}


void MainWindow::on_pushButton_play1_2_clicked()
{
    updateSpeed(1.0);
}


void MainWindow::on_pushButton_play1_3_clicked()
{
    updateSpeed(1.5);
}


void MainWindow::on_pushButton_play1_4_clicked()
{
    updateSpeed(2.0);
}

//void MainWindow::on_pushButton_saveVedieo_clicked()
//{
//    // 检查是否有视频正在播放
//    if (!capture.isOpened()) {
//        QMessageBox::warning(this, "提示", "没有打开视频！", QMessageBox::Ok);
//        return;
//    }

//    // 构建保存视频的完整路径
//    QString savePath = "C:/Users/yll/Desktop/QT5-Image-and-video-processing-software/QT5-Image-and-video-processing-software-master/res_videos/" + videoName + ".avi";

//    // 获取视频帧率和尺寸
//    double frameRate = capture.get(CAP_PROP_FPS);
//    int frameWidth = capture.get(CAP_PROP_FRAME_WIDTH);
//    int frameHeight = capture.get(CAP_PROP_FRAME_HEIGHT);

//    // 创建 VideoWriter 对象
//    VideoWriter writer(savePath.toStdString(), VideoWriter::fourcc('M', 'J', 'P', 'G'), frameRate, Size(frameWidth, frameHeight));

//    if (!writer.isOpened()) {
//        QMessageBox::warning(this, "提示", "无法创建视频文件！", QMessageBox::Ok);
//        return;
//    }

//    // 开始保存视频
//    Mat frame;
//    while (capture.read(frame)) {
//        // 在这里进行视频处理，如果需要的话

//        // 写入帧到视频文件
//        writer.write(frame);

//        // 显示处理后的帧（可选）
//        QImage image = MatToQImage(frame);
//        double scale = ui->horizontalSlider_suofang->value() / 100.0;
//        QSize qs = ui->label_13->rect().size() * scale;

//        ui->label_13->setPixmap(QPixmap::fromImage(image).scaled(qs, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//        ui->label_13->setAlignment(Qt::AlignCenter);
//        ui->label_13->repaint();

//        // 停止按钮
//        if (!isstart) {
//            break;
//        }
//    }

//    // 释放 VideoWriter 和 VideoCapture
//    writer.release();
//    capture.release();

//    QMessageBox::information(this, "提示", "视频保存成功！", QMessageBox::Ok);
//}
