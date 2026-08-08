#include "types.h"

/*
 * resource_3af owner at 0x020022c0, 856 bytes: the overlay's largest
 * staged ensemble scene -- eight actors placed and started together,
 * then a 16-frame animated drift, then a long teardown. It is case 14
 * of the 0x02001444 entry driver.
 *
 * TRAP-3 WARNING, twice over. This owner carries THREE literal pools,
 * and two of them are INSIDE the body with code branching over them:
 * 0x020023b6-0x020023d7 (hopped by the `b.n 0x020023d8` at
 * 0x020023b4) and 0x0200252a-0x02002557 (hopped by the `b.n
 * 0x02002558` at 0x02002528). Only the third, 0x02002608-0x02002617,
 * is trailing. A span read that stops at the first pool loses two
 * thirds of the function.
 *
 * Opening: Func_0808a018, Func_0808a158(0, 15), the id-record
 * pass-through Func_080091e0(Func_0808a080(0), 0), then TWO data blocks
 * each with its own frame advance -- 0x0200d160 (the overlay's shared
 * block) and 0x0200d340 (this owner's own). Same two-block shape as
 * 0x02001db0, different second block.
 *
 * Staging: actors 22 and 21 are placed and given u16 +6 values 0xd000
 * and 0xb000; actors 24 through 29 are placed; actors 24-27 get a byte
 * written at record +99 (0, 1, 0, 2 respectively -- the first and third
 * come from a pool word that is literally zero, not from a computed
 * register); actor 20 is parked at the origin. The six ensemble actors
 * then get data pointers in three pairs -- 24 and 25 share 0x0200c8c4,
 * 26 and 27 share 0x0200c8b0, 28 and 29 share 0x0200c8d8 -- and all six
 * are started with Func_0808a158(id, 3).
 *
 * The transition-mode write is the additive arm again (displacement 448
 * from `movs r2,#224 / lsls r2,#1`, value 514 from `adds r2,#66`),
 * followed by the full Func_0808a360/Func_0808a370 pair and an 80-frame
 * wait.
 *
 * The drift loop is the one real loop in the body:
 *
 *     counter = 0;
 *     do {
 *         *(s32 *)(record + 24) += 0xf5c;
 *         *(s32 *)(record + 28) += 0xf5c;
 *         counter++;
 *         Func_080000c0(1);
 *     } while (counter <= 15);
 *
 * Sixteen iterations on actor 31's record, advancing one frame each
 * pass -- an animated slide, not a setup loop. The bound is `bls`, an
 * UNSIGNED compare, so the counter is a u32; the increment sits between
 * the two field updates and the frame advance exactly as spelled.
 *
 * Actor 30 is then configured field by field rather than through
 * helpers (record +24/+28 = 0x11999, +8 = 0xc20000, +12 = 0x500000,
 * +16 = 0x02820000, u16 +6 = 0x5000, +68 = 0x6666, +72 = 0x20000),
 * gets another 80-frame wait and the same sound 147, and actor 31 is
 * parked at the origin.
 *
 * Then the byte at slot 0's record +85 is cleared -- again from a pool
 * word holding zero -- and actors 30, 28 and 29 are dressed and handed
 * off with Func_0808a0a8/Func_0808a0b0.
 *
 * Func_02000bb8, this overlay's once-only flag-0x271 reward grant, is
 * called at 0x0200257c. That is the FOURTH caller (after 0x02001c14,
 * 0x020029d4 and 0x02001db0) and the pattern is now firm: every caller
 * is a scripted beat, and the grant always sits at the beat's climax
 * rather than in any dispatcher.
 *
 * Teardown: all six ensemble actors released with Func_0808a0a8, a
 * 10-frame wait, then the window-engine bracket first named for
 * resource_3c9's 0x4bec -- Func_08015210(0x1e45, 1, 0) between
 * Func_08009208 and Func_08009210, the SAME dialogue/window id
 * 0x02001db0 uses -- and Func_0808a248(14). There is no Func_0808a020
 * to close the opening Func_0808a018.
 *
 * Complete owner: `push {r5, r6, r7, lr}` at 0x020022c0 through
 * `pop {r5, r6, r7} / pop {r0} / bx r0` at 0x02002602-0x02002606; the
 * trailing pool (0x0000cccc, 0x00019999, 0x0200c888, 0x00001e45) ends
 * at 0x02002617, and the next owner prologue (`push {r5, lr}`, the
 * queue's 0x02002618 row) begins at 0x02002618.
 *
 * Second read done (over 400 bytes): two corrections surfaced. The
 * first pass ended the function at 0x020023b6 on the first pool and
 * called it a 246-byte owner -- wrong, that pool is internal and the
 * `b.n` before it hops to 0x020023d8. The second was the drift loop's
 * trip count: read as 15 iterations from `cmp r5,#15` alone, it is 16,
 * because the increment precedes the compare.
 *
 * Not found by the structural inventory walk: reached only by `bl`
 * (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml -- resource_3af 22c0 2618, the +2
 * rule against the raw image). The 0x02001444 driver confirms it
 * independently as dispatch case 14.
 *
 * Uncertainty: actor ids and argument roles by shape; coordinates,
 * fixed-point pairs, data pointers and the raw record-field writes on
 * actors 30 and 31 are transcribed, meanings unknown.
 */

extern u8 *Data_03001ebc;       /* overlay workspace pointer, IWRAM cell */

extern void Func_0808a018(void);
extern void Func_0808a030(s32 dataBlock);
extern void Func_080000c0(s32 arg0);
extern void Func_0808a010(s32 frames);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a098(s32 id, s32 arg1);
extern void Func_0808a0a8(s32 id);
extern void Func_0808a0b0(s32 id, s32 arg1);
extern void Func_0808a0b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0c0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0f0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a158(s32 id, s32 arg1);
extern void Func_0808a1d8(s32 id);
extern void Func_0808a248(s32 arg0);
extern void Func_0808a360(void);
extern void Func_0808a368(void);
extern void Func_0808a370(void);
extern void Func_080091e0(void *record, s32 arg1);
extern void Func_08009208(void);
extern void Func_08009210(void);
extern void Func_08015210(s32 arg0, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 sound_id);
extern void Func_02000bb8(void);
extern void Func_02003a00(s32 id, s32 arg1);

void Func_020022c0(void)
{
    u8 *record;
    u8 *workspace;
    u32 counter;

    Func_0808a018();
    Func_0808a158(0, 15);
    Func_080091e0(Func_0808a080(0), 0);
    Func_0808a030(0x0200d160);
    Func_080000c0(1);
    Func_0808a030(0x0200d340);
    Func_080000c0(1);

    Func_0808a0f0(22, 176 << 16, 174 << 18);
    record = Func_0808a080(22);
    *(u16 *)(record + 6) = (u16)(208 << 8);
    Func_0808a0f0(21, 132 << 17, 0x02960000);
    record = Func_0808a080(21);
    Func_0808a0f0(24, 184 << 16, 168 << 18);
    Func_0808a0f0(25, 202 << 16, 173 << 18);
    *(u16 *)(record + 6) = (u16)(176 << 8);
    Func_0808a0f0(26, 252 << 16, 0x02860000);
    Func_0808a0f0(27, 128 << 17, 0x02ae0000);
    Func_0808a0f0(28, 172 << 16, 158 << 18);
    Func_0808a0f0(29, 128 << 17, 0x026e0000);

    record = Func_0808a080(24);
    record = Func_0808a080(25);
    record[99] = 1;
    record = Func_0808a080(26);
    record[99] = 0;
    record[99] = 0;
    record = Func_0808a080(27);
    Func_0808a0f0(20, 0, 0);
    record[99] = 2;

    Func_0808a098(24, 0x0200c8c4);
    Func_0808a098(25, 0x0200c8c4);
    Func_0808a098(26, 0x0200c8b0);
    Func_0808a098(27, 0x0200c8b0);
    Func_0808a098(28, 0x0200c8d8);
    Func_0808a098(29, 0x0200c8d8);
    Func_0808a158(24, 3);
    Func_0808a158(25, 3);
    Func_0808a158(26, 3);
    Func_0808a158(27, 3);
    Func_0808a158(28, 3);
    Func_0808a158(29, 3);

    workspace = Data_03001ebc;
    *(s32 *)(workspace + 448) = 448 + 66;
    Func_0808a360();
    Func_0808a370();
    Func_0808a010(80);
    Func_080f9010(147);

    record = Func_0808a080(31);
    *(s32 *)(record + 24) = 0x1999;
    *(s32 *)(record + 28) = 0x1999;
    *(s32 *)(record + 8) = 194 << 16;
    *(s32 *)(record + 16) = 0x02820000;
    counter = 0;
    do {
        *(s32 *)(record + 24) += 0xf5c;
        *(s32 *)(record + 28) += 0xf5c;
        counter++;
        Func_080000c0(1);
    } while (counter <= 15);

    record = Func_0808a080(30);
    *(s32 *)(record + 24) = 0x11999;
    *(s32 *)(record + 28) = 0x11999;
    *(s32 *)(record + 8) = 194 << 16;
    *(s32 *)(record + 12) = 160 << 15;
    *(s32 *)(record + 16) = 0x02820000;
    *(u16 *)(record + 6) = (u16)(160 << 7);
    *(s32 *)(record + 68) = 0x6666;
    *(s32 *)(record + 72) = 128 << 10;

    Func_0808a010(80);
    Func_080f9010(147);
    Func_0808a0f0(31, 0, 0);
    Func_080091e0(Func_0808a080(30), 1);

    Func_0808a090(0, 0x19999, 0xcccc);
    record = Func_0808a080(0);
    record[85] = 0;
    Func_0808a0b8(0, 216, 153 << 2);
    Func_0808a090(30, 0x19999, 0xcccc);
    Func_0808a0c0(30, 196, 150 << 2);
    Func_0808a0c0(30, 216, 150 << 2);
    Func_0808a0a8(28);

    Func_080000c0(1);
    Func_0808a090(28, 0x19999, 0xcccc);
    Func_0808a098(28, 0x0200c888);
    Func_02003a00(30, 208 << 8);

    Func_02000bb8();

    Func_0808a010(10);
    Func_0808a098(30, 0x0200c888);
    Func_0808a0a8(29);
    Func_080000c0(1);
    Func_0808a090(29, 0x19999, 0xcccc);
    Func_0808a0b0(29, 0x0200c888);
    Func_0808a010(20);
    Func_0808a368();
    Func_0808a370();

    Func_0808a0a8(24);
    Func_0808a0a8(25);
    Func_0808a0a8(26);
    Func_0808a0a8(27);
    Func_0808a0a8(28);
    Func_0808a0a8(29);
    Func_0808a010(10);

    Func_08009208();
    Func_0808a1d8(21);
    Func_08015210(0x1e45, 1, 0);
    Func_08009210();
    Func_0808a248(14);
}
