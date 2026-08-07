#include "types.h"

/*
 * resource_3c9 owner at 0x020008b4, 2508 bytes: the second-largest owner
 * in this overlay and the longest single scripted cutscene drafted in it
 * -- 255 call sites over 40 distinct callees, exactly one conditional
 * branch (the skip-beat counter increment near the end). A full
 * scripted-scene bracket: Func_0808a018() on entry, Func_0808a020() as
 * the last call before the epilogue.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + the r8/r9/sl/fp spill at
 * 0x020008b4-0x020008c0 through `pop {r3, r5, r6, r7} / mov r8,r3 /
 * mov r9,r5 / mov sl,r6 / mov fp,r7 / pop {r5, r6, r7} / pop {r0} /
 * bx r0` at 0x0200123a-0x2001248, trailing pool 0x200124a-0x200127f;
 * next owner (resource_3c9_c_02001280.c, already drafted) exactly at
 * 0x02001280. Two inline pool clusters (0x2000a6c-0x2000a87,
 * 0x2000e98-0x2000edb) are reached only by unconditional forward
 * branches. No incoming arguments read, so `void`.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_3c9 8b4 1280's
 * +2 rule). The queue's ~600-byte estimate was wrong; the real span is
 * 0x8b4-0x1280.
 *
 * SHARED IDIOMS, cited not re-derived: the scripted-scene bracket
 * Func_0808a018/Func_0808a020 (resource_38d_c_020005f4.c), the
 * skip-beat counter u16 at workspace+472 gated on Func_0808a070(0,0)==1
 * (resource_38b_c_02000584.c names the cell; the ==1 gate shape here),
 * and the workspace displacement/value write idiom (first named in
 * resource_377_c_02000a0c.c) -- HERE with a new value: displacement
 * 448 = 224<<1 as usual, but value 514 (448+66), NOT the recurring 513.
 * Transcribed from the arithmetic, not the precedent.
 *
 * Local callees: Func_02001280 (this overlay's own drafted two-way
 * branch owner, called with (id, 0|1)), and the two small pre-owner
 * helpers at 0x02000894 / 0x020008a8 (declared, not drafted; 0x894
 * takes one argument that is sometimes a small id (1, 3, 19, 21) and
 * sometimes a packed constant (0x2013, 0x2014, 0x4013, 0x8001, 0x8015,
 * 0xa014), 0x8a8 takes (same-kind arg, u16-range value) -- recorded as
 * s32 pass-throughs, roles unresolved). Func_0808a188's first argument
 * gets the same mixed id/packed-constant treatment at five sites --
 * kept as compiled.
 *
 * Uncertainty: the dialogue/text ids (0x101..0x105 family, 0x202,
 * 0x209, 0x212, 0x213, 0x245, 0x2757), the fade constants
 * (0xcccc/0x6666, 0x19999/0x3333, 0x9999/0x4cccc, 0x6666/0xccc pairs
 * through Func_0808a208/Func_0808a090), and record field roles
 * (+6 u16, +8/+0xc/+0x10/+0x28 words, +0x23/+0x55/+0x5a bytes) are
 * recorded by shape only.
 */

extern u8 *Data_03001ebc;

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern s32 Func_0808a070(s32 arg0, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0b0(s32 id, s32 arg1);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a118(s32 id);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a170(s32 textId);
extern void Func_0808a178(s32 arg0, s32 arg1);
extern void Func_0808a188(s32 arg0, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 arg1);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern u8 *Func_0808a228(void);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080000c0(s32 arg0);
extern void Func_08009128(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080f9010(s32 arg0);
extern void Func_02000894(s32 arg0);
extern void Func_020008a8(s32 arg0, s32 arg1);
extern void Func_02001280(s32 arg0, s32 arg1);

void Func_020008b4(void)
{
    u8 *record;
    u8 *record0;
    u8 *workspace;

    record = Func_0808a080(18);
    Func_0808a018();
    Func_02001280(1, 0);
    Func_02001280(2, 0);
    Func_02001280(3, 0);
    Func_0808a210(-1, -1, -1, 0);
    Func_080000c0(1);
    record[0x55] = 0;
    record[0x23] |= 2;

    Func_080091e0(Func_0808a080(18), 0);
    Func_0808a1e0(18, 1);
    Func_0808a0f0(18, 145 << 18, 169 << 17);

    record0 = Func_0808a080(0);
    record0[0x55] = 0;
    Func_0808a1e0(0, 1);
    Func_0808a0f0(0, 0x02450000, 144 << 17);
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_0808a090(18, 128 << 9, 128 << 8);
    Func_0808a090(0, 128 << 9, 128 << 8);
    record = Func_0808a080(0);
    record[0x5a] &= ~1;

    /* first dialogue exchange */
    Func_0808a0b8(18, 145 << 2, 221);
    Func_0808a0c0(0, 0x245, 171);
    Func_0808a0b8(18, 0x212, 211);
    Func_0808a0c0(0, 0x213, 161);
    Func_0808a0b8(18, 130 << 2, 191);
    Func_0808a0c0(0, 0x209, 141);
    Func_080091e0(Func_0808a080(18), 1);
    Func_0808a0b8(18, 0x203, 171);
    Func_0808a0c0(0, 129 << 2, 121);
    Func_080f9010(144 << 1);
    Func_0808a010(20);

    Func_0808a100(0, 6);
    Func_0808a118(0);
    *(s32 *)(record0 + 8) = 129 << 18;
    *(s32 *)(record0 + 0xc) = 128 << 12;
    *(s32 *)(record0 + 0x10) = 148 << 16;
    *(u16 *)(record0 + 6) = 128 << 8;
    record0[0x55] = 3;
    Func_080f9010(152);
    *(s32 *)(record0 + 0x28) = 128 << 11;
    Func_080f9010(152);
    Func_080091e0(Func_0808a080(0), 1);
    Func_0808a0c0(0, 252 << 1, 148);
    Func_0808a010(10);

    record = Func_0808a080(0);
    record[0x5a] |= 1;
    *(s32 *)(record0 + 0xc) = 0xffe00000;
    *(u16 *)(record0 + 6) = 128 << 7;
    Func_0808a010(20);
    Func_080f9010(154 << 1);

    Func_0808a0c0(18, 131 << 2, 191);
    Func_080091e0(Func_0808a080(18), 0);
    Func_0808a0c0(18, 0x212, 211);
    Func_0808a0c0(18, 145 << 2, 221);
    Func_0808a0b8(18, 145 << 2, 169 << 1);

    record = Func_0808a080(0);
    record[0x23] |= 1;
    Func_0808a090(0, 0xcccc, 0x6666);

    Func_02001280(1, 1);
    Func_02001280(2, 1);
    Func_02001280(3, 1);

    Func_0808a0c8(0, 246 << 1, 164);
    Func_0808a0c8(1, 0x202, 164);
    Func_0808a0c8(2, 246 << 1, 140);
    Func_0808a0d0(3, 0x202, 140);
    Func_0808a100(0, 1);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(0, 128 << 7, 0);
    Func_0808a1b8(1, 128 << 7, 0);
    Func_0808a1b8(2, 128 << 7, 0);
    Func_0808a1b8(3, 128 << 7, 0);
    Func_0808a0e8(18);
    Func_0808a0f0(18, 0, 0);

    Func_080f9010(0x121);
    Func_0808a170(0x2757);
    Func_02000894(0x4013);

    Func_0808a1b8(0, 128 << 8, 0);
    Func_0808a1b8(1, 128 << 8, 0);
    Func_0808a1b8(2, 128 << 8, 0);
    Func_020008a8(3, 128 << 8);

    record = Func_0808a228();
    record[0x55] = 0;
    Func_0808a208(0x0004cccc, 0x9999);
    Func_0808a210(152 << 17, 128 << 14, 158 << 16, 1);
    Func_0808a218();
    Func_0808a010(20);

    Func_020008a8(20, 208 << 8);
    Func_0808a138(20, 1);
    Func_080f9010(61);
    Func_02000894(0x2014);
    Func_0808a100(19, 4);
    Func_02000894(0x4013);
    Func_020008a8(20, 176 << 8);
    Func_0808a1e8(20, 0x105, 40);
    Func_02000894(0x2014);
    Func_02000894(21);
    Func_0808a1e8(19, 128 << 1, 0);
    Func_0808a1e8(20, 128 << 1, 20);
    Func_0808a1b8(6, 192 << 6, 0);
    Func_0808a1b8(19, 160 << 7, 0);
    Func_0808a1b8(20, 160 << 7, 20);
    Func_0808a208(0x19999, 0x3333);
    Func_0808a210(147 << 17, -1, 194 << 16, 1);
    Func_0808a010(20);
    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a0d0(21, 136 << 1, 200);
    Func_0808a138(20, 1);
    Func_0808a010(20);
    Func_02000894(0x2014);
    Func_0808a1e8(19, 0x103, 20);
    Func_02000894(19);
    Func_0808a110(21, 3);
    Func_02000894(21);
    Func_0808a1e8(20, 0x101, 40);
    Func_02000894(0x2014);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Func_0808a1e8(19, 0x101, 60);
    Func_0808a188(19, 0, 40);
    Func_0808a1e8(19, 131 << 1, 40);
    Func_020008a8(19, 128 << 8);
    Func_02000894(0x2013);
    Func_0808a1b8(6, 0, 0);
    Func_0808a1e8(21, 0x103, 40);
    Func_02000894(21);
    Func_0808a1b8(19, 192 << 6, 0);
    Func_0808a1b8(20, 176 << 8, 40);
    Func_0808a138(21, 1);
    Func_02000894(21);
    Func_0808a1b8(20, 160 << 7, 0);
    Func_020008a8(19, 160 << 7);
    Func_0808a1e8(19, 132 << 1, 20);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x103, 20);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(20, 128 << 8, 40);
    Func_0808a110(20, 4);
    Func_0808a188(0x2014, 0, 40);
    Func_0808a110(21, 3);
    Func_0808a188(21, 0, 20);
    Func_0808a1b8(20, 160 << 7, 20);
    Func_0808a130(21, 2);
    Func_02000894(21);
    Func_0808a1e8(20, 0x105, 0);
    Func_0808a1e8(19, 0x105, 80);
    Func_0808a130(21, 2);
    Func_02000894(21);
    Func_0808a1e8(19, 0x101, 60);
    Func_02000894(0x2013);
    Func_0808a110(21, 3);
    Func_02000894(21);
    Func_0808a130(19, 1);
    Func_0808a138(20, 1);
    Func_0808a010(20);
    Func_0808a110(21, 4);
    Func_0808a188(21, 0, 20);
    Func_0808a1e8(20, 0x105, 60);
    Func_0808a188(0x2014, 0, 20);
    Func_020008a8(21, 176 << 8);
    Func_02000894(21);
    Func_020008a8(6, 192 << 6);
    Func_0808a138(6, 2);
    Func_0808a010(20);
    Func_020008a8(21, 208 << 8);
    Func_0808a110(19, 4);
    Func_02000894(0x2013);
    Func_0808a1b8(6, 0, 0);
    Func_0808a138(21, 1);
    Func_02000894(21);
    Func_0808a110(20, 3);
    Func_02000894(0x2014);
    Func_0808a110(21, 4);
    Func_02000894(21);
    Func_0808a1e8(20, 0x101, 0);
    Func_0808a1e8(19, 0x101, 80);
    Func_0808a1b8(19, 128 << 8, 0);
    Func_0808a1b8(20, 128 << 8, 0);
    Func_0808a208(0x6666, 0xccc);
    Func_0808a210(147 << 17, -1, 180 << 16, 1);
    Func_0808a0d0(21, 131 << 1, 176);
    Func_0808a1b8(21, 128 << 8, 40);
    Func_0808a1b8(21, 0, 20);
    Func_0808a130(21, 2);
    Func_02000894(0x8015);
    Func_0808a1e8(19, 128 << 1, 20);
    Func_02000894(0x2013);
    Func_0808a130(21, 2);
    Func_02000894(0x8015);
    Func_0808a1e8(20, 0x103, 40);
    Func_0808a188(0xa014, 0, 20);
    Func_0808a1e8(21, 0x105, 20);
    Func_02000894(0x8015);
    Func_0808a1e8(19, 0x103, 20);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x101, 40);
    Func_02000894(0x8015);
    Func_0808a110(20, 4);
    Func_02000894(0xa014);
    Func_0808a110(19, 3);
    Func_02000894(0x2013);
    Func_0808a1e8(21, 0x103, 60);
    Func_0808a1b8(21, 128 << 8, 20);
    Func_0808a188(0xa015, 0, 40);
    Func_0808a1e8(6, 0x105, 120);
    Func_0808a1e8(20, 0x105, 60);
    Func_02000894(0xa014);
    Func_0808a1b8(21, 0, 40);
    Func_0808a100(19, 3);
    Func_02000894(0x2013);
    Func_0808a138(21, 1);
    Func_0808a188(0x8015, 0, 20);
    Func_02000894(0xa014);
    Func_0808a1e8(21, 128 << 1, 40);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);
    Func_0808a1e8(6, 0x105, 40);
    Func_0808a1e8(20, 132 << 1, 40);
    Func_02000894(0xa014);
    Func_0808a1e8(19, 0x103, 20);
    Func_02000894(0x2013);
    Func_0808a138(21, 1);
    Func_0808a010(20);
    Func_0808a130(20, 2);
    Func_02000894(0xa014);
    Func_0808a100(19, 4);
    Func_02000894(0x2013);

    /* workspace+448 = 514 -- the displacement/value idiom with a new
     * value (448+66), not the usual 513 */
    workspace = Data_03001ebc;
    *(u32 *)(workspace + 448) = 514;

    Func_0808a368();
    Func_0808a370();
    Func_0808a210(252 << 17, 0xffe80000, 168 << 16, 0);
    Func_080000c0(1);
    Func_08009128();
    Func_080000c0(1);
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);
    Func_0808a138(1, 1);
    Func_02000894(0x8001);
    Func_0808a1e8(3, 0x101, 40);
    Func_02000894(3);
    Func_0808a110(2, 3);
    Func_0808a188(0x1002, 0, 40);
    Func_0808a128(1, 2, 20);
    Func_02000894(0x8001);
    Func_0808a178(0x8001, 0);
    Func_0808a1b8(0, 0, 0);
    Func_0808a1b8(2, 128 << 7, 0);
    Func_0808a1b8(3, 192 << 6, 0);

    if (Func_0808a070(0, 0) == 1) {
        *(u16 *)(Data_03001ebc + 472) += 1; /* skip-beat counter */
    }

    Func_0808a010(20);
    Func_02000894(1);
    Func_0808a1b8(2, 0, 0);
    Func_0808a1b8(3, 128 << 8, 20);
    Func_0808a100(0, 3);
    Func_0808a100(1, 3);
    Func_0808a100(2, 3);
    Func_0808a110(3, 3);
    Func_0808a098(1, 0x0200dfc4);
    Func_0808a098(2, 0x0200dfc4);
    Func_0808a0b0(3, 0x0200dfc4);
    Func_0808a010(20);
    Func_0808a020();
}
