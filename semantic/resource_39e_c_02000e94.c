#include "types.h"

/*
 * resource_39e owner at 0x02000e94, 236 bytes: the ASCENT mirror of
 * resource_39e_c_02000db4.c -- same slot-19 record (Func_0808a080),
 * same four-pass loop (Func_080000c0(8/6/4/2), record word +64
 * rewritten 0x80000000 each pass) but record word +16 gains +0x10000
 * per pass instead of losing it, then a post-loop extra bump of
 * +0x180000 (192<<13) and one more +64 rewrite that the descent twin
 * does not have. Clears the u16 at +30 of the pointer in record word
 * +80, plays sound 227, and fires the mirrored four-spawn burst
 * through the overlay's eight-argument spawner:
 *   1: (rec[8],           rec[12], rec[16] + 0xc0000, -0xcccc, 0,  0x3333, 0, 0)
 *   2: (rec[8],           rec[12], rec[16] + 0xc0000,  0xcccc, 0,  0x3333, 0, 0)
 *   3: (rec[8] - 0x60000, rec[12], rec[16] - 0x80000,  0x3333, 0, 0x10000, 0, 0)
 *   4: (rec[8] + 0x60000, rec[12], rec[16] - 0x80000,  0x3333, 0, 0x10000, 0, 0)
 * i.e. every vertical sign in the descent twin's burst flipped.
 * Called 2x within this overlay.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/sl spill and a
 * 16-byte sp frame at 0x02000e94-0x02000e9e through the full unspill
 * to `pop {r0} / bx r0` at 0x02000f66-0x02000f68, five pool words plus
 * an alignment halfword ending 0x02000f7f; the next owner prologue at
 * 0x02000f80 is the already-drafted resource_39e_c_02000f80.c.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e e94 f68's
 * +2 rule; the four spawn sites are intra-overlay bl to Func_0200013c).
 *
 * Uncertainty: callees identified by shape; field meanings and the
 * fixed-point constants are transcribed, meaning unknown.
 */

extern u8 *Func_0808a080(s32 id);
extern void Func_080000c0(s32 arg0);
extern void Func_080f9010(s32 sound_id);
extern void Func_0200013c(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

void Func_02000e94(void)
{
    u8 *record;
    u32 i;
    s32 step;

    record = Func_0808a080(19);
    i = 0;
    step = 8;
    do {
        Func_080000c0(step);
        *(s32 *)(record + 16) += 128 << 9;
        *(u32 *)(record + 64) = 0x80000000;
        i++;
        step -= 2;
    } while (i <= 3);
    *(s32 *)(record + 16) += 192 << 13;
    *(u32 *)(record + 64) = 0x80000000;
    Func_080f9010(227);
    *(u16 *)(*(u8 **)(record + 80) + 30) = 0;
    Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (192 << 12), 0xffff3334,
                  0, 0x00003333, 0, 0);
    Func_0200013c(*(s32 *)(record + 8), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + (192 << 12), 0x0000cccc,
                  0, 0x00003333, 0, 0);
    Func_0200013c(*(s32 *)(record + 8) + 0xfffa0000, *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x00003333,
                  0, 128 << 9, 0, 0);
    Func_0200013c(*(s32 *)(record + 8) + (192 << 11), *(s32 *)(record + 12),
                  *(s32 *)(record + 16) + 0xfff80000, 0x00003333,
                  0, 128 << 9, 0, 0);
}
