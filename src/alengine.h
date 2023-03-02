#ifndef __ALEngine__H__
#define __ALEngine__H__
#include "ALUtil.h"
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
using namespace std;


#define ALENGINE_BUFFER_COUNT 3
struct PcmdFrame{
    unsigned char* data = nullptr;
    int size = 0;
};
class ALEngine{
public:
    ALEngine();
    ~ALEngine();
    void threadFunction();

    void play();
    void stop();
    unsigned char* getTestWaveData(int size);
    void setChannels(int n);
    void changePosX(bool inc);
    void changePosY(bool inc);
    void changePosZ(bool inc);
    void setLoop(bool loop);
    void setFrequency(int freq);
    bool getLoop();
    bool isPlaying();
    void pushPcmData(unsigned char* data, int size);
private:
    void getBufferInfo(ALuint buuferID);
    void eraseFront();
    void cleanQueuedBuffers();
    void cleanProcessedBuffers();
    int getQueuedBuffersCount();
    int getProcessedBuffersCount();
    void updateSourceProperty();
    void _stop();
    void _play();
private:
    ALenum _format = AL_FORMAT_STEREO16;
    int _channels = 2;
    int _freq = 1000;
    ALuint _source;
    ALCdevice* pDevice=nullptr;
    ALCcontext* pContext = nullptr;
    float _posX = 0;
    float _posY = 0;
    float _posZ = 0;
    bool _loop = false;
    thread* _thread=nullptr;
    bool stopThread = false;
    vector<PcmdFrame> _pcmFrames;
    std::mutex _mutex;
    bool _isPlaying = false;
};
#endif
