#include "types.h"

/*
 * resource_3a4 owner at 0x02001d0c, 788 bytes: the SIBLING of the row
 * immediately above it. Another linear cutscene on actor 10 -- the same
 * camera-placement and fade idiom, the same actor-handle field group, the
 * same closing shot from actor 0 -- ending by handing a scene off to this
 * overlay's own 0x02002bb0 and then WAITING for it, and setting story
 * flag 0x905 where its neighbour sets 0x904.
 *
 * A PUBLISHED CALLBACK (sweep B): no `bl` in the image reaches it. No
 * arguments, void.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x02001d0c through the
 * high-register unwind `pop {r3, r5, r6, r7} / mov r8, r3 / mov r9, r5 /
 * mov sl, r6 / mov fp, r7 / pop {r5, r6, r7} / pop {r0} / bx r0` at
 * 0x02001ff6-0x02002004, one alignment halfword, and the six-word
 * literal pool 0x02002008-0x0200201f; the next owner is the
 * already-drafted 0x02002020. That is 788 bytes.
 *
 * THE SPAN WAS NOT MEASURED TWICE. 0x02001838 measured to exactly its
 * recorded-owner upper bound of 1236, and those two rows tile
 * 0x02001838..0x02002020 with nothing between them, so 788 was fixed
 * before this row was read; the epilogue and pool above merely confirm
 * it. measureSpan reports 762, undersizing by the closing pool, and was
 * not used as a bound in either direction.
 *
 * IT PUBLISHES 0x02001770 TOO. The pool word at 0x02001df0 is
 * 0x02009771 -- image offset 0x1770 with the Thumb bit under the
 * base + 0x8000 in-image spelling -- stored into the actor handle at
 * +108 twice and cleared twice, and never called. With 0x02001838 that
 * makes 0x02001770 a row with TWO publishers and no callers at all.
 *
 * Sixty-eight call sites, every one resolved with the `+2` rule via
 * `bun tools/lib/overlay_call_targets.ts resource_3a4 0x1d0c 0x2020` and
 * none read from the listing's arrows. Site count cross-checked: the
 * resolver reports sites=68 for these bounds and the listing carries 68
 * `bl` lines. They collapse onto eighteen targets: seventeen import
 * veneers and ONE in-image prologue -- the site at 0x02001fa6 resolves
 * to 0x2002bb0, so Func_02002bb0(130 << 16, 0, 168 << 16, 0), whose
 * four-argument signature is the one that owner's own draft already
 * carries.
 *
 * THE ONLY BRANCHING IN THE ROW is the closing wait, 0x02001fb0-
 * 0x02001fca: after handing the scene to Func_02002bb0 and yielding 60
 * frames it polls the IWRAM word at 0x03001c94, and if that word is
 * still zero it yields one frame at a time until the word goes non-zero
 * or a counter passes 59. The counter is r5 -- the SAME register that
 * has been the zero written into the handle fields for the whole body,
 * reused as the loop variable at 0x02001fbc. Reading it as a live zero
 * there would silently make the loop unbounded.
 *
 * Three more registers are recycled in the tail and none may be read as
 * live across the change: r7 holds the actor-10 handle from 0x02001d20
 * until 0x02001fd4, where it becomes the actor-0 handle; r6 holds the
 * published pointer until 0x02001fb8, where it becomes the address
 * 0x03001c94; sl holds 0 throughout and fp holds handle + 102
 * throughout, so those two are safe.
 *
 * The handle flags byte at Func_0808a080(10) + 90 is cleared of bit 0
 * once and set once around the middle fade, and the separate byte at
 * +91 is cleared twice through r9; those are two different fields, not
 * one read two ways.
 *
 * Constants are written in the shifted-literal form the encodings build
 * -- 128 << 15, 216 << 16, 128 << 16, 202 << 16, 192 << 6, 160 << 7,
 * 128 << 8, 128 << 9, 128 << 10, 128 << 11, 130 << 16, 168 << 16 --
 * because that shape, not the flattened value, is what the row shows.
 * Note 0x02001eca is `lsls r0, r0, #10` where every sibling fade uses
 * #9, so that one Func_080091f0 gets 128 << 10 and is not a copy of the
 * others. Pool constants 0x26666, 0x4ccc, 0x1170000, 0xcccc, 0x1999,
 * 0x6666, 0x13333, 0x9999, 0xe666, 0x4cccc and 0x905 are transcribed as
 * words.
 */

extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a138(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a208(s32 arg0, s32 arg1);
extern void Func_0808a210(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void Func_0808a218(void);
extern void Func_080091e0(u8 *handle, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080770c8(s32 flag_id);
extern void Func_080f9010(s32 sound_id);
extern void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern s32 Data_03001c94;   /* polled by the closing wait */

/* image offset 0x1770 with the Thumb bit: this overlay's own 0x02001770 */
#define PUBLISHED_02001770 0x02009771

void Func_02001d0c(void)
{
    u8 *actor;
    u8 *other;
    s32 waited;

    actor = Func_0808a080(10);
    Func_0808a018();
    Func_0808a0a8(10);

    Func_0808a208(0x26666, 0x4ccc);
    Func_0808a210(0x1170000, 128 << 15, 216 << 16, 1);
    Func_0808a218();
    Func_080f9010(147);
    Func_0808a138(10, 2);
    Func_0808a010(40);

    Func_0808a1b8(10, 192 << 6, 20);
    Func_0808a1b8(10, 160 << 7, 20);
    Func_0808a1b8(10, 128 << 8, 40);

    Func_0808a208(0xcccc, 0x1999);
    Func_0808a210(128 << 16, 128 << 15, 202 << 16, 1);

    waited = 0;

    *(u16 *)(actor + 100) = 0;
    *(s32 *)(actor + 104) = 0;
    *(u16 *)(actor + 102) = 0;
    *(s32 *)(actor + 72) = 0x6666;
    *(s32 *)(actor + 108) = PUBLISHED_02001770;

    Func_0808a090(10, 0x13333, 0x9999);
    Func_0808a0c0(10, 212, 200);
    Func_0808a0c0(10, 103, 200);

    actor[91] = 0;
    *(s32 *)(actor + 108) = 0;

    Func_0808a010(10);
    Func_0808a100(10, 1);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(20);
    Func_0808a1b8(10, 160 << 7, 40);

    other = Func_0808a080(10);
    other[90] = (u8)(other[90] & 0xfe);

    Func_0808a090(10, 0x13333, 0x9999);
    Func_080091e0(Func_0808a080(10), 0);
    Func_080f9010(153);

    other = Func_0808a080(10);
    *(s32 *)(other + 40) = 128 << 11;

    Func_0808a100(10, 3);
    Func_0808a0c0(10, 86, 214);
    Func_0808a100(10, 1);
    Func_080091e0(Func_0808a080(10), 1);

    Func_0808a010(10);
    Func_080f9010(229);
    Func_080091f0(128 << 10, 0, 128 << 9);
    Func_0808a010(8);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);

    other = Func_0808a080(10);
    other[90] = (u8)(other[90] | 1);

    Func_0808a1b8(10, 192 << 6, 20);
    Func_0808a1b8(10, 0, 40);

    *(s32 *)(actor + 104) = 0;
    *(u16 *)(actor + 100) = 0;
    *(u16 *)(actor + 102) = 0;
    *(s32 *)(actor + 108) = PUBLISHED_02001770;

    Func_0808a090(10, 0x13333, 0x9999);
    Func_0808a0c0(10, 120, 215);

    actor[91] = 0;
    *(s32 *)(actor + 108) = 0;

    Func_0808a100(10, 1);
    Func_0808a010(16);
    Func_080f9010(229);
    Func_080091f0(128 << 9, 0, 128 << 9);
    Func_0808a010(4);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(40);
    Func_0808a1b8(10, 0, 10);

    Func_080f9010(147);
    Func_0808a138(10, 2);
    Func_0808a010(80);
    Func_0808a100(10, 3);

    Func_02002bb0(130 << 16, 0, 168 << 16, 0);
    Func_0808a010(60);

    if (Data_03001c94 == 0) {
        do {
            Func_0808a010(1);
            waited++;
            if (waited > 59) {
                break;
            }
        } while (Data_03001c94 == 0);
    }

    other = Func_0808a080(0);
    Func_0808a208(0x4cccc, 0x9999);
    Func_0808a210(*(s32 *)(other + 8), *(s32 *)(other + 12),
                  *(s32 *)(other + 16), 1);
    Func_0808a218();

    Func_080770c8(0x905);
    Func_0808a020();
}
