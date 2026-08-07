#include "types.h"

/*
 * Resource 3ad, owner at 0x02000094 (64 bytes, 3 call sites).
 *
 * Complete owner: `push {lr}` / `sub sp, #8` prologue at 0x02000094 and the
 * matching `add sp, #8 / pop {r0} / bx r0` interworking return at 0x020000ce.
 * The popped register is r0, so r0 holds the return address and nothing is
 * returned - the owner is `void`.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts` (an overlay `bl`
 * stores target_offset - 2, so the disassembler's own annotations are wrong):
 *   0x0200009a -> veneer 0x02001b3c -> Func_0808a080
 *   0x020000c2 -> veneer 0x02001aec -> Func_080091c0
 *   0x020000ca -> veneer 0x02001b0c -> Func_080770c8
 * Per-target multiset: Func_0808a080 x1, Func_080091c0 x1, Func_080770c8 x1,
 * matching the three C call expressions below.  (The inventory row says
 * calls=2; that field counts distinct pre-correction targets and is a known
 * lower bound.)
 *
 * No literal pool lies inside the span - every constant is an immediate.
 */

/* Old-style declarations: overlay import arities vary per call site. */
u8 *Func_0808a080();    /* scene entity record by selector */
void Func_080091c0();   /* six-argument renderer; last two arguments on the stack */
void Func_080770c8();   /* set a story flag */

void Func_02000094(void)
{
    u8 *entity;

    entity = Func_0808a080(9);
    if (entity != 0) {
        /*
         * Two byte fields of the entity record.  The disassembly forms each
         * address with `adds rN, r0, #0 / adds rN, #imm / strb`, so both are
         * unsigned byte stores at fixed offsets.
         */
        entity[0x23] = 1;
        entity[0x55] = 0;
    }

    Func_080091c0(7, 32, 1, 1, 8, 32);

    /* 0x81 << 2 = 0x204, built as `movs r0,#129 / lsls r0,#2`. */
    Func_080770c8(0x204);
}
