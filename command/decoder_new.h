/*
 * decoder_new.h: A program for the Video Disk Recorder
 *
 * See the README file for copyright information and how to reach the author.
 *
 */

#ifndef __decoder_new_h_
#define __decoder_new_h_

#include <vector>
#include "global.h"

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/file.h>
}

#define AVLOGLEVEL AV_LOG_ERROR

#if LIBAVCODEC_VERSION_INT >= ((58<<16)+(35<<8)+100)   // error codes from AC3 parser
#define AAC_AC3_PARSE_ERROR_SYNC         -0x1030c0a
#define AAC_AC3_PARSE_ERROR_BSID         -0x2030c0a
#define AAC_AC3_PARSE_ERROR_SAMPLE_RATE  -0x3030c0a
#define AAC_AC3_PARSE_ERROR_FRAME_SIZE   -0x4030c0a
#define AAC_AC3_PARSE_ERROR_FRAME_TYPE   -0x5030c0a
#define AAC_AC3_PARSE_ERROR_CRC          -0x6030c0a
#define AAC_AC3_PARSE_ERROR_CHANNEL_CFG  -0x7030c0a
#endif

// libavcodec versions of some distributions
// #if LIBAVCODEC_VERSION_INT >= ((58<<16)+(35<<8)+100)    Ubuntu 20.04
// #if LIBAVCODEC_VERSION_INT >= ((57<<16)+(107<<8)+100)   Ubuntu 18.04
// #if LIBAVCODEC_VERSION_INT >= ((57<<16)+(64<<8)+101)    Debian Stretch
// #if LIBAVCODEC_VERSION_INT >= ((56<<16)+(57<<8)+100)    Ubuntu 14.04
// #if LIBAVCODEC_VERSION_INT >= ((56<<16)+(26<<8)+100)    Debian Jessie
// #if LIBAVCODEC_VERSION_INT >= ((56<<16)+(1<<8)+0)       Rasbian Jessie


class cDecoder
{
    public:
        cDecoder(int threads);
        ~cDecoder();
        bool DecodeDir(const char * recDir);
        void Reset();
        AVFormatContext *GetAVFormatContext();
        AVCodecContext **GetAVCodecContext();
        bool DecodeFile(const char * filename);
        int GetVideoHeight();
        int GetVideoWidth();
        int GetVideoFramesPerSecond();
        int GetVideoRealFrameRate();
        bool GetNextFrame();
        AVPacket *GetPacket();
        bool SeekToFrame(long int iFrame);
        AVFrame *DecodePacket(AVFormatContext *avctx, AVPacket *avpkt);
        bool GetFrameInfo(MarkAdContext *maContext);
        bool isVideoStream(short int streamIndex);
        bool isVideoPacket();
        bool isVideoIFrame();
        bool isAudioAC3Packet();
        bool isAudioStream(short int streamIndex);
        bool isAudioPacket();
        long int GetFrameNumber();
        long int GetIFrameCount();
        bool isInterlacedVideo();
        long int GetIFrameRangeCount(long int beginFrame, long int endFrame);
        long int GetIFrameBefore(long int iFrame);
        long int GetTimeFromIFrame(long int iFrame);

    private:
        char *recordingDir = NULL;
        int fileNumber = 0;
        int threadCount = 0;
        AVFormatContext *avctx = NULL;
        AVPacket avpkt = {};
        AVCodec *codec = NULL;
        AVCodecContext **codecCtxArray = NULL;
        long int framenumber=-1;
        long int iFrameCount=0;
        int64_t pts_time_ms_LastFile=0;
        struct iFrameInfo
        {
            int fileNumber=0;
            long int iFrameNumber=0;
            int64_t pts_time_ms=0;
        };
        std::vector<iFrameInfo> iFrameInfoVector;
        struct structFrameData {
            bool Valid=false; // flag, if true data is valid
            uchar *Plane[4] = {};  // picture planes (YUV420)
            int PlaneLinesize[4] = {}; // size int bytes of each picture plane line
        } iFrameData;
        bool msgDecodeFile=true;
        bool msgGetFrameInfo=true;
        int interlaced_frame=-1;
        bool stateEAGAIN=false;
};
#endif
