#include "TYPES.H"

s32 Func_080b6c08(s32, s16 *);
#define BattleParty_ListActorIds Func_080b6c08
void Func_080c0f98(s32, s32);

void BattlePres_SetOtherActorRecordModes(s32 excluded)
{
    s16 values[14];
    s32 count = BattleParty_ListActorIds(3, values);
    s32 index;

    for (index = 0; index < count; index++) {
        if (values[index] != excluded)
            Func_080c0f98(values[index], 1);
    }
}
