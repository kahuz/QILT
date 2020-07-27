#pragma once
//QT lib
#include <QtWidgets/QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QMouseEvent>
#include <QEvent>
#include <QPixmap>
#include <QPainter>
//OpenCV lib
#include <opencv2/core.hpp>		// basic OpenCV structures ( Mat , Scalar, etc... )
#include <opencv2/videoio.hpp>	// for VideoCapture, VideoWriter
#include <opencv2/highgui.hpp>	// OpenCV Window IO
#include <opencv2/opencv.hpp>
//cpp lib
#include <Windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
//user lib
#include "ui_MainWindow.h"
#include "LabelDialog.h"

#define PLAY    true
#define PAUSE   false

using namespace cv;

class Viewer
{
public:

    void show();
    bool is_running();
    bool is_runable();

    void set_running(bool is_running);
    void set_runable(bool is_runable);
    void set_viwer(QLabel* parent_viwer);
    VideoCapture video;

private:
    QLabel *viwer;

    bool b_running = false;
    bool b_runable = false;
};
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    ~MainWindow();
    void init_viewer_pos_value();
    void draw_label();
    void save_label();
    void open_video(std::string path);
    void play_video(QAction* button, bool play);

private slots:
    void on_triggered_menu_openLabelView();
    void on_triggered_menu_openVideoImage();
    void on_triggered_menu_openImagesDirectory();

    void on_triggered_videoPlay();
    void on_triggered_videoPause();
    void on_triggered_labelling();

private:
    const QString main_title = "MainWindow Title";
    LabelDialog label_dialog;
    Viewer main_viewer;
    bool b_activeLB = false;
    bool b_activeLabel = false;

    QRect label_rect;
    cv::Point2i viewer_img_start_pos;
    cv::Point2i viewer_img_cur_pos;
    cv::Point2i viewer_img_end_pos;
    virtual bool eventFilter(QObject* object, QEvent* event);
    virtual void paintEvent(QPaintEvent* event) override;

    

    Ui::MainWindow ui;
};

