#include "types.h"

s32 BattleParty_ListActorIds();
void ObjMotRst();

void Func_0811d9a4(void)
{
    u16 ids[14];
    s32 count = BattleParty_ListActorIds(3, ids);
    s32 i;

    for (i = 0; i < count; i++)
        ObjMotRst(ids[i]);
}
