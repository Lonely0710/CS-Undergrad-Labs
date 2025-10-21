#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <QTimer>
#include <QImage>
#include <QMessageBox>
#include <QThread>
#include <QProgressDialog>
#include <QMath.h>

using namespace cv;

namespace Ui {
class MainWindow;
}

class MainWindow;

//视频类
class VideoProcessor : public QThread
{
    Q_OBJECT

public:
    explicit VideoProcessor(MainWindow *mainWindow, QObject *parent = nullptr);
    void run() override;
    void stop();
    void setType(int type);
    void setSpeedFactor(double factor);
    void stopMainWindowTimer();

private:
    MainWindow *mainWindow;
    VideoCapture capture;
    bool running;
    int type;
    double speedFactor;

signals:
    void frameReady(const QImage &image);
};

//主窗口类
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void showascii();
    QImage gray(QImage image);//灰度化

    QImage setRGB(QImage image,int value_r,int value_g,int value_b);//调整rgb
    QImage AdjustContrast(QImage image, int value);//调整对比度
    QImage ImageCenter(QImage  qimage,QLabel *qLabel);//调整图片比例
    QImage AdjustSaturation(QImage Img, int iSaturateValue);//调整饱和度

    QImage fuhe(QImage images);//边缘+原图变换
    QImage gamma(QImage image);//伽玛变换
    QImage * sharpen(QImage * origin);//锐化

    //多图处理函数
    QPixmap opencvMosaic(const cv::Mat &mat1, const cv::Mat &mat2);//水平拼接
    QPixmap opencvMerge(const cv::Mat &mat1,const cv::Mat &mat2);//图像融合
    void setShowImage(int index);//多图处理展示

    //纹理迁移主函数
    Mat performTextureTransfer(const cv::Mat& mat1, const cv::Mat& mat2, int cutCols, QProgressDialog* progressDialog);

    QStringList srcDirPathList;//图片list
    int index =0;//图片index
    int type =0;//视频操作类型
    
    QImage bianyuan(QImage image);//边缘检测
    
    //QImage & Mat类型转换
    QImage  MatToQImage(const cv::Mat& mat);
    Mat QImageToMat(QImage image);

    //视频处理函数
    QString stom(int s);

    void stopTimerFromThread();


private slots:

    void on_action_Open_triggered();

    void on_pushButton_clicked();

    void on_action_Save_triggered();

    void on_pushButton_gray_clicked();

    void on_pushButton_junzhi_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_origin_clicked();

    void on_pushButton_gamma_clicked();

    void on_pushButton_turnleft_clicked();

    void on_pushButton_turnright_clicked();

    void on_pushButton_turnleft_2_clicked();

    void on_pushButton_turnleft_3_clicked();

    void on_pushButton_bianyuan_clicked();

    void on_pushButton_bianyuan_2_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value1);

    void on_horizontalSlider_R_valueChanged(int value);

    void on_horizontalSlider_G_valueChanged(int value);

    void on_horizontalSlider_B_valueChanged(int value);

    void on_horizontalSlider_erzhi_valueChanged(int value);

    void on_horizontalSlider_duibi_valueChanged(int value);

    void on_horizontalSlider_baohe_valueChanged(int value);

    void on_pushButton_bianyuan_5_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pick_img_1_clicked();

    void on_img_keep_clicked();

    void on_img_fill_clicked();

    void on_opencv_mosaic_clicked();

    void on_opencv_merge_clicked();

    void on_pick_img_2_clicked();

    void on_opencv_texture_clicked();

    void on_pushButton_6_clicked();

    void updatePosition();

    void onTimeout();

    void on_horizontalSlider_suofang_valueChanged(int value);

    void on_VideohorizontalSlider_2_valueChanged(int value);

    void on_pushButton_open_clicked();

    void updateFrame();

    void on_pushButton_saveframes_clicked();

    void on_pushButton_grey_clicked();

    void on_pushButton_origin_2_clicked();

    void on_pushButton_smooth_clicked();

    void on_pushButton_binary_clicked();

    void on_pushButton_edge_clicked();

    void onFrameReady(const QImage &image);

    void on_pushButton_play1_clicked();

    void updateSpeed(double speedFactor);

    void on_pushButton_play1_2_clicked();

    void on_pushButton_play1_3_clicked();

    void on_pushButton_play1_4_clicked();

private:
    Ui::MainWindow *ui;
    bool language=true;
    bool isstart=false;
    int textureImageCounter = 0;  // 添加静态变量用于图像编号

    QString origin_path;// 原始图片路径

    //视频处理成员
    VideoProcessor *videoProcessor; // 视频处理对象
    VideoCapture capture; //视频捕获对象
    QString video_dir; //视频路径
    QString videoName; //视频名称


    QTimer timer;//视频播放的定时器
    int pace;//视频播放速率
    int delay = 30;//帧延迟
    cv::Mat currentFrame; // 存储当前帧
    int frameCount = 0;
    double currentSpeedFactor = 1.0;  // 初始倍速为 1.0x
    QImage lastImage;  // 用于保存上一帧图像
    QImage globalImage;

    Mat srcimage;//原图
    QMessageBox customMsgBox;

    //多图处理成员
    //QImage对象
    QImage m_image_1;
    QImage m_image_2;
    QPixmap m_showPixmap;

    //CV::Mat对象
    cv::Mat m_mat_1;
    cv::Mat m_mat_2;

    //纹理迁移函数
    Mat SearchForSimilarAreas(Mat currentPatch, Mat texturePic);
    Mat Horizontal(Mat rock1_, Mat rock2_, int cutCols);
    Mat Vertical(Mat rock1_, Mat rock2_, int cutRows);

    friend class VideoProcessor; // 使VideoProcessor类可以访问MainWindow的私有成员

};

#endif // MAINWINDOW_H
