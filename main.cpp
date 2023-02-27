#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

int main(int argc, char** argv)
{
    const int buf_size = 1024 * 1024 * 30;
    char* buf = new char[buf_size];
    //char * buf = (char *)malloc(buf_size);	//.c请用这句

    char exePath[255];
    GetModuleFileName(NULL, exePath, 255);
    strrchr(exePath, '\\')[1] = 0; // 0是字符串的结尾

    char pcmFilePath[255];
    sprintf(pcmFilePath, "%s%s", exePath, "taqing.pcm");

    FILE* thbgm; //文件
    fopen_s(&thbgm, pcmFilePath, "rb");
    /*	thbgm	=	fopen("pcm16k.pcm","rb");	//导入ffmpeg转换出来的pcm，未找到fopen_s所在的库，请使用这句
    if(NULL == thbgm)	return -1;
    */
    fread(buf, sizeof(char), buf_size, thbgm); //预读取文件
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


    return 0;
}
