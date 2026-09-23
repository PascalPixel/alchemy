#include "TYPES.H"
#include "GLOBAL_CELLS.H"

/* Writes one tile into a window's tilemap; modes 2-4 add palette bits. */
void UiWindow_SetTilemapEntry(
    u8 *window, s32 value, s32 x, s32 y, u32 mode)
{
    u16 *map = *(u16 **)ADDR_03001E8C;
    s32 mask;
    s32 index;

    y += 1;
    x += 1;
    if ((u32)y > (u32)(*(u16 *)(window + 10) - 1))
        return;

    if ((u32)x > (u32)(*(u16 *)(window + 8) - 1))
        return;

    switch (mode) {
    case 3:
        mask = 0xf000;
        break;
    case 2:
        mask = 0xe000;
        break;
    case 4:
        mask = 0x1000;
        break;
    default:
        mask = 0;
        break;
    }

    switch (mode) {
    case 0:
        goto plain;
    case 1:
        return;
    case 2:
    case 3:
    case 4:
        break;
    default:
        goto plain;
    }

    index = ((*(u16 *)(window + 14) + y) << 5)
        + (*(u16 *)(window + 12) + x);
    if ((u32)index >= 640)
        return;
    *(u16 *)((u8 *)map + (index << 1)) = (u16)(mask | value);
    return;

plain:
    index = ((*(u16 *)(window + 14) + y) << 5)
        + (*(u16 *)(window + 12) + x);
    if ((u32)index >= 640)
        return;
    *(u16 *)((u8 *)map + (index << 1)) = (u16)value;
}
