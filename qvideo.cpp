#include "qvideo.h"

QVideo::QVideo(const QString &file_name)
{

}

QVideo::~QVideo()
{

}

void QVideo::Init(AVFormatContext *av_format_context)
{
    //avcodec_init();
    //avcodec_register_all();
    av_register_all();
    avformat_network_init();
    av_format_context_ = avformat_alloc_context();
}

void QVideo::UnInit(AVFormatContext *av_format_context)
{
    avformat_network_deinit();
    if(nullptr != av_format_context_)
    {
        avformat_free_context(av_format_context_);
    }
}

bool QVideo::StartPlay(const QString &file_name, AVFormatContext *av_format_context/*, AVCodecContext *avcodec_context_lsd*/)
{
   if (!OpenFile(file_name, av_format_context) || nullptr == av_format_context)
   {
      return false;
   }
   else
   {
        int ret = avformat_find_stream_info(av_format_context, nullptr);
        if(0 > ret)
        {
            return false;
        }
        int video_index = -1;
        for (int index = 0; index < av_format_context->nb_streams; index++)
        {
            if(AVMEDIA_TYPE_VIDEO == av_format_context->streams[index]->codec->codec_type)
            {
                video_index = index;
            }
        }
        if(0 > video_index)
        {
            return false;
        }
        AVCodecContext *avcodec_context = nullptr;
        avcodec_context = av_format_context->streams[video_index]->codec;
        AVCodec *avcodec = nullptr;
        avcodec = avcodec_find_decoder(avcodec_context->codec_id);
        if (nullptr == avcodec)
        {
            return false;
        }
        ret = avcodec_open2(avcodec_context, avcodec, nullptr);
        if( 0 > ret)
        {
            return false;
        }
        AVFrame *avframe = nullptr;
        AVFrame *avframe_YUV = nullptr;
        avframe = avcodec_alloc_frame();
        avframe_YUV = avcodec_alloc_frame();
        u_int8_t *output_buffer = nullptr;
        int num_bytes = avpicture_get_size(PIX_FMT_YUV420P\
                                           , avcodec_context->width\
                                           , avcodec_context->height);
        output_buffer =  (u_int8_t*)av_malloc[sizeof(u_int8_t)*num_bytes];
        avpicture_fill((AVPicture*)avframe_YUV\
                       , output_buffer, PIX_FMT_YUV420P\
                       , avcodec_context->width, avcodec_context->height);
        /*SDL */
        if (SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_TIMER))
        {
            return false;
        }
        //SDL_Surface *screen = nullptr;
        //screen = SDL_SetSurfaceColorMod();
        //SDL_SetVideoMode();
        SDL_Window *windows = nullptr;
        windows = SDL_CreateWindow("XPlayer", SDL_WINDOWPOS_CENTERED\
                                   , SDL_WINDOWPOS_CENTERED, avcodec_context->width\
                                   , avcodec_context->height, SDL_WINDOW_SHOWN);
        if(!windows)
        {
            //count << SDL_GetError() << endl;
            return false;
        }
        SDL_Renderer *render = nullptr;
        render = SDL_CreateRenderer(windows ,  -1\
                                    ,  SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
        if(!render)
        {
            return false;
        }
        SDL_Texture *texture = nullptr;
        texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_YV12\
                                    , SDL_TEXTUREACCESS_STREAMING, avcodec_context->width\
                                    , avcodec_context->height);
        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.w = avcodec_context->width;
        rect.h = avcodec_context->height;
        /*SDL*/
        AVPacket packet;
        av_new_packet(&packet, num_bytes);
        av_dump_format(av_format_context, 0, file_name.toStdString().c_str(), 0);

        static struct SwsContext *img_convert_context = nullptr;
        img_convert_context = sws_getContext(avcodec_context->width, avcodec_context->height\
                                             , avcodec_context->pix_fmt, avcodec_context->width\
                                             , avcodec_context->height, PIX_FMT_YUV420P, SWS_BICUBIC\
                                             , nullptr, nullptr, nullptr);
        int frame_finished = -1;
        while(av_read_frame(av_format_context, &packet) >= 0)
        {
            if(video_index == packet.stream_index)
            {
               ret = avcodec_decode_video2(avcodec_context, avframe, &frame_finished, &packet);
               if( 0 <= ret)
               {
                    if(frame_finished)
                    {
                        sws_scale(img_convert_context, (const u_int8_t* const*)avframe->data\
                                  , avframe->linesize, 0, avcodec_context->height, avframe_YUV->data\
                                  ,avframe_YUV->linesize);
                        SDL_UpdateYUVTexture(texture, &rect, avframe_YUV->data[0]\
                                , avframe_YUV->linesize[0], avframe_YUV->data[1]\
                                , avframe_YUV->linesize[1], avframe_YUV->data[2]\
                                , avframe_YUV->linesize[2]);
                        SDL_RenderClear(render);
                        SDL_RenderCopy(render, texture, &rect, &rect);
                        SDL_RenderPresent(render);
                    }
                    SDL_Delay(50);
               }
               else
               {
                   //log out
                   return false;
               }
            }
        }
        av_free_packet(&packet);
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type) {
        case SDL_QUIT:
        {
            SDL_Quit();
            exit(0);
            break;
        }
        default:
            break;
        }
        SDL_DestroyTexture(texture);
        av_frame_free(&avframe);
        av_frame_free(&avframe_YUV);
        avcodec_close(avcodec_context);
        CloseFile(file_name, av_format_context);
   }
}

bool QVideo::StopPlay()
{
    return true;
}

bool QVideo::OpenFile(const QString &file_name, AVFormatContext *av_format_context)
{
   if(file_name.isEmpty())
   {
       return false;
   }
   else
   {
        int ret = avformat_open_input(&av_format_context, file_name.toStdString().c_str(),nullptr,nullptr);
        return 0 == ret ?true:false;
   }
}

bool QVideo::CloseFile(const QString &file_name, AVFormatContext *av_format_context)
{
    if(nullptr != av_format_context)
    {
        avformat_close_input(&av_format_context);
    }
}

bool QVideo::PlayVideo()
{

}

bool QVideo::PlayAudio()
{

}


