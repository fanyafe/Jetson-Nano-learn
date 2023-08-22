#include <memory>
#include <thread>
#include <iostream>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include <opencv4/opencv2/videoio.hpp>
 
#include <chrono>
#include <signal.h>
#include <JetsonGPIO.h>
 
using namespace GPIO;
using namespace std;
using namespace cv;
 
static bool end_this_program = false;
 
inline void delay(int s)
{
	this_thread::sleep_for(chrono::seconds(s));
}
 
void signalHandler(int s)
{
	end_this_program = true;
}
// 创建CSI管道
string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
	return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
		to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
		"/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
		to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}
 
int main(int argc, char** argv)
{	
	int a=0;
	signal(SIGINT, signalHandler);
	int csi_to_usb = 16; 
	int usb_to_csi = 18; 
 
	GPIO::setmode(GPIO::BOARD);
 
	GPIO::setup(csi_to_usb, GPIO::IN);
	GPIO::setup(usb_to_csi, GPIO::IN);
	while(1)
	{
			if(a==0)
			{
				flag1:
				cv::VideoCapture cap(1);
				namedWindow("USB Camera", WINDOW_AUTOSIZE);
				Mat img;
					
				while (true)
				{	
					if (!cap.read(img))
					{
						std::cout << "未能成功调用摄像头" << std::endl;
						return -1;
					}
					int new_width, new_height, width, height, channel;
					width = img.cols;
					height = img.rows;
					channel = img.channels();
					cout << width << "  " << height << "  " << channel << endl;
					new_width = 800;
					if (width > 800)
					{
						new_height = int(new_width * 1.0 / width * height);
					}
					resize(img, img, cv::Size(new_width, new_height));
					cv::imshow("USB Camera", img);
					int exit_value1 = GPIO::input(usb_to_csi);
					if (exit_value1 == 0)
						break;
					//int keycode = cv::waitKey(1);
					int keycode = cv::waitKey(1) & 0xff;
					if (keycode == 27)
						break;
				}
				cap.release();
				destroyAllWindows();
				a = 1;		
			}
 
 
			else if (a==1)
			{
				flag2:
				int capture_width = 640;
				int capture_height = 480;
				int display_width = 640;
				int display_height = 480;
				int framerate = 25;
				int flip_method = 0;
				//创建管道
				string pipeline = gstreamer_pipeline(capture_width,
				capture_height,
				display_width,
				display_height,
				framerate,
				flip_method);
				std::cout << "使用gstreamer管道: \n\t" << pipeline << "\n";
				//管道与视频流绑定
				VideoCapture cap(pipeline, CAP_GSTREAMER);
				if (!cap.isOpened())
				{
					std::cout << "打开摄像头失败." << std::endl;
					return (-1);
				}
				namedWindow("CSI Camera", WINDOW_AUTOSIZE);
				Mat img;
				//逐帧显示
 
				while (true)
				{
					if (!cap.read(img))
					{
						std::cout << "捕获失败" << std::endl;
						break;
					}
					cv::imshow("CSI Camera", img);
					int exit_value2 = GPIO::input(csi_to_usb);
					if (exit_value2 == 0)
						break;
					int keycode = cv::waitKey(1) & 0xff;
					if (keycode == 27)
						break;
				}
				cap.release();
				destroyAllWindows();
				a = 0;	
			}
	}
	GPIO::cleanup();
	return 0;
}

//run this code
//1. g++ -o two_camera button_twoCamera.cpp -lJetsonGPIO -lpthread  `pkg-config --cflags --libs opencv4
//2. ./two_camera
