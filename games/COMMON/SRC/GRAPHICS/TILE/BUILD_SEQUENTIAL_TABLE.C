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
    } while (count >= 0 && count <= 63);

    value = 0x03020100;
    count = 0;
    do {
        count += 1;
        *destination++ = value;
        value += 0x04040404;
    } while (count >= 0 && count <= 55);

    fill = Iwram_FillWords;
    fill(destination, 544, -1);
}
