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

//출력할 영상을 읽어올 경로 변수 선언 및 초기화
std::string video_path("C:\\Users\\kahu\\Videos\\driving\\bus.mp4");
//출력한 프레임 수를 저장할 변수 선언 및 초기화
int frame_cnt = 0;

//param event : mouse event
//param x, y : mouse pos
//param flags : mouse flags
//param user_data : 추가적으로 전달받을 데이터

void MainWindow::view_video(std::string path)
{
	//영상 경로를 전역변수가 아닌 입력에 따라 변경할 경우 아래 주석을 따르면 된다
//cout << "input file path\n";
//cin >> video_path;
//cout << "file path is " << video_path << endl;
//video_path의 영상 파일 정보를 가져온다. CAP_FFMPEG는 영상 파일을 읽어오거나 FFPMEG 라이브러리를 이용해 스트리밍을 제공
//단순 파일을 읽어올 경우 CAP_FFMPEG 플래그를 생략해도 상관 없다
	VideoCapture bus_video(path);

	//bus_video에 연결된 영상 파일로부터 프레임 이미지를 받아올 Mat 변수 선언
	Mat bus_image;

	//video_path로 부터 파일 정보를 가져오지 못할 경우
	if (!bus_video.isOpened())
	{
		std::cout << "Could not open video" << video_path << std::endl;
	}
	bool is_play = true;
	//while을 돌며 bus_video로부터 프레임 이미지를 읽어옴
	while (is_play)
	{
		//bus_video로부터 하나의 프레임을 읽어 bus_image에 저장
		bus_video >> bus_image;
		//bus_image가 전달받은 이미지가 없다면
		if (bus_image.empty())
		{
			std::cout << "video end" << std::endl;
			break;
		}

		//opencv로 받아온 이미지는 BGR 형태로 저장되므로 마지막 플래그를 BFR888로 전달하여 이미지 저장
		ui.mainLabel->setPixmap(QPixmap::fromImage(QImage(bus_image.data, bus_image.cols, bus_image.rows, bus_image.step, QImage::Format_BGR888)));
		//비디오 프레임 카운트 변수, 생략해도 상관없다
		frame_cnt++;
		//비디오 프레임 출력
		std::cout << "Frame: " << frame_cnt << "\n";
		//비디오를 읽어올 지연 속도를 waitKey로 지정
		//33으로 지정한 것은 약 30fps 속도로 출력하기 위함
		char c = (char)waitKey(33);
		//이미지가 변경됬으므로 mainwindow의 컴포넌트를 refrash하기 위해 show 함수 호출
		this->show();
		// esc 키 입력이 들어오면 while 종료구문
		// QT event 형식으로 변경 예정
		if (c == 27)
			is_play = false;
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