#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTime>
#include <QElapsedTimer>

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

private:

    QTimer *Timer_process, *Timer_get_cam;
    QElapsedTimer timer_check;

    Ui::MainWindow *ui;

    VideoCapture *cap;
    enum camera_source_type_df{
        type_camera_source_number,
        type_camera_source_string,
    };

    struct camera_source_define{
        int type = type_camera_source_number;

        int source_number = 0;
        string source_link = "";

        bool check_lock = false;
    }camera_source;
    void open_cam_source(VideoCapture *cap);


    int dem_time;

    Mat mat_image_for_pause;
    bool Camera_Pause = false;

    int size_wid, size_hei;


    //FPS
    bool FRAME_COUNT_CHECK = true;
    QElapsedTimer timer_fps_check;
    int fps_count = 0;

public slots:
    void Process();
    void Open_cam();

protected:
    void Reset_Camera();
    void Display(Mat image);
    bool GetCamera(Mat &image);
    static   QPixmap Mat2QPixmap(cv::Mat const& _frame);

private slots:
//    void on_pushButton_Open_clicked();
//    void on_pushButton_Pause_clicked();
//    void on_pushButton_Frame_clicked();
//    void on_pushButton_Res_clicked();
    void on_Capture_clicked();
    void on_ReOpen_cam_clicked();
    void on_Record_video_clicked();
    void on_Check_video_clicked();
    void on_VIDEO_FILE_stateChanged(int arg1);
    void on_UVC_CAM_stateChanged(int arg1);
    void on_horizontalSlider_actionTriggered(int action);
};

#endif // MAINWINDOW_H
