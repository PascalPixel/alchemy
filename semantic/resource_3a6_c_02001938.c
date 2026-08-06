#include "types.h"




/* Enable an actor and place it at the centre of the requested grid cell. */
extern u8 * Func_02003602(s32 actorId);
extern void Func_02003670(s32 actorId, s32 mode);
void Func_02001938(s32 actorId, s32 cellX, s32 cellZ)
{
    u8 *actor = Func_02003602(actorId);

    if (actor == 0)
        return;
    Func_02003670(actorId, 3);
    actor[34] = 2;
    actor[35] |= 2;
    *(s32 *)(actor + 8) = (cellX << 20) + 0x80000;
    *(s32 *)(actor + 16) = (cellZ << 20) + 0x80000;
}
