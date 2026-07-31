typedef unsigned char u8;
typedef signed int s32;

/*
 * Resource 3b5, owner at 0x02000ca8 (84 bytes; 80 bytes of code plus the
 * 4-byte literal pool at 0x02000cf8).
 *
 * Complete owner: `push {r5, lr} / sub sp, #8` at 0x02000ca8-0x02000cac and
 * the matching `add sp, #8 / pop {r5} / pop {r0} / bx r0` at
 * 0x02000cf0-0x02000cf6.  r0 holds the popped return address: void.
 *
 * Role: called once, from the overlay initialiser Func_02000728, when story
 * flag 0x201 is set.  It re-seats actor 9 and lays down a 16x16 map patch,
 * then marks flag 0x201 handled.
 *
 * Func_080091c0 is the six-argument renderer ABI: r0-r3 plus two stack words
 * pushed at [sp,#0] and [sp,#4], which is why this owner reserves 8 bytes of
 * stack it otherwise never touches.
 *
 * Call targets resolved with `tools/overlay_call_targets.ts`.
 */

/* Old-style declarations: overlay import arities are not fixed per name. */
u8 *Func_0808a080();
void Func_080091e0();
void Func_0808a100();
void Func_080091c0();
void Func_080770c8();

void Func_02000ca8(void)
{
    u8 *actor;

    actor = Func_0808a080(9);
    if (actor != 0) {
        Func_080091e0(actor, 0);
        actor[0x23] = 2;
        actor[0x55] = 0;
    }

    Func_0808a100(9, 5);
    Func_080091c0(36, 16, 1, 1, 34, 16);
    Func_080770c8(0x201);
}
