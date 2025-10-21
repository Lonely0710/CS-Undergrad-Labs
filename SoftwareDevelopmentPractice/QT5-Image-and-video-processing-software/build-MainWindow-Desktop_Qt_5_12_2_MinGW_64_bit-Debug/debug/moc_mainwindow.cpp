/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QT5-Image-and-video-processing-software-master/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VideoProcessor_t {
    QByteArrayData data[4];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VideoProcessor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VideoProcessor_t qt_meta_stringdata_VideoProcessor = {
    {
QT_MOC_LITERAL(0, 0, 14), // "VideoProcessor"
QT_MOC_LITERAL(1, 15, 10), // "frameReady"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 5) // "image"

    },
    "VideoProcessor\0frameReady\0\0image"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VideoProcessor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,

       0        // eod
};

void VideoProcessor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VideoProcessor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->frameReady((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VideoProcessor::*)(const QImage & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VideoProcessor::frameReady)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VideoProcessor::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_VideoProcessor.data,
    qt_meta_data_VideoProcessor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VideoProcessor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VideoProcessor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VideoProcessor.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int VideoProcessor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void VideoProcessor::frameReady(const QImage & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[57];
    char stringdata0[1466];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 24), // "on_action_Open_triggered"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 21), // "on_pushButton_clicked"
QT_MOC_LITERAL(4, 59, 24), // "on_action_Save_triggered"
QT_MOC_LITERAL(5, 84, 26), // "on_pushButton_gray_clicked"
QT_MOC_LITERAL(6, 111, 28), // "on_pushButton_junzhi_clicked"
QT_MOC_LITERAL(7, 140, 26), // "on_pushButton_save_clicked"
QT_MOC_LITERAL(8, 167, 28), // "on_pushButton_origin_clicked"
QT_MOC_LITERAL(9, 196, 27), // "on_pushButton_gamma_clicked"
QT_MOC_LITERAL(10, 224, 30), // "on_pushButton_turnleft_clicked"
QT_MOC_LITERAL(11, 255, 31), // "on_pushButton_turnright_clicked"
QT_MOC_LITERAL(12, 287, 32), // "on_pushButton_turnleft_2_clicked"
QT_MOC_LITERAL(13, 320, 32), // "on_pushButton_turnleft_3_clicked"
QT_MOC_LITERAL(14, 353, 30), // "on_pushButton_bianyuan_clicked"
QT_MOC_LITERAL(15, 384, 32), // "on_pushButton_bianyuan_2_clicked"
QT_MOC_LITERAL(16, 417, 32), // "on_horizontalSlider_valueChanged"
QT_MOC_LITERAL(17, 450, 5), // "value"
QT_MOC_LITERAL(18, 456, 34), // "on_horizontalSlider_2_valueCh..."
QT_MOC_LITERAL(19, 491, 6), // "value1"
QT_MOC_LITERAL(20, 498, 34), // "on_horizontalSlider_R_valueCh..."
QT_MOC_LITERAL(21, 533, 34), // "on_horizontalSlider_G_valueCh..."
QT_MOC_LITERAL(22, 568, 34), // "on_horizontalSlider_B_valueCh..."
QT_MOC_LITERAL(23, 603, 38), // "on_horizontalSlider_erzhi_val..."
QT_MOC_LITERAL(24, 642, 38), // "on_horizontalSlider_duibi_val..."
QT_MOC_LITERAL(25, 681, 38), // "on_horizontalSlider_baohe_val..."
QT_MOC_LITERAL(26, 720, 32), // "on_pushButton_bianyuan_5_clicked"
QT_MOC_LITERAL(27, 753, 23), // "on_pushButton_3_clicked"
QT_MOC_LITERAL(28, 777, 23), // "on_pushButton_4_clicked"
QT_MOC_LITERAL(29, 801, 21), // "on_pick_img_1_clicked"
QT_MOC_LITERAL(30, 823, 19), // "on_img_keep_clicked"
QT_MOC_LITERAL(31, 843, 19), // "on_img_fill_clicked"
QT_MOC_LITERAL(32, 863, 24), // "on_opencv_mosaic_clicked"
QT_MOC_LITERAL(33, 888, 23), // "on_opencv_merge_clicked"
QT_MOC_LITERAL(34, 912, 21), // "on_pick_img_2_clicked"
QT_MOC_LITERAL(35, 934, 25), // "on_opencv_texture_clicked"
QT_MOC_LITERAL(36, 960, 23), // "on_pushButton_6_clicked"
QT_MOC_LITERAL(37, 984, 14), // "updatePosition"
QT_MOC_LITERAL(38, 999, 9), // "onTimeout"
QT_MOC_LITERAL(39, 1009, 40), // "on_horizontalSlider_suofang_v..."
QT_MOC_LITERAL(40, 1050, 39), // "on_VideohorizontalSlider_2_va..."
QT_MOC_LITERAL(41, 1090, 26), // "on_pushButton_open_clicked"
QT_MOC_LITERAL(42, 1117, 11), // "updateFrame"
QT_MOC_LITERAL(43, 1129, 32), // "on_pushButton_saveframes_clicked"
QT_MOC_LITERAL(44, 1162, 26), // "on_pushButton_grey_clicked"
QT_MOC_LITERAL(45, 1189, 30), // "on_pushButton_origin_2_clicked"
QT_MOC_LITERAL(46, 1220, 28), // "on_pushButton_smooth_clicked"
QT_MOC_LITERAL(47, 1249, 28), // "on_pushButton_binary_clicked"
QT_MOC_LITERAL(48, 1278, 26), // "on_pushButton_edge_clicked"
QT_MOC_LITERAL(49, 1305, 12), // "onFrameReady"
QT_MOC_LITERAL(50, 1318, 5), // "image"
QT_MOC_LITERAL(51, 1324, 27), // "on_pushButton_play1_clicked"
QT_MOC_LITERAL(52, 1352, 11), // "updateSpeed"
QT_MOC_LITERAL(53, 1364, 11), // "speedFactor"
QT_MOC_LITERAL(54, 1376, 29), // "on_pushButton_play1_2_clicked"
QT_MOC_LITERAL(55, 1406, 29), // "on_pushButton_play1_3_clicked"
QT_MOC_LITERAL(56, 1436, 29) // "on_pushButton_play1_4_clicked"

    },
    "MainWindow\0on_action_Open_triggered\0"
    "\0on_pushButton_clicked\0on_action_Save_triggered\0"
    "on_pushButton_gray_clicked\0"
    "on_pushButton_junzhi_clicked\0"
    "on_pushButton_save_clicked\0"
    "on_pushButton_origin_clicked\0"
    "on_pushButton_gamma_clicked\0"
    "on_pushButton_turnleft_clicked\0"
    "on_pushButton_turnright_clicked\0"
    "on_pushButton_turnleft_2_clicked\0"
    "on_pushButton_turnleft_3_clicked\0"
    "on_pushButton_bianyuan_clicked\0"
    "on_pushButton_bianyuan_2_clicked\0"
    "on_horizontalSlider_valueChanged\0value\0"
    "on_horizontalSlider_2_valueChanged\0"
    "value1\0on_horizontalSlider_R_valueChanged\0"
    "on_horizontalSlider_G_valueChanged\0"
    "on_horizontalSlider_B_valueChanged\0"
    "on_horizontalSlider_erzhi_valueChanged\0"
    "on_horizontalSlider_duibi_valueChanged\0"
    "on_horizontalSlider_baohe_valueChanged\0"
    "on_pushButton_bianyuan_5_clicked\0"
    "on_pushButton_3_clicked\0on_pushButton_4_clicked\0"
    "on_pick_img_1_clicked\0on_img_keep_clicked\0"
    "on_img_fill_clicked\0on_opencv_mosaic_clicked\0"
    "on_opencv_merge_clicked\0on_pick_img_2_clicked\0"
    "on_opencv_texture_clicked\0"
    "on_pushButton_6_clicked\0updatePosition\0"
    "onTimeout\0on_horizontalSlider_suofang_valueChanged\0"
    "on_VideohorizontalSlider_2_valueChanged\0"
    "on_pushButton_open_clicked\0updateFrame\0"
    "on_pushButton_saveframes_clicked\0"
    "on_pushButton_grey_clicked\0"
    "on_pushButton_origin_2_clicked\0"
    "on_pushButton_smooth_clicked\0"
    "on_pushButton_binary_clicked\0"
    "on_pushButton_edge_clicked\0onFrameReady\0"
    "image\0on_pushButton_play1_clicked\0"
    "updateSpeed\0speedFactor\0"
    "on_pushButton_play1_2_clicked\0"
    "on_pushButton_play1_3_clicked\0"
    "on_pushButton_play1_4_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      51,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,  269,    2, 0x08 /* Private */,
       3,    0,  270,    2, 0x08 /* Private */,
       4,    0,  271,    2, 0x08 /* Private */,
       5,    0,  272,    2, 0x08 /* Private */,
       6,    0,  273,    2, 0x08 /* Private */,
       7,    0,  274,    2, 0x08 /* Private */,
       8,    0,  275,    2, 0x08 /* Private */,
       9,    0,  276,    2, 0x08 /* Private */,
      10,    0,  277,    2, 0x08 /* Private */,
      11,    0,  278,    2, 0x08 /* Private */,
      12,    0,  279,    2, 0x08 /* Private */,
      13,    0,  280,    2, 0x08 /* Private */,
      14,    0,  281,    2, 0x08 /* Private */,
      15,    0,  282,    2, 0x08 /* Private */,
      16,    1,  283,    2, 0x08 /* Private */,
      18,    1,  286,    2, 0x08 /* Private */,
      20,    1,  289,    2, 0x08 /* Private */,
      21,    1,  292,    2, 0x08 /* Private */,
      22,    1,  295,    2, 0x08 /* Private */,
      23,    1,  298,    2, 0x08 /* Private */,
      24,    1,  301,    2, 0x08 /* Private */,
      25,    1,  304,    2, 0x08 /* Private */,
      26,    0,  307,    2, 0x08 /* Private */,
      27,    0,  308,    2, 0x08 /* Private */,
      28,    0,  309,    2, 0x08 /* Private */,
      29,    0,  310,    2, 0x08 /* Private */,
      30,    0,  311,    2, 0x08 /* Private */,
      31,    0,  312,    2, 0x08 /* Private */,
      32,    0,  313,    2, 0x08 /* Private */,
      33,    0,  314,    2, 0x08 /* Private */,
      34,    0,  315,    2, 0x08 /* Private */,
      35,    0,  316,    2, 0x08 /* Private */,
      36,    0,  317,    2, 0x08 /* Private */,
      37,    0,  318,    2, 0x08 /* Private */,
      38,    0,  319,    2, 0x08 /* Private */,
      39,    1,  320,    2, 0x08 /* Private */,
      40,    1,  323,    2, 0x08 /* Private */,
      41,    0,  326,    2, 0x08 /* Private */,
      42,    0,  327,    2, 0x08 /* Private */,
      43,    0,  328,    2, 0x08 /* Private */,
      44,    0,  329,    2, 0x08 /* Private */,
      45,    0,  330,    2, 0x08 /* Private */,
      46,    0,  331,    2, 0x08 /* Private */,
      47,    0,  332,    2, 0x08 /* Private */,
      48,    0,  333,    2, 0x08 /* Private */,
      49,    1,  334,    2, 0x08 /* Private */,
      51,    0,  337,    2, 0x08 /* Private */,
      52,    1,  338,    2, 0x08 /* Private */,
      54,    0,  341,    2, 0x08 /* Private */,
      55,    0,  342,    2, 0x08 /* Private */,
      56,    0,  343,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   19,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QImage,   50,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double,   53,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_action_Open_triggered(); break;
        case 1: _t->on_pushButton_clicked(); break;
        case 2: _t->on_action_Save_triggered(); break;
        case 3: _t->on_pushButton_gray_clicked(); break;
        case 4: _t->on_pushButton_junzhi_clicked(); break;
        case 5: _t->on_pushButton_save_clicked(); break;
        case 6: _t->on_pushButton_origin_clicked(); break;
        case 7: _t->on_pushButton_gamma_clicked(); break;
        case 8: _t->on_pushButton_turnleft_clicked(); break;
        case 9: _t->on_pushButton_turnright_clicked(); break;
        case 10: _t->on_pushButton_turnleft_2_clicked(); break;
        case 11: _t->on_pushButton_turnleft_3_clicked(); break;
        case 12: _t->on_pushButton_bianyuan_clicked(); break;
        case 13: _t->on_pushButton_bianyuan_2_clicked(); break;
        case 14: _t->on_horizontalSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 15: _t->on_horizontalSlider_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->on_horizontalSlider_R_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: _t->on_horizontalSlider_G_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: _t->on_horizontalSlider_B_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 19: _t->on_horizontalSlider_erzhi_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 20: _t->on_horizontalSlider_duibi_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 21: _t->on_horizontalSlider_baohe_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->on_pushButton_bianyuan_5_clicked(); break;
        case 23: _t->on_pushButton_3_clicked(); break;
        case 24: _t->on_pushButton_4_clicked(); break;
        case 25: _t->on_pick_img_1_clicked(); break;
        case 26: _t->on_img_keep_clicked(); break;
        case 27: _t->on_img_fill_clicked(); break;
        case 28: _t->on_opencv_mosaic_clicked(); break;
        case 29: _t->on_opencv_merge_clicked(); break;
        case 30: _t->on_pick_img_2_clicked(); break;
        case 31: _t->on_opencv_texture_clicked(); break;
        case 32: _t->on_pushButton_6_clicked(); break;
        case 33: _t->updatePosition(); break;
        case 34: _t->onTimeout(); break;
        case 35: _t->on_horizontalSlider_suofang_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 36: _t->on_VideohorizontalSlider_2_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 37: _t->on_pushButton_open_clicked(); break;
        case 38: _t->updateFrame(); break;
        case 39: _t->on_pushButton_saveframes_clicked(); break;
        case 40: _t->on_pushButton_grey_clicked(); break;
        case 41: _t->on_pushButton_origin_2_clicked(); break;
        case 42: _t->on_pushButton_smooth_clicked(); break;
        case 43: _t->on_pushButton_binary_clicked(); break;
        case 44: _t->on_pushButton_edge_clicked(); break;
        case 45: _t->onFrameReady((*reinterpret_cast< const QImage(*)>(_a[1]))); break;
        case 46: _t->on_pushButton_play1_clicked(); break;
        case 47: _t->updateSpeed((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 48: _t->on_pushButton_play1_2_clicked(); break;
        case 49: _t->on_pushButton_play1_3_clicked(); break;
        case 50: _t->on_pushButton_play1_4_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 51)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 51;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 51)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 51;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
