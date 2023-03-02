#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")
#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alut.h"
#include <math.h>
#include <iostream>

#include "test.h"
#include "alengine.h"
using namespace std;
#include <string>

char ExePath[255];
 
 
 
void testOpenAl()
{
    ALEngine engine;
    //engine.setLoop(true);
    for (int i = 1; i <= 5; i++)
    {
        int size = i * 1000;
        unsigned char* data = engine.getTestWaveData(size);
        engine.pushPcmData(data, size);
        delete data;
    }
    engine.play();

    string cmd;
    while (cin >> cmd)
    {
        if (cmd == "q")
        {
            return;
        }
        else if (cmd == "x")
        {
            engine.changePosX(true);
        }
        else if (cmd == "xx")
        {
            engine.changePosX(false);
        }else if (cmd =="y")
        {
            engine.changePosY(true);
        }else if (cmd =="yy")
        {
            engine.changePosY(false);
        }else if (cmd =="z")
        {
            engine.changePosZ(true);
        }else if (cmd =="zz")
        {
            engine.changePosZ(false);
        }else if (cmd =="loop")
        {
            engine.setLoop(!engine.getLoop());
        }else if (cmd =="putbuf")
        {
            for (int i = 1; i <= 2; i++)
            {
                int size = i * 1000;
                unsigned char* data = engine.getTestWaveData(size);
                engine.pushPcmData(data, size);
                delete data;
            }
            engine.play();
        }else if (cmd =="stop")
        {
            engine.stop();
        }
        else if (cmd =="play")
        {
            engine.play();
        }
    }

   
}

void playPcmAudioByWindowSystemAPI()
{
    const int buf_size = 1024 * 1024 * 30;
    char* buf = new char[buf_size];



    char pcmFilePath[255];
    sprintf(pcmFilePath, "%s%s", ExePath, "taqing.pcm");

    FILE* thbgm; //文件
    fread(buf, sizeof(char), buf_size, thbgm); //预读取文件
    fopen_s(&thbgm, pcmFilePath, "rb");
    fclose(thbgm);

    WAVEFORMATEX wfx = { 0 };
    wfx.wFormatTag = WAVE_FORMAT_PCM; //设置波形声音的格式
    wfx.nChannels = 2;            //设置音频文件的通道数量
    wfx.nSamplesPerSec = 44100; //设置每个声道播放和记录时的样本频率
    wfx.wBitsPerSample = 16;    //每隔采样点所占的大小

    wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

    HANDLE wait = CreateEvent(NULL, 0, 0, NULL);
    HWAVEOUT hwo;
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT); //打开一个给定的波形音频输出装置来进行回放

    int data_size = 20480;
    char* data_ptr = buf;
    WAVEHDR wh;

    while (data_ptr - buf < buf_size)
    {
        //这一部分需要特别注意的是在循环回来之后不能花太长的时间去做读取数据之类的工作，不然在每个循环的间隙会有“哒哒”的噪音
        wh.lpData = data_ptr;
        wh.dwBufferLength = data_size;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;

        data_ptr += data_size;

        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR)); //准备一个波形数据块用于播放
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR)); //在音频媒体中播放第二个函数wh指定的数据

        WaitForSingleObject(wait, INFINITE); //等待
    }
    waveOutClose(hwo);
    CloseHandle(wait);
}
void testAlutPlayWav(){
    alutInit(nullptr, nullptr);
    char mp3File[255];
    sprintf(mp3File, "%s%s", ExePath, "1.wav");// 只支持了wav
    FILE* soundFile = fopen(mp3File, "rb");
    ALuint buffer = alutCreateBufferFromFile(mp3File);
    if (buffer== AL_NONE)
    {
        cout << alutGetErrorString(alutGetError());
        return;
    }
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
    ALint state;
    do 
    {
        alutSleep(0.3f);
        alGetSourcei(source, AL_SOURCE_STATE, &state);
    } while (state == AL_PLAYING);
}
int main(int argc, char** argv)
{
    GetModuleFileName(NULL, ExePath, 255);
    strrchr(ExePath, '\\')[1] = 0; // 0是字符串的结尾


    //testOpenAl();
    testMain("E:/proj/tank5/client/frameworks/qt-editor/res/gaga.mp4");
    //testAlutPlayWav();

    return 0;
}
