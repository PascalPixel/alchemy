#include "TYPES.H"

s32 BattleParty_ListActorIds(s32, u16 *);

u32 BattleParty_IsUnitListed(u32 arg0)
{
    u16 values[8];
    s32 count;
    s32 total;
    s32 i;

    count = 1;
    if (arg0 > 7) {
        count = 2;
    }
    total = BattleParty_ListActorIds(count, values);

    for (i = 0; i < total; i++) {
        if (values[i] == arg0) {
            break;
        }
    }

    return i != total;
}
