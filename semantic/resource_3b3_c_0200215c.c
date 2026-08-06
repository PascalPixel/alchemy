#include "types.h"

extern u8 *Data_03001f30;




/*
 * Complete 80-byte actor-coincidence latch through the two-word pool before
 * 0x020021ac.  Actor 0 and actor 13 are compared on their integer X/Z cells.
 */
extern u8 * Func_02004c7e(s32 actorId);
extern u8 * Func_02004c86(s32 actorId);
extern void Func_02004c80(s32 flagId);
extern void Func_02004c98(s32 flagId);
void Func_0200215c(void)
{
    u8 *actor0 = Func_02004c7e(0);
    u8 *actor13 = Func_02004c86(13);

    if ((*(s32 *)(actor13 + 8) >> 20) == (*(s32 *)(actor0 + 8) >> 20) &&
        (*(s32 *)(actor13 + 16) >> 20) == (*(s32 *)(actor0 + 16) >> 20)) {
        Data_03001f30[53] = 1;
        Func_02004c80(0x203);
    } else {
        Func_02004c98(0x203);
    }
}
