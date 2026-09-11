#include "types.h"
#include "scene.h"
#include "abi/battle/party/is_unit_listed.h"

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
    total = Battle_Apply(count, values);

    for (i = 0; i < total; i++) {
        if (values[i] == arg0) {
            break;
        }
    }

    return i != total;
}
