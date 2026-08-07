#include "types.h"

/*
 * Resource 37f, owner at 0x02000a24 (the scenario-0x10 step dispatcher).
 *
 * Extent: prologue `push {r5, r6, lr} / sub sp, #8` at 0x02000a24 through the
 * single interworking return `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x02000cda, plus the trailing literal pool at 0x02000ce4-0x02000d1b.  r0 is
 * the popped return address, so the owner is `void`.
 *
 * Pool map derived by a control-flow walk from the prologue, not from the
 * disassembler's "pool words referenced" list.  Two ranges are never reached
 * as instructions: the 16-entry jump table at 0x02000a78-0x02000ab7, which the
 * `bls`/`b` pair at 0x02000a6c branches around, and the literal pool after the
 * epilogue.  Register state is carried ACROSS the jump table: r1 is loaded
 * with the `Data_02000240` base at 0x02000a5c, before the dispatch, and is
 * still live in the step-8 case, which is the only user of it.
 *
 * Link base, third witness in this overlay: the jump-table base pool word is
 * 0x02008a78 while the table is physically at file offset 0xa78, and its
 * sixteen entries are likewise 0x8000 above the case bodies they select.  They
 * carry no Thumb bit, which is correct for `mov pc, r3`.  Decoded entries, by
 * `Data_02000240[225] - 1`:
 *   0,1 -> 0xab8   2 -> 0xb10   7 -> 0xb1e   10,11,12 -> 0xb42
 *   13,14,15 -> 0xc30   3,4,5,6,8,9 -> 0xcda (the shared exit)
 *
 * All 66 call sites were resolved with `tools/overlay_call_targets.ts` (the
 * inventory's `calls=4` is badly low here precisely because the jump table
 * disassembles as plausible code — the documented undercount).  62 are import
 * veneers and 4 are calls to this overlay's own routines: Func_02000d1c,
 * Func_02000f8c, Func_0200161c and Func_020017c0.
 *
 * Func_0808a080(id) is the scene-record accessor; its result is handed
 * straight to Func_080091e0(record, 0) in the long release loops, which is why
 * those pairs are written as one nested call each.
 */

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_080770c0();
void Func_080770c8();
void Func_0808a5e0();
void Func_080091f0();
void Func_0808a2f8();
void Func_08009180();
void Func_080091c0();
void Func_08009128();
s32 Func_0808a080();
void Func_080091e0();
void Func_0808a0f0();

void Func_02000d1c();
void Func_02000f8c();
void Func_0200161c(void);
void Func_020017c0();

void Func_02000a24(void)
{
    u8 *workspace;
    s16 *scenario;
    u32 step;
    s32 tint;

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 516;

    if (Func_080770c0(0x814) != 0) {
        Func_0808a5e0(141);
        Func_080091f0(0x10000, 0x10000, 0x10000);
        Func_0808a2f8();
    }

    if (step > 15) {
        return;
    }
    scenario = Data_02000240;
    step = (u32)(scenario[225] - 1);

    switch (step) {
    case 0:
    case 1:
        if (Func_080770c0(0x81a) == 0) {
            return;
        }
        Func_08009180(1, 109, 4, 81, 1, 1);
        Func_08009180(0, 70, 30, 42, 1, 1);
        Func_08009180(0, 29, 3, 1, 3, 2);
        Func_080091c0(0, 29, 3, 2, 3, 1);
        Func_08009128();
        return;

    case 2:
        Func_080091e0(Func_0808a080(9), 0);
        return;

    case 7:
        /* Both stores go through the r1 that was loaded before the dispatch. */
        scenario[288] = 0x10;
        return;
        scenario[289] = 8;
        if (Func_080770c0(0x802) != 0) {
            return;
        }
        Func_02000d1c();

    case 10:
    case 11:
    case 12:
        /* Fully UNROLLED in the assembly: 11 separate
         * Func_0808a080/Func_080091e0 pairs at 0x02000b44-0x02000bc2, not a loop. */
        Func_080091e0(Func_0808a080(9), 0);
        Func_080091e0(Func_0808a080(10), 0);
        Func_080091e0(Func_0808a080(11), 0);
        Func_080091e0(Func_0808a080(12), 0);
        Func_080091e0(Func_0808a080(13), 0);
        Func_080091e0(Func_0808a080(14), 0);
        Func_080091e0(Func_0808a080(15), 0);
        Func_080091e0(Func_0808a080(16), 0);
        Func_080091e0(Func_0808a080(17), 0);
        Func_080091e0(Func_0808a080(18), 0);
        Func_080091e0(Func_0808a080(19), 0);
        if (Func_080770c0(0x804) == 0) {
            Func_02000f8c();
        }

        if (Func_080770c0(0x303) != 0) {
            Func_0808a0f0(9, 0x5d80000, 0x880000);
        } else if (Func_080770c0(0x302) != 0) {
            Func_0808a0f0(9, 0x5f80000, 0x880000);
        }

        if (Func_080770c0(0x301) != 0) {
            tint = 227;
        } else {
            if (Func_080770c0(0x300) == 0) {
                return;
            }
            tint = 231;
        }
        Func_0808a0f0(10, tint << 19, 0x880000);
        return;

    case 13:
    case 14:
    case 15:
        /* Fully UNROLLED in the assembly: 6 separate
         * Func_0808a080/Func_080091e0 pairs at 0x02000c32-0x02000c74, not a loop. */
        Func_080091e0(Func_0808a080(9), 0);
        Func_080091e0(Func_0808a080(10), 0);
        Func_080091e0(Func_0808a080(11), 0);
        Func_080091e0(Func_0808a080(12), 0);
        Func_080091e0(Func_0808a080(13), 0);
        Func_080091e0(Func_0808a080(14), 0);
        if (Func_080770c0(0x825) == 0) {
            Func_0200161c();
        }
        Func_020017c0(1);
        Func_080770c8(564);

        if (Func_080770c0(0x821) == 0) {
            return;
        }
        Func_08009180(0, 71, 100, 71, 1, 1);
        Func_08009180(122, 20, 120, 30, 1, 2);
        Func_080091c0(122, 20, 1, 2, 120, 30);
        Func_08009128();
        return;

    default:
        return;
    }
}
