#include "types.h"
#include "ram_buffer.h"

// 8 フレームごとにパレットの色 5-9 と 13-15 を 1 つずつ送る
void Func_02000c14(void)
{
    if ((*Ram_FrameCount & 7) == 0)
    {
        volatile u16 *palette;
        s16           saved;
        s32           index;

        saved = *(volatile u16 *)0x05000172;
        index = 9;
    first:
        palette = (u16 *)0x05000160;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 4)
            goto first;
        *(volatile u16 *)0x05000168 = saved;
    }
    if ((*Ram_FrameCount & 7) == 0)
    {
        volatile u16 *palette;
        s16           saved;
        s32           index;

        saved = *(volatile u16 *)0x0500017e;
        index = 15;
    second:
        palette = (u16 *)0x05000160;
        palette[index] = (palette - 1)[index];
        index--;
        if (index != 12)
            goto second;
        *(volatile u16 *)0x05000178 = saved;
    }
}
