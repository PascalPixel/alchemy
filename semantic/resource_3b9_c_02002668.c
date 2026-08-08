#include "types.h"

/*
 * resource_3b9 owner at 0x02002668, 440 bytes: one long, fully linear
 * setup sequence (about 40 calls, no branches) -- three near-identical
 * calls with shifted-constant position/size pairs, several id-tagged
 * calls (0-11) with a shared shifted-constant pair, three calls sharing
 * a pool address, and a closing run touching ids 0/11/20/60/67.
 *
 * Complete owner: `push {r5, lr}` at 0x02002668 through `pop {r5} / pop
 * {r0} / bx r0` at 0x02002804-0x02002808, followed by the five-word
 * literal pool 0x0200280c-0x0200281f; the next owner's prologue is at
 * 0x02002820 (this overlay's own resource_3b9_c_02002820.c candidate,
 * not yet drafted). No incoming arguments are read before being
 * overwritten, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl`, resolved with `cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --`'s `+2`
 * rule.
 *
 * The two formerly conflated sites at 0x20026ea and 0x2002732 resolve to
 * different imports, Func_0808a100 and Func_0808a090 respectively; their
 * differing argument counts are therefore not an arity conflict.
 *
 * Uncertainty: none of the twenty-three resolved callees are identified beyond
 * call shape; every id/constant is recorded as a passed value, not
 * decoded further.
 */

extern void Func_0808a018(void);
extern void Func_0808a0f0(s32, s32, s32);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_0808a010();
extern void Func_0808a138(s32, s32);
extern void Func_0808a100(s32, s32);
extern void Func_0808a170(s32);
extern void Func_02001c48(s32);
extern void Func_0808a1b8(s32, s32, s32);
extern void Func_0808a090(s32, s32, s32);
extern void Func_0808a098(s32, s32);
extern void Func_0808a0b0(s32, s32);
extern void Func_02001c5c(s32, s32);
extern void Func_0808a110(s32, s32);
extern void Func_0808a0c0(s32, s32, s32);
extern void Func_0808a0b8(s32, s32, s32);
extern void Func_0808a208(s32, s32);
extern void Func_0808a210(s32, s32, s32, s32);
extern void Func_0808a0d0(s32, s32, s32);
extern void Func_0808a0c8(s32, s32, s32);
extern void Func_0808a368(void);
extern void Func_0808a248(s32);

void Func_02002668(void)
{
    s32 shared = 0x0200adac;

    Func_0808a018();
    Func_0808a0f0(1, 0x3180000, 0x880000);
    Func_0808a0f0(2, 0x3380000, 0x880000);
    Func_0808a0f0(3, 0x3280000, 0x980000);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_0808a138(8, 1);
    Func_0808a100(8, 3);
    Func_0808a170(0x2134);
    Func_02001c48(8);
    Func_0808a138(9, 1);
    Func_02001c48(9);
    Func_0808a138(10, 1);
    Func_02001c48(10);
    Func_0808a138(11, 1);
    Func_0808a100(11, 3);
    Func_02001c48(11);
    Func_0808a1b8(1, 0xe000, 0);
    Func_0808a1b8(2, 0xa000, 20);
    Func_0808a090(1, 0x10000, 0x8000);
    Func_0808a090(2, 0x10000, 0x8000);
    Func_0808a090(3, 0x10000, 0x8000);
    Func_0808a098(1, shared);
    Func_0808a098(2, shared);
    Func_0808a0b0(3, shared);
    Func_0808a010(20);
    Func_02001c5c(0, 0);
    Func_0808a110(0, 3);
    Func_0808a110(11, 3);
    Func_0808a090(11, 0x10000, 0x8000);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a100(11, 2);
    Func_0808a0c0(11, 0x33e, 152);
    Func_0808a0c0(11, 808, 164);
    Func_0808a0b8(11, 808, 312);
    Func_0808a010(20);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(0x3280000, -1, 0x1380000, 1);
    Func_0808a0d0(0, 808, 164);
    Func_0808a0c8(0, 808, 312);
    Func_0808a010(60);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(67);
}
