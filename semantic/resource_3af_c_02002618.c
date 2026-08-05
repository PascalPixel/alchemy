#include "types.h"

/*
 * resource_3af owner at 0x02002618, 956 bytes: the overlay's climax
 * scene -- a four-slot spawn fan-out with null-guarded placement, a
 * screen-effect pair, and a hand-off that writes the same EWRAM control
 * block the 0x02001444 driver reads its selector from. It is dispatch
 * case 15 of that driver.
 *
 * TRAP-3 AGAIN: an internal literal pool at 0x0200275c-0x0200277f sits
 * INSIDE the body, hopped by the `b.n 0x02002780` at 0x0200275a. Its
 * last word, 0x0000e666 at 0x0200277c, disassembles as
 * `b.n 0x0200244c` -- a backward branch that would land in the middle
 * of the NEIGHBOURING owner 0x020022c0. It is data. A branch census run
 * without hopping the pool reports an impossible cross-function jump
 * and invites exactly the wrong conclusion about the span.
 *
 * Opening: Func_0808a018, data block 0x0200d418 (this owner's own --
 * neither the shared 0x0200d160 nor 0x02001db0's 0x0200d208 nor
 * 0x020022c0's 0x0200d340), a frame, Func_0808a200(25, 1), another
 * frame, then Func_0808a100(21, 5) and the id-record pass-through on
 * slot 21. The transition mode is 0x202 (514) straight from a pool
 * word, followed by the full Func_0808a360/Func_0808a370 pair.
 *
 * Func_02000bb8, this overlay's once-only flag-0x271 reward grant, is
 * called at 0x0200267e -- the FIFTH and last caller, and the earliest
 * one in its own scene: twenty frames after the transition opens,
 * before any of the staging. Across all five callers (0x02001c14,
 * 0x02001db0, 0x020022c0, 0x020029d4 and this one) the grant is always
 * inside a scripted beat and never in a dispatcher, and the driver's
 * own handling of flags 0x271/0x272 -- it CLEARS both when flag 0x109
 * is set -- makes the design plain: the twins are per-playthrough
 * grants that a 0x109 replay resets.
 *
 * Two Func_080091f0 calls bracket a 20-frame wait: (0x40000, 0x40000,
 * 0x10000) and then (-1, -1, 0xe666). The negatives are built with
 * `movs r0,#1 / negs r0,r0`, so they are genuine -1 arguments, not a
 * large unsigned constant.
 *
 * Actor 22's record is configured by raw field writes rather than
 * helpers: +40 = 0x80000, +72 = 0xb333, +24 and +28 = 0x1a000,
 * +68 = 0x8000, and +108 = a THUMB FUNCTION POINTER. The pool word is
 * 0x020088c1, which in the overlay's +0x8000 link form is
 * Func_020008c0 (already tracked byte-exact) with the Thumb bit set. So
 * this scene installs a per-record callback; it is spelled as the
 * address-of expression, not as the raw constant, because the target is
 * resolved and certain.
 *
 * The fan-out is three copies of one null-guarded idiom:
 *
 *     record = Func_0808a080(0);
 *     if (record != 0)
 *         Func_0808a0f0(n, *(s32 *)(record + 8), *(s32 *)(record + 16));
 *
 * for n = 1, 2, 3 -- slots 1..3 are placed at slot 0's own position
 * fields. The record is re-fetched before each test; the guard is
 * re-evaluated three times, not hoisted. Slots 0..3 are then dressed
 * together in four parallel runs (Func_0808a090, Func_0808a0c8,
 * Func_0808a100, Func_0808a1b8, Func_0808a1f0), which is what makes the
 * back half of this function long rather than complicated.
 *
 * The close writes three fields of the EWRAM block at 0x02000240 --
 * `*(u16 *)(base + 452) = 0x6f`, `*(u16 *)(base + 454) = 30`,
 * `*(u8 *)(base + 555) = 3` -- and this is the cross-link worth having:
 * the 0x02001444 driver reads its scene selector from
 * `*(s16 *)(base + 450)`, two bytes below the first of these. The same
 * structure carries the driver's dispatch state and this scene's
 * hand-off, so 0x02000240 is the overlay's control block, not an
 * incidental pointer. Then Func_0808a260(0x6d, 16),
 * Func_0808a250(0x3e, 3), and Func_0808a020 -- one of the few owners
 * here that does close its Func_0808a018 bracket.
 *
 * Complete owner: `push {r5, lr}` at 0x02002618 through `pop {r5} /
 * pop {r0} / bx r0` at 0x020029a6-0x020029aa; the trailing pool
 * (0x0000b333, 0x020088c1, 0x0000026a, 0x00000262, 0x00000256,
 * 0x0000cccc, 0x00006666, 0x02000240, 0x0000006f, 0x0000006d) ends at
 * 0x020029d3, and the next owner prologue (`push {r5, r6, lr}`, the
 * drafted 0x020029d4) begins at 0x020029d4.
 *
 * Second read done (over 400 bytes): two corrections. The internal pool
 * above was first read as a real backward branch and briefly taken as
 * evidence that 0x020022c0 and this owner were one function. And the
 * record +108 write was first transcribed as the opaque constant
 * 0x020088c1; subtracting the 0x8000 link base identified it as
 * Func_020008c0's Thumb pointer.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (bun tools/overlay_call_targets.ts resource_3af 2618 29d4, the +2
 * rule against the raw image). The 0x02001444 driver confirms it
 * independently as dispatch case 15.
 *
 * Uncertainty: slot ids and argument roles by shape; the three
 * Data_02000240 field values, the Func_080091f0 triples and the record
 * field writes on actor 22 are transcribed, meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */
extern u8 Data_02000240[];      /* the overlay's EWRAM control block */

extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a128(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a1e0(s32 id, s32 mode);
extern void Func_0808a1f0(s32 id, s32 arg1);
extern void Func_0808a200(s32 id, s32 arg1);
extern void Func_0808a250(s32 group, s32 arg1);
extern void Func_0808a260(s32 group, s32 arg1);
extern void Func_0808a360(void);
extern void Func_0808a370(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_080091f0(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 sound_id);
extern void Func_02000bb8(void);
extern void Func_020008c0(void);
extern void Func_02003a00(s32 id, s32 arg1);

void Func_02002618(void)
{
    u8 *record;
    u8 *workspace;

    Func_0808a018();
    Func_0808a030(0x0200d418);
    Func_080000c0(1);
    Func_0808a200(25, 1);
    Func_080000c0(1);
    Func_0808a100(21, 5);
    Func_080091e0(Func_0808a080(21), 0);

    Func_0808a0f0(0, 0, 0);
    record = Func_0808a080(0);
    *(u16 *)(record + 6) = (u16)(128 << 7);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 0x202;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(20);

    Func_02000bb8();

    Func_0808a010(10);
    Func_0808a0f0(0, 216 << 16, 0x024a0000);
    Func_0808a090(0, 128 << 9, 128 << 8);
    Func_0808a0d0(0, 216, 0x256);
    Func_0808a010(20);
    Func_02003a00(0, 192 << 7);
    Func_0808a128(0, 2, 10);
    Func_0808a090(0, 0x19999, 0xcccc);
    Func_0808a0d0(0, 194, 156 << 2);
    Func_080f9010(181);

    Func_080091f0(128 << 11, 128 << 11, 128 << 9);
    Func_0808a010(20);
    Func_080091f0(-1, -1, 0xe666);
    Func_0808a010(10);
    Func_0808a1b8(0, 192 << 8, 20);

    record = Func_0808a080(25);
    record[85] = 0;
    Func_0808a090(25, 128 << 10, 128 << 9);
    Func_0808a0b8(25, 216, 153 << 2);
    Func_080f9010(149);

    Func_0808a1e0(22, 2);
    Func_0808a100(22, 5);
    record = Func_0808a080(22);
    *(s32 *)(record + 72) = 0xb333;
    *(s32 *)(record + 40) = 128 << 12;
    *(s32 *)(record + 28) = 208 << 9;
    *(s32 *)(record + 108) = (s32)&Func_020008c0 | 1;
    *(s32 *)(record + 24) = 208 << 9;
    *(s32 *)(record + 68) = 128 << 8;
    Func_0808a090(22, 192 << 11, 192 << 10);
    Func_0808a0c0(22, 182, 0x26a);
    Func_080091e0(Func_0808a080(22), 0);

    Func_02003a00(0, 160 << 8);
    Func_0808a128(0, 6, 80);
    Func_0808a0b8(25, 232, 141 << 2);
    Func_0808a0d0(0, 204, 0x262);
    Func_0808a0d0(0, 208, 0x256);
    Func_0808a0d0(0, 248, 0x256);

    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(2, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    record = Func_0808a080(0);
    if (record != 0) {
        Func_0808a0f0(3, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }

    Func_0808a090(1, 128 << 9, 128 << 8);
    Func_0808a090(2, 128 << 9, 128 << 8);
    Func_0808a090(3, 128 << 9, 128 << 8);
    Func_0808a0c8(0, 250, 146 << 2);
    Func_0808a0c8(1, 240, 150 << 2);
    Func_0808a0c8(2, 254, 150 << 2);
    Func_0808a0d0(3, 248, 154 << 2);
    Func_0808a100(0, 1);
    Func_0808a100(1, 1);
    Func_0808a100(2, 1);
    Func_0808a1b8(0, 192 << 8, 0);
    Func_0808a1b8(1, 192 << 8, 0);
    Func_0808a1b8(2, 192 << 8, 0);
    Func_0808a1b8(3, 192 << 8, 20);
    Func_080f9010(149);
    Func_0808a010(40);
    Func_0808a1f0(0, 129 << 1);
    Func_0808a1f0(1, 129 << 1);
    Func_0808a1f0(2, 129 << 1);
    Func_0808a1f0(3, 129 << 1);
    Func_0808a010(60);
    Func_0808a090(0, 0xcccc, 0x6666);
    Func_0808a090(1, 0xcccc, 0x6666);
    Func_0808a090(2, 0xcccc, 0x6666);
    Func_0808a090(3, 0xcccc, 0x6666);
    Func_0808a0c8(0, 248, 141 << 2);
    Func_0808a0c8(1, 248, 141 << 2);
    Func_0808a010(20);
    Func_0808a0c8(2, 248, 141 << 2);
    Func_0808a0c8(3, 248, 141 << 2);
    Func_0808a010(20);

    *(u16 *)(Data_02000240 + 452) = 0x6f;
    *(u16 *)(Data_02000240 + 454) = 30;
    *(u8 *)(Data_02000240 + 555) = 3;
    Func_0808a260(0x6d, 16);
    Func_0808a250(0x3e, 3);
    Func_0808a020();
}
