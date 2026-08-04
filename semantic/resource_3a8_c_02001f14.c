#include "types.h"

/*
 * resource_3a8 owner at 0x02001f14, 236 bytes.
 *
 * Complete owner: `push {lr}` at 0x02001f14 through the single interworking
 * epilogue `pop {r0} / bx r0` at 0x02001ff2.  Two ranges inside the span are
 * data, not code: the five-entry jump table at 0x02001f28-0x02001f3b and the
 * four-word literal pool at 0x02001ff8-0x02002007.  Both are reached only by
 * `ldr rN,[pc,#imm]` / `mov pc,r3`, never fallen into.  Nothing is live past
 * the return.
 *
 * Signature.  The epilogue is `pop {r0} ; bx r0`, so r0 holds the popped
 * return address and the owner returns nothing: `void`.  No incoming register
 * is read before the first write, so it takes no arguments either - this is a
 * per-frame step routine driven entirely by its own state word.
 *
 * LINK BASE 0x02008000, confirmed here independently.  The dispatcher loads
 * the table base as the pool word 0x02009f28, and the table it indexes sits at
 * file offset 0x1f28 = 0x02009f28 - 0x8000.  Its five entries 0x02009f3c /
 * 0x02009f58 / 0x02009f84 / 0x02009fb0 / 0x02009fdc likewise map to the five
 * case bodies at 0x1f3c / 0x1f58 / 0x1f84 / 0x1fb0 / 0x1fdc.  So the remaining
 * pool words 0x0200bec0 and 0x0200bfb0 are in-image data at offsets 0x3ec0 and
 * 0x3fb0, and only 0x0200d144 - above the image - is a RAM global.  That last
 * one is already named Data_0200d144 by the byte-exact sibling
 * assets/code/resource_3a8_c_02001ed8.c, which zeroes it just before this
 * sequence begins.  It is this state machine's step counter.
 *
 * Call accounting: 12 `bl` sites, resolved with
 * `bun tools/overlay_call_targets.ts resource_3a8 1f14` to exactly two import
 * veneers - 0x02003c1c -> Func_0808a080 (8 sites) and 0x02003c34 ->
 * Func_0808a098 (4 sites).  That matches the two distinct BL encodings in the
 * body (`f003 fe0d` x8, `f003 fe19` x4).  The disassembler's own annotations
 * print twelve different targets and are wrong in the usual overlay way (an
 * overlay `bl` stores `target_offset - 2`); they were not used.
 *
 * Func_0808a080(selector) returns the scene entity for a selector and
 * Func_0808a098(selector, script) starts a scripted motion on it; both
 * spellings come from the byte-exact and semantic siblings across this and
 * other overlays.  The halfword at entity+100 is the same completion word
 * those siblings increment and decrement; here it is cleared before a script
 * is started and polled with `ldrsh` (signed) to detect the script finishing.
 *
 * The overall shape is a five-step ping-pong between entities 20 and 21: start
 * a script, wait for it to complete, start the next.  Steps 1-4 return early
 * without advancing while the previous script is still running.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
u8 *Func_0808a080();                /* scene entity by selector */
void Func_0808a098();               /* start a scripted motion on an entity */

extern s32 Data_0200d144;           /* RAM: this sequence's step counter */

extern u8 Data_0200bec0[];          /* in-image script at offset 0x3ec0 */
extern u8 Data_0200bfb0[];          /* in-image script at offset 0x3fb0 */

/* The completion halfword of a scene entity.  Written out at every use: each
 * one is a SEPARATE Func_0808a080 call site in the assembly (eight of them),
 * so it is deliberately not a macro or a cached local. */

void Func_02001f14(void)
{
    s32 step = Data_0200d144;

    switch (step) {
    case 0:
        *(s16 *)(Func_0808a080(21) + 100) = 0;
        Func_0808a098(21, Data_0200bec0);
        Data_0200d144 = step + 1;
        break;

    case 1:
        if (*(s16 *)(Func_0808a080(21) + 100) == 0) {
            return;
        }
        *(s16 *)(Func_0808a080(20) + 100) = 0;
        Func_0808a098(20, Data_0200bfb0);
        Data_0200d144 = step + 1;
        break;

    case 2:
        if (*(s16 *)(Func_0808a080(20) + 100) == 0) {
            return;
        }
        *(s16 *)(Func_0808a080(20) + 100) = 0;
        Func_0808a098(20, Data_0200bec0);
        Data_0200d144 = step + 1;
        break;

    case 3:
        if (*(s16 *)(Func_0808a080(20) + 100) == 0) {
            return;
        }
        *(s16 *)(Func_0808a080(21) + 100) = 0;
        Func_0808a098(21, Data_0200bfb0);
        Data_0200d144 = step + 1;
        break;

    case 4:
        if (*(s16 *)(Func_0808a080(21) + 100) == 0) {
            return;
        }
        Data_0200d144 = 0;
        break;

    default:
        /* `cmp #4 / bhi` falls straight through to the epilogue. */
        break;
    }
}
