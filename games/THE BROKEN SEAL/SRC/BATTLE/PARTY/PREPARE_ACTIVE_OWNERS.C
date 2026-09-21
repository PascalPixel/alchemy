#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "PARTY_STATE.H"

#define BattleParty_PrepareActiveOwners Func_080b6a60

extern struct PartyState Data_02000240;

s32 Func_08077148(void);
struct BattleUnit *Runtime_GetObject(s32 unit_id);

/* Caps the active party at four owners, or three in the alternate battle mode,
 * optionally writes their identifiers with a 0xff terminator, marks each
 * selected battle unit with status 2, and returns the selected count. */
s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (((u8 *)BattleWorkPtr)[68] != 0)
        limit = 3;

    count = Func_08077148();
    if (count > limit)
        count = limit;

    for (index = 0; index < count; index++) {
        s32 owner = PARTY_STATE.active_owners[index];

        if (owners != 0)
            *owners++ = owner;
        Runtime_GetObject(owner)->status_12a = 2;
    }

    if (owners != 0)
        *owners = 0xff;
    return count;
}
