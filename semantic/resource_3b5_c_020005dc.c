#include "types.h"

/*
 * Resource 3b5, cutscene script step at 0x020005dc (104 bytes; 92 bytes of
 * code plus the 12-byte literal pool at 0x02000638-0x02000643).
 *
 * Complete owner: `push {lr}` at 0x020005dc and `pop {r0} / bx r0` at
 * 0x02000634.  r0 holds the popped return address: void.
 *
 * Role: named in advance by the cutscene script table, which holds
 * 0x020085dd in a record's handler word — Func_020005dc + the Thumb bit under
 * this overlay's proven 0x02008000 link base.  A no-argument script step.
 *
 * It is a one-shot: story flag 0x8bf gates the long arm and is set by that
 * arm's own Func_080770c8 call, so a replay takes the short arm and only
 * re-shows the alternate line 0x236a.
 *
 * Call targets resolved with `tools/lib/overlay_call_targets.ts`; twelve sites,
 * against the inventory's `calls=11`, which is a documented lower bound.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
void Func_0808a018();
s32 Func_080770c0();
void Func_080770c8();
void Func_0808a170();
void Func_0808a180();
void Func_0808a398();
void Func_0808a100();
void Func_0808a060();
void Func_0808a020();

void Func_020005dc(void)
{
    Func_0808a018();

    if (Func_080770c0(0x8bf) == 0) {
        Func_080770c8(0x8bf);

        Func_0808a170(0x2368);
        Func_0808a180(19, 0);

        Func_0808a398(0xe9, 3);
        Func_0808a180(19, 0);

        Func_0808a100(0, 1);
        Func_0808a060(0xe9, 0);
    } else {
        Func_0808a170(0x236a);
        Func_0808a180(19, 0);
    }

    Func_0808a020();
}
