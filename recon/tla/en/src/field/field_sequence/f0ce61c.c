#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "field/event_work.h"
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

extern char MsgKey0dba, MsgKey0dbb, MsgKey0dbc,
    MsgKey0dbd;

struct BattleAct *BattleAction_Get(s32 actionId);
struct FieldUnit *Unit_Get(s32 unitId);
void Unit_AddPpFar(s32 unitId, s32 amount);
void Leader_StopMoving(void);
void GameFlag_ClearBit(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);
void MessageArgument_PushFar(s32 value, s32 kind);
void FieldMessage_ShowFar(s32 message, s32 mode);
s32 Func_080d295c(s32 cursor);
void Message_CloseFinishedWindowsFar(void);
s32 Func_080ce31c(s32 kind);
struct PendingEvent *Func_080ce458(u32 filter, s32 kind, s32 selector);
void Field_RunPsynergyEvent(struct PendingEvent *event, s32 a, s32 b);
s32 Event_ExecutePackedAction(u32 action);
s32 Party_GetLeaderActor(void);
s32 Func_080dbcd8(s32 x, s32 z, s32 terrain);
void BattleEffect_LoadActionEffectResources(s32 action, s32 mode);
void BattleEffect_SetupObjectPair(s32 caster, s32 target);
void Func_080dc6d8(void);
void BattleEffect_DispatchRequestKind(void);
void BattleEffect_Run(void);
void BattleEffect_StopMainActor(void);
void BattleEffect_ClearChildValueOnMismatch(void);
void BattleEffect_CleanupSceneObjects(void);

#define FIELD_PSYNERGY_MESSAGE(user, ability, message)                         \
  (MessageArgument_PushFar((user), 1), MessageArgument_PushFar((ability), 4),          \
   FieldMessage_ShowFar((s32)&(message), 1))

s32 Func_080ce61c(u32 code) {
  u8 *event = Ram_HeapSlots->eventWork;
  s32 abilityId = code & 0x3ff;
  struct PendingEvent *before;
  s32 group = BattleAction_Get(abilityId)->group;
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
    cost = Func_080d295c(1);
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
  pending = Func_080ce458(0x80000005, group, selector);
  if (pending != NULL) {
    target = -1;
    if (selector != -1 && (selector & 0x100))
      target = selector & 0xff;
    Field_RunPsynergyEvent(pending, userId, target);
  }
  if (code & 0x2000)
    return Event_ExecutePackedAction(code);

  selector = Func_080ce31c(group);
  before = Func_080ce458(0x10000005, group, selector);
  after = Func_080ce458(5, group, selector);
  during = Func_080ce458(0x50000005, group, selector);
  if (abilityId == 156 && gMainRoutineWork.scene != (s32)&SceneId_WorldMap &&
      before == NULL && after == NULL && during == NULL) {
    FIELD_PSYNERGY_MESSAGE(userId, 156, MsgKey0dbd);
    return 0;
  }
  if (userId <= 7) {
    struct BattleAct *action = BattleAction_Get(abilityId);

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
  {
    u32 zero = 0;

    if (before != NULL && (before->trigger & 0x200)) {
      effect = Ram_HeapSlots->effectWork;
      effect[34] = zero;
      effect[35] = zero;
    }
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
  BattleEffect_SetupObjectPair(Party_GetLeaderActor(), target);
  Func_080dc6d8();
  Field_RunPsynergyEvent(before, userId, target);
  if (GameFlag_Test(320)) {
    if (GameFlag_Test(321))
      BattleEffect_DispatchRequestKind();
    else
      BattleEffect_Run();
  }
  BattleEffect_StopMainActor();
  hit = ((s8 *)event)[0xd30];
  if (hit != -1) {
    selector = hit | 0x100;
    pending = Func_080ce458(0x20000005, group, selector);
    if (pending != NULL)
      Field_RunPsynergyEvent(pending, userId, ((s8 *)event)[0xd30]);
  }
  Field_RunPsynergyEvent(after, userId, target);
  if (GameFlag_Test(320))
    BattleEffect_ClearChildValueOnMismatch();
  event[0xcb6] = 0;
  BattleEffect_CleanupSceneObjects();
  GameFlag_ClearBit(320);
  GameFlag_ClearBit(321);
  return 0;
}
