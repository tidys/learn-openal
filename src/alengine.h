#ifndef __ALEngine__H__
#define __ALEngine__H__
#include <iostream>
#include <thread>
#include <vector>
#include "util.h"
#include <mutex>
using namespace std;
struct PcmdFrame{
    unsigned char* data = nullptr;
    int size = 0;
    int hz = 1000;
};
class ALEngine{
public:
    ALEngine();
    ~ALEngine();
    void threadFunction();

    void play();
    void stop();
    unsigned char* getTestData(int size=2000);
    void changePosX(bool inc);
    void changePosY(bool inc);
    void changePosZ(bool inc);
    void setLoop(bool loop);
    bool getLoop(){ return this->_loop; }
    bool isPlaying();
    int getQueuedBuffersCount();
    void cleanQueuedBuffers();
    void cleanProcessedBuffers();
    void pushPcmData(unsigned char* data, int size, int hz);
private:
    void updateSourceProperty();
    void _stop();
    void _play();
private:
    ALuint _source;// 用于播放声音
    ALuint _buffer;// 声音数据
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
