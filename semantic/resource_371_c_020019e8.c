#include "types.h"

/*
 * Resource 371 owner at 0x020019e8 (176 bytes, 17 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x020019e8 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02001a7c.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x02001a82-0x02001a93 are
 * alignment plus the literal pool (0x1f080000, 0x9999, 0x4ccc, 0x0200d158,
 * 0x927).
 *
 * One of three byte-identical bodies — this one and 0x02001888 and
 * 0x02001938 — that differ only in the single immediate at +0x8a
 * (105 here).  Their `bl`
 * halfwords are bit-identical, so their printed targets differ by exactly the
 * 0xb0 spacing of the owners themselves: the seventeen imports below are the
 * same seventeen callees in all three files under three different sets of
 * names.  See resource_371_c_0200008c.c for the same proof on a smaller pair.
 *
 * A blocking cutscene beat: acquire actor 8, set it up, clear the halfword
 * frame counter at +0x64, then spin on `Func_080000c0(1)` until that counter
 * becomes non-zero — a signed halfword read (`ldrsh`), so the wait ends on
 * any non-zero value.  The layout used on the actor (+0x18/+0x1c amplitude
 * pair, +0x64 frame counter) is the one proven by the byte-exact sibling
 * `assets/code/resource_371_c_020004a0.c` and shared with the owners at
 * 0x02003fb4 and 0x02004004.
 *
 * Argument recovery: `bl 0x02005cd0` sets no register but r0 still
 * holds the object returned by 0x02005cea, so that dataflow is kept.
 * 0x02005e78, 0x02005eaa, 0x02005eb6 and
 * 0x02005d5e follow another `bl` with nothing set, so no arguments
 * are asserted for them.
 *
 * 0x0200d158 is in-image data — the overlay is linked at 0x02008000, so it
 * lies at file offset 0x5158 — and is kept under its raw pool spelling, the
 * same convention the byte-exact `assets/code/resource_371_c_02000290.c`
 * uses for `Data_0200d27c`.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import.
 * Old-style declarations, because the interfaces are unknown.
 */

extern u8 Data_0200d158[];
















extern u8 * Func_02005cea();
extern void Func_02005cd0();
extern void Func_02005dfa();
extern void Func_02005b90();
extern void Func_02005d72();
extern void Func_02005d7e();
extern void Func_02005bb4();
extern void Func_02005e1c();
extern void Func_02005e78();
extern void Func_02005d52();
extern void Func_02005d68();
extern void Func_02005bde();
extern void Func_02005eaa();
extern void Func_02005eb6();
extern void Func_02005d04();
extern void Func_02005e82();
extern void Func_02005d5e();
void Func_020019e8(void)
{
    u8 *actor = Func_02005cea(8);
    s16 *frame;

    Func_02005cd0(actor);
    Func_02005dfa(-1, -1, -1, 0);
    Func_02005b90(1);
    Func_02005d72(0, 0, 0);
    Func_02005d7e(8, 0x1f080000, 0xc80000);

    *(s32 *)(actor + 0x18) = 0x14000;
    *(s32 *)(actor + 0x1c) = 0x14000;

    Func_02005bb4(1);
    Func_02005e1c(8, 1);
    Func_02005e78();
    Func_02005d52(8, 0x9999, 0x4ccc);

    frame = (s16 *)(actor + 0x64);
    Func_02005d68(8, Data_0200d158);
    *frame = 0;

    do {
        Func_02005bde(1);
    } while (*frame == 0);

    Func_02005eaa();
    Func_02005eb6();
    Func_02005d04(0x927);
    Func_02005e82(105);
    Func_02005d5e();
}
