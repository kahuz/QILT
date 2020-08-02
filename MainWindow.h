#pragma once
#ifndef __MAIN_WINDOW__
#define __MAIN_WINDOW__
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
#include "LabelSettingDialog.h"

#define PLAY    true
#define PAUSE   false

#define LABEL_TEXT_HEIGHT_MARGIN -10
#define LABEL_TEXT_RATE 0.5

using namespace cv;

class Viewer
{
public:

    void show();
    bool is_running();
    bool is_runable();

    void draw_label(int x, int y);
    void release_label();

    void init_label(int x, int y);
    void set_label_color(int color);
    void set_label_thickness(int thickness);
    void set_label_name(std::string tag);
    void set_running(bool is_running);
    void set_runable(bool is_runable);
    void set_viwer(QLabel* parent_viwer);
    VideoCapture video;
    Mat view_img;
    Mat view_org_img;

    Rect label_rect;
    int label_thickness;
    std::string label_tag;
    Scalar label_color;
    Scalar color_set[COMMON_COLOR_MAX] = { Scalar(0,0,0), Scalar(255,255,255), 
        Scalar(0,0,255),Scalar(0,255,0), Scalar(255,0,0), Scalar(0,255,255) };
    

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
    cv::Range get_Range(int loc_1, int loc_2);
    void open_video(std::string path);
    void play_video(QAction* play_btn, QAction* pause_btn, bool play);

private slots:
    void on_triggered_menu_openLabelView();
    void on_triggered_menu_openVideoImage();
    void on_triggered_menu_openImagesDirectory();

    void on_triggered_videoPlay();
    void on_triggered_videoPause();
    void on_triggered_labelling();

private:
    const QString main_title = "MainWindow Title";
    int label_file_idx = 1;
    LabelDialog label_dialog;
    LabelSettingDialog label_setting_dialog;
    Viewer main_viewer;
    bool b_activeLB = false;
    bool b_activeLabel = false;

    cv::Point2i viewer_img_start_pos;
    cv::Point2i viewer_img_cur_pos;
    cv::Point2i viewer_img_end_pos;
    virtual bool eventFilter(QObject* object, QEvent* event);

    

    Ui::MainWindow ui;
};

#endif