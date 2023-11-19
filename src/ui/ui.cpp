#include "ui.h"

char* ui_get_string(FILE* stream) {
    assert(stream);

    char* input = nullptr;
    size_t input_size = 0;

    ssize_t input_len = getline(&input, &input_size, stream);

    if (input_len <= 0) {
        FREE(input);
        return nullptr;
    }

    input[input_len - 1] = '\0'; //< \n removal

    return input;
}

char ui_get_char(FILE* stream) {
    assert(stream);

    int c = '\0';

    while ((c = getchar()) == '\n' && !feof(stream) && !ferror(stream)) {}

    if (c == EOF || c == '\n') {
        return '\0';
    }

    if (getchar() != '\n') {
        input_flush(stream);
        return '\0';
    }

    return (char)c;
}

bool input_flush(FILE* stream) {
    assert(stream);

    int c = ' ';
    bool non_space = true;

    do {
        if (!isspace(c))
            non_space = false;

    } while ((c = fgetc(stream)) != '\n' && c != EOF && !feof(stream) && !ferror(stream));

    return non_space;
}
