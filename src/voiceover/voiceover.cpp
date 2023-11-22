#ifdef _WIN32

#define TX_COMPILED
#define TX_USE_SPEAK
#include "TXLib.h"

#include "voiceover.h"

void voiceover(const bool speak_all, const bool async, const char* format, ...) {
    static size_t buf_size = 0;
    static const size_t BUFSIZE = _TX_BUFSIZE - 125; //< 125 is size of ssml tags
    static char buf[BUFSIZE] = "";

    if (format != nullptr) {
        char print_buf[BUFSIZE] = "";

        va_list args = {};
        va_start(args, format);

        vsnprintf(print_buf, BUFSIZE - 1, format, args);

        va_end(args);

        size_t token_len = strlen(print_buf);

        if (buf_size + token_len > BUFSIZE - 1) {
            buf[buf_size] = '\0';
            txSpeak("%s", buf);
            buf_size = 0;
        }

        strncpy(buf + buf_size, print_buf, token_len);
        buf_size += token_len;
    }

    if (speak_all) {
        buf[buf_size] = '\0';

        if (async)
            txSpeak("\a<speak version=\"1.0\" xmlns=\"http://www.w3.org/2001/10/synthesis\" xml:lang=\"RU\">"
                    "<prosody rate=\"2\">%s</prosody></speak>", buf);
        else
            txSpeak("<speak version=\"1.0\" xmlns=\"http://www.w3.org/2001/10/synthesis\" xml:lang=\"RU\">"
                    "<prosody rate=\"2\">%s</prosody></speak>", buf);

        buf_size = 0;
    }
}

#endif //< #ifdef _WIN32
