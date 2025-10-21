/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action_New;
    QAction *action_Dock;
    QAction *action_Open;
    QAction *action_Save;
    QAction *action_Asave;
    QAction *action_About;
    QAction *action_Help;
    QAction *action_L;
    QAction *action_L_2;
    QAction *action_R;
    QAction *action_C;
    QAction *action_V;
    QAction *action_H;
    QAction *action_J;
    QAction *action_B;
    QAction *action_Y;
    QAction *action_G;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *widget;
    QLabel *label_show;
    QLabel *label_other;
    QPushButton *pushButton_save;
    QLabel *label_other_1;
    QLabel *label_other_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_3;
    QWidget *tab_2;
    QWidget *label_widget;
    QLabel *label_13;
    QPushButton *pushButton_6;
    QPushButton *pushButton_grey;
    QPushButton *pushButton_origin_2;
    QPushButton *pushButton_edge;
    QPushButton *pushButton_smooth;
    QPushButton *pushButton_binary;
    QSlider *horizontalSlider_suofang;
    QLabel *label_suofangvalue;
    QLabel *label_14;
    QPushButton *pushButton_open;
    QSlider *VideohorizontalSlider_2;
    QLabel *label_15;
    QPushButton *pushButton_saveframes;
    QPushButton *pushButton_play1;
    QPushButton *pushButton_play1_2;
    QPushButton *pushButton_play1_3;
    QPushButton *pushButton_play1_4;
    QWidget *tab_3;
    QLabel *lb_image_1;
    QLabel *lb_image_2;
    QPushButton *pick_img_1;
    QPushButton *pick_img_2;
    QLabel *lb_imageShow;
    QPushButton *opencv_mosaic;
    QPushButton *opencv_merge;
    QPushButton *img_keep;
    QPushButton *img_fill;
    QPushButton *opencv_texture;
    QDockWidget *dockWidget_3;
    QWidget *dockWidgetContents_6;
    QGridLayout *gridLayout_4;
    QSlider *horizontalSlider_R;
    QLabel *label_8;
    QSlider *horizontalSlider_B;
    QLabel *label_11;
    QLabel *label_7;
    QSlider *horizontalSlider_G;
    QDockWidget *dockWidget_2;
    QWidget *dockWidgetContents_5;
    QSlider *horizontalSlider;
    QLabel *label_light;
    QLabel *label_6;
    QLabel *label;
    QSlider *horizontalSlider_erzhi;
    QLabel *label_9;
    QSlider *horizontalSlider_duibi;
    QLabel *label_10;
    QSlider *horizontalSlider_baohe;
    QLabel *label_yuzhi;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QPushButton *pushButton;
    QPushButton *pushButton_gray;
    QPushButton *pushButton_junzhi;
    QPushButton *pushButton_origin;
    QPushButton *pushButton_gamma;
    QPushButton *pushButton_turnleft;
    QPushButton *pushButton_turnright;
    QPushButton *pushButton_turnleft_2;
    QPushButton *pushButton_turnleft_3;
    QPushButton *pushButton_bianyuan;
    QPushButton *pushButton_bianyuan_2;
    QLabel *label_12;
    QLabel *label_fuhe;
    QSlider *horizontalSlider_2;
    QPushButton *pushButton_bianyuan_5;
    QMenuBar *menuBar;
    QMenu *menu_F;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1662, 1116);
        MainWindow->setStyleSheet(QString::fromUtf8("QMainWindow{\n"
"background-image: url(:/myImage/mac/Home/Downloads/keju1.png);\n"
"\n"
"}\n"
"\n"
"QPushButton{\n"
"border-radius:10px;\n"
"/*color: rgb(255, 255, 255);*/\n"
"background-color: rgb(255, 255, 255);\n"
"border:2px solid rgb(0,0,0);\n"
"outline: none;\n"
"}\n"
"QPushButton:pressed{\n"
"background-color: rgb(204, 204, 215);\n"
"}\n"
"QPushButton:disabled{\n"
"background-color: rgb(167, 164, 170);\n"
"}\n"
"QSlider::handle:vertical{\n"
"color:black;\n"
"}\n"
"font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        action_New = new QAction(MainWindow);
        action_New->setObjectName(QString::fromUtf8("action_New"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/myImage/images/filenew.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_New->setIcon(icon);
        action_Dock = new QAction(MainWindow);
        action_Dock->setObjectName(QString::fromUtf8("action_Dock"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/myImage/images/zoom-in.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Dock->setIcon(icon1);
        action_Open = new QAction(MainWindow);
        action_Open->setObjectName(QString::fromUtf8("action_Open"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/myImage/images/picture.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Open->setIcon(icon2);
        action_Save = new QAction(MainWindow);
        action_Save->setObjectName(QString::fromUtf8("action_Save"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/myImage/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Save->setIcon(icon3);
        action_Asave = new QAction(MainWindow);
        action_Asave->setObjectName(QString::fromUtf8("action_Asave"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/myImage/images/filesaveas.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Asave->setIcon(icon4);
        action_About = new QAction(MainWindow);
        action_About->setObjectName(QString::fromUtf8("action_About"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/myImage/images/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_About->setIcon(icon5);
        action_Help = new QAction(MainWindow);
        action_Help->setObjectName(QString::fromUtf8("action_Help"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/myImage/images/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Help->setIcon(icon6);
        action_L = new QAction(MainWindow);
        action_L->setObjectName(QString::fromUtf8("action_L"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/myImage/images/zh-en.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_L->setIcon(icon7);
        action_L_2 = new QAction(MainWindow);
        action_L_2->setObjectName(QString::fromUtf8("action_L_2"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/myImage/images/left.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_L_2->setIcon(icon8);
        action_R = new QAction(MainWindow);
        action_R->setObjectName(QString::fromUtf8("action_R"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/myImage/images/right.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_R->setIcon(icon9);
        action_C = new QAction(MainWindow);
        action_C->setObjectName(QString::fromUtf8("action_C"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/myImage/images/center.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_C->setIcon(icon10);
        action_V = new QAction(MainWindow);
        action_V->setObjectName(QString::fromUtf8("action_V"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/myImage/images/video.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_V->setIcon(icon11);
        action_H = new QAction(MainWindow);
        action_H->setObjectName(QString::fromUtf8("action_H"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/myImage/images/gray.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_H->setIcon(icon12);
        action_J = new QAction(MainWindow);
        action_J->setObjectName(QString::fromUtf8("action_J"));
        QIcon icon13;
        icon13.addFile(QString::fromUtf8(":/myImage/images/junzhi.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_J->setIcon(icon13);
        action_B = new QAction(MainWindow);
        action_B->setObjectName(QString::fromUtf8("action_B"));
        QIcon icon14;
        icon14.addFile(QString::fromUtf8(":/myImage/images/bianyuan.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_B->setIcon(icon14);
        action_Y = new QAction(MainWindow);
        action_Y->setObjectName(QString::fromUtf8("action_Y"));
        QIcon icon15;
        icon15.addFile(QString::fromUtf8(":/myImage/images/fuhe.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Y->setIcon(icon15);
        action_G = new QAction(MainWindow);
        action_G->setObjectName(QString::fromUtf8("action_G"));
        QIcon icon16;
        icon16.addFile(QString::fromUtf8(":/myImage/images/gamma.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_G->setIcon(icon16);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(20, 10, 1051, 941));
        tabWidget->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius: 10px;\n"
"QTabBar::tab {\n"
"        background: #cccccc;\n"
"        font-family: \"\345\276\256\350\275\257\351\233\205\351\273\221\";  /* \350\256\276\347\275\256\345\255\227\344\275\223 */\n"
"        font-size: 12pt;           /* \350\256\276\347\275\256\345\255\227\344\275\223\345\244\247\345\260\217 */\n"
"        color: #000000;            /* \350\256\276\347\275\256\345\255\227\344\275\223\351\242\234\350\211\262 */\n"
"        padding: 10px;             /* \350\256\276\347\275\256\345\206\205\350\276\271\350\267\235 */\n"
"        border-top-left-radius: 10px;    /* \350\256\276\347\275\256\345\267\246\344\270\212\345\234\206\350\247\222 */\n"
"        border-top-right-radius: 10px;   /* \350\256\276\347\275\256\345\217\263\344\270\212\345\234\206\350\247\222 */\n"
"    }"));
        tabWidget->setElideMode(Qt::ElideLeft);
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        widget = new QWidget(tab);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(9, -1, 1061, 851));
        widget->setStyleSheet(QString::fromUtf8("font: 75 10pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_show = new QLabel(widget);
        label_show->setObjectName(QString::fromUtf8("label_show"));
        label_show->setGeometry(QRect(140, 30, 781, 531));
        label_show->setMinimumSize(QSize(371, 0));
        label_show->setStyleSheet(QString::fromUtf8("border:5px solid black;"));
        label_show->setFrameShape(QFrame::WinPanel);
        label_show->setLineWidth(1);
        label_other = new QLabel(widget);
        label_other->setObjectName(QString::fromUtf8("label_other"));
        label_other->setGeometry(QRect(180, 580, 181, 121));
        label_other->setStyleSheet(QString::fromUtf8("border:5px solid red;"));
        label_other->setFrameShape(QFrame::NoFrame);
        label_other->setFrameShadow(QFrame::Plain);
        pushButton_save = new QPushButton(widget);
        pushButton_save->setObjectName(QString::fromUtf8("pushButton_save"));
        pushButton_save->setGeometry(QRect(440, 730, 161, 41));
        pushButton_save->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_other_1 = new QLabel(widget);
        label_other_1->setObjectName(QString::fromUtf8("label_other_1"));
        label_other_1->setGeometry(QRect(430, 580, 181, 121));
        label_other_1->setStyleSheet(QString::fromUtf8("border:5px solid black;"));
        label_other_1->setFrameShape(QFrame::NoFrame);
        label_other_1->setFrameShadow(QFrame::Plain);
        label_other_3 = new QLabel(widget);
        label_other_3->setObjectName(QString::fromUtf8("label_other_3"));
        label_other_3->setGeometry(QRect(670, 580, 181, 121));
        label_other_3->setStyleSheet(QString::fromUtf8("border:5px solid black;"));
        label_other_3->setFrameShape(QFrame::NoFrame);
        label_other_3->setFrameShadow(QFrame::Plain);
        pushButton_4 = new QPushButton(widget);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(970, 280, 71, 71));
        pushButton_4->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-color: rgb(255, 255, 255);\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/next1.png);"));
        pushButton_4->setFlat(false);
        pushButton_3 = new QPushButton(widget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(20, 280, 71, 71));
        pushButton_3->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-color: rgb(255, 255, 255);\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/prev1.png);"));
        pushButton_3->setFlat(false);
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_widget = new QWidget(tab_2);
        label_widget->setObjectName(QString::fromUtf8("label_widget"));
        label_widget->setGeometry(QRect(0, 0, 1031, 761));
        label_13 = new QLabel(label_widget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setGeometry(QRect(60, 30, 961, 481));
        label_13->setStyleSheet(QString::fromUtf8("border:5px solid black;\n"
"border-radius: 10px;"));
        label_13->setFrameShape(QFrame::WinPanel);
        label_13->setFrameShadow(QFrame::Plain);
        pushButton_6 = new QPushButton(label_widget);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setEnabled(false);
        pushButton_6->setGeometry(QRect(550, 590, 64, 64));
        pushButton_6->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-image: url(:/myImage/images/start.png);\n"
"border:none;"));
        pushButton_grey = new QPushButton(label_widget);
        pushButton_grey->setObjectName(QString::fromUtf8("pushButton_grey"));
        pushButton_grey->setGeometry(QRect(870, 580, 131, 51));
        pushButton_grey->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_origin_2 = new QPushButton(label_widget);
        pushButton_origin_2->setObjectName(QString::fromUtf8("pushButton_origin_2"));
        pushButton_origin_2->setGeometry(QRect(180, 580, 131, 51));
        pushButton_origin_2->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_edge = new QPushButton(label_widget);
        pushButton_edge->setObjectName(QString::fromUtf8("pushButton_edge"));
        pushButton_edge->setGeometry(QRect(690, 650, 131, 51));
        pushButton_edge->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_smooth = new QPushButton(label_widget);
        pushButton_smooth->setObjectName(QString::fromUtf8("pushButton_smooth"));
        pushButton_smooth->setGeometry(QRect(870, 650, 131, 51));
        pushButton_smooth->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_binary = new QPushButton(label_widget);
        pushButton_binary->setObjectName(QString::fromUtf8("pushButton_binary"));
        pushButton_binary->setGeometry(QRect(690, 580, 131, 51));
        pushButton_binary->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalSlider_suofang = new QSlider(label_widget);
        horizontalSlider_suofang->setObjectName(QString::fromUtf8("horizontalSlider_suofang"));
        horizontalSlider_suofang->setGeometry(QRect(270, 660, 221, 31));
        horizontalSlider_suofang->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider_suofang->setMaximum(200);
        horizontalSlider_suofang->setValue(100);
        horizontalSlider_suofang->setOrientation(Qt::Horizontal);
        label_suofangvalue = new QLabel(label_widget);
        label_suofangvalue->setObjectName(QString::fromUtf8("label_suofangvalue"));
        label_suofangvalue->setGeometry(QRect(180, 655, 61, 31));
        label_suofangvalue->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-color: rgb(255, 255, 255);\n"
"alignment: center;\n"
"border-radius: 10px;"));
        label_14 = new QLabel(label_widget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(10, 655, 121, 31));
        label_14->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 10px;"));
        pushButton_open = new QPushButton(label_widget);
        pushButton_open->setObjectName(QString::fromUtf8("pushButton_open"));
        pushButton_open->setGeometry(QRect(30, 580, 131, 51));
        pushButton_open->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        VideohorizontalSlider_2 = new QSlider(label_widget);
        VideohorizontalSlider_2->setObjectName(QString::fromUtf8("VideohorizontalSlider_2"));
        VideohorizontalSlider_2->setGeometry(QRect(40, 530, 981, 21));
        VideohorizontalSlider_2->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:black;\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 25px;\n"
"        width:25px;\n"
"        border-image: url(:/myImage/images/handle.png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        VideohorizontalSlider_2->setMaximum(1000);
        VideohorizontalSlider_2->setOrientation(Qt::Horizontal);
        label_15 = new QLabel(label_widget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(10, 700, 121, 31));
        label_15->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"background-color: rgb(255, 255, 255);\n"
"border-radius: 10px;"));
        pushButton_saveframes = new QPushButton(label_widget);
        pushButton_saveframes->setObjectName(QString::fromUtf8("pushButton_saveframes"));
        pushButton_saveframes->setGeometry(QRect(330, 580, 131, 51));
        pushButton_saveframes->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_play1 = new QPushButton(label_widget);
        pushButton_play1->setObjectName(QString::fromUtf8("pushButton_play1"));
        pushButton_play1->setGeometry(QRect(180, 700, 71, 31));
        pushButton_play1->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_play1_2 = new QPushButton(label_widget);
        pushButton_play1_2->setObjectName(QString::fromUtf8("pushButton_play1_2"));
        pushButton_play1_2->setGeometry(QRect(260, 700, 71, 31));
        pushButton_play1_2->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_play1_3 = new QPushButton(label_widget);
        pushButton_play1_3->setObjectName(QString::fromUtf8("pushButton_play1_3"));
        pushButton_play1_3->setGeometry(QRect(340, 700, 71, 31));
        pushButton_play1_3->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pushButton_play1_4 = new QPushButton(label_widget);
        pushButton_play1_4->setObjectName(QString::fromUtf8("pushButton_play1_4"));
        pushButton_play1_4->setGeometry(QRect(420, 700, 71, 31));
        pushButton_play1_4->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        tabWidget->addTab(tab_2, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QString::fromUtf8("tab_3"));
        lb_image_1 = new QLabel(tab_3);
        lb_image_1->setObjectName(QString::fromUtf8("lb_image_1"));
        lb_image_1->setGeometry(QRect(60, 20, 381, 301));
        lb_image_1->setMinimumSize(QSize(371, 0));
        lb_image_1->setStyleSheet(QString::fromUtf8("border:5px solid black;\n"
"border-radius: 10px;"));
        lb_image_1->setFrameShape(QFrame::WinPanel);
        lb_image_1->setLineWidth(1);
        lb_image_2 = new QLabel(tab_3);
        lb_image_2->setObjectName(QString::fromUtf8("lb_image_2"));
        lb_image_2->setGeometry(QRect(570, 20, 381, 301));
        lb_image_2->setMinimumSize(QSize(371, 0));
        lb_image_2->setStyleSheet(QString::fromUtf8("border:5px solid black;\n"
"border-radius: 10px;"));
        lb_image_2->setFrameShape(QFrame::WinPanel);
        lb_image_2->setLineWidth(1);
        pick_img_1 = new QPushButton(tab_3);
        pick_img_1->setObjectName(QString::fromUtf8("pick_img_1"));
        pick_img_1->setGeometry(QRect(170, 340, 161, 51));
        pick_img_1->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        pick_img_2 = new QPushButton(tab_3);
        pick_img_2->setObjectName(QString::fromUtf8("pick_img_2"));
        pick_img_2->setGeometry(QRect(690, 340, 171, 51));
        pick_img_2->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        lb_imageShow = new QLabel(tab_3);
        lb_imageShow->setObjectName(QString::fromUtf8("lb_imageShow"));
        lb_imageShow->setGeometry(QRect(60, 400, 611, 361));
        lb_imageShow->setMinimumSize(QSize(371, 0));
        lb_imageShow->setStyleSheet(QString::fromUtf8("border:5px solid black;\n"
"border-radius: 10px;"));
        lb_imageShow->setFrameShape(QFrame::WinPanel);
        lb_imageShow->setLineWidth(1);
        opencv_mosaic = new QPushButton(tab_3);
        opencv_mosaic->setObjectName(QString::fromUtf8("opencv_mosaic"));
        opencv_mosaic->setGeometry(QRect(770, 400, 201, 51));
        opencv_mosaic->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        opencv_merge = new QPushButton(tab_3);
        opencv_merge->setObjectName(QString::fromUtf8("opencv_merge"));
        opencv_merge->setGeometry(QRect(770, 480, 201, 51));
        opencv_merge->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        img_keep = new QPushButton(tab_3);
        img_keep->setObjectName(QString::fromUtf8("img_keep"));
        img_keep->setGeometry(QRect(770, 560, 201, 51));
        img_keep->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        img_fill = new QPushButton(tab_3);
        img_fill->setObjectName(QString::fromUtf8("img_fill"));
        img_fill->setGeometry(QRect(770, 640, 201, 51));
        img_fill->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        opencv_texture = new QPushButton(tab_3);
        opencv_texture->setObjectName(QString::fromUtf8("opencv_texture"));
        opencv_texture->setGeometry(QRect(770, 720, 201, 51));
        opencv_texture->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        tabWidget->addTab(tab_3, QString());
        dockWidget_3 = new QDockWidget(centralWidget);
        dockWidget_3->setObjectName(QString::fromUtf8("dockWidget_3"));
        dockWidget_3->setGeometry(QRect(1120, 410, 251, 381));
        dockWidget_3->setMinimumSize(QSize(172, 166));
        dockWidget_3->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 100);\n"
"font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius: 10px;"));
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QString::fromUtf8("dockWidgetContents_6"));
        gridLayout_4 = new QGridLayout(dockWidgetContents_6);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSlider_R = new QSlider(dockWidgetContents_6);
        horizontalSlider_R->setObjectName(QString::fromUtf8("horizontalSlider_R"));
        horizontalSlider_R->setContextMenuPolicy(Qt::DefaultContextMenu);
        horizontalSlider_R->setToolTipDuration(1);
        horizontalSlider_R->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: red;\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:white;\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
"\n"
""));
        horizontalSlider_R->setMaximum(255);
        horizontalSlider_R->setOrientation(Qt::Horizontal);
        horizontalSlider_R->setTickPosition(QSlider::NoTicks);

        gridLayout_4->addWidget(horizontalSlider_R, 1, 0, 1, 1);

        label_8 = new QLabel(dockWidgetContents_6);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_4->addWidget(label_8, 2, 0, 1, 1);

        horizontalSlider_B = new QSlider(dockWidgetContents_6);
        horizontalSlider_B->setObjectName(QString::fromUtf8("horizontalSlider_B"));
        horizontalSlider_B->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: blue;\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:white;\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
"\n"
""));
        horizontalSlider_B->setMaximum(255);
        horizontalSlider_B->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_B, 5, 0, 1, 1);

        label_11 = new QLabel(dockWidgetContents_6);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_4->addWidget(label_11, 4, 0, 1, 1);

        label_7 = new QLabel(dockWidgetContents_6);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_4->addWidget(label_7, 0, 0, 1, 1);

        horizontalSlider_G = new QSlider(dockWidgetContents_6);
        horizontalSlider_G->setObjectName(QString::fromUtf8("horizontalSlider_G"));
        horizontalSlider_G->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: green;\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:white;\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
"\n"
""));
        horizontalSlider_G->setMaximum(255);
        horizontalSlider_G->setOrientation(Qt::Horizontal);

        gridLayout_4->addWidget(horizontalSlider_G, 3, 0, 1, 1);

        dockWidget_3->setWidget(dockWidgetContents_6);
        dockWidget_2 = new QDockWidget(centralWidget);
        dockWidget_2->setObjectName(QString::fromUtf8("dockWidget_2"));
        dockWidget_2->setGeometry(QRect(1120, 0, 251, 401));
        dockWidget_2->setMinimumSize(QSize(187, 247));
        dockWidget_2->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 100);\n"
"font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius: 10px;"));
        dockWidget_2->setFloating(false);
        dockWidget_2->setAllowedAreas(Qt::AllDockWidgetAreas);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QString::fromUtf8("dockWidgetContents_5"));
        horizontalSlider = new QSlider(dockWidgetContents_5);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(9, 76, 231, 16));
        horizontalSlider->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider->setMinimum(-300);
        horizontalSlider->setMaximum(300);
        horizontalSlider->setOrientation(Qt::Horizontal);
        label_light = new QLabel(dockWidgetContents_5);
        label_light->setObjectName(QString::fromUtf8("label_light"));
        label_light->setGeometry(QRect(150, 10, 51, 61));
        label_light->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label_6 = new QLabel(dockWidgetContents_5);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(9, 9, 141, 61));
        label_6->setMinimumSize(QSize(141, 0));
        label_6->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        label = new QLabel(dockWidgetContents_5);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(9, 97, 141, 61));
        label->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalSlider_erzhi = new QSlider(dockWidgetContents_5);
        horizontalSlider_erzhi->setObjectName(QString::fromUtf8("horizontalSlider_erzhi"));
        horizontalSlider_erzhi->setGeometry(QRect(9, 165, 231, 16));
        horizontalSlider_erzhi->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider_erzhi->setMaximum(255);
        horizontalSlider_erzhi->setValue(128);
        horizontalSlider_erzhi->setOrientation(Qt::Horizontal);
        label_9 = new QLabel(dockWidgetContents_5);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(9, 186, 181, 61));
        label_9->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalSlider_duibi = new QSlider(dockWidgetContents_5);
        horizontalSlider_duibi->setObjectName(QString::fromUtf8("horizontalSlider_duibi"));
        horizontalSlider_duibi->setGeometry(QRect(9, 253, 231, 16));
        horizontalSlider_duibi->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider_duibi->setMaximum(255);
        horizontalSlider_duibi->setOrientation(Qt::Horizontal);
        label_10 = new QLabel(dockWidgetContents_5);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(9, 274, 181, 61));
        label_10->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalSlider_baohe = new QSlider(dockWidgetContents_5);
        horizontalSlider_baohe->setObjectName(QString::fromUtf8("horizontalSlider_baohe"));
        horizontalSlider_baohe->setGeometry(QRect(9, 330, 231, 16));
        horizontalSlider_baohe->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider_baohe->setOrientation(Qt::Horizontal);
        label_yuzhi = new QLabel(dockWidgetContents_5);
        label_yuzhi->setObjectName(QString::fromUtf8("label_yuzhi"));
        label_yuzhi->setGeometry(QRect(150, 97, 51, 61));
        label_yuzhi->setStyleSheet(QString::fromUtf8("font: 75 9pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        dockWidget_2->setWidget(dockWidgetContents_5);
        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        dockWidget = new QDockWidget(MainWindow);
        dockWidget->setObjectName(QString::fromUtf8("dockWidget"));
        dockWidget->setMinimumSize(QSize(172, 378));
        dockWidget->setStyleSheet(QString::fromUtf8("background-color: rgba(255, 255, 255, 100);\n"
"font: 100 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";\n"
"border-radius: 10px;"));
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        pushButton = new QPushButton(dockWidgetContents);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(20, 17, 131, 41));
        pushButton->setStyleSheet(QString::fromUtf8(""));
        pushButton_gray = new QPushButton(dockWidgetContents);
        pushButton_gray->setObjectName(QString::fromUtf8("pushButton_gray"));
        pushButton_gray->setGeometry(QRect(20, 120, 131, 41));
        pushButton_junzhi = new QPushButton(dockWidgetContents);
        pushButton_junzhi->setObjectName(QString::fromUtf8("pushButton_junzhi"));
        pushButton_junzhi->setGeometry(QRect(20, 170, 131, 41));
        pushButton_origin = new QPushButton(dockWidgetContents);
        pushButton_origin->setObjectName(QString::fromUtf8("pushButton_origin"));
        pushButton_origin->setGeometry(QRect(20, 70, 131, 41));
        pushButton_gamma = new QPushButton(dockWidgetContents);
        pushButton_gamma->setObjectName(QString::fromUtf8("pushButton_gamma"));
        pushButton_gamma->setGeometry(QRect(20, 270, 131, 41));
        pushButton_turnleft = new QPushButton(dockWidgetContents);
        pushButton_turnleft->setObjectName(QString::fromUtf8("pushButton_turnleft"));
        pushButton_turnleft->setGeometry(QRect(20, 540, 64, 64));
        pushButton_turnleft->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/turn-left.png);"));
        pushButton_turnleft->setFlat(false);
        pushButton_turnright = new QPushButton(dockWidgetContents);
        pushButton_turnright->setObjectName(QString::fromUtf8("pushButton_turnright"));
        pushButton_turnright->setGeometry(QRect(100, 540, 64, 64));
        pushButton_turnright->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/turn-right.png);"));
        pushButton_turnleft_2 = new QPushButton(dockWidgetContents);
        pushButton_turnleft_2->setObjectName(QString::fromUtf8("pushButton_turnleft_2"));
        pushButton_turnleft_2->setGeometry(QRect(20, 620, 64, 64));
        pushButton_turnleft_2->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/shuiping.png);"));
        pushButton_turnleft_3 = new QPushButton(dockWidgetContents);
        pushButton_turnleft_3->setObjectName(QString::fromUtf8("pushButton_turnleft_3"));
        pushButton_turnleft_3->setGeometry(QRect(100, 620, 64, 64));
        pushButton_turnleft_3->setStyleSheet(QString::fromUtf8("border-radius:32px;\n"
"background-repeat:no-repeat;\n"
"background-position:center;\n"
"background-image: url(:/myImage/images/chuizhi.png);"));
        pushButton_bianyuan = new QPushButton(dockWidgetContents);
        pushButton_bianyuan->setObjectName(QString::fromUtf8("pushButton_bianyuan"));
        pushButton_bianyuan->setGeometry(QRect(20, 220, 131, 41));
        pushButton_bianyuan_2 = new QPushButton(dockWidgetContents);
        pushButton_bianyuan_2->setObjectName(QString::fromUtf8("pushButton_bianyuan_2"));
        pushButton_bianyuan_2->setGeometry(QRect(20, 320, 131, 41));
        label_12 = new QLabel(dockWidgetContents);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 470, 61, 51));
        label_fuhe = new QLabel(dockWidgetContents);
        label_fuhe->setObjectName(QString::fromUtf8("label_fuhe"));
        label_fuhe->setGeometry(QRect(70, 470, 101, 51));
        label_fuhe->setStyleSheet(QString::fromUtf8("font: 75 8pt \"\345\276\256\350\275\257\351\233\205\351\273\221\";"));
        horizontalSlider_2 = new QSlider(dockWidgetContents);
        horizontalSlider_2->setObjectName(QString::fromUtf8("horizontalSlider_2"));
        horizontalSlider_2->setGeometry(QRect(20, 420, 141, 31));
        horizontalSlider_2->setStyleSheet(QString::fromUtf8("QSlider::add-page:Horizontal\n"
"     {     \n"
"        background-color: rgb(223, 223, 223);\n"
"        height:4px;\n"
"     }\n"
"     QSlider::sub-page:Horizontal \n"
"    {\n"
"        background-color:rgb(0, 170, 255);\n"
"        height:4px;\n"
"     }\n"
"    QSlider::groove:Horizontal \n"
"    {\n"
"        background:transparent;\n"
"        height:10px;\n"
"		border-radius: 5px;\n"
"    }\n"
"    QSlider::handle:Horizontal \n"
"    {\n"
"        height: 10px;\n"
"        width:25px;\n"
"	border-image: url(:/myImage/mac/Home/Downloads/24gf-circle-copy (1).png);\n"
"        margin: -8 0px;\n"
"    }\n"
""));
        horizontalSlider_2->setMaximum(100);
        horizontalSlider_2->setSingleStep(1);
        horizontalSlider_2->setValue(50);
        horizontalSlider_2->setOrientation(Qt::Horizontal);
        pushButton_bianyuan_5 = new QPushButton(dockWidgetContents);
        pushButton_bianyuan_5->setObjectName(QString::fromUtf8("pushButton_bianyuan_5"));
        pushButton_bianyuan_5->setGeometry(QRect(20, 370, 131, 41));
        dockWidget->setWidget(dockWidgetContents);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1662, 21));
        menu_F = new QMenu(menuBar);
        menu_F->setObjectName(QString::fromUtf8("menu_F"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menu_F->menuAction());
        menu_F->addAction(action_Open);
        menu_F->addAction(action_Save);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        action_New->setText(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\226\207\344\273\266(&N)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_New->setToolTip(QApplication::translate("MainWindow", "\346\226\260\345\273\272\346\226\207\344\273\266", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_New->setShortcut(QApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_NO_SHORTCUT
        action_Dock->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272Dock(&D)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Dock->setToolTip(QApplication::translate("MainWindow", "\346\230\276\347\244\272Dock", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_Dock->setShortcut(QApplication::translate("MainWindow", "Ctrl+D", nullptr));
#endif // QT_NO_SHORTCUT
        action_Open->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\347\211\207(&O)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Open->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_Open->setShortcut(QApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        action_Save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\233\276\347\211\207(&S)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Save->setToolTip(QApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_Save->setShortcut(QApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_NO_SHORTCUT
        action_Asave->setText(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272(&A)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Asave->setToolTip(QApplication::translate("MainWindow", "\345\217\246\345\255\230\344\270\272", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_Asave->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        action_About->setText(QApplication::translate("MainWindow", "\345\205\263\344\272\216(&A)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_About->setToolTip(QApplication::translate("MainWindow", "\345\205\263\344\272\216", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_About->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        action_Help->setText(QApplication::translate("MainWindow", "\345\270\256\345\212\251(&H)", nullptr));
#ifndef QT_NO_TOOLTIP
        action_Help->setToolTip(QApplication::translate("MainWindow", "\345\270\256\345\212\251", nullptr));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_SHORTCUT
        action_Help->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_NO_SHORTCUT
        action_L->setText(QApplication::translate("MainWindow", "\345\210\207\346\215\242\344\270\255\350\213\261\346\226\207(&L)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_L->setShortcut(QApplication::translate("MainWindow", "Ctrl+L", nullptr));
#endif // QT_NO_SHORTCUT
        action_L_2->setText(QApplication::translate("MainWindow", "\345\267\246\345\257\271\351\275\220(&L)", nullptr));
        action_R->setText(QApplication::translate("MainWindow", "\345\217\263\345\257\271\351\275\220(&R)", nullptr));
        action_C->setText(QApplication::translate("MainWindow", "\345\261\205\344\270\255(&C)", nullptr));
        action_V->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\350\247\206\351\242\221(&V)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_V->setShortcut(QApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_NO_SHORTCUT
        action_H->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\345\214\226(&H)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_H->setShortcut(QApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_NO_SHORTCUT
        action_J->setText(QApplication::translate("MainWindow", "\345\235\207\345\200\274\346\273\244\346\263\242(&J)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_J->setShortcut(QApplication::translate("MainWindow", "Ctrl+J", nullptr));
#endif // QT_NO_SHORTCUT
        action_B->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230\346\243\200\346\265\213(&B)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_B->setShortcut(QApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_NO_SHORTCUT
        action_Y->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230+\345\216\237\345\233\276(&Y)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_Y->setShortcut(QApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_NO_SHORTCUT
        action_G->setText(QApplication::translate("MainWindow", "\344\274\275\351\251\254\345\217\230\346\215\242(&G)", nullptr));
#ifndef QT_NO_SHORTCUT
        action_G->setShortcut(QApplication::translate("MainWindow", "Ctrl+G", nullptr));
#endif // QT_NO_SHORTCUT
        label_show->setText(QString());
        label_other->setText(QString());
        pushButton_save->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\345\233\276\347\211\207", nullptr));
        label_other_1->setText(QString());
        label_other_3->setText(QString());
        pushButton_4->setText(QString());
        pushButton_3->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "\345\233\276\345\203\217\345\244\204\347\220\206", nullptr));
        label_13->setText(QString());
        pushButton_6->setText(QString());
        pushButton_grey->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\345\214\226", nullptr));
        pushButton_origin_2->setText(QApplication::translate("MainWindow", "\345\216\237\347\224\273", nullptr));
        pushButton_edge->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230\346\243\200\346\265\213", nullptr));
        pushButton_smooth->setText(QApplication::translate("MainWindow", "\345\271\263\346\273\221", nullptr));
        pushButton_binary->setText(QApplication::translate("MainWindow", "\344\272\214\345\200\274\345\214\226", nullptr));
        label_suofangvalue->setText(QApplication::translate("MainWindow", "1", nullptr));
        label_14->setText(QApplication::translate("MainWindow", " \347\274\251\346\224\276\357\274\232\303\227", nullptr));
        pushButton_open->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\350\247\206\351\242\221", nullptr));
        label_15->setText(QApplication::translate("MainWindow", " \345\200\215\351\200\237\350\256\276\347\275\256", nullptr));
        pushButton_saveframes->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230\350\247\206\351\242\221\345\270\247", nullptr));
        pushButton_play1->setText(QApplication::translate("MainWindow", "0.5\303\227", nullptr));
        pushButton_play1_2->setText(QApplication::translate("MainWindow", "1.0\303\227", nullptr));
        pushButton_play1_3->setText(QApplication::translate("MainWindow", "1.5\303\227", nullptr));
        pushButton_play1_4->setText(QApplication::translate("MainWindow", "2.0\303\227", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "\350\247\206\351\242\221\345\244\204\347\220\206", nullptr));
        lb_image_1->setText(QString());
        lb_image_2->setText(QString());
        pick_img_1->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\2071", nullptr));
        pick_img_2->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\2072", nullptr));
        lb_imageShow->setText(QString());
        opencv_mosaic->setText(QApplication::translate("MainWindow", "OpenCV\346\213\274\346\216\245", nullptr));
        opencv_merge->setText(QApplication::translate("MainWindow", "OpenCV\350\236\215\345\220\210", nullptr));
        img_keep->setText(QApplication::translate("MainWindow", "\347\255\211\346\257\224\344\276\213\350\260\203\346\225\264", nullptr));
        img_fill->setText(QApplication::translate("MainWindow", "\345\241\253\345\205\205\345\205\250\345\233\276", nullptr));
        opencv_texture->setText(QApplication::translate("MainWindow", "\347\272\271\347\220\206\350\277\201\347\247\273", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "\345\244\232\345\233\276\345\244\204\347\220\206", nullptr));
        dockWidget_3->setWindowTitle(QApplication::translate("MainWindow", "\350\211\262\345\275\251\350\260\203\346\225\264", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "G\345\200\274\357\274\232", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "B\345\200\274\357\274\232", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "R\345\200\274\357\274\232", nullptr));
        dockWidget_2->setWindowTitle(QApplication::translate("MainWindow", "\345\237\272\347\241\200\350\260\203\346\225\264", nullptr));
        label_light->setText(QApplication::translate("MainWindow", "0", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "\350\260\203\350\212\202\344\272\256\345\272\246\345\200\274\357\274\232", nullptr));
        label->setText(QApplication::translate("MainWindow", "\344\272\214\345\200\274\345\214\226\357\274\232", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\345\257\271\346\257\224\345\272\246\357\274\232", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\351\245\261\345\222\214\345\272\246\357\274\232", nullptr));
        label_yuzhi->setText(QApplication::translate("MainWindow", "128", nullptr));
        dockWidget->setWindowTitle(QApplication::translate("MainWindow", " \345\267\245\345\205\267\347\256\261", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276\347\211\207", nullptr));
        pushButton_gray->setText(QApplication::translate("MainWindow", "\347\201\260\345\272\246\345\214\226", nullptr));
        pushButton_junzhi->setText(QApplication::translate("MainWindow", "\345\235\207\345\200\274\346\273\244\346\263\242", nullptr));
        pushButton_origin->setText(QApplication::translate("MainWindow", "\346\230\276\347\244\272\345\216\237\345\233\276", nullptr));
        pushButton_gamma->setText(QApplication::translate("MainWindow", "\344\274\275\351\251\254\345\217\230\346\215\242", nullptr));
        pushButton_turnleft->setText(QString());
        pushButton_turnright->setText(QString());
        pushButton_turnleft_2->setText(QString());
        pushButton_turnleft_3->setText(QString());
        pushButton_bianyuan->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230\346\243\200\346\265\213", nullptr));
        pushButton_bianyuan_2->setText(QApplication::translate("MainWindow", "\350\276\271\347\274\230+\345\216\237\345\233\276", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "  \346\257\224\344\276\213\357\274\232", nullptr));
        label_fuhe->setText(QApplication::translate("MainWindow", "    0.5:0.5", nullptr));
        pushButton_bianyuan_5->setText(QApplication::translate("MainWindow", "\345\233\276\347\211\207\351\224\220\345\214\226", nullptr));
        menu_F->setTitle(QApplication::translate("MainWindow", "\346\226\207\344\273\266(&F)", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
