#include "EDITION.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "SCENE.H"
#include "BATTLE_WORK.H"

extern volatile u32 gKeysHeld;
extern volatile u32 gKeysRepeat;
extern volatile u32 gKeyState;

void WaitFrames(s32);

/* What holding Select answers: each edition's build has its own value. */
#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
    struct BattleSession *battle;
    volatile u32 *keys;

    if (gKeysHeld & KEY_START) {
        keys = &gKeysRepeat;
loop:
        battle = gBattleWork;
        if (*keys & 0x20)
            battle->debug_value -= 1;
        if (*keys & 0x10)
            battle->debug_value += 1;
        if (*keys & 0x40)
            battle->debug_value -= 100;
        if (*keys & 0x80)
            battle->debug_value += 100;
        if (gKeyState & KEY_A) {
            ret = battle->debug_value;
            goto done;
        }
        WaitFrames(1);
        goto loop;
    }
done:
    if (gKeysHeld & KEY_SELECT)
        ret = DEBUG_SELECT_VALUE;
    return ret;
}
