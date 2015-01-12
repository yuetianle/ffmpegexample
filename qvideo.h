#ifndef QVIDEO_H
#define QVIDEO_H
#include <QString>
extern "C"
{
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libswscale/swscale.h>
    #include <SDL2/SDL.h>
    #include <SDL2/SDL_thread.h>
}
#include "vqthread.h"
#include "audio.h"

class QVideo
{
public:
    QVideo(const QString &file_name);
    ~QVideo();

   AVFormatContext *av_format_context_;
   AVCodecContext *av_codec_context_;
    void Init(AVFormatContext *av_format_context = nullptr);
    void UnInit(AVFormatContext *av_format_context = nullptr);
    bool StartPlay(const QString &file_name, AVFormatContext *av_format_context/*, AVCodecContext *avcodec_context*/);
    bool StopPlay();
    bool Pause();
private:
   VQThread video_thread;
   VAudioThread audio_thread;

   AVCodec *av_codec_;
  /* enum QERROR{
       QEOPEN = -1000,
   }*/
private:
   bool OpenFile(const QString &file_name, AVFormatContext *av_format_context);
   bool CloseFile(const QString &file_name, AVFormatContext *av_format_context);
   bool PlayVideo();
   bool PlayAudio();
   bool Stop();
   //bool Pause();
};

#endif // QVIDEO_H
