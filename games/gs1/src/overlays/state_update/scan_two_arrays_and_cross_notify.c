#include "types.h"

/*
 * Resource 3b1 unindexed helper at 0x020063ac (116 bytes, 2 calls).
 *
 * Derived span: no inventory row (item 28's unindexed population). `push
 * {r5,r6,r7,lr}` plus `fp`/`r9`/`r8` spill at 0x020063ac, epilogue
 * `pop {r3,r5,r6} / mov r8,r3 / mov r9,r5 / mov sl,r6 / pop {r5,r6,r7} /
 * pop {r0} / bx r0` at 0x02006412-0x0200641e. No literal pool for this
 * owner itself -- the bytes immediately after (0x02006420 onward) are a
 * `ldr r4,[pc,#0] / bx r4` ARM-interworking veneer table into main ROM
 * (targets like 0x080000c1), a recognized structural category the
 * item-28 scan's prologue test correctly does not flag, so the span is
 * exactly 0x020063ac-0x02006420, 116 bytes.
 *
 * Two first-match linear scans (index 0..8 inclusive, each breaking out
 * on the first hit and calling a per-element handler), then two more
 * calls that deliberately cross-pair each scan's *miss count* against
 * the *other* array: `cnt_a` (from scanning `a`) indexes into `b`, and
 * `cnt_b` (from scanning `b`) indexes into `a`.
 *
 * Call symbols are per-site, not per-import: the raw region (games/gs1/assets/code/
 * resource_3b1_overlay.s around line 9152) spells the six calls as
 * `bl sub_0200c876`, `bl sub_0200c890`, `bl sub_0200c896`, `bl sub_0200c8b0`,
 * `bl sub_0200c8b0` (again), `bl sub_0200c8b8` -- five distinct local
 * veneers (the second scan's hit handler and the first `GameFlag_Set` call
 * share sub_0200c8b0).  A single shared main-image-style name per logical
 * import (as this file previously used) produces the wrong pc-relative
 * displacement at every site; use the exact local sub_ symbols instead.
 */

s32 Func_0200c876();
void Func_0200c890();
s32 Func_0200c896();
void Func_0200c8b0();
void Func_0200c8b8();

void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b)
{
    s32 cnt_a = 0;
    s32 cnt_b = 0;
    u32 i;

    for (i = 0; i <= 8; i++) {
        u8 *p = a + i;
        if (Func_0200c876(p) != 0) {
            Func_0200c890(p);
            break;
        }
        cnt_a++;
    }

    for (i = 0; i <= 8; i++) {
        u8 *p = b + i;
        if (Func_0200c896(p) != 0) {
            Func_0200c8b0(p);
            break;
        }
        cnt_b++;
    }

    Func_0200c8b0(b + cnt_a);
    Func_0200c8b8(a + cnt_b);
}
