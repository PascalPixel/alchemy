#include "types.h"

/*
 * Resource 37f, owner at 0x0200007c (76 bytes of code + a seven-word literal
 * pool at 0x020000cc-0x020000eb).  `push {r5, lr}` at 0x0200007c,
 * interworking return `pop {r5} / pop {r1} / bx r1` at 0x020000c6: the popped
 * register is r1, so r0 survives and IS the result — the owner returns the
 * selected in-image data pointer.
 *
 * The single call site was resolved with `tools/overlay_call_targets.ts` and
 * is the import veneer for Func_0808a038.
 *
 * Selector: the same `Data_02000240` scenario array the byte-exact
 * `assets/code/resource_37f_c_02000030.c` uses — element 224 is the scenario
 * id (built as base + 448 with an `ldrsh` at index 0) and element 225 (base +
 * 450) is a sub-step.  Every returned pool word is EVEN and lies in
 * 0x02009xxx/0x0200axxx, so under this overlay's proven 0x02008000 link base
 * they are in-image data blocks (file offsets 0x1fc0, 0x1fd8, 0x2050, 0x21b8,
 * 0x22a8), not callbacks.
 *
 * Note the `< 11` and `> 16` arms share one body: the same block is entered by
 * both branches, so it is written once here rather than duplicated.
 */

extern s16 Data_02000240[];
extern u8 Data_02009fc0[];
extern u8 Data_02009fd8[];
extern u8 Data_0200a050[];
extern u8 Data_0200a1b8[];
extern u8 Data_0200a2a8[];

void Func_0808a038();

u8 *Func_0200007c(void)
{
    s16 scenario = Data_02000240[224];
    s16 step;

    if (scenario == 0x10) {
        step = Data_02000240[225];
        if (step >= 11 && step <= 13) {
            return Data_0200a050;
        }
        if (step >= 14 && step <= 16) {
            return Data_0200a1b8;
        }
        Func_0808a038(Data_02009fd8);
        return Data_02009fd8;
    }

    if (scenario == 0x13) {
        return Data_0200a2a8;
    }

    return Data_02009fc0;
}
