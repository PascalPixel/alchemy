#include "script.h"
#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_bridge.h"
#include "unit/scene/common/kolima_forest_bridge_falling_effect.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "tbs_edition.h"
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "kolima.h"
#include "staged_actor.h"
#include "staged_actor_effect.h"
#include "text/msg_ids.h"
#include "objcmd.h"

// Forward declarations.
static void FieldScene_RunBranchingFormationPresentation(void);
static void Event_SayThenWait(s32 speaker, s32 frames);
static void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
static void SceneEffect_SpawnObject26EveryEightFrames(void);
static void KolimaForestBridge_AlternateBridgePostHeights(void);

struct Sub {
  u8 pad00[9];
  u8 f09;
  u8 pad0a[28];
  u8 f26;
};

extern s32 gKolimaForestBridge_TriggerPending;
extern s32 gKolimaForestBridge_PartyFlag;
extern s32 gKolimaForestBridge_SparkleSound;

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

static s32 OverlayObject_ClearPendingAndRestoreMode(
    struct ScriptInterpreter *interpreter) {
  u8 *object = (u8 *)interpreter;
  s32 *pending = &gKolimaForestBridge_TriggerPending;
  if (*pending) {
    Object_SetMode(object, 2);
    *pending = 0;
  }
  return 1;
}

static s32
SceneActor_UpdateRandomCounterMode(struct ScriptInterpreter *interpreter) {
  u8 *object = (u8 *)interpreter;
  u16 *counter = (u16 *)(object + 100);
  *counter = (*counter + (((u32)Engine_RandomNext() * 100) >> 16));
  if ((s16)*counter > 1000)
    ObjectGroup_SetChildValue(object, 7);
  else
    ObjectGroup_SetChildValue(object, 10);
  if (*(s16 *)counter > 1200) {
    u16 z = 0;
    *counter = z;
  }
  return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gKolimaForestBridge_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gKolimaForestBridge_Exits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  return gKolimaForestBridge_Placements;
}

static void EV10_SCRIPT(void) {
  struct StagedActorProbe res;
  Engine_EventBegin();

  if (StagedActor_FindClearPosition(&res)) {
    SceneActor_MoveAndRedraw(res);
    if (res.actorSlot == 10 && (res.positionX >> 20) == 20) {
      struct FieldActor *actor;

      Engine_ActorSetAnimation(10, 3);
      Actor_SetDestinationOffset(10, -18, 6);
      Engine_EventWait(30);
      Engine_AudioPlayCue(240);
      Engine_ActorSetAnimation(10, 8);
      Object_GetById(10)->priorityFlags = 2;
      Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
      Engine_GameFlagSet(0x200);
      actor = Object_GetById(10);
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }

  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/kolima_jump.h"

const struct SceneEvent *Scene_GetEvents(void) {
  return gKolimaForestBridge_Events;
}

s32 Scene_Initialize(void) {
  FieldScene_RedrawActorFootprint(10);
  if (Engine_GameFlagIsSet(0x200) != 0) {
    Object_GetById(10)->priorityFlags = 2;
    Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
  }
  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);
  if (gGameState.entrance == 4) {
    if (Engine_GameFlagIsSet(0x843) == 0) {
      FieldScene_RunBranchingFormationPresentation();
    }
  }
  if (Engine_GameFlagIsSet(0x845) != 0) {
    Engine_ActorSetPosition(17, 0, 0);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_ActorSetPosition(21, 0, 0);
  }
  return 0;
}

static void FieldScene_RunBranchingFormationPresentation(void) {
  struct FieldActor *record;
  s32 *party;
  const s32 *entryAction;
  const s32 *motionAction;
  s32 *partyFlag;
  const s32 *resetAction;
  const s32 *formationAction;
  const s32 *finishAction;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_MoveTo(0xf60000, -1, 0x25c0000, 0);
  party = &gKolimaForestBridge_PartyFlag;
  *party = GameFlag_IsSet(3);
  record = Object_GetById(13);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(14);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(15);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(16);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(17);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(18);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(19);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(20);
  Engine_ActorSetSpriteFlags(record, 0);
  record = Object_GetById(21);
  Engine_ActorSetSpriteFlags(record, 0);
  entryAction = gKolimaForestBridge_EntryAction;
  Actor_SetActionScript(17, entryAction);
  Actor_SetActionScript(18, entryAction);
  Actor_SetActionScript(19, entryAction);
  Actor_SetActionScript(20, entryAction);
  Actor_SetActionScript(21, entryAction);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0x740000, 0x25a0000);
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
  Actor_SetSpeed(GARET, 0x9999, 0x4ccc);
  Actor_SetSpeed(IVAN, 0x9999, 0x4ccc);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
    }
  }
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
    }
  }
  Actor_SetActionScript(GARET, gKolimaForestBridge_GaretAction);
  Actor_SetActionScript(IVAN, gKolimaForestBridge_IvanAction);
  if (*party != 0) {
    Actor_SetSpeed(MIA, 0x9999, 0x4ccc);
    {
      struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    Actor_SetActionScript(MIA, gKolimaForestBridge_MiaAction);
  }
  Object_RefreshSelectorById(2);
  SceneActor_SetPairZeroAndValue(2, 0x2000, 40);
  SceneActor_SetPairZeroAndValue(2, 0x8000, 20);
  SceneActor_SetPairZeroAndValue(2, 0x4000, 40);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
  SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgKolimaMatterIvan);
  Event_SayThenWait(GARET, 10);
  Engine_EventSetMessage(MsgKolimaSparklyStuffOnGround);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(IVAN, 20);
  SceneActor_SetPairZeroAndValue(1, 0, 20);
  SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
  SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 30);
  SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
  SceneActor_SetPairZeroAndValue(0, 0xe000, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
  Event_PlayCue(17);
  Event_PlayCue(206);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gKolimaForestBridge_SparkleSound = 1;
  FieldScheduler_AddCallback(SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(0x405210, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(60);
  motionAction = gKolimaForestBridge_PartyAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, motionAction);
  Actor_SetActionScript(GARET, motionAction);
  Actor_SetActionScript(IVAN, motionAction);
  Actor_SetActionScript(MIA, motionAction);
  Engine_EventWait(100);
  Event_SayThenWait(GARET, 20);
  Event_SayThenWait(IVAN, 40);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Engine_EventWait(40);
    Actor_ShowEmote(MIA, 0x102, 0);
    Engine_EventWait(40);
    Event_SayThenWait(MIA, 40);
  } else {
    gEventWork->message += 1;
  }
  Engine_EventWait(20);
  if (gKolimaForestBridge_PartyFlag != 0) {
    record = Object_GetById(3);
    record->velocityY = 0x20000;
    Engine_EventWait(10);
    Actor_SetSpeed(MIA, 0x20000, 0x20000);
    Actor_SetDestinationOffset(MIA, -2, 0);
    Actor_SetActionScript(MIA, gKolimaForestBridge_ResetAction);
    record = Object_GetById(3);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(MIA, 19);
    Engine_EventWait(10);
  }
  record = Object_GetById(0);
  record->velocityY = 0x20000;
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
  resetAction = gKolimaForestBridge_ResetAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, resetAction);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(20);
  record = Object_GetById(1);
  record->velocityY = 0x20000;
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, 0x20000, 0x20000);
  Actor_SetActionScript(GARET, resetAction);
  record = Object_GetById(1);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_EventWait(40);
  record = Object_GetById(2);
  record->velocityY = 0x20000;
  Engine_EventWait(10);
  Actor_SetActionScript(IVAN, resetAction);
  record = Object_GetById(2);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(IVAN, 19);
  gKolimaForestBridge_SparkleSound = 0;
  Engine_EventWait(160);
  Engine_TaskRemoveCallback(SceneEffect_SpawnObject26EveryEightFrames);
  Engine_EventWait(120);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = 1;
  FieldScheduler_AddCallback(FieldScene_UpdateFallingEffect, 0xc80);
  Engine_EventWait(180);
  Event_PlayCue(21);
  Event_SayThenWait(GARET, 80);
  Event_SayThenWait(IVAN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 0);
  Engine_EventWait(60);
  Event_SayThenWait(IVAN, 20);
  gFallingEffectState = 2;
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(MIA, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(IVAN, 3);
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_EventWait(20);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Actor_ShowEmote(GARET, 0x102, 0);
  Event_SayThenWait(GARET, 20);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Actor_ShowEmote(MIA, 0x102, 0);
    Event_SayThenWait(MIA, 10);
  } else {
    gEventWork->message += 1;
  }
  gFallingEffectState = 3;
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(1)->priorityFlags &= 254;
  Object_GetById(2)->priorityFlags &= 254;
  Object_GetById(3)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  gKolimaForestBridge_TriggerPending = 0;
  FieldScheduler_AddCallback(KolimaForestBridge_AlternateBridgePostHeights,
                             0xc80);
  Event_PlayCue(220);
  Object_GetById(13)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(13, 2);
  Actor_SetPosition(13, 0xfd0000, 0x25b0000);
  formationAction = gKolimaForestBridge_FormationAction;
  Actor_SetActionScript(13, formationAction);
  Object_GetById(14)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(14, 2);
  Actor_SetPosition(14, 0xe90000, 0x2750000);
  Actor_SetActionScript(14, formationAction);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Object_GetById(15)->priorityFlags &= 254;
    Engine_ActorSetSpritePriority(15, 2);
    Actor_SetPosition(15, 0xcf0000, 0x2610000);
    Actor_SetActionScript(15, formationAction);
  }
  Object_GetById(16)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(16, 2);
  Actor_SetPosition(16, 0xe30000, 0x2440000);
  Actor_SetActionScript(16, formationAction);
  if (gFallingEffectState != 0) {
    do {
      Engine_TaskWait(1);
    } while (gFallingEffectState != 0);
  }
  Engine_EventWait(0x12c);
  Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
  Engine_EventWait(120);
  Event_PlayCue(17);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_ActorStop(13);
  Engine_ActorStop(14);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Engine_ActorStop(15);
  }
  Engine_ActorStop(16);
  Engine_TaskWait(1);
  finishAction = gKolimaForestBridge_FinishAction;
  Actor_SetActionScript(13, finishAction);
  Actor_SetActionScript(14, finishAction);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Actor_SetActionScript(15, finishAction);
  }
  Actor_RunActionScriptAndWait(16, finishAction);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  Actor_SetPosition(11, 0xdc0000, 0x1ee0000);
  Actor_SetPosition(12, 0xdc0000, 0x1ee0000);
  Engine_TaskWait(1);
  if (Engine_EventChooseYesNo(11, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Event_SayThenWait(IVAN, 20);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaKnowThoseFieldsWere);
    Event_SayThenWait(MIA, 40);
  }
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, 0x101, 0);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventSetMessage(MsgKolimaWasOurPsynergy);
  Event_SayThenWait(IVAN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Engine_ActorSetSpritePriority(GARET, 2);
  Object_GetById(1)->priorityFlags |= 1;
  record = Object_GetById(1);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(GARET, 6, 0);
  Actor_SetDestinationOffset(GARET, -3, 0);
  Engine_ActorSetAnimation(GARET, 1);
  SceneActor_SetPairZeroAndValue(1, 0x4000, 60);
  Event_SayThenWait(GARET, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Event_SayThenWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
  Actor_ShowEmote(GARET, 0x101, 0);
  Engine_EventWait(40);
  SceneActor_SetPairZeroAndValue(1, 0x6000, 40);
  SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
  SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
  SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Engine_EventWait(20);
  Event_SayThenWait(GARET, 20);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Actor_ShowEmote(MIA, 0x100, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(MIA, 2);
    Object_GetById(3)->priorityFlags |= 1;
    record = Object_GetById(3);
    Engine_ActorSetSpriteFlags(record, 1);
    Engine_ActorJump(MIA, 4, 0);
    Actor_SetDestinationOffset(MIA, -2, 0);
    Engine_ActorSetAnimation(MIA, 1);
    SceneActor_SetPairZeroAndValue(3, 0xe000, 60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    Event_SayThenWait(MIA, 20);
  } else {
    gEventWork->message += 1;
  }
  Engine_ActorJump(GARET, 2, 0);
  SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
  Event_SayThenWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(IVAN, 2);
  Object_GetById(2)->priorityFlags |= 1;
  record = Object_GetById(2);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(IVAN, 4, 0);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Object_GetById(0)->priorityFlags |= 1;
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
  Actor_ShowEmote(IVAN, 0x105, 0);
  Engine_EventWait(60);
  SceneActor_SetPairZeroAndValue(0, 0xa000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
  SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(IVAN, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(20);
  Event_OpenMessage(IVAN, 0);
  Engine_ActorSetAnimation(IVAN, 3);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimation(IVAN, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    gEventWork->message += 1;
  } else {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessage(GARET, 0);
  }
  SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Event_SayThenWait(GARET, 20);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(IVAN, 10);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    SceneActor_SetPairZeroAndValue(3, 0, 20);
    SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
    Engine_ActorSetAnimation(MIA, 4);
    Event_SayThenWait(MIA, 10);
  } else {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(80);
  SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Event_SayThenWait(IVAN, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(IVAN, 10);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(40);
  Event_SayThenWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, 0x102, 0);
  Engine_EventWait(40);
  Event_SayThenWait(GARET, 20);
  Engine_ActorSetAnimation(IVAN, 3);
  Event_SayThenWait(IVAN, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(GARET, 3);
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(40);
    gEventWork->message += 1;
  }
  Event_ShowMessage(GARET, 0);
  Event_PlayCue(21);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = 1;
  FieldScheduler_AddCallback(FieldScene_UpdateFallingEffect, 0xc80);
  Engine_EventWait(80);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(60);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
  Engine_EventSetMessage(MsgKolimaWatchOutItsHappeningAgain);
  Event_SayThenWait(IVAN, 10);
  SceneActor_SetPairZeroAndValue(1, 0xc000, 10);
  SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
  if (gKolimaForestBridge_PartyFlag != 0) {
    SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
  }
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(1)->priorityFlags &= 254;
  Object_GetById(2)->priorityFlags &= 254;
  Object_GetById(3)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  gFallingEffectState = 2;
  Event_PlayCue(220);
  Actor_SetPosition(13, 0xfd0000, 0x25b0000);
  formationAction = gKolimaForestBridge_FormationAction;
  Actor_SetActionScript(13, formationAction);
  Actor_SetPosition(14, 0xe90000, 0x2750000);
  Actor_SetActionScript(14, formationAction);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Actor_SetPosition(15, 0xcf0000, 0x2610000);
    Actor_SetActionScript(15, formationAction);
  }
  Actor_SetPosition(16, 0xe30000, 0x2440000);
  Actor_SetActionScript(16, formationAction);
  Engine_EventWait(120);
  gFallingEffectState = 3;
  do {
    Engine_TaskWait(1);
  } while (gFallingEffectState != 0);
  Event_SayThenWait(11, 80);
  Event_SayThenWait(12, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
  Actor_ShowEmote(GARET, 0x101, 0);
  Actor_ShowEmote(IVAN, 0x101, 0);
  Actor_ShowEmote(MIA, 0x101, 0);
  Engine_EventWait(60);
  Event_SayThenWait(12, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(12, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
  Actor_ShowEmote(GARET, 0x100, 0);
  Actor_ShowEmote(IVAN, 0x100, 0);
  Actor_ShowEmote(MIA, 0x100, 0);
  Engine_EventWait(40);
  Event_SayThenWait(11, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  SceneActor_SetPairZeroAndValue(2, 0xc000, 40);
  Event_SayThenWait(12, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0xc000, 80);
  Event_SayThenWait(12, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0, 40);
  Event_SayThenWait(11, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
  Engine_ActorSetAnimation(GARET, 4);
  Engine_ActorSetAnimation(MIA, 4);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(60);
  Event_SayThenWait(12, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Event_SayThenWait(12, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0, 20);
  Event_SayThenWait(12, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Event_SayThenWait(12, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0, 20);
  Event_SayThenWait(11, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
  Actor_ShowEmote(GARET, 0x102, 0);
  Actor_ShowEmote(MIA, 0x102, 0);
  Actor_ShowEmote(IVAN, 0x102, 0);
  Engine_EventWait(40);
  Event_SayThenWait(12, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
  Event_SayThenWait(12, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(60);
  Event_ShowMessage(12, 0);
  Event_ShowMessage(11, 0);
  Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
  Engine_EventWait(80);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(80);
  Engine_ActorStop(13);
  Engine_ActorStop(14);
  partyFlag = &gKolimaForestBridge_PartyFlag;
  Engine_ActorStop(15);
  Engine_ActorStop(16);
  Engine_TaskWait(1);
  finishAction = gKolimaForestBridge_FinishAction;
  Actor_SetActionScript(13, finishAction);
  Actor_SetActionScript(14, finishAction);
  if (*partyFlag != 0) {
    Actor_SetActionScript(15, finishAction);
  }
  Actor_RunActionScriptAndWait(16, finishAction);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetSpritePriority(GARET, 2);
  Engine_ActorSetSpritePriority(IVAN, 2);
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(0)->priorityFlags |= 1;
  Object_GetById(1)->priorityFlags |= 1;
  Object_GetById(2)->priorityFlags |= 1;
  Object_GetById(3)->priorityFlags |= 1;
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      SceneActor_SetPairZeroAndValue(3, 0, 20);
      Actor_ShowEmote(GARET, 0x101, 0);
      Actor_ShowEmote(IVAN, 0x101, 0);
      Actor_ShowEmote(MIA, 0x101, 0);
      Engine_EventWait(40);
      SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
      Event_SayThenWait(GARET, 10);
      SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
      SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Event_SayThenWait(IVAN, 20);
      SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
    } else {
      SceneActor_SetPairZeroAndValue(3, 0, 20);
      Actor_ShowEmote(GARET, 0x102, 0);
      Actor_ShowEmote(IVAN, 0x102, 0);
      Actor_ShowEmote(MIA, 0x102, 0);
      Engine_EventWait(40);
      SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
      Engine_EventSetMessage(MsgKolimaHesAsStumpedAsWe);
      Event_SayThenWait(GARET, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Event_SayThenWait(IVAN, 20);
    }
    Engine_ActorSetAnimation(MIA, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaYoureIsaacThereIsntMuch);
    Event_SayThenWait(GARET, 10);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Event_OpenMessage(IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(IVAN, 0x103, 0);
      Engine_EventWait(40);
      SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
      Event_SayThenWait(IVAN, 10);
      if (*partyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 3);
        Event_SayThenWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
      Actor_ShowEmote(GARET, 0x102, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Event_SayThenWait(GARET, 20);
      Actor_ShowEmote(GARET, 0x105, 0);
      Engine_EventWait(120);
      Event_SayThenWait(IVAN, 40);
      if (gKolimaForestBridge_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
        Engine_ActorSetAnimationAndWait(MIA, 4);
        Event_SayThenWait(MIA, 10);
      } else {
        gEventWork->message += 1;
      }
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      if (gKolimaForestBridge_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(2, 0xa000, 40);
        SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
      }
      Event_SayThenWait(IVAN, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(40);
      Event_SayThenWait(IVAN, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
      Engine_ActorSetAnimationAndWait(GARET, 3);
      Engine_EventWait(20);
      Engine_ActorSetAnimation(MIA, 3);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
    } else {
      Actor_ShowEmote(IVAN, 0x105, 0);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Engine_EventSetMessage(MsgKolimaThatsReliefIsaacThoughtYoud);
      Event_SayThenWait(IVAN, 20);
      if (*partyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 1);
        Event_SayThenWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(GARET, 0x102, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Event_SayThenWait(GARET, 20);
      Actor_ShowEmote(IVAN, 0x105, 0);
      Engine_EventWait(80);
      Event_SayThenWait(IVAN, 40);
      if (gKolimaForestBridge_PartyFlag != 0) {
        SceneActor_SetPairZeroAndValue(3, 0x2000, 20);
        Engine_ActorSetAnimation(MIA, 4);
        Event_SayThenWait(MIA, 40);
      } else {
        gEventWork->message += 1;
      }
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventWait(20);
      Event_SayThenWait(IVAN, 20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(40);
      Event_SayThenWait(IVAN, 20);
    }
  }
  Event_PlayCue(17);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Actor_SetSpeed(IVAN, 0x13333, 0x9999);
  Actor_SetSpeed(MIA, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, 2);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, 2);
  {
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

    if (leader != 0) {
      Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
    }
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  if (gKolimaForestBridge_PartyFlag != 0) {
    Engine_ActorSetAnimation(MIA, 2);
    {
      struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
      }
    }
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(0x843);
  Actor_SetPosition(11, 0, 0);
  Actor_SetPosition(12, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

static void Event_SayThenWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

static void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c) {
  Engine_ActorFaceDirection(a, b, 0);
  Engine_EventWait(c);
}

// Shared source is included here in ROM order.
#include "common/kolima_grow.h"

static void SceneEffect_SpawnObject26EveryEightFrames(void) {

  struct FieldActor *obj;
  struct Sub *sprite;
  s32 phase;
  s32 v;
  s32 w;
  s32 c1 = 0x00e70000;
  s32 c2 = 0x01cc0000;
  s32 c3 = 0x00e70000;
  s32 c4 = 0x02700000;

  phase = gFrameCount & 7;
  if (phase != 0)
    return;
  if (gKolimaForestBridge_SparkleSound != 0)
    Engine_AudioPlayCue(200);
  obj = Engine_ObjectCreate(26, c1, 0, c2);
  if (obj == 0)
    return;
  sprite = (struct Sub *)obj->sprite;
  sprite->f26 = phase;
  v = 0xfe;
  v &= obj->priorityFlags;
  obj->priorityFlags = v;
  w = ~12;
  w &= sprite->f09;
  w |= 4;
  sprite->f09 = w;
  obj->scaleX = 0x1999;
  obj->speed = 0x80000;
  obj->acceleration = 0x80000;
  obj->motionFlags = phase;
  Object_SetMode(obj, 2);
  Engine_ObjectSetPosition(obj, c3, 0, c4);
  Engine_ObjectSetScript(obj, gKolimaForestBridge_Object26Script);
}

// Shared source is included here in ROM order.
#include "common/kolima_blink.h"

static s32
SceneActor_CheckRegionTrigger(struct ScriptInterpreter *interpreter) {
  struct FieldActor *arg0 = (struct FieldActor *)interpreter;
  s32 x;

  if (!(gKolimaForestBridge_PartyFlag != 0
            ? (x = arg0->x.fixed,
               (x > 0xc00000 && x < 0x1120000 && arg0->z.fixed > 0x2360000 &&
                arg0->z.fixed < 0x2640000) ||
                   (x > 0xca0000 && x < 0xff0000 && arg0->z.fixed > 0x2250000 &&
                    arg0->z.fixed < 0x2780000))
            : (x = arg0->x.fixed,
               (x > 0xc00000 && x < 0xf40000 && arg0->z.fixed > 0x2250000 &&
                arg0->z.fixed <= 0x248ffff) ||
                   (x > 0xf40000 && x < 0x1120000 &&
                    arg0->z.fixed > 0x23b0000 && arg0->z.fixed <= 0x25cffff) ||
                   (x > 0xd30000 && x < 0xff0000 && arg0->z.fixed > 0x2540000 &&
                    arg0->z.fixed < 0x2780000)))) {
    return 0;
  }
  Engine_AudioPlayCue(106);
#ifdef UBFIX
  Engine_ObjectSetScript(arg0, gKolimaForestBridge_TriggerScript);
#else
  /* FAKEMATCH: called through a cast to the call type the bytes show; called by
   * its prototype or through Object_SetScript, r0 is set before the script
   * literal loads (2 differ) */
  ((s32 (*)())Engine_ObjectSetScript)((s32)arg0,
                                      (s32)gKolimaForestBridge_TriggerScript);
#endif
  gKolimaForestBridge_TriggerPending = 1;
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_forest_bridge_falling_effect.h"
s32 gKolimaForestBridge_TriggerPending __attribute__((section(".bss")));
s32 gKolimaForestBridge_PartyFlag __attribute__((section(".bss")));
s32 gKolimaForestBridge_SparkleSound __attribute__((section(".bss")));

static void KolimaForestBridge_AlternateBridgePostHeights(void) {
  struct FieldActor *p;

  p = Object_GetById(13);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = 0x1f40000;
    }
  }
  p = Object_GetById(14);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = 0x1f40000;
    }
  }
  p = Object_GetById(15);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = 0x1f40000;
    }
  }
  p = Object_GetById(16);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = 0x1f40000;
    }
  }
}

// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};
// clang-format on

s32 gKolimaForestBridge_GaretAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(234), 0, PIXELS(576)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ARG(1), SC_END,
};

s32 gKolimaForestBridge_IvanAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(234), 0, PIXELS(623)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1), SC_END,
};

s32 gKolimaForestBridge_MiaAction[] = {
    SC_ARG(2), SC_MOVE(PIXELS(211), 0, PIXELS(602)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTH),
    SC_ARG(1), SC_END,
};

s32 gKolimaForestBridge_PartyAction[] = {
    SC_ADD(SW_SCALE_X, -0x4000), SC_ADD(SW_SCALE_Y, 0x2000),  SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),  SC_ADD(SW_SCALE_Y, -0x1000), SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolimaForestBridge_ResetAction[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestBridge_FormationAction[] = {
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 1310),
    SC_ADD(SW_SCALE_Y, 1310),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(OverlayObject_ClearPendingAndRestoreMode),
    SC_END,
};

s32 gKolimaForestBridge_FinishAction[] = {
    SC_ARG(1),
    SC_ADD(SW_Z, -0x1999),
    SC_ADD(SW_SCALE_X, -983),
    SC_ADD(SW_SCALE_Y, -983),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKolimaForestBridge_EntryAction[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_SET(SW_COLLISION, 0),
    SC_SET(0x23, 0),
    SC_LABEL(1),
    SC_CALL(SceneActor_UpdateRandomCounterMode),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gKolimaForestBridge_Entrances[] = {
    {0, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 172, 0, 602, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKolimaForestBridge_Exits[] = {
    {40, 0x0, 4137, 16, 4136, 0x20, 4136, 48, -32766, 64, 511, 0},
};

struct SceneEntrance gKolimaForestBridge_Placements[] = {
    {1, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0},
    {2, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0},
    {3, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0},
    {205, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 128, 16384, 2},
    {207, CONDITION_ALWAYS, 7, 0, 0, 0xd0, 0, 0, 0, 296, 16384, 2},
    {205, CONDITION_ALWAYS, 7, 0, 0, 0x168, 0, 0, 0, 304, 16384, 2},
    {55, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0},
    {220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0},
    {220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256},
    {220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256},
    {220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256},
    {285, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 655, 16384, 0},
    {285, CONDITION_ALWAYS, 7, 0, 0, 0xa6, 0, 0, 0, 576, 16384, 256},
    {285, CONDITION_ALWAYS, 7, 0, 0, 0xbd, 0, 0, 0, 532, 16384, 256},
    {285, CONDITION_ALWAYS, 7, 0, 0, 0x120, 0, 0, 0, 570, 16384, 256},
    {285, CONDITION_ALWAYS, 7, 0, 0, 0x14b, 0, 0, 0, 659, 16384, 256},
    {SCENE_TABLE_END},
};

struct SceneEvent gKolimaForestBridge_Events[] = {
    {0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT)},
    {TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV11_TOUCH)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SCENE_EVENTS_END},
};

s32 gKolimaForestBridge_Object26Script[] = {
    SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolimaForestBridge_TriggerScript[] = {
    SC_SET(SW_SCALE_X, 0x20000),
    SC_SET(SW_SCALE_Y, 0x20000),
    SC_WAIT(10),
    SC_FREE,
};

s32 gFallingEffectScript[] = {
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(SceneEffect_SetModeByFrameBit1),
    SC_WAIT(1),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(SceneEffect_SetModeByFrameBit1),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_CALL(SceneEffect_SetModeByFrameBit1),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(SceneActor_CheckRegionTrigger),
    SC_FREE,
};
