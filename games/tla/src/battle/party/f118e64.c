#include "types.h"

s32 BattleParty_PrepareActiveOwners(u16 *ids);
s32 BattleParty_PrepareReserveOwners(u16 *ids);
s32 BattleParty_ListActorIds(s32 side, u16 *ids);
u8 *UnitGet(s32 id);

void Func_08118e64(void)
{
    u16 ids[16];
    u16 *id = ids;
    s32 count;

    count = BattleParty_PrepareActiveOwners(ids);
    count += BattleParty_PrepareReserveOwners(ids + count);
    count += BattleParty_ListActorIds(2, ids + count);
    if (count > 0) {
        do {
            u8 *unit = UnitGet(*id++);

            unit[299] = 0;
            unit[325] = 0;
            count--;
        } while (count != 0);
    }
}
