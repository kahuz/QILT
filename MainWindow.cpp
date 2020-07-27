#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")	//for debugging

#include "MainWindow.h"
#include <QDesktopWidget>

void MainWindow::draw_label()
{

}

void MainWindow::save_label()
{

}
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

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	QDesktopWidget desktop;

	ui.setupUi(this);
	label_dialog.move(desktop.screenGeometry().width() / 2 + this->width() / 2, desktop.screenGeometry().height() / 2 - this->height() / 2);

	init_viewer_pos_value();

	//connect signal-slot
	connect(ui.actionOpen_Label_View, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openLabelView()));
	connect(ui.actionOpen_Video_Image, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openVideoImage()));
	connect(ui.actionOpen_Images_Directory, SIGNAL(triggered()), this, SLOT(on_triggered_menu_openVideoImage()));
	connect(ui.actionVideoPlay, SIGNAL(triggered()), this, SLOT(on_triggered_videoPlay()));
	connect(ui.actionVideoPause, SIGNAL(triggered()), this, SLOT(on_triggered_videoPause()));
	connect(ui.actionLabelling, SIGNAL(triggered()), this, SLOT(on_triggered_labelling()));

	//register event
	ui.mainLabel->installEventFilter(this);
}

MainWindow::~MainWindow()
{
	if (main_viewer.is_runable())
		main_viewer.set_runable(false);

	if (main_viewer.is_running())
		main_viewer.set_running(false);
}

void MainWindow::init_viewer_pos_value()
{
	viewer_img_start_pos.x	= 0;
	viewer_img_start_pos.y	= 0;
	viewer_img_cur_pos.x	= 0;
	viewer_img_cur_pos.y	= 0;
	viewer_img_end_pos.x	= 0;
	viewer_img_end_pos.y	= 0;

}
//Whell, MouseButtonDblClick, MouseButtonPress, MouseButtonRelease, MouseMove
//Event Function
bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
	QMouseEvent* mouseEvent = (QMouseEvent*)event;
	if (object == ui.mainLabel && b_activeLabel)
	{
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			b_activeLB = true;
			label_rect.setX(mouseEvent->x());
			label_rect.setY(mouseEvent->y());

			std::cout << "start x pos : " << label_rect.x() << " start y pos : " << label_rect.y() << "\n";

			break;
		case QEvent::MouseMove:

			if (b_activeLB)
			{
				label_rect.setWidth(std::abs(mouseEvent->x() - label_rect.x()));
				label_rect.setWidth(std::abs(mouseEvent->y() - label_rect.y()));

				std::cout << "cur width : " << label_rect.width() << " cur height : " << label_rect.height() << "\n";
				
			}

			break;
		case QEvent::MouseButtonRelease:
			label_rect.setWidth(std::abs(mouseEvent->x() - label_rect.x()));
			label_rect.setWidth(std::abs(mouseEvent->y() - label_rect.y()));
			std::cout << "end width : " << label_rect.width() << " end height : " << label_rect.height() << "\n";
			//call label save func

			//init pos values
			init_viewer_pos_value();
			b_activeLB = false;

			break;
		}

		ui.mainLabel->update();
	}
	return false;
}
void MainWindow::paintEvent(QPaintEvent* event)
{
	QPainter painter;
	painter.begin(ui.mainLabel);

	if (b_activeLabel && b_activeLB)
	{
		painter.drawRect(label_rect);

	}
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


void MainWindow::on_triggered_videoPlay()
{
	play_video(ui.actionVideoPlay, PLAY);
}

void MainWindow::on_triggered_videoPause()
{
	play_video(ui.actionVideoPause, PAUSE);
}

void MainWindow::on_triggered_labelling()
{
	b_activeLabel = !b_activeLabel;
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
