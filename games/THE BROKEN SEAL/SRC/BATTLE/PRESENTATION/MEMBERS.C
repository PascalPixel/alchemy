#include "TYPES.H"
#include "SERIAL_RUNTIME.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_WORK.H"

extern u8 IwramClearWords[];

/* Indirect IWRAM clear entry; the third argument is ignored. */
s16 _call_via_r3(s32, s32, s16, s32);

/* Record the active party's member slots. */
s32 BattleParty_AssignMemberSlots(void)
{
    u16 active_members[8];
    struct BattleSession *battle_state = gBattleWork;
    s32 party_size = BattleParty_PrepareActiveOwners(active_members);
    s32 member_slot;
    s32 unit_id;

    for (member_slot = 0; member_slot < party_size; member_slot++) {
        unit_id = active_members[member_slot];
        battle_state->owner_slots[unit_id] = member_slot - 128;
    }
}

/* The uninitialized third argument is ignored by the clear entry. */
char Battle_ClearSerialTransferPayload(s16 arg2)
{
  s16 val;
  s16 val2;
  _call_via_r3((s32)gSerialTransfer.payload, 0x10, val, (u32)IwramClearWords);
  val2 = arg2;
  val = val2;
}
