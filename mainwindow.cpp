#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include <QTimer>
#include "ui_mainwindow.h"

/*      Notice
    Create new label and new button
    Set label at "ignored" mode of "sizePolicy" setting.
    Tick on "scaleContents"
*/

using namespace cv;
using namespace std;
std::string videoStreamAddress;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);

//    cap = new VideoCapture();

    videoStreamAddress = "udp://@192.168.0.48:1234"; //http://192.168.0.42:8080/?action=stream"; //"http://192.168.0.42:8080/?action=stream"; //"udp://@:1234"; // "C:/Users/Hino_Thanh/Desktop/winftp/mjpeg_video.avi" //h264_video.264"; //"http://192.168.0.42:8080/?action=stream";

//    cap.open(videoStreamAddress, CAP_FFMPEG);
//    cap.open(0);
    camera_source.source_link = "http://192.168.0.42:8080/?action=stream";

//    cap.set(CAP_PROP_FRAME_WIDTH, 512);
//    cap.set(CAP_PROP_FRAME_HEIGHT, 288);

//    Timer_process = new QTimer(this);
//    connect(Timer_process,SIGNAL(timeout()),this, SLOT(Process()));
//    Timer_process->start(0);

    OpenCamera = true;


}

MainWindow::~MainWindow()
{
    delete ui;
//    destroyAllWindows();

//    cap.release();
//    cap.~VideoCapture();
}

void MainWindow::closeEvent(QCloseEvent *event) {

//    if(Timer_process->isActive()) {
//        Timer_process->stop();
//        cout << "XXXXXXXXXX" << endl;
//    }

//    cap.release();
//    cap.~VideoCapture();

    cout << "EXIT" << endl;
}

//Ham xu ly chinh. Get every frame depend on Timer_process
int frame_count = 0;
void MainWindow::Process(){
    if(OpenCamera==false) return;


    return;
    Mat image;

    if(GetCamera(image)==false) {

        ui->label->setText("     No Camera");

        if(dem_time==0) {
            timer_check.start();
            dem_time = 1;
        }

        // check camera every 0.5s
        int gettime_for_camera_check = timer_check.elapsed();

        if((gettime_for_camera_check>500)) {
            cap.release();
            cap.~VideoCapture();

    //        cap = new VideoCapture();

            open_cam_source(cap);
            dem_time = 0;
        }
        return;
        //if(!Check_cam->isActive()) Check_cam->start(500);
    }

     cout << "have data" << endl;

    frame_count++;
    ui->label_3->setText("Frame : " + QString::number(frame_count));

    ui->label_2->setText(QString::number(size_wid) + " x " + QString::number(size_hei));
    if(image.empty()) {ui->label->setText("    No Camera");return;}//camera not work


    Display(image);
}

// Ham ho tro hien thi
//#########################################################################
void MainWindow::open_cam_source(VideoCapture cap){
    if(camera_source.type==type_camera_source_number) {
        cap.open(camera_source.source_number);
    }
    else if(camera_source.type==type_camera_source_string) {
        cap.open(camera_source.source_link);
    }
}
bool MainWindow::GetCamera(Mat &image)
{
    image.release();

     cap.read(image);
//    cap.operator >>(image);
    if (image.empty()) return false;

    size_wid = image.cols;
    size_hei = image.rows;

    return true;
}
void MainWindow::Display(Mat image)
{

    //resize de ra kich thuoc chuan
    bool resize = true;
    if(resize == true){
    int x, y;
    // convert gia tri
    if((size_hei*ui->label->width())>(size_wid*ui->label->height())){
        y = ui->label->height();
        x = y*size_wid/size_hei;
    }
    else{
        x = ui->label->width();
        y = x*size_hei/size_wid;
    }

    cv::resize(image,image,Size(x,y));
    }

    QPixmap p = Mat2QPixmap(image);
    ui->label->setPixmap(p);
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
//#########################################################################






//Chi thay doi mau sac va test
void Button_change_color_state(QPushButton *button, bool state, QString text_change_true, QString text_change_false){
    if(state==true) {
        if(text_change_true.length()!=0) button->setText(text_change_true);
        button->setStyleSheet("background-color: rgb(100,125,250)");
    }
    else {
        if(text_change_false.length()!=0) button->setText(text_change_false);
        button->setStyleSheet("background-color: rgb(200,200,200)");
    }
}
void Button_change_color_state(QPushButton *button, bool state){
    if(state==true) {
        button->setStyleSheet("background-color: rgb(100,125,250)");
    }
    else {
        button->setStyleSheet("background-color: rgb(200,200,200)");
    }
}


void MainWindow::on_pushButton_Open_clicked()
{
    QString camera_source_tmp = ui->lineEdit_camera_source->text();

    cout << camera_source_tmp.toUtf8().constData() << endl;

    bool camera_source_is_number = true;

    for(int i= 0; i< camera_source_tmp.length(); i++){
        int kq = isdigit(camera_source_tmp.toUtf8().constData()[i]);
        if(kq==0) {
                camera_source_is_number = false;
                break;
        }
    }

    if(camera_source_is_number==true){
        camera_source.type = type_camera_source_number;
        camera_source.source_number = atoi(ui->lineEdit_camera_source->text().toUtf8().constData());
        cout << "camera source is number " << camera_source.source_number << endl;
    }
    else {
        camera_source.type = type_camera_source_string;
        camera_source.source_link = ui->lineEdit_camera_source->text().toUtf8().constData();
        cout << "camera source is string " << camera_source.source_link << endl;
    }
}

void MainWindow::on_pushButton_Pause_clicked()
{
    Camera_Pause = !Camera_Pause;
    Button_change_color_state(ui->pushButton_Pause, Camera_Pause);
}
