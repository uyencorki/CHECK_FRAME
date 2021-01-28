#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include <QTimer>
#include "ui_mainwindow.h"

/*
* Chuong trinh toi uu hoa khi quit van khong bi treo
* Ho tro open by id (number) va bang stream link (string)
* Khi stream bang link, thong thuong khong the set fps
*/

using namespace cv;
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //tao 1 cap moi
    cap = new VideoCapture();

    //set time loop cho Process
    Timer_process = new QTimer(this);
    connect(Timer_process,SIGNAL(timeout()),this, SLOT(Process()));
    Timer_process->start(0);

    //set time loop cho open cam
    Timer_get_cam = new QTimer(this);
    connect(Timer_get_cam,SIGNAL(timeout()),this, SLOT(Open_cam()));


}
MainWindow::~MainWindow()
{
    delete ui;
//    Reset_Camera();
}



//Ham xu ly chinh. Get every frame depend on Timer_process
//#########################################################################
int time_check_camera = 500; // milisecond
void MainWindow::Process(){
    Mat image;

    if(GetCamera(image)==false) {

        ui->label_screen->setText("     No Camera");

        if(Timer_get_cam->isActive()==true) return;
        else Timer_get_cam->start(time_check_camera);

        return;
    }
    else {
        if(Timer_get_cam->isActive()==true) Timer_get_cam->stop();
    }
//    cout << "have data" << endl;

    //Pause
    if(Camera_Pause==false) mat_image_for_pause = image.clone();
    image = mat_image_for_pause.clone();


    if(image.empty()) {
        ui->label_screen->setText("    No Camera");
        return;
    }//camera not work

    //Update Camera size
    ui->label_Resolution->setText(QString::number(size_wid) + " x " + QString::number(size_hei));

    //Frame count
    if(FRAME_COUNT_CHECK==true)
    {
        //frame rate
        fps_count++;

        if(fps_count==100)
        {
            double time_total = timer_fps_check.elapsed();
            double frame_rate = fps_count*1000/(time_total);

            ui->label_framerate->setText(QString::number(frame_rate));

            fps_count = 0;
            timer_fps_check.start();
        }
    }

    Display(image);
}

//#########################################################################



// Ham ho tro hien thi
//#########################################################################
void MainWindow::Open_cam(){
    cout << "check camera" << endl;

    //neu open bang link thi khong can phai mo lai lam gi. Se khien chuong trinh bi treo
    if((camera_source.type==type_camera_source_string)&&(camera_source.check_lock==true)) {

        ui->label_screen->setText("     Please check camera source link again!");
        Timer_get_cam->stop();
        return;
    }

    Reset_Camera();
    open_cam_source(cap);

    if(camera_source.type==type_camera_source_string) camera_source.check_lock = true; // neu la string thi chi can check 1 lan thoi
}
void MainWindow::Reset_Camera()
{
    cap->release();
    cap->~VideoCapture();
//    cap = new VideoCapture();
}
//Luu y la khi open cac link stream -> neu link khon dung se dan den corrupt. Viec open thuong se rat lau
//Cach khac phuc -> tu build lai opencv ffmpeg from github  -> https://www.google.com/search?sxsrf=ALeKk02UqOCyAHCiBSRO31vwNevqIHHNkQ%3A1611039560909&ei=SIMGYOqJN8GsoATX7YKIDA&q=OpenCV+VideoCapture+timeout+http+rtsp&oq=OpenCV+VideoCapture+timeout+http+rtsp&gs_lcp=CgZwc3ktYWIQAzIICCEQFhAdEB46BggjECcQEzoGCAAQFhAeOgQIABATOggIABAWEB4QEzoFCCEQoAE6BwghEAoQoAFQzi5YlTxgnT9oAHAAeAGAAZ8DiAGFD5IBBzAuOS40LTGYAQCgAQGqAQdnd3Mtd2l6wAEB&sclient=psy-ab&ved=0ahUKEwjqzfyFtqfuAhVBFogKHde2AMEQ4dUDCA0&uact=5
void MainWindow::open_cam_source(VideoCapture *cap){
    if(camera_source.type==type_camera_source_number) {
        cap->open(camera_source.source_number);
    }
    else if(camera_source.type==type_camera_source_string) {
        cap->open(camera_source.source_link);
    }
}
bool MainWindow::GetCamera(Mat &image)
{
    image.release();

     cap->read(image);
//    cap->operator >>(image);
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
    if((size_hei*ui->label_screen->width())>(size_wid*ui->label_screen->height())){
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
//#########################################################################



//Chi thay doi mau sac va test
//#########################################################################
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
//#########################################################################



//Cac nut nhan button
//#########################################################################
void MainWindow::on_pushButton_Open_clicked()
{
    QString camera_source_tmp = ui->lineEdit_camera_source->text();
    //check xem la number hay la link
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


    //release
    Reset_Camera();

    //unlock check
    camera_source.check_lock = false;
}

void MainWindow::on_pushButton_Pause_clicked()
{
    Camera_Pause = !Camera_Pause;
    Button_change_color_state(ui->pushButton_Pause, Camera_Pause);
}

void MainWindow::on_pushButton_Frame_clicked()
{
    FRAME_COUNT_CHECK = !FRAME_COUNT_CHECK;
    Button_change_color_state(ui->pushButton_Frame, FRAME_COUNT_CHECK);

    timer_fps_check.start();
    fps_count = 0;
}

void MainWindow::on_pushButton_Res_clicked()
{
    Reset_Camera();
    open_cam_source(cap);
    cap->set(CAP_PROP_FRAME_WIDTH, ui->lineEdit_camera_width->text().toInt());
    cap->set(CAP_PROP_FRAME_HEIGHT, ui->lineEdit_camera_height->text().toInt());
}
//#########################################################################
