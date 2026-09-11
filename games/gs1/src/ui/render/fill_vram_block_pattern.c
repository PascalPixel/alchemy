#include "types.h"
#include "scene.h"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

s32 Ui_FillVramBlockPattern(void)
{
    FillWordsFn fill = (FillWordsFn)0x03000168;

    return fill((void *)0x06002500, 0xF00, 0x44444444);
}
