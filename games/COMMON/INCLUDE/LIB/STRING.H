/* The C library's memcpy by its standard prototype. The compiler copies a
 * constant size inline, so no library routine is linked. */
#ifndef ALCHEMY_STRING_H
#define ALCHEMY_STRING_H

void *memcpy(void *destination, const void *source, unsigned long size);

#endif
