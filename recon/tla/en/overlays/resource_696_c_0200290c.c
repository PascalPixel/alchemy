// Near miss of Func_0200290c of overlay 696 (119 instructions, differ 127): the
// logic is complete, the registers are not. The native keeps the countdown in
// r10 (i in r8, count in r9) and its frame is the 20 bytes of the owner list;
// the draft gives the countdown a call-clobbered r1 with a save slot (frame
// 24), because its 13 loop-weighted uses over 2 calls make caller-save
// profitable (refs > 4 * calls). The native countdown therefore has at most 8
// weighted uses or crosses more calls. Measured: u8 or u32 countdown, owners[i]
// or a walking pointer.
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

s32 PartyInventory_FindOwner(s32 item);
u8 *Unit_Get(s32 owner);
u8 *Func_02004894(s32 item);
void Object_LinkObjectAndSetCallback(s32 actor, s32 target);

void Func_0200290c(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u32 remaining;
  u16 *items;
  s16 *owner = owners;

  Func_0200498c((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindOwner(65) != -1) {
    count = Party_ListActiveOwners(owners);
    remaining = 8;
    for (i = 0; i < count; i++) {
      items = (u16 *)(Unit_Get(*owner++) + 216);
      for (slot = 0;;) {
        if (slot >= 15 || *items == 0)
          break;
        if ((u8)(Func_02004894(*items)[2] - 1) <= 3)
          remaining = (u8)(remaining - 1);
        items++;
        slot++;
        if (remaining == 0)
          break;
      }
      if (remaining == 0)
        break;
    }
    EVENT_WORK->field1c4++;
    if (remaining == 0) {
      EVENT_WORK->field1c4++;
      ShamanVillage_MessagePut(0x200f, 0);
      Object_LinkObjectAndSetCallback(16, gAdventure.currentOwner);
      Object_LinkObjectAndSetCallback(17, gAdventure.currentOwner);
      Object_LinkObjectAndSetCallback(15, gAdventure.currentOwner);
      GameFlag_SetBit(0x9e0);
      GameFlag_SetBit(0x201);
      return;
    }
  }
  /* FAKEMATCH: the emote and the frames are loaded before the actor; see
   * SHAMAN_VILLAGE_EMOTE. */
  SHAMAN_VILLAGE_EMOTE(15, 257, 20)
  ShamanVillage_MessagePut(0x200f, 0);
  Func_0200289c();
}
