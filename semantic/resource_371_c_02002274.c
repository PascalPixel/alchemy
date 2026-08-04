#include "types.h"

/*
 * Resource 371 owner at 0x02002274 (564 bytes, 49 distinct call targets).
 *
 * Complete owner: `push {r5, lr}` at 0x02002274 and the interworking return
 * `pop {r5} ; pop {r0} ; bx r0` at 0x02002434.  r0 holds the popped return
 * address, so the owner returns nothing.  Bytes 0x0200243a-0x020024a7 are
 * alignment plus the literal pool — 27 words, all reached only by
 * `ldr rN, [pc, #imm]`, and 0x02002274 + 564 = 0x020024a8 lands exactly one
 * past the last of them.
 *
 * A long cutscene carpet: acquire actor 10, then issue fifty commands.  Fifty
 * `bl`s reach forty-nine distinct targets — 0x0200662a is called at both
 * 0x020022cc and 0x020022f4 — which matches the inventory's distinct-target
 * count of 49 exactly and accounts for every call.
 *
 * Argument recovery: `bl 0x0200655c` at 0x0200227e sets no register, but r0
 * still holds the object returned by 0x02006576, so the dataflow is kept.
 * 0x020066f6, 0x0200670a, 0x02006868, 0x02006874 and 0x02006716 follow
 * another `bl` (or, at 0x020022b8, a block that writes only r3 and memory)
 * with nothing else set, so no arguments are asserted for them.
 *
 * Constants: 192 << 9 = 0x18000 is the amplitude pair written to +0x18/+0x1c
 * and 128 << 7 = 0x4000 the facing halfword at +0x06 — the same fields the
 * owners at 0x02003fb4/0x02004004 and 0x020001c4 use, whose offsets come
 * from the byte-exact `assets/code/resource_371_c_020004a0.c`.  The pooled
 * 0x15xx0000/0x08xx0000 words are 16.16 coordinates; the pooled 0x15xx/0x08xx
 * words without a fractional part are the same values as plain integers.
 *
 * Call-target convention: each `bl` is named by the address its call site
 * computes, which is a per-call-site label for a load-time-relocated import
 * (see resource_371_c_0200008c.c).  Old-style declarations, because the
 * interfaces are unknown and one import can be reached with different
 * argument counts.
 */

u8 *Func_0808a080();
void Func_0808a018();
void Func_0808a210();
void Func_080000c0();
void Func_0808a0f0();
void Func_0808a360();
void Func_0808a370();
void Func_0808a010();
void Func_0808a0c0();
void Func_080f9010();
void Func_0808a090();
void Func_0808a100();
void Func_0808a208();
void Func_0808a128();
void Func_0808a0d0();
void Func_0808a1b8();
void Func_0808a368();
void Func_0808a248();
void Func_0808a020();

void Func_02002274(void)
{
    u8 *actor = Func_0808a080(10);

    Func_0808a018(actor);
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    Func_0808a0f0(0, 0, 0);
    Func_080000c0(1);

    *(s32 *)(actor + 0x18) = 0x18000;
    *(s32 *)(actor + 0x1c) = 0x18000;
    *(u16 *)(actor + 0x06) = 0x4000;

    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a0f0(10, 0x15680000, 0x08380000);
    Func_080000c0(1);
    Func_080f9010(141);
    Func_0808a090(10, 0x19999, 0x6666);
    Func_0808a100(10, 2);
    Func_0808a0c0(10, 0x156d, 0x858);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(0x15b80000, -1, 0x08580000, 1);
    Func_0808a0c0(10, 0x159e, 0x858);
    Func_0808a0c0(10, 0x15a8, 0x86e);
    Func_0808a0c0(10, 0x15e8, 0x878);
    Func_0808a100(10, 1);
    Func_080f9010(0x121);
    Func_0808a010(20);
    Func_0808a0f0(0, 0x15d80000, 0x08780000);
    Func_080000c0(1);
    Func_0808a128(0, 6, 0);
    Func_0808a090(0, 0x20000, 0x10000);
    Func_0808a0d0(0, 0x15c8, 0x878);
    Func_0808a1b8(0, 0, 40);
    Func_080f9010(141);
    Func_0808a100(10, 2);
    Func_0808a0c0(10, 0x15f8, 0x878);
    Func_0808a1b8(0, 0xe000, 0);
    Func_0808a0c0(10, 0x15f8, 0x838);
    Func_0808a1b8(0, 0xc000, 0);
    Func_0808a0c0(10, 0x15bd, 0x838);
    Func_0808a0c0(10, 0x15b8, 0x853);
    Func_0808a1b8(0, 0xa000, 0);
    Func_0808a0c0(10, 0x1572, 0x858);
    Func_0808a0c0(10, 0x1568, 0x838);
    Func_0808a0f0(10, 0, 0);
    Func_080f9010(0x121);
    Func_0808a010(40);
    Func_0808a210(0x15d80000, -1, 0x08580000, 1);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a0d0(0, 0x15d8, 0x858);
    Func_0808a368();
    Func_0808a370();
    Func_0808a248(20);
    Func_0808a020();
}
