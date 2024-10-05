#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <fstream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <filesystem>
#include <QVBoxLayout>
#include <QScrollArea>
#include <vector>
#include <QAudioOutput>
#include <QFileInfo>
#include <QSlider>

QList button_list = {"BMute","BBack","BPause","BNext","BReturn","BReload"};
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    stackedwidget = new QStackedWidget(this);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    firstlayer = new QWidget(this);
    QGridLayout *firstlayout = new QGridLayout(firstlayer);

    QWidget *scrollWidget = new QWidget(this);
    scrollWidget->setLayout(firstlayout);
    scrollArea->setWidget(scrollWidget);

    int x, y, i = 0;
    
    for (auto const & folder:std::filesystem::directory_iterator(home_dir))
    {
        QPushButton *button = new QPushButton(QString::fromStdString(folder.path().filename().string()));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        button->setMinimumSize(100, 150);

        x = i / 2;
        y = i % 2;
        firstlayout->addWidget(button, x, y);

        connect(button, &QPushButton::clicked, [this, folder]()
        {
            switchTosecondLayer(folder.path().filename().string());
            stackedwidget->setCurrentIndex(2);
        });
        i++;
    }
    
    if (i < 8)
    {
        for (int j = 0; j < 8 - i; j++)
        {
            QLabel *fixingscale = new QLabel(this);
            fixingscale->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            fixingscale->setMinimumSize(100, 150);
            fixingscale->setText(" ");
            firstlayout->addWidget(fixingscale);
        }
    }
    firstlayout->update();
    

    stackedwidget->addWidget(scrollArea);
    stackedwidget->addWidget(ui->centralwidget);
    setCentralWidget(stackedwidget);

    stackedwidget->setCurrentWidget(scrollArea);
    this->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::switchTosecondLayer(std::string folder_name){
    folder_path = folder_name;


    secondlayer = new QWidget(this);
    QGridLayout *secondlayout = new QGridLayout(secondlayer);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    QWidget *scrollWidget = new QWidget(this);
    scrollWidget->setLayout(secondlayout);

    scrollArea->setWidget(scrollWidget);

    int i = 0;
    workingdirectory.clear();
    for (const auto &file : std::filesystem::directory_iterator(home_dir + folder_path))
    {   
        workingdirectory.push_back(QUrl(QString::fromStdString(file.path().string())));
        std::string ext = file.path().extension();
        if (ext == ".mp3" || ext==".m4a")
        {
            QString file_path = QString::fromStdString(file.path().string());
            const std::string &file_name = file.path().stem().string();

            QPushButton *bname = new QPushButton(QString::fromStdString(file_name), this);
            bname->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::MinimumExpanding);
            bname->setMinimumSize(100, 100);

            secondlayout->addWidget(bname, i, 0);
            for(int i=0;i<workingdirectory.size();i++){
                if(workingdirectory[i]==QUrl(file_path)){
                    break;
                }
            }
            connect(bname, &QPushButton::clicked, [this, i]()
            {
                switchTothirdLayer(i);
                stackedwidget->setCurrentIndex(1);
            });
        }
        i++;
    }

 
    if (i < 8)
    {
        for (int j = 0; j < 8 - i; j++)
        {
            QLabel *fixingscale = new QLabel(this);
            fixingscale->setText(" ");
            fixingscale->setMinimumSize(100, 500);
            secondlayout->addWidget(fixingscale, i, 0);
        }
    }

    secondlayout->update();
    stackedwidget->addWidget(scrollArea);
    stackedwidget->setCurrentWidget(scrollArea);
}



void MainWindow::switchTothirdLayer(int index)
{   
    ui->centralwidget->update();
    song_number = index;
    if (media==nullptr){
        for(int i=0;i<button_list.size();i++){
            QString  bname = button_list[i];
            QIcon *icon = new QIcon("cache/icons/"+bname+".png");
            QPushButton *button = this->findChild<QPushButton *>(bname);
            disconnect(button,nullptr,this,nullptr);
            button->setIcon(*icon);
            button->setIconSize(QSize(button->size().width()/1.5,button->size().height()/1.5));
            connect(button,&QPushButton::clicked,[this,i](){
                MPL(i);
            });
        }
    }
    update_thirdlayer();

 
}




void MainWindow::MPL(int c)
{   
    switch(c){
        case 0:
            if (!Muted){audiooutput->setVolume(0),ui->BMute->setIcon(QIcon("cache/icons/BAnMute.png"));}
            else{audiooutput->setVolume(1);ui->BMute->setIcon(QIcon("cache/icons/BMute.png"));}
            Muted=!Muted;
            break;
        case 1:
            if(song_number>0){
                song_number-=1;
                media->stop();
                update_thirdlayer();
            }

            break;
        case 2:
            if(played){media->pause();ui->BPause->setIcon(QIcon("cache/icons/BContinue.png"));}
            else{media->play();ui->BPause->setIcon(QIcon("cache/icons/BPause.png"));}
            played=!played;

            break;
        case 3:
            if(song_number<workingdirectory.size()-1){
                song_number+=1;
                media->stop();
                update_thirdlayer();
            }
            break;
        case 4:
            switchTosecondLayer(folder_path);
            break;
        case 5:
        //onereload BReload shuffeling 
            switch(reload_stats){
                case 0:
                    ui->BReload->setIcon(QIcon("cache/icons/BReload.png"));
                    break;
                case 1:
                    ui->BReload->setIcon(QIcon("cache/icons/shuffling.png"));
                    break;
                case 2:
                    ui->BReload->setIcon(QIcon("cache/icons/onereload.png"));
                    break;
                
            }  
            
            reload_stats=(reload_stats+1)%3;
            break;
    }
}



void MainWindow::update_thirdlayer(){
    slider = ui->Slider;
    album_image = ui->album_image;
    song_name = ui->Song_Name;
    std::string file_name = workingdirectory[song_number].fileName().split('.').first().toStdString();
    std::string imagepath = "cache/images/"+file_name+".png";
    if (!std::filesystem::exists("cache/images/"+file_name+".png")){
        imagepath = "cache/images/default.png";
    }
    QPixmap image(QString::fromStdString(imagepath));
    QPixmap scaledimage = image.scaled(album_image->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    
    album_image->setPixmap(scaledimage);
    if (file_name.size()>16){
        song_name->setText(QString::fromStdString(file_name.erase(15,-1)+"..."));
    }else{
    song_name->setText(QString::fromStdString(file_name));
    }
    if(media!=nullptr){delete media;}
    media = new QMediaPlayer(this);
    audiooutput=new QAudioOutput(this);
    connect(media,&QMediaPlayer::positionChanged,this,&MainWindow::sliderposition);
    connect(media,&QMediaPlayer::durationChanged,this,&MainWindow::songposition);
    connect(slider,&QSlider::sliderMoved,this,&MainWindow::songprogress);

    media->setAudioOutput(audiooutput);
    media->setSource(workingdirectory[song_number]);
    audiooutput->setVolume(!Muted);
    played=true;
    media->play();

}
void MainWindow::sliderposition(qint64 position){
    slider->setValue(position);
    QLabel *sr = ui->song_runtime;
    int total = media->duration()/1000;
    int current = position/1000;
    int total_min = total/60;
    int total_second=total-total_min*60;
    int current_min = current/60;
    int current_second=current-current_min*60;
    std::ostringstream oss_current_min, oss_current_second, oss_total_min, oss_total_second;
    oss_current_min << std::setfill('0') << std::setw(2) << current_min;
    oss_current_second << std::setfill('0') << std::setw(2) << current_second;
    oss_total_min << std::setfill('0') << std::setw(2) << total_min;
    oss_total_second << std::setfill('0') << std::setw(2) << total_second;

    sr->setText(QString::fromStdString(oss_current_min.str()) + ":" + QString::fromStdString(oss_current_second.str()) +
            "/" + QString::fromStdString(oss_total_min.str()) + ":" + QString::fromStdString(oss_total_second.str()));
    if (current==total){
        if (song_number==workingdirectory.size()-1 && reload_stats==1){
            song_number=0;
        }
        else if(reload_stats==2){
            srand(time(NULL));
            song_number = rand()%workingdirectory.size()-1;
        }
        else if(reload_stats==1){
            song_number +=1;

        }
        update_thirdlayer();

    }
}

void MainWindow::songposition(qint64 duration){
    slider->setRange(0,duration);
}
void MainWindow::songprogress(int position){
    media->setPosition(position);
}
