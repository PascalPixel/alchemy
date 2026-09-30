#include "TYPES.H"

extern s16 EventTable_AbilityLoadouts[][33];
s32 GameFlag_TestFar(s32);
s32 GameFlag_SetBitFar(s32);
void Ability_GetMaximum(s32, s32);

s32 EventTable_GetRowType(s32 index)
{
    return EventTable_AbilityLoadouts[index][32];
}
