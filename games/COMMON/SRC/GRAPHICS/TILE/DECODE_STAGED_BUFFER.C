#include "TYPES.H"
extern u32 gMapBlocks[];
#include "RAM_BUFFER.H"
extern u8 gMapCellBuffer[];

void Tilemap_DecodeStagedBuffer(s32 size)
{
    u16 *destination;
    u16 *source;
    s32 count;
    s32 index;

    source = (u16 *)(Ram_MapCellBuffer + 2);
    destination = (u16 *)gMapBlocks;
    count = (size - 1) / 2;
    if (size & 1) {
        switch (((u8 *)gMapCellBuffer)[1]) {
        case 0:
            for (index = 0; index < count; index++)
                *destination++ = *source++;
            break;
        case 1: {
            u32 previous = 0;
            u8 *left = Ram_MapCellBuffer + 2;
            u8 *right = left + count;

            for (index = 0; index < count; index++) {
                u32 value = ((*left++ << 8) | *right++) ^ previous;
                *destination++ = value;
                previous = value;
            }
            break;
        }
        case 2: {
            s32 previous = 0;

            for (index = 0; index < count; index++) {
                s32 value = *source++ ^ previous;
                *destination++ = value;
                previous = value;
            }
            break;
        }
        }
    }
}
