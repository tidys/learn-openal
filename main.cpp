#include <Windows.h>
#include <stdio.h>
#pragma comment(lib, "winmm.lib")

int main(int argc, char** argv)
{
    const int buf_size = 1024 * 1024 * 30;
    char* buf = new char[buf_size];
    //char * buf = (char *)malloc(buf_size);	//.c�������

    char exePath[255];
    GetModuleFileName(NULL, exePath, 255);
    strrchr(exePath, '\\')[1] = 0; // 0���ַ����Ľ�β

    char pcmFilePath[255];
    sprintf(pcmFilePath, "%s%s", exePath, "taqing.pcm");

    FILE* thbgm; //�ļ�
    fopen_s(&thbgm, pcmFilePath, "rb");
    /*	thbgm	=	fopen("pcm16k.pcm","rb");	//����ffmpegת��������pcm��δ�ҵ�fopen_s���ڵĿ⣬��ʹ�����
    if(NULL == thbgm)	return -1;
    */
    fread(buf, sizeof(char), buf_size, thbgm); //Ԥ��ȡ�ļ�
    fclose(thbgm);

    WAVEFORMATEX wfx = { 0 };
    wfx.wFormatTag = WAVE_FORMAT_PCM; //���ò��������ĸ�ʽ
    wfx.nChannels = 2;            //������Ƶ�ļ���ͨ������
    wfx.nSamplesPerSec = 44100; //����ÿ���������źͼ�¼ʱ������Ƶ��
    wfx.wBitsPerSample = 16;    //ÿ����������ռ�Ĵ�С

    wfx.nBlockAlign = wfx.nChannels * wfx.wBitsPerSample / 8;
    wfx.nAvgBytesPerSec = wfx.nBlockAlign * wfx.nSamplesPerSec;

    HANDLE wait = CreateEvent(NULL, 0, 0, NULL);
    HWAVEOUT hwo;
    waveOutOpen(&hwo, WAVE_MAPPER, &wfx, (DWORD_PTR)wait, 0L, CALLBACK_EVENT); //��һ�������Ĳ�����Ƶ���װ�������лط�

    int data_size = 20480;
    char* data_ptr = buf;
    WAVEHDR wh;

    while (data_ptr - buf < buf_size)
    {
        //��һ������Ҫ�ر�ע�������ѭ������֮���ܻ�̫����ʱ��ȥ����ȡ����֮��Ĺ�������Ȼ��ÿ��ѭ���ļ�϶���С����ա�������
        wh.lpData = data_ptr;
        wh.dwBufferLength = data_size;
        wh.dwFlags = 0L;
        wh.dwLoops = 1L;

        data_ptr += data_size;

        waveOutPrepareHeader(hwo, &wh, sizeof(WAVEHDR)); //׼��һ���������ݿ����ڲ���
        waveOutWrite(hwo, &wh, sizeof(WAVEHDR)); //����Ƶý���в��ŵڶ�������whָ��������

        WaitForSingleObject(wait, INFINITE); //�ȴ�
    }
    waveOutClose(hwo);
    CloseHandle(wait);


    return 0;
}
