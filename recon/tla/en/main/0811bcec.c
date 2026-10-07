#include "TYPES.H"
#include "BTLWORK.H"
#include "FIXED_POINT_POSITION.H"

s32 Battle_GetWorkEntryPair(s32 no, struct FixedPointPosition *out)
{
    struct BtlWork *state = Ram_HeapSlots->battle_work;

    out->x = state->slots[no].anchor_x;
    out->y = 0;
    out->z = state->slots[no].anchor_z;
    return 0;
}
