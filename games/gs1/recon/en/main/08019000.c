#include "types.h"
#include "global_cells.h"

#define UiWindow_SetTilemapEntry Func_08019000

void UiWindow_SetTilemapEntry(
    u8 *window, s32 value, s32 x, s32 y, u32 mode)
{
    u16 *map = *(u16 **)ADDR_03001E8C;
    s32 mask;
    s32 index;

    y += 1;
    if ((u32)y > (u32)(*(u16 *)(window + 10) - 1))
        return;

    x += 1;
    if ((u32)x > (u32)(*(u16 *)(window + 8) - 1))
        return;

    if (mode == 3) {
        mask = 0xf000;
    } else if (mode <= 3) {
        if (mode == 2) {
            mask = 0xe000;
        } else {
            mask = 0;
        }
    } else {
        if (mode == 4) {
            mask = 0x1000;
        } else {
            mask = 0;
        }
    }

    if (mode == 1)
        return;
    if (1 > mode)
        goto plain;
    if (mode > 4)
        goto plain;

    index = ((*(u16 *)(window + 14) + y) << 5)
        + (*(u16 *)(window + 12) + x);
    if ((u32)index >= 640)
        return;
    map[index] = (u16)(mask | value);
    return;

plain:
    index = ((*(u16 *)(window + 14) + y) << 5)
        + (*(u16 *)(window + 12) + x);
    if ((u32)index >= 640)
        return;
    map[index] = (u16)value;
}
