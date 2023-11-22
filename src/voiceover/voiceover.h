#ifdef _WIN32

#ifndef VOICEOVER_H_
#define VOICEOVER_H_

#include <assert.h>
#include <string.h>
#include <stdarg.h>

void voiceover(const bool speak_all, const bool async, const char* format, ...);

#endif //< #ifndef VOICEOVER_H_

#endif //< #ifdef _WIN32
