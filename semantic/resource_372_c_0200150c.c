#include "types.h"

/*
 * Resource 372 cutscene script step at 0x0200150c (244 bytes, 23 distinct
 * call targets across 24 call sites).
 *
 * Complete owner: `push {r5, lr}` at 0x0200150c, matching
 * `pop {r5} / pop {r0} / bx r0` at 0x020015de.  `pop {r0} ; bx r0` — r0 is the
 * popped return address, so the owner returns nothing.  0x020015e4-0x020015ff
 * is the literal pool (0x32a, 0x03250000, 0x339, 0x357, 0xe67, 0x0200c8c0,
 * 0x835), reached only by `ldr rN, [pc, #imm]`.
 *
 * `out/decomp/overlays.json` also seeds a row at 0x0200153e "contained_by"
 * this owner, and the byte-exact `assets/code/resource_372_c_02000434.c`
 * contains `bl sub_0200153e`.  0x0200153e is *not* a function: it is the
 * `lsls r2, r2, #9` in the middle of this owner's fourth argument setup, with
 * this owner's `push {r5, lr}` already executed.  Under this overlay's
 * established reading an encoded `bl` target is an import identity rather than
 * a location, so that call site is not evidence of a second entry point here;
 * the owner is reconstructed whole from its own prologue to its own epilogue.
 *
 * Note `Func_0808a170` is reached twice with different argument counts (one
 * argument at 0x02001598, two at 0x020015b8).  Old-style declarations are
 * therefore mandatory.
 *
 * Uncertainties: the first call, 0x02005c0c, and 0x02005e24 / 0x02005ce0 are
 * reached with no argument register written, so they are asserted as taking
 * none.  r5 carries a message identifier 0xe67 that is bumped to 0xe6b before
 * its second use.
 */

/* Imports, old-style: interfaces unknown, and 0x02005d6e takes different
 * argument counts at its two sites. */




















/* In-image data block referenced by the pool word 0x0200c8c0. */
extern u8 Data_0200c8c0[];

extern void Func_02005c0c();
extern void Func_02005c98();
extern void Func_02005cb4();
extern void Func_02005ca8();
extern void Func_02005c7e();
extern void Func_02005cf0();
extern void Func_02005cd4();
extern void Func_02005cf4();
extern void Func_02005d0e();
extern void Func_02005d38();
extern void Func_02005e24();
extern void Func_02005c72();
extern void Func_02005d3a();
extern void Func_02005d9e();
extern void Func_02005d6e();
extern void Func_02005d86();
extern void Func_02005c9c();
extern void Func_02005da4();
extern void Func_02005d94();
extern void Func_02005db6();
extern void Func_02005d2e();
extern void Func_02005cbc();
extern void Func_02005ce0();
void Func_0200150c(void)
{
    s32 msg = 0xe67;   /* r5 */

    Func_02005c0c();
    Func_02005c98(0, 0x106, 0x32a);          /* 131 << 1 */
    Func_02005cb4(20, 0x1060000, 0x3250000); /* 131 << 17 */
    Func_02005ca8(20, 0x106, 0x339);         /* 131 << 1 */
    Func_02005c7e(0, 0x20000, 0x10000);      /* 128 << 10, 128 << 9 */
    Func_02005cf0(0, 2, 0);
    Func_02005cd4(0, 0x11a, 0x357);          /* 141 << 1 */
    Func_02005cf4(20, 1);
    Func_02005d0e(0, 4, 0);
    Func_02005d38(0, 20, 0);
    Func_02005e24();
    Func_02005c72(30);
    Func_02005d3a(0, 2);
    Func_02005d9e(20, 0x100, 20);            /* 128 << 1 */
    Func_02005d6e(msg);                      /* one argument here */
    Func_02005d86(20, 0);
    Func_02005c9c(20);
    Func_02005da4(20, 0);
    msg += 4;
    Func_02005d6e(20, 2);                    /* two arguments here */
    Func_02005d94(msg);
    Func_02005db6(20, 0, 20);
    Func_02005d2e(20, Data_0200c8c0);
    Func_02005cbc(0x835);
    Func_02005ce0();
}
