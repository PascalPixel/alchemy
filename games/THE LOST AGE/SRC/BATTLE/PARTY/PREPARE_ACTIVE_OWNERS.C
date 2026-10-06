#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "OWNER_STATE.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"


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
        ((struct BattleUnit *)Owner_GetState(owner))->status_12a = 2;
    }

    if (owners != 0)
        *owners = 0xff;
    return count;
}
