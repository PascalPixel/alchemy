#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"

void Graphics_BuildSequentialTileTable(s32 *destination)
{
    s32 value;
    s32 count;
    s32 (*fill)(void *, s32, u32);

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

    fill = Iwram_FillWords;
    fill(destination, 0x220, -1);
}
