typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/*
 * resource_39e owner at 0x02000bf4, 224 bytes: the FORWARD twin of the
 * retreat-along-arc owner resource_39e_c_02000cd4.c, immediately before
 * it in the image. Same slot-19 record, same BAM-angle sweep written as
 * a u16 into +30 of the sub-object at +80, same multiply-by-six
 * along/across trig pairing (Func_08000120/Func_08000118) -- but the
 * sweep runs DOWN (angle index 8 to 4, 0x8000 to 0x4000), the wait is
 * (index - 4) * 2 frames (steps speed up as the sweep ends, mirroring
 * cd4's (12 - index) * 2), and both trig terms are ADDED to +8/+16
 * where cd4 subtracts. After the sweep +12 and +60 take 18.0
 * (144 << 13) -- cd4's extra -0.8 write to +24 is absent here -- then
 * sound 227 and three spawns through the local eight-argument spawner,
 * the sign-flipped, order-reversed image of cd4's burst:
 *   x = +8 - 12.0,  -0.2 (0xffffcccd), 0.4 (0x6666)
 *   x = +8,         -0.8 (0xffff3334), 0.3 (0x4ccc)
 *   x = +8 + 10.0,  -1.0 (0xffff0000), 0.2 (0x3333)
 * all sharing y = +12 and z = +16 + 8.0, last three stack arguments
 * zero. Called 3x within this overlay.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8 spill and a
 * 16-byte sp frame at 0x02000bf4-0x02000bfe through `add sp, #16 /
 * pop {r3} / mov r8, r3 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02000caa-0x02000cb4, seven pool words plus an alignment halfword
 * ending 0x02000cd3; the next owner prologue at 0x02000cd4 is the
 * already-drafted cd4 twin itself.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e bf4 cb4's
 * +2 rule; the three spawn sites are intra-overlay bl to Func_0200013c).
 *
 * The loop is a decrementing do-while closed by `cmp r6, #3 / bhi` --
 * unsigned counter per the signedness rule. Uncertainty: callees by
 * shape; constants transcribed, meaning unknown.
 */

u8 *Func_02005004();           /* record fetch, returns the record */
void Func_02004f56();          /* advance the task scheduler */
s32 Func_02004f8c();           /* along-axis trig lookup */
s32 Func_02004f96();           /* across-axis trig lookup */
void Func_020051cc();          /* one-argument notify */
void Func_02000dae();          /* local: spawn, eight arguments (entry 1) */
void Func_02000dc6();          /* local: spawn, eight arguments (entry 2) */
void Func_02000de4();          /* local: spawn, eight arguments (entry 3) */

void Func_02000bf4(void)
{
    u8 *record = Func_02005004(19);
    u32 index;
    s32 angle;

    for (index = 8; index > 3; index--) {
        angle = index << 12;
        *(u16 *)(*(u8 **)(record + 80) + 30) = (u16)angle;
        Func_02004f56((index - 4) * 2);
        *(s32 *)(record + 8) += Func_02004f8c(angle) * 6;
        *(s32 *)(record + 16) += Func_02004f96(angle) * 6;
    }

    *(s32 *)(record + 12) = 0x120000;          /* 144 << 13, i.e. 18.0 */
    *(s32 *)(record + 60) = 0x120000;

    Func_020051cc(227);

    Func_02000dae(*(s32 *)(record + 8) - 0xc0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffffcccd, 0x6666, 0, 0, 0);
    Func_02000dc6(*(s32 *)(record + 8),
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff3334, 0x4ccc, 0, 0, 0);
    Func_02000de4(*(s32 *)(record + 8) + 0xa0000,
                  *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0x80000,
                  0xffff0000, 0x3333, 0, 0, 0);
}
