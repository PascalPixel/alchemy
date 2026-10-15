#include "types.h"
#include "ram_buffer.h"

struct BtlAct {
    u8 target_mode;
    u8 unknown_01[5];
    u8 type;
    u8 unknown_07;
    u8 rng;
};

struct PsynergyMenuState {
    u8 unknown_000[0x16c];
    u16 pane_action[2];
};

struct BtlAct *BtlActGet(s32 action);
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
    struct BtlAct *psynergy;
    s32 diff;
    s32 ret;
    /* FAKEMATCH: the barrier keeps the action's load ahead of the 0x3fff mask, and the action pinned to r3 keeps its register, as native; plain, the mask starts first (differ 2), the barrier alone loads into r0 (differ 8), and the pin alone changes nothing (differ 2). */
    register u32 action asm("r3");

    menu = Ram_HeapSlots->menu_runtime;
    action = menu->pane_action[0];
    asm volatile("");
    psynergy = BtlActGet(0x3fff & action);
#if defined(TLA_EDITION_ES) || defined(TLA_EDITION_IT)
    if (Func_080c84f8(psynergy->type) != 0)
#else
    if (BattleFx_HasMatchingEvent5Far(psynergy->type) != 0)
#endif
        return 0;
    ret = 2;
    if (psynergy->rng != 0xff) {
        u8 kind = psynergy->target_mode;

        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (u32)ret >> 31;
        ret = 1 - ret;
    }
    return ret;
}
