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
 * by `bl` (bun tools/lib/overlay_call_targets.ts resource_3af 1920 199a's
 * +2 rule; the Func_02004218 site is an intra-overlay bl).
 *
 * Uncertainty: callees and argument roles by shape; the fixed-point
 * pairs and data pointers transcribed, meanings unknown.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a030(s32 arg0);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_080000c0(s32 arg0);
extern s32 Func_080770c0(s32 flagId);
extern void Func_02004218(void);

void Func_02001920(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a030(0x0200d160);
    Func_080000c0(1);
    Func_0808a0f0(20, 0, 0);
    Func_0808a0f0(23, 238 << 16, 0x02720000);
    Func_0808a0f0(22, 134 << 17, 0x02a60000);
    record = Func_0808a080(22);
    Func_0808a098(22, 0x0200c980);
    record = Func_0808a080(21);
    record[89] |= 0x80;
    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a098(21, 0x0200c628);
    *(u16 *)(record + 6) = 0;
    if (Func_080770c0(0x109) != 0) {
        Func_02004218();
    }
    Func_0808a020();
}
