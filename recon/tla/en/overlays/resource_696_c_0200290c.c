// Matches: differ 0 (tla-en, 2026-11-08)
// 2026-11-08: the count is read through ListOwners, a forwarding inline (tagged
// wrapper). Inlined, the call's argument is built straight into r0 (mov r0, sp)
// with no pseudo holding it, so after the call CSE knows no register equal to
// owners and the pointer rebuilds sp (mov r7, sp), as the native does. Called
// directly, the argument's pseudo survives the call and the pointer copies it
// (53). Measured before settling on the wrapper: an empty asm after the call
// (2: the count's copy is held above it), the same asm inside the argument (0,
// a barrier), a block-scoped pointer for the argument with a dead store after
// the call (48: CSE leaves a REG_EQUAL note on the pointer's store and doubles
// its live length), owner as the argument with a dead store and owner = owners
// again (7), a once-only block around the call (53: cse2 still merges), a
// pointer through a statement expression (53). The emote macro's pins stay:
// Func_020049cc called plainly still costs 2.
// Before, differ 4: slot reused for the owner's id (the native's id sits in r5,
// slot's register) closed the allocation: 31 to 4, the pointer taking sp before
// the call. Measured then: owner = owners after the call, after remaining, or
// in the for init (53 to 56), at the top of the function (8), owners[i],
// *(owners + i), &owners[i] or a block-scoped pointer (54, not reduced to a
// giv), a once-only block around the pointer's store (4 before the call, 53
// after), owner as the call's argument (4).
// Before, with a separate s16 id (31): owner = owners after the call, before or
// in the for init (62, 71), owners[i] or &owners[i] (69, 71), declaration order;
// permutes of 4 to 10 minutes found nothing below 31.
// Func_0200290c of overlay 696. The emote arm comes first, reached by goto from
// the failed owner test; the owner pointer is a biv set after the count, not a
// giv (loop.c would place a giv's start after the loop guard); items[slot]
// through an s32 item gives ldrh r0.
#include "../../../../games/tla/src/scene/shaman_village/shaman_village.h"

#define EVENT_WORK ((struct ShamanVillageEventWork *)Ram_HeapSlots->eventWork)

s32 PartyInventory_FindOwner(s32 item);
u8 *Unit_Get(s32 owner);
u8 *Func_02004894(s32 item);
void Actor_WatchAndSetScript(s32 actor, s32 target);

static inline s32 ListOwners(s16 *list) {
  /* FAKEMATCH(wrapper): the call through this inline; as a direct call to
   * Party_ListActiveOwners, 53 of 119 instructions differ in tla-en. */
  return Party_ListActiveOwners(list);
}

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
  count = ListOwners(owners);
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
