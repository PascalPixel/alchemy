#include "TYPES.H"

s32 BattleParty_ListActorIds(s32, s16 *);
void BattlePres_SetActorRecordMode(s32, s32);

void BattlePres_SetOtherActorRecordModes(s32 excluded)
{
    s16 values[14];
    s32 count = BattleParty_ListActorIds(3, values);
    s32 index;

    for (index = 0; index < count; index++) {
        if (values[index] != excluded)
            BattlePres_SetActorRecordMode(values[index], 1);
    }
}
