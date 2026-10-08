#include "types.h"
#include "serial_runtime.h"
#include "btlpty.h"
#include "btlwork.h"
#include "iwram_call.h"

s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    struct BtlWork *battle_state = BtlWk;
    s32 party_size = BattleParty_PrepareActiveOwners(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        battle_state->owner_slots[unit_id] = member_slot - 128;
    }
}

s32 Battle_ClearSerialTransferPayload(void)
{
    return Iwram_ClearWords(gSerialTransfer.payload, 16);
}
