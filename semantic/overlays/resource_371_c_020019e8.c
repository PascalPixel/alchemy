typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

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

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a210();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a200();
void Func_0808a360();
void Func_0808a090();
void Func_0808a098();
void Func_0808a368();
void Func_0808a370();
void Func_080770c8();
void Func_0808a248();
void Func_0808a020();

void Func_020019e8(void)
{
    u8 *actor = Func_0808a080(8);
    s16 *frame;

    Func_0808a018(actor);
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_0808a0f0(8, 0x1f080000, 0xc80000);

    *(s32 *)(actor + 0x18) = 0x14000;
    *(s32 *)(actor + 0x1c) = 0x14000;

    Func_080000c0(1);
    Func_0808a200(8, 1);
    Func_0808a360();
    Func_0808a090(8, 0x9999, 0x4ccc);

    frame = (s16 *)(actor + 0x64);
    *frame = 0;
    Func_0808a098(8, Data_0200d158);

    do {
        Func_080000c0(1);
    } while (*frame == 0);

    Func_0808a368();
    Func_0808a370();
    Func_080770c8(0x927);
    Func_0808a248(105);
    Func_0808a020();
}
