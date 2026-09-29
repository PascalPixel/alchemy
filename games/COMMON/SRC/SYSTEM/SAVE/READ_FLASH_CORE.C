#include "TYPES.H"

/* Byte copy the flash reader runs from RAM. */
void ReadFlashCore(const u8 *src, u8 *dest, u32 size)
{
    u32 remaining = size - 1;

    if (size != 0) {
        do {
            *dest++ = *src++;
        } while (remaining-- != 0);
    }
}
