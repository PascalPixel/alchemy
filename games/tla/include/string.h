#ifndef ALCHEMY_STRING_H
#define ALCHEMY_STRING_H
#include "edition.h"

/// Copies size bytes from source to destination, for the modern build.
void *memcpy(void *destination, const void *source, unsigned long size);

#endif
