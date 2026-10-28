// Draft of overlay 685's Func_02000f48, differ 68 of 205: the trident talk,
// which takes the first of items 439 to 441 the party holds and runs the next
// of three flagged scenes with it, or says one of four lines. The gotos give
// the native's block order (the shared removal and the item 441 test ahead of
// the item 439 test), and the flag and marker calls go through forwarding
// inlines as in scene_call.h (differ 197 without them). Left, allocation: the
// native compares each owner in r0 and keeps copies in r10 and r8 for the next
// test (CSE's -1), with the item in r5 and the actor in r6; here the owners
// take r5 and r6, the item r7 and the actor r8. Tried: owner variables (86), a
// forwarding inline for PartyInventory_FindOwner (68).
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
void ObjectMotion_SetAngleToward(s32 actor, s32 target, s32 frames);
void ObjectMotion_ArmCallback(s32 actor, s32 angle, s32 frames);

static __inline__ s32 GameFlag_Test_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void GameFlag_SetBit_(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Func_0200343c_(s32 x, s32 z, s32 a, s32 b) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_0200343c(x, z, a, b);
}

void Func_02000f48(s32 actor) {
  s32 item;

  if ((Object_GetById(gAdventure.currentOwner)->x >> 20) <= 41)
    goto first;
  ((struct EventFlags685 *)Ram_HeapSlots->eventWork)->field15a = 1;
  return;
remove:
  PartyInventory_Remove(item);
  goto chosen;
last:
  item = 441;
  if (PartyInventory_FindOwner(item) != -1)
    goto remove;
  item = -1;
  goto chosen;
first:
  item = 439;
  if (PartyInventory_FindOwner(item) != -1)
    goto remove;
  item++;
  if (PartyInventory_FindOwner(item) == -1)
    goto last;
  PartyInventory_Remove(item);
chosen:
  if (item != -1) {
    if (!GameFlag_Test_(0x975)) {
      GameFlag_SetBit_(0x975);
      Func_02000804(item);
    } else if (!GameFlag_Test_(0x976)) {
      GameFlag_SetBit_(0x976);
      Func_02000978(item);
    } else if (!GameFlag_Test_(0x977)) {
      GameFlag_SetBit_(0x977);
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
    ObjectMotion_SetAngleToward(actor, gAdventure.currentOwner, 5);
    if (!GameFlag_Test_(0x975))
      Func_0200350c((s32)MsgIMeanIt);
    else if (!GameFlag_Test_(0x976))
      Func_0200350c((s32)MsgTheLegendSaysThat);
    else if (!GameFlag_Test_(0x977))
      Func_0200350c((s32)MsgYesThereS);
    else
      Func_0200350c((s32)MsgTheTridentSeems);
    Func_02003524(actor, 0);
    ObjectMotion_ArmCallback(actor, 0x5000, 0);
  }
}
