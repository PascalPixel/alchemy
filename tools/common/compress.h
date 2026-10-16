#ifndef AGS_COMPRESS_H
#define AGS_COMPRESS_H
#include "common.h"
typedef struct {
  size_t window, ahead, distance, paletteAhead;
} LzMachine;
extern const LzMachine packer;
/* shipped and packed differ only at documented post-packing edits. */
Buffer compressLz(const Buffer *shipped, const Buffer *packed, const char *kind,
                  LzMachine machine);
Buffer compressDelta7(const Buffer *data);
Buffer compressHalfwords(const Buffer *data);
Buffer compressIcon(const Buffer *pixels);
Buffer compressZeros(const Buffer *pixels);
Buffer deltaMap(const Buffer *data, unsigned mode);
Buffer compressArena(const Buffer *data, const Buffer *arena);
#endif
