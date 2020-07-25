#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

#include "MainWindow.h"
#include <QDesktopWidget>
//opencv lib
#include <opencv2/core.hpp>		// basic OpenCV structures ( Mat , Scalar, etc... )
#include <opencv2/videoio.hpp>	// for VideoCapture, VideoWriter
#include <opencv2/highgui.hpp>	// OpenCV Window IO
#include <opencv2/opencv.hpp>
//cpp lib
#include <iostream>
#include <string>

using namespace cv;

//����� ������ �о�� ��� ���� ���� �� �ʱ�ȭ
std::string video_path("C:\\Users\\kahu\\Videos\\driving\\bus.mp4");
//����� ������ ���� ������ ���� ���� �� �ʱ�ȭ
int frame_cnt = 0;

//param event : mouse event
//param x, y : mouse pos
//param flags : mouse flags
//param user_data : �߰������� ���޹��� ������

void MainWindow::view_video(std::string path)
{
	//���� ��θ� ���������� �ƴ� �Է¿� ���� ������ ��� �Ʒ� �ּ��� ������ �ȴ�
//cout << "input file path\n";
//cin >> video_path;
//cout << "file path is " << video_path << endl;
//video_path�� ���� ���� ������ �����´�. CAP_FFMPEG�� ���� ������ �о���ų� FFPMEG ���̺귯���� �̿��� ��Ʈ������ ����
//�ܼ� ������ �о�� ��� CAP_FFMPEG �÷��׸� �����ص� ��� ����
	VideoCapture bus_video(path);

	//bus_video�� ����� ���� ���Ϸκ��� ������ �̹����� �޾ƿ� Mat ���� ����
	Mat bus_image;

	//video_path�� ���� ���� ������ �������� ���� ���
	if (!bus_video.isOpened())
	{
		std::cout << "Could not open video" << video_path << std::endl;
	}
	bool is_play = true;
	//while�� ���� bus_video�κ��� ������ �̹����� �о��
	while (is_play)
	{
		//bus_video�κ��� �ϳ��� �������� �о� bus_image�� ����
		bus_video >> bus_image;
		//bus_image�� ���޹��� �̹����� ���ٸ�
		if (bus_image.empty())
		{
			std::cout << "video end" << std::endl;
			break;
		}

		//opencv�� �޾ƿ� �̹����� BGR ���·� ����ǹǷ� ������ �÷��׸� BFR888�� �����Ͽ� �̹��� ����
		ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(bus_image.data, bus_image.cols, bus_image.rows, bus_image.step, QImage::Format_BGR888)));
		//���� ������ ī��Ʈ ����, �����ص� �������
		frame_cnt++;
		//���� ������ ���
		std::cout << "Frame: " << frame_cnt << "\n";
		//������ �о�� ���� �ӵ��� waitKey�� ����
		//33���� ������ ���� �� 30fps �ӵ��� ����ϱ� ����
		char c = (char)waitKey(33);
		//�̹����� ��������Ƿ� mainwindow�� ������Ʈ�� refrash�ϱ� ���� show �Լ� ȣ��
		this->show();
		// esc Ű �Է��� ������ while ���ᱸ��
		// QT event �������� ���� ����
		if (c == 27)
			is_play = false;
	}
}

void MainWindow::video_mouse_callback(int event, int x, int y, int flags, void* user_data) {
	std::cout << "Cur Event : " << event << " Flags : " << flags << " Cur x : " << x << " Cur y : " << y << std::endl;

	if (EVENT_MOUSEMOVE)//0
	{
		//���� ��ư�� ����ä �巡�� �� ���� �Ͻ�
		if (flags == EVENT_FLAG_LBUTTON);
		//������ ��ư�� ����ä �巡�� �� ���� �Ͻ�
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
	
}

//Trigger Functions
void MainWindow::on_triggered_menu_openVideoImage()
{
	std::cout << "on_triggered_menu_openVideoImage" << std::endl;

	QString video_img_path = QFileDialog::getOpenFileName(this,
		"Select source file",
		".",
		"All files (*.*)");
	view_video(video_img_path.toStdString());

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