#include "scene.h"
#include "abi/battle/party/assign_member_slots.h"
#include "battle_party.h"

extern u8 *gBattleWork;

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    u8 *battle_state = gBattleWork;
    s32 party_size = BattleParty_ListActiveMembers(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        unit_id += 72;
        battle_state[unit_id] = (s8)(member_slot - 128);
    }
}
