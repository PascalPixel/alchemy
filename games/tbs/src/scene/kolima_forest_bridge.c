#include "script.h"
#include "flags.h"
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
#include "songs.h"
#include "fixed_math.h"

// Kolima Forest's bridge, where the road from the world map enters the
// forest. Of its three logs, the one pushed west to the bank tips into the
// river and bridges it. Arriving from the world map (entrance 4) before the
// party has seen the curse in Kolima, they see it fall here:
// sparkles of Tret's glamour rain down, domes of their own Psynergy shield
// them, and Tret and Laurel, unseen, speak of the dying forest.
//
// FLAG_SCENE_TEMP_00: the log is in the river.

enum {
  RIVER_LOG = 10, // actors 8 to 10 are the logs
  TRET = 11,      // "Hroom!"
  LAUREL = 12,
  // The domes of Psynergy, one over each member; Mia's only with Mia.
  LEADER_BARRIER = 13,
  IVAN_BARRIER = 14,
  MIA_BARRIER = 15,
  GARET_BARRIER = 16,
  // The sparkles of Tret's glamour on the ground, gone once the curse is
  // lifted.
  FIRST_GROUND_SPARKLE = 17,
  LAST_GROUND_SPARKLE = 21
};

static void KolimaForestBridge_WitnessForestCurse(void);
static void KolimaForestBridge_ShowMessageAndWait(s32 speaker, s32 frames);
static void KolimaForestBridge_FaceActorAndWait(s32 actorId, s32 facing,
                                                s32 waitFrames);
static void KolimaForestBridge_SpawnCurseSparkle(void);
static void KolimaForestBridge_FlickerBarriers(void);

extern s32 gKolimaForestBridge_BarrierHit;
extern s32 gKolimaForestBridge_MiaInParty;
extern s32 gKolimaForestBridge_SparkleSoundOn;

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// The end of a dome's growing: if a sparkle has burst on a dome, the dome
// takes its next pose.
static s32
KolimaForestBridge_FlashHitBarriers(struct ScriptInterpreter *interpreter) {
  u8 *object = (u8 *)interpreter;
  s32 *pending = &gKolimaForestBridge_BarrierHit;
  if (*pending) {
    Object_SetMode(object, 2);
    *pending = 0;
  }
  return 1;
}

// Each frame: a ground sparkle's timer runs on at random, and the sparkle
// shows its bright frame (7) while the timer is past 1000, its dim one (10)
// before; at 1200 the timer starts over.
static s32
KolimaForestBridge_TwinkleGroundSparkle(struct ScriptInterpreter *interpreter) {
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

// Where the party enters from the clearing and the world map.
const struct SceneEntrance *KolimaForestBridge_GetEntrances(void) {
  return gKolimaForestBridge_Entrances;
}

// The bridge has no regions.
const struct SceneRegion *KolimaForestBridge_GetRegions(void) { return 0; }

// The paths to the clearing and the world map.
const u32 *KolimaForestBridge_GetExits(void) {
  return gKolimaForestBridge_Exits;
}

// The party's companions, the logs, Tret, Laurel, the domes and the ground
// sparkles.
const struct ScenePlacement *KolimaForestBridge_GetPlacements(void) {
  return gKolimaForestBridge_Placements;
}

// Walking into a log: it rolls ahead until something stops it. The river
// log, stopped at the bank (cell column 20), tips over with a splash and
// floats in the water; its cells become ground the party can cross.
static void KolimaForestBridge_PushLog(void) {
  struct StagedActorProbe res;
  Engine_EventBegin();

  if (StagedActor_FindClearPosition(&res)) {
    SceneActor_MoveAndRedraw(res);
    if (res.actorSlot == RIVER_LOG && (res.positionX >> 20) == 20) {
      struct FieldActor *actor;

      Engine_ActorSetAnimation(RIVER_LOG, 3);
      Actor_SetDestinationOffset(RIVER_LOG, -18, 6);
      Engine_EventWait(30);
      Engine_AudioPlayCue(SE_240);
      Engine_ActorSetAnimation(RIVER_LOG, 8);
      Object_GetById(RIVER_LOG)->priorityFlags = 2;
      Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
      StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
      Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
      actor = Object_GetById(RIVER_LOG);
      Engine_ActorSetSpriteFlags(actor, 0);
    }
  }

  Engine_EventEnd();
}

// Shared source is included here in ROM order.
#include "common/kolima_jump.h"

// The logs, the jump east from the bank and the two paths.
const struct SceneEvent *KolimaForestBridge_GetEvents(void) {
  return gKolimaForestBridge_Events;
}

// On entry a log already in the river floats there again and the logs mark
// the cells they cover. From the world map, before the curse has been seen, the
// party sees it fall; once the curse is lifted the ground sparkles are gone.
s32 KolimaForestBridge_Initialize(void) {
  FieldScene_RedrawActorFootprint(RIVER_LOG);
  if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00) != 0) {
    Object_GetById(RIVER_LOG)->priorityFlags = 2;
    Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
    StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(RIVER_LOG), 0);
  }
  FieldScene_RedrawActorFootprint(8);
  FieldScene_RedrawActorFootprint(9);
  if (gGameState.entrance == 4) {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
      KolimaForestBridge_WitnessForestCurse();
    }
  }
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_ActorSetPosition(FIRST_GROUND_SPARKLE, 0, 0);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_ActorSetPosition(19, 0, 0);
    Engine_ActorSetPosition(20, 0, 0);
    Engine_ActorSetPosition(LAST_GROUND_SPARKLE, 0, 0);
  }
  return 0;
}

// The party, on the bridge, sees the curse fall. Ivan notices sparkly stuff
// on the ground; sparkles rain from the sky, the party sinks under them, and
// domes of Psynergy rise around each member before the sparkles reach them.
// They argue over whose Psynergy it was, the sparkles fall again and the
// domes rise again, and Tret and Laurel speak: Tret turned Kolima's people
// into trees for cutting him, and only his gentle side, deep in the forest,
// can save them. Lines and choices for Mia are skipped when she is not in
// the party.
static void KolimaForestBridge_WitnessForestCurse(void) {
  struct FieldActor *record;
  s32 *party;
  const s32 *sparkleAction;
  const s32 *squashAction;
  s32 *miaInParty;
  const s32 *restoreScaleAction;
  const s32 *growBarrierAction;
  const s32 *shrinkBarrierAction;

  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  Camera_MoveTo(PIXELS(246), -1, PIXELS(604), 0);
  // Flag 3: Mia is in the party.
  party = &gKolimaForestBridge_MiaInParty;
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
  sparkleAction = gKolimaForestBridge_GroundSparkleAction;
  Actor_SetActionScript(17, sparkleAction);
  Actor_SetActionScript(18, sparkleAction);
  Actor_SetActionScript(19, sparkleAction);
  Actor_SetActionScript(20, sparkleAction);
  Actor_SetActionScript(21, sparkleAction);
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(116), PIXELS(602));
  Engine_TaskWait(1);
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
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
    Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    {
      struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

      if (leader != 0) {
        Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
      }
    }
    Actor_SetActionScript(MIA, gKolimaForestBridge_MiaAction);
  }
  Object_RefreshSelectorById(2);
  KolimaForestBridge_FaceActorAndWait(2, 0x2000, 40);
  KolimaForestBridge_FaceActorAndWait(2, 0x8000, 20);
  KolimaForestBridge_FaceActorAndWait(2, 0x4000, 40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 60);
  KolimaForestBridge_FaceActorAndWait(3, 0x2000, 10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0xa000, 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgKolimaMatterIvan);
  KolimaForestBridge_ShowMessageAndWait(GARET, 10);
  Engine_EventSetMessage(MsgKolimaSparklyStuffOnGround);
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
  KolimaForestBridge_FaceActorAndWait(1, 0, 20);
  KolimaForestBridge_FaceActorAndWait(0, 0xa000, 40);
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 20);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 30);
  KolimaForestBridge_FaceActorAndWait(1, 0x6000, 20);
  KolimaForestBridge_FaceActorAndWait(0, 0xe000, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 20);
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 10);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_206);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  gKolimaForestBridge_SparkleSoundOn = 1;
  FieldScheduler_AddCallback(KolimaForestBridge_SpawnCurseSparkle, 0xc80);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(0x405210, 1);
  ColorBuffer_ApplyTarget(0x10000, 2);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(60);
  squashAction = gKolimaForestBridge_SquashAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, squashAction);
  Actor_SetActionScript(GARET, squashAction);
  Actor_SetActionScript(IVAN, squashAction);
  Actor_SetActionScript(MIA, squashAction);
  Engine_EventWait(100);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 40);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Engine_EventWait(40);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
    Engine_EventWait(40);
    KolimaForestBridge_ShowMessageAndWait(MIA, 40);
  } else {
    gEventWork->message += 1;
  }
  Engine_EventWait(20);
  if (gKolimaForestBridge_MiaInParty != 0) {
    record = Object_GetById(3);
    record->velocityY = PIXELS(2);
    Engine_EventWait(10);
    Actor_SetSpeed(MIA, PIXELS(2), PIXELS(2));
    Actor_SetDestinationOffset(MIA, -2, 0);
    Actor_SetActionScript(MIA, gKolimaForestBridge_RestoreScaleAction);
    record = Object_GetById(3);
    Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorSetAnimation(MIA, 19);
    Engine_EventWait(10);
  }
  record = Object_GetById(0);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(2), PIXELS(2));
  restoreScaleAction = gKolimaForestBridge_RestoreScaleAction;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, restoreScaleAction);
  record = Object_GetById(0);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(20);
  record = Object_GetById(1);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, PIXELS(2), PIXELS(2));
  Actor_SetActionScript(GARET, restoreScaleAction);
  record = Object_GetById(1);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_EventWait(40);
  record = Object_GetById(2);
  record->velocityY = PIXELS(2);
  Engine_EventWait(10);
  Actor_SetActionScript(IVAN, restoreScaleAction);
  record = Object_GetById(2);
  Engine_ActorSetSpriteFlags(record, 0);
  Engine_ActorSetAnimation(IVAN, 19);
  gKolimaForestBridge_SparkleSoundOn = 0;
  Engine_EventWait(160);
  Engine_TaskRemoveCallback(KolimaForestBridge_SpawnCurseSparkle);
  Engine_EventWait(120);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = 0x800000;
  gFallingEffectState = 1;
  FieldScheduler_AddCallback(FieldScene_UpdateFallingEffect, 0xc80);
  Engine_EventWait(180);
  Event_PlayCue(MUS_021);
  KolimaForestBridge_ShowMessageAndWait(GARET, 80);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 40);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(60);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
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
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
    KolimaForestBridge_ShowMessageAndWait(MIA, 10);
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
  gKolimaForestBridge_BarrierHit = 0;
  FieldScheduler_AddCallback(KolimaForestBridge_FlickerBarriers, 0xc80);
  Event_PlayCue(SE_220);
  Object_GetById(LEADER_BARRIER)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(LEADER_BARRIER, 2);
  Actor_SetPosition(LEADER_BARRIER, PIXELS(253), PIXELS(603));
  growBarrierAction = gKolimaForestBridge_BarrierGrowAction;
  Actor_SetActionScript(LEADER_BARRIER, growBarrierAction);
  Object_GetById(IVAN_BARRIER)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(IVAN_BARRIER, 2);
  Actor_SetPosition(IVAN_BARRIER, PIXELS(233), PIXELS(629));
  Actor_SetActionScript(IVAN_BARRIER, growBarrierAction);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Object_GetById(MIA_BARRIER)->priorityFlags &= 254;
    Engine_ActorSetSpritePriority(MIA_BARRIER, 2);
    Actor_SetPosition(MIA_BARRIER, PIXELS(207), PIXELS(609));
    Actor_SetActionScript(MIA_BARRIER, growBarrierAction);
  }
  Object_GetById(GARET_BARRIER)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(GARET_BARRIER, 2);
  Actor_SetPosition(GARET_BARRIER, PIXELS(227), PIXELS(580));
  Actor_SetActionScript(GARET_BARRIER, growBarrierAction);
  if (gFallingEffectState != 0) {
    do {
      Engine_TaskWait(1);
    } while (gFallingEffectState != 0);
  }
  Engine_EventWait(300);
  Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
  Engine_EventWait(120);
  Event_PlayCue(MUS_017);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_ActorStop(LEADER_BARRIER);
  Engine_ActorStop(IVAN_BARRIER);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Engine_ActorStop(MIA_BARRIER);
  }
  Engine_ActorStop(GARET_BARRIER);
  Engine_TaskWait(1);
  shrinkBarrierAction = gKolimaForestBridge_BarrierShrinkAction;
  Actor_SetActionScript(LEADER_BARRIER, shrinkBarrierAction);
  Actor_SetActionScript(IVAN_BARRIER, shrinkBarrierAction);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Actor_SetActionScript(MIA_BARRIER, shrinkBarrierAction);
  }
  Actor_RunActionScriptAndWait(GARET_BARRIER, shrinkBarrierAction);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  Actor_SetPosition(TRET, PIXELS(220), PIXELS(494));
  Actor_SetPosition(LAUREL, PIXELS(220), PIXELS(494));
  Engine_TaskWait(1);
  if (Engine_EventChooseYesNo(11, 0) == 1) {
    gEventWork->message += 1;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaKnowThoseFieldsWere);
    KolimaForestBridge_ShowMessageAndWait(MIA, 40);
  }
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventSetMessage(MsgKolimaWasOurPsynergy);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Engine_ActorSetSpritePriority(GARET, 2);
  Object_GetById(1)->priorityFlags |= 1;
  record = Object_GetById(1);
  Engine_ActorSetSpriteFlags(record, 1);
  Engine_ActorJump(GARET, 6, 0);
  Actor_SetDestinationOffset(GARET, -3, 0);
  Engine_ActorSetAnimation(GARET, 1);
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 60);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  KolimaForestBridge_ShowMessageAndWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  KolimaForestBridge_FaceActorAndWait(1, 0x2000, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(40);
  KolimaForestBridge_FaceActorAndWait(1, 0x6000, 40);
  KolimaForestBridge_FaceActorAndWait(1, 0x2000, 20);
  KolimaForestBridge_FaceActorAndWait(1, 0x6000, 20);
  KolimaForestBridge_FaceActorAndWait(1, 0x2000, 10);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Engine_EventWait(20);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
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
    KolimaForestBridge_FaceActorAndWait(3, 0xe000, 60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    KolimaForestBridge_ShowMessageAndWait(MIA, 20);
  } else {
    gEventWork->message += 1;
  }
  Engine_ActorJump(GARET, 2, 0);
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  KolimaForestBridge_FaceActorAndWait(1, 0x2000, 10);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
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
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  KolimaForestBridge_FaceActorAndWait(0, 0xa000, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 10);
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
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
    KolimaForestBridge_FaceActorAndWait(1, 0x2000, 10);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessage(GARET, 0);
  }
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 10);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    KolimaForestBridge_FaceActorAndWait(3, 0, 20);
    KolimaForestBridge_FaceActorAndWait(3, 0x2000, 10);
    Engine_ActorSetAnimation(MIA, 4);
    KolimaForestBridge_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message += 1;
  }
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0xa000, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  KolimaForestBridge_FaceActorAndWait(1, 0x4000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(80);
  KolimaForestBridge_FaceActorAndWait(2, 0xe000, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0xa000, 40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  KolimaForestBridge_FaceActorAndWait(0, 0x6000, 10);
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
  Actor_SetAttachedEffect(GARET, EMOTE_IN_FRONT | 2);
  Engine_EventWait(40);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  KolimaForestBridge_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimation(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  KolimaForestBridge_FaceActorAndWait(1, 0x2000, 10);
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
  Event_PlayCue(MUS_021);
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
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 10);
  Engine_EventSetMessage(MsgKolimaWatchOutItsHappeningAgain);
  KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
  KolimaForestBridge_FaceActorAndWait(1, 0xc000, 10);
  KolimaForestBridge_FaceActorAndWait(0, 0xc000, 10);
  if (gKolimaForestBridge_MiaInParty != 0) {
    KolimaForestBridge_FaceActorAndWait(3, 0xc000, 10);
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
  Event_PlayCue(SE_220);
  Actor_SetPosition(LEADER_BARRIER, PIXELS(253), PIXELS(603));
  growBarrierAction = gKolimaForestBridge_BarrierGrowAction;
  Actor_SetActionScript(LEADER_BARRIER, growBarrierAction);
  Actor_SetPosition(IVAN_BARRIER, PIXELS(233), PIXELS(629));
  Actor_SetActionScript(IVAN_BARRIER, growBarrierAction);
  if (gKolimaForestBridge_MiaInParty != 0) {
    Actor_SetPosition(MIA_BARRIER, PIXELS(207), PIXELS(609));
    Actor_SetActionScript(MIA_BARRIER, growBarrierAction);
  }
  Actor_SetPosition(GARET_BARRIER, PIXELS(227), PIXELS(580));
  Actor_SetActionScript(GARET_BARRIER, growBarrierAction);
  Engine_EventWait(120);
  gFallingEffectState = 3;
  do {
    Engine_TaskWait(1);
  } while (gFallingEffectState != 0);
  KolimaForestBridge_ShowMessageAndWait(TRET, 80);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 20);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Engine_EventWait(40);
  KolimaForestBridge_ShowMessageAndWait(TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  KolimaForestBridge_FaceActorAndWait(2, 0xc000, 40);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0xc000, 80);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0, 40);
  KolimaForestBridge_ShowMessageAndWait(TRET, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0xc000, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
  Engine_ActorSetAnimation(GARET, 4);
  Engine_ActorSetAnimation(MIA, 4);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(60);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0, 20);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0, 20);
  KolimaForestBridge_ShowMessageAndWait(TRET, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
  Engine_EventWait(40);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  KolimaForestBridge_FaceActorAndWait(3, 0xc000, 10);
  KolimaForestBridge_ShowMessageAndWait(LAUREL, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(60);
  Event_ShowMessage(LAUREL, 0);
  Event_ShowMessage(TRET, 0);
  Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
  Engine_EventWait(80);
  ColorBuffer_ApplyTarget(0x10000, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(80);
  Engine_ActorStop(LEADER_BARRIER);
  Engine_ActorStop(IVAN_BARRIER);
  miaInParty = &gKolimaForestBridge_MiaInParty;
  Engine_ActorStop(MIA_BARRIER);
  Engine_ActorStop(GARET_BARRIER);
  Engine_TaskWait(1);
  shrinkBarrierAction = gKolimaForestBridge_BarrierShrinkAction;
  Actor_SetActionScript(LEADER_BARRIER, shrinkBarrierAction);
  Actor_SetActionScript(IVAN_BARRIER, shrinkBarrierAction);
  if (*miaInParty != 0) {
    Actor_SetActionScript(MIA_BARRIER, shrinkBarrierAction);
  }
  Actor_RunActionScriptAndWait(GARET_BARRIER, shrinkBarrierAction);
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
  KolimaForestBridge_FaceActorAndWait(2, 0xe000, 10);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      KolimaForestBridge_FaceActorAndWait(3, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
      Engine_EventWait(40);
      KolimaForestBridge_FaceActorAndWait(1, 0x4000, 20);
      KolimaForestBridge_ShowMessageAndWait(GARET, 10);
      KolimaForestBridge_FaceActorAndWait(2, 0xc000, 20);
      KolimaForestBridge_FaceActorAndWait(2, 0xe000, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
      KolimaForestBridge_FaceActorAndWait(1, 0x2000, 20);
    } else {
      KolimaForestBridge_FaceActorAndWait(3, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 0);
      Engine_EventWait(40);
      KolimaForestBridge_FaceActorAndWait(1, 0x4000, 20);
      Engine_EventSetMessage(MsgKolimaHesAsStumpedAsWe);
      KolimaForestBridge_ShowMessageAndWait(GARET, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ActorSetAnimation(MIA, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaYoureIsaacThereIsntMuch);
    KolimaForestBridge_ShowMessageAndWait(GARET, 10);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    KolimaForestBridge_FaceActorAndWait(0, 0x6000, 20);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Event_OpenMessage(IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 3, 0);
      Engine_EventWait(40);
      KolimaForestBridge_FaceActorAndWait(2, 0xe000, 10);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
      if (*miaInParty != 0) {
        KolimaForestBridge_FaceActorAndWait(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 3);
        KolimaForestBridge_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      KolimaForestBridge_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
      Engine_EventWait(120);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestBridge_MiaInParty != 0) {
        KolimaForestBridge_FaceActorAndWait(3, 0x2000, 10);
        Engine_ActorSetAnimationAndWait(MIA, 4);
        KolimaForestBridge_ShowMessageAndWait(MIA, 10);
      } else {
        gEventWork->message += 1;
      }
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      if (gKolimaForestBridge_MiaInParty != 0) {
        KolimaForestBridge_FaceActorAndWait(2, 0xa000, 40);
        KolimaForestBridge_FaceActorAndWait(2, 0xe000, 20);
      }
      KolimaForestBridge_ShowMessageAndWait(IVAN, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(40);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
      Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
      Engine_ActorSetAnimationAndWait(GARET, 3);
      Engine_EventWait(20);
      Engine_ActorSetAnimation(MIA, 3);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
    } else {
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
      Engine_EventWait(40);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Engine_EventSetMessage(MsgKolimaThatsReliefIsaacThoughtYoud);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
      if (*miaInParty != 0) {
        KolimaForestBridge_FaceActorAndWait(3, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 1);
        KolimaForestBridge_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message += 1;
      }
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      KolimaForestBridge_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
      Engine_EventWait(80);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 40);
      if (gKolimaForestBridge_MiaInParty != 0) {
        KolimaForestBridge_FaceActorAndWait(3, 0x2000, 20);
        Engine_ActorSetAnimation(MIA, 4);
        KolimaForestBridge_ShowMessageAndWait(MIA, 40);
      } else {
        gEventWork->message += 1;
      }
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventWait(20);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(40);
      KolimaForestBridge_ShowMessageAndWait(IVAN, 20);
    }
  }
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
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
  if (gKolimaForestBridge_MiaInParty != 0) {
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
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Actor_SetPosition(TRET, 0, 0);
  Actor_SetPosition(LAUREL, 0, 0);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

// Show the next line, then leave time for the next beat.
static void KolimaForestBridge_ShowMessageAndWait(s32 speaker, s32 frames) {
  Engine_EventShowMessage(speaker, 0);
  Engine_EventWait(frames);
}

// Turn an actor at once, then leave time for the next beat.
static void KolimaForestBridge_FaceActorAndWait(s32 actorId, s32 facing,
                                                s32 waitFrames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(waitFrames);
}

// Shared source is included here in ROM order.
#include "common/kolima_grow.h"

// Every eighth frame a sparkle of Tret's glamour (object 26) falls toward
// the party, with its tinkle while the rain is at its height.
static void KolimaForestBridge_SpawnCurseSparkle(void) {
  struct FieldActor *sparkle;
  struct AnimationObject *sprite;
  s32 framePhase;
  s32 priorityFlags;
  s32 spawnX = PIXELS(231);
  s32 spawnZ = PIXELS(460);
  s32 targetX = PIXELS(231);
  s32 targetZ = PIXELS(624);

  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    return;
  if (gKolimaForestBridge_SparkleSoundOn != 0)
    Engine_AudioPlayCue(SE_200);
  sparkle = Engine_ObjectCreate(26, spawnX, 0, spawnZ);
  if (sparkle == 0)
    return;
  sprite = sparkle->sprite;
  sprite->flags = framePhase;
  priorityFlags = 0xff & ~ACTOR_PRIORITY_AUTOMATIC;
  priorityFlags &= sparkle->priorityFlags;
  sparkle->priorityFlags = priorityFlags;
  sprite->image.priority = 1;
  sparkle->scaleX = Q16_ONE / 10;
  sparkle->speed = PIXELS(8);
  sparkle->acceleration = PIXELS(8);
  sparkle->motionFlags = framePhase;
  Object_SetMode(sparkle, 2);
  Engine_ObjectSetPosition(sparkle, targetX, 0, targetZ);
  Engine_ObjectSetScript(sparkle, gKolimaForestBridge_SparkleScript);
}

// Shared source is included here in ROM order.
#include "common/kolima_blink.h"

// The end of a sparkle's fall: one that lands on a dome (the domes' area,
// which differs with Mia) bursts with a sound, and the domes are told.
static s32
KolimaForestBridge_BurstOnBarrier(struct ScriptInterpreter *interpreter) {
  struct FieldActor *arg0 = (struct FieldActor *)interpreter;
  s32 x;

  if (!(gKolimaForestBridge_MiaInParty != 0
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
  Engine_AudioPlayCue(SE_106);
#ifdef UBFIX
  Engine_ObjectSetScript(arg0, gKolimaForestBridge_SparkleBurstScript);
#else
  /* FAKEMATCH: called through a cast to the call type the bytes show; called by
   * its prototype or through Object_SetScript, r0 is set before the script
   * literal loads (2 differ) */
  ((s32 (*)())Engine_ObjectSetScript)(
      (s32)arg0, (s32)gKolimaForestBridge_SparkleBurstScript);
#endif
  gKolimaForestBridge_BarrierHit = 1;
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_forest_bridge_falling_effect.h"
s32 gKolimaForestBridge_BarrierHit __attribute__((section(".bss")));
s32 gKolimaForestBridge_MiaInParty __attribute__((section(".bss")));
s32 gKolimaForestBridge_SparkleSoundOn __attribute__((section(".bss")));

// Each frame the domes flicker: two show on even frames and two on odd,
// the hidden ones lifted 500 pixels out of sight.
static void KolimaForestBridge_FlickerBarriers(void) {
  struct FieldActor *p;

  p = Object_GetById(LEADER_BARRIER);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(IVAN_BARRIER);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(MIA_BARRIER);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) == 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
    }
  }
  p = Object_GetById(GARET_BARRIER);
  if (p != 0) {
    p->motionFlags = 0;
    if ((gFrameCount & 1) != 0) {
      p->y.fixed = 0;
    } else {
      p->y.fixed = PIXELS(500);
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

// Garet, Ivan and Mia walk to their places beside the leader.
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

// The glamour presses the party down: each squashes and wobbles until
// released.
s32 gKolimaForestBridge_SquashAction[] = {
    SC_ADD(SW_SCALE_X, -0x4000), SC_ADD(SW_SCALE_Y, 0x2000),  SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2000),  SC_ADD(SW_SCALE_Y, -0x1000), SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1000),  SC_ADD(SW_SCALE_Y, -2048),   SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolimaForestBridge_RestoreScaleAction[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

// A dome grows from a tenth of its size over 70 frames.
s32 gKolimaForestBridge_BarrierGrowAction[] = {
    SC_SET(SW_SCALE_X, 0x1999),
    SC_SET(SW_SCALE_Y, 0x1999),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 1310),
    SC_ADD(SW_SCALE_Y, 1310),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(KolimaForestBridge_FlashHitBarriers),
    SC_END,
};

// A dome sinks and shrinks away over 90 frames.
s32 gKolimaForestBridge_BarrierShrinkAction[] = {
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

s32 gKolimaForestBridge_GroundSparkleAction[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_SET(SW_COLLISION, 0),
    SC_SET(0x23, 0),
    SC_LABEL(1),
    SC_CALL(KolimaForestBridge_TwinkleGroundSparkle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gKolimaForestBridge_Entrances[] = {
    {0, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 172, 0, 602, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// Exit 1 to the clearing (scene 41) and exit 4 to the world map (scene 2).
// Exits 2 and 3, which no event uses, lead into this map (scene 40).
u32 gKolimaForestBridge_Exits[] = {
    SCENE_EXITS(40),      SCENE_EXIT(1, 41, 1), SCENE_EXIT(2, 40, 1),
    SCENE_EXIT(3, 40, 1), SCENE_EXIT(4, 2, 8),  SCENE_EXITS_END,
};

// Garet, Ivan and Mia for the curse; the three logs; Tret and Laurel; the
// four domes; and the five ground sparkles.
// clang-format off
struct ScenePlacement gKolimaForestBridge_Placements[] = {
    { 1, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 2, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 3, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcf, CONDITION_ALWAYS, 7,
        PIXELS(208), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(360), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x37, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x38, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdc, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdc, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdc, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdc, CONDITION_ALWAYS, ACTOR_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, CONDITION_ALWAYS, 7,
        PIXELS(232), 0, PIXELS(655), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x11d, CONDITION_ALWAYS, 7,
        PIXELS(166), 0, PIXELS(576), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, CONDITION_ALWAYS, 7,
        PIXELS(189), 0, PIXELS(532), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, CONDITION_ALWAYS, 7,
        PIXELS(288), 0, PIXELS(570), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, CONDITION_ALWAYS, 7,
        PIXELS(331), 0, PIXELS(659), FACING_SOUTH, TALK_FACE_PARTY,
        PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestBridge_Events[] = {
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBridge_PushLog)},
    {TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV11_TOUCH)},
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
    {SCENE_EVENTS_END},
};

s32 gKolimaForestBridge_SparkleScript[] = {
    SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

// A sparkle bursts on a dome: it doubles in size and is gone.
s32 gKolimaForestBridge_SparkleBurstScript[] = {
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
    SC_CALL(KolimaForestBridge_BurstOnBarrier),
    SC_FREE,
};
