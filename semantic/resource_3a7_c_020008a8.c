/*
 * resource_3a7 one-shot cutscene beat at 0x020008a8, 156 bytes.
 *
 * Complete owner: `push {r5, r6, lr} / sub sp, #8` at 0x020008a8 through
 * `add sp,#8 / pop {r5, r6} / pop {r0} / bx r0` at 0x02000932.  The return
 * address is popped into r0, so the owner is `void`.  The words at 0x0200093c
 * (0x000009a8) and 0x02000940 (0x00001528) are the literal pool - past the
 * epilogue, inside the 156-byte span, and data.
 *
 * Same one-shot gate as 0x02000368 / 0x0200048c / 0x020004d0 and as the
 * byte-exact sibling assets/code/resource_3a7_c_020003e0.c: test a flag, run the
 * beat only while it is clear, raise it immediately.  This owner's flag is
 * 0x9a8, one below 0x02000368's 0x9a9 and two below the 0x9aa of the
 * 0x0200048c/0x020004d0 pair - consecutive one-shot ids for consecutive beats.
 *
 * Call targets resolved with
 *   bun tools-rs/overlay-show resource_3a7 08a8 -n 156 |
 *   bun tools/lib/overlay_call_targets.ts resource_3a7 --annotate
 * Per-target multiset over the 14 sites, matching the inventory's calls=14:
 *   Func_080770c0 x1 <- veneer 0x02001100  (test flag 0x9a8)
 *   Func_08015040 x1 <- veneer 0x020010f8
 *   Func_080770c8 x1 <- veneer 0x02001108  (raise flag 0x9a8)
 *   Func_080f9010 x2 <- veneer 0x02001150  (scripted delays, 0x9b then 0x9c)
 *   Func_080091b8 x4 <- veneer 0x020010c8  (six-argument scene requests)
 *   Func_0808a010 x4 <- veneer 0x02001118
 *   Func_02000754 x1  (this overlay's own prologue - the beat's tail)
 *
 * r5 (27) and r6 (92, later 25) are constant holders for the two stack words of
 * the six-argument request ABI; r6 is REASSIGNED at 0x02000900 from 92 to 25,
 * which is the documented "long-lived alias reassigned mid-owner" trap, so the
 * value is tracked per use below rather than as one variable.
 *
 * Uncertainties: the request ids and the Func_0808a010 arguments are
 * transcribed, not interpreted.  The delay arguments 0x9b/0x9c sit numerically
 * next to the flag ids in this overlay but are passed to a different import, and
 * the byte-exact sibling passes 0x50 to the same one, so they are frame counts
 * rather than ids.
 */

/* Old-style declarations: arities vary between call sites in this overlay.
 * The flag test must still declare a return type because it is tested. */
#include "types.h"

s32 Func_020019b0();
void Func_020019b4();
void Func_020019ca();
void Func_02001a18();
void Func_020019a4();
void Func_020019fa();
void Func_020019ba();
void Func_02001a10();
void Func_02001a4e();
void Func_020019d8();
void Func_02001a2e();
void Func_020019ee();
void Func_02001a44();
void Func_02001084();
                        /* test a scene completion flag */
                     
                        /* raise a scene completion flag */
                        /* scripted delay, in frames */
                        /* six-argument scene request, last two on the stack */

void Func_020008a8(void)
{
    if (Func_020019b0(0x9a8) != 0) {
        return;
    }

    Func_020019b4(0x1528, 1);
    Func_020019ca(0x9a8);
    Func_02001a18(0x9b);

    Func_020019a4(107, 27, 1, 1, 92, 27);
    Func_020019fa(39);
    Func_020019ba(108, 27, 1, 1, 92, 27);
    Func_02001a10(50);

    Func_02001a4e(0x9c);

    Func_020019d8(1, 24, 1, 2, 25, 27);
    Func_02001a2e(40);
    Func_020019ee(2, 24, 1, 2, 25, 27);
    Func_02001a44(40);

    Func_02001084();
}
