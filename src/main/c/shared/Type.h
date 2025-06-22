#ifndef TYPE_HEADER
#define TYPE_HEADER

typedef enum {
	false = 0,
	true = 1
} boolean;

typedef int Token;

typedef enum {
    STRING_TYPE,
    ATOMIC_TYPE,
    BUFFER_TYPE
} Type;

struct Buffer {
	int lenght;
	uint8_t *data;
};

#endif
