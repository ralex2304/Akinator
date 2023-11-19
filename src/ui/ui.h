#ifndef UI_H_
#define UI_H_

#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#include "utils/macros.h"

/**
 * @brief Gets one line from input stream
 *
 * @param stream
 * @return char*
 */
char* ui_get_string(FILE* stream = stdin);

/**
 * @brief Gets one char from stream
 *
 * @param stream
 * @return char
 */
char ui_get_char(FILE* stream = stdin);

/**
 * @brief Flushes input stream
 * @param stream
 *
 * @return bool false - if there are non space symbols in stream left
 */
bool input_flush(FILE* stream = stdin);

#endif //< #ifndef UI_H_
