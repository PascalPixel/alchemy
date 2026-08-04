#include "types.h"

extern u8 *Data_03001f30;
extern u8 *Func_0808a080(s32 actorId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);

/*
 * Complete 80-byte actor-coincidence latch through the two-word pool before
 * 0x020021ac.  Actor 0 and actor 13 are compared on their integer X/Z cells.
 */
void Func_0200215c(void)
{
    u8 *actor0 = Func_0808a080(0);
    u8 *actor13 = Func_0808a080(13);

    if ((*(s32 *)(actor13 + 8) >> 20) == (*(s32 *)(actor0 + 8) >> 20) &&
        (*(s32 *)(actor13 + 16) >> 20) == (*(s32 *)(actor0 + 16) >> 20)) {
        Func_080770c8(0x203);
        Data_03001f30[53] = 1;
    } else {
        Func_080770d0(0x203);
    }
}
