#include "../../../../games/tla/src/scene/champa_cliff_caves/scene.h"

struct EventFlags685 {
  u8 unk000[0x15a];
  u16 field15a;
};

s32 PartyInventory_FindOwner(s32 item);
void PartyInventory_Remove(s32 item);
void Func_0200343c(s32 x, s32 z, s32 a, s32 b);
void Func_02000804(s32 item);
void Func_02000978(s32 item);
void Func_02000a34(s32 item);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200343c(x, z, a, b);
}

static __inline__ void Actor_ArmCallback_(s32 actor, s32 angle, s32 frames) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_ArmCallback(actor, angle, frames);
}

void Func_02000f48(s32 actor) {
  s32 item;
  /* FAKEMATCH(dead): each owner stored and never read; as the bare tests,
   * 68 of 205 instructions differ in tla-en. */
  s32 owner;

  if ((Actor_GetById(gMainRoutineWork.currentOwner)->x >> 20) <= 41)
    goto first;
  ((struct EventFlags685 *)Ram_HeapSlots->eventWork)->field15a = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = 441;
  if ((owner = PartyInventory_FindOwner(item)) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = 439;
  if ((owner = PartyInventory_FindOwner(item)) != -1)
    goto remove;
  item++;
  if ((owner = PartyInventory_FindOwner(item)) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!GameFlag_Test_(0x975)) {
      GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 117)));
      Func_02000804(item);
    } else if (!GameFlag_Test_(0x976)) {
      GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 118)));
      Func_02000978(item);
    } else if (!GameFlag_Test_(0x977)) {
      GameFlag_SetBit(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 119)));
      Func_0200343c_(164 << 18, 208 << 16, 0, -4);
      Func_0200343c_(164 << 18, 224 << 16, 0, -4);
      Func_0200343c_(164 << 18, 240 << 16, 0, -4);
      Func_0200343c_(164 << 18, 128 << 17, 0, -4);
      Func_02000a34(item);
      Func_0200343c_(164 << 18, 208 << 16, 0, 0);
      Func_0200343c_(164 << 18, 224 << 16, 0, 0);
      Func_0200343c_(164 << 18, 240 << 16, 0, 0);
      Func_0200343c_(164 << 18, 128 << 17, 0, 0);
    }
  } else {
    Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 5);
    if (!GameFlag_Test_(SPLIT_CONSTANT(SPLIT_CONSTANT(0x900, 117))))
      Func_0200350c((s32)MsgIMeanIt);
    else if (!GameFlag_Test_(SPLIT_CONSTANT(0x976)))
      Func_0200350c((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Test_(0x977))
      Func_0200350c((s32)MsgYesThereS);
    else
      Func_0200350c((s32)MsgTheTridentSeems);
    Func_02003524(actor, 0);
    Actor_ArmCallback_(actor, 0x5000, 0);
  }
}
