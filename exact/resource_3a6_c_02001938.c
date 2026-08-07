/*
 * BYTE-EXACT and adopted 2026-08-07 with -fthumb-orr-dead-input-reuse: the sole
 * residual was the direction of the `orrs' that sets bit 1 of the flag byte at
 * actor+35.  The reference reuses the dying constant register as the
 * destination (`orrs r2, r3'), we defaulted to the loaded byte (`orrs r3, r2'),
 * which also dragged the following `movs r2, #128' one slot earlier.
 */

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
    {
        u8 *field = actor + 34;
        u32 cur;

        *field = 2;
        field = field + 1;
        cur = *field;
        cur |= 2;
        *field = (u8)cur;
    }
    *(s32 *)(actor + 8) = (cellX << 20) + 0x80000;
    *(s32 *)(actor + 16) = (cellZ << 20) + 0x80000;
}
