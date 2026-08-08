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
 * fifty-four resolve with cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- — an overlay `bl`
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
void Func_02000b20();
void Func_02000b7e();
void Func_02000b24();
s32 Func_02000afe();
void Func_02000b26();
void Func_02000b06();
void Func_02000b4c();
void Func_02000b14();
void Func_02000b5a();
void Func_02000b30();
void Func_02000b66();
void Func_02000bf2();
void Func_02000ba8();
void Func_02000bbe();
void Func_02000c12();
void Func_02000ba0();
void Func_02000c18();
void Func_02000c08();
void Func_02000bb6();
void Func_02000bf4();
void Func_02000c48();
void Func_02000bd6();
void Func_02000c62();
void Func_02000be8();
void Func_02000c8c();
void Func_02000bfa();
void Func_02000c7e();
void Func_02000c0c();
void Func_02000c6c();
void Func_02000c1a();
void Func_02000c92();
void Func_02000c28();
void Func_02000cb2();
void Func_02000c38();
void Func_02000c88();
void Func_02000c46();
void Func_02000cbe();
void Func_02000c54();
void Func_02000ce0();
void Func_02000cd8();
void Func_02000c9a();
void Func_02000d14();
void Func_02000d1e();
void Func_02000cdc();
void Func_02000d12();
void Func_02000d44();
void Func_02000d30();
void Func_02000cf0();
void Func_02000d6a();
                       /* begin step */
                       /* present message by id */
                       /* wait / delay by count */
                       /* creates the object this sequence drives */

                     
                     
                       /* x/y scale, 0x10000 = one */

                       /* act on the subject */

                     
                     
                       /* end step */

void Func_0200013c(void)
{
    s32 handle;

    Func_02000b20();
    Func_02000b7e(0x2394);
    Func_02000b24(40);

    /* r0 = 142 << 1, r1 = 150 << 18, r3 = 206 << 18; r2 is set to 0. */
    handle = Func_02000afe(284, 0x2580000, 0, 0x3380000);

    /* r0 is not reloaded: the handle just returned is this call's argument. */
    Func_02000b26(handle, 0);

    Func_02000b06(handle, 6);
    Func_02000b4c(10);
    Func_02000b14(handle, 1);
    Func_02000b5a(40);
    Func_02000b30(handle);
    Func_02000b66(2);

    Func_02000bf2(25, 256, 50);
    Func_02000ba8(25, 0x10000, 0x8000);
    Func_02000bbe(25, 600, 848);
    Func_02000c12(25, 0xc000, 0);
    Func_02000ba0(40);
    Func_02000c18(25, 0);
    Func_02000c08(25, 2);
    Func_02000bb6(30);

    Func_02000bf4(25, 568, 848);
    Func_02000c48(25, 0xc000, 0);
    Func_02000bd6(30);
    Func_02000c62(25, 264, 50);
    Func_02000be8(20);

    /* r2 is built as `movs #16 / negs`, so the third argument is -16. */
    Func_02000c8c(0, 0, -16);

    Func_02000bfa(20);
    Func_02000c7e(25, 0x3000, 0);
    Func_02000c0c(30);
    Func_02000c6c(25, 2);
    Func_02000c1a(20);
    Func_02000c92(25, 0);
    Func_02000c28(20);

    Func_02000cb2(0, 0x101, 50);
    Func_02000c38(20);
    Func_02000c88(25, 4);
    Func_02000c46(20);
    Func_02000cbe(25, 0);
    Func_02000c54(30);

    Func_02000ce0(25, 258, 50);
    Func_02000cd8(25, 0);

    Func_02000c9a(25, 0x16666, 0xb333);
    Func_02000d14(25, 16, 0);
    Func_02000d1e(25, 0, 32);
    Func_02000c8c(20);
    Func_02000cdc(25, 3);
    Func_02000c9a(20);
    Func_02000d12(25, 0);

    Func_02000d44(0, 16, 0);
    Func_02000d30(0, 0x8000, 0);
    Func_02000cbe(20);

    Func_02000cf0(25, 0x1cccc, 0xe666);
    Func_02000d6a(25, 0, 48);
    Func_02000d14(25, 0, 0);

    Func_02000cf0();
}
