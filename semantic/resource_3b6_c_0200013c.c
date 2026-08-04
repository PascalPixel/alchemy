#include "types.h"

/*
 * Resource 3b6 scripted cutscene sequence at 0x0200013c.
 *
 * Complete owner: `push {r5, lr}` at 0x0200013c and the matching
 * `pop {r5} / pop {r0} / bx r0` interworking return at 0x0200030a.  The return
 * address is popped into r0, so the owner is `void`, and it takes no argument:
 * r0 is written before every call and never read on entry.  r5 holds the
 * handle returned by 0x080090c8 across the whole body.  A six-word literal
 * pool follows the return at 0x02000310-0x02000327 and is data.
 *
 * Straight-line: not one branch instruction between the prologue and the
 * epilogue, so there is nothing to skip and no arm to fold.
 *
 * CALL COUNT.  The row carries fifty-four `bl` instructions, transcribed below
 * one for one in source order.  The inventory advertises `calls=49`, so the
 * manifest undercounts by five; since the body has no branches and no interior
 * pool, every one of the fifty-four is on the single reachable path.  All
 * fifty-four resolve with tools/overlay_call_targets.ts — an overlay `bl`
 * stores `target_offset - 2`, so the disassembler's labels are artefacts — to
 * seventeen distinct import veneers, each named for the main-image address in
 * its trailing word.
 *
 * Reached as an ordinary call from the overlay's dispatcher at 0x020003dc
 * (site 0x02000496, resolving to this prologue), on the once-only path for
 * scene id 19.
 *
 * The begin/message/act/end quartet is the one tracked byte-exact source
 * assets/code/resource_3b6_c_0200073c.c uses under its printed names, which
 * also fixes the void return.
 *
 * Fixed point.  The pool words passed to 0x0808a090 are 0x16666, 0xb333,
 * 0x1cccc and 0xe666, and the immediate forms passed to the same import are
 * 0x10000/0x8000.  Read against 0x10000 as one, those are 1.4/0.7, 1.8/0.9 and
 * 1.0/0.5 — consistent x/y scale pairs, which is the evidence for reading that
 * import as a scale setter.  The reading is inferred from the constants, not
 * from the callee, and is recorded as an uncertainty.
 *
 * Repeated blocks — the `delay / act on 25` pairs and the three 0x0808a1e8
 * calls that differ only in one argument — are transcribed as the separate
 * call sites they are.  None is a loop and none is folded.
 */

/* Old-style declarations: interfaces vary by call site across this overlay. */
void Func_0808a018();  /* begin step */
void Func_0808a170();  /* present message by id */
void Func_0808a010();  /* wait / delay by count */
s32 Func_080090c8();   /* creates the object this sequence drives */
void Func_080091e0();
void Func_08009080();
void Func_080090d0();
void Func_0808a1e8();
void Func_0808a090();  /* x/y scale, 0x10000 = one */
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a180();  /* act on the subject */
void Func_0808a138();
void Func_0808a580();
void Func_0808a110();
void Func_0808a0f0();
void Func_0808a020();  /* end step */

void Func_0200013c(void)
{
    s32 handle;

    Func_0808a018();
    Func_0808a170(0x2394);
    Func_0808a010(40);

    /* r0 = 142 << 1, r1 = 150 << 18, r3 = 206 << 18; r2 is set to 0. */
    handle = Func_080090c8(284, 0x2580000, 0, 0x3380000);

    /* r0 is not reloaded: the handle just returned is this call's argument. */
    Func_080091e0(handle, 0);

    Func_08009080(handle, 6);
    Func_0808a010(10);
    Func_08009080(handle, 1);
    Func_0808a010(40);
    Func_080090d0(handle);
    Func_0808a010(2);

    Func_0808a1e8(25, 256, 50);
    Func_0808a090(25, 0x10000, 0x8000);
    Func_0808a0d0(25, 600, 848);
    Func_0808a1b8(25, 0xc000, 0);
    Func_0808a010(40);
    Func_0808a180(25, 0);
    Func_0808a138(25, 2);
    Func_0808a010(30);

    Func_0808a0d0(25, 568, 848);
    Func_0808a1b8(25, 0xc000, 0);
    Func_0808a010(30);
    Func_0808a1e8(25, 264, 50);
    Func_0808a010(20);

    /* r2 is built as `movs #16 / negs`, so the third argument is -16. */
    Func_0808a580(0, 0, -16);

    Func_0808a010(20);
    Func_0808a1b8(25, 0x3000, 0);
    Func_0808a010(30);
    Func_0808a138(25, 2);
    Func_0808a010(20);
    Func_0808a180(25, 0);
    Func_0808a010(20);

    Func_0808a1e8(0, 0x101, 50);
    Func_0808a010(20);
    Func_0808a110(25, 4);
    Func_0808a010(20);
    Func_0808a180(25, 0);
    Func_0808a010(30);

    Func_0808a1e8(25, 258, 50);
    Func_0808a180(25, 0);

    Func_0808a090(25, 0x16666, 0xb333);
    Func_0808a580(25, 16, 0);
    Func_0808a580(25, 0, 32);
    Func_0808a010(20);
    Func_0808a110(25, 3);
    Func_0808a010(20);
    Func_0808a180(25, 0);

    Func_0808a580(0, 16, 0);
    Func_0808a1b8(0, 0x8000, 0);
    Func_0808a010(20);

    Func_0808a090(25, 0x1cccc, 0xe666);
    Func_0808a580(25, 0, 48);
    Func_0808a0f0(25, 0, 0);

    Func_0808a020();
}
