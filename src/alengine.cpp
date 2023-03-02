#include "ALEngine.h"

ALEngine::ALEngine()
{
   stopThread = false;
    pDevice = alcOpenDevice(NULL);
    pContext = alcCreateContext(pDevice, NULL);
    alcMakeContextCurrent(pContext);
    alGenSources(1, &_source);
    this->updateSourceProperty();
}

ALEngine::~ALEngine()
{
    this->cleanProcessedBuffers();
    this->cleanQueuedBuffers();
    alDeleteSources(1, &_source);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(pContext);
    alcCloseDevice(pDevice);
    stopThread = true;
    if (this->_thread)
    {
        this->_thread->join();
        delete this->_thread;
        this->_thread = nullptr;
    }
}

void ALEngine::getBufferInfo(ALuint buuferID)
{
    ALint size;
    alGetBufferi(buuferID, AL_SIZE, &size);
    ALint channels;
    alGetBufferi(buuferID, AL_CHANNELS, &channels);
    ALint freq;
    alGetBufferi(buuferID, AL_FREQUENCY, &freq);
}

void ALEngine::eraseFront()
{
    PcmdFrame frame = this->_pcmFrames.front();
    delete frame.data;
    frame.data = nullptr;
    this->_pcmFrames.erase(this->_pcmFrames.begin());
}

void ALEngine::threadFunction()
{
    while (!stopThread)
    {
        //this_thread::sleep_for(std::chrono::milliseconds(300));
        this->cleanProcessedBuffers();
        if (!this->_isPlaying)
        {
            this->_stop();
            continue;
        }
        if (this->getQueuedBuffersCount() < ALENGINE_BUFFER_COUNT && this->_pcmFrames.size() > 0)
        {
            unique_lock<mutex> lock(this->_mutex);
            PcmdFrame frame = this->_pcmFrames.front();

            ALuint buffer;
            alGenBuffers(1, &buffer);
            CHECHERROR();
            alBufferData(buffer, this->_format, frame.data, frame.size, this->_freq);
            CHECHERROR();
            alSourceQueueBuffers(this->_source, 1, &buffer);
            CHECHERROR();

            this->eraseFront();
            this->_play();
        }
        else{
            //emit end
        }
    }
    printf("check thread over");
}


void ALEngine::updateSourceProperty()
{
    // 源声音的位置
    ALfloat SourcePos[] = { this->_posX, this->_posY, this->_posZ };
    printf("Position x:%.2f, y:%.2f, z:%.2f\n", this->_posX, this->_posY,this->_posZ);
    // 源声音的速度
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    alSourcef(_source, AL_PITCH, 1.0f);
    alSourcef(_source, AL_GAIN, 1.0f);
    alSourcefv(_source, AL_POSITION, SourcePos);
    alSourcefv(_source, AL_VELOCITY, SourceVel);
    this->setLoop(this->_loop);
}

void ALEngine::_stop()
{
    ALint state;
    alGetSourcei(this->_source, AL_SOURCE_STATE, &state);
    if (state != AL_STOPPED)
    {
        printf("stop");
        alSourceStop(_source);
    }
}

void ALEngine::_play()
{
    ALint state;
    alGetSourcei(this->_source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
    {
        int queueNum=this->getQueuedBuffersCount();
        if (queueNum > 0)
        {
            alSourcePlay(_source);
        }
    }
}


void ALEngine::play()
{
    this->_isPlaying = true;
    if (this->_thread == nullptr)
    {
        this->_thread = new thread(&ALEngine::threadFunction, this);
    }
    this->_play();
}

void ALEngine::stop()
{
    this->_isPlaying =false;
}

unsigned char* ALEngine::getTestWaveData(int size)
{
    unsigned char* data = new unsigned char[size];
    float max = CHAR_MAX / 4;
    float rad = 0;

    for (int i = 0; i < size; i++)
    {
        data[i] = (max * cosf(rad));
        rad += 1.f;
    }
    return data;
}

void ALEngine::setChannels(int n)
{
    this->_channels = n;
    this->_format = n > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
}

void ALEngine::changePosX(bool inc)
{
    inc ? this->_posX += 10 : this->_posX -= 10;
    this->updateSourceProperty();
}

void ALEngine::changePosY(bool inc)
{
    inc ? this->_posY += 10 : this->_posY -= 10;
    this->updateSourceProperty();
}

void ALEngine::changePosZ(bool inc)
{
    inc ? this->_posZ += 10 : this->_posZ -= 10;
    this->updateSourceProperty();
}

void ALEngine::setLoop(bool loop)
{
    this->_loop = loop;
    alSourcei(_source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

void ALEngine::setFrequency(int freq)
{
    this->_freq = freq;
}

bool ALEngine::getLoop()
{
    return this->_loop;
}

bool ALEngine::isPlaying()
{
    ALint state;
    alGetSourcei(_source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

int ALEngine::getQueuedBuffersCount()
{
    ALint process = 0;
    alGetSourcei(_source, AL_BUFFERS_QUEUED, &process);
    return process;
}

int ALEngine::getProcessedBuffersCount()
{
    ALint process = 0;
    alGetSourcei(_source, AL_BUFFERS_PROCESSED, &process);
    return process;
}

void ALEngine::cleanQueuedBuffers()
{
    ALint process = this->getQueuedBuffersCount();
    while ((process--) > 0)
    {
        ALuint bufferID;
        alSourceUnqueueBuffers(_source, 1, &bufferID);
        alDeleteBuffers(1, &bufferID);
    }
}
// 清理队列中已经完成的buffer
void ALEngine::cleanProcessedBuffers()
{
    ALint process = this->getProcessedBuffersCount();
    while ((process--) > 0)
    {
        ALuint bufferID;
        alSourceUnqueueBuffers(_source, 1, &bufferID);
        alDeleteBuffers(1, &bufferID);
        printf("clean buffer\n");
    }
}

void ALEngine::pushPcmData(unsigned char* data, int size)
{
    unique_lock<mutex> lock(this->_mutex);
    PcmdFrame frame;
    frame.size = size;
    frame.data = new unsigned char[size];
    memcpy(frame.data, data, size);
    this->_pcmFrames.push_back(frame);
}