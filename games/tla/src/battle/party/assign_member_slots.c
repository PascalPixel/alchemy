#include "battle_side.h"
#include "ram_buffer.h"

s32 BattleParty_AssignMemberSlots(void) {
  u16 activeMembers[8];
  u8 *battleState = (u8 *)Ram_HeapSlots->battleWork;
  s32 partySize = BattleParty_PrepareActiveOwners(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    unitId += 72;
    battleState[unitId] = (s8)(memberSlot - 128);
  }
}
