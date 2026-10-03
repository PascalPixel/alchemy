/* Trial 2026-10-03: use OWNER_STATE.H with an explicit BattleUnit view.
 * Complete object unchanged; fresh EN score 60, 1 differing row.
 */
#include "OWNER_STATE.H"
/* Near miss: score 60. ⚓️ reads the battle work through its heap slot
   battle_work, the owners from gPartyState and calls the owner state
   directly. It schedules sub sp, #4 straight after loading the battle work;
   this draft after reading its flag with the approved game flags. */
#include "BATTLE_RUNTIME.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "PARTY_STATE.H"
#include "RAM_BUFFER.H"



s32 Party_CountActiveOwnersFar(void);

/* Caps the active party at four owners, or three in the alternate battle mode,
 * optionally writes their identifiers with a 0xff terminator, marks each
 * selected battle unit with status 2, and returns the selected count. */

s32 BattleParty_PrepareActiveOwners(u16 *owners)
{
    s32 limit;
    s32 count;
    s32 index;

    limit = 4;
    if (((u8 *)Ram_HeapSlots->battle_work)[68] != 0)
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
