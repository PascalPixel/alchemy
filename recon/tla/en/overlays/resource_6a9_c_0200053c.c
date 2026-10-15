/* Draft, differ 107 of 161, written from the listing: the branches, calls and flags are the
 * native's. Left: the work pointer and the offset take r5 and r6 where the native has r6 and r5;
 * each height is copied out of r0 before its shift (the native shifts in r0, then adds the offset
 * into r3); the second branch shares 134 << 18 in r5. Tried: the helper as a macro over two
 * inlines, the shift as its own statement, permuter 3 minutes (83, by changing behaviour). */
#include "../../../../games/tla/src/scene/caverns/caverns.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020010cc(s32 x, s32 z, s32 layer, s32 height);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

static __inline__ void SetCellHeight(s32 x, s32 z, s32 offset)
{
    s32 height = (Map_GetTerrainHeight(2, x, z) >> 19) + offset;

    Func_020010cc(x, z, 2, height);
}

static __inline__ s32 Flag_Test(s32 flag)
{
    return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag)
{
    GameFlag_SetBit(flag);
}

static __inline__ void Flag_Clear(s32 flag)
{
    GameFlag_ClearBit(flag);
}

void Func_0200053c(s32 level)
{
    u8 *work = *(u8 **)((u8 *)Ram_HeapSlots->eventWork + 0xcbc);
    s32 offset = level << 3;

    if (HALF_AT(work, 14) > 31 || Flag_Test(0x213)) {
        SetCellHeight(142 << 18, 134 << 18, offset);
        SetCellHeight(146 << 18, 138 << 18, offset);
        if (offset == 8)
            Flag_Set(0x213);
        else
            Flag_Clear(0x213);
    } else if (HALF_AT(work, 18) == 536 || Flag_Test(0x213)) {
        SetCellHeight(134 << 18, 138 << 18, offset);
        SetCellHeight(138 << 18, 138 << 18, offset);
        SetCellHeight(142 << 18, 138 << 18, offset);
        if (offset == 6)
            Flag_Set(0x213);
        else
            Flag_Clear(0x213);
    } else {
        SetCellHeight(134 << 18, 134 << 18, offset);
        SetCellHeight(138 << 18, 134 << 18, offset);
        SetCellHeight(142 << 18, 134 << 18, offset);
    }
}
