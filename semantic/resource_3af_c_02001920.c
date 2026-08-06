#include "types.h"

/*
 * resource_3af owner at 0x02001920, 160 bytes: a near-twin of this
 * overlay's 0x0200185c (resource_3af_c_0200185c.c) -- same bracket,
 * same data block 0x0200d160 (Func_0808a030), same one-frame advance,
 * same id-20/23 placements, same closing test of story flag 0x109
 * calling Func_02004218 when SET. Three differences, all in the
 * actor-22 handling:
 *   - id 22 is placed at (134<<17, 0x02a60000) rather than
 *     (204<<16, 0x02090000);
 *   - actor 22 gets its u16 at record +6 CLEARED instead of the
 *     record +12 write and the byte +89 bit-7 set;
 *   - actor 22 gets only Func_0808a098(22, 0x0200c980) -- no
 *     Func_0808a090 pair.
 * Actor 21 is handled identically to the twin (byte +89 bit 7 set,
 * Func_0808a090(21, 0xcccc, 0x6666), Func_0808a098(21, 0x0200c628)).
 * Called 1x within this overlay.
 *
 * Complete owner: `push {lr}` at 0x02001920 through `pop {r0} / bx r0`
 * at 0x0200199a-0x0200199c, eight pool words plus an alignment
 * halfword ending 0x020019bf; the next owner prologue
 * (`push {r5, lr}`) begins at 0x020019c0.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_3af 1920 199a's
 * +2 rule; the Func_02004218 site is an intra-overlay bl).
 *
 * Uncertainty: callees and argument roles by shape; the fixed-point
 * pairs and data pointers transcribed, meanings unknown.
 */












extern void Func_02005c10(void);
extern void Func_02005c26(s32 arg0);
extern void Func_02005b8c(s32 arg0);
extern void Func_02005ca6(s32 id, s32 arg1, s32 arg2);
extern void Func_02005cb2(s32 id, s32 arg1, s32 arg2);
extern void Func_02005cbe(s32 id, s32 arg1, s32 arg2);
extern u8 * Func_02005c64(s32 id);
extern void Func_02005c88(s32 id, s32 arg1);
extern u8 * Func_02005c76(s32 id);
extern void Func_02005c9a(s32 id, s32 arg1, s32 arg2);
extern void Func_02005caa(s32 id, s32 arg1);
extern s32 Func_02005c58(s32 flagId);
extern void Func_02005bac(void);
extern void Func_02005c8c(void);
void Func_02001920(void)
{
    u8 *record;

    Func_02005c10();
    Func_02005c26(0x0200d160);
    Func_02005b8c(1);
    Func_02005ca6(20, 0, 0);
    Func_02005cb2(23, 238 << 16, 0x02720000);
    Func_02005cbe(22, 134 << 17, 0x02a60000);
    record = Func_02005c64(22);
    Func_02005c88(22, 0x0200c980);
    record = Func_02005c76(21);
    record[89] |= 0x80;
    Func_02005c9a(21, 0xcccc, 0x6666);
    Func_02005caa(21, 0x0200c628);
    *(u16 *)(record + 6) = 0;
    if (Func_02005c58(0x109) != 0) {
        Func_02005bac();
    }
    Func_02005c8c();
}
