// 使用前要先配置jetson nano 的c++ GPIO库。采用两个按键控制摄像头开关，按键控制引脚分别连接GPIO16 ，GPIO18。配置库如下链接

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
 
int a = 0;
 
static bool end_this_program = false;
 
inline void delay(int s) { this_thread::sleep_for(chrono::seconds(s)); }
 
void signalHandler(int s) { end_this_program = true; }
 
int main()
{
    // When CTRL+C pressed, signalHandler will be called
    signal(SIGINT, signalHandler);
 
    // Pin Definitions
    //int a = 0;
    int but1_pin = 18; // BOARD pin 18
    int but2_pin = 16; //BOARD Pin 16
 
    // Pin Setup.
    GPIO::setmode(GPIO::BOARD);
 
    // set pin as an output pin with optional initial state of LOW
    
    GPIO::setup(but1_pin, GPIO::IN);
    GPIO::setup(but2_pin, GPIO::IN);
    int exit_value = GPIO::input(but2_pin);
    cout << "Starting demo now! Press CTRL+C to exit" << endl;
 
    while (!end_this_program)
    {
        cout << "Waiting for button event" << endl;
        if(a==0)
        {
           // 等待下降沿，按键按下程序继续，不按下程序停止在这里  
	       GPIO::wait_for_edge(but1_pin, GPIO::Edge::FALLING);
	       cout << "Button Pressed!" << endl;
	       //打开摄像头
               //flag1:
               VideoCapture cap(1);
 
               //创建显示窗口
               namedWindow("USB Camera", WINDOW_AUTOSIZE);
               Mat img;
 
               //逐帧显示
               while(true)
               {
                   if (!cap.read(img))
                   {
                       std::cout<<"捕获失败"<<std::endl;
                       break;
                   }
                   int new_width,new_height,width,height,channel;
                   width=img.cols;
                   height=img.rows;
                   channel=img.channels();
                   cout<<width<<"  "<<height<<"  "<<channel<<endl;
 
                   new_width=800;
                   if(width>800)
                   {
                        new_height=int(new_width*1.0/width*height);
                   } 
 
                   resize(img, img, cv::Size(new_width, new_height));
                   imshow("USB Camera",img);
                   //cout << "value1=" << value1
                   int exit_value = GPIO::input(but2_pin);
                   //cout << "value1=" << exit_value <<endl;
                   // 按键按下停止程序
		               if (exit_value == 0)
                   {
                       //cap.release();
                       //destroyAllWindows();
                       //cout << "value1=" << exit_value <<endl;                  
                       break;
                   }
                   int keycode = cv::waitKey(30) & 0xff ; //ESC键退出
                   if (keycode == 27) break ;
               }
               cap.release();
               destroyAllWindows(); 
               break; 
        } 	                                                   
    }
    //cout << "value1=" << exit_value <<endl;
    GPIO::cleanup();
    
    return 0;
}
