#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "TBS_EDITION.H"

extern u8 Data_03001e90[];
s32 Runtime_ReleaseHeapBlock(s32);
void UiWork_Finalize(struct Work *work, s32 release);

extern u8 Data_03001e8c[];

void UiWork_FinalizeAndReleaseBlock16(void)
{
    UiWork_Finalize(**(s32 **)((u32)&Data_03001e90), 1);
    Runtime_ReleaseHeapBlock(0x10);
}

void UiWindow_SetTileAttributeBitRect(
    const u8 *window, s32 x, s32 y, s32 width, s32 height, u32 field)
{
    u8 *base = *(u8 **)((u32)&Data_03001e8c);

    x += *(u16 *)(window + 12) + 1;
    y += *(u16 *)(window + 14) + 1;
    field &= 1;
    field <<= 12;
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (x + width > 29) {
        width = 30 - x;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (y + height > 29) {
        height = 20 - y;
    }
    if (width > 0 && height > 0) {
        y <<= 6;
        x = y + (x << 1);
        do {
            u16 *cell = (u16 *)((u32)x + (u32)base);
            s32 remaining = width;
            while (remaining != 0) {
                u32 value = *cell;
                value &= 0xFFFFEFFF;
                value |= field;
                remaining--;
                *cell = value;
                cell++;
            }
            height--;
            x += 64;
        } while (height != 0);
        base[RENDER_DIRTY_OFS] = 1;
    }
}
