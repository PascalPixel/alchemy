#include "battle_party.h"
#include "ram_buffer.h"

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    u8 *battle_state = (u8 *)Ram_HeapSlots->battleWork;
    s32 party_size = BattleParty_PrepareActiveOwners(active_members);
    s32 member_slot;
    s32 unitId;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unitId = active_members[member_slot];
        unitId += 72;
        battle_state[unitId] = (s8)(member_slot - 128);
    }
}
