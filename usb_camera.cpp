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
 
int main( int argc, char** argv )
{
    //打开摄像头
    VideoCapture cap(1);    //使用 ls /dev/vid*查看使用的摄像头
 
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
 
        int keycode = cv::waitKey(30) & 0xff ; //ESC键退出
            if (keycode == 27) break ;
    }
 
    cap.release();
    destroyAllWindows() ;
}

// run this code 
//1.    g++ usb_camera.cpp -o test `pkg-config --cflags --libs opencv4`
//2.    ./test
