#include "types.h"

/*
 * Resource 371 owner at 0x02001888 (176 bytes, 17 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x02001888 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x0200191c.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x02001922-0x02001933 are
 * alignment plus the literal pool (0x1f080000, 0x9999, 0x4ccc, 0x0200d158,
 * 0x927).
 *
 * One of three byte-identical bodies — this one and 0x02001938 and
 * 0x020019e8 — that differ only in the single immediate at +0x8a
 * (103 here).  Their `bl`
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
 * Argument recovery: `bl 0x02005b70` sets no register but r0 still
 * holds the object returned by 0x02005b8a, so that dataflow is kept.
 * 0x02005d18, 0x02005d4a, 0x02005d56 and
 * 0x02005bfe follow another `bl` with nothing set, so no arguments
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
















extern u8 * Func_02005b8a();
extern void Func_02005b70();
extern void Func_02005c9a();
extern void Func_02005a30();
extern void Func_02005c12();
extern void Func_02005c1e();
extern void Func_02005a54();
extern void Func_02005cbc();
extern void Func_02005d18();
extern void Func_02005bf2();
extern void Func_02005c08();
extern void Func_02005a7e();
extern void Func_02005d4a();
extern void Func_02005d56();
extern void Func_02005ba4();
extern void Func_02005d22();
extern void Func_02005bfe();
void Func_02001888(void)
{
    u8 *actor = Func_02005b8a(8);
    s16 *frame;

    Func_02005b70(actor);
    Func_02005c9a(-1, -1, -1, 0);
    Func_02005a30(1);
    Func_02005c12(0, 0, 0);
    Func_02005c1e(8, 0x1f080000, 0xc80000);

    *(s32 *)(actor + 0x18) = 0x14000;
    *(s32 *)(actor + 0x1c) = 0x14000;

    Func_02005a54(1);
    Func_02005cbc(8, 1);
    Func_02005d18();
    Func_02005bf2(8, 0x9999, 0x4ccc);

    frame = (s16 *)(actor + 0x64);
    *frame = 0;
    Func_02005c08(8, Data_0200d158);

    do {
        Func_02005a7e(1);
    } while (*frame == 0);

    Func_02005d4a();
    Func_02005d56();
    Func_02005ba4(0x927);
    Func_02005d22(103);
    Func_02005bfe();
}
