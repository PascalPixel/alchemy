#include "types.h"

/*
 * resource_377 owner at 0x020003f8, 384 bytes (0x03f8..0x0557), followed by its
 * literal pool at 0x0558..0x0577 and the next owner's prologue at 0x0578.
 * The pool was mapped by walking control flow from the prologue: the final
 * 'movs r0,#0' join at 0x02000550 is reached by every arm, and the epilogue
 * 'pop {r5} ; pop {r1} ; bx r1' at 0x02000552 ends the owner, so nothing after
 * 0x0557 is code.  Pool words: 0x02000240, 0x0000012f, 0x03001ebc, 0x00000834,
 * 0x0000087a, 0x00000109, 0x00000203, 0x00001f84.
 *
 * Signature: the interworking return pops into r1, not r0, so r0 survives and
 * IS the result -- the owner returns s32.  Every path funnels through the
 * 'movs r0,#0' join, so the value returned is always 0.  Nothing reads r0 on
 * entry, so the owner takes no argument.
 *
 * Data_02000240[225] is the cross-overlay scene/mode selector (the signed
 * halfword at byte offset 450); assets/code/resource_36f_c_02000054.c and
 * resource_371_c_02000030.c spell the same array.  Note the second read at
 * 0x02000492 is an 'ldrh' (unsigned) while the two others are 'ldrsh'; the
 * unsigned one is compared as '((u16)(v - 6) << 16) > (128 << 9)', i.e.
 * (u16)(v - 6) > 1, so the guarded arm runs for selector 6 and 7 only.
 *
 * Data_03001ebc is the family workspace POINTER CELL, as in the byte-exact
 * sibling assets/code/resource_377_c_020002e0.c, which writes the same
 * 'displacement 448 / adds #73 -> value 521' additive store.  The word at
 * 0x03001ec8 (r5+12 at 0x02000524) is a second pointer in the same cell block;
 * it is spelled here as ((u8 **)&Data_03001ebc)[3].  Uncertainty: only this
 * owner uses that +12 cell in resource_377, so its role beyond "base of a
 * record whose u16 at +0x1f84 is set to 1" is not established here.
 *
 * All 33 call sites resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 03f8 --json' (the inventory's
 * calls=32 is the documented undercount).  Per-target multiset, site -> callee:
 *   0x040c Func_080770d0                       x1
 *   0x0422 0x0476 0x04a2 0x04ac 0x0518 Func_080770c0   x5
 *   0x0430 0x043a 0x0444 0x044e 0x0458 0x0462 Func_0808a0f0 x6
 *   0x0468 Func_020013b4 (prologue, exact sibling in assets/code) x1
 *   0x0470 Func_0808a1e0                       x1
 *   0x0480 0x04c4 Func_0808a080                x2
 *   0x0486 0x04ca Func_080091e0                x2
 *   0x04b6 0x04be Func_08009188                x2
 *   0x04d2 Func_0808a100                       x1
 *   0x04e8 Func_02000a0c (prologue, this overlay) x1
 *   0x04f4 Func_080770c8                       x1
 *   0x04f8 Func_02000578 (prologue, this overlay) x1
 *   0x0502 Func_020013e4 (prologue, this overlay) x1
 *   0x0520 Func_0808a2c8   0x052e Func_0808a2d8
 *   0x0534 0x054c Func_080000c0                x2
 *   0x0538 Func_0808a360   0x053c Func_0808a370
 *   0x0540 Func_0808a2e0   0x0546 Func_08009128
 * The C below contains exactly those counts.  The disassembler's own 'bl'
 * annotations are wrong, as on every overlay.
 */

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;






















extern void Func_02001d0a();
extern s32 Func_02001d10();
extern void Func_02001da6();
extern void Func_02001db0();
extern void Func_02001dba();
extern void Func_02001dc4();
extern void Func_02001dce();
extern void Func_02001dd8();
extern void Func_0200181e();
extern void Func_02001e6e();
extern s32 Func_02001d64();
extern void Func_02001d2c();
extern s32 Func_02001dae();
extern s32 Func_02001d90();
extern s32 Func_02001d9a();
extern void Func_02001d3c();
extern void Func_02001d44();
extern void Func_02001d70();
extern s32 Func_02001df2();
extern void Func_02001e50();
extern void Func_02000ef6();
extern void Func_02001dea();
extern void Func_02000a72();
extern void Func_020018e8();
extern s32 Func_02001e06();
extern void Func_02001f5e();
extern void Func_02001f7c();
extern void Func_02001d4a();
extern void Func_02001f96();
extern void Func_02001faa();
extern void Func_02001db4();
extern void Func_02001d62();
s32 Func_020003f8(void)
{
    s16 scene;
    s32 sel;

    if (Data_02000240[225] == 19) {
        Func_02001d0a(0x12F);
        *(s32 *)(Data_03001ebc + 448) = 521;
        return 0;
    }

    if (Func_02001d10(0x834) != 0) {
        Func_02001da6(11, 0, 0);
        Func_02001db0(12, 0, 0);
        Func_02001dba(13, 0, 0);
        Func_02001dc4(14, 0, 0);
        Func_02001dce(15, 0, 0);
        Func_02001dd8(16, 0, 0);
    } else {
        Func_0200181e();
    }

    Func_02001e6e(13, 1);

    if (Func_02001d64(0x87A) != 0) {
        Func_02001d2c(Func_02001dae(17), 0);

        /* selector 6 or 7 only; see the unsigned compare noted above */
        sel = (u16)Data_02000240[225];
        if ((u16)(sel - 6) <= 1) {
            if (Func_02001d90(0x109) != 0) {
                if (Func_02001d9a(0x203) != 0) {
                    Func_02001d3c(12);
                }
            } else {
                Func_02001d44(11);
                Func_02001d70(Func_02001df2(8), 0);
                Func_02001e50(8, 10);
            }
        }
        return 0;
    }

    scene = Data_02000240[225];
    if (scene == 21) {
        Func_02000ef6();
    } else if (scene == 20) {
        Func_02001dea(0x834);
        Func_02000a72();
    } else if (scene == 22) {
        Func_020018e8();
    } else {
        *(s32 *)(Data_03001ebc + 448) = 521;
        if (Func_02001e06(0x834) != 0) {
            Func_02001f5e();
            *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1F84) = 1;
            Func_02001f7c();
            Func_02001d4a(30);
            Func_02001f96();
            Func_02001faa();
            Func_02001f96();
        } else {
            Func_02001db4();
            Func_02001d62(1);
        }
    }

    return 0;
}
