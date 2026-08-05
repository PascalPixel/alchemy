#include "types.h"

/*
 * resource_39e owner at 0x02000db4, 224 bytes: a four-step descent
 * followed by a four-way spawn burst through this overlay's
 * eight-argument spawner (resource_39e_c_0200013c.c). Fetches the
 * slot-19 record (Func_0808a080), then loops four times (unsigned
 * counter, bls): Func_080000c0(step) with step = 8, 6, 4, 2, record
 * word +16 -= 0x10000 each pass, and record word +64 rewritten with
 * 0x80000000 each pass. After the loop the u16 at +30 of the pointer
 * held in record word +80 is cleared (the same record[80] field the
 * 0x02000bf4 row touches), sound 227 plays (Func_080f9010), and four
 * spawns fire, all descriptor-less (extra = 0):
 *   1: (rec[8],           rec[12], rec[16] - 0x80000, -0xcccc, 0, -0x3333, 0, 0)
 *   2: (rec[8],           rec[12], rec[16] - 0x80000,  0xcccc, 0, -0x3333, 0, 0)
 *   3: (rec[8] - 0x60000, rec[12], rec[16] + 0xa0000,  0x3333, 0, -0x10000, 0, 0)
 *   4: (rec[8] + 0x60000, rec[12], rec[16] + 0xa0000,  0x3333, 0, -0x10000, 0, 0)
 * (0xffff3334 = -0xcccc, 0xffffcccd = -0x3333, 0xfffa0000 = -0x60000,
 * 0xffff0000 = -0x10000 -- mirrored left/right pairs, transcribed from
 * the pool words.) Called 2x within this overlay.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl spill and a
 * 16-byte sp frame at 0x02000db4-0x02000dbe through the full unspill to
 * `pop {r0} / bx r0` at 0x02000e72-0x02000e74, seven pool words plus an
 * alignment halfword ending 0x02000e93; the next owner prologue
 * (0x02000e94, this round's next row) follows immediately.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e db4 e74's
 * +2 rule; the four spawn sites are intra-overlay bl to Func_0200013c).
 *
 * Uncertainty: callees identified by shape; field meanings and the
 * fixed-point constants are transcribed, meaning unknown.
 */

extern u8 *Func_020051c6(s32 id);
extern void Func_02005118(s32 arg0);
extern void Func_02005370(s32 sound_id);
extern void Func_02000f4c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000f62(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000f86(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);
extern void Func_02000fa2(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

void Func_02000db4(void)
{
    u8 *record;
    u32 i;
    s32 step;

    record = Func_020051c6(19);
    i = 0;
    step = 8;
    do {
        Func_02005118(step);
        *(s32 *)(record + 16) += 0xffff0000;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 0;
    Func_02005370(227);
    Func_02000f4c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0xffff3334,
                  0, 0xffffcccd, 0, 0);
    Func_02000f62(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x0000cccc,
                  0, 0xffffcccd, 0, 0);
    Func_02000f86(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
    Func_02000fa2(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (160 << 12), 0x00003333,
                  0, 0xffff0000, 0, 0);
}
