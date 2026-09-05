#include "types.h"

/*
 * resource_39a owner at 0x02000ed8, 88 bytes.
 *
 * Creates an actor and, when creation succeeded, initialises it: mode field of
 * the linked record to 1, state byte at +85 cleared, two setup calls, and bit 1
 * of the flag byte at +35 raised. Returns the actor, or null.
 *
 * The mode write is a bitfield assignment; as explicit mask arithmetic the
 * compiler narrows -13 to a byte and the `movs r3,#13 / negs r3,r3` pair is
 * lost.
 */

struct Mode_39a {
    u8 pad00[9];
    u8 lo : 2;
    u8 mode : 2;                /* +9, bits 2..3 */
    u8 hi : 4;
};

struct Actor_39a {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Mode_39a *f80;       /* +80 */
    u8 pad51[1];
    u8 f85;                     /* +85 */
};

struct Actor_39a *Func_020031a2();
void Func_02003206();
void Func_020032a6();

struct Actor_39a *OverlayObject_CreateAndInitialize(s32 a, s32 b, s32 c, s32 d)
{
    struct Actor_39a *actor = Func_020031a2(d, a, b, c);

    if (actor != 0) {
        actor->f80->mode = 1;
        actor->f85 = 0;
        Func_02003206(actor, 0);
        Func_020032a6(actor, 15);
        actor->f35 |= 2;
        return actor;
    }
    return 0;
}
