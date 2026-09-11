#include "types.h"
#include "scene.h"

typedef void (*FillWordsFn)(void *dst, s32 size, s32 value);

void Graphics_BuildSequentialTileTable(s32 *destination)
{
    s32 value;
    s32 count;
    FillWordsFn fill;

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x3F);

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while ((u32)count <= 0x37);

    fill = (FillWordsFn)0x03000168;
    fill(destination, 0x220, -1);
}
