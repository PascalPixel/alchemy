typedef unsigned char u8;
typedef int s32;
typedef unsigned int u32;

/*
 * resource_3af owner at 0x0200185c, 196 bytes: a scene-entry setup
 * inside the Func_0808a018/Func_0808a020 bracket. Registers the data
 * block 0x0200d160 (Func_0808a030), advances one frame
 * (Func_080000c0(1)), then places three actors with Func_0808a0f0 --
 * id 20 at (0,0), id 23 at (238<<16, 0x02720000), id 22 at
 * (204<<16, 0x02090000). Actors 22 and 21 each get the same treatment:
 * fetch the record (Func_0808a080), set bit 7 of byte +89, then
 * Func_0808a090 with a fixed-point pair (22: 0x9999/0x4ccc, 21:
 * 0xcccc/0x6666) and Func_0808a098 with an overlay data pointer
 * (22: 0x0200c58c, 21: 0x0200c628). Actor 22 additionally gets
 * record +12 = 0x100000 before the flag bit. Closes by testing story
 * flag 0x109 (Func_080770c0) and, if SET, calling this overlay's own
 * Func_02004218 (already covered on main), then the bracket close.
 * Called 1x within this overlay.
 *
 * Note the branch sense: `bne` past the call means the call runs when
 * the flag test returns nonzero -- the opposite of the 0x02000bb8
 * twins' once-only guard in the same overlay.
 *
 * Complete owner: `push {r5, lr}` at 0x0200185c through `pop {r5} /
 * pop {r0} / bx r0` at 0x020018f0-0x020018f4, nine pool words plus an
 * alignment halfword ending 0x0200191f; the next owner prologue
 * (`push {lr}`, the queue's 0x02001920 row) begins at 0x02001920.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_3af 185c 18f2's
 * +2 rule; the Func_02004218 site is an intra-overlay bl).
 *
 * Uncertainty: callees and argument roles by shape; the fixed-point
 * pairs and the three data pointers are transcribed, meanings unknown.
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

void Func_0200185c(void)
{
    u8 *record;

    Func_0808a018();
    Func_0808a030(0x0200d160);
    Func_080000c0(1);
    Func_0808a0f0(20, 0, 0);
    Func_0808a0f0(23, 238 << 16, 0x02720000);
    Func_0808a0f0(22, 204 << 16, 0x02090000);
    record = Func_0808a080(22);
    *(s32 *)(record + 12) = 128 << 13;
    record = Func_0808a080(22);
    record[89] |= 0x80;
    Func_0808a090(22, 0x9999, 0x4ccc);
    Func_0808a098(22, 0x0200c58c);
    record = Func_0808a080(21);
    record[89] |= 0x80;
    Func_0808a090(21, 0xcccc, 0x6666);
    Func_0808a098(21, 0x0200c628);
    if (Func_080770c0(0x109) != 0) {
        Func_02004218();
    }
    Func_0808a020();
}
