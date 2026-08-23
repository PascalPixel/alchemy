#include "types.h"

/*
 * resource_3bb owner at 0x02002940, 144 bytes: seed a second per-instance
 * task's state and pick which of five mode-table addresses it dispatches
 * through.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02002940 through `pop {r5, r6} /
 * pop {r0} / bx r0` at 0x02003460-0x02003464, followed by the 15-word literal
 * pool 0x02003444-0x02003467 that closes the 144-byte span; the next owner's
 * prologue is at 0x02003468.  Two arguments (r0, r1), void.
 *
 * Not found by the structural inventory walk (unindexed): the one call site,
 * 0x020033ee, resolves with the `+2` rule to veneer 0x02004848 ->
 * Func_080000d0, the same task installer as the sibling
 * resource_3bb_c_02003abc.c.  The installed callback, 0x0200a451 under this
 * overlay's proven 0x02008000 link base, is this overlay's own
 * 0x02002ee8 -- outside this mandate's called-function list (never reached by
 * `bl`, only installed) and not drafted here.
 *
 * The branch chain 0x020033f4-0x0200340a picks one of five addresses by
 * `mode` (and, only when `mode == 3`, by whether `param` is zero) and stores
 * the selected one as a 32-bit value into Data_0200cc40 -- a mode-table /
 * handler-select pattern.  Whether the five addresses are code (dispatch
 * targets) or data (per-mode records) is not resolved here; they are typed as
 * plain addresses.  The four sibling stores that follow
 * (Data_0200cc3c/dc38/dbac cleared as halfwords, Data_0200cc10 cleared as a
 * word) reset the rest of the task's state block alongside the handler
 * selection.
 */

extern u16 Data_0200cc30;
extern u16 Data_0200cc04;
extern u16 Data_0200cc3c;
extern s32 Data_0200cc40;
extern u16 Data_0200cc98;
extern u16 Data_0200cc0c;
extern s32 Data_0200cc10;

extern s32 Data_0200ca1e;   /* default handler (mode not 2/3/4) */
extern s32 Data_0200c0ce;   /* handler for mode == 2 */
extern s32 Data_0200ca4a;   /* handler for mode == 4 */
extern s32 Data_0200c0f6;   /* handler for mode == 3, param != 0 */
extern s32 Data_0200cac8;   /* handler for mode == 3, param == 0 */

void Func_02006700(s32 taskAddress, s32 frameBudget);

void Func_02002940(u32 mode, u32 param)
{
    s32 handler;

    Data_0200cc30 = (u16)mode;
    Data_0200cc04 = (u16)(param << 4);

    {
        s32 budget = 0xc80;
        s32 task = 0x0200a451;
        Func_02006700(task, budget);
    }

    handler = (s32)&Data_0200ca1e;
    if (mode == 2) {
        handler = (s32)&Data_0200c0ce;
    }
    if (mode == 4) {
        handler = (s32)&Data_0200ca4a;
    }
    if (mode == 3) {
        if (param != 0) {
            handler = (s32)&Data_0200c0f6;
        } else {
            handler = (s32)&Data_0200cac8;
        }
    }

    Data_0200cc3c = 0;
    Data_0200cc40 = handler;
    Data_0200cc98 = 0;
    Data_0200cc0c = 0;
    Data_0200cc10 = 0;
}
