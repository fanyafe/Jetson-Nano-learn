import cv2
import numpy as np
 
def main():
    print('开始运行')
    cap = cv2.VideoCapture(0)  # 捕获摄像头
    i = 0                      # 定时装置初始值
    photoname = 1              # 文件名序号初始值
 
    while True:
        i = i + 1
        reg, frame = cap.read()
        frame = cv2.flip(frame, 1)   # 图片左右调换
        cv2.imshow('window', frame)
 
        if i == 10:                 # 定时装置，定时截屏，可以修改。
            filename = str(photoname) + '.png'               # filename为图像名字，将photoname作为编号命名保存的截图
            cv2.imwrite('/home/fan/Downloads/camera/6.camera_cut/picture/' + '\\' + filename, frame)  # 截图，前面为图像存放位置
            print(filename + '保存成功')  # 打印保存成功
            i = 0                        # 清零
 
            photoname = photoname + 1
            if photoname >= 20:  # 最多截图20张 然后退出（如果调用photoname = 1 不用break为不断覆盖图片）
                # photoname = 1
                break
        if cv2.waitKey(1) & 0xff == ord('q'):
            break
    # 释放资源
    cap.release()
    cv2.destroyAllWindows()
if __name__ == '__main__':
    main()
