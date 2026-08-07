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

s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_0808a0f0();
void Func_020013b4();
void Func_0808a1e0();
s32 Func_0808a080();
void Func_080091e0();
void Func_08009188();
void Func_0808a100();
void Func_02000a0c();
void Func_02000578();
void Func_020013e4();
void Func_0808a2c8();
void Func_0808a2d8();
void Func_080000c0();
void Func_0808a360();
void Func_0808a370();
void Func_0808a2e0();
void Func_08009128();

s32 Func_020003f8(void)
{
    s16 scene;

    if (Data_02000240[225] == 19) {
        Func_080770d0(0x12F);
        *(s32 *)(Data_03001ebc + 448) = 521;
        return 0;
    }

    if (Func_080770c0(0x834) != 0) {
        Func_0808a0f0(11, 0, 0);
        Func_0808a0f0(12, 0, 0);
        Func_0808a0f0(13, 0, 0);
        Func_0808a0f0(14, 0, 0);
        Func_0808a0f0(15, 0, 0);
        Func_0808a0f0(16, 0, 0);
    } else {
        Func_020013b4();
    }

    Func_0808a1e0(13, 1);

    if (Func_080770c0(0x87A) != 0) {
        Func_080091e0(Func_0808a080(17), 0);

        /* selector 6 or 7 only; see the unsigned compare noted above */
        if ((u16)((u16)Data_02000240[225] - 6) <= 1) {
            if (Func_080770c0(0x109) != 0) {
                if (Func_080770c0(0x203) != 0) {
                    Func_08009188(12);
                }
            } else {
                Func_08009188(11);
                Func_080091e0(Func_0808a080(8), 0);
                Func_0808a100(8, 10);
            }
        }
        return 0;
    }

    scene = Data_02000240[225];
    if (scene == 21) {
        Func_02000a0c();
    } else if (scene == 20) {
        Func_080770c8(0x834);
        Func_02000578();
    } else if (scene == 22) {
        Func_020013e4();
    } else {
        *(s32 *)(Data_03001ebc + 448) = 521;
        if (Func_080770c0(0x834) != 0) {
            Func_0808a2c8();
            *(u16 *)(((u8 **)&Data_03001ebc)[3] + 0x1F84) = 1;
            Func_0808a2d8();
            Func_080000c0(30);
            Func_0808a360();
            Func_0808a370();
            Func_0808a2e0();
        } else {
            Func_08009128();
            Func_080000c0(1);
        }
    }

    return 0;
}
