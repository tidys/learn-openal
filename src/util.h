#ifdef _WIN32
#include <AL/al.h>
#include <AL/alc.h>
#else
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif


char* getErrorString(ALenum err);
void checkError(char* file, int line);
#define CHECHERROR() checkError(__FILE__, __LINE__);
char* getState(ALint state);
void alState(ALuint source,char* file, int line);
#define ALSTATE(source)  alState(source,__FILE__, __LINE__);