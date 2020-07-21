#include "QILT.h"

//opencv lib
#include <opencv2/core.hpp>		// basic OpenCV structures ( Mat , Scalar, etc... )
#include <opencv2/videoio.hpp>	// for VideoCapture, VideoWriter
#include <opencv2/highgui.hpp>	// OpenCV Window IO
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
void video_mouse_callback(int event, int x, int y, int flags, void* user_data) {
	cout << "Cur Event : " << event << " Flags : " << flags << " Cur x : " << x << " Cur y : " << y << endl;

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
	else if (EVENT_MOUSEHWHEEL)	//10
	{
		int wheel_val = 0;
		if (getMouseWheelDelta(flags) > 0)
			wheel_val++;
		else
			wheel_val--;
	}
}

QILT::QILT(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	//���� ��θ� ���������� �ƴ� �Է¿� ���� ������ ��� �Ʒ� �ּ��� ������ �ȴ�
//cout << "input file path\n";
//cin >> video_path;
//cout << "file path is " << video_path << endl;
//video_path�� ���� ���� ������ �����´�. CAP_FFMPEG�� ���� ������ �о���ų� FFPMEG ���̺귯���� �̿��� ��Ʈ������ ����
//�ܼ� ������ �о�� ��� CAP_FFMPEG �÷��׸� �����ص� ��� ����
	VideoCapture bus_video(video_path);

	//bus_video�� ����� ���� ���Ϸκ��� ������ �̹����� �޾ƿ� Mat ���� ����
	Mat bus_image;

	//video_path�� ���� ���� ������ �������� ���� ���
	if (!bus_video.isOpened())
	{
		std::cout << "Could not open video" << video_path << std::endl;
	}

	//�̹����� ����� â�� �ĺ��� ���� ���� �� �ʱ�ȭ
	const char* WIN_TITLE = "BUS Video";
	//�̹����� ����� â�� ����
	namedWindow(WIN_TITLE, WINDOW_AUTOSIZE);

	//while�� ���� bus_video�κ��� ������ �̹����� �о��
	while (true)
	{
		//bus_video�κ��� �ϳ��� �������� �о� bus_image�� ����
		bus_video >> bus_image;

		//bus_image�� ���޹��� �̹����� ���ٸ�
		if (bus_image.empty())
		{
			std::cout << "video end" << std::endl;
			break;
		}
		//���� ������ ī��Ʈ ����, �����ص� �������
		frame_cnt++;
		//���� ������ ���
		std::cout << "Frame: " << frame_cnt << "\n";
		//bus_video�κ��� �о�� �̹����� WIN_TITLE â�� ���
		imshow(WIN_TITLE, bus_image);
		//������ �о�� ���� �ӵ��� waitKey�� ����
		//33���� ������ ���� �� 30fps �ӵ��� ����ϱ� ����
		char c = (char)waitKey(33);
		// esc Ű �Է��� ������ while ����
		if (c == 27)
			break;
	}
}
