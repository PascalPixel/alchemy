#ifndef ALCHEMY_STRING_H
#define ALCHEMY_STRING_H

/* The one C library string function the code declares. */

#include "edition.h"

/* Copies size bytes from source to destination, for the modern build. */
void *memcpy(void *dest, const void *src, unsigned long size);

#endif
