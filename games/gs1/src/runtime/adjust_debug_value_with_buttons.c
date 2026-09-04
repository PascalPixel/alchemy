#include "types.h"

#define Runtime_AdjustDebugValueWithButtons Func_080c0eec

extern volatile u32 Data_03001ae8;
extern volatile u32 Data_03001b04;
extern u8 * volatile Data_03001e74;
extern volatile u32 Data_03001c94;

void WaitFrames(s32);

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
    u8 *base;
    volatile u32 *keys;

    if (Data_03001ae8 & 8) {
        keys = &Data_03001b04;
loop:
        base = Data_03001e74;
        if (*keys & 0x20)
            *(s32 *)(base + 0x828) -= 1;
        if (*keys & 0x10)
            *(s32 *)(base + 0x828) += 1;
        if (*keys & 0x40)
            *(s32 *)(base + 0x828) -= 100;
        if (*keys & 0x80)
            *(s32 *)(base + 0x828) += 100;
        if (Data_03001c94 & 1) {
            ret = *(s32 *)(base + 0x828);
            goto done;
        }
        WaitFrames(1);
        goto loop;
    }
done:
    if (Data_03001ae8 & 4)
        ret = 0x18f;
    return ret;
}
