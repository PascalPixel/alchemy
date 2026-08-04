#include "types.h"

/*
 * Resource 3ae, owner at 0x02000958 (378 bytes; the four-word pool at
 * 0x02000a84..0x02000a96 sits INSIDE the body and is branched over by the lone
 * forward `b.n 0x02000a98` at 0x02000a80, so the owner continues past it to
 * the epilogue at 0x02000aca).
 *
 * Role known in advance from the call graph: `Func_0200086c` dispatches on the
 * workspace room id and calls this owner for room 0x70 with no arguments — the
 * per-room setup hook for that room.
 *
 * Prologue `push {r5,r6,lr}` + `sub sp,#8` at 0x02000958; epilogue
 * `add sp,#8 / pop {r5,r6} / pop {r0} / bx r0`.  The return address is popped
 * into r0, so r0 does not survive and the owner is void.  The 8-byte frame
 * exists only to carry the fifth and sixth arguments of Func_080091c0.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts` (an overlay `bl`
 * stores target offset - 2; the disassembler's annotation is wrong):
 *
 *   0x0200095c -> Func_02000af8 (prologue, this overlay)
 *   Func_080770c0 x4 (flag test), Func_080770d0 x2, Func_0808a158,
 *   Func_0808a0f0 x5, Func_0808a1b8 x5, Func_0808a080 x2, Func_080091c0 x2
 *
 * 22 call sites, matching the inventory's calls=22 exactly, so the call set is
 * complete.
 *
 * Workspace: r5 = `&Data_02000240[225]`, the secondary room sub-state selector
 * also used by Func_0200086c and Func_020008cc.  It is read once signed
 * (`ldrsh`, compared against 3) and then twice unsigned (`ldrh`); the unsigned
 * read is re-done after the Func_080770d0 call because that call may change it.
 * The second test is `(u16)state << 16 == 0x80 << 9`, i.e. the halfword equals
 * 0x10000 >> 16 ... concretely `lsls r3,r2,#16` puts the halfword in the top
 * half and compares it against `0x80 << 9 == 0x10000`, which is true only when
 * the halfword is 1.  It is written that way below.
 *
 * Placement arguments to Func_0808a0f0 are 16.16 fixed point (`movs rN,#k /
 * lsls rN,rN,#17` is `2k << 16`, i.e. 2k.0; `lsls rN,rN,#16` is k.0).  The
 * Func_0808a1b8 second argument is a 16-bit facing angle, as in the sibling
 * `semantic/overlays/resource_3ae_c_020002dc.c`.
 *
 * Func_0808a080(id) returns the object record for a slot (same import and same
 * use as in resource_3ae_c_020002dc.c).  Its field 0x50 is a pointer to a
 * sprite/attribute sub-record; the byte writes at +0x23, +0x59 and the |= 12
 * bit sets at sub+9 / sub+21 are flag bytes, and the halfword at sub+30 takes
 * the angle 0xc000.
 *
 * Func_080091c0 is called with the established six-argument renderer ABI
 * (r0-r3 plus two stack words).
 *
 * Uncertainties: 0x950, 0x8aa, 0x8ab, 0x12f are read as event-flag ids from
 * their position as the sole argument of the Func_080770cX family.  The field
 * offsets on the Func_0808a080 record are taken from the instruction encodings
 * only; no independent layout witness for +0x23/+0x59 exists in this overlay.
 */

/* Old-style declarations: overlay imports vary in arity between call sites. */
int Func_080770c0();
void Func_080770d0();
void *Func_0808a080();
void Func_0808a0f0();
void Func_0808a158();
void Func_0808a1b8();
void Func_080091c0();

void Func_02000af8(void);

extern s16 Data_02000240[];

void Func_02000958(void)
{
    u8 *object;
    u8 *sub;

    Func_02000af8();

    /* movs r0,#149 / lsls r0,r0,#4 == 0x950 */
    if (Func_080770c0(0x950) != 0) {
        Func_0808a158(12, 2);
    }

    if (Data_02000240[225] == 3) {
        Func_080770d0(0x12f);
    }

    if ((u16)Data_02000240[225] == 1) {
        Func_080770d0(0x8aa);
    }

    if (Func_080770c0(0x8aa) != 0) {
        Func_0808a0f0(8, 408 << 16, 296 << 16);
        Func_0808a1b8(8, 0x8000, 0);
    }

    if (Func_080770c0(0x8ab) != 0) {
        Func_0808a0f0(13, 280 << 16, 296 << 16);
        Func_0808a1b8(13, 0xc000, 0);

        Func_0808a0f0(16, 288 << 16, 280 << 16);
        Func_0808a1b8(16, 0xe000, 0);

        Func_0808a0f0(10, 232 << 16, 304 << 16);
        Func_0808a1b8(10, 0x4000, 0);

        Func_0808a0f0(11, 240 << 16, 312 << 16);
        Func_0808a1b8(11, 0xc000, 0);

        object = (u8 *)Func_0808a080(10);
        object[0x59] = 0;
        object[0x23] = 2;
        sub = *(u8 **)(object + 0x50);
        sub[9] |= 12;
        sub = *(u8 **)(object + 0x50);
        sub[0x26] = 0;
        sub = *(u8 **)(object + 0x50);
        *(u16 *)(sub + 30) = 0xc000;

        object = (u8 *)Func_0808a080(11);
        object[0x23] = 0;
        sub = *(u8 **)(object + 0x50);
        sub[9] |= 12;
        sub = *(u8 **)(object + 0x50);
        sub[21] |= 12;
    }

    if (Func_080770c0(0x950) != 0) {
        Func_080091c0(18, 18, 1, 1, 14, 18);
        Func_080091c0(18, 18, 1, 1, 15, 18);
    }
}
