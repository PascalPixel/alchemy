#include "types.h"

/*
 * Resource 3bc, owner at 0x02000c5c.  276 strict bytes: 264 bytes of code
 * (0x0c5c-0x0d63) and a three-word literal pool at 0x0d64-0x0d6f.
 *
 * 26 call sites resolved with tools/lib/overlay_call_targets.ts (23 veneers, two
 * internal prologues, one classified `call_via`).  The inventory row says
 * calls=25 — the documented lower bound.  Per-target multiset, which the C
 * below reproduces exactly:
 *   02003cc0 x1, 0808a018 x1, 02003ddc x1, 02003cd0 x1, 0808a0a0 x1,
 *   0808a090 x4, 0808a0c8 x2, 0808a0d0 x2, 0808a100 x4, 0808a150 x1,
 *   0808a010 x3, 0808a110 x1, 0808a250 x1, 0808a260 x1, 0808a268 x1,
 *   080770c8 x1  =  26.
 *
 * The `call_via` classification of 0x02003cc0 is the documented false
 * positive: this owner loads neither r3 nor r4 before the branch, and
 * 0x02003cc0 really is a standalone two-byte `bx lr` no-op leaf (its
 * neighbour at 0x02003cc4 is a separate two-instruction leaf).  It is spelled
 * as an ordinary call.
 *
 * The `Func_0808a0a0(8)` loop is `movs r5,#9` with the decrement *before* the
 * call and `cmp r5,#0 / bge` after it, so it runs ten times (r5 = 8..-1).
 *
 * Every large argument is built by a shift and is written here as the value:
 *   0x80 << 9 = 0x10000, 0x80 << 8 = 0x8000, 0x80 << 10 = 0x20000,
 *   191 << 3 = 1528, 187 << 3 = 1496, 188 << 3 = 1504, 190 << 3 = 1520,
 *   141 << 1 = 0x11A.
 * 0x91 and 72 are plain small constants (ids), not addresses — under the
 * proven 0x02008000 link base they are far below the image.
 *
 * `Data_02000240 + 0x22B` is a byte store into the cross-overlay table that
 * the byte-exact assets/code/resource_3bc_c_02000658.c declares as s16[]; the
 * offset is odd, so it is spelled through a u8 pointer here.
 *
 * Epilogue `pop {r5, r6} / pop {r0} / bx r0` — r0 is the popped return
 * address, so the owner is void.
 *
 * Uncertainty: Func_02003ddc(3, 17)'s result feeds only `5 - result` as the
 * second argument of Func_0808a250; its meaning is not established here.
 */

extern u8 Data_02000240[];

void Func_02003cc0();
void Func_0808a018();
s32 Func_02003ddc();
void Func_02003cd0();
void Func_0808a0a0();
void Func_0808a090();
void Func_0808a0c8();
void Func_0808a0d0();
void Func_0808a100();
void Func_0808a150();
void Func_0808a010();
void Func_0808a110();
void Func_0808a250();
void Func_0808a260();
void Func_0808a268();
void Func_080770c8();

void Func_02000c5c(void)
{
    s32 slot;
    s32 i;

    Func_02003cc0();
    Func_0808a018();
    slot = Func_02003ddc(3, 17);
    Func_02003cd0();

    i = 9;
    do {
        Func_0808a0a0(8);
        i--;
    } while (i >= 0);

    Func_0808a090(8, 0x10000, 0x8000);
    Func_0808a0c8(8, 1528, 192);
    Func_0808a090(0, 0x10000, 0x8000);
    Func_0808a0d0(0, 1496, 192);
    Func_0808a100(8, 1);
    Func_0808a150(0, 8, 0);
    Func_0808a010(10);
    Func_0808a100(8, 3);
    Func_0808a110(0, 3);
    Func_0808a010(20);

    Func_0808a090(0, 0x20000, 0x10000);
    Func_0808a090(8, 0x20000, 0x10000);
    Func_0808a0c8(0, 1504, 192);
    Func_0808a0d0(8, 1520, 192);
    Func_0808a100(0, 16);
    Func_0808a100(8, 9);
    Func_0808a010(10);

    Func_0808a250(72, 5 - slot);
    Func_0808a260(0x91, 4);
    Data_02000240[0x22B] = 3;
    Func_0808a268(0x91, 5);
    Func_080770c8(0x11A);
}
