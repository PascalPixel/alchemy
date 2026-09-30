#include "TYPES.H"
#include "SCENE.H"

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern u8 *gBattleWork;
extern volatile u32 gKeyState;

void WaitFrames(s32);

/* What holding Select answers: each edition's build has its own value. */
#if defined(TBS_EDITION_JA)
#define DEBUG_SELECT_VALUE 480
#else
#define DEBUG_SELECT_VALUE 0x18f
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
    u8 *base;
    volatile u32 *keys;

    if (gKeysHeld & 8) {
        keys = &gKeysRepeat;
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
        if (gKeyState & 1) {
            ret = *(s32 *)(base + 0x828);
            goto done;
        }
        WaitFrames(1);
        goto loop;
    }
done:
    if (gKeysHeld & 4)
        ret = DEBUG_SELECT_VALUE;
    return ret;
}
