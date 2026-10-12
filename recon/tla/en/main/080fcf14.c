/* Draft, differ 2 of 34: The Broken Seal's PsynergyMenu_ClassifySelectedPsynergy. The native loads
 * the selected action before it starts the 0x3fff mask (ldrh r3, [r3]; movs r0, #252); this form
 * starts the mask first. The mask on the other side of the and is the same; a temporary with a
 * one-pass block or an empty asm after the load is differ 8. */
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
s32 BattleFx_HasMatchingEvent5Far(s32 type);

// 選んでいるエナジーの使い方：0 = その場で働く、1 = 相手を選ぶ、2 = 使えない
s32 PsynergyMenu_ClassifySelectedPsynergy(void)
{
    struct BtlAct *psynergy;
    s32 diff;
    s32 ret;

    psynergy = BtlActGet(
        0x3fff & ((struct PsynergyMenuState *)Ram_HeapSlots->menu_runtime)->pane_action[0]);
    if (BattleFx_HasMatchingEvent5Far(psynergy->type) != 0) {
        return 0;
    }
    ret = 2;
    if (psynergy->rng != 0xff) {
        u8 kind = psynergy->target_mode;
        diff = kind ^ 2;
        ret = (0 - diff) | diff;
        ret = (u32)ret >> 0x1f;
        ret = 1 - ret;
    }
    return ret;
}
