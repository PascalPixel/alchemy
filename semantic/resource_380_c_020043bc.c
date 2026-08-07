#include "types.h"

/*
 * resource_380 owner at 0x020043bc, 224 bytes: a 32-tick spawn burst.
 * Builds a 16-byte parameter block on the stack (1, 5, descriptor
 * 0x6666, weight 0x30000, id halfword 284 at +24), then ticks 32
 * times: every even tick it derives a spawn position from two resident
 * counters (x = base_x + (Func_080000f8()*24 masked to its high half)
 * - 0xc0000, y = base_y + (Func_080000f8()<<5 masked likewise)
 * + 0x200000) and fires the in-overlay spawner Func_0200013c with
 * fixed velocity constants (-0x40000, 0x1b0000); on tick 20 it nudges
 * channel 14 by 256.  Afterwards it settles channel 14 and hands off.
 *
 * Complete owner: `push {r5,r6,r7,lr}` + r8/sl spill at 0x020043bc
 * through `pop {r0} / bx r0` at 0x0200448e, then the literal pool
 * 0x02004490-0x0200449b (0x6666, 0xfff40000, 0xfffc0000); next owner's
 * prologue at 0x0200449c (the drafted resource_380_c_0200449c.c).
 *
 * CORRECTION (name sweep): this file's callee names came from a naive
 * pc-relative decode and were wrong; they are resolved here through the
 * overlay's import-veneer table under the +2 rule
 * (tools/lib/overlay_call_targets.ts) to their main-ROM identities.  Two
 * phantom names collapsed onto Func_080091e0 and two onto
 * Func_080000f8 -- the "two different functions" reading was the
 * decode, not the code.  Both Func_080091e0 sites do genuinely take the
 * preceding Func_0808a080 result in r0 (checked at 0x020043d4-0x020043dc:
 * `movs r0,#14 / bl / movs r1,#0 / bl`), so the nesting is kept.
 *
 * Uncertainty: callee roles beyond call shape remain open; the context
 * object from Func_0808a080(14) supplies base coordinates at +8/+12 and
 * a third value at +16 passed straight through.
 */

extern s32 Func_080000f8(void);
extern s32 Func_080091e0(s32 arg0, s32 arg1);
extern void Func_0808a010(s32 arg0);
extern s32 Func_0808a080(s32 channel);
extern void Func_0808a158(s32 channel, s32 amount);
extern void Func_080f9010(s32 arg0);
extern void Func_0200013c(
    s32 x, s32 y, s32 arg2, s32 arg3,
    s32 velocity_x, s32 flag, s32 velocity_y, void *parameters);

void Func_020043bc(void)
{
    s32 parameters[7];
    u32 tick;
    s32 context = Func_0808a080(14);

    Func_080f9010(190);
    Func_080091e0(Func_0808a080(14), 0);

    parameters[0] = 1;
    parameters[1] = 5;
    parameters[2] = 0x6666;
    parameters[3] = 0x30000;
    *(u16 *)((u8 *)parameters + 24) = 284;

    for (tick = 0; tick <= 31; tick++) {
        Func_0808a010(1);
        if ((tick & 1) == 0) {
            s32 x = *(s32 *)(context + 8)
                + (u32)((Func_080000f8() * 24) & 0xffff0000)
                + 0xfff40000;
            s32 y = *(s32 *)(context + 12)
                + (u32)((Func_080000f8() << 5) & 0xffff0000)
                + 0x200000;

            Func_0200013c(x, y, *(s32 *)(context + 16), 0,
                0xfffc0000, 0, 0x1b0000, parameters);
        }
        if (tick == 20)
            Func_0808a158(14, 256);
    }
    Func_0808a158(14, 0);
    Func_080091e0(Func_0808a080(14), 1);
}
