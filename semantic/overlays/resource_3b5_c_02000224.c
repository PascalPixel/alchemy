typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b5, owner at 0x02000224 (60 bytes, all code; no literal pool).
 *
 * Complete owner: `push {lr}` at 0x02000224 and `pop {r0} / bx r0` at
 * 0x0200025c.  r0 holds the popped return address, so this owner is void.
 *
 * Role: called once, from the overlay initialiser Func_02000728, when story
 * flag 0x200 is set.  It re-seats actor 8 (clears its byte at +0x59 and its
 * animation), places the camera and marks flag 0x200 handled.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`; the inventory's
 * `calls=4` is a lower bound, the five sites below are the real count.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
u8 *Func_0808a080();
void Func_080091e0();
void Func_08009278();
void Func_080770c8();

void Func_02000224(void)
{
    u8 *actor;

    actor = Func_0808a080(8);
    if (actor != 0) {
        actor[0x59] = 0;
    }

    Func_080091e0(Func_0808a080(8), 0);
    Func_08009278(0, 0x2200000, 0x1200000, 0xfd);
    Func_080770c8(0x200);
}
