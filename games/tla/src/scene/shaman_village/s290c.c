#include "shaman_village.h"
#include "game/item.h"
#include "event_inline.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

u8 *Unit_Get(s32 owner);
struct ItemDefinition *Func_02004894(s32 item);

// Forwards to Party_ListActiveOwners; tagged where it is used.
void Func_0200290c(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u8 remaining;
  u16 *items;
  s16 *owner;

  Event_SetMessageId((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindOwner(65) == -1)
    goto refuse;
  /* FAKEMATCH(wrapper): the count read through ListOwners; as a direct
   * Party_ListActiveOwners call with owner = owners before it, 4 of 119
   * instructions differ in tla-en. */
  count = FieldParty_ListMembers(owners);
  owner = owners;
  remaining = 8;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = (u16 *)(Unit_Get(slot) + 216);
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Func_02004894(item)->type - 1) <= 3)
        remaining--;
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->field1c4++;
  if (remaining != 0) {
  refuse:
    /* FAKEMATCH(pin): the emote and the frames pinned to r1 and r2 by
     * SHAMAN_VILLAGE_EMOTE, so they load before the actor; as a plain
     * Func_080d47b4(15, 257, 20), 2 of 119 instructions differ in tla-en. */
    SHAMAN_VILLAGE_EMOTE(15, 257, 20)
    Event_ShowMessageAndWait(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    Event_ShowMessageAndWait(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    GameFlag_SetBit(0x9e0);
    GameFlag_SetBit(0x201);
  }
}
