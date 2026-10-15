#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_work.h"
#include "party_state.h"
#include "ram_buffer.h"


s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (Ram_HeapSlots->battle_work->two_sided != 0)
        limit = 3;

    count = Party_CountActiveOwnersFar();
    if (count > limit)
        count = limit;

    for (index = 0; index < count; index++) {
        s32 owner = gPartyState.active_owners[index];

        if (owners != 0)
            *owners++ = owner;
        ((struct BattleUnit *)UnitGet(owner))->side = 2;
    }

    if (owners != 0)
        *owners = 0xff;
    return count;
}
