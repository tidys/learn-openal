#include "ALUtil.h"
#include <stdio.h>
char* getErrorString(ALenum err)
{
    switch (err)
    {
    case AL_NO_ERROR:return "AL_NO_ERROR";
    case AL_INVALID_NAME:return "AL_INVALID_NAME";
    case AL_INVALID_ENUM:return "AL_INVALID_ENUM";
    case AL_INVALID_VALUE:return "AL_INVALID_VALUE";
    case AL_INVALID_OPERATION:return "AL_INVALID_OPERATION";
    case AL_OUT_OF_MEMORY:return "AL_OUT_OF_MEMORY";
    default:return "un know error";
    }
}

void checkError(char* file, int line)
{
    ALenum err = alGetError();
    if (err != AL_NO_ERROR)
    {
        printf("[%d] openal error: %s\n", line, getErrorString(err));
    }
}

char* getState(ALint state)
{
    switch (state)
    {
    case AL_STOPPED:return "AL_STOPPED";
    case AL_PAUSED:return "AL_PAUSED";
    case AL_PLAYING:return "AL_PLAYING";
    case  AL_INITIAL:return "AL_INITIAL";
    default:return "unknow";
    }
}

void alState(ALuint source, char* file, int line)
{
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    printf("[%d] openal state: %s\n", line, getState(state));
}
