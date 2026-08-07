#include "types.h"

/*
 * Resource 3b6 scene dispatcher at 0x020003dc.
 *
 * Complete owner.  The prologue is the two-part high-register form
 * `push {r5, r6, lr} / mov r6, r8 / push {r6}` plus `sub sp, #8` for the
 * stack-carried arguments of the six-argument call below.  The matching
 * epilogue at 0x020004e4 is `add sp, #8 / pop {r3} / mov r8, r3 /
 * pop {r5, r6} / pop {r1} / bx r1`.
 *
 * Signature.  The saved return address is popped into r1, so r0 survives and
 * IS the result.  r0 is set to 0 at the single join 0x020004e2 that both exit
 * paths reach, so the owner returns s32 0 unconditionally.  It takes no
 * argument.  A four-word literal pool closes the row at
 * 0x020004f0-0x020004ff.
 *
 * Eighteen `bl` sites, matching the row's advertised call count exactly.  All
 * resolve with tools/overlay_call_targets.ts — an overlay `bl` stores
 * `target_offset - 2`, so the disassembler's labels are artefacts.  Sixteen
 * are import veneers, named for the main-image address in each entry's
 * trailing word.
 *
 * TWO ARE OVERLAY-INTERNAL CALLS, and they are the reason this owner is the
 * root of the overlay's call graph: 0x02000496 reaches the real prologue at
 * 0x0200013c and 0x020004c0 reaches the real prologue at 0x02000328.  Both
 * targets are genuine function starts with their own `push`/`pop` pairs, so
 * these are ordinary calls, not the hidden-context shape that the skip rule
 * covers.
 *
 * 0x03001ebc is a pointer CELL: `ldr r3,[pc]` loads the cell address and
 * `ldr r3,[r3]` loads the workspace pointer out of it, matching the spelling
 * in byte-exact assets/code/resource_3b6_c_0200009c.c.  That source also
 * writes `*(s32 *)(state + 0x1C0)`, the same field this owner writes, which
 * confirms both the offset and its s32 width.
 *
 * UNCERTAINTY, recorded rather than resolved.  The pool word 0x02000240 is
 * NOT an in-image address: this overlay is linked at 0x02008000, so only pool
 * words in 0x0200_8xxx..0200_bxxx are image offsets, and 0x02000240 falls
 * below that band.  It is therefore a fixed EWRAM record, and it is written
 * here as its own object.  Its 0x1c2 offset sits suspiciously close to the
 * 0x1c0 field written through the 0x03001ebc workspace pointer above, so the
 * two may well be the same record — but if they were, the s32 0x209 stored at
 * +0x1c0 would leave 0 in the halfword at +0x1c2 and both scene tests below
 * would be dead.  That contradiction is not resolved from this row alone, so
 * the two are kept distinct, which is faithful to the assembly either way.
 */

extern u8 *Data_03001ebc;

/* Old-style declarations: interfaces vary by call site across this overlay. */
s32 Func_080770c0();   /* query a numeric id; nonzero when selected */
void Func_080770c8();  /* set that same id */
void Func_080091c0();  /* six arguments, the last two carried on the stack */
u8 *Func_0808a080();   /* fetch a record by slot */
void Func_0808a0f0();
void Func_0808a1b8();
void Func_0808a360();

/* Overlay-internal callees, both real prologues in this image. */
void Func_0200013c(void);
void Func_02000328(void);

s32 Func_020003dc(void)
{
    u8 *workspace;
    u8 *record;
    u8 *attributes;
    s16 *scene;

    workspace = Data_03001ebc;

    /* 224 << 1 = 0x1c0; the stored value is that 448 plus 73, i.e. 521. */
    *(s32 *)(workspace + 0x1c0) = 521;

    if (Func_080770c0(0x950) == 0) {
        return 0;
    }

    /* Arguments five and six go to [sp, #0] and [sp, #4]. */
    Func_080091c0(51, 47, 3, 1, 51, 45);

    /*
     * The same two-step edit is applied to slots 31 and 32.  These are two
     * call sites of 0x0808a080 and are kept separate; the mask -13 clears bits
     * 0, 2 and 3 before bit 3 is set.
     */
    record = Func_0808a080(31);
    attributes = *(u8 **)(record + 0x50);
    attributes[9] = (u8)((attributes[9] & ~13) | 8);
    record[0x23] = 0;

    attributes = *(u8 **)(record + 0x50);
    record = Func_0808a080(32);
    record[0x23] = 0;
    attributes[9] = (u8)((attributes[9] & ~13) | 8);

    if (Func_080770c0(0x8bc) != 0) {
        /* 140 << 18 = 0x2300000, 170 << 18 = 0x2a80000, 128 << 8 = 0x8000. */
        Func_0808a0f0(25, 0x2300000, 0x2a80000);
        Func_0808a1b8(25, 0x8000, 0);
    }

    /* 225 << 1 = 0x1c2, read signed (ldrsh). */
    scene = (s16 *)((u8 *)0x02000240 + 0x1c2);

    /*
     * Two once-only scene entries.  Each tests the scene id, then queries and
     * sets a flag, then announces and runs its sequence.  The 0x8bc flag is
     * the same one tested above, so the earlier branch and this one are two
     * distinct sites of 0x080770c0 on one id and are kept distinct.
     */
    if (scene[0] == 19 && Func_080770c0(0x8bc) == 0) {
        Func_080770c8(0x8bc);
        Func_0808a360();
        Func_0200013c();
    }

    /* 192 << 2 = 768. */
    if (scene[0] == 16 && Func_080770c0(768) == 0) {
        Func_080770c8(768);
        Func_0808a360();
        Func_02000328();
    }

    if (Func_080770c0(0x8ab) != 0) {
        /* Two sites of one import on consecutive slots; not a loop. */
        Func_0808a0f0(35, 0, 0);
        Func_0808a0f0(36, 0, 0);
    }

    return 0;
}
