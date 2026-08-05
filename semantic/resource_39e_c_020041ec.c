#include "types.h"

/*
 * resource_39e owner at 0x020041ec, 336 bytes: a 64-shot randomized
 * particle rain. Opening: sound 0x83, install the thumb callback
 * pointer 0x0200c1c5 into field +108 of the slot-8 record
 * (Func_0808a080), wait 40, Func_0808a338(0x10000, 0) /
 * Func_0808a330(0x205c54, 1) / Func_0808a348(60), wait 40, sound 0x83
 * again, same callback into the slot-2 record's +108, wait 120. Then a
 * 64-iteration loop (unsigned counter, bls against 63) spawning
 * through the overlay's eight-argument spawner with a stack descriptor
 * (word 1, word 2, u16 0x011d at +24):
 *   - every fourth iteration (i & 3) == 0 plays sound 246;
 *   - x = rec8[+8]  + ((Func_080000f8() * 48) & 0xffff0000) - 0xc0000
 *   - y = rec8[+12] + ((Func_080000f8() * 32) & 0xffff0000) - 0x100000
 *   - a5 = ((Func_080000f8() * 4) >> 16 << 15) + 0x8000
 *   - Func_0200013c(x, y, rec8[+16], 0, a5, 0, 0x130000, descriptor)
 *   - Func_080000c0(2) between shots.
 * (Func_080000f8 is the no-argument random source named in earlier
 * drafts; the masks keep only the whole-unit part of the scaled roll.)
 * Close: sound 220, wait 30, Func_0808a330(0x10000, 1) /
 * Func_0808a348(60), wait 40, clear both +108 callback fields (slot 8
 * and slot 2), then Func_0808a158(8, 0) / Func_0808a158(2, 0).
 * Called 1x within this overlay.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8/r9/sl spill and
 * a 56-byte sp frame at 0x020041ec-0x020041f8 through the full unspill
 * to `pop {r0} / bx r0` at 0x02004322-0x02004324, five pool words plus
 * an alignment halfword ending 0x0200433b; the next item at 0x0200433c
 * is a `ldr r4, [pc, #0] / bx r4` call_via veneer whose pool word
 * (0x03000380) sits at 0x02004340 -- not a prologue-started owner, so
 * the span closes at the veneer.
 *
 * Not found by the structural inventory walk (unindexed): reached only
 * by `bl` (bun tools/overlay_call_targets.ts resource_39e 41ec 4324's
 * +2 rule; the spawn site is an intra-overlay bl to Func_0200013c).
 *
 * Uncertainty: callees by shape; 0x0200c1c5 is transcribed as a thumb
 * code pointer (odd address in the overlay image) but its target is
 * not identified; constants transcribed, meaning unknown.
 */

extern u8 *Func_02008606(s32 id);
extern u8 *Func_0200863a(s32 id);
extern u8 *Func_02008648(s32 id);
extern u8 *Func_020086fa(s32 id);
extern u8 *Func_02008704(s32 id);
extern void Func_020085e0(s32 frames);
extern void Func_020085fe(s32 frames);
extern void Func_02008612(s32 frames);
extern void Func_020086ae(s32 frames);
extern void Func_020086c4(s32 frames);
extern void Func_02008798(s32 id, s32 arg1);
extern void Func_020087a0(s32 id, s32 arg1);
extern void Func_0200872a(s32 arg0, s32 arg1);
extern void Func_020087f0(s32 arg0, s32 arg1);
extern void Func_02008740(s32 arg0);
extern void Func_02008806(s32 arg0);
extern void Func_0200860e(s32 arg0);
extern s32 Func_020085c6(void);
extern s32 Func_020085de(void);
extern s32 Func_020085f2(void);
extern void Func_02008778(s32 sound_id);
extern void Func_020087ac(s32 sound_id);
extern void Func_020087e2(s32 sound_id);
extern void Func_02008850(s32 sound_id);
extern void Func_020043fe(s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, u32 flags, u8 *extra);

struct Descriptor {
    u32 field0;
    u32 field4;
    u8 unused8[16];
    u16 field24;
    u8 unused26[6];
};

void Func_020041ec(void)
{
    struct Descriptor descriptor;
    u8 *record;
    u32 i;
    s32 x;
    s32 y;
    s32 scale;

    Func_02008778(0x83);
    record = Func_02008606(8);
    *(u32 *)(record + 108) = 0x0200c1c5;
    Func_020085e0(40);
    Func_0200872a(128 << 9, 0);
    Func_0200872a(0x205c54, 1);
    Func_02008740(60);
    Func_020085fe(40);
    Func_020087ac(0x83);
    record = Func_0200863a(2);
    *(u32 *)(record + 108) = 0x0200c1c5;
    Func_02008612(120);
    record = Func_02008648(8);
    descriptor.field0 = 1;
    descriptor.field4 = 2;
    descriptor.field24 = 0x011d;
    for (i = 0; i <= 63; i++) {
        if ((i & 3) == 0) {
            Func_020087e2(246);
        }
        x = *(s32 *)(record + 8)
            + (((u32)(Func_020085c6() * 3) << 4) & 0xffff0000) + 0xfff40000;
        y = *(s32 *)(record + 12)
            + (((u32)Func_020085de() << 5) & 0xffff0000) + 0xfff00000;
        scale = (((u32)((u32)Func_020085f2() << 2) >> 16) << 15) + (128 << 8);
        Func_020043fe(x, y, *(s32 *)(record + 16), 0,
                      scale, 0, 152 << 13, (u8 *)&descriptor);
        Func_0200860e(2);
    }
    Func_02008850(220);
    Func_020086ae(30);
    Func_020087f0(128 << 9, 1);
    Func_02008806(60);
    Func_020086c4(40);
    record = Func_020086fa(8);
    *(u32 *)(record + 108) = 0;
    record = Func_02008704(2);
    *(u32 *)(record + 108) = 0;
    Func_02008798(8, 0);
    Func_020087a0(2, 0);
}
