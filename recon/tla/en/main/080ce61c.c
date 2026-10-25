// Draft, differ 243 of 543: uses Psynergy on the field, this game's form of
// ☀️'s Field_UsePsynergy (games/tbs/src/overworld/overworld/psynergy.c).
// code holds the Psynergy (bits 0 to 9), its user (10 to 13, 15 for the
// first) and bit 13 to run only its packed action. Psynergy does nothing
// with flag 382 set; in event mode 3, abilities 144 and 155 do nothing and
// 153 sets flag 327 (and the event work's 0x16a to 253) on the ship with
// flag 0x8df; ability 139 does nothing on the ship. Retreat (149) asks
// first and, unless flag 324 blocks it, sets the party's destination to its
// retreat point. The map's events for the cast (stage 8 before anything,
// then stages 1, 0 and 5 of the Psynergy's group, Func_080ce458) decide the
// target and whether the effect plays; the user pays the PP, the effect
// runs between the before and after events, and an object the effect hit
// (0xd30) runs its stage-2 event.
// Left: the native keeps the retreat flag, group, before event and event
// work at sp 0, 4, 8 and 12 (here 4 bytes higher) and the order of a few
// constants; the message names stand for 0xdba to 0xdbd.
#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"
#include "scene_ids.h"

struct PendingEvent {
  u8 unk00[4];
  u16 trigger;
};

struct BattleAct {
  u8 unk00[6];
  u8 group;
  u8 unk07[2];
  u8 ppCost;
};

struct FieldUnit {
  u8 unk00[58];
  s16 pp;
};

extern char MsgNotEnoughPp, MsgNothingHappened, MsgUseAbilityConfirm,
    MsgDoesNotWorkHere;

struct BattleAct *BattleAction_Get(s32 actionId);
struct FieldUnit *Unit_Get(s32 unitId);
void Unit_AddPpFar(s32 unitId, s32 amount);
void EventRuntime_PrepareCurrentObject(void);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
s32 Func_080d295c(s32 cursor);
void UiWork_FinalizePendingCoreFar(void);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void EventRuntime_RunPendingEvent(struct PendingEvent *event, s32 a, s32 b);
s32 EventRuntime_ExecutePackedAction(u32 action);
s32 EventRuntime_GetControlledOwner(void);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_LoadActionEffectResources(s32 action, s32 mode);
void BattleEffect_SetupObjectPair(s32 caster, s32 target);
void Func_080dc6d8(void);
void BattleEffect_DispatchRequestKind(void);
void BattleEffect_Run(void);
void EffectRuntime_StopCurrentObject(void);
void BattleEffect_ClearChildValueOnMismatch(void);
void BattleEffect_CleanupSceneObjects(void);

#define FIELD_PSYNERGY_MESSAGE(user, ability, message)                         \
  (UiText_DrawQuantity((user), 1), UiText_DrawQuantity((ability), 4),          \
   UiText_ShowPositionedMessageAndWaitFar((s32)&(message), 1))

s32 Func_080ce61c(u32 code) {
  s32 abilityId = code & 0x3ff;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 group = BattleAction_Get(abilityId)->group;
  s32 userId = (code >> 10) & 15;
  s32 retreat = 0;
  struct PendingEvent *before;
  struct PendingEvent *after;
  struct PendingEvent *during;
  struct PendingEvent *pending;
  struct ObjectRuntime *object;
  s32 selector;
  s32 target;
  s32 cost;
  s32 hit;
  u8 *effect;

  EventRuntime_PrepareCurrentObject();
  event[0xd30] = 255;
  *(s16 *)(event + 0xd48) = retreat;
  GameFlag_ClearBit(325);
  if (userId == 15)
    userId = 0;
  if (GameFlag_Test(382)) {
    FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
    return 0;
  }
  if (((s8 *)event)[394] == 3) {
    if (abilityId == 144 || abilityId == 155) {
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
      return 0;
    }
    if (abilityId == 153) {
      if (gPartyState.ownerOverride != 0 && GameFlag_Test(0x8df)) {
        GameFlag_SetBit(327);
        *(s16 *)(event + 362) = 253;
        return 0;
      }
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgNothingHappened);
      return 0;
    }
  }
  if (gPartyState.ownerOverride != 0 && abilityId == 139) {
    FIELD_PSYNERGY_MESSAGE(userId, 139, MsgNothingHappened);
    return 0;
  }
  if (abilityId == 149) {
    if (GameFlag_Test(324)) {
      FIELD_PSYNERGY_MESSAGE(userId, 149, MsgDoesNotWorkHere);
      return 0;
    }
    UiText_DrawQuantity(149, 4);
    UiText_ShowPositionedMessageAndWaitFar((s32)&MsgUseAbilityConfirm, 13);
    cost = Func_080d295c(1);
    UiWork_FinalizePendingCoreFar();
    if (cost != 0)
      return 0;
    {
      struct PartyState *state = &gPartyState;
      s32 scene, entrance;

      scene = (u16)state->retreatScene;
      state->scene = scene;
      entrance = (u16)state->retreatEntrance;
      state->entrance = entrance;
    }
    *(s16 *)(event + 344) = 999;
    retreat = 1;
  }

  selector = Func_080ce31c(group);
  pending = Func_080ce458(0x80000005, group, selector);
  if (pending != NULL) {
    target = -1;
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    EventRuntime_RunPendingEvent(pending, userId, target);
  }
  if (code & 0x2000)
    return EventRuntime_ExecutePackedAction(code);

  selector = Func_080ce31c(group);
  before = Func_080ce458(0x10000005, group, selector);
  after = Func_080ce458(5, group, selector);
  during = Func_080ce458(0x50000005, group, selector);
  if (abilityId == 156 && gPartyState.scene != (s32)&SceneId_Ov64a &&
      before == NULL && after == NULL && during == NULL) {
    FIELD_PSYNERGY_MESSAGE(userId, 156, MsgDoesNotWorkHere);
    return 0;
  }
  if (userId <= 7) {
    cost = BattleAction_Get(abilityId)->ppCost;
    if (Unit_Get(userId)->pp < cost) {
      if (retreat)
        *(s16 *)(event + 344) = 0;
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgNotEnoughPp);
      return 0;
    }
    Unit_AddPpFar(userId, -cost);
  }
  if (group == 23) {
    object = ObjectTable_Get(EventRuntime_GetControlledOwner());
    if (Func_080dbcd8(object->x, object->z, object->terrainId) == 0) {
      before = NULL;
      after = NULL;
      during = NULL;
    }
  }
  GameFlag_SetBit(320);
  GameFlag_ClearBit(321);
  target = -1;
  if (before != NULL || after != NULL || during != NULL) {
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    GameFlag_SetBit(321);
    if (after != NULL && (after->trigger & 0x400)) {
      GameFlag_ClearBit(320);
      GameFlag_ClearBit(321);
    }
  }
  BattleEffect_LoadActionEffectResources(abilityId, 0);
  event[0xcb6] = 1;
  if (before != NULL && (before->trigger & 0x200)) {
    effect = Ram_HeapSlots->effectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  if (after != NULL && (after->trigger & 0x200)) {
    effect = Ram_HeapSlots->effectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  if (during != NULL && (during->trigger & 0x200)) {
    effect = Ram_HeapSlots->effectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  BattleEffect_SetupObjectPair(EventRuntime_GetControlledOwner(), target);
  Func_080dc6d8();
  EventRuntime_RunPendingEvent(before, userId, target);
  if (GameFlag_Test(320)) {
    if (GameFlag_Test(321))
      BattleEffect_DispatchRequestKind();
    else
      BattleEffect_Run();
  }
  EffectRuntime_StopCurrentObject();
  hit = ((s8 *)event)[0xd30];
  if (hit != -1) {
    pending = Func_080ce458(0x20000005, group, hit | 0x100);
    if (pending != NULL)
      EventRuntime_RunPendingEvent(pending, userId, ((s8 *)event)[0xd30]);
  }
  EventRuntime_RunPendingEvent(after, userId, target);
  if (GameFlag_Test(320))
    BattleEffect_ClearChildValueOnMismatch();
  event[0xcb6] = 0;
  BattleEffect_CleanupSceneObjects();
  GameFlag_ClearBit(320);
  GameFlag_ClearBit(321);
  return 0;
}
