typedef signed int s32;
typedef unsigned char u8;

/*
 * Resource 3c8 at 0x0200300c: reset an actor's mode/state bytes and rearm it.
 *
 * The owner is complete: `push {r5, r6, lr}` at 0x0200300c and the matching
 * `pop {r5, r6} ; pop {r0} ; bx r0` at 0x0200304a..0x0200304e.  No literal
 * pool sits inside the span and no live frame or register state escapes the
 * row.
 *
 * `pop {r0} ; bx r0` means r0 holds the popped return address, so the owner
 * returns nothing.
 *
 * The incoming r0 is copied to r6 but is NOT reloaded before the first `bl`,
 * so the lookup at 0x02007ebe receives the same actor index; r6 is only kept
 * so it can be passed again at 0x02007fa2.  Likewise the actor pointer
 * returned in r0 is still live at 0x02007e72 - only r1 is set there - so that
 * call takes the actor plus 0.
 *
 * Field offsets 0x23, 0x55 and 0x59 are the ones already proven for this
 * package's actor record (see semantic/overlays/resource_3c8_c_02001d48.c,
 * which writes `mode55`, `state59` and `flags23` on the same structure).
 *
 * All four `bl` sites are accounted for: 0x02007ebe, 0x02007e72, 0x02007e0a,
 * 0x02007fa2.  Import naming follows the note in
 * resource_3c8_c_020002f0.c - each name is the address its call site
 * computes.
 */

struct Actor_0200300c {
    u8 unknown_00[0x23];
    u8 flags23;
    u8 unknown_24[0x31];
    u8 mode55;          /* offset 0x55 */
    u8 unknown_56[3];
    u8 state59;         /* offset 0x59 */
};

/* Used for its return value. */
struct Actor_0200300c *Func_02007ebe();

/* Old-style declarations: the imports' real interfaces are not known here. */
void Func_02007e72();
void Func_02007e0a();
void Func_02007fa2();

void Func_0200300c(s32 actorIndex)
{
    struct Actor_0200300c *actor;

    actor = Func_02007ebe(actorIndex);
    actor->mode55 = 0;
    actor->state59 &= 0xfc;
    Func_02007e72(actor, 0);
    Func_02007e0a(actor, 5);
    Func_02007fa2(actorIndex, 3);
    actor->flags23 |= 2;
}
