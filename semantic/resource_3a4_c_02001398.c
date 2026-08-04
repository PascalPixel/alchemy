#include "types.h"

/*
 * resource_3a4 owner at 0x02001398, 736 bytes: a once-only cutscene. Bail
 * if either of two story flags is already set, set a third to mark it
 * played, then run sixty-odd staging calls -- place and light two actors,
 * step a long series of camera keys, run six dialogue lines -- and finish
 * through this overlay's own 0x02000ec0 before closing the scene.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. No
 * arguments, void.
 *
 * Complete owner: `push {r5, r6, lr}` at 0x02001398 through the
 * interworking return `pop {r5, r6} / pop {r0} / bx r0` at 0x02001644-
 * 0x02001648, one alignment halfword, and the eleven-word literal pool
 * 0x0200164c-0x02001677; the next owner is the already-drafted 0x02001678.
 * measureSpan reports 690, undersizing by the pool.
 *
 * IT CALLS 0x02000ec0 TOO. The site at 0x0200163c encodes `f000 ff5f`,
 * whose stored displacement + 2 is 0xec0 -- an in-image prologue, not a
 * veneer: Func_02000ec0(10). Together with resource_3a4_c_02000d2c.c,
 * which calls it with 5, that is TWO published callbacks calling the one
 * sweep-A row. 0x02000ec0's callers are both invisible to a BL walk, which
 * is why it sat in residue with a caller that no census could name.
 *
 * Sixty-six call sites, every one resolved with the `+2` rule via
 * `bun tools/overlay_call_targets.ts resource_3a4 0x1398 0x1678` and none
 * read from the listing's arrows, which print sixty-six nonexistent
 * 0x02004xxx and 0x02005xxx addresses. They collapse onto twenty-one
 * targets -- twenty veneers and the one in-image prologue. Thirteen sites
 * share Func_0808a1b8 alone and seven share Func_0808a0c0, so the listing
 * is at its most misleading here: twenty identical encodings printing
 * twenty different callees.
 *
 * The two guards are the established story-flag pair: Func_080770c0 tests
 * and Func_080770c8 sets. Flags 0x908 and 0xf14 are tested, 0x205 is set,
 * and BOTH guards jump to the same shared tail at 0x02001640 -- the tail
 * still runs Func_0808a020, so the early exit is not a plain return and
 * the two arms are not independent blocks.
 *
 * The handle flag byte is touched four times through Func_0808a080's
 * return plus 90: cleared of bit 0 twice and set twice, alternating around
 * the two Func_0808a010 waits. The register holding the mask (r5, then r6)
 * is REUSED as the result of the mask at 0x020014b0, so reading r5 as a
 * live constant after that point would be wrong; each site is transcribed
 * from its own instructions.
 *
 * Constants are written in the shifted-literal form the encodings build
 * -- 195 << 2, 200 << 2, 198 << 2, 199 << 2, 201 << 2, 191 << 2, 192 << 8,
 * 128 << 10, 128 << 9, 128 << 8, 128 << 7, 160 << 10, 160 << 9, 129 << 1
 * -- because that shape, not the flattened value, is what the row shows.
 * Pool constants 0xcccc, 0x6666, 0x19999, 0xe666, 0x316, 0x312, 0x101,
 * 0x1953, 0x908, 0xf14 and 0x205 are transcribed as words.
 */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_08015040(s32 arg0, s32 arg1);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_080770c8(s32 flag_id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a130(s32 id, s32 arg1);
extern void Func_0808a170(s32 arg0);
extern void Func_0808a188(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e8(s32 id, s32 arg1, s32 arg2);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a010(s32 frames);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 sound_id);
extern void Func_02000ec0(s32 arg0);

void Func_02001398(void)
{
    u8 *handle;

    Func_0808a018();
    Func_08015040(0x1953, 1);

    if (Func_080770c0(0x908) == 0 && Func_080770c0(0xf14) == 0) {
        Func_080770c8(0x205);

        Func_0808a090(0, 0xcccc, 0x6666);
        Func_0808a0d0(0, 0x316, 140);
        Func_0808a0d0(0, 195 << 2, 140);
        Func_0808a1b8(0, 192 << 8, 0);

        handle = Func_0808a080(0);
        if (handle != 0) {
            Func_0808a0f0(1, *(s32 *)(handle + 8), *(s32 *)(handle + 16));
        }

        Func_0808a090(1, 0xcccc, 0x6666);
        Func_0808a0d0(1, 200 << 2, 140);
        Func_0808a1b8(1, 192 << 8, 20);
        Func_0808a170(0x1953 + 1);
        Func_0808a100(1, 4);
        Func_0808a010(20);
        Func_0808a188(1, 0, 10);
        Func_0808a128(1, 6, 0);
        Func_0808a090(1, 0x19999, 0xcccc);

        handle = Func_0808a080(1);
        handle[90] = (u8)(handle[90] & 0xfe);

        Func_0808a0d0(1, 198 << 2, 110);
        Func_0808a010(1);

        handle = Func_0808a080(1);
        handle[90] = (u8)(handle[90] | 1);

        Func_080f9010(161);
        Func_080091f0(128 << 10, 128 << 9, 128 << 9);

        handle = Func_0808a080(1);
        handle[90] = (u8)(handle[90] & 0xfe);

        Func_0808a0d0(1, 198 << 2, 120);
        Func_0808a010(1);

        handle = Func_0808a080(1);
        handle[90] = (u8)(handle[90] | 1);

        Func_080091f0(-1, -1, 0xe666);
        Func_0808a010(80);
        Func_080f9010(141);
        Func_080091f0(128 << 9, 128 << 9, 128 << 9);
        Func_0808a010(40);

        Func_0808a1e8(0, 0x101, 0);
        Func_0808a1e8(1, 0x101, 60);
        Func_0808a1b8(0, 128 << 8, 0);
        Func_0808a1b8(1, 0, 20);
        Func_0808a1b8(0, 0, 0);
        Func_0808a1b8(1, 128 << 8, 40);
        Func_0808a1b8(0, 128 << 8, 0);
        Func_0808a1b8(1, 0, 40);
        Func_0808a1b8(0, 192 << 8, 0);
        Func_0808a1b8(1, 192 << 8, 40);
        Func_0808a1e8(1, 129 << 1, 60);
        Func_0808a1b8(1, 128 << 7, 20);
        Func_0808a130(1, 2);
        Func_0808a188(1, 0, 10);
        Func_0808a090(1, 160 << 10, 160 << 9);
        Func_0808a100(1, 5);

        Func_0808a0c0(1, 199 << 2, 138);
        Func_0808a1b8(0, 0, 0);
        Func_0808a0c0(1, 201 << 2, 140);
        Func_0808a1b8(0, 128 << 7, 0);
        Func_0808a0c0(1, 201 << 2, 166);
        Func_0808a0c0(1, 191 << 2, 166);
        Func_0808a0c0(1, 191 << 2, 198);
        Func_0808a0c0(1, 0x312, 198);
        Func_0808a1e8(0, 129 << 1, 0);
        Func_0808a0c0(1, 0x312, 246);

        Func_0808a100(1, 1);
        Func_0808a0f0(1, 0, 0);
        Func_0808a010(40);
        Func_02000ec0(10);
    }

    Func_0808a020();
}
