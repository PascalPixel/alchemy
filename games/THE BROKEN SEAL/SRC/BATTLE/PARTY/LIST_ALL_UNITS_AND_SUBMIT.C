#include "TYPES.H"

s32 Func_080b6c08(s32, void *);
#define BattleParty_ListActorIds Func_080b6c08
s32 Func_080b7b6c(void *, s32);

void BattleParty_ListAllUnitsAndSubmit(void)
{
    u8 local[28];
    BattleParty_ListActorIds(3, local);
    Func_080b7b6c(local, 0);
}
