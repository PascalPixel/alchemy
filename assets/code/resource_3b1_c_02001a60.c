#include "types.h"

/*
 * resource_3b1 owner at 0x02001a60, 212 bytes.  It is gated by flag 0x922,
 * returns through `pop {r0} / bx r0` at 0x02001b16-0x02001b18, includes the
 * alignment halfword at 0x02001b1a and six pool words at
 * 0x02001b1c-0x02001b33, and ends immediately before the next owner's
 * saved-register prologue at 0x02001b34.
 *
 * Per-site call veneers (raw asm confirms each callee slot uses its own
 * local stub, distinct from the generic main-image symbol name). One local
 * stub (`sub_02006336`) is reused verbatim across two call sites with
 * different argument counts (a 4-argument setter, then a later 2-argument
 * setter) -- both compile fine as old-style declarations since neither
 * call site is prototype-checked.
 */

extern s32 Func_02007f06(s32 flag);
extern void Func_02007f36(void);
extern void Func_020080ba(void);
extern void Func_02008072(s32 arg0, s32 arg1);
extern void Func_02006336();
extern void Func_02008038(s32 message);
extern void Func_02006302(s32 id);
extern void Func_02006308(s32 id);
extern void Func_02006326(s32 id, s32 value);
extern void Func_02006318(s32 id);
extern void Func_02006328(s32 id);
extern void Func_02006346(s32 id, s32 value);
extern void Func_02006338(s32 id);
extern void Func_020080a6(s32 id, s32 arg1, s32 arg2);
extern void Func_02006348(s32 id);
extern void Func_02006366(s32 id, s32 value);
extern void Func_02006358(s32 id);
extern void Func_0200635e(s32 id);
extern void Func_02006364(s32 id);
extern void Func_02006382(s32 id, s32 value);
extern void Func_02006374(s32 id);
extern void Func_02007fb8(s32 flag);
extern void Func_02007fe4(void);

void Func_02001a60(void)
{
    if (Func_02007f06(0x922) == 0)
        return;

    Func_02007f36();
    Func_020080ba();
    Func_02008072(0x19999, 0x3333);
    Func_02006336(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
    Func_02008038(0x1d26);

    Func_02006302(8);
    Func_02006308(10);
    Func_02006326(8, 0x3000);
    Func_02006318(8);
    Func_02006336(10, 0xd000);
    Func_02006328(10);
    Func_02006346(9, 0x5000);
    Func_02006338(9);

    Func_020080a6(8, 0, 20);
    Func_02006348(8);
    Func_02006366(9, 0x8000);
    Func_02006358(9);
    Func_0200635e(10);
    Func_02006364(8);
    Func_02006382(10, 0xb000);
    Func_02006374(8);

    Func_02007fb8(0x920);
    Func_02007fe4();
}
