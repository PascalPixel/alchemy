#include "script.h"
#include "flags.h"
#include "character_ids.h"
#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/scene/kolima.h"
#include "unit/scene/common/kolima_forest_bridge_falling_effect.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "kolima.h"
#include "object_runtime.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"
#include "fixed_math.h"
#include "palbuf.h"
#include "field_runtime.h"
#include "songs.h"
#include "item_ids.h"

// Forward declarations.
static void Kolima_WitnessForestCurse(void);
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames);
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames);
static void Kolima_SpawnCurseSparkle(void);
static void Kolima_RestoreBridgeState(void);

/// Apply the deferred pose change requested by the falling-effect script.
static s32 Kolima_ApplyPendingPose(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (gKolima_PendingPose != 0) {
    Object_SetMode(actor, 2);
    gKolima_PendingPose = 0;
  }
  return 1;
}

/// Alternate an ambient actor's child appearance with a random timer.
static s32 Kolima_UpdateTimedAppearance(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  s32 randomValue = Engine_RandomNext();
  s32 scaledRandom = randomValue * 100;
  s32 appearanceTimer = (u16)actor->timer + ((u32)scaledRandom >> 16);
  actor->timer = appearanceTimer;
  if ((s16)appearanceTimer > 1000) {
    ObjectGroup_SetChildValue(actor, 7);
  } else {
    ObjectGroup_SetChildValue(actor, 10);
  }
  if ((s16)actor->timer > 1200) {
    actor->timer = 0;
  }
  return 1;
}

/// Replace the nine villagers' placement sprites with their cursed trees.
static void
Kolima_InitializeCursedVillagers(struct ScenePlacement *placements) {
  struct ScenePlacement *villager = &placements[3];
  u32 villagerIndex;
  s32 treeSprite;
  s32 alternateTreeSprite;

  villagerIndex = 0;
  treeSprite = 105;
  alternateTreeSprite = 110;
  for (; villagerIndex <= 8; villagerIndex++) {
    villager->sprite = treeSprite;
    if ((villagerIndex - 6) <= 1) {
      villager->sprite = alternateTreeSprite;
    }
    villager->talkFacing = TALK_KEEP_FACING;
    villager->behavior = ACTOR_STAND;
    villager++;
  }
}

/// Move, enlarge and release one bridge fragment after its lifetime.
static s32 Kolima_UpdateFallingDebris(struct ScriptInterpreter *interpreter) {
  struct FieldActor *debris = (struct FieldActor *)interpreter;
  debris->x.fixed += debris->velocityX;
  debris->z.fixed += debris->velocityZ;
  debris->velocityZ -= PIXELS(1) / 25;
  debris->scaleX += 3 * Q16_ONE / 128;
  debris->scaleY += 3 * Q16_ONE / 128;
  {
    s32 framesLeft = (u16)debris->timer - 1;
    debris->timer = framesLeft;
    if ((u16)framesLeft == 0) {
      Engine_ObjectDispatchRelease(debris);
    }
  }
  return 1;
}

// Shared source is included here in ROM order.
#include "common/object_facing.h"

/// Select the entrance table for the village, interior or bridge.
const struct SceneEntrance *Kolima_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kolima3) {
    return gKolimaEntrances3;
  }
  if (scene == (s32)&SceneId_Kolima2) {
    return gKolimaEntrances2;
  }
  return gKolimaEntrances;
}

/// Return the interior's regions; the other maps have none.
const struct SceneRegion *Kolima_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_Kolima2) {
    return gKolimaRegions2;
  }
  return 0;
}

/// Return the shared packed exit table.
const u32 *Kolima_GetExits(void) { return gKolimaExits; }

/// Select placements and apply tree sprites while the villagers are cursed.
const struct ScenePlacement *Kolima_GetPlacements(void) {
  s32 scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kolima1) {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Kolima_InitializeCursedVillagers(gKolimaPlacements1);
    }
    return gKolimaPlacements1;
  }
  if (scene == (s32)&SceneId_Kolima3) {
    return gKolimaPlacements3;
  }
  return gKolimaPlacements;
}

/// Offer sanctum services to a north-facing leader, or discuss the trees.
static void Kolima_TalkToHealer(void) {
  struct FieldActor *leader = Object_GetById(0);
  s32 facing = leader->facing;
  Engine_EventBegin();
  if (FACING_IS_NORTH(facing)) {
    Engine_SanctumOpen(16);
  } else {
    Engine_EventSetMessage(MsgKolimaToldHolyTrees);
    Engine_EventAskYesNo(16, 0);
  }
  Engine_EventEnd();
}

/// Let the village's Djinni join the party.
static void Kolima_RecruitDjinni(void) { Djinn_JoinParty(27, 0, 1); }

/// Select the village or bridge event table.
const struct SceneEvent *Kolima_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_Kolima3) {
    return gKolimaEvents3;
  }
  return gKolimaEvents;
}

/// Walk the leader northward through the selected exit.
static void Kolima_WalkThroughExit(s32 exitId) {
  Object_GetById(0)->motionFlags = 0;
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
  gEventWork->transitionFrames = 16;
  Engine_EventRequestExit(exitId);
}

/// Animate the touched map cells or leave through the southern exit.
static void Kolima_AnimateSwitchOrExit(void) {
  struct EventWork *work = gEventWork;
  s32 x = 0;
  s32 y = 0;

  Engine_EventBegin();
  Engine_AudioPlayCue(SE_DOOR_OPEN);
  switch (work->touchedTrigger) {
  case 5:
    x = 71;
    y = 9;
    break;
  case 6:
    x = 73;
    y = 17;
    break;
  case 7:
    x = 80;
    y = 21;
    break;
  case 8:
    x = 84;
    y = 12;
    break;
  case 9:
    Object_GetById(0)->motionFlags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1) / 2, PIXELS(1) / 4);
    Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 8);
    gEventWork->transitionFrames = 16;
    Engine_EventRequestExit(9);
    Engine_EventEnd();
    return;
  }
  Engine_MapAnimateCells(gKolima_SwitchCells, x, y);
  Kolima_WalkThroughExit(work->touchedTrigger);
  Engine_EventEnd();
}

/// Show the villager's crossing and the bridge collapsing into debris.
static void Kolima_ShowBridgeCollapse(void) {
  u32 debrisIndex;

  Engine_EventBegin();
  Camera_SetSpeed(PIXELS(2), PIXELS(1) / 4);
  Camera_MoveTo(PIXELS(168), 0, PIXELS(246), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 40);
  Engine_ActorRunRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgKolimaGoingCrossBridge);
  Engine_EventShowMessage(8, 0);
  Engine_CameraSetSpeed(PIXELS(2) / 5, PIXELS(1) / 20);
  Camera_MoveTo(PIXELS(168), 0, PIXELS(234), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 174, 0x116);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(8, 3);
  Engine_EventShowMessage(8, 0);
  Actor_FaceDirection(8, FACING_WEST + FACING_STEP, 20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Object_GetById(8)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(8, PIXELS(2), PIXELS(1));
  Engine_ActorJump(8, 2, 0);
  Engine_ObjectMotionSetPositionAndCommit(8, 224, 197);
  Engine_AudioPlayCue(SE_176);
  Engine_EventWait(10);
  Engine_ObjectMotionSetPositionAndCommit(8, 234, 200);
  Engine_EventWait(10);
  Engine_AudioPlayCue(SE_198);
  Engine_EventWait(30);
  // Animate the bridge in progressively shorter collapse steps.
  Map_CopyCellsTo(91, 0, 72, 9, 5, 4);
  Engine_EventWait(12);
  Map_CopyCellsTo(91, 4, 72, 9, 5, 4);
  Engine_EventWait(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Map_CopyCellsTo(91, 8, 72, 9, 5, 5);
  Engine_EventWait(6);
  Map_CopyCellsTo(91, 13, 72, 9, 5, 6);
  Engine_EventWait(3);
  Engine_AudioPlayCue(SE_188);
  // Scatter ten fragments with alternating horizontal velocities.
  for (debrisIndex = 0; debrisIndex < 10; debrisIndex++) {
    struct FieldActor *debris =
        Engine_ObjectCreate(222, PIXELS(148 + debrisIndex * 4), 0, PIXELS(258));

    if (debris != 0) {
      debris->motionFlags = 0;
      Engine_ActorSetSpriteFlags(debris, 0);
      debris->sprite->image.priority = 0;
      {
        u16 delay = (((u32)(Engine_RandomNext() * 40)) >> 16) + 40;

        debris->timer = delay;
      }
      debris->velocityX = (s32)((PIXELS(debrisIndex & 3) + PIXELS(1))) >> 1;
      debris->velocityZ = PIXELS(1);
      if (debrisIndex & 1) {
        debris->velocityX = -((s32)(((debrisIndex & 3) << 16) + 0x10000) >> 1);
      }
      Object_SetMode(debris, 1);
      Engine_ObjectSetScript(debris, gKolima_DebrisScript);
    }
  }
  Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
  Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
  Map_SetShake(0, PIXELS(4), Q16_ONE);
  Engine_EventWait(10);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 0);
  Engine_EventWait(20);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  // From now on the villager on the far side turns to keep facing the leader.
  FieldObject_SetTargetAndCallback(
      8, ACTOR_LINK_KEEP_MOTION | ACTOR_PARTY_LEADER, gKolima_FaceLeaderScript);
  Engine_EventWait(60);
  Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 0x102, 60);
  Engine_GameFlagSet(FLAG_KOLIMA_BRIDGE_COLLAPSED);
  Engine_EventEnd();
}

/// Restore the current map and play the first visit to cursed Kolima.
s32 Kolima_Initialize(void) {
  s16 scene;
  u32 actor;
  const s32 *table;

  scene = gGameState.scene;

  if (scene == (s32)&SceneId_Kolima3) {
    Kolima_RestoreBridgeState();
    return 0;
  }

  if (scene == (s32)&SceneId_Kolima2) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    return 0;
  }

  Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(25), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(26), 0);

  table = gKolima_EntryActions;
  Actor_SetActionScript(23, table);
  Actor_SetActionScript(24, table);
  Actor_SetActionScript(25, table);
  Actor_SetActionScript(26, table);

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    for (actor = 8; actor <= 16; actor++) {
      Engine_ActorSetSpriteFlags(Object_GetById(actor), 0);
    }
    Map_CopyCellAttributes(13, 9, 1, 1, 13, 8);
    Map_CopyCellAttributes(13, 9, 1, 1, 15, 8);
    Map_CopyCellAttributes(13, 9, 1, 1, 14, 9);
  }

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) == 0) {
    if (gGameState.entrance == 1) {
      Kolima_WitnessForestCurse();
    }
  }

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_WITNESSED) != 0) {
    Engine_ActorDestroy(GARET);
    Engine_ActorDestroy(IVAN);
    Engine_ActorDestroy(MIA);
    Engine_ActorDestroy(17);
    Engine_ActorDestroy(18);
    Engine_ActorDestroy(19);
    Engine_ActorDestroy(20);
    Engine_ActorDestroy(21);
    Engine_ActorDestroy(22);
    Event_CallWithLastActiveObjectId(gKolima_EntryScript);
  }

  return 0;
}

/// Show a protective field on its assigned alternating frame.
static __inline__ void Kolima_SetProtectiveFieldHeight(s32 actorId,
                                                       s32 visibleOnOddFrames) {
  struct FieldActor *field = Object_GetById(actorId);
  if (field != NULL) {
    field->motionFlags = 0;
    if (visibleOnOddFrames ? (gFrameCount & 1) : !(gFrameCount & 1)) {
      field->y.fixed = 0;
    } else {
      field->y.fixed = PIXELS(500);
    }
  }
}

/// Alternate the four protective sprites between visible and hidden heights.
static void Kolima_AlternateProtectiveFieldHeights(void) {
  Kolima_SetProtectiveFieldHeight(19, 0);
  Kolima_SetProtectiveFieldHeight(20, 1);
  Kolima_SetProtectiveFieldHeight(21, 0);
  Kolima_SetProtectiveFieldHeight(22, 1);
}

/// Begin Mind Read on one of the villagers transformed into a tree.
static void Kolima_ReadVillagerThoughts(s32 actorId, s32 targetId) {
  Engine_PsynergyBegin(ABILITY_MIND_READ, 1);
  Engine_PsynergySetTarget(actorId, targetId);
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_TaskWait(1);
}

/// Finish Mind Read and release its temporary scene objects.
static void Kolima_FinishReadingThoughts(void) {
  Engine_PsynergyPlayEffect(2);
  Engine_PsynergyLowerHands();
  BattleEffect_CleanupSceneObjects();
}

/// Give the four companions the same nod, waiting on the final actor.
static __inline__ void Kolima_NodTogether(void) {
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(MIA, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
}

/// Show the forest's curse, the party's protective Psynergy and the trees'
/// appeal.
static void Kolima_WitnessForestCurse(void) {
  struct FieldActor *actor;
  s32 *partyLayout;
  s32 motionFacingFlag;
  // One staging word is reused for facing bytes, jump values and flags.
  s32 stagingValue;
  const s32 *squashActions;
  const s32 *restoreScaleActions;
  u32 *firstCurtainState;
  s32 *sceneState;
  const s32 *growFieldActions;
  const s32 *shrinkFieldActions;
  u32 *secondCurtainState;

  // Enter the deserted village and let Ivan inspect one of its trees.
  Engine_EventBegin();
  partyLayout = &gKolima_MiaInParty;
  *partyLayout = GameFlag_IsSet(FLAG_IN_PARTY(MIA));
  actor = Object_GetById(19);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(20);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(21);
  Engine_ActorSetSpriteFlags(actor, 0);
  actor = Object_GetById(22);
  Engine_ActorSetSpriteFlags(actor, 0);
  Camera_MoveTo(PIXELS(104), -1, PIXELS(256), 0);
  Engine_MapRedraw();
  Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(23), PIXELS(247));
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
  Actor_SetSpeed(GARET, PIXELS(3) / 5, PIXELS(3) / 10);
  Actor_SetSpeed(IVAN, PIXELS(3) / 5, PIXELS(3) / 10);
  actor = Object_GetById(0);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  actor = Object_GetById(0);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  Actor_SetActionScript(GARET, gKolima_ActionTable1);
  Actor_SetActionScript(IVAN, gKolima_ActionTable2);
  if (*partyLayout != 0) {
    Actor_SetSpeed(MIA, PIXELS(3) / 5, PIXELS(3) / 10);
    actor = Object_GetById(0);
    if (actor != NULL) {
      Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
    }
    Actor_SetActionScript(MIA, gKolima_ActionTable3);
  }
  Object_RefreshSelectorById(2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 20);
  partyLayout = &gKolima_MiaInParty;
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_NORTHEAST, 0);
  }
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Actor_FaceActor(GARET, ACTOR_PARTY_LEADER, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  }
  Engine_EventWait(40);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_EventSetMessage(MsgKolimaQuiet);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(30);
  Actor_WalkToAndWait(IVAN, 72, 0x11e);
  Actor_WalkToAndWait(IVAN, 72, 0x12e);
  Actor_WalkToAndWait(IVAN, 88, 0x136);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Actor_FaceActor(ACTOR_PARTY_LEADER, IVAN, 0);
  Actor_FaceActor(GARET, IVAN, 0);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, IVAN, 0);
  }
  Engine_EventWait(30);
  Engine_ActorSetAnimation(GARET, 3);
  if (*partyLayout != 0) {
    Engine_ActorSetAnimation(MIA, 3);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(30);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Engine_EventWait(20);
  // Mind Read reveals that the trees are the missing villagers.
  Kolima_ReadVillagerThoughts(IVAN, 9);
  Engine_EventWait(40);
  Kolima_FinishReadingThoughts();
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 40);
  Actor_SetSpeed(IVAN, PIXELS(1), PIXELS(1) / 2);
  Object_GetById(IVAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkToAndWait(IVAN, 80, 0x136);
  motionFacingFlag = ACTOR_FACES_MOTION;
  Engine_EventWait(1);
  Object_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 40);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (*partyLayout != 0) {
    Actor_FaceActor(MIA, ACTOR_PARTY_LEADER, 0);
  }
  Engine_ActorFaceEachOther(IVAN, GARET, 60);
  if (*partyLayout != 0) {
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
  }
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Kolima_ShowMessageAndWait(GARET, 20);
  Actor_SetSpeed(IVAN, PIXELS(1) / 2, PIXELS(1) / 4);
  Object_GetById(IVAN)->steeringFlags &= 254;
  Actor_WalkToAndWait(IVAN, 72, 0x11e);
  Engine_EventWait(1);
  Object_GetById(IVAN)->steeringFlags |= motionFacingFlag;
  Actor_SetActionScript(IVAN, gKolima_ActionTable2);
  if (*partyLayout != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 0);
    Engine_EventWait(60);
    Kolima_ShowMessageAndWait(MIA, 20);
    Engine_ActorSetAnimation(MIA, 3);
  } else {
    gEventWork->message++;
  }
  Actor_FaceActor(IVAN, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(10);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Event_OpenMessage(GARET, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimation(GARET, 3);
  } else {
    Engine_ActorSetAnimationAndWait(GARET, 4);
    gEventWork->message++;
  }
  Kolima_ShowMessageAndWait(GARET, 40);
  Kolima_FaceActorAndWait(IVAN, FACING_SOUTHEAST, 40);
  Kolima_FaceActorAndWait(IVAN, FACING_WEST, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_SOUTH, 40);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  stagingValue = 160;
  Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, (stagingValue << 8), 10);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventSetMessage(MsgKolimaWhatIsIt);
  Kolima_ShowMessageAndWait(GARET, 10);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Kolima_FaceActorAndWait(GARET, 0, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, (stagingValue << 8), 40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 30);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 30);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Engine_EventWait(40);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Event_PlayCue(MUS_017);
  Event_PlayCue(SE_206);
  ColorBuffer_ApplyTarget(0x7fff, 0);
  Engine_ColorBufferInterpolate(1);
  Engine_TaskWait(1);
  // The falling sparkles threaten to turn the party into trees as well.
  gKolima_EffectActive = 1;
  FieldScheduler_AddCallback(Kolima_SpawnCurseSparkle, 0xc80);
  Engine_TaskWait(20);
  ColorBuffer_ApplyTarget(0x405210, 1);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 2);
  Engine_ColorBufferInterpolate(120);
  Engine_TaskWait(60);
  squashActions = gKolima_PartySquashActions;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, squashActions);
  Actor_SetActionScript(GARET, squashActions);
  Actor_SetActionScript(IVAN, squashActions);
  Actor_SetActionScript(MIA, squashActions);
  Engine_EventWait(100);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_ShowMessageAndWait(IVAN, 40);
  if (gKolima_MiaInParty != 0) {
    Engine_EventWait(40);
    Actor_SetAttachedEffect(MIA, 0x102);
    Engine_EventWait(40);
    Kolima_ShowMessageAndWait(MIA, 40);
  } else {
    gEventWork->message++;
  }
  Engine_EventWait(20);
  if (gKolima_MiaInParty != 0) {
    stagingValue = 128;
    actor = Object_GetById(3);
    actor->velocityY = (stagingValue << 10);
    Engine_EventWait(10);
    Actor_SetSpeed(MIA, (stagingValue << 10), (stagingValue << 10));
    Actor_SetDestinationOffset(MIA, -2, 0);
    Actor_SetActionScript(MIA, gKolima_PartyRestoreScaleActions);
    actor = Object_GetById(3);
    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorSetAnimation(MIA, 19);
    Engine_EventWait(10);
  }
  stagingValue = 128;
  actor = Object_GetById(0);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, (stagingValue << 10),
                 (stagingValue << 10));
  restoreScaleActions = gKolima_PartyRestoreScaleActions;
  Actor_SetActionScript(ACTOR_PARTY_LEADER, restoreScaleActions);
  actor = Object_GetById(0);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
  Engine_EventWait(20);
  actor = Object_GetById(1);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetSpeed(GARET, (stagingValue << 10), (stagingValue << 10));
  Actor_SetActionScript(GARET, restoreScaleActions);
  actor = Object_GetById(1);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(GARET, 19);
  Engine_EventWait(40);
  actor = Object_GetById(2);
  actor->velocityY = (stagingValue << 10);
  Engine_EventWait(10);
  Actor_SetActionScript(IVAN, restoreScaleActions);
  actor = Object_GetById(2);
  Engine_ActorSetSpriteFlags(actor, 0);
  Engine_ActorSetAnimation(IVAN, 19);
  gKolima_EffectActive = 0;
  Engine_EventWait(160);
  Engine_TaskRemoveCallback(Kolima_SpawnCurseSparkle);
  Engine_EventWait(120);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  // Bring the falling curtain over the party and reveal their protective
  // fields.
  gFallingEffectWidth = 0;
  firstCurtainState = &gFallingEffectState;
  gFallingEffectOffset = PIXELS(128);
  *firstCurtainState = 1;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, 0xc80);
  Engine_EventWait(180);
  Event_PlayCue(MUS_021);
  Kolima_ShowMessageAndWait(GARET, 80);
  Kolima_ShowMessageAndWait(IVAN, 40);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(IVAN, 20);
  *firstCurtainState = 2;
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
  Actor_SetAttachedEffect(GARET, 0x102);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_SetAttachedEffect(MIA, 0x102);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message++;
  }
  sceneState = (s32 *)&gFallingEffectState;
  *sceneState = 3;
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(1)->priorityFlags &= 254;
  Object_GetById(2)->priorityFlags &= 254;
  Object_GetById(3)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  stagingValue = 0;
  Engine_ActorSetSpritePriority(MIA, 3);
  gKolima_PendingPose = stagingValue;
  FieldScheduler_AddCallback(Kolima_AlternateProtectiveFieldHeights, 0xc80);
  Event_PlayCue(SE_220);
  Object_GetById(19)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(19, 2);
  Actor_SetPosition(19, PIXELS(120), PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  Actor_SetActionScript(19, growFieldActions);
  Object_GetById(20)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(20, 2);
  Actor_SetPosition(20, PIXELS(100), PIXELS(274));
  Actor_SetActionScript(20, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Object_GetById(21)->priorityFlags &= 254;
    Engine_ActorSetSpritePriority(21, 2);
    Actor_SetPosition(21, PIXELS(74), PIXELS(254));
    Actor_SetActionScript(21, growFieldActions);
  }
  Object_GetById(22)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(22, 2);
  Actor_SetPosition(22, PIXELS(94), PIXELS(225));
  Actor_SetActionScript(22, growFieldActions);
  if (*sceneState != 0) {
    do {
      Engine_TaskWait(1);
    } while (gFallingEffectState != 0);
  }
  Engine_EventWait(300);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(120);
  Event_PlayCue(MUS_017);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  Engine_ActorStop(19);
  Engine_ActorStop(20);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorStop(21);
  }
  Engine_ActorStop(22);
  Engine_TaskWait(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  Actor_SetActionScript(19, shrinkFieldActions);
  Actor_SetActionScript(20, shrinkFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetActionScript(21, shrinkFieldActions);
  }
  Actor_RunActionScriptAndWait(22, shrinkFieldActions);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Engine_EventWait(40);
  Event_OpenMessage(GARET, 0);
  Actor_SetPosition(17, PIXELS(87), PIXELS(139));
  Actor_SetPosition(18, PIXELS(87), PIXELS(139));
  Engine_TaskWait(1);
  if (Engine_EventChooseYesNo(17, 0) == 1) {
    gEventWork->message++;
  }
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaKnowThoseFieldsWere);
    Kolima_ShowMessageAndWait(MIA, 40);
  }
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(80);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  // The companions discover that their Psynergy formed the barriers.
  Engine_EventSetMessage(MsgKolimaWasOurPsynergy);
  Kolima_ShowMessageAndWait(IVAN, 40);
  Engine_ActorRunRepeatedMotion(GARET, 3);
  Engine_EventWait(40);
  Engine_ActorSetSpritePriority(GARET, 2);
  Object_GetById(1)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  actor = Object_GetById(1);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(GARET, 6, 0);
  Engine_ActorSetAnimation(GARET, 1);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 60);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Kolima_ShowMessageAndWait(GARET, 10);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 40);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHWEST, 20);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(40);
  Engine_ActorJump(GARET, 2, 0);
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 4, 0);
  Engine_EventWait(20);
  Kolima_ShowMessageAndWait(GARET, 20);
  if (gKolima_MiaInParty != 0) {
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(80);
    Engine_ActorSetSpritePriority(MIA, 2);
    Object_GetById(3)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
    actor = Object_GetById(3);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_ActorJump(MIA, 4, 0);
    Actor_SetDestinationOffset(MIA, -2, 0);
    Engine_ActorSetAnimation(MIA, 1);
    Kolima_FaceActorAndWait(MIA, FACING_NORTHEAST, 60);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    Kolima_ShowMessageAndWait(MIA, 20);
  } else {
    gEventWork->message++;
  }
  Engine_ActorJump(GARET, 2, 0);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(20);
  stagingValue = 1;
  Engine_ActorSetSpritePriority(IVAN, 2);
  Object_GetById(2)->priorityFlags |= stagingValue;
  actor = Object_GetById(2);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(IVAN, 4, 0);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_EventWait(10);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Object_GetById(0)->priorityFlags |= stagingValue;
  actor = Object_GetById(0);
  Engine_ActorSetSpriteFlags(actor, 1);
  Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 60);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 5, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
  Engine_EventWait(60);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Kolima_ShowMessageAndWait(IVAN, 20);
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
    gEventWork->message++;
  } else {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Event_ShowMessage(GARET, 0);
  }
  Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 10);
  Engine_ActorSetAnimationAndWait(GARET, 4);
  Kolima_ShowMessageAndWait(GARET, 20);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Kolima_ShowMessageAndWait(IVAN, 10);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Kolima_FaceActorAndWait(MIA, 0, 20);
    Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimation(MIA, 4);
    Kolima_ShowMessageAndWait(MIA, 10);
  } else {
    gEventWork->message++;
  }
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  stagingValue = 128;
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Kolima_FaceActorAndWait(GARET, (stagingValue << 7), 10);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 80);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Engine_ActorStartRepeatedMotion(IVAN, 2);
  Kolima_ShowMessageAndWait(IVAN, 20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 40);
  Actor_FaceDirection(GARET, (stagingValue << 7), 0);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimationAndWait(IVAN, 3);
  Engine_EventWait(20);
  Actor_SetAttachedEffect(GARET, 0x102);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(GARET, 20);
  Engine_ActorSetAnimation(IVAN, 3);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Engine_ActorRunRepeatedMotion(GARET, 2);
  Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 10);
  Event_OpenMessage(GARET, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(GARET, 3);
  } else {
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(40);
    gEventWork->message++;
  }
  Event_ShowMessage(GARET, 0);
  Event_PlayCue(MUS_021);
  ColorBuffer_ApplyTarget(0x406218, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(60);
  gFallingEffectWidth = 0;
  gFallingEffectOffset = PIXELS(128);
  secondCurtainState = &gFallingEffectState;
  *secondCurtainState = 1;
  FieldScheduler_AddCallback(Kolima_UpdateCurseDust, 0xc80);
  Engine_EventWait(80);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Engine_EventWait(60);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 10);
  // A second shower tests the barriers before the forest spirits speak.
  Engine_EventSetMessage(MsgKolimaWatchOutItsHappeningAgain);
  Kolima_ShowMessageAndWait(IVAN, 10);
  Kolima_FaceActorAndWait(GARET, FACING_NORTH, 10);
  Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
  if (gKolima_MiaInParty != 0) {
    Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  }
  Object_GetById(0)->priorityFlags &= 254;
  Object_GetById(1)->priorityFlags &= 254;
  Object_GetById(2)->priorityFlags &= 254;
  Object_GetById(3)->priorityFlags &= 254;
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetSpritePriority(GARET, 3);
  Engine_ActorSetSpritePriority(IVAN, 3);
  Engine_ActorSetSpritePriority(MIA, 3);
  *secondCurtainState = 2;
  Event_PlayCue(SE_220);
  Actor_SetPosition(19, PIXELS(120), PIXELS(248));
  growFieldActions = gKolima_ProtectiveFieldGrowActions;
  Actor_SetActionScript(19, growFieldActions);
  Actor_SetPosition(20, PIXELS(100), PIXELS(274));
  Actor_SetActionScript(20, growFieldActions);
  if (gKolima_MiaInParty != 0) {
    Actor_SetPosition(21, PIXELS(74), PIXELS(254));
    Actor_SetActionScript(21, growFieldActions);
  }
  Actor_SetPosition(22, PIXELS(94), PIXELS(225));
  Actor_SetActionScript(22, growFieldActions);
  Engine_EventWait(120);
  *secondCurtainState = 3;
  do {
    Engine_TaskWait(1);
  } while (gFallingEffectState != 0);
  // Hear the trees debate the party\'s immunity to Tret\'s glamour.
  Kolima_ShowMessageAndWait(17, 80);
  Kolima_ShowMessageAndWait(18, 20);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(18, 20);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(18, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(18, 10);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 0);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(17, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 40);
  Kolima_ShowMessageAndWait(18, 10);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 80);
  Kolima_ShowMessageAndWait(18, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 40);
  Kolima_ShowMessageAndWait(17, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
  Engine_ActorSetAnimation(GARET, 4);
  Engine_ActorSetAnimation(MIA, 4);
  Engine_ActorSetAnimationAndWait(IVAN, 4);
  Engine_EventWait(60);
  Kolima_ShowMessageAndWait(18, 10);
  Kolima_NodTogether();
  Kolima_ShowMessageAndWait(18, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(18, 10);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
  Engine_ActorStartRepeatedMotion(GARET, 2);
  Engine_ActorStartRepeatedMotion(MIA, 2);
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 0);
  Kolima_ShowMessageAndWait(18, 20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, 0, 20);
  Kolima_ShowMessageAndWait(17, 20);
  Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
  Actor_SetAttachedEffect(GARET, 0x102);
  Actor_SetAttachedEffect(MIA, 0x102);
  Actor_SetAttachedEffect(IVAN, 0x102);
  Engine_EventWait(40);
  Kolima_ShowMessageAndWait(18, 10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Kolima_FaceActorAndWait(MIA, FACING_NORTH, 10);
  Kolima_ShowMessageAndWait(18, 10);
  Kolima_NodTogether();
  Engine_EventWait(60);
  // End the forest\'s appeal and let the party decide whether to help.
  Event_ShowMessage(18, 0);
  Event_ShowMessage(17, 0);
  Engine_TaskRemoveCallback(Kolima_UpdateCurseDust);
  Engine_EventWait(80);
  ColorBuffer_ApplyTarget(PALETTE_TRANSFORM_PLAIN, 1);
  Engine_ColorBufferInterpolate(60);
  Engine_TaskWait(80);
  Engine_ActorStop(19);
  Engine_ActorStop(20);
  sceneState = &gKolima_MiaInParty;
  Engine_ActorStop(21);
  Engine_ActorStop(22);
  Engine_TaskWait(1);
  shrinkFieldActions = gKolima_ProtectiveFieldShrinkActions;
  Actor_SetActionScript(19, shrinkFieldActions);
  Actor_SetActionScript(20, shrinkFieldActions);
  if (*sceneState != 0) {
    Actor_SetActionScript(21, shrinkFieldActions);
  }
  Actor_RunActionScriptAndWait(22, shrinkFieldActions);
  Engine_EventWait(20);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
  Engine_ActorSetSpritePriority(GARET, 2);
  Engine_ActorSetSpritePriority(IVAN, 2);
  stagingValue = 1;
  Engine_ActorSetSpritePriority(MIA, 2);
  Object_GetById(0)->priorityFlags |= stagingValue;
  Object_GetById(1)->priorityFlags |= stagingValue;
  Object_GetById(2)->priorityFlags |= stagingValue;
  Object_GetById(3)->priorityFlags |= stagingValue;
  Engine_ActorRunRepeatedMotion(IVAN, 2);
  Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
  Event_OpenMessage(IVAN, 0);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_EAST, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(GARET, 2);
    Engine_EventWait(10);
    Event_OpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Kolima_FaceActorAndWait(MIA, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 1, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 1, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 0);
      Engine_EventWait(40);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      Kolima_ShowMessageAndWait(GARET, 10);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTH, 20);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTHEAST, 20);
    } else {
      Kolima_FaceActorAndWait(MIA, 0, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 0);
      Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 2, 40);
      Kolima_FaceActorAndWait(GARET, FACING_SOUTH, 20);
      Engine_EventSetMessage(MsgKolimaHesAsStumpedAsWe);
      Kolima_ShowMessageAndWait(GARET, 20);
      Engine_ActorSetAnimationAndWait(IVAN, 3);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
    Engine_ActorSetAnimation(MIA, 3);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
  } else {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GARET, 3);
    Engine_EventWait(10);
    Engine_EventSetMessage(MsgKolimaYoureIsaacThereIsntMuch);
    Kolima_ShowMessageAndWait(GARET, 10);
    Actor_FaceDirection(GARET, FACING_SOUTH, 0);
    Kolima_FaceActorAndWait(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 20);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(10);
    Engine_ActorSetAnimationAndWait(IVAN, 4);
    Event_OpenMessage(IVAN, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 3, 0);
      Engine_EventWait(40);
      Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 10);
      Kolima_ShowMessageAndWait(IVAN, 10);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 3);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message++;
      }
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Actor_SetAttachedEffect(GARET, 0x102);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 5, 0);
      Engine_EventWait(120);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 10);
        Engine_ActorSetAnimationAndWait(MIA, 4);
        Kolima_ShowMessageAndWait(MIA, 10);
      } else {
        gEventWork->message++;
      }
      Engine_EventWait(60);
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(IVAN, FACING_NORTHWEST, 40);
        Kolima_FaceActorAndWait(IVAN, FACING_NORTHEAST, 20);
      }
      Kolima_ShowMessageAndWait(IVAN, 10);
      Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Engine_EventWait(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
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
      Kolima_ShowMessageAndWait(IVAN, 20);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, 0, 10);
        Engine_ActorStartRepeatedMotion(MIA, 1);
        Kolima_ShowMessageAndWait(MIA, 20);
      } else {
        gEventWork->message++;
      }
      Actor_SetAttachedEffect(GARET, 0x102);
      Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
      Engine_EventWait(40);
      Engine_ActorRunRepeatedMotion(GARET, 2);
      Kolima_ShowMessageAndWait(GARET, 20);
      Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 5, 0);
      Engine_EventWait(80);
      Kolima_ShowMessageAndWait(IVAN, 40);
      if (gKolima_MiaInParty != 0) {
        Kolima_FaceActorAndWait(MIA, FACING_SOUTHEAST, 20);
        Engine_ActorSetAnimation(MIA, 4);
        Kolima_ShowMessageAndWait(MIA, 40);
      } else {
        gEventWork->message++;
      }
      Engine_ActorRunRepeatedMotion(IVAN, 2);
      Engine_EventWait(20);
      Kolima_ShowMessageAndWait(IVAN, 20);
      Engine_ActorStartRepeatedMotion(GARET, 2);
      Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
      Engine_EventWait(40);
      Kolima_ShowMessageAndWait(IVAN, 20);
    }
  }
  // Reunite the party and remember that the first visit has played.
  Event_PlayCue(MUS_017);
  Actor_SetSpeed(GARET, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(IVAN, PIXELS(6) / 5, PIXELS(3) / 5);
  Actor_SetSpeed(MIA, PIXELS(6) / 5, PIXELS(3) / 5);
  Engine_ActorSetAnimation(GARET, 2);
  actor = Object_GetById(0);
  if (actor != NULL) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorSetAnimation(IVAN, 2);
  actor = Object_GetById(0);
  if (actor != NULL) {
    Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Actor_SetPosition(17, 0, 0);
  Actor_SetPosition(18, 0, 0);
  if (gKolima_MiaInParty != 0) {
    Engine_ActorSetAnimation(MIA, 2);
    actor = Object_GetById(0);
    if (actor != NULL) {
      Actor_SetDestination(MIA, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(MIA);
    Actor_SetPosition(MIA, 0, 0);
  }
  GameFlag_Set(FLAG_KOLIMA_CURSE_WITNESSED);
  Audio_PlayCueFromEventWork();
  Engine_EventEnd();
}

/// Show the selected dialogue through an actor, then hold for its reaction.
static void Kolima_ShowMessageAndWait(s32 actorId, s32 waitFrames) {
  Engine_EventShowMessage(actorId, 0);
  Engine_EventWait(waitFrames);
}

/// Turn an actor immediately, then leave time for the next scene beat.
static void Kolima_FaceActorAndWait(s32 actorId, s32 facing, s32 waitFrames) {
  Engine_ActorFaceDirection(actorId, facing, 0);
  Engine_EventWait(waitFrames);
}

// Shared source is included here in ROM order.
#include "common/kolima_grow.h"

/// Spawn a moving curse sparkle every eighth frame.
static void Kolima_SpawnCurseSparkle(void) {
  struct FieldActor *sparkle;
  struct AnimationObject *sprite;
  s32 framePhase;
  s32 priorityFlags;
  s32 spawnX = PIXELS(98);
  s32 spawnZ = PIXELS(105);
  s32 targetX = PIXELS(98);
  s32 targetZ = PIXELS(269);

  framePhase = gFrameCount & 7;
  if (framePhase != 0) {
    return;
  }
  if (gKolima_EffectActive != 0) {
    Engine_AudioPlayCue(SE_200);
  }
  sparkle = Engine_ObjectCreate(26, spawnX, 0, spawnZ);
  if (sparkle == 0) {
    return;
  }
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
  Engine_ObjectSetScript(sparkle, gKolima_Object26Script);
}

// Shared source is included here in ROM order.
#include "common/kolima_blink.h"

/// Trigger a temporary pose when an effect enters the party's occupied area.
static s32 Kolima_TriggerRegionScript(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  if (gKolima_MiaInParty != 0) {
    if ((actor->x.fixed > PIXELS(59) && actor->x.fixed < PIXELS(141) &&
         actor->z.fixed > PIXELS(211) && actor->z.fixed < PIXELS(257)) ||
        (actor->x.fixed > PIXELS(69) && actor->x.fixed < PIXELS(122) &&
         actor->z.fixed > PIXELS(194) && actor->z.fixed < PIXELS(277))) {
      Engine_AudioPlayCue(SE_106);
      Engine_ObjectSetScript(actor, gKolima_RegionScript);
      gKolima_PendingPose = 1;
    }
  } else {
    if ((actor->x.fixed > PIXELS(59) && actor->x.fixed < PIXELS(111) &&
         actor->z.fixed > PIXELS(194) && actor->z.fixed < PIXELS(230)) ||
        (actor->x.fixed > PIXELS(111) && actor->x.fixed < PIXELS(141) &&
         actor->z.fixed > PIXELS(216) && actor->z.fixed < PIXELS(250)) ||
        (actor->x.fixed > PIXELS(78) && actor->x.fixed < PIXELS(122) &&
         actor->z.fixed > PIXELS(241) && actor->z.fixed < PIXELS(277))) {
      Engine_AudioPlayCue(SE_106);
      Engine_ObjectSetScript(actor, gKolima_RegionScript);
      gKolima_PendingPose = 1;
    }
  }
  return 0;
}

// Shared source is included here in ROM order.
#include "common/kolima_forest_bridge_falling_effect.h"
s32 gKolima_PendingPose __attribute__((section(".bss")));
s32 gKolima_MiaInParty __attribute__((section(".bss")));
s32 gKolima_EffectActive __attribute__((section(".bss")));

/// Restore the bridge's villagers and collision after its collapse.
static void Kolima_RestoreBridgeState(void) {
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    Engine_ActorSetPosition(8, 0, 0);
    {
      Map_CopyCellAttributes(9, 17, 5, 1, 9, 18);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
  } else {
    Engine_ActorSetPosition(9, 0, 0);
  }
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_BRIDGE_COLLAPSED) != 0) {
    {
      Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
    }
    {
      Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
    }
    Engine_MapRedraw();
    Engine_TaskWait(1);
  }
}

s32 gKolima_ActionTable1[] = {
    SC_ARG(2), SC_MOVE(PIXELS(97), 0, PIXELS(221)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_ARG(1), SC_END,
};

s32 gKolima_ActionTable2[] = {
    SC_ARG(2), SC_MOVE(PIXELS(101), 0, PIXELS(268)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_EAST),
    SC_ARG(1), SC_END,
};

s32 gKolima_ActionTable3[] = {
    SC_ARG(2), SC_MOVE(PIXELS(78), 0, PIXELS(247)),
    SC_SYNC,   SC_SET(SW_FACING, FACING_NORTH),
    SC_ARG(1), SC_END,
};

s32 gKolima_PartySquashActions[] = {
    SC_ADD(SW_SCALE_X, -Q16_ONE / 4),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 8),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 8),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 16),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, Q16_ONE / 16),
    SC_ADD(SW_SCALE_Y, -Q16_ONE / 32),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_PartyRestoreScaleActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_WAIT(1),
    SC_END,
};

s32 gKolima_ProtectiveFieldGrowActions[] = {
    SC_SET(SW_SCALE_X, Q16_ONE / 10),
    SC_SET(SW_SCALE_Y, Q16_ONE / 10),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, Q16_ONE / 50),
    SC_ADD(SW_SCALE_Y, Q16_ONE / 50),
    SC_WAIT(1),
    SC_LOOP(70, SC_TO(1)),
    SC_CALL(Kolima_ApplyPendingPose),
    SC_END,
};

s32 gKolima_ProtectiveFieldShrinkActions[] = {
    SC_ARG(1),
    SC_ADD(SW_Z, -PIXELS(1) / 10),
    SC_ADD(SW_SCALE_X, -3 * Q16_ONE / 200),
    SC_ADD(SW_SCALE_Y, -3 * Q16_ONE / 200),
    SC_WAIT(1),
    SC_LOOP(90, SC_TOP),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

s32 gKolima_EntryActions[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 4),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 4),
    SC_SET(SW_COLLISION, 0),
    SC_SET(0x23, 0),
    SC_LABEL(1),
    SC_CALL(Kolima_UpdateTimedAppearance),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_DebrisScript[] = {
    SC_CALL(Kolima_UpdateFallingDebris),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_FaceLeaderScript[] = {
    SC_CALL(Scene_TurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gKolima_SwitchCells[] = {
    MAP_CELL_STEP(66, 32, 1, 2, 4),
    MAP_CELL_STEP(67, 32, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gKolimaEntrances[] = {
    {0, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 418, 0, 285, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 119, 0, 182, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 151, 0, 310, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 264, 0, 373, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 329, 0, 230, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 263, 0, 326, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 360, 0, 346, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 232, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKolimaEntrances3[] = {
    {1, CONDITION_ALWAYS, 170, 0, 386, FACING_NORTH, 0, 0, 0, 288, 416, 0},
    {2, CONDITION_ALWAYS, 168, 0, 50, FACING_SOUTH, 0, 48, 0, 288, 416, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKolimaEntrances2[] = {
    {1, CONDITION_ALWAYS, 216, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneRegion gKolimaRegions2[] = {
    {207, 31, 311, 225, 49, 329, -1, 1},
    {303, 31, 303, 321, 49, 321, -1, 2},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gKolimaExits[] = {
    SCENE_EXITS(36),
    SCENE_EXIT(1, 2, 7),
    SCENE_EXIT(2, 2, 38),
    SCENE_EXIT(5, 37, 4),
    SCENE_EXIT(6, 37, 3),
    SCENE_EXIT(7, 37, 7),
    SCENE_EXIT(8, 37, 10),
    SCENE_EXIT(9, 37, 8),
    SCENE_EXIT(10, 38, 2),
    SCENE_EXIT(0, 39, 0),
    SCENE_EXIT(1, 2, 11),
    SCENE_EXIT(2, 2, 46),
    SCENE_EXIT(0, 38, 0),
    SCENE_EXIT(1, 37, 9),
    SCENE_EXIT(2, 36, 8),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaPlacements[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gKolimaPlacements1[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(168), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(344), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(392), FACING_SOUTH, 3, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(264), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(264), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x37, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x38, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdc, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xdc, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdc, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xdc, FLAG_KOLIMA_CURSE_WITNESSED, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(24), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, ACTOR_STAND,
        PIXELS(40), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x5a, 0x31, ACTOR_STAND,
        PIXELS(328), 0, PIXELS(360), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKolima_EntryScript[] = {
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(120), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(440), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(296), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0x11d, FLAG_KOLIMA_CURSE_LIFTED, 7,
        PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKolimaPlacements3[] = {
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, 3, 0 },
    { 0x69, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(234), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKolimaEvents[] = {
    { EVENT_EXIT, 99, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_AnimateSwitchOrExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_AnimateSwitchOrExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_AnimateSwitchOrExit) },
    { TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_AnimateSwitchOrExit) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_AnimateSwitchOrExit) },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_TALK, 8, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaThisIsKolimaVillageOfLumberjacks) },
    { EVENT_TALK, 9, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWeRelyOnTheForestsSo) },
    { EVENT_TALK, 10, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYouKnowBeingATreeGives) },
    { EVENT_TALK, 11, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaITurnedIntoATreeRight) },
    { EVENT_TALK, 12, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaDontBlameTheVillagersForBeing) },
    { EVENT_TALK, 13, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaWhoWouldTryToUseThe) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIDidntMindBeingATree) },
    { EVENT_TALK, 15, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaYawnTreesAreSoSleepy) },
    { EVENT_TALK, 16, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(Kolima_TalkToHealer) },
    { EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(Kolima_RecruitDjinni) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaForAWhileIThoughtId) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaTheForestTreesAreAliveJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIWonderIfIdLiveLonger) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfThoseSparklesStartFallingAgain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIveBeenPoorAllMyLife) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaItsNotCalledTheHolyTree) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfWeWereAllTreesThered) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaNobodyYellsAtYouWhenYoure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaIfTheHolyTreeAloneCaused) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICouldSpeakIdTell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKolimaHowCouldAnyoneTurnAGirl) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKolimaThisIsADreamItHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKolimaBeingUnableToMoveIsSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKolimaIAmOldIveLivedA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKolimaItsTheSacredTreesCurseWhy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKolimaTheWaterAtMyFeetHuh) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKolimaIfICantMoveThenMom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKolimaMaybeIfIMeditateHarderIll) },
    { SEARCH(SEARCH_JAR), 100, 0xf57, FIND_ITEM(ITEM_APPLE) },
    // A Herb (item 0xb4).
    { SEARCH(SEARCH_JAR), 101, 0xf58, FIND_ITEM(ITEM_HERB) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKolimaEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 10, FLAG_KOLIMA_BRIDGE_COLLAPSED, EVENT_SCRIPT(Kolima_ShowBridgeCollapse) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKolimaForAWhileIThoughtNo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKolimaYouWouldntUnderstandButItsTruly) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gKolima_Object26Script[] = {
    SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKolima_RegionScript[] = {
    SC_SET(SW_SCALE_X, 2 * Q16_ONE),
    SC_SET(SW_SCALE_Y, 2 * Q16_ONE),
    SC_WAIT(10),
    SC_FREE,
};

s32 gFallingEffectScript[] = {
    SC_ADD(SW_Y, -PIXELS(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_ADD(SW_Y, -PIXELS(2)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, -PIXELS(1)),
    SC_CALL(Kolima_FlickerCurseDust),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_CALL(Kolima_TriggerRegionScript),
    SC_FREE,
};
