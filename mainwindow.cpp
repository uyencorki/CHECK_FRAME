#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include <QTimer>
#include "ui_mainwindow.h"
#include <QDir>
#include <QDebug>
#include <QElapsedTimer>
#include <QThread>
#include <QDateTime>
#include <QPlainTextEdit>
#include <QDirIterator>
#include <QDir>


#include <string>               // for strings
#include <iomanip>              // for controlling float print precision
#include <sstream>              // string to number conversion
#include <opencv2/core.hpp>     // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>  // OpenCV window I/O


using namespace cv;
using namespace std;

int check_video = 1;
string file_name;

bool VIDEO_FILE = true;
bool UVC_CAM = false;

#define Multi_Thread 1
#define Slider_playback 1


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    ui->label_screen->setText("Please insert file name and check video buttom");

    //tao 1 cap moi


    //  Or, if you are inside an AppImage package:

//        if (check_video == 1)
//        {
//             cap = new VideoCapture();
//             cout << "open file " << endl;
//             cap->open(file_name);

//             cap->open(0);
//             Timer_process = new QTimer(this);
//             connect(Timer_process,SIGNAL(timeout()),this, SLOT(Process()));
//             Timer_process->start(0);

//             //set time loop cho open cam
//             Timer_get_cam = new QTimer(this);
//             QPixmap pm("C:/Users/uyen/Documents/Laplace_Find_2/HinoApp.PNG"); // <- path to image file
//         }
//         else
//         {
//             ui->label_screen->setText("Please insert file name and check video buttom");
//         }

 #if Multi_Thread

//    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);
//    while (iterator.hasNext()) {
//             iterator.next();
//                 QFile file(iterator.next());
//                  if ( file.open( QIODevice::ReadOnly ) ) {
//                        qDebug() << QFileInfo(file).fileName() << endl;
//                  } else {
//                      qDebug() << "Can't open " << QFileInfo(file).fileName() << endl;
//                  }
//          }
#endif

}

MainWindow::~MainWindow()
{
    delete ui;
    if(cap->isOpened()) Reset_Camera();
}



//Ham xu ly chinh. Get every frame depend on Timer_process
//#########################################################################
int time_check_camera = 500; // milisecond
int count_U = 0;
int capture = 0;

bool record = false;
int create_file = 0;
 VideoWriter video;


int cnt_frame = 0;

void yuv_transform(unsigned char *src_y,unsigned char *src_c,unsigned char *dst,unsigned int w,unsigned int h)
{
    int w_align,h_align,w_mbs,h_mbs;
    int flag;
    int i,j,m;
    unsigned char *ybuf,*cbuf,*yuvbuf;
    unsigned char *ubuf,*vbuf;
    ybuf   = src_y;
    cbuf   = src_c;
    yuvbuf = dst;


    unsigned int base[256] = {
        0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,
        4,4,4,4,4,4,4,4, 5,5,5,5,5,5,5,5,
        6,6,6,6,6,6,6,6, 7,7,7,7,7,7,7,7,
        0,0,0,0,0,0,0,0, 1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2, 3,3,3,3,3,3,3,3,
        4,4,4,4,4,4,4,4, 5,5,5,5,5,5,5,5,
        6,6,6,6,6,6,6,6, 7,7,7,7,7,7,7,7,

        8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,
        10,10,10,10,10,10,10,10, 11,11,11,11,11,11,11,11,
        12,12,12,12,12,12,12,12, 13,13,13,13,13,13,13,13,
        14,14,14,14,14,14,14,14, 15,15,15,15,15,15,15,15,
        8,8,8,8,8,8,8,8, 9,9,9,9,9,9,9,9,
        10,10,10,10,10,10,10,10, 11,11,11,11,11,11,11,11,
        12,12,12,12,12,12,12,12, 13,13,13,13,13,13,13,13,
        14,14,14,14,14,14,14,14, 15,15,15,15,15,15,15,15,
    };
    unsigned int offset[256] = {
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,

        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,

        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7, 0,1,2,3,4,5,6,7,

        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
        8,9,10,11,12,13,14,15, 8,9,10,11,12,13,14,15,
    };
    unsigned int pos[256];

    unsigned int cbase[64] = {
        0,0,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,
        2,2,2,2,2,2,2,2,
        3,3,3,3,3,3,3,3,
        4,4,4,4,4,4,4,4,
        5,5,5,5,5,5,5,5,
        6,6,6,6,6,6,6,6,
        7,7,7,7,7,7,7,7,
    };
    unsigned int coffset[64] = {
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
        0,1,2,3,4,5,6,7,
    };
    unsigned int cpos[64];

    w_align = (w + 15)&(~15);
    h_align = (h + 15)&(~15);

    w_mbs = w_align / 16;
    h_mbs = h_align / 16;

    for (i = 0 ; i < 256;i ++)
    {
        pos[i] = base[i] * w_align + offset[i];
    }
    for (i = 0 ; i < 64;i ++)
    {
        cpos[i] = cbase[i] * w_align / 2 + coffset[i];
    }

    memset(yuvbuf,0,w_align * h_align *3 /2);
    unsigned char *data;
    unsigned int offset_num;

    // Y
    for (i = 0;i < h_mbs ;i ++)
    {
        data = ybuf + i * w_mbs * 256;
        offset_num = i * w_mbs * 256;
        for (j = 0;j < w_mbs;j ++)
        {
            for (m = 0; m <256;m++)
            {
                *(yuvbuf + offset_num + pos[m]) = *(data + m);
            }
            data += 256;
            offset_num += 16;
        }
    }

    // UV
    ubuf = yuvbuf + w_align*h_align;
    vbuf = yuvbuf + w_align*h_align + w_align*h_align/4;
    for (i = 0;i < h_mbs ;i ++)
    {
        data = cbuf + i * w_mbs * 128;
        offset_num = i * w_mbs * 64;
        for (j = 0;j < w_mbs;j ++)
        {
            for (m = 0; m < 64;m++)
            {
                *(ubuf + offset_num + cpos[m]) = *(data + m);
                *(vbuf + offset_num + cpos[m]) = *(data + 64 + m);
            }
            data += 128;
            offset_num += 8;
        }
    }


}


Scalar getMSSIM( const Mat i1, const Mat i2)
{
    const double C1 = 6.5025, C2 = 58.5225;
    /***************************** INITS **********************************/
    int d = CV_32F;
    Mat I1, I2;
    i1.convertTo(I1, d);            // cannot calculate on one byte large values
    i2.convertTo(I2, d);
    Mat I2_2   = I2.mul(I2);        // I2^2
    Mat I1_2   = I1.mul(I1);        // I1^2
    Mat I1_I2  = I1.mul(I2);        // I1 * I2

    /*************************** END INITS **********************************/
    Mat mu1, mu2;                   // PRELIMINARY COMPUTING
    GaussianBlur(I1, mu1, Size(11, 11), 1.5);
    GaussianBlur(I2, mu2, Size(11, 11), 1.5);
    Mat mu1_2   =   mu1.mul(mu1);
    Mat mu2_2   =   mu2.mul(mu2);
    Mat mu1_mu2 =   mu1.mul(mu2);
    Mat sigma1_2, sigma2_2, sigma12;
    GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
    sigma1_2 -= mu1_2;
    GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
    sigma2_2 -= mu2_2;
    GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
    sigma12 -= mu1_mu2;
    Mat t1, t2, t3;
    t1 = 2 * mu1_mu2 + C1;
    t2 = 2 * sigma12 + C2;
    t3 = t1.mul(t2);                 // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);                 // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))
    Mat ssim_map;
    divide(t3, t1, ssim_map);        // ssim_map =  t3./t1;
    Scalar mssim = mean(ssim_map);   // mssim = average of ssim map
    return mssim;
}

int first_frm = 0;
Mat before_img;
qint64 start_time  = 0 ;
QElapsedTimer timer;
#define check_file 0

QString H2644_file_path = QDir::currentPath();
char file_path_264[300];
unsigned int file_count = 0;



void MainWindow::Process(){

    ui->label_5->setText( QString::number(ui->horizontalSlider->value()));
    Mat image;
    QPixmap pm("C:/Users/uyen/Documents/Laplace_Find_2/HinoApp.PNG"); // <- path to image file
    QStringList lines = ui->plainTextEdit->toPlainText().split('\n', QString::SkipEmptyParts);

    if (lines.at(file_count).toStdString().size() == 0 )
           {
                return;
           }

//    GetCamera(image);

    if(GetCamera(image)==false) {

        file_count ++;
        cnt_frame = 0;

        file_name = H2644_file_path.toStdString() + "/VIDEO/" + lines.at(file_count).toStdString() ;
        cap = new VideoCapture();
        memcpy( file_path_264, file_name.c_str() ,file_name.size());
        file_path_264[file_name.size()] = 0;


#if Multi_Thread
        cout << "111" << endl;
#else
        cap->open(file_path_264 );
#endif


       QThread::sleep(2);

    }
    else
    {
        if(Timer_get_cam->isActive()==true) Timer_get_cam->stop();
    }


    char set_text [50] ;
    memcpy(set_text, lines.at(file_count).toStdString().c_str(), lines.at(file_count).toStdString().size() );
    set_text[lines.at(file_count).toStdString().size()]  = 0;

    ui->label_3->setText( set_text);
    //  cnt_frame
    ui->frm_nuber->setText(QString::number(cnt_frame));
    //Pause
    if(Camera_Pause==false) mat_image_for_pause = image.clone();
    image = mat_image_for_pause.clone();

    if(image.empty())
    {
        ui->label_screen->setText("No Camera 2");
        return;
    }
    //camera not work

    //Update Camera size
    ui->label_Resolution->setText(QString::number(size_wid) + " x " + QString::number(size_hei));

    //Frame count
    if(FRAME_COUNT_CHECK==true)
    {
        //frame rate
        fps_count++;
        if(fps_count==30)
        {
            double time_total = timer_fps_check.elapsed();
            double frame_rate = fps_count*1000/(time_total);
            ui->cam_fps->setText(QString::number(frame_rate, 'f', 2));
            fps_count = 0;
            timer_fps_check.start();
        }
    }



// check similar image
      cnt_frame ++;
      Scalar mssimV;
      Mat resized_cur_img;
      double per_R, per_G, per_B;

      // save file
//      QString cur_path = QDir::currentPath();
//      cur_path = cur_path + "/save_image/UVC_FRAME__" +QString::number(cnt_frame) + ".jpg";
//      char file_path[300];
//      memcpy( file_path, cur_path.toStdString().c_str() ,cur_path.size());
//      file_path[cur_path.size()] = 0;

//      cout << "path " <<  cur_path.toStdString() << endl;
//      cout << "file_path 1111  " << file_path << endl;
//      char file_path_2 [300];
//      char file_name [300] ;
//      sprintf(file_path_2,"D:/HUU_UYEN/Software/QT_creator/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release/save_image/UVC_FRAME__2");
//      QByteArray ba = cur_path.toLocal8Bit();
//      const char *c_str2 = ba.data();
//      printf("str2: %s \n", c_str2) ;
//      sprintf(file_name,"D:/HUU_UYEN/Software/QT_creator/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release/save_image/%s%d.jpg","UVC_FRAME__",cnt_frame);
//      sprintf(file_name,"D:/HUU_UYEN/Software/QT_creator/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release/save_image/%s%d.jpg","UVC_FRAME__",cnt_frame);
//      sprintf(file_name,"C:/Users/Hino_Thanh/Desktop/UVC_OPEN_APP_RELEASE/RELEASE_HN/save_image/%s%d.jpg","UVC_FRAME__",cnt_frame);


      cv::resize(image, resized_cur_img, Size(480,270), INTER_LINEAR);
//      imshow("resized_down", resized_cur_img);



//      cout << "resized_cur_img.type()"<< resized_cur_img.type() << endl ;
//      cout << "resized_cur_img.channels()"<< resized_cur_img.channels() << endl ;
//    first_frm = 0;

      if (first_frm == 0 )
      {
          before_img = resized_cur_img;
          first_frm = 1;
      }

     mssimV = getMSSIM(before_img, resized_cur_img);
     before_img = resized_cur_img;
     per_R =  mssimV.val[2] * 100;
     per_G =  mssimV.val[1] * 100;
     per_B =  mssimV.val[0] * 100;


     if (cnt_frame == 1)
     {
         timer.start();
     }




     if ((per_R < 90 || per_G < 90 || per_B < 90 ) || cnt_frame == 10)
     {
//         cout << " MSSIM: "<< endl;
//         cout << " R " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[2] * 100 << "%" << endl;
//         cout << " G " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[1] * 100 << "%" << endl;
//         cout << " B " << setiosflags(ios::fixed) << setprecision(2) << mssimV.val[0] * 100 << "%" << endl;

         // save file
         int time_sec = 0;
         int time_min = 0 ;
         time_sec = timer.nsecsElapsed() / 1000000000 ;
         QString cur_path = QDir::currentPath();
         char file_path[300];

#if check_file
         if (time_sec >= 60 )
         {
             time_min = time_sec/60;


             time_sec = time_sec%60;

             cur_path = cur_path + "/save_image/" + lines.at(file_count).toStdString().c_str() + "__"  +QString::number(cnt_frame) + "_time_" +
                       QString::number(time_min) + "M"+ QString::number(time_sec) +  "S.jpg";
             memcpy( file_path, cur_path.toStdString().c_str() ,cur_path.size());
             file_path[cur_path.size()] = 0;
         }
         else
         {
             cur_path = cur_path + "/save_image/"+lines.at(file_count).toStdString().c_str() + "__"  +QString::number(cnt_frame) + "_time_" +
                              QString::number(time_sec) +  "S.jpg";
             memcpy( file_path, cur_path.toStdString().c_str() ,cur_path.size());
             file_path[cur_path.size()] = 0;
         }

         if (cnt_frame > 3)
            imwrite(file_path, image);

#else

         QLocale testLocale = QLocale(QLocale::English, QLocale::Japan);
//         QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "dddd/dd/MM/yyyy/hh:mm:ss AP");
         QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "yyyy_MM_dd_hh_mm_ss");
         cout <<  "dateTimeText " << dateTimeText.toStdString() << endl;

         dateTimeText = cur_path + "/save_image/" + dateTimeText + "_FRM_"+ QString::number(cnt_frame) + ".jpg";

         memcpy(file_path, dateTimeText.toStdString().c_str(),dateTimeText.size());
         file_path [dateTimeText.size()]  = 0 ;
         cout << " file_path last " << file_path << endl;

         imwrite(file_path, image);


#endif

     }



    count_U ++ ;

    if (capture == 1 )
    {
        //imwrite(path, image); // A JPG FILE IS BEING SAVED
        capture = 0;
    }

    Display (image);



}




//#########################################################################
// Ham ho tro hien thi
//#########################################################################




void MainWindow::Open_cam()
{
   cout << "check camera" << endl;
    Reset_Camera();
   // open_cam_source(cap);

    if(camera_source.type==type_camera_source_string) camera_source.check_lock = true; // neu la string thi chi can check 1 lan thoi
}
void MainWindow::Reset_Camera()
{
     cout << "check camera" << endl;
    cap->release();
    cap->~VideoCapture();
}



bool MainWindow::GetCamera(Mat &image)
{
    image.release();
    cap->set(CV_CAP_PROP_FRAME_WIDTH,1920);
    cap->set(CV_CAP_PROP_FRAME_HEIGHT,1088);

    cap->read(image);

    if (image.empty()) return false;

    cv::resize(image,image,Size(1920,1080));

    size_wid = image.cols;
    size_hei = image.rows;


    if (record == true)
       {
        if (create_file == 0)
        {
            cout << "creat file video" << endl;
            VideoWriter video("video.avi",CV_FOURCC('D','I','V','X'),30, Size(1920,1088),true);

            create_file = 1;
        }
       // VideoWriter video("video.avi",CV_FOURCC('D','I','V','X'),30, Size(1920,1088));  // ,true);
       // cout << "save file video " << endl;

        video.write(image);

        //imshow("MyVideo", image);
        }


    return true;
}


void MainWindow::Display(Mat image)
{
    //resize de ra kich thuoc chuan
    bool resize = true;
    if(resize == true)
    {
        int x, y;
        // convert gia tri
        if((size_hei*ui->label_screen->width())>(size_wid*ui->label_screen->height()))
        {
            y = ui->label_screen->height();
            x = y*size_wid/size_hei;
        }
        else{
            x = ui->label_screen->width();
            y = x*size_hei/size_wid;
        }

        cv::resize(image,image,Size(x,y));
    }

    QPixmap p = Mat2QPixmap(image);
    ui->label_screen->setPixmap(p);
}



QPixmap MainWindow::Mat2QPixmap(cv::Mat const& _frame)
{
    switch( _frame.type())
    {
    case CV_8UC4:
    {
        QPixmap p = QPixmap::fromImage(QImage(_frame.data,_frame.cols,_frame.rows,_frame.step,QImage::Format_RGB32));

        return p;
    }
    case CV_8UC3:
    {
        QPixmap p = QPixmap::fromImage(QImage(_frame.data,_frame.cols,_frame.rows,_frame.step,QImage::Format_RGB888).rgbSwapped());
        return p;
    }
    case CV_8UC1:
    {
        QPixmap p = QPixmap::fromImage(QImage(_frame.data,_frame.cols,_frame.rows,_frame.step,QImage::Format_Indexed8));
        return p;
    }
        break;
    }
    return QPixmap();

}




void MainWindow::on_Capture_clicked()
{
    capture = 1;

}

void MainWindow::on_ReOpen_cam_clicked()
{

    if (UVC_CAM == true)
    {
        cap = new VideoCapture();
        cap->open(0);
        cout << "open file " << endl;
        Timer_process = new QTimer(this);
        connect(Timer_process,SIGNAL(timeout()),this, SLOT(Process()));
        Timer_process->start(0);
        //set time loop cho open cam
        Timer_get_cam = new QTimer(this);
        QPixmap pm("C:/Users/uyen/Documents/Laplace_Find_2/HinoApp.PNG"); // <- path to image file
    }
    else
    {
        ui->label_screen->setText("Please, Check UVC_CAM");
    }



}

int cnt_record = 0;
void MainWindow::on_Record_video_clicked()
{
    cnt_record ++ ;
    record = !record;
    if (cnt_record  == 2 )
    {
          create_file = 0;
    }
}


Mat rgbImage_before;
#define h264_check 1
int g_slider_position = 0;
CvCapture* video_capture = NULL;

void onTrackbarSlide(int current_frame)
{
    current_frame = g_slider_position;
    cvSetCaptureProperty(video_capture,CV_CAP_PROP_POS_FRAMES,current_frame);
}


void MainWindow::on_Check_video_clicked()
{



//#if  Slider_playback
#if  0
    Mat image;
    cap = new VideoCapture();

    cap->open("D:/HUU_UYEN/Software/QT_creator/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release/VIDEO/file_1.264" );
    GetCamera(image);

    cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );
    video_capture = cvCreateFileCapture( "VIDEO/chn_352_288_03_09_2.mp4");   // file_1.264  chn_352_288_03_09_2.mp4
    video_capture = cvCreateFileCapture( "VIDEO/file_1.264");
    int no_of_frames = (int) cvGetCaptureProperty(video_capture,CV_CAP_PROP_FRAME_COUNT);
    cout << "no_of_frames = " << no_of_frames << endl ;
    if( no_of_frames != 0 )
    {
       cvCreateTrackbar("Slider","Video",&g_slider_position,no_of_frames,onTrackbarSlide);
    }

    IplImage* frame;

        while(1)
        {
            frame = cvQueryFrame( video_capture );
            if( !frame ) break;
            cvShowImage( "Video", frame );
            //cv::imshow("Video", image);
            cvSetTrackbarPos("Slider","Video",++g_slider_position);
            char c = cvWaitKey(33);
            if( c == 27 ) break;
        }


//        cvReleaseCapture( &video_capture );
//        cvDestroyWindow( "Video" );


#endif


//#if Multi_Thread
#if 0

        //  QStringList lines = ui->plainTextEdit->toPlainText().split('\n', QString::SkipEmptyParts);

        file_name = H2644_file_path.toStdString() + "/VIDEO/" ;
        memcpy( file_path_264, file_name.c_str() ,file_name.size());
        file_path_264[file_name.size()] = 0;
        cout << "file_path_264 " << file_path_264 << endl;

        QDir dir;
        QDirIterator iterator(file_path_264 );
//      QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);

        QRegExp rx("(\\ |\\,|\\/|\\:|\\t)");                //RegEx for ' ' or ',' or '.' or ':' or '\t'

        QRegExp RegEx("(\\ |\\,|\\/|\\:|\\t)");                //RegEx for ' ' or ',' or '.' or ':' or '\t'

        QStringList query;
        QString file_name_1 ;
        QString file_name_2 = "hinoeng" ;

        QString total_file ;
        int len = 0;

        while (iterator.hasNext())
        {
            QFile file(iterator.next());
            if ( file.open( QIODevice::ReadOnly ) )
            {
                file_name_1 = file.fileName().toUtf8();
                query = file.fileName().split(rx);
                QStringList list1 = file_name_1.split('.');
                cout << "query = " << query[ query.length() - 1].toStdString()  << endl;
               // ui->plainTextEdit->setPlainText(query[ query.length() - 1]);


                file_name_2 =  query[ query.length() - 1] + "\n";

                total_file = total_file + file_name_2;


                ui->plainTextEdit->setPlainText(file_name_2);

//                file_name_2 =  list1[0].toUtf8();
//                QStringList list_2 = file_name_2.split('/');
                cout << endl;
            }
            else
                qDebug() << "Can't open " << file.fileName() << file.errorString() << endl;
          }


        ui->plainTextEdit->setPlainText(total_file);
        cout << "total file name " << total_file.toStdString() << endl;

#endif




#if h264_check
    QStringList lines = ui->plainTextEdit->toPlainText().split('\n', QString::SkipEmptyParts);

    file_name = H2644_file_path.toStdString() + "/VIDEO/" + lines.at(0).toStdString() ;
    cap = new VideoCapture();

    memcpy( file_path_264, file_name.c_str() ,file_name.size());
    file_path_264[file_name.size()] = 0;

    cout << "file_path_264 " << file_path_264 << endl;
    cap->open(file_path_264 );


     Timer_process = new QTimer(this);
     connect(Timer_process,SIGNAL(timeout()),this, SLOT(Process()));
     Timer_process->start(0);

       //set time loop cho open cam
     Timer_get_cam = new QTimer(this);
     QPixmap pm("C:/Users/uyen/Documents/Laplace_Find_2/HinoApp.PNG"); // <- path to image file


#else

  // YUV part

     qint64 SIZE_OF_YUV420p_FRAME = 152064;
     QByteArray  sendBuffer;
     int frame_cnt = 0 ;
     Mat rgbImage;
     Mat resized_cur_img ;
     Scalar mssimV;
     int per_R = 0, per_G = 0 , per_B = 0;

     QFile file("D:/HUU_UYEN/Software/QT_creator"
                "/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release"
                "/VIDEO/chn_352_288_03_09_OK.yuv");    //chn_352_288_03_09_OK.yuv"   kantool_video.264



     Mat rgbImage_1, mask, backtorgb;




     VideoWriter outputVideo; // For writing the video

     int width = 352; // Declare width here
     int height = 288; // Declare height here
     Size S = Size(width, height); // Declare Size structure

     // Open up the video for writing
     const string filename = "D:/HUU_UYEN/Software/QT_creator"
                             "/build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release"
                             "/VIDEO/chn_352_288_03_09_OK.yuv"; // Declare name of file here

     // Declare FourCC code - OpenCV 2.x
     // int fourcc = CV_FOURCC('H','2','6','4');
     // Declare FourCC code - OpenCV 3.x and beyond
     int fourcc = VideoWriter::fourcc('H','2','6','4');

     // Declare FPS here
     double fps = 25;
     outputVideo.open(filename, fourcc, fps, S);


     if(file.open(QIODevice::ReadOnly))
         {
            while (!file.atEnd())
            {
                  frame_cnt ++;

                  sendBuffer = file.read(SIZE_OF_YUV420p_FRAME); //capture one frame
                  Mat yuvImg = cv::Mat(288+288/2,352,CV_8UC1,sendBuffer.data()); //translating QByteArray to Mat
                  cvtColor(yuvImg,rgbImage,COLOR_YUV420p2RGB);// converting to RGB

//                cout << "yuvImg.type()"<< yuvImg.type() << endl ;
//                cout << "yuvImg.type()"<< yuvImg.channels() << endl ;

                  rgbImage.convertTo(mask,CV_8UC1, 256);

//                cout << "mask.type()"<< mask.type() << endl ;
//                cout << "mask.type()"<< mask.channels() << endl ;
//                imshow ("mask ", mask);

//                 cvtColor(yuvImg,rgbImage,COLOR_YUV420p2RGB);// converting to RGB

//                 cvtColor(rgbImage, mask, CV_RGB2GRAY);

//                 cvtColor(mask,backtorgb,COLOR_GRAY2RGB);

//                 imshow ("backtorgb", backtorgb);

//                  rgbImage = mask;

//                  cvtColor(yuvImg,rgbImage,COLOR_YUV420p2RGB);// converting to RGB

//                  /*rgbImage*/.convertTo(CV_8UC3)
//                  rgbImage.convertTo(rgbImage_1, CV_8UC1);

//                  cout << "image.type()"<< rgbImage.type() << endl ;
//                  cout << "image.type()"<< rgbImage.channels() << endl ;



//                Display(rgbImage);
                  cv::resize(rgbImage, resized_cur_img, Size(480,270), INTER_LINEAR);
//                imshow("bgr",rgbImage);
//                imshow ("resized_cur_img", resized_cur_img) ;


                  QPixmap p1;
                  QPixmap p = Mat2QPixmap(resized_cur_img);
                  ui->label_screen->setPixmap(p);

                  waitKey(30);
                  cout << "frame_cnt " << frame_cnt << endl;



                 if (frame_cnt > 1)
                    mssimV = getMSSIM(rgbImage_before ,resized_cur_img );


                 if (frame_cnt == 1)
                 {
                     rgbImage_before = resized_cur_img ;
                     imwrite ("D:/HUU_UYEN/Software/QT_creator/"
                              "build-Camera_Example-Desktop_Qt_5_14_2_MinGW_32_bit-Release/"
                              "save_image/firt_frame_rgbImage_before.jpg", rgbImage_before);
                 }

                  per_R =  mssimV.val[2] * 100;
                  per_G =  mssimV.val[1] * 100;
                  per_B =  mssimV.val[0] * 100;

                  cout << "per_R = " << per_R << "; per_G = " << per_G << "; per_B = " << per_B << endl ;
                  cout << "frame_cnt = " << frame_cnt << endl;


                  if (per_R < 90 || per_G < 90 || per_B < 90 )
                  {
                      cout << "deferen image " << frame_cnt << endl;


                      char file_path[300];

                      QString cur_path = QDir::currentPath();
                      QLocale testLocale = QLocale(QLocale::English, QLocale::Japan);
             //       QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "dddd/dd/MM/yyyy/hh:mm:ss AP");
                      QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "yyyy_MM_dd_hh_mm_ss");
                      cout <<  "dateTimeText " << dateTimeText.toStdString() << endl;

                      dateTimeText = cur_path + "/save_image/" + dateTimeText + "_FRM_"+ QString::number(frame_cnt)+".jpg";
                      memcpy(file_path, dateTimeText.toStdString().c_str(),dateTimeText.size());
                      file_path [dateTimeText.size()]  = 0 ;

                      cout << " file_path last " << file_path << endl;
                      imwrite(file_path, resized_cur_img);


                  }

//                  cout << " file_path last " << file_path << endl;
//                  if (frame_cnt > 50 && frame_cnt < 55  )
//                  {
//                      char file_path[300];
//                      QString cur_path = QDir::currentPath();
//                      QLocale testLocale = QLocale(QLocale::English, QLocale::Japan);
//             //       QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "dddd/dd/MM/yyyy/hh:mm:ss AP");
//                      QString dateTimeText = testLocale.toString(QDateTime::currentDateTime(), "yyyy_MM_dd_hh_mm_ss");
//                      cout <<  "dateTimeText " << dateTimeText.toStdString() << endl;
//                      dateTimeText = cur_path + "/save_image/" + dateTimeText + "_FRM_"+ QString::number(frame_cnt)+".jpg";
//                      memcpy(file_path, dateTimeText.toStdString().c_str(),dateTimeText.size());
//                      file_path [dateTimeText.size()]  = 0 ;
//                      cout << " file_path last " << file_path << endl;
//                      imwrite(file_path, resized_cur_img);
//                  }
            }

         }
         file.close();

 #endif

}

void MainWindow::on_VIDEO_FILE_stateChanged(int arg1)
{
    VIDEO_FILE= !VIDEO_FILE;
    cout << "video file " << VIDEO_FILE << endl;
}

void MainWindow::on_UVC_CAM_stateChanged(int arg1)
{
    UVC_CAM = !UVC_CAM;
    cout << "UVC_CAM " << UVC_CAM << endl;
}

void MainWindow::on_horizontalSlider_actionTriggered(int action)
{
    ui->label_5->setText(QString::number(ui->horizontalSlider->value()));





}

