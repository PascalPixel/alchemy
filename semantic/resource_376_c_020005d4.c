#include "types.h"

/*
 * resource_376 owner at 0x020005d4, 132 bytes: the actor-20 directional
 * selector, with a pre-existing local scene as its flag-0x815 branch.
 *
 * Complete owner: `push {lr}` at 0x020005d4 through the interworking return
 * at 0x0200063a-0x0200063d, one alignment halfword, and six referenced pool
 * words through 0x02000657.  The next independent prologue is 0x02000658.
 *
 * As in the preceding two owners, actor zero's +6 u16 is biased by
 * 0xffff5fff and compared unsigned with 0x3ffe.  The closed original range
 * 0xa001..0xdfff hands off through Func_080b0008(3, 20).  Outside it, flag
 * 0x87a selects a short dialogue-0x1c0a scene.  If that flag is clear,
 * flag 0x815 selects the already reconstructed Func_020001e8 scene; when
 * both flags are clear a dialogue-0x0f55 scene runs instead.
 *
 * All thirteen static call sites resolve independently under the overlay +2
 * branch rule, including the direct in-image call to Func_020001e8.
 */

extern u8 *Func_0808a080(s32 actor_id);
extern void Func_080b0008(s32 arg0, s32 actor_id);
extern s32 Func_080770c0(s32 flag_id);
extern void Func_0808a018(void);
extern void Func_0808a170(s32 dialogue_id);
extern void Func_0808a180(s32 actor_id, s32 mode);
extern void Func_0808a020(void);
extern void Func_020001e8(void);

void Func_020005d4(void)
{
    u8 *actor;
    u32 biased_direction;

    actor = Func_0808a080(0);
    biased_direction = *(u16 *)(actor + 6);
    biased_direction += 0xffff5fff;

    if (biased_direction <= 0x3ffe) {
        Func_080b0008(3, 20);
        return;
    }

    if (Func_080770c0(0x87a) != 0) {
        Func_0808a018();
        Func_0808a170(0x1c0a);
        Func_0808a180(20, 0);
        Func_0808a020();
    } else if (Func_080770c0(0x815) != 0) {
        Func_020001e8();
    } else {
        Func_0808a018();
        Func_0808a170(0x0f55);
        Func_0808a180(20, 0);
        Func_0808a020();
    }
}
