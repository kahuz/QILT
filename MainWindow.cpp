#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")	//for debugging

#include "MainWindow.h"
#include <QDesktopWidget>

void MainWindow::open_video(std::string path)
{
	if (main_viewer.video.isOpened())
		main_viewer.video.release();

	main_viewer.video.open(path);

	if (!main_viewer.video.isOpened())
	{
		QMessageBox MsgBox;
		MsgBox.setText("Could not open video/image");
		MsgBox.setStandardButtons(QMessageBox::Ok);
		MsgBox.setDefaultButton(QMessageBox::Ok);

		if (MsgBox.exec() == QMessageBox::Ok)
		{
			MsgBox.close();
		}
	}

	Mat viewer_img;

	main_viewer.video >> viewer_img;

	ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(viewer_img.data, viewer_img.cols, viewer_img.rows, viewer_img.step, QImage::Format_BGR888)));

	main_viewer.set_runable(true);
	main_viewer.set_running(PAUSE);
	main_viewer.set_viwer(ui.mainLabel);

	this->repaint();
}
//note this->repaint()
void MainWindow::play_video(QAction* button, bool play)
{
	std::cout << __FUNCTION__ ;
	if (play == PLAY)
	{
		std::cout << "  Play is True , is_running : " << main_viewer.is_running() << " is_runable : " << main_viewer.is_runable() << std::endl;
		//재생 중일 때 재생 버튼을 클릭되지 않은 상태로 돌린다
		if (main_viewer.is_running())
		{
			std::cout << " already play " << std::endl;
			button->setChecked(true);
			return;
		}

		if (main_viewer.is_runable())
		{
			std::cout << " now playing video " << std::endl;
			main_viewer.set_running(PLAY);

			std::thread video_thread(&Viewer::show, &main_viewer);

			video_thread.detach();
		}
	}
	else if (play == PAUSE)
	{
		if (main_viewer.is_running())
		{
			//일시중지
			main_viewer.set_running(PAUSE);
		}
		//재생 중지 혹은 비디오가 출력중이지 않을때 멈춤 버튼을 클릭되지 않은 상태로 돌린다
		button->setChecked(true);
		return;
	}

	else if (!main_viewer.video.isOpened())
	{
		QMessageBox MsgBox;
		MsgBox.setText("Please, open the video/image file");
		MsgBox.setStandardButtons(QMessageBox::Ok);
		MsgBox.setDefaultButton(QMessageBox::Ok);

		if (MsgBox.exec() == QMessageBox::Ok)
		{
			MsgBox.close();
			button->setChecked(false);
		}
	}
}

void MainWindow::video_mouse_callback(int event, int x, int y, int flags, void* user_data) {
	std::cout << "Cur Event : " << event << " Flags : " << flags << " Cur x : " << x << " Cur y : " << y << std::endl;

	if (EVENT_MOUSEMOVE)//0
	{
		//왼쪽 버튼을 누른채 드래그 중 임을 암시
		if (flags == EVENT_FLAG_LBUTTON);
		//오른쪽 버튼을 누른채 드래그 중 임을 암시
		else if (flags == EVENT_FLAG_RBUTTON);

	}
	else if (EVENT_LBUTTONDOWN);	//1
	else if (EVENT_LBUTTONUP);		//4
	else if (EVENT_RBUTTONDOWN);	//2
	else if (EVENT_RBUTTONUP);		//5
	else if (EVENT_MOUSEHWHEEL)		//10
	{
		int wheel_val = 0;
		if (getMouseWheelDelta(flags) > 0)
			wheel_val++;
		else
			wheel_val--;
	}
}

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	QDesktopWidget desktop;

	ui.setupUi(this);
	label_dialog.move(desktop.screenGeometry().width() / 2 + this->width() / 2, desktop.screenGeometry().height() / 2 - this->height() / 2);

	connect(ui.actionOpen_Label_View, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openLabelView()));
	connect(ui.actionOpen_Video_Image, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openVideoImage()));
	connect(ui.actionOpen_Images_Directory, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openVideoImage()));
	connect(ui.actionVideoPlay, SIGNAL(triggered()), this, SLOT(on_triggered_menu_videoPlay()));
	connect(ui.actionVideoPause, SIGNAL(triggered()), this, SLOT(on_triggered_menu_videoPause()));
}

MainWindow::~MainWindow()
{
	if (main_viewer.is_runable())
		main_viewer.set_runable(false);

	if (main_viewer.is_running())
		main_viewer.set_running(false);
}

//Trigger Functions
void MainWindow::on_triggered_menu_openVideoImage()
{
	std::cout << "on_triggered_menu_openVideoImage" << std::endl;

	QString video_img_path = QFileDialog::getOpenFileName(this,
		"Select source file",
		".",
		"All files (*.*)");
	open_video(video_img_path.toStdString());

	std::cout << video_img_path.toStdString() << std::endl;
}

void MainWindow::on_triggered_menu_openImagesDirectory()
{
	std::cout << "on_triggered_menu_openImagesDirectory" << std::endl;

	QString imgs_folder_path = QFileDialog::getOpenFileName(this,
		"Select source file",
		".",
		"All files (*.*)");

	std::cout << imgs_folder_path.toStdString() << std::endl;
}

void MainWindow::on_triggered_menu_openLabelView()
{
	std::cout << "on_triggered_menu_openLabelView" << std::endl;
	label_dialog.show();
}


void MainWindow::on_triggered_menu_videoPlay()
{
	play_video(ui.actionVideoPlay, PLAY);
}

void MainWindow::on_triggered_menu_videoPause()
{
	play_video(ui.actionVideoPause, PAUSE);
}

void Viewer::show()
{
	while (b_runable)
	{
		while (b_running == PAUSE) { Sleep(100); };

		Mat viewer_img;
		this->video >> viewer_img;
		std::cout << "asdf " << std::endl;
		this->viwer->setPixmap(QPixmap::fromImage(QImage(viewer_img.data, viewer_img.cols, viewer_img.rows, viewer_img.step, QImage::Format_BGR888)));
		//30fps
		Sleep(33);
	}

	this->set_runable(true);
}

bool Viewer::is_running()
{
	return this->b_running;
}

bool Viewer::is_runable()
{
	return this->b_runable;
}

void Viewer::set_running(bool is_running)
{
	if (is_running)
		this->b_running = true;
	else
		this->b_running = false;
}

void Viewer::set_runable(bool is_runable)
{
	if (is_runable)
		this->b_runable = true;
	else
		this->b_runable = false;
}

void Viewer::set_viwer(QLabel* parent_viwer)
{
	this->viwer = parent_viwer;
}
