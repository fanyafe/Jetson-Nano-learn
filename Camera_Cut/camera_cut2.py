import argparse
import os
import cv2
 
def parse_args():
    """
    Parse input arguments
    """
    parser = argparse.ArgumentParser(description='Process pic')
    parser.add_argument('--input', help='video to process', dest='input', default=None, type=str)
    parser.add_argument('--output', help='pic to store', dest='output', default=None, type=str)
    #default为间隔多少帧截取一张图片
    parser.add_argument('--skip_frame', dest='skip_frame', help='skip number of video', default=1, type=int)
    #input为输入视频的路径 ，output为输出存放图片的路径
    args = parser.parse_args(['--input',r'/home/fan/Desktop/shipin/12.mp4','--output',r'/home/fan/Desktop/shipin'])
    return args
 
def process_video(i_video, o_video, num):
    cap = cv2.VideoCapture(0)
    # VideoCapture()中的参数若为0，则表示打开笔记本的内置摄像头
    # 若为视频文件路径，则表示打开视频
 
    num_frame = cap.get(cv2.CAP_PROP_FRAME_COUNT) # 获取视频总帧数
    # print(num_frame)
 
    expand_name = '.jpg'
    if not cap.isOpened():
        print("Please check the path.")
 
    cnt = 0 
    count = 0
    while 1:
        ret, frame = cap.read()
        # cap.read()表示按帧读取视频。ret和frame是获取cap.read()方法的两个返回值
        # 其中，ret是布尔值。如果读取正确，则返回TRUE；如果文件读取到视频最后一帧的下一帧，则返回False
        # frame就是每一帧的图像
 
        if not ret:
            break
 
        cnt += 1 # 从1开始计帧数
        gray_img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        img1 = cv2.resize(gray_img, (288, 288))
 
        if cnt % num == 0: # num表示每隔num帧进行一次裁剪
            count += 1
            cv2.imwrite(os.path.join(o_video, str(count) + expand_name), img1)
 
 
if __name__ == '__main__':
    args = parse_args()
    if not os.path.exists(args.output):
        os.makedirs(args.output)
    print('Called with args:')
    print(args)
    process_video(args.input, args.output, args.skip_frame)
