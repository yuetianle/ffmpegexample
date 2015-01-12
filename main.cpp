#include <QCoreApplication>
#include <iostream>
#include "qvideo.h"
using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
   QString file_name = argv[1];
    QVideo *video = new QVideo(file_name);
    video->Init();
    video->StartPlay(file_name, video->av_format_context_);
    video->StopPlay();
    video->UnInit();
    return a.exec();
}
