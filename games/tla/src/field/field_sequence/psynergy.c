/// @file
/// Using Psynergy on the field.
// Part of field_sequence.c.

#include "../field_sequence_internal.h"
#include "module/field/event_commands.h"
#include "module/field/object.h"

#define FIELD_PSYNERGY_MESSAGE(user, ability, message)                         \
  (MessageArgument_PushFar((user), 1), MessageArgument_PushFar((ability), 4),  \
   FieldMessage_ShowFar((s32)&(message), 1))

s32 Field_UsePsynergy(u32 code) {
  u8 *event = Ram_HeapSlots->eventWork;
  s32 abilityId = code & 0x3ff;
  struct PendingEvent *before;
  s32 group = Ability_Get(abilityId)->fieldEffect;
  s32 userId = (code >> 10) & 15;
  s32 retreat = 0;
  struct PendingEvent *after;
  struct PendingEvent *during;
  struct PendingEvent *pending;
  struct Actor *object;
  s32 selector;
  s32 target;
  s32 cost;
  s32 hit;
  u8 *effect;

  Leader_StopMoving();
  event[0xd30] = 255;
  *(s16 *)(event + 0xd48) = retreat;
  GameFlag_ClearBit(325);
  if (userId == 15)
    userId = 0;
  if (GameFlag_Test(382)) {
    FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
    return 0;
  }
  if (((s8 *)event)[394] == 3) {
    if (abilityId == 144 || abilityId == 155) {
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
    if (abilityId == 153) {
      if (gMainRoutineWork.ownerOverride != 0 && GameFlag_Test(0x8df)) {
        GameFlag_SetBit(327);
        ((struct EventWork *)event)->unk16a = 253;
        return 0;
      }
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dbb);
      return 0;
    }
  }
  if (gMainRoutineWork.ownerOverride != 0 && abilityId == 139) {
    FIELD_PSYNERGY_MESSAGE(userId, 139, MsgKey0dbb);
    return 0;
  }
  if (abilityId == 149) {
    if (GameFlag_Test(324)) {
      FIELD_PSYNERGY_MESSAGE(userId, 149, MsgKey0dbd);
      return 0;
    }
    MessageArgument_PushFar(149, 4);
    FieldMessage_ShowFar((s32)&MsgKey0dbc, 13);
    cost = FieldYesNo_Ask(1);
    Message_CloseFinishedWindowsFar();
    if (cost != 0)
      return 0;
    {
      struct MainRoutineWork *state = &gMainRoutineWork;
      s32 scene, entrance;

      scene = (u16)state->retreatScene;
      state->scene = scene;
      entrance = (u16)state->retreatEntrance;
      state->entrance = entrance;
    }
    ((struct EventWork *)event)->countdown = 999;
    retreat = 1;
  }

  selector = Func_080ce31c(group);
  pending = Scene_FindPsynergyTrigger(0x80000005, group, selector);
  if (pending != NULL) {
    target = -1;
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    Field_RunPsynergyEvent(pending, userId, target);
  }
  if (code & 0x2000)
    return Field_RunPsynergyEndEvents(code);

  selector = Func_080ce31c(group);
  before = Scene_FindPsynergyTrigger(0x10000005, group, selector);
  after = Scene_FindPsynergyTrigger(5, group, selector);
  during = Scene_FindPsynergyTrigger(0x50000005, group, selector);
  if (abilityId == 156 && gMainRoutineWork.scene != (s32)&SceneId_WorldMap &&
      before == NULL && after == NULL && during == NULL) {
    FIELD_PSYNERGY_MESSAGE(userId, 156, MsgKey0dbd);
    return 0;
  }
  if (userId <= 7) {
    struct BattleAct *action = Ability_Get(abilityId);

    struct FieldUnit *unit = Unit_Get(userId);
    s32 ppCost = action->ppCost;

    if (unit->pp < ppCost) {
      if (retreat)
        ((struct EventWork *)event)->countdown = 0;
      FIELD_PSYNERGY_MESSAGE(userId, abilityId, MsgKey0dba);
      return 0;
    }
    Unit_AddPpFar(userId, -ppCost);
  }
  if (group == 23) {
    object = ObjectTable_Get(Party_GetLeaderActor());
    if (MapGrid_IsSandCell(object->x, object->z, object->terrainId) == 0) {
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
    if (after != NULL && (after->flags & 0x400)) {
      GameFlag_ClearBit(320);
      GameFlag_ClearBit(321);
    }
  }
  BattleEffect_LoadActionEffectResources(abilityId, 0);
  event[0xcb6] = 1;
  {
    u32 zero = 0;

    if (before != NULL && (before->flags & 0x200)) {
      effect = Ram_HeapSlots->effectWork;
      effect[34] = zero;
      effect[35] = zero;
    }
  }
  if (after != NULL && (after->flags & 0x200)) {
    effect = Ram_HeapSlots->effectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  if (during != NULL && (during->flags & 0x200)) {
    effect = Ram_HeapSlots->effectWork;
    effect[34] = 0;
    effect[35] = 0;
  }
  FieldPsynergy_SetupActorPair(Party_GetLeaderActor(), target);
  EventObject_Initialize();
  Field_RunPsynergyEvent(before, userId, target);
  if (GameFlag_Test(320)) {
    if (GameFlag_Test(321))
      FieldPsynergy_Dispatch();
    else
      FieldPsynergy_Run();
  }
  BattleEffect_StopMainActor();
  hit = ((s8 *)event)[0xd30];
  if (hit != -1) {
    selector = hit | 0x100;
    pending = Scene_FindPsynergyTrigger(0x20000005, group, selector);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, userId, ((s8 *)event)[0xd30]);
  }
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_Test(320))
    FieldPsynergy_FinishMindRead();
  event[0xcb6] = 0;
  BattleEffect_CleanupSceneObjects();
  GameFlag_ClearBit(320);
  GameFlag_ClearBit(321);
  return 0;
}

s32 Field_RunPsynergyEvent(struct PendingEvent *event, s32 mode, s32 target) {
  if (event != NULL) {
    if (event->action != 0) {
      if (event->action < 0x10000) {
        Event_Start();
        Event_SetMessageId(event->action);
        Event_ShowMessageAndWait(target, 0);
        Event_End();
      } else {
        ((void (*)(s32, s32))event->action)(mode, target);
      }
    }
    if (GameFlag_Test(0x142)) {
      Event_Start();
      FieldMessage_ShowFar((s32)MsgNothingHappens, 1);
      Event_End();
    }
  }
  return 0;
}

s32 BattleEffect_HasMatchingEvent5(s32 effectId) {
  u16 kind = effectId;
  s32 result =
      (s32)Scene_FindPsynergyTrigger(0xf0000005, kind, Func_080ce31c(kind));

  return (u32)((-result) | result) >> 31;
}

u32 BattleEffect_HasTrigger(u16 effectId) {
  s32 result;

  result = Field_FindItemTrigger(effectId);
  return (u32)((0 - result) | result) >> 31;
}
