#include "musicplayerdecoderthread.h"
#include<QDebug>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libswresample/swresample.h"
}
#define MAX_AUDIO_FRAME_SIZE 19200
MusicPlayerDecoderThread::MusicPlayerDecoderThread(QObject *parent) : QThread(parent)
{

}

MusicPlayerDecoderThread::~MusicPlayerDecoderThread()
{

}

void MusicPlayerDecoderThread::setAudioPath(const QString &path)
{
    m_strAudioPath=path;
}


void MusicPlayerDecoderThread::run()
{
    //ffmpeg一般的解码流程
    //1.注册组件,完成封装上下文的初始化
    av_register_all();
    avformat_network_init();//* Do global initialization of network libraries. This is optional,and not recommended anymore.
    AVFormatContext *format_ctx=avformat_alloc_context();
    //2.获取音频文件路径，并打开文件,返回值为0
    std::string path=m_strAudioPath.toStdString();
    if(avformat_open_input(&format_ctx,path.c_str(),NULL,NULL)!=0){
        qDebug()<<"Can not open the file!!";
        return;
    }
    av_dump_format(format_ctx, 0, path.c_str(), 0); //输出概要信息
    //3.获取音频信息，主要是找到需要用的音频流
    if(avformat_find_stream_info(format_ctx,NULL)<0){
        qDebug()<<"Can not find audio info!!";
        return;
    }
    int audio_stream_index=-1;
    for(int i=0;i<format_ctx->nb_streams;i++){
        //从混合流中找到音频流的位置
        if(format_ctx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
            audio_stream_index=i;
            break;
        }
    }
   //4.获取解码器
    AVCodecParameters *codec_para=format_ctx->streams[audio_stream_index]->codecpar;
    AVCodec *codec_id=avcodec_find_decoder(codec_para->codec_id);
    if(codec_id==0){
        qDebug()<<"Con not find codec!!";
        return;
    }
    AVCodecContext *codec_ctx=avcodec_alloc_context3(codec_id);
    avcodec_parameters_to_context(codec_ctx,codec_para);

    if(avcodec_open2(codec_ctx,codec_id,NULL)<0){
        qDebug()<<"Can not open codec!!";
        return;
    }
    //5.设置输出的格式
    uint64_t out_channel_layout = codec_ctx->channel_layout;//？？？
    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;//格式
    int out_sample_rate = codec_ctx->sample_rate;
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);
    uint8_t *out_buffer = (uint8_t *)av_malloc(MAX_AUDIO_FRAME_SIZE*2);

    SwrContext *swr_ctx = swr_alloc_set_opts(NULL, out_channel_layout, out_sample_fmt,out_sample_rate, codec_ctx->channel_layout, codec_ctx->sample_fmt, codec_ctx->sample_rate, 0, 0);
    swr_init(swr_ctx);

    AVFrame *frame = av_frame_alloc();
    AVFrame *frame_rgb = av_frame_alloc();
    AVPacket *pkt = av_packet_alloc();
    av_init_packet(pkt);

    while (av_read_frame(format_ctx, pkt) >= 0)
    {
        if (pkt->stream_index == audio_stream_index)
        {
            if (pkt->pts != AV_NOPTS_VALUE)
            {
                qDebug("video pts = %f\n", pkt->pts * av_q2d(codec_ctx->time_base));
            }
            else
            {
                qDebug("video pts = AV_NOPTS_VALUE\n");
            }

            int ret = avcodec_send_packet(codec_ctx, pkt);
            if ( ret < 0)
            {
                continue;
            }

            while (avcodec_receive_frame(codec_ctx, frame) >= 0)
            {
                int len = swr_convert(swr_ctx, &out_buffer, MAX_AUDIO_FRAME_SIZE, (const uint8_t **)frame->data, frame->nb_samples);
                if (len <= 0)
                {
                    continue;
                }
                int dst_bufsize = av_samples_get_buffer_size(0, out_channels, len, out_sample_fmt, 1);
                QByteArray ba((const char *)out_buffer, dst_bufsize);
                emit get_One_Frame(ba);
            }
        }

        av_packet_unref(pkt);
    }

    av_packet_free(&pkt);
    av_frame_free(&frame);
    av_frame_free(&frame_rgb);
    avcodec_close(codec_ctx);
    avformat_close_input(&format_ctx);
}
