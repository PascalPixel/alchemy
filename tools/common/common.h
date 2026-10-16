#ifndef AGS_COMMON_H
#define AGS_COMMON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Buffers own their allocation. Append operations keep a spare trailing NUL;
 * len counts only the payload, which may contain zero bytes. */
typedef struct {
  uint8_t *data;
  size_t len, cap;
} Buffer;
typedef struct {
  char **items;
  size_t len, cap;
} Strings;
typedef struct {
  Buffer bytes;
  size_t count;
} Bits;

void startTool(int argc, char **argv, const char *usage);
_Noreturn void fail(const char *format, ...);
void *allocate(size_t count, size_t size);
void *resize(void *data, size_t count, size_t size);
char *copyString(const char *text);
char *formatString(const char *format, ...);
void reserve(Buffer *buffer, size_t extra);
void append(Buffer *buffer, const void *data, size_t size);
void byte(Buffer *buffer, unsigned value);
void half(Buffer *buffer, unsigned value);
void word(Buffer *buffer, uint32_t value);
void print(Buffer *buffer, const char *format, ...);
void alignBuffer(Buffer *buffer, size_t alignment);
uint16_t read16(const uint8_t *data);
uint32_t read32(const uint8_t *data);
void write16(uint8_t *data, unsigned value);
void write32(uint8_t *data, uint32_t value);
Buffer readFile(const char *path);
void writeFile(const char *path, const Buffer *buffer);
void writeChanged(const char *path, const Buffer *buffer);
bool exists(const char *path);
void makeParents(const char *path);
char *trim(char *text);
Strings split(const char *text, char separator);
Strings words(const char *text);
uint64_t decimalOrHex(const char *text, uint64_t maximum);
void freeStrings(Strings *strings);
void addString(Strings *strings, const char *text);
bool hasString(const Strings *strings, const char *text);
void sortStrings(Strings *strings);
uint64_t number(const char *text, int base, uint64_t maximum);
char *beside(const char *path, const char *name);
const char *extension(const char *path);
void putBits(Bits *bits, uint32_t value, unsigned count);
void alignBits(Bits *bits, unsigned alignment, unsigned fill);
unsigned bitLength(size_t value);

#endif
