#include "types.h"

/*
 * resource_380 owner at 0x02000f8c, 1220 bytes: a scripted scene beat
 * over actors 5/9/10/11/12 and the id-8/id-12 record pair, with two
 * counted animation loops in the middle.  The head choreographs
 * placements and holds, seeds both records' +24/+28 fields with
 * 0x1999 and record 12's +12 field with 0x280000, then a dialogue
 * burst against id 0x01d70000; the first loop runs 90 one-frame ticks
 * sliding +12 down by 0x1999 and all four +24/+28 fields up by 0x28f
 * per tick, the second runs 60 ticks stepping +12 down by 0x8000.
 * Between them a byte at record12->field80_ptr+38 and the byte at
 * record12+0x55 sequence 0 -> 5 -> 3 -> (flag 1), driving whatever
 * animation channel those bytes select.  The close is another
 * dialogue pair (ids 0x1080000/0x01dd0000) around the skip-beat
 * counter bump at Data_03001ebc+472, gated on Func_0808a070(0, 0).
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8/r9/sl spill at
 * 0x02000f8c-0x02000f94 through the matching unspill and `pop {r0} /
 * bx r0` at 0x02001438-0x02001444, alignment halfword, tail pool
 * 0x02001448-0x0200144f; next owner (resource_380_c_02001450.c,
 * already drafted) exactly at 0x02001450.  The queue's ~870-byte
 * estimate was short by 350 bytes.  The apparent `b.n 0x200107a` at
 * 0x020013a8 is the mid-function pool word 0xffffe667 (-0x1999, the
 * loop-1 decrement) decoded as an instruction -- a literal-pool
 * masquerade, not a branch; the only real control flow is the two
 * counted loops, the skip-counter beq, and the b.n over that pool.
 *
 * Callee names from bun tools/lib/overlay_call_targets.ts resource_380
 * f8c 1450 (the +2 rule): 118 sites, 30 distinct targets, 102 through
 * the import-veneer table, 16 to the local wrapper Func_02004248.
 *
 * Uncertainty: the record structure behind Func_0808a080 (fields +12,
 * +24, +28 slid by the loops; the pointer at +0x50 whose target's +38
 * byte and the record's own +0x55 byte take values 0/5/3/1; the +0x23
 * flag byte) is transcribed by offset with roles open; the two
 * Func_02004248 sites whose first argument is a packed constant
 * (0x400c, 0x5009) rather than an actor id are kept as compiled, same
 * treatment as Func_0808a188's mixed ids in the 3c9 drafts.
 */

extern u8 *Data_03001ebc;

extern void Func_02004248(s32 arg0, s32 arg1);

extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_0808a010(s32 frames);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a150(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080f9010(s32 arg0);

void Func_02000f8c(void)
{
    u8 *record12;
    u8 *record8;
    u8 *channel;
    u8 *phase;
    s32 tick;

    Func_080f9010(0x3d);
    Func_0808a100(10, 4);
    Func_0808a170(0x107d);
    Func_02004248(10, 10);
    Func_0808a100(11, 4);
    Func_02004248(11, 30);
    Func_0808a1e8(9, 258, 60);
    Func_0808a128(9, 4, 10);
    Func_0808a128(9, 6, 30);
    Func_02004248(9, 10);
    Func_0808a138(10, 1);
    Func_0808a1b8(10, 0xb000, 10);
    Func_02004248(10, 20);
    Func_0808a138(11, 1);
    Func_0808a1b8(11, 0xd000, 20);
    Func_02004248(11, 30);
    Func_0808a1e8(9, 258, 60);
    Func_0808a1b8(5, 0, 0);
    Func_0808a1b8(9, 0x7000, 80);
    Func_0808a1e8(5, 258, 40);
    Func_02004248(5, 20);
    Func_0808a138(9, 2);
    Func_0808a100(9, 4);
    Func_02004248(9, 10);

    record12 = Func_0808a080(12);
    record8 = Func_0808a080(8);
    channel = *(u8 **)(record12 + 0x50) + 38;
    *(s32 *)(record12 + 24) = 0x1999;
    *(s32 *)(record8 + 28) = 0x1999;
    *(s32 *)(record12 + 28) = 0x1999;
    *(s32 *)(record8 + 24) = 0x1999;
    *channel = 0;
    Func_0808a158(12, 256);
    *phase = 0;
    *(s32 *)(record12 + 12) = 0x280000;
    Func_0808a0f0(12, 0x01d70000, 0x1220000);
    Func_0808a010(1);
    phase = record12 + 0x55;
    Func_02004248(12, 10);
    Func_0808a1e8(5, 256, 0);
    Func_0808a1e8(9, 256, 30);
    Func_0808a1b8(5, 0xc000, 0);
    Func_0808a1b8(9, 0xb000, 10);
    Func_0808a1b8(11, 0xd000, 0);
    Func_0808a1b8(10, 0xb000, 0);
    Func_0808a208(0x20000, 0x4000);
    Func_0808a210(0x01d70000, -1, 0x01350000, 1);
    Func_0808a218();
    Func_0808a0f0(8, 0x01d70000, 0x1220000);
    Func_080f9010(0xbe);
    Func_0808a1e0(12, 2);

    for (tick = 0; tick != 90; tick++) {
        *(s32 *)(record12 + 12) += -0x1999;
        *(s32 *)(record12 + 24) += 0x28f;
        *(s32 *)(record12 + 28) += 0x28f;
        *(s32 *)(record8 + 24) += 0x28f;
        *(s32 *)(record8 + 28) += 0x28f;
        Func_0808a010(1);
    }
    *phase = 5;
    Func_0808a010(80);
    for (tick = 0; tick != 60; tick++) {
        *(s32 *)(record12 + 12) += -0x8000;
        Func_0808a010(1);
    }
    *phase = 3;
    Func_0808a010(30);
    *channel = 1;

    Func_0808a0f0(8, 0, 0);
    Func_0808a1e0(12, 1);
    Func_0808a080(12)[0x23] |= 1;
    Func_0808a158(12, 0);
    Func_0808a090(12, 0x8000, 0x4000);
    Func_0808a0d0(12, 471, 306);
    Func_0808a010(40);
    Func_0808a138(12, 2);
    Func_02004248(0x400c, 20);
    Func_0808a150(5, 9, 0);
    Func_0808a010(20);
    Func_0808a130(5, 2);
    Func_0808a138(9, 2);
    Func_0808a010(40);
    Func_0808a130(10, 1);
    Func_0808a138(11, 1);
    Func_0808a010(20);
    Func_0808a110(10, 4);
    Func_0808a1b8(10, 0x5000, 0);
    Func_0808a1b8(11, 0x5000, 10);
    Func_02004248(10, 30);
    Func_0808a1f0(12, 258);
    Func_0808a010(60);
    Func_0808a138(11, 1);
    Func_0808a010(10);
    Func_02004248(11, 30);
    Func_0808a1b8(11, 0xd000, 30);
    Func_0808a110(11, 3);
    Func_0808a010(20);
    Func_02004248(11, 30);
    Func_0808a110(12, 3);
    Func_0808a010(20);
    Func_0808a1b8(11, 0x5000, 40);
    Func_0808a1b8(9, 0x5000, 0);
    Func_0808a1b8(5, 0x6000, 20);
    Func_0808a110(10, 3);
    Func_0808a010(20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x1080000, -1, 0x1cc0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(40);
    Func_02004248(10, 40);
    Func_0808a130(0, 3);
    Func_0808a138(1, 3);
    Func_0808a010(80);
    Func_0808a178(11, 0);
    if (Func_0808a070(0, 0) != 0) {
        *(u16 *)(Data_03001ebc + 472) += 1; /* skip-beat counter */
    }
    Func_0808a188(9, 0, 20);
    Func_0808a368();
    Func_0808a370();
    Func_0808a210(0x01dd0000, -1, 0x14e0000, 0);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a138(10, 2);
    Func_0808a1b8(10, 0xb000, 10);
    Func_0808a170(0x108d);
    Func_02004248(10, 20);
    Func_0808a1b8(5, 0x2000, 0);
    Func_0808a1b8(9, 0x3000, 10);
    Func_0808a100(9, 4);
    Func_02004248(0x5009, 40);
    Func_0808a138(11, 1);
    Func_02004248(11, 10);
    Func_0808a130(5, 2);
    Func_0808a138(9, 2);
}
