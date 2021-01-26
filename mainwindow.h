#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTime>
#include <QElapsedTimer>

#include <QCloseEvent>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void closeEvent(QCloseEvent *event);

private:

    QTimer *Timer_process;
    QElapsedTimer timer_check;

    Ui::MainWindow *ui;

    VideoCapture cap;
    enum camera_source_type_df{
        type_camera_source_number,
        type_camera_source_string,
    };

    struct camera_source_define{
        int type = type_camera_source_number;

        int source_number = 0;
        string source_link = "";
    }camera_source;
    void open_cam_source(VideoCapture cap);


    int dem_time;

    Mat mat_image_for_pause;
    bool Camera_Pause = false;

    int size_wid, size_hei;


    bool OpenCamera;

public slots:
    void Process();
    void Display(Mat image);
    bool GetCamera(Mat &image);
//    bool GetCameraxx(Mat &image);

protected:
 static   QPixmap Mat2QPixmap(cv::Mat const& _frame);

private slots:
 void on_pushButton_Open_clicked();
 void on_pushButton_Pause_clicked();
};

#endif // MAINWINDOW_H
