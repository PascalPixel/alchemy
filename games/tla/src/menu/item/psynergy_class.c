#include "types.h"
#include "ram_buffer.h"

struct BattleAct {
    u8 targetMode;
    u8 unk01[5];
    u8 type;
    u8 unk07;
    u8 range;
};

struct PsynergyMenuState {
    u8 unk000[0x16c];
    u16 paneAction[2];
};

struct BattleAct *BattleActGet(s32 action);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
// The Spanish and Italian far-call tables reach BattleFx_HasMatchingEvent5
// through this stub (as selrow.c).
s32 Func_080c84f8(s32 type);
#else
s32 BattleFx_HasMatchingEvent5Far(s32 type);
#endif

// How the selected psynergy is used: 0 acts at once, 1 picks a target,
// 2 cannot be used here. The Broken Seal's
// PsynergyMenu_ClassifySelectedPsynergy.
s32 Func_080fcf14(void)
{
    struct PsynergyMenuState *menu;
    struct BattleAct *psynergy;
    s32 diff;
    s32 ret;
    /* FAKEMATCH: the barrier keeps the action's load ahead of the 0x3fff mask, and the action pinned to r3 keeps its register, as native; plain, the mask starts first (differ 2), the barrier alone loads into r0 (differ 8), and the pin alone changes nothing (differ 2). */
    register u32 action asm("r3");

    menu = Ram_HeapSlots->menuRuntime;
    action = menu->paneAction[0];
    asm volatile("");
    psynergy = BattleActGet(0x3fff & action);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    if (Func_080c84f8(psynergy->type) != 0)
#else
    if (BattleFx_HasMatchingEvent5Far(psynergy->type) != 0)
#endif
        return 0;
    ret = 2;
    if (psynergy->range != 0xff) {
        u8 kind = psynergy->targetMode;

        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (u32)ret >> 31;
        ret = 1 - ret;
    }
    return ret;
}
