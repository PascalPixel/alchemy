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

s32 BattleParty_PrepareReserveOwners(u16 *owners)
{
    s32 count;
    s32 index;

    count = Party_CountActiveOwnersFar();
    if (count > 4)
        count -= 4;
    else
        count = 0;
    if (Ram_HeapSlots->battle_work->two_sided != 0)
        count = 0;

    for (index = 0; index < count; index++) {
        s32 owner = gPartyState.active_owners[index + 4];

        if (owners != 0)
            *owners++ = owner;
        ((struct BattleUnit *)Owner_GetState(owner))->status_12a = 2;
    }

    if (owners != 0)
        *owners = 0xff;
    return count;
}
