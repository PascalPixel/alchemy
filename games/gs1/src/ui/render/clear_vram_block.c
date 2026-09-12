#include "types.h"

typedef s32 (*FillWordsFn)(void *dst, s32 size, s32 value);

s32 Ui_ClearVramBlock(void)
{
    FillWordsFn fill = (FillWordsFn)0x03000168;

    return fill((void *)0x06002500, 0xF00, 0);
}
