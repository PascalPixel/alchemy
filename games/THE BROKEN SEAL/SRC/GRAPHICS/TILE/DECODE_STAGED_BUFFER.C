#include "TYPES.H"

/* Decodes the tilemap staged at 0x02010000 (mode byte at +1) into
   0x02020000: a plain halfword copy, a byte-planar delta, or a halfword
   delta. */
void Tilemap_DecodeStagedBuffer(s32 size)
{
    u16 *destination;
    u16 *source;
    s32 count;
    s32 index;
    u32 temp;

    source = (u16 *)0x02010002;
    destination = (u16 *)0x02020000;
    temp = size - 1;
    count = (s32)(temp + (temp >> 31)) >> 1;
    if (size & 1) {
        switch (*(u8 *)0x02010001) {
        case 0: {
            index = 0;
            if (index < count) {
                do {
                    *destination = *source;
                    index++;
                    source++;
                    destination++;
                } while (index < count);
            }
            /* FAKEMATCH: an empty do-while fixes the case-1 source register */
            do {
            } while (0);
            break;
        }
        case 1: {
            u32 previous;
            u8 *left;
            u8 *right;
            previous = 0;
            left = (u8 *)0x02010002;
            index = 0;
            right = left;
            right += count;
            if (index < count) {
                do {
                    s32 low = *right;
                    s32 high = *left << 8;
                    s32 value = (high | low) ^ previous;
                    index++;
                    *destination = value;
                    right++;
                    left++;
                    destination++;
                    previous = value;
                } while (index < count);
            }
            break;
        }
        case 2: {
            s32 previous = 0;
            if (count > 0) {
                index = count;
                do {
                    s32 value = *source ^ previous;
                    index--;
                    *destination = value;
                    source++;
                    destination++;
                    previous = value;
                } while (index != 0);
            }
            break;
        }
        }
    }
}
