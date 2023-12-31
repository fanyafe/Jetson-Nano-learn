//Jetson nano use csi  camera

#include <iostream>
#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/objdetect.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>
#include <opencv4/opencv2/videoio.hpp>
 
using namespace std;
using namespace cv;
 
string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
           to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}
 
int main( int argc, char** argv )
{
    int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 1280 ;
    int display_height = 720 ;
    int framerate = 60 ;
    int flip_method = 0 ;
 
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
    if(!cap.isOpened())
    {
        std::cout<<"打开摄像头失败."<<std::endl;
        return (-1);
    }
 
    //创建显示窗口
    namedWindow("CSI Camera", WINDOW_AUTOSIZE);
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
 
 
        //调整图像大小
        new_width=640;
        if(width>800)
          {
             new_height=int(new_width*1.0/width*height);
           }
         resize(img, img, cv::Size(new_width, new_height));
 
        imshow("CSI Camera",img);
 
        int keycode = cv::waitKey(30) & 0xff ; //ESC键退出
            if (keycode == 27) break ;
    }
 
    cap.release();
    destroyAllWindows() ;
}
// run  this code 
//1.    g++ csi_camera.cpp -o test `pkg-config --cflags --libs opencv4`
//2.    ./test
