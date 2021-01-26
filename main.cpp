#include "mainwindow.h"

#include <QApplication>


#if 1
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
#else
#include <iostream>

cv::Mat image;
int main(int, char**) {
    cv::VideoCapture vcap;



    const std::string videoStreamAddress = "http://192.168.0.42:8080/?action=stream";

    const string h264_video = "C:/Users/Hino_Thanh/Desktop/winftp/h264_video.264";   //C:/Users/Hino_Thanh/Desktop/winftp/h264_video.264

    if(!vcap.open(h264_video)) {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    int demmm = 0;
    QString save_link;
    for(;;) {
        if(!vcap.read(image)) {
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }
        cv::imshow("Output Window", image);

        demmm++;
//        save_link = "save/image_" + QString::number(demmm) + ".jpg";

//        imwrite(save_link.toUtf8().constData(), image);

        cout << "dem = " << demmm << endl;
        if(cv::waitKey(1) >= 0) break;
    }

}

#endif
