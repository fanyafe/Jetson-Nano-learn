// jetson nano taking screenshots with usb camera

#include <iostream>
#include <string>
#include <time.h>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include <opencv4/opencv2/videoio.hpp>
 
using namespace std;
using namespace cv;
 
//选择图片保存位置
string writePath = "/home/fan/Downloads/camera/6.camera_cut/picture/";
 
int main(int argc, char** argv)
{
      VideoCapture cap(0);                              // 捕获摄像头
      string name;                                      // 用于命名图片
      namedWindow("USB Camera", WINDOW_AUTOSIZE);        
      Mat img;
      int i = 0;           // 用于定时
      int photoname = 0;   // 用于图片命名
      while (true)
      {     
            if (!cap.read(img))              // 判断是否捕捉到画面
            {
                  std::cout<<"捕获失败"<<std::endl;
                  break;
            }
            // 调整画面宽高等
            int new_width,new_height,width,height,channel;
            width=img.cols;
            height=img.rows;
            channel=img.channels();
            new_width=800;
            if(width>800)
            {
                  new_height=int(new_width*1.0/width*height);
            }
            resize(img, img, cv::Size(new_width, new_height));
            imshow("USB Camera",img);        // 显示画面
            
            i = i + 1;
            if (i == 10)   // 计数到10，截取图片
            {
                  photoname++;
                  name = writePath + to_string(photoname)+".jpg"; // 图片命名
                  imwrite(name, img);                             // 保存图片
                  cout << name << endl;
                  cout << "保存" << name << "成功" << endl;
                  i = 0;
            }
            int keycode = cv::waitKey(30) & 0xff ; //ESC键退出
                  if (keycode == 27) break ;
      }
      cap.release();
      destroyAllWindows();
     //waitKey(0);
}
// run this code
//1. g++ -o camera camera_cut.cpp `pkg-config --cflags --libs opencv4`
//2. ./camera
