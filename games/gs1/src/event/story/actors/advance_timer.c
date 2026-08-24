#include "types.h"

extern void Func_02004348();

void StoryActor_AdvanceTimer(u8 *actor)
{
    u16 *timer = (u16 *)(actor + 0x64);

    /* Arm order decides the branch sense: the reference falls through to the
     * increment and branches away to the call, which is `bgt`. */
    if (*(s16 *)timer <= 0) {
        *timer = (u16)(*timer + 1);
    } else {
        Func_02004348(actor);
    }
}
