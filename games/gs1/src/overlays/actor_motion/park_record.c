#include "types.h"

/*
 * Park a record: stamp the sentinel 0x80000000 into the three mirror fields at
 * +56, +60 and +64, zero the three at +36, +40 and +44, and clear the u16
 * angle at +100.  The 24-byte owner at 0x02005688 has no prologue, no stack
 * use and no literal pool.  0x80000000 is read as a sentinel because it is
 * stamped into three position-family fields at once and then tested for
 * equality; the roles of +36, +40 and +44 are not established.
 */

void SceneActor_ParkRecord(u8 *record)
{
    /* The sentinel is built as 128 shifted left by 24, not pooled. */
    *(s32 *)(record + 56) = (s32)0x80000000;
    *(s32 *)(record + 60) = (s32)0x80000000;
    *(s32 *)(record + 64) = (s32)0x80000000;

    *(s32 *)(record + 36) = 0;
    *(s32 *)(record + 40) = 0;
    *(s32 *)(record + 44) = 0;

    *(u16 *)(record + 100) = 0;
}
