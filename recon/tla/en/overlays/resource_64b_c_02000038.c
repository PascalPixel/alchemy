/* Near miss of the six world-map callbacks Func_02000038, 0200009c, 02000100, 02000164,
 * 020001c8 and 0200022c (100 bytes each; they differ only in the two arguments of
 * Func_02004d50: 0x80a/8, 0x80b/28, 0x80c/31, 0x80d/72, 0x80e/13, 0x80f/53).
 * As below every instruction and register is the native's, but the first three rows come as
 * `movs r1, #128; movs r3, #192; ldr r2, =gPartyState` where the native has the ldr first.
 * alchemy diff prints this as a pool-offset difference only; alchemy show after a build
 * shows the order. Measured against it:
 *  - a party pointer declared first: movs r1; ldr r2; movs r3 (one row late);
 *  - that pointer and a one-pass block after it: ldr r2 first, but movs r3 before movs r1;
 *  - that pointer behind a "+l" barrier: the native order, but party in r1 and the offset in
 *    r2, the work in r7;
 *  - a mode pointer or a mode value taken before the work load: other registers.
 * Queue 10, from the sched2 dump (alchemy diff --rtl): the first block's chains are
 * movs r1 (priority 7) -> lsls (6) -> adds #18 (5) -> adds r3, r2, r1 (4); ldr r2 (6, a
 * load's cost 2 over that 4); movs r3 (6) -> lsls (5) -> ldr r6 (4). Ties fall to RTL order.
 * The native order (ldr r2; movs r1; movs r3; lsls r1; lsls r3; adds r1; ldr r6) is this
 * schedule with ldr r2 alone taken first, so the ldr either outranked movs r1 or stood
 * alone before a scheduling barrier with nothing else ahead of it. Measured:
 *  - a loop note as the first statement, then the party pointer, then the work: no effect
 *    (movs r1; ldr r2; movs r3), the note has nothing before it to bind;
 *  - the pointer, a note, then the offset 0x212 as a local before the work load: the byte
 *    is read as ldrb r3, [r1, r3] and the registers move (differ 31).
 * The Japanese build has no render mode test (its listing is the first Func_02000038 of
 * resource_64b_overlay.s), so the test is a later addition around the old body. */
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"

u32 Random16Far(void);
void Func_02004d50(s32 a, s32 b);

void Func_02000038(void)
{
    u8 *work = (u8 *)Ram_HeapSlots->event_work;

    if (gPartyState.render_mode == 1) {
        s32 *last = (s32 *)((u8 *)&gPartyState + 0x258);
        s32 *steps = (s32 *)(work + 0x198);

        if (*last >= *steps * 9 / 10) {
            if (Random16Far() < 0x8000) {
                Func_02004d50(0x80a, 8);
                *(s32 *)(work + 0x194) = 0;
            } else {
                *last = *steps;
            }
        }
    }
}
