#ifndef ENVIRONMENT_HEADER
#define ENVIRONMENT_HEADER

#include "Type.h"
#include <stdlib.h>
#include <string.h>

/**
 * Analog to "getStringOrDefault", but returning true if the string is exactly
 * "true"; false otherwise. The default value is only used when the variable
 * is undefined.
 */
const boolean getBooleanOrDefault(const char * name, const boolean defaultValue);

/**
 * Gets the value of an environment variable by name, or returns a default
 * value if the variable is undefined.
 * 
 * @see https://cplusplus.com/reference/cstdlib/getenv/
 */
const char * getStringOrDefault(const char * name, const char * defaultValue);


/**
 * https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */
char *base64_encode(const unsigned char *data, size_t input_length, size_t *output_length);

/**
 * https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */
unsigned char *base64_decode(const char *data, size_t input_length, size_t *output_length);

/**
 * https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */
void build_decoding_table();

/**
 * https://stackoverflow.com/questions/342409/how-do-i-base64-encode-decode-in-c
 */
void base64_cleanup();

#endif
