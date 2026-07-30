typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

/*
 * Resource 371 owner at 0x02001938 (176 bytes, 17 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x02001938 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x020019cc.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x020019d2-0x020019e3 are
 * alignment plus the literal pool (0x1f080000, 0x9999, 0x4ccc, 0x0200d158,
 * 0x927).
 *
 * One of three byte-identical bodies — this one and 0x02001888 and 0x020019e8 — that
 * differ only in the single immediate at +0x8a (104 here).  Their `bl`
 * halfwords are bit-identical, so their printed targets differ by exactly the
 * 0xb0 spacing of the owners themselves: the seventeen imports below are the
 * same seventeen callees in all three files under three different sets of
 * names.  See resource_371_c_0200008c.c for the same proof on a smaller pair.
 *
 * A blocking cutscene beat: acquire actor 8, set it up, clear the halfword
 * frame counter at +0x64, then spin on `Func_02005b2e(1)` until that counter
 * becomes non-zero — a signed halfword read (`ldrsh`), so the wait ends on
 * any non-zero value.  The layout used on the actor (+0x18/+0x1c amplitude
 * pair, +0x64 frame counter) is the one proven by the byte-exact sibling
 * `assets/code/resource_371_c_020004a0.c` and shared with the owners at
 * 0x02003fb4 and 0x02004004.
 *
 * Argument recovery: `bl 0x02005c20` sets no register but r0 still
 * holds the object returned by 0x02005c3a, so that dataflow is kept.
 * 0x02005dc8, 0x02005dfa, 0x02005e06 and
 * 0x02005cae follow another `bl` with nothing set, so no arguments
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

u8 *Func_02005c3a();
void Func_02005c20();
void Func_02005d4a();
void Func_02005ae0();
void Func_02005cc2();
void Func_02005cce();
void Func_02005b04();
void Func_02005d6c();
void Func_02005dc8();
void Func_02005ca2();
void Func_02005cb8();
void Func_02005b2e();
void Func_02005dfa();
void Func_02005e06();
void Func_02005c54();
void Func_02005dd2();
void Func_02005cae();

void Func_02001938(void)
{
    u8 *actor = Func_02005c3a(8);
    s16 *frame;

    Func_02005c20(actor);
    Func_02005d4a(-1, -1, -1, 0);
    Func_02005ae0(1);
    Func_02005cc2(0, 0, 0);
    Func_02005cce(8, 0x1f080000, 0xc80000);

    *(s32 *)(actor + 0x18) = 0x14000;
    *(s32 *)(actor + 0x1c) = 0x14000;

    Func_02005b04(1);
    Func_02005d6c(8, 1);
    Func_02005dc8();
    Func_02005ca2(8, 0x9999, 0x4ccc);

    frame = (s16 *)(actor + 0x64);
    *frame = 0;
    Func_02005cb8(8, Data_0200d158);

    do {
        Func_02005b2e(1);
    } while (*frame == 0);

    Func_02005dfa();
    Func_02005e06();
    Func_02005c54(0x927);
    Func_02005dd2(104);
    Func_02005cae();
}
