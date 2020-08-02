#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")	//for debugging

#include "MainWindow.h"
#include <QDesktopWidget>

void MainWindow::draw_label()
{

}

void MainWindow::save_label()
{

}
cv::Range MainWindow::get_Range(int loc_1, int loc_2)
{
	int start_loc, end_loc;

	if (loc_1 > loc_2)
	{
		start_loc = loc_2;
		end_loc = loc_1;
	}
	else
	{
		start_loc = loc_1;
		end_loc = loc_2;
	}

	return cv::Range(start_loc, end_loc);
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

	main_viewer.video >> main_viewer.view_img;

	ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(main_viewer.view_img.data, main_viewer.view_img.cols, main_viewer.view_img.rows, main_viewer.view_img.step, QImage::Format_BGR888)));

	main_viewer.set_runable(true);
	main_viewer.set_running(PAUSE);

	ui.mainLabel->repaint();
}
//note this->repaint()
void MainWindow::play_video(QAction* play_btn, QAction* pause_btn, bool play)
{
	std::cout << __FUNCTION__ ;
	if (play == PLAY)
	{
		std::cout << "  Play is True , is_running : " << main_viewer.is_running() << " is_runable : " << main_viewer.is_runable() << std::endl;
		//재생 중일 때 재생 버튼을 클릭되지 않은 상태로 돌린다
		if (main_viewer.is_running())
		{
			std::cout << " already play " << std::endl;
			play_btn->setChecked(true);
			pause_btn->setChecked(false);
			return;
		}

		if (main_viewer.is_runable())
		{
			std::cout << " now playing video " << std::endl;
			main_viewer.set_running(PLAY);

			std::thread video_thread(&Viewer::show, &main_viewer);

			video_thread.detach();

			pause_btn->setChecked(false);
		}
	}
	else if (play == PAUSE)
	{
		if (main_viewer.is_running())
		{
			//일시중지
			main_viewer.set_running(PAUSE);
			//재생 중지 혹은 비디오가 출력중이지 않을때 멈춤 버튼을 클릭되지 않은 상태로 돌린다
			pause_btn->setChecked(true);
			play_btn->setChecked(false);
		}
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
			play_btn->setChecked(false);
			pause_btn->setChecked(false);
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

	main_viewer.set_label_thickness(2);
	main_viewer.set_label_color(COMMON_COLOR_RED);
	main_viewer.set_viwer(ui.mainLabel);

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
		int mouse_x = mouseEvent->x();
		int mouse_y = mouseEvent->y();
		int start_x, start_y, end_x, end_y;
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			main_viewer.set_running(PAUSE);
			b_activeLB = true;
			//label_rect.setX(mouseEvent->x());
			//label_rect.setY(mouseEvent->y());
			main_viewer.init_label(mouse_x, mouse_y);
			std::cout << "start x pos : " << mouse_x << " start y pos : " << mouse_y << "\n";

			if (label_setting_dialog.get_is_setting())
			{
				int label_color = label_setting_dialog.get_label_color();
				int label_thickness = label_setting_dialog.get_label_thickness();
				std::string label_tag = label_setting_dialog.get_label_name();
				
				main_viewer.set_label_thickness(label_thickness);
				main_viewer.set_label_color(label_color);
				main_viewer.set_label_name(label_tag);

				label_setting_dialog.increase_label_cnt();
				
			}
			break;
		case QEvent::MouseMove:

			if (b_activeLB)
			{
				main_viewer.draw_label(mouse_x, mouse_y);
				std::cout << "draw x : " << mouse_x << " draw y : " << mouse_y << "\n";
			
				ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(main_viewer.view_img.data, main_viewer.view_img.cols, main_viewer.view_img.rows, main_viewer.view_img.step, QImage::Format_BGR888)));

				ui.mainLabel->repaint();
			}

			break;
		case QEvent::MouseButtonRelease:

			std::cout << "end width : " << main_viewer.label_rect.width << " end height : " << main_viewer.label_rect.width << "\n";
			std::cout << "label img x1 : " << main_viewer.label_rect.x << " x2 : " << mouse_x << "label img y1 : " << main_viewer.label_rect.y << " y2 : " << mouse_y << "\n";
			
			cv::Mat label_img = main_viewer.view_org_img(get_Range(main_viewer.label_rect.y, mouse_y), get_Range(main_viewer.label_rect.x, mouse_x));
			//call label save func
			if (label_dialog.get_is_open())
			{
				QPixmap prev_img(QPixmap::fromImage(QImage(label_img.data, label_img.cols, label_img.rows, label_img.step, QImage::Format_BGR888)));
				label_dialog.draw_prev_img(prev_img);
			}
			std::string label_file_name;
			label_file_name.append(PATH_LABEL_DIR);
			label_file_name.append(label_setting_dialog.get_label_name());
			label_file_name.append(IMG_FORMAT_JPG);
			imwrite(label_file_name, label_img);
			main_viewer.release_label();

			//init pos values
			init_viewer_pos_value();
			b_activeLB = false;

			ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(main_viewer.view_img.data, main_viewer.view_img.cols, main_viewer.view_img.rows, main_viewer.view_img.step, QImage::Format_BGR888)));

			ui.mainLabel->repaint();
			main_viewer.set_running(PLAY);
			break;
		}

	}
	return false;
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
	label_dialog.set_is_opne(true);
}


void MainWindow::on_triggered_videoPlay()
{
	play_video(ui.actionVideoPlay, ui.actionVideoPause, PLAY);
}

void MainWindow::on_triggered_videoPause()
{
	play_video(ui.actionVideoPlay, ui.actionVideoPause, PAUSE);
}

void MainWindow::on_triggered_labelling()
{
	b_activeLabel = !b_activeLabel;

	if(b_activeLabel)
		label_setting_dialog.show();
}



void Viewer::show()
{
	while (b_runable)
	{
		while (b_running == PAUSE) { Sleep(100); };

		this->video >> this->view_img;
		this->viwer->setPixmap(QPixmap::fromImage(QImage(view_img.data, view_img.cols, view_img.rows, view_img.step, QImage::Format_BGR888)));
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

void Viewer::set_label_color(int color)
{
	this->label_color = color_set[color];
}

void Viewer::set_label_thickness(int thickness)
{
	this->label_thickness = thickness;
}

void Viewer::set_label_name(std::string tag)
{
	this->label_tag.clear();
	this->label_tag.assign(tag, 0, tag.size());
}

void Viewer::init_label(int x, int y)
{
	this->view_org_img = this->view_img.clone();
	this->label_rect.x = x;
	this->label_rect.y = y;
}

void Viewer::draw_label(int x, int y)
{
	cv::Point start_label_pos(label_rect.x, label_rect.y + LABEL_TEXT_HEIGHT_MARGIN);
	cv::Point start_rect_pos(label_rect.x, label_rect.y);
	cv::Point end_pos(x, y);
	std::string tag = this->label_tag;

	this->view_img = this->view_org_img.clone();


	this->label_rect.width = std::abs(x - this->label_rect.x);
	this->label_rect.height = std::abs(y - this->label_rect.y);

	cv::rectangle(this->view_img, start_rect_pos, end_pos, this->label_color, this->label_thickness);
	cv::putText(this->view_img, this->label_tag, start_label_pos, cv::FONT_HERSHEY_SIMPLEX, LABEL_TEXT_RATE,this->label_color, this->label_thickness);

}

void Viewer::release_label()
{
	this->label_rect.x = 0;
	this->label_rect.y = 0;
	this->view_img = this->view_org_img.clone();
}

void Viewer::set_running(bool is_running)
{
	this->b_running = is_running;
}

void Viewer::set_runable(bool is_runable)
{
	this->b_runable = is_runable;
}

void Viewer::set_viwer(QLabel* parent_viwer)
{
	this->viwer = parent_viwer;
}
