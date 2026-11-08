// Differ 4 of 119 (tla-en, 2026-11-08, -mtune=gba).
// 2026-11-08: slot reused for the owner's id (the native's id sits in r5, slot's
// register) closed the allocation: 31 to 4. Left: the native rebuilds sp for the
// call (mov r0, sp) and again for the owner pointer after the count (mov r7, sp);
// here the pointer takes sp before the call and the argument copies it. Measured:
// owner = owners after the call, after remaining, or in the for init (53 to 56),
// at the top of the function (8), owners[i], *(owners + i), &owners[i] or a
// block-scoped pointer (54, not reduced to a giv), a once-only block around the
// pointer's store (4 before the call, 53 after), owner as the call's argument (4).
// Before, with a separate s16 id (31): owner = owners after the call, before or
// in the for init (62, 71), owners[i] or &owners[i] (69, 71), declaration order;
// permutes of 4 to 10 minutes found nothing below 31.
// Near miss of Func_0200290c of overlay 696. The emote arm comes first, reached
// by goto from the failed owner test; owner = &owners[i] is the giv loop.c
// reduces; items[slot] through an s32 item gives ldrh r0. Func_020049cc called
// plainly costs 2, so SHAMAN_VILLAGE_EMOTE stays.
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

s32 PartyInventory_FindOwner(s32 item);
u8 *Unit_Get(s32 owner);
u8 *Func_02004894(s32 item);
void Actor_WatchAndSetScript(s32 actor, s32 target);

void Func_0200290c(void) {
  s16 owners[10];
  s32 count;
  s32 i;
  s32 slot;
  u32 remaining;
  u16 *items;
  s16 *owner;

  Func_0200498c((s32)MsgShamanMissingShamanRod);
  if (PartyInventory_FindOwner(65) == -1)
    goto refuse;
  owner = owners;
  count = Party_ListActiveOwners(owners);
  remaining = 8;
  for (i = 0; i < count; i++) {
    // slot holds the owner's id until the item loop reuses it.
    slot = *owner++;
    items = (u16 *)(Unit_Get(slot) + 216);
    for (slot = 0; slot < 15; slot++) {
      s32 item = items[slot];
      if (item == 0)
        break;
      if ((u8)(Func_02004894(item)[2] - 1) <= 3)
        remaining = (u8)(remaining - 1);
      if (remaining == 0)
        break;
    }
    if (remaining == 0)
      break;
  }
  EVENT_WORK->field1c4++;
  if (remaining != 0) {
  refuse:
    /* FAKEMATCH: the emote and the frames are loaded before the actor; see
     * SHAMAN_VILLAGE_EMOTE. */
    SHAMAN_VILLAGE_EMOTE(15, 257, 20)
    ShamanVillage_MessagePut(0x200f, 0);
    Func_0200289c();
  } else {
    EVENT_WORK->field1c4++;
    ShamanVillage_MessagePut(0x200f, 0);
    Actor_WatchAndSetScript(16, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(17, gMainRoutineWork.currentOwner);
    Actor_WatchAndSetScript(15, gMainRoutineWork.currentOwner);
    GameFlag_SetBit(0x9e0);
    GameFlag_SetBit(0x201);
  }
}
