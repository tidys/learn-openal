#ifdef OPENAL_PLAIN_INCLUDES
#include "al.h"
#include "alc.h"
#else
#include "AL/al.h"
#include "AL/alc.h"
#endif
typedef unsigned char unit8_t;

char* getErrorString(ALenum err);
void checkError(char* file, int line);
#define CHECHERROR() checkError(__FILE__, __LINE__);
char* getState(ALint state);
void alState(ALuint source,char* file, int line);
#define ALSTATE(source)  alState(source,__FILE__, __LINE__);