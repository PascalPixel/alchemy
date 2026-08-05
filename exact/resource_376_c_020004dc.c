#include "types.h"

/*
 * resource_376 owner at 0x020004dc, 128 bytes: choose the scene that follows
 * from actor zero's directional halfword and two story flags.
 *
 * Complete owner: `push {lr}` at 0x020004dc through the interworking return
 * at 0x0200053c-0x0200053f, followed by the seven referenced pool words at
 * 0x02000540-0x0200055b.  The next independent prologue starts at
 * 0x0200055c.
 *
 * The opening test is the same unsigned wrapped-range idiom established in
 * resource_39e at 0x02002508.  The u16 at actor+6 is increased by
 * 0xffff5fff (that is, reduced by 0xa001 modulo 2^32), then compared unsigned
 * with 0x3ffe.  Its fall-through therefore covers the closed original range
 * 0xa001..0xdfff.  Values below 0xa001 wrap high and take the other arm.
 *
 * In range, control is handed directly to Func_080b0008(1, 21).  Out of
 * range, a scripted sequence opens.  Flag 0x87a selects dialogue 0x1c06 and
 * the Func_0808a190 terminator.  Otherwise flag 0x815 selects dialogue
 * 0x11a2 instead of 0x0f53, and both choices use Func_0808a180(21, 0).
 * The scripted sequence then closes on both out-of-range paths.
 *
 * All eleven static call sites resolve independently under the overlay +2
 * branch rule.  The actor record is fetched before either path is chosen;
 * only its +6 halfword is read.  Flag identifiers and dialogue identifiers
 * are literal, while the higher-level meaning of the directional range is
 * not yet established.
 */

extern u8 *Func_020016a6(s32 actor_id);
extern void Func_020017e2(s32 arg0, s32 actor_id);
extern void Func_02001698(void);
extern s32 Func_020016a0(s32 flag_id);
extern void Func_02001772(s32 dialogue_id);
extern void Func_02001778(s32 actor_id, s32 mode);
extern void Func_0200178a(s32 actor_id, s32 mode);
extern void Func_020016de(void);

void Func_020004dc(void)
{
    u8 *actor;
    u32 biased_direction;

    actor = Func_020016a6(0);
    biased_direction = *(u16 *)(actor + 6);
    biased_direction += 0xffff5fff;

    if (biased_direction <= 0x3ffe) {
        Func_020017e2(1, 21);
        return;
    }

    Func_02001698();
    if (Func_02001686(0x87a) != 0) {
        Func_02001750(0x1c06);
        Func_02001778(21, 0);
    } else {
        if (Func_020016a0(0x815) != 0) {
            Func_0200176a(0x11a2);
        } else {
            Func_02001772(0x0f53);
        }
        Func_0200178a(21, 0);
    }
    Func_020016de();
}
