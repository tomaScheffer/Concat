#ifndef TYPE_HEADER
#define TYPE_HEADER

#pragma warning(disable: 4005)
#include <stdint.h>

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef int Token;

struct Buffer {
	int lenght;
	uint8_t *data;
};

#endif
