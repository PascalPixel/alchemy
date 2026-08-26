#include "battle_runtime.h"
#include "battle_types.h"
#include "battle_work.h"
#include "party_state.h"

#define BattleParty_PrepareActiveOwners Func_080b6a60

s32 Func_08077148(void);

s32 BattleParty_PrepareActiveOwners(s16 *owners)
{
    s32 limit;
    s32 left;
    s32 count;
    s32 index;

    limit = 4;
    if (((u8 *)BattleWorkPtr)[68] != 0)
        limit = 3;

    count = Func_08077148();
    if (count > limit)
        count = limit;

    index = 0;
    if (count > 0) {
        left = count;
        do {
            s32 owner = Data_02000240.active_owners[index++];

            if (owners != 0)
                *owners++ = owner;
            BattleUnit_Get(owner)->status_12a = 2;
            left--;
        } while (left != 0);
    }

    if (owners != 0)
        *owners = 0xff;
    return count;
}
