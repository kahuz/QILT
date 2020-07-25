#pragma once
#include <QtWidgets/QMainWindow>
#include <QPixmap>

#include <QMessageBox>
#include <QFileDialog>
#include "ui_MainWindow.h"
#include "LabelDialog.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = Q_NULLPTR);
    void MainWindow::view_video(std::string path);

private slots:
    void on_triggered_menu_openLabelView();
    void on_triggered_menu_openVideoImage();
    void on_triggered_menu_openImagesDirectory();

private:
    Ui::MainWindow ui;
    LabelDialog label_dialog;
    void MainWindow::video_mouse_callback(int event, int x, int y, int flags, void* user_data);

    const QString main_title = "MainWindow Title";
};

