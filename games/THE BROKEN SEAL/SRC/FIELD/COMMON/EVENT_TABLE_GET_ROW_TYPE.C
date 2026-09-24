#include "TYPES.H"

extern s16 EventTable_AbilityLoadouts[][33];

s32 EventTable_GetRowType(s32 index)
{
    return EventTable_AbilityLoadouts[index][32];
}
