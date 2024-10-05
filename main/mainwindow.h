#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QMediaPlayer>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool played;
    bool Muted=false;
    std::vector<QUrl> workingdirectory;
    std::string folder_path;
    std::string home_dir = "/home/pain/Music/";
    int reload_stats=1;


private slots:
    void switchTosecondLayer(std::string folder_path);
    void switchTothirdLayer(int index);
    void MPL(int c);
    void update_thirdlayer();
    void sliderposition(qint64 position);
    void songposition(qint64 duration);
    void songprogress(int position);



private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedwidget;
    QWidget *firstlayer;
    QWidget *secondlayer;
    QWidget *thirdlayer;
    QMediaPlayer *media=nullptr;
    QLabel *album_image;
    QLabel *song_name;
    QSlider *slider;
    QAudioOutput *audiooutput;
    int song_number;
};
#endif