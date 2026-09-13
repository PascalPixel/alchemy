#include "types.h"
#include "scene.h"

extern volatile u32 gIw;
extern volatile u32 gIw2;
extern u8 *volatile gBattleWork;
extern volatile u32 gIw3;

void WaitFrames(s32);

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
    u8 *base;
    volatile u32 *keys;

    if (gIw & 8) {
        keys = &gIw2;
loop:
        base = gBattleWork;
        if (*keys & 0x20)
            *(s32 *)(base + 0x828) -= 1;
        if (*keys & 0x10)
            *(s32 *)(base + 0x828) += 1;
        if (*keys & 0x40)
            *(s32 *)(base + 0x828) -= 100;
        if (*keys & 0x80)
            *(s32 *)(base + 0x828) += 100;
        if (gIw3 & 1) {
            ret = *(s32 *)(base + 0x828);
            goto done;
        }
        WaitFrames(1);
        goto loop;
    }
done:
    if (gIw & 4)
        ret = 0x18f;
    return ret;
}
