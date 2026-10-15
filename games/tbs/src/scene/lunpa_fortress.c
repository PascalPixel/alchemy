/*----------------------------------------------------------
    ルンパ城  イベント
    Lunpa Fortress events.
----------------------------------------------------------*/
#include "game_state.h"
#include "field_event.h"
#include "unit/import.h"
#include "unit/scene/lunpa_fortress.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "edition.h"
#include "lunpa_fortress_staging.h"
#include "event.h"
#include "scene_ids.h"
#include "tbs_edition.h"
#include "map_scroll.h"
#include "text/msg_ids.h"
#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static s32 TryStartActorInteraction(s32 actorId, s32 interaction_id);
static void FieldScene_RunScene3bfSequenceC(void);
static s32 IsActorInteractionAvailable(s32 actorId);
static s32 IsSceneActorWithinFourSteps(s32 actorId);
static s32 IsSceneActorWithinTriggerBox(s32 actorId);
static void RunActorScriptedSequenceA(s32 actorId);
static void RunActorScriptedSequenceB(s32 handle);
static void RunActorScriptedSequenceC(s32 actorId);
static void LunpaFortress_ActOutGuardShiftDialogue(s32 a0);
static void RunActorScriptedSequenceD(s32 actorId);
static void FieldScene_InstallSceneTasks(void);
static void FieldScene_SetupActorsForScene(void);
static void FieldScene_RestoreActorsFromFlags(void);
static void FieldScene_ActivateThreeActorGroup(void);
static void FieldScene_ActivateTwoActorGroup(void);
static void FieldScene_ActivateAlternateActorGroup(void);
static void ActivateFiveActorGroupFromFlags(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_staged_actor.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressEntrances1;
  }
  if (scene == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressEntrances2;
  }
  if (scene == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressEntrances3;
  }
  return gLunpaFortressEntrancesOther;
}

s32 GetEmptySceneData(void) { return 0; }

const u32 *Scene_GetExits(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressExits2;
  }
  if (scene == (s32)&SceneId_LunpaFortress3 ||
      scene == (s32)&SceneId_LunpaFortress4) {
    return gLunpaFortressExits3And4;
  }
  return gLunpaFortressExitsOther;
}

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_WestLunpaCave) {
    return gLunpaFortressPlacementsWestLunpaCave;
  }
  if (scene == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressPlacements3;
  }
  if (scene == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressPlacements2;
  }
  if (scene == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressPlacements1;
  }
  if (scene == (s32)&SceneId_LunpaFortress4) {
    return gLunpaFortressPlacements4;
  }
  return gLunpaFortressPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressEvents1;
  }
  if (scene == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressEvents2;
  }
  if (scene == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressEvents3;
  }
  return gLunpaFortressEventsOther;
}

static void ConfigureSceneActor12(void) {

  struct FieldActor *actor;

  Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  actor = Object_GetById(12);
  if (actor != 0) {
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->motionFlags = 0;
    actor->priorityFlags = 2;
  }
}

static void EV70_INIT(void) { StagedActor_AdvancePair(); }

static void EV50_START(void) {

  OBJ_SPEED(12, 0x10000, 0x8000);
  Actor_SetDestination(12, 248, 0x178);
  Engine_ActorWaitForMove(12);
  SE(215);
  WAIT(60);
  ConfigureSceneActor12();
  FLAG_SET(0x943);
}

static void EV12_MOVE(void) {
  struct FieldActor *actor;

  actor = Actor_Get(12);
  if ((actor->z.fixed >> 20) > 22) {
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    SE(144);
    Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
    Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
    actor = (struct FieldActor *)(s32)Actor_Get(12);
    if (actor != NULL) {
      Engine_ActorSetSpriteFlags(actor, 0);
      actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
    FLAG_SET(0x943);
  }
}

static void PlaceActorTwelveAndFinishScene(void) {
  OBJ_POS_SET(12, 248, 376);
  ConfigureSceneActor12();
}

static void PlaceSceneObjectPairFromTableA(s32 tableIndex) {

  s32 positionX = gLunpaFortressPairTableA[tableIndex * 2];
  s32 positionZ = gLunpaFortressPairTableA[tableIndex * 2 + 1];

  Map_CopyCells(0, 0x4d, 1, 3, positionX, positionZ);
  Map_CopyCells(1, 0x4d, 1, 1, positionX + 1, positionZ);
  Map_CopyCells(positionX, positionZ - 0x30, 1, 1, positionX, positionZ - 0x2e);
}

static void EV40_A(void) {

  struct EventWork *work;
  s32 trigger;
  s32 index;

  work = gEventWork;
  if (Engine_PartyInventoryFindOwner(234) != -1) {
    trigger = work->touchedTrigger;
    index = trigger - 40;
    if (FLAG_CHECK(0x941) == 0 || index != 4) {
      PlaceSceneObjectPairFromTableA(index);
      SE(157);
      Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
      Work_SetValuesIfNonNegative(-1, -1, 0xe666);
      FLAG_SET(trigger + 0x328);
    }
  }
}

static void FieldScene_SetPositionPairs(s32 idx) {
  s32 top_x;
  s32 top_y;
  s32 bottom_y;

  top_x = gLunpaFortressSupportPairs[idx * 2];
  top_y = gLunpaFortressSupportPairs[idx * 2 + 1];
  Map_CopyCells(0, 77, 1, 3, top_x, top_y);
  Map_CopyCells(1, 77, 1, 1, top_x + 1, top_y);
  bottom_y = top_y - 44;
  Map_CopyCellAttributes(top_x, top_y - 45, 1, 1, top_x, bottom_y);
  if (idx == 1)
    Map_CopyCellAttributes(top_x, bottom_y, 1, 1, top_x, top_y - 43);
}

static void EV40_B(void) {

  struct EventWork *work;
  s32 trigger;

  work = gEventWork;
  if (Engine_PartyInventoryFindOwner(234) != -1) {
    trigger = work->touchedTrigger;
    FieldScene_SetPositionPairs(trigger - 40);
    SE(157);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FLAG_SET(trigger + 0x32d);
  }
}

static void PlaceSceneObjectPairFromTableB(s32 tableIndex) {

  s32 positionX = gLunpaFortressPairTableB[tableIndex * 2];
  s32 positionZ = gLunpaFortressPairTableB[tableIndex * 2 + 1];

  Map_CopyCells(0x37, 0x79, 1, 3, positionX, positionZ);
  Map_CopyCells(0x38, 0x79, 1, 1, positionX + 1, positionZ);
  Map_CopyCells(positionX, positionZ - 0x3f, 1, 1, positionX, positionZ - 0x3e);
}

static void EV40_D(void) {

  struct EventWork *work;
  s32 trigger;

  work = gEventWork;
  if (Engine_PartyInventoryFindOwner(234) != -1) {
    trigger = work->touchedTrigger;
    PlaceSceneObjectPairFromTableB(trigger - 40);
    SE(157);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FLAG_SET(trigger + 0x330);
  }
}

static void PlaceSceneObjectPairFromTableC(s32 tableIndex) {

  s32 positionX = gLunpaFortressPairTableC[tableIndex * 2];
  s32 positionZ = gLunpaFortressPairTableC[tableIndex * 2 + 1];

  Map_CopyCells(1, 0x50, 1, 3, positionX, positionZ);
  Map_CopyCells(2, 0x50, 1, 1, positionX + 1, positionZ);
  Engine_MapCopyCellAttributes(positionX, positionZ - 0x3f, 1, 1, positionX,
                               positionZ - 0x3e);
}

static void EV40_C(void) {

  struct EventWork *work;
  s32 trigger;

  work = gEventWork;
  if (Engine_PartyInventoryFindOwner(234) != -1) {
    trigger = work->touchedTrigger;
    PlaceSceneObjectPairFromTableC(trigger - 40);
    SE(157);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FLAG_SET(trigger + 0x332);
  }
}

static void EV99_JAIL(void) {
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) == -1) {
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  }
}

static void EV98_DOOR(void) {
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
}

static void Actor8_Interact(void) {
  if (TryStartActorInteraction(8, 8) != 0) {
    Engine_GameFlagSet(0xf2a);
  }
}

static void Actor9_Interact(void) {
  if (TryStartActorInteraction(9, 7) != 0) {
    Engine_GameFlagSet(0xf2b);
  }
}

static void Actor10_Interact(void) {
  if (TryStartActorInteraction(10, 6) != 0) {
    Engine_GameFlagSet(0xf2c);
  }
}

static void Actor11_Interact(void) {
  if (TryStartActorInteraction(11, 5) != 0) {
    Engine_GameFlagSet(0xf2d);
  }
}

static s32 TryStartActorInteraction(s32 actorId, s32 interactionId) {
  s32 started = 0;
  s32 interaction;

  EV_BEGIN();
  interaction =
      Engine_BattleFxPlayCueAndStartEmitterOnTarget(0, actorId, interactionId);
  if (Engine_PartyGiveItem(interactionId, 0) != -1) {
    OBJ_MODE(actorId, 2);
    started = 1;
  } else {
    SE(0x7d);
    OBJ_MODE(actorId, 5);
  }
  Engine_ObjectDispatchRelease(interaction);
  EV_END();
  return started;
}

static void NoOpSceneCallbackA(void) {}

static void NoOpSceneCallbackB(void) {}

static void NoOpSceneCallbackC(void) {}

static void NoOpSceneCallbackD(void) {}

static void EV08_KEY(void) {

  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 0x108, 0x318);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
  WAIT(10);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  Engine_ItemShowFound(ITEM_CELL_KEY, 3);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
  Engine_GameFlagSet(0xf2e);
  Engine_ActorSetPosition(8, 0, 0);
}

static s32 IsPlayerInAccidentTriggerArea(void) {
  SceneActor *player = Object_GetById(0);
  s32 z = player->z;
  s32 x;
  s32 zz, xx;

  if (z < 0) {
    z += 0xfffff;
  }
  x = player->x;
  zz = z >> 20;
  if (x < 0) {
    x += 0xfffff;
  }
  xx = x >> 20;
  if ((u32)(zz - 5) <= 2 && xx <= 10) {
    return 1;
  }
  if ((u32)(xx - 8) <= 1 && zz > 22) {
    return 1;
  }
  return 0;
}

static void FieldScene_UpdateActorPairInteraction(void) {
  struct ObjectRuntime *actor = Actor_Get(9);
  struct ObjectRuntime *other = Actor_Get(10);
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
  s16 *scene = *(s16 **)&gMapWork[19];

  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || scene[191] != 0 || scene[192] != 0) {
    actor->movementState = 1;
    other->movementState = 1;
  } else if (!FLAG_CHECK(0x214)) {
    actor->movementState = 0;
    other->movementState = 0;
    if (!FLAG_CHECK(0x214) && actor->movementState == 0) {
      layer->phaseX = 8912896.0 - actor->x;
    }
    if (!IsPlayerInAccidentTriggerArea()) {
      if (gGameState.cloaked != 0) {
        if (IsSceneActorWithinFourSteps(9) && gGameState.cloaked != 0) {
          SetSceneValue(&scene[191], 0x2092);
          return;
        }
        if (IsSceneActorWithinFourSteps(10) && gGameState.cloaked != 0) {
          SetSceneValue(&scene[191], 0x2092);
          return;
        }
      }
      if (gGameState.cloaked == 0) {
        if (IsActorInteractionAvailable(9)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
        if (IsActorInteractionAvailable(10)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&scene[193], 91);
      }
    }
  }
}

static void EV91_SET(void) {
  EV_BEGIN();
  Engine_ActorStop(9);
  Engine_ActorSetDestinationOffset(9, 0, 0);
  OBJ_MODE(9, 0);
  OBJ_FACE(9, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(9, 256, 0);
  RunActorScriptedSequenceA(10);
  EV_END();
}

static s32 AreSceneActorsInPassingLane(void) {
  SceneActor *player = Object_GetById(0);
  SceneActor *passing_actor = Object_GetById(17);
  s32 ox = player->x;
  s32 pz;
  s32 px;
  s32 oxx, pzz, pxx;

  if (ox < 0) {
    ox += 0xfffff;
  }
  oxx = ox >> 20;
  pz = passing_actor->z;
  if (pz < 0) {
    pz += 0xfffff;
  }
  px = passing_actor->x;
  pzz = pz >> 20;
  if (px < 0) {
    px += 0xfffff;
  }
  pxx = px >> 20;
  if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
    return 1;
  }
  if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
    return 1;
  }
  return 0;
}

static void FieldScene_UpdateActorSeventeenInteraction(void) {

  struct ObjectRuntime *actor = Actor_Get(17);
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
  s16 *scene = *(s16 **)&gMapWork[19];

  Engine_EventGetViewCenter();
  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || scene[191] != 0 || scene[192] != 0) {
    actor->movementState = 1;
  } else if (!FLAG_CHECK(0x214)) {
    actor->movementState = 0;
    if (!FLAG_CHECK(0x214) && actor->movementState == 0) {
      layer->phaseX = 0x3400000 - actor->x;
      layer->phaseY = 0x2400000 - actor->z;
    }
    if (!AreSceneActorsInPassingLane()) {
      IsSceneActorWithinTriggerBox(17);
      if (IsSceneActorWithinFourSteps(17) && gGameState.cloaked != 0) {
        SetSceneValue(&scene[191], 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (IsActorInteractionAvailable(17)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&scene[193], 92);
      }
    }
  }
}

static void EV92_ON(void) {
  RunActorScriptedSequenceA(17);
  EV_END();
}

static s32 IsPlayerInSecondaryTriggerArea(void) {
  SceneActor *player = Object_GetById(0);
  s32 zz = player->z / 0x100000;
  s32 xx = player->x / 0x100000;

  if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
    return 1;
  }
  if (xx == 41 && zz > 37 && zz <= 41) {
    return 1;
  }
  if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
    return 1;
  }
  return 0;
}

static void FieldScene_UpdateActorEighteenInteraction(void) {

  struct ObjectRuntime *actor = Actor_Get(18);
  struct MapLayerScroll *layer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
  s16 *scene = *(s16 **)&gMapWork[19];

  if (gFrameCount & 1) {
    layer->speedX = 1;
    layer->speedY = 1;
  } else {
    layer->speedX = -1;
    layer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || scene[191] != 0 || scene[192] != 0) {
    actor->movementState = 1;
  } else if (!FLAG_CHECK(0x214)) {
    actor->movementState = 0;
    if (!FLAG_CHECK(0x214) && actor->movementState == 0) {
      layer->phaseX = 0x2f00000 - actor->x;
      layer->phaseY = 0x1f00000 - actor->z;
    }
    if (!IsPlayerInSecondaryTriggerArea()) {
      if (IsSceneActorWithinFourSteps(18) && gGameState.cloaked != 0) {
        SetSceneValue(&scene[191], 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (IsActorInteractionAvailable(18)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&scene[193], 93);
      }
    }
  }
}

static void EV93_ON(void) {
  RunActorScriptedSequenceA(18);
  EV_END();
}

static s32 IsPlayerOutsideSceneRectangle(void) {
  SceneActor *player = Object_GetById(0);
  s32 zz = player->z / 0x100000;
  s32 xx = player->x / 0x100000;

  if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
    return 0;
  }
  return 1;
}

static void FieldScene_RunScene3bfSequenceA(void) {

  struct EventWork *work;

  work = gEventWork;
  if (FLAG_CHECK(0x214) == 0) {
    if (IsPlayerOutsideSceneRectangle() == 0) {
      if (gGameState.cloaked == 0) {
        if (IsActorInteractionAvailable(17) != 0) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214) != 0) {
        work->raisedTrigger = 94;
      }
    }
  }
}

static void EV94_MAIN(void) {
  RunActorScriptedSequenceA(17);
  EV_END();
}

static void TriggerSceneStage95FromActor12(void) {

  struct EventWork *sceneState = gEventWork;

  if (IsActorInteractionAvailable(12) != 0 && gGameState.cloaked == 0) {
    s16 *sceneStage;
    s32 nextStage;

    Engine_TaskRemoveCallback(TriggerSceneStage95FromActor12);
    sceneStage = &sceneState->raisedTrigger;
    nextStage = 95;
    *sceneStage = nextStage;
  }
}

static void FieldScene_RunScene3bfSequenceB(void) {

  struct EventWork *work;

  work = gEventWork;
  if (FLAG_CHECK(0x225) == 0) {
    if (IsActorInteractionAvailable(13) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(0x225);
        Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
        Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
        work->raisedTrigger = 96;
      }
    }
  }
}

static void FieldScene_RunScene3bfSequenceC(void) {

  struct EventWork *work;

  work = gEventWork;
  if (FLAG_CHECK(0x225) == 0) {
    if (IsActorInteractionAvailable(21) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(0x225);
        Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
        Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
        work->raisedTrigger = 96;
      }
    }
  }
}

static s32 IsSceneActorVerticallyNearPlayer(s32 actorId) {
  SceneActor *sceneActor = Object_GetById(actorId);
  SceneActor *player = Object_GetById(0);
  s32 actorZ = sceneActor->z / 0x100000;
  s32 actorX = sceneActor->x / 0x100000;
  s32 playerZ = player->z / 0x100000;
  s32 playerX = player->x / 0x100000;
  s32 zDistance = actorZ - playerZ;

  if (zDistance >= -6 && zDistance <= 6 && actorX - 1 < playerX &&
      actorX + 1 > playerX) {
    return 1;
  }
  return 0;
}

static s32 IsSceneActorHorizontallyNearPlayer(s32 actorId) {
  SceneActor *sceneActor = Object_GetById(actorId);
  SceneActor *player = Object_GetById(0);
  s32 actorZ = sceneActor->z / 0x100000;
  s32 actorX = sceneActor->x / 0x100000;
  s32 playerZ = player->z / 0x100000;
  s32 playerX = player->x / 0x100000;
  s32 xDistance = actorX - playerX;

  if (xDistance < -6 || xDistance > 6) {
    return 0;
  }
  if (actorZ - 2 < playerZ && actorZ + 2 > playerZ) {
    return 1;
  }
  return 0;
}

static s32 IsActorInteractionAvailable(s32 actorId) {
  if (IsSceneActorWithinTriggerBox(actorId) == 0) {
    return 0;
  }
  if (IsSceneActorVerticallyNearPlayer(actorId) != 0) {
    return 1;
  }
  {
    s32 result = IsSceneActorHorizontallyNearPlayer(actorId);

    return (u32)(result | -result) >> 31;
  }
}

static s32 IsSceneActorWithinFourSteps(s32 actorId) {
  SceneActor *sceneActor = Object_GetById(actorId);
  SceneActor *player = Object_GetById(0);
  s32 actorZ = sceneActor->z / 0x100000;
  s32 actorX = sceneActor->x / 0x100000;
  s32 playerZ = player->z / 0x100000;
  s32 playerX = player->x / 0x100000;
  s32 xDistance = actorX - playerX;
  s32 zDistance;

  actorZ += 1;
  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = actorZ - playerZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance + zDistance <= 4) {
    return 1;
  }
  return 0;
}

static s32 IsSceneActorWithinTriggerBox(s32 actorId) {
  SceneActor *sceneActor = Object_GetById(actorId);
  SceneActor *player = Engine_EventGetViewCenter();
  s32 actorX = sceneActor->x / 0x100000;
  s32 actorZ = sceneActor->z / 0x100000;
  s32 playerX = player->x / 0x100000;
  s32 playerZ = player->z / 0x100000;
  s32 xDistance = actorX - playerX;
  s32 zDistance;

  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = actorZ - playerZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance > 7 || zDistance > 5) {
    return 0;
  }
  return 1;
}

static void TriggerScene41AtVillagePath(void) {

  SceneActor *player = Object_GetById(0);

  if (Engine_GameFlagIsSet(859) == 0) {
    s32 playerX = player->x / 0x100000;
    s32 playerZ = player->z / 0x100000;

    if (playerX == 43 && playerZ > 28 && playerZ <= 31) {
      s16 *q = &gEventWork->touchedTrigger;
      s32 v = 41;

      *q = v;
      EV40_C();
    }
  }
}

static void TriggerScene40AtVillagePath(void) {

  DirectionalSceneActor *player = Object_GetById(0);

  if (Engine_GameFlagIsSet(856) == 0) {
    s32 playerX = player->x / 0x100000;
    s32 playerZ = player->z / 0x100000;

    if (playerX == 16 && playerZ > 55 && playerZ <= 58 &&
        (player->dir == 0xc000 || player->dir == 0x4000)) {
      s16 *q = &gEventWork->touchedTrigger;
      s32 v = 40;

      *q = v;
      EV40_D();
    }
  }
}

static void RunActor9ScriptedSequence(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(9, 0, 0);
  Engine_ActorEnableActionCallback(9, 1);
  Engine_ActorStop(9);
  OBJ_MODE(9, 0);
  Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
  {
    s32 t = MsgLunpaWho2;

    MESSAGE_SET(t);
    MESSAGE_PUT(9, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    MESSAGE_SET(t + 1);
  }
  MESSAGE_PUT(9, 0);
  Engine_EventRequestExit(60);
  Engine_EventCloseScreen();
  EV_END();
}

static void RunActorScriptedSequenceA(s32 actorId) {
  EV_BEGIN();
  EV_BEGIN();
  Engine_ActorShowEmote(actorId, 256, 1);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Engine_ActorEnableActionCallback(actorId, 1);
  OBJ_MODE(actorId, 0);
  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Engine_ActorEnableActionCallback(actorId, 1);
  Engine_ActorStop(actorId);
  OBJ_MODE(actorId, 0);
  Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
  {
    s32 t = MsgLunpaWho2;

    MESSAGE_SET(t);
    MESSAGE_PUT(actorId, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 60);
    MESSAGE_SET(t + 1);
  }
  MESSAGE_PUT(actorId, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
}

static void TurnActorToSceneDirection(s32 actorId) {

  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    RunActorScriptedSequenceB(actorId);
    break;
  case 1:
    RunActorScriptedSequenceC(actorId);
    break;
  case 2:
    LunpaFortress_ActOutGuardShiftDialogue(actorId);
    break;
  case 3:
    RunActorScriptedSequenceD(actorId);
    break;
  default:
    RunActorScriptedSequenceC(actorId);
    break;
  }
}

static void RunActorScriptedSequenceB(s32 handle) {
  s32 id;

  Engine_ActorRunRepeatedMotion(handle, 1);
  id = MsgLunpaIntruder;
  MESSAGE_SET(id);
  MESSAGE_PUT(handle, 0);
  OBJ_EMOTE(handle, 258, 60);
  MESSAGE_SET(id + 1);
  MESSAGE_PUT(handle, 0);
  id += 2;
  Engine_ActorSetAnimationAndWait(handle, 4);
  MESSAGE_SET(id);
  MESSAGE_PUT(handle, 0);
}

static void RunActorScriptedSequenceC(s32 actorId) {
  s32 t = MsgLunpaGuardWhosThat;

  MESSAGE_SET(t);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(t + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(t + 2);
  MESSAGE_PUT(actorId, 0);
}

static void LunpaFortress_ActOutGuardShiftDialogue(s32 a0) {
  s32 intruder;

  intruder = MsgLunpaShiftAlready;
  MESSAGE_SET(intruder);
  MESSAGE_PUT(a0, 0);
  WAIT(120);
  OBJ_EMOTE(a0, 0x101, 60);
  MESSAGE_SET((intruder + 1));
  MESSAGE_PUT(a0, 0);
  Engine_ActorRunRepeatedMotion(a0, 1);
  MESSAGE_SET((intruder + 2));
  MESSAGE_PUT(a0, 0);
  Engine_ActorSetAnimationAndWait(a0, 4);
  MESSAGE_SET((intruder + 3));
  MESSAGE_PUT(a0, 0);
}

static void RunActorScriptedSequenceD(s32 actorId) {
  s32 t = MsgLunpaScoundrel;

  MESSAGE_SET(t);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(t + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(t + 2);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 3);
  MESSAGE_SET(t + 3);
  MESSAGE_PUT(actorId, 0);
}

static void EV33_CHECK(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  SE(113);
  Engine_ActorShowEmote(15, 256, 60);
  TurnActorToSceneDirection(15);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(15, 0, 0);
  EV_END();
  Engine_GameFlagSet(2380);
}

static void EV30_TREASURE_CHEST(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  SE(113);
  Engine_ActorShowEmote(11, 256, 60);
  TurnActorToSceneDirection(11);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(11, 0, 0);
  EV_END();
  Engine_GameFlagSet(2377);
}

static void EV31_GUARD(void) {
  s32 line;

  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  OBJ_MODE(12, 1);
  OBJ_MODE(13, 1);
  OBJ_MODE(14, 1);
  SE(113);
  OBJ_EMOTE(12, 0x100, 0);
  WAIT(30);
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
  line = MsgLunpaWho3;
  MESSAGE_SET(line);
  MESSAGE_PUT(12, 0);
  OBJ_EMOTE(13, 0x100, 0);
  OBJ_EMOTE(14, 0x100, 0);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  WAIT(65);
  OBJ_DIR(13, 0x5000, 0);
  OBJ_DIR(14, 0xd000, 0);
  MESSAGE_SET((line + 1));
  MESSAGE_PUT(13, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((line + 2));
  MESSAGE_PUT(14, 0);
  MESSAGE_SET((line + 3));
  MESSAGE_PUT(12, 0);
  Engine_ActorRunRepeatedMotion(13, 1);
  MESSAGE_SET((line + 4));
  MESSAGE_PUT(13, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((line + 5));
  MESSAGE_PUT(14, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  WAIT(60);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  WAIT(70);
  OBJ_WALK(12, 672, 88);
  Engine_ActorWaitForMove(12);
#if EDITION_INTERNATIONAL
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
#endif
  Engine_ActorSetAnimationAndWait(12, 3);
  WAIT(30);
  MESSAGE_SET((line + 6));
  MESSAGE_PUT(12, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Engine_PartySetFields1ceAnd1d0((s32)&SceneId_LunpaFortress2, 31);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 3);
  OBJ_POS_SET(12, 0, 0);
  OBJ_POS_SET(13, 0, 0);
  OBJ_POS_SET(14, 0, 0);
  EV_END();
  FLAG_SET(0x94a);
}

static void FieldScene_RunSequenceTail(void) {
  EV_BEGIN();
  OBJ_POS_SET(12, 688, 88);
  OBJ_POS_SET(13, 704, 88);
  OBJ_POS_SET(14, 720, 96);
  OBJ_MODE(12, 5);
  OBJ_MODE(13, 5);
  OBJ_MODE(14, 5);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  EV_END();
  Engine_EventOpenScreen();
}

static void EV32_TOUCH(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  SE(113);
  Engine_ActorShowEmote(16, 256, 60);
  TurnActorToSceneDirection(16);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(16, 0, 0);
  EV_END();
  Engine_GameFlagSet(2379);
}

static void EV50_MAIN(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
  SE(113);
  Engine_ActorShowEmote(12, 256, 60);
  {
    s32 t = MsgLunpaWho2;

    MESSAGE_SET(t);
    MESSAGE_PUT(12, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, 258, 50);
    MESSAGE_SET(t + 1);
  }
  MESSAGE_PUT(12, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
  Engine_GameFlagSet(548);
}

static void EV51_MAIN(void) {
  s32 msg;

  EV_BEGIN();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, 1);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  SE(113);
  OBJ_EMOTE(21, 0x100, 0);
  OBJ_EMOTE(13, 0x100, 60);
  OBJ_FACE(21, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  msg = MsgLunpaWho2;
  MESSAGE_SET(msg);
  MESSAGE_PUT(13, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x102, 30);
  MESSAGE_SET(msg + 1);
  MESSAGE_PUT(13, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
  FLAG_SET(0x225);
}

static void ConfigureInteractionRegionA(void) {
  Map_CopyCells(2, 82, 1, 2, 21, 81);
  Map_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

static void ConfigureInteractionRegionB(void) {
  Map_CopyCells(2, 84, 1, 2, 6, 55);
  Map_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

static void ConfigureInteractionRegionC(void) {
  Map_CopyCells(2, 86, 1, 2, 27, 62);
  Map_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

static void EV52_WELL(void) {

  if (*(s16 *)(((u8 *)gEventWork) + 0xcb8) != 0) {
    if (FLAG_CHECK(0x947) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(188);
      WAIT(1);
      Map_CopyCells(6, 77, 1, 2, 17, 82);
      WAIT(5);
      Map_CopyCells(7, 77, 1, 2, 17, 82);
      WAIT(1);
      ConfigureInteractionRegionA();
      FLAG_SET(0x947);
    }
  }
}

static void EV51_SUB(void) {

  if (*(s16 *)(((u8 *)gEventWork) + 0xcb8) != 0) {
    if (FLAG_CHECK(0x948) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(188);
      WAIT(1);
      Map_CopyCells(6, 77, 1, 2, 3, 55);
      WAIT(5);
      Map_CopyCells(7, 77, 1, 2, 3, 55);
      WAIT(1);
      ConfigureInteractionRegionB();
      FLAG_SET(0x948);
    }
  }
}

static void EV00_REVEAL1(void) {
  Map_CopyCells(5, 77, 1, 2, 17, 82);
  Map_CopyCells(5, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  Map_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

static void EV00_REVEAL2(void) {
  Map_CopyCells(8, 77, 1, 2, 17, 82);
  Map_CopyCells(8, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  Map_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

static void EV08_WARDROBE(void) {
  Engine_GameFlagSet(2372);
  Engine_GameFlagClear(535);
  Engine_ActorSetPosition(8, 0, 0);
}

static void EV09_FIREWOOD(void) {
  Engine_GameFlagSet(2373);
  ConfigureInteractionRegionC();
  Engine_ActorSetPosition(9, 0, 0);
}

static void EV10_BOOK(void) {
  Engine_GameFlagSet(2374);
  Engine_GameFlagClear(536);
  Engine_ActorSetPosition(10, 0, 0);
}

static void NoOpInteractionCallback(void) {}

static void LunpaFortress_GatherPartyAroundActorTwelve(void) {
  struct FieldActor *actor;

  actor = Actor_Get(0);
  if (actor != NULL) {
    Actor_SetPosition(IVAN, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(0);
  if (actor != NULL) {
    Actor_SetPosition(MIA, actor->x.fixed, actor->z.fixed);
  }
  actor = Actor_Get(0);
  if (actor != NULL) {
    Actor_SetPosition(GARET, actor->x.fixed, actor->z.fixed);
  }
  OBJ_DIR(ACTOR_PARTY_LEADER, 0, 0);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_WALK(IVAN, 456, 192);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(MIA, 440, 184);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_WALK(GARET, 448, 240);
  Engine_ActorWaitForMove(IVAN);
  OBJ_FACE(IVAN, 12, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorWaitForMove(MIA);
  OBJ_FACE(GARET, 12, 0);
  OBJ_FACE(MIA, 12, 0);
  WAIT(15);
}

static void LunpaFortress_ActivateLeverTrap(void) {
  Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
  SE(141);
  WAIT(80);
  SE(0x120);
  WAIT(5);
  SE(145);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x100, 0);
  OBJ_EMOTE(GARET, 0x100, 0);
  OBJ_EMOTE(IVAN, 0x100, 0);
  OBJ_EMOTE(MIA, 0x100, 0);
  OBJ_EMOTE(12, 0x100, 0);
  WAIT(60);
}

static void LunpaFortress_RevealMonster(void) {
  Engine_CameraMoveToActor(11, 1);
  Engine_CameraWaitForMove();
  WAIT(60);
  MESSAGE_SET(MsgLunpaPrepareBecomeMonster);
  MESSAGE_PUT(13, 0);
  OBJ_SPEED(11, 0x10000, 0x8000);
  OBJ_SPEED(15, 0x10000, 0x8000);
  OBJ_WALK(11, 472, 180);
  OBJ_WALK(15, 472, 180);
  Engine_CameraFollowActor(11, 1);
  Engine_ActorWaitForMove(11);
  OBJ_MODE(11, 4);
  WAIT(30);
}

static void LunpaFortress_PositionPartyForMonster(void) {
  OBJ_SPEED(IVAN, 0x10000, 0x8000);
  OBJ_WALK(IVAN, 504, 216);
  OBJ_SPEED(MIA, 0x10000, 0x8000);
  OBJ_WALK(MIA, 440, 232);
  OBJ_SPEED(GARET, 0x10000, 0x8000);
  OBJ_WALK(GARET, 480, 224);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, 0xc000, 0);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, 0xc000, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, 0xc000, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, 0xc000, 0);
}

static void EV20_CUTSCENE(void) {
  s32 textLine;

  if (FLAG_CHECK(769) != 0)
    return;
  FLAG_SET(624);
  EV_BEGIN();
  if (FLAG_CHECK(2370) != 0) {
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
    LunpaFortress_GatherPartyAroundActorTwelve();
    OBJ_EMOTE(12, 256, 60);
    OBJ_DIR(12, 32768, 0);
    Engine_ActorJump(12, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(12), 1);
    WAIT(30);
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(2, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    OBJ_DIR(ACTOR_PARTY_LEADER, 16384, 0);
    OBJ_DIR(IVAN, 16384, 0);
    OBJ_DIR(GARET, 16384, 0);
    OBJ_DIR(MIA, 16384, 0);
    OBJ_POS_SET(13, 456, 320);
    Camera_SetSpeed(131072, 16384);
    textLine = MsgLunpaBackMoreGuess;
    MESSAGE_SET(textLine);
    MESSAGE_PUT(13, 0);
    OBJ_WALK(13, 458, 272);
    Engine_ActorWaitForMove(13);
    OBJ_DIR(13, 20480, 0);
    WAIT(40);
    Actor_SetDestinationOffset(13, -8, 8);
    Engine_ActorWaitForMove(13);
    WAIT(60);
    SE(155);
    Engine_MessageShowCentered(textLine + 1, 1);
    Actor_SetDestinationOffset(13, 8, -8);
    LunpaFortress_ActivateLeverTrap();
    WAIT(120);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Engine_ActorStartRepeatedMotion(MIA, 2);
    WAIT(20);
    textLine += 2;
    LunpaFortress_PositionPartyForMonster();
    OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET(textLine);
    MESSAGE_PUT(13, 0);
    LunpaFortress_RevealMonster();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    WAIT(1);
    Battle_SetStart(3);
    Engine_PartySetFields1ceAnd1d0((s32)&SceneId_LunpaFortress4, 4);
    Engine_BattleFxSetWeightedResult(98, 4);
  } else {
    OBJ_MODE(ACTOR_PARTY_LEADER, 1);
    SE(17);
    WAIT(30);
    textLine = MsgLunpaTimeEat;
    MESSAGE_SET(textLine);
    MESSAGE_PUT(12, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    WAIT(140);
    OBJ_DIR(12, 32768, 0);
    Engine_ActorJump(12, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(12), 1);
    MESSAGE_SET(textLine + 1);
    MESSAGE_PUT(12, 0);
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
    LunpaFortress_GatherPartyAroundActorTwelve();
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(2, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    MESSAGE_SET(textLine + 2);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(12, 256, 0);
    WAIT(110);
    SE(60);
    MESSAGE_SET(textLine + 3);
    MESSAGE_PUT(12, 0);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    WAIT(20);
    OBJ_MODE(IVAN, 1);
    Engine_ActorStartRepeatedMotion(12, 1);
    WAIT(20);
    OBJ_SPEED(12, 26214, 13107);
    OBJ_WALK(12, 520, 208);
    Engine_ActorWaitForMove(12);
    OBJ_MODE(12, 1);
    WAIT(20);
    OBJ_DIR(12, 45056, 0);
    WAIT(30);
    OBJ_DIR(12, 20480, 0);
    WAIT(30);
    OBJ_FACE(12, IVAN, 0);
    WAIT(20);
    MESSAGE_SET(textLine + 4);
    MESSAGE_PUT(12, 0);
    WAIT(40);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    WAIT(20);
    OBJ_EMOTE(12, 264, 0);
    WAIT(120);
    MESSAGE_SET(textLine + 5);
    MESSAGE_PUT(12, 0);
    WAIT(25);
    Engine_ActorSetAnimationAndWait(IVAN, 3);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(12, 3);
    WAIT(40);
    OBJ_WALK(2, 480, 200);
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorFaceEachOther(IVAN, 12, 0);
    WAIT(60);
    MESSAGE_SET(textLine + 6);
    MESSAGE_PUT(IVAN, 0);
    WAIT(20);
    OBJ_MODE(12, 4);
    WAIT(80);
    MESSAGE_SET(textLine + 7);
    MESSAGE_PUT(12, 0);
    OBJ_POS_SET(13, 456, 320);
    SE(19);
    MESSAGE_SET(textLine + 8);
    MESSAGE_PUT(13, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
    OBJ_FACE(IVAN, 13, 0);
    OBJ_FACE(GARET, 13, 0);
    WAIT(5);
    OBJ_DIR(MIA, 16384, 0);
    OBJ_FACE(12, 13, 0);
    WAIT(30);
    SE(61);
    Camera_SetSpeed(131072, 16384);
    Engine_CameraMoveToActor(13, 1);
    Engine_CameraWaitForMove();
    OBJ_SPEED(13, 52428, 26214);
    OBJ_WALK(13, 456, 304);
    Engine_CameraFollowActor(13, 1);
    Engine_ActorWaitForMove(13);
    Engine_CameraFollowActor(GARET, 1);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, 258, 0);
    OBJ_EMOTE(IVAN, 258, 0);
    OBJ_EMOTE(GARET, 258, 0);
    OBJ_EMOTE(MIA, 258, 0);
    OBJ_EMOTE(12, 258, 0);
    WAIT(60);
    OBJ_FACE(12, 13, 0);
    Engine_ActorStartRepeatedMotion(12, 2);
    WAIT(60);
    MESSAGE_SET(textLine + 9);
    MESSAGE_PUT(12, 0);
    OBJ_FACE(13, 12, 0);
    MESSAGE_SET(textLine + 10);
    MESSAGE_PUT(13, 0);
    WAIT(60);
    OBJ_FACE(13, IVAN, 0);
    WAIT(30);
    MESSAGE_SET(textLine + 11);
    MESSAGE_PUT(13, 0);
    Engine_ActorFaceEachOther(MIA, IVAN, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
    OBJ_FACE(IVAN, 13, 0);
    OBJ_FACE(GARET, 13, 0);
    OBJ_FACE(MIA, 13, 0);
    Engine_ActorStartRepeatedMotion(13, 1);
    WAIT(60);
    MESSAGE_SET(textLine + 12);
    MESSAGE_PUT(13, 0);
    OBJ_EMOTE(1, 259, 0);
    WAIT(60);
    OBJ_MODE(13, 4);
    MESSAGE_SET(textLine + 13);
    MESSAGE_PUT(13, 0);
    OBJ_WALK(1, 456, 248);
    Engine_ActorWaitForMove(GARET);
    OBJ_DIR(1, 16384, 0);
    MESSAGE_SET(textLine + 14);
    MESSAGE_PUT(GARET, 0);
    OBJ_WALK(2, 472, 216);
    Engine_ActorWaitForMove(IVAN);
    OBJ_DIR(2, 16384, 0);
    WAIT(10);
    MESSAGE_SET(textLine + 15);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(12, 261, 0);
    WAIT(60);
    MESSAGE_SET(textLine + 16);
    MESSAGE_PUT(12, 0);
    OBJ_WALK(3, 440, 216);
    Engine_ActorWaitForMove(MIA);
    OBJ_FACE(MIA, 13, 0);
    Engine_ActorSetAnimationAndWait(MIA, 3);
    WAIT(10);
    MESSAGE_SET(textLine + 17);
    MESSAGE_PUT(MIA, 0);
    OBJ_MODE(13, 4);
    MESSAGE_SET(textLine + 18);
    MESSAGE_PUT(13, 0);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, 258, 0);
    OBJ_EMOTE(GARET, 258, 0);
    OBJ_EMOTE(MIA, 258, 0);
    OBJ_EMOTE(IVAN, 258, 0);
    OBJ_EMOTE(13, 264, 0);
    WAIT(60);
    MESSAGE_SET(textLine + 19);
    MESSAGE_PUT(13, 0);
    WAIT(20);
    OBJ_EMOTE(1, 259, 0);
    WAIT(60);
    MESSAGE_SET(textLine + 20);
    MESSAGE_PUT(GARET, 0);
    Engine_ActorStartRepeatedMotion(13, 1);
    WAIT(60);
    MESSAGE_SET(textLine + 21);
    MESSAGE_PUT(13, 0);
    OBJ_WALK(13, 456, 280);
    Engine_ActorWaitForMove(13);
    OBJ_DIR(13, 20480, 0);
    WAIT(80);
    Actor_SetDestinationOffset(13, -8, 8);
    Engine_ActorWaitForMove(13);
    WAIT(60);
    SE(155);
    Engine_MessageShowCentered(MsgLunpaDodonpaPulledLever, 1);
    Actor_SetDestinationOffset(13, 8, -8);
    OBJ_FACE(13, 11, 0);
    LunpaFortress_ActivateLeverTrap();
    SE(52);
    MESSAGE_SET(textLine + 23);
    MESSAGE_PUT(13, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, 11, 0);
    OBJ_FACE(GARET, 11, 0);
    OBJ_FACE(IVAN, 11, 0);
    OBJ_FACE(MIA, 11, 0);
    OBJ_FACE(12, 11, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    FLAG_SET(2370);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    WAIT(1);
    Battle_SetStart(3);
    Engine_PartySetFields1ceAnd1d0((s32)&SceneId_LunpaFortress4, 4);
    Engine_BattleFxSetWeightedResult(98, 4);
  }
  EV_END();
}

static void FieldScene_RunMainScriptSequence(void) {
  struct FieldActor *record;
  s32 scriptA;
  s32 scriptB;
  s32 scriptC;

  FLAG_SET(0x301);
  FLAG_SET(0x941);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  PlaceSceneObjectPairFromTableA(4);
  EV_BEGIN();
  record = Actor_Get(12);
  Engine_ActorSetSpriteFlags(record, 1);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 456, 184);
  OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
  OBJ_POS_SET(GARET, 440, 192);
  OBJ_DIR(GARET, 0x4000, 0);
  OBJ_POS_SET(MIA, 488, 184);
  OBJ_DIR(MIA, 0x4000, 0);
  OBJ_POS_SET(IVAN, 472, 184);
  OBJ_DIR(IVAN, 0x4000, 0);
  OBJ_POS_SET(12, 520, 224);
  OBJ_DIR(12, 0xb000, 0);
  OBJ_POS_SET(11, 448, 237);
  OBJ_DIR(11, 0x8000, 0);
  OBJ_POS_SET(15, 448, 238);
  Engine_ActorSetSpritePriority(15, 3);
  OBJ_MODE(11, 3);
  OBJ_POS_SET(13, 458, 243);
  OBJ_DIR(13, 0x4000, 0);
  OBJ_MODE(13, 5);
  Engine_EventOpenScreen();
  WAIT(120);

  scriptA = MsgLunpaUhnnGetOff;
  MESSAGE_SET(scriptA);
  MESSAGE_PUT(13, 0);
  OBJ_EMOTE(GARET, 0x101, 60);
  MESSAGE_SET((scriptA + 1));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 1);
  OBJ_EMOTE(IVAN, 0x101, 1);
  OBJ_EMOTE(MIA, 0x101, 1);
  WAIT(60);
  OBJ_FACE(12, 13, 0);
  WAIT(60);
  MESSAGE_SET((scriptA + 2));
  MESSAGE_PUT(12, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
  OBJ_FACE(GARET, 12, 0);
  OBJ_FACE(IVAN, 12, 0);
  OBJ_FACE(MIA, 12, 0);
  WAIT(60);
  OBJ_WALK(12, 512, 232);
  OBJ_MODE(12, 4);
  WAIT(60);
  MESSAGE_SET((scriptA + 3));
  MESSAGE_PUT(12, 0);
  WAIT(15);
  Engine_ActorStartRepeatedMotion(13, 2);
  MESSAGE_SET((scriptA + 4));
  MESSAGE_PUT(13, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  OBJ_FACE(IVAN, 13, 0);
  OBJ_FACE(MIA, 13, 0);
  OBJ_FACE(GARET, 13, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((scriptA + 5));
  MESSAGE_PUT(IVAN, 0);
  OBJ_EMOTE(MIA, 0x105, 0);
  OBJ_MODE(MIA, 4);
  WAIT(80);
  OBJ_MODE(MIA, 1);
  MESSAGE_SET((scriptA + 6));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(12, 0x102, 65);
  Engine_ActorStartRepeatedMotion(12, 2);
  WAIT(100);
  MESSAGE_SET((scriptA + 7));
  MESSAGE_PUT(13, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET((scriptA + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((scriptA + 9));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((scriptA + 10));
  MESSAGE_OPEN(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    OBJ_MODE(IVAN, 3);
    OBJ_MODE(GARET, 3);
    OBJ_MODE(MIA, 3);
    OBJ_FACE(12, 13, 0);
    WAIT(60);
  } else {
    MESSAGE_SET((scriptA + 11));
    MESSAGE_PUT(MIA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
    OBJ_FACE(GARET, 12, 0);
    OBJ_FACE(IVAN, 12, 0);
    OBJ_FACE(MIA, 12, 0);
    WAIT(20);
    OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
    WAIT(60);
    OBJ_FACE(12, 13, 0);
    WAIT(80);
    Engine_ActorSetAnimationAndWait(12, 3);
    WAIT(30);
    OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET((scriptA + 12));
    MESSAGE_PUT(12, 0);
    WAIT(60);
    OBJ_FACE(IVAN, 12, 0);
    OBJ_FACE(GARET, 12, 0);
    OBJ_FACE(MIA, 12, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, 12, 0);
    OBJ_MODE(IVAN, 3);
    OBJ_MODE(GARET, 3);
    OBJ_MODE(MIA, 3);
    OBJ_MODE(ACTOR_PARTY_LEADER, 3);
    WAIT(80);
    OBJ_FACE(12, 13, 0);
    OBJ_FACE(IVAN, 13, 0);
    OBJ_FACE(GARET, 13, 0);
    OBJ_FACE(MIA, 13, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  }
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  OBJ_SPEED(GARET, 0x8000, 0x4000);
  OBJ_SPEED(MIA, 0x8000, 0x4000);
  OBJ_SPEED(IVAN, 0x8000, 0x4000);
  OBJ_WALK(GARET, 416, 216);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 416, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 440, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, 0xc000, 0);
  OBJ_WALK(ACTOR_PARTY_LEADER, 440, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_DIR(ACTOR_PARTY_LEADER, 0, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 456, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, 0xc000, 0);
  OBJ_WALK(IVAN, 456, 216);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, 0x8000, 0);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  OBJ_MODE(IVAN, 3);
  OBJ_MODE(GARET, 3);
  OBJ_MODE(MIA, 3);
  WAIT(100);
  SE(226);
  OBJ_MODE(13, 7);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
  OBJ_SPEED(GARET, 0x1999, 0xccc);
  Actor_SetDestinationOffset(GARET, -24, 0);
  OBJ_SPEED(MIA, 0x1999, 0xccc);
  Actor_SetDestinationOffset(MIA, -24, 0);
  OBJ_SPEED(IVAN, 0x1999, 0xccc);
  Actor_SetDestinationOffset(IVAN, -24, 0);
  OBJ_SPEED(11, 0x1999, 0xccc);
  OBJ_SPEED(15, 0x1999, 0xccc);
  Actor_SetDestinationOffset(11, -24, 0);
  Actor_SetDestinationOffset(15, -24, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  SE(0x120);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
  OBJ_DIR(GARET, 0xc000, 0);
  OBJ_DIR(IVAN, 0x4000, 0);
  OBJ_DIR(MIA, 0xc000, 0);
  WAIT(60);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  OBJ_MODE(GARET, 3);
  OBJ_MODE(IVAN, 3);
  OBJ_MODE(MIA, 3);
  WAIT(100);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 184);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_MODE(ACTOR_PARTY_LEADER, 1);
  OBJ_DIR(ACTOR_PARTY_LEADER, 0x4000, 0);
  OBJ_WALK(GARET, 464, 256);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 480, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 440, 192);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, 1);
  OBJ_DIR(GARET, 0x4000, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 488, 184);
  OBJ_WALK(IVAN, 472, 184);
  Engine_ActorWaitForMove(IVAN);
  OBJ_MODE(IVAN, 1);
  OBJ_DIR(IVAN, 0x4000, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_MODE(MIA, 1);
  OBJ_DIR(MIA, 0x4000, 0);
  WAIT(30);
  WAIT(60);
  scriptB = MsgLunpaAbleGet;
  MESSAGE_SET(scriptB);
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(13, 6);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(13, 2);
  WAIT(60);
  OBJ_MODE(13, 7);
  MESSAGE_SET((scriptB + 1));
  MESSAGE_PUT(13, 0);
  WAIT(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(10);
  OBJ_EMOTE(MIA, 0x101, 80);
  OBJ_FACE(MIA, 13, 0);
  OBJ_FACE(IVAN, 13, 0);
  MESSAGE_SET((scriptB + 2));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(13, 0x102, 70);
  Engine_ActorStartRepeatedMotion(13, 2);
  WAIT(60);
  OBJ_MODE(13, 5);
  WAIT(70);
  MESSAGE_SET((scriptB + 3));
  MESSAGE_PUT(13, 0);
  OBJ_MODE(13, 7);
  OBJ_EMOTE(IVAN, 0x108, 40);
  MESSAGE_SET((scriptB + 4));
  MESSAGE_PUT(IVAN, 0);
  Engine_ActorSetAnimationAndWait(12, 3);
  OBJ_EMOTE(GARET, 0x103, 60);
  OBJ_WALK(GARET, 440, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((scriptB + 5));
  MESSAGE_PUT(GARET, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 0);
  OBJ_EMOTE(12, 0x101, 0);
  WAIT(70);
  OBJ_FACE(GARET, IVAN, 0);
  OBJ_EMOTE(GARET, 0x102, 75);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(GARET, 4);
  MESSAGE_SET((scriptB + 6));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(12, 0x101, 0);
  WAIT(60);
  MESSAGE_SET((scriptB + 7));
  MESSAGE_PUT(12, 0);
  OBJ_WALK(GARET, 448, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, 1);
  OBJ_FACE(GARET, 12, 0);
  MESSAGE_SET((scriptB + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(12, 1);
  WAIT(60);
  OBJ_FACE(GARET, 12, 0);
  WAIT(60);
  MESSAGE_SET((scriptB + 9));
  MESSAGE_PUT(12, 0);
  OBJ_MODE(GARET, 4);
  WAIT(60);
  MESSAGE_SET((scriptB + 10));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((scriptB + 11));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, MIA, 0);
  WAIT(20);
  MESSAGE_SET((scriptB + 12));
  MESSAGE_PUT(GARET, 0);
  WAIT(30);
  OBJ_EMOTE(MIA, 0x100, 80);
  OBJ_FACE(MIA, GARET, 0);
  MESSAGE_SET((scriptB + 13));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(MIA, 13, 0);
  MESSAGE_SET((scriptB + 14));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  OBJ_FACE(IVAN, 13, 0);
  OBJ_FACE(GARET, 13, 0);
  OBJ_FACE(MIA, 13, 0);
  WAIT(120);
  OBJ_EMOTE(13, 0x102, 30);
  Engine_ActorStartRepeatedMotion(13, 1);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(12, 1);
  WAIT(60);
  MESSAGE_SET((scriptB + 15));
  MESSAGE_PUT(12, 0);
  OBJ_EMOTE(GARET, 0x107, 110);
  OBJ_WALK(GARET, 456, 212);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, 1);
  MESSAGE_SET((scriptB + 16));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(IVAN, GARET, 0);
  WAIT(60);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((scriptB + 17));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, 13, 0);
  WAIT(80);
  OBJ_FACE(GARET, IVAN, 0);
  WAIT(60);
  OBJ_FACE(GARET, 13, 0);
  WAIT(30);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  OBJ_FACE(MIA, 13, 0);
  OBJ_FACE(13, 13, 0);
  OBJ_FACE(12, 13, 0);
  WAIT(80);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  WAIT(30);
  MESSAGE_SET((scriptB + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(13, 2);
  WAIT(70);
  MESSAGE_SET((scriptB + 19));
  MESSAGE_PUT(13, 0);
  OBJ_EMOTE(MIA, 0x100, 60);
  MESSAGE_SET((scriptB + 20));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(80);
  MESSAGE_SET((scriptB + 21));
  MESSAGE_OPEN(IVAN, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
  OBJ_POS_SET(14, 456, 304);
  OBJ_SPEED(14, 0x8000, 0x4000);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    SE(19);
    MESSAGE_SET((scriptB + 22));
    MESSAGE_PUT(14, 0);
  } else {
    SE(19);
    MESSAGE_SET((scriptB + 23));
    MESSAGE_PUT(14, 0);
  }
  OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
  OBJ_FACE(GARET, 14, 0);
  OBJ_FACE(MIA, 14, 0);
  OBJ_FACE(IVAN, 14, 0);
  OBJ_FACE(12, 14, 0);
  OBJ_FACE(13, 14, 0);
  Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
  Engine_ActorStartRepeatedMotion(13, 1);
  WAIT(60);
  SE(8);
  scriptC = MsgLunpaDad;
  MESSAGE_SET(scriptC);
  MESSAGE_PUT(13, 0);
  OBJ_WALK(14, 456, 280);
  Engine_ActorWaitForMove(14);
  OBJ_WALK(14, 440, 256);
  Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
  MESSAGE_SET((scriptC + 1));
  MESSAGE_PUT(14, 0);
  Engine_ActorWaitForMove(14);
  OBJ_DIR(14, 0xd000, 0);
  OBJ_EMOTE(13, 0x102, 80);
  OBJ_MODE(14, 4);
  WAIT(89);
  MESSAGE_SET((scriptC + 2));
  MESSAGE_PUT(14, 0);
  Engine_ActorRunRepeatedMotion(13, 2);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((scriptC + 3));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(13, 0x100, 80);
  OBJ_MODE(14, 4);
  WAIT(80);
  OBJ_DIR(14, 0x3000, 0);
  WAIT(20);
  MESSAGE_SET((scriptC + 4));
  MESSAGE_PUT(14, 0);
  Engine_ActorStartRepeatedMotion(13, 1);
  MESSAGE_SET((scriptC + 5));
  MESSAGE_PUT(13, 0);
  WAIT(30);
  Engine_ActorSetAnimationAndWait(14, 3);
  OBJ_FACE(14, 13, 0);
  WAIT(20);
  MESSAGE_SET((scriptC + 6));
  MESSAGE_PUT(14, 0);
  Engine_ActorStartRepeatedMotion(13, 1);
  OBJ_EMOTE(13, 0x102, 80);
  MESSAGE_SET((scriptC + 7));
  MESSAGE_PUT(13, 0);
  OBJ_EMOTE(14, 0x103, 60);
  MESSAGE_SET((scriptC + 8));
  MESSAGE_PUT(14, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((scriptC + 9));
  MESSAGE_PUT(14, 0);
  WAIT(20);
  Actor_SetAttachedEffect(13, 0x101);
  WAIT(80);
  Actor_SetAttachedEffect(13, 0);
  OBJ_WALK(14, 496, 240);
  Engine_ActorWaitForMove(14);
  OBJ_FACE(14, 12, 0);
  WAIT(20);
  OBJ_FACE(12, 14, 0);
  MESSAGE_SET((scriptC + 10));
  MESSAGE_PUT(14, 0);
  Engine_ActorStartRepeatedMotion(12, 1);
  MESSAGE_SET((scriptC + 11));
  MESSAGE_PUT(12, 0);
  WAIT(40);
  Engine_ActorSetAnimationAndWait(14, 3);
  WAIT(20);
  MESSAGE_SET((scriptC + 12));
  MESSAGE_PUT(14, 0);
  OBJ_MODE(14, 3);
  OBJ_EMOTE(12, 0x102, 60);
  MESSAGE_SET((scriptC + 13));
  MESSAGE_PUT(12, 0);
  OBJ_EMOTE(14, 0x100, 70);
  MESSAGE_SET((scriptC + 14));
  MESSAGE_PUT(14, 0);
  OBJ_MODE(12, 3);
  WAIT(140);
  OBJ_MODE(14, 3);
  WAIT(120);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 14, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  OBJ_MODE(IVAN, 3);
  OBJ_MODE(GARET, 3);
  OBJ_MODE(MIA, 3);
  WAIT(120);
  OBJ_EMOTE(14, 0x108, 180);
  MESSAGE_SET((scriptC + 15));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(12, 0x101, 80);
  MESSAGE_SET((scriptC + 16));
  MESSAGE_PUT(12, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(80);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  WAIT(60);
  OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
  OBJ_FACE(GARET, 14, 0);
  OBJ_FACE(IVAN, 14, 0);
  OBJ_FACE(MIA, 14, 0);
  WAIT(60);
  MESSAGE_SET((scriptC + 17));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 0);
  WAIT(100);
  MESSAGE_SET((scriptC + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(60);
  MESSAGE_SET((scriptC + 19));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(14, IVAN, 0);
  WAIT(20);
  MESSAGE_SET((scriptC + 20));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, 0x101, 0);
  OBJ_EMOTE(GARET, 0x101, 0);
  OBJ_EMOTE(IVAN, 0x101, 0);
  OBJ_EMOTE(MIA, 0x101, 0);
  WAIT(120);
  OBJ_MODE(14, 4);
  WAIT(120);
  MESSAGE_SET((scriptC + 21));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(14, 0x102, 90);
  MESSAGE_SET((scriptC + 22));
  MESSAGE_PUT(14, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  OBJ_MODE(IVAN, 3);
  OBJ_MODE(MIA, 3);
  OBJ_MODE(GARET, 3);
  OBJ_MODE(12, 3);
  WAIT(80);
  OBJ_MODE(14, 4);
  WAIT(120);
  MESSAGE_SET((scriptC + 23));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(12, 0x100, 60);
  MESSAGE_SET((scriptC + 24));
  MESSAGE_PUT(12, 0);
  OBJ_FACE(14, 12, 0);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(14, 1);
  WAIT(50);
  MESSAGE_SET((scriptC + 25));
  MESSAGE_PUT(14, 0);
  OBJ_MODE(GARET, 3);
  WAIT(70);
  MESSAGE_SET((scriptC + 26));
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(MIA, 3);
  MESSAGE_SET((scriptC + 27));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  WAIT(20);
  MESSAGE_SET((scriptC + 28));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(IVAN, 0x101, 90);
  MESSAGE_SET((scriptC + 29));
  MESSAGE_PUT(IVAN, 0);
  OBJ_DIR(14, 0x5000, 0);
  WAIT(20);
  MESSAGE_SET((scriptC + 30));
  MESSAGE_PUT(14, 0);
  Engine_ActorStartRepeatedMotion(12, 1);
  WAIT(60);
  MESSAGE_SET((scriptC + 31));
  MESSAGE_PUT(12, 0);
  OBJ_FACE(14, 12, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(14, 3);
  WAIT(20);
  MESSAGE_SET((scriptC + 32));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(GARET, 0x100, 70);
  MESSAGE_SET((scriptC + 33));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  MESSAGE_SET((scriptC + 34));
  MESSAGE_PUT(14, 0);
  OBJ_EMOTE(MIA, 0x100, 80);
  MESSAGE_SET((scriptC + 35));
  MESSAGE_PUT(MIA, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((scriptC + 36));
  MESSAGE_PUT(14, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  OBJ_MODE(IVAN, 3);
  WAIT(100);
  MESSAGE_SET((scriptC + 37));
  MESSAGE_OPEN(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    MESSAGE_SET((scriptC + 38));
    MESSAGE_PUT(14, 0);
  }
  WAIT(30);
  OBJ_MODE(GARET, 3);
  OBJ_MODE(IVAN, 3);
  OBJ_MODE(MIA, 3);
  WAIT(100);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  OBJ_MODE(12, 3);
  WAIT(100);
  OBJ_MODE(GARET, 2);
  record = Actor_Get(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  OBJ_POS_SET(GARET, 0, 0);
  OBJ_MODE(IVAN, 2);
  record = Actor_Get(0);
  if (record != 0) {
    Actor_SetDestination(IVAN, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  OBJ_POS_SET(IVAN, 0, 0);
  OBJ_MODE(MIA, 2);
  record = Actor_Get(0);
  if (record != 0) {
    Actor_SetDestination(MIA, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  OBJ_POS_SET(MIA, 0, 0);
  WAIT(30);
  OBJ_SPEED(12, 0x6666, 0x3333);
  OBJ_WALK(12, 472, 184);
  Engine_ActorWaitForMove(12);
  OBJ_MODE(12, 1);
  OBJ_DIR(12, 0x3000, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(14, 3);
  WAIT(20);
  OBJ_FACE(14, 13, 0);
  WAIT(20);
  MESSAGE_SET((scriptC + 39));
  MESSAGE_PUT(14, 0);
  Engine_ActorRunRepeatedMotion(13, 2);
  MESSAGE_SET((scriptC + 40));
  MESSAGE_PUT(13, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  WAIT(30);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  OBJ_SPEED(12, 0x10000, 0x8000);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  OBJ_WALK(ACTOR_PARTY_LEADER, 480, 248);
  WAIT(40);
  Engine_ActorSetSpritePriority(12, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
  OBJ_WALK(12, 480, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_FACE(ACTOR_PARTY_LEADER, 14, 0);
  Engine_ActorWaitForMove(12);
  OBJ_FACE(12, 14, 0);
  OBJ_DIR(12, 0x3000, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, 3);
  WAIT(5);
  OBJ_MODE(12, 3);
  WAIT(100);
  OBJ_MODE(14, 3);
  WAIT(100);
  OBJ_WALK(12, 480, 248);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 248);
  Engine_ActorWaitForMove(12);
  OBJ_WALK(12, 456, 248);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 360);
  Engine_ActorWaitForMove(12);
  OBJ_WALK(12, 456, 360);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 352, 360);
  Engine_ActorWaitForMove(12);
  OBJ_WALK(12, 352, 360);
  WAIT(20);
  SE(17);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventCloseScreen();
  WAIT(1);
  WAIT(210);
  Engine_EventRequestExit(4);
  EV_END();
}

static void EV21_MESSAGE(void) {

  switch (gLunpaFortressRandomPick) {
  case 0:
    MESSAGE_SET(MsgLunpaHammetGreatMerchant);
    MESSAGE_PUT(21, 0);
    break;
  case 1:
    MESSAGE_SET(MsgLunpaZZZ);
    MESSAGE_PUT(21, 0);
    break;
  case 2:
    MESSAGE_SET(MsgLunpaDodonpasOrdersAbsolute);
    MESSAGE_PUT(21, 0);
    break;
  case 3:
    MESSAGE_SET(MsgLunpaSighBadCouldnt);
    MESSAGE_PUT(21, 0);
    break;
  case 4:
    MESSAGE_SET(MsgLunpaStrangeSwearSomeone);
    MESSAGE_PUT(21, 0);
    break;
  case 6:
    MESSAGE_SET(MsgLunpaToldStandGuard);
    MESSAGE_PUT(21, 0);
    break;
  case 7:
    MESSAGE_SET(MsgLunpaWhoDisruptingSleep);
    MESSAGE_PUT(21, 0);
    break;
  case 5:
    Engine_ActorFaceDirection(21, 0xd000, 0);
    WAIT(50);
    Engine_ActorFaceDirection(21, 0xb000, 0);
    WAIT(50);
    Engine_ActorFaceDirection(21, 0x5000, 0);
    WAIT(50);
    MESSAGE_SET(MsgLunpaTakeCareAnybody);
    MESSAGE_PUT(21, 0);
    break;
  }
}

static void EV21_MAIN(void) {

  s32 msg;

  OBJ_EMOTE(21, 0x101, 30);
  OBJ_DIR(21, 0xd000, 0);
  WAIT(50);
  OBJ_DIR(21, 0xb000, 0);
  WAIT(50);
  OBJ_DIR(21, 0x5000, 0);
  WAIT(50);
  msg = MsgLunpaLeftGuardHearsSomeone;
  MESSAGE_SET(msg);
  MESSAGE_PUT(21, 0);
  OBJ_MODE(21, 4);
  WAIT(60);
  OBJ_DIR(21, 0xb000, 0);
  WAIT(40);
  MESSAGE_SET(msg + 1);
  MESSAGE_PUT(21, 0);
}

static void EV25_DONPA(void) {
  s32 msg;
  s32 msg2;

  EV_BEGIN();
  if (FLAG_CHECK(0x941) != 0) {
    MESSAGE_SET(MsgLunpaDonpaGrateful);
    MESSAGE_PUT(18, 0);
    EV_END();
  } else {
    if (FLAG_CHECK(0x313) != 0) {
      MESSAGE_SET(MsgLunpaMaybeDodonpasEyes);
      MESSAGE_OPEN(25, 0);
      EV_END();
    } else {
      OBJ_EMOTE(25, 0x102, 30);
      OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
      msg = MsgLunpaShhhPleaseDont;
      MESSAGE_SET(msg);
      MESSAGE_PUT(25, 0);
      OBJ_FACE(25, 24, 0);
      Engine_CameraMoveToActor(24, 1);
      Engine_CameraWaitForMove();
      WAIT(60);
      Engine_CameraMoveToActor(0, 1);
      WAIT(20);
      OBJ_EMOTE(25, 0x105, 60);
      MESSAGE_SET(msg + 1);
      MESSAGE_PUT(25, 0);
      OBJ_EMOTE(25, 0x107, 60);
      MESSAGE_SET(msg + 2);
      MESSAGE_PUT(25, 0);
      WAIT(70);
      OBJ_EMOTE(25, 0x100, 60);
      OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
      MESSAGE_SET(msg + 3);
      MESSAGE_OPEN(25, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        MESSAGE_SET(msg + 4);
        MESSAGE_OPEN(25, 0);
      } else {
        MESSAGE_SET(msg + 5);
        MESSAGE_OPEN(25, 0);
      }
      WAIT(60);
      OBJ_EMOTE(25, 0x105, 60);
      msg2 = MsgLunpaDonpaKnowsCoddled;
      MESSAGE_SET(msg2);
      MESSAGE_OPEN(25, 0);
      Engine_ActorRunRepeatedMotion(25, 1);
      MESSAGE_SET(msg2 + 1);
      MESSAGE_OPEN(25, 0);
      Engine_ActorSetAnimationAndWait(25, 3);
      MESSAGE_SET(msg2 + 2);
      MESSAGE_OPEN(25, 0);
      FLAG_SET(0x313);
      EV_END();
    }
  }
}

static void EV25_SELECT(void) {
  if (Engine_GameFlagIsSet(0x941)) {
    MESSAGE_SET(MsgLunpaDifficultDonpaRight);
    MESSAGE_PUT(25, 0);
  } else {
    MESSAGE_SET(MsgLunpaSomeonePunishDodonpa);
    MESSAGE_PUT(25, 0);
  }
}

static void EV24_SELECT(void) {
  if (Engine_GameFlagIsSet(0x941)) {
    MESSAGE_SET(MsgLunpaFatherStayAngry);
    MESSAGE_PUT(24, 0);
  } else {
    MESSAGE_SET(MsgLunpaFatherSorryDodonpa);
    MESSAGE_PUT(24, 0);
  }
}

static void EV24_MAIN(void) {
  s32 sequence_id;

  if (FLAG_CHECK(2369) != 0) {
    if (FLAG_CHECK(2382) == 0 && FLAG_CHECK(788) == 0) {
      sequence_id = MsgLunpaThankHelpDodonpa;
      MESSAGE_SET(sequence_id);
      MESSAGE_PUT(PRIMARY_ID, 0);
      Engine_ActorRunRepeatedMotion(PRIMARY_ID, 1);
      WAIT(30);
      OBJ_SPEED(PRIMARY_ID, 6553, 3276);
      Actor_SetDestinationOffset(PRIMARY_ID, -4, 0);
      Engine_ActorWaitForMove(PRIMARY_ID);
      OBJ_MODE(PRIMARY_ID, 3);
      WAIT(60);
      OBJ_SPEED(PRIMARY_ID, 13107, 6553);
      Actor_SetDestinationOffset(PRIMARY_ID, -6, 0);
      OBJ_FACE(PRIMARY_ID, 0, 0);
      Engine_ActorWaitForMove(PRIMARY_ID);
      MESSAGE_SET(sequence_id + 1);
      MESSAGE_PUT(PRIMARY_ID, 0);
      Engine_ActorRunRepeatedMotion(PRIMARY_ID, 1);
      OBJ_FACE(DERIVED_ID, PRIMARY_ID, 0);
      MESSAGE_SET(sequence_id + 2);
      MESSAGE_PUT(PRIMARY_ID, 0);
      WAIT(70);
      OBJ_MODE(DERIVED_ID, 3);
      WAIT(60);
      OBJ_SPEED(DERIVED_ID, 26214, 13107);
      OBJ_WALK(DERIVED_ID, 880, 112);
      Engine_ActorWaitForMove(DERIVED_ID);
      OBJ_DIR(DERIVED_ID, 53248, 0);
      MESSAGE_SET(sequence_id + 3);
      MESSAGE_PUT(PRIMARY_ID, 0);
      OBJ_MODE(PRIMARY_ID, 3);
      WAIT(70);
      Actor_SetDestinationOffset(PRIMARY_ID, 8, 0);
      Engine_ActorWaitForMove(PRIMARY_ID);
      OBJ_MODE(PRIMARY_ID, 5);
      MESSAGE_SET(sequence_id + 4);
      MESSAGE_PUT(PRIMARY_ID, 0);
      OBJ_WALK(0, 896, 120);
      Engine_ActorWaitForMove(0);
      Engine_ActorFaceEachOther(0, DERIVED_ID, 0);
      WAIT(60);
      OBJ_MODE(DERIVED_ID, 3);
      WAIT(30);
      FLAG_SET(788);
    } else {
      MESSAGE_SET(MsgLunpaWellWorriedDodonpa);
      MESSAGE_PUT(PRIMARY_ID, 0);
    }
  } else {
    MESSAGE_SET(MsgLunpaZZZZ);
    MESSAGE_PUT(PRIMARY_ID, 0);
  }
}

static void EV26_SET(void) {
  Engine_BattleFxRunPageEffectForSlot(26, 1, 5);
  Engine_GameFlagSet(0x94e);
}

static void EV14_SET(void) {
  Engine_ActorRunRepeatedMotion(14, 2);
  MESSAGE_SET(MsgLunpaOwwwDontHurt);
  MESSAGE_PUT(14, 0);
}

static void EV13_SET(void) {
  Engine_ActorRunRepeatedMotion(13, 2);
  MESSAGE_SET(MsgLunpaRightRightGive);
  MESSAGE_PUT(13, 0);
}

static void EV12_SET(void) {
  Engine_ActorRunRepeatedMotion(12, 2);
  MESSAGE_SET(MsgLunpaGuysTougherThought);
  MESSAGE_PUT(12, 0);
}

static void EV18_SET(void) {
  MESSAGE_SET(MsgLunpaKnowWhereDodonpa);
  Engine_EventAskYesNo(18, 0);
}

static void EV20_MAIN(void) {
  s32 msg;
  s32 msg2;

  if (FLAG_CHECK(0x226) != 0) {
    MESSAGE_SET(MsgLunpaWontTellAnyone);
    MESSAGE_PUT(20, 0);
  } else {
    EV_BEGIN();
    OBJ_FACE(20, ACTOR_PARTY_LEADER, 0);
    if (FLAG_CHECK(0x227) == 0) {
      Engine_ActorJump(20, 4, 0);
      Engine_ActorStop(20);
      Object_RefreshSelectorById(20);
      WAIT(20);
      msg = MsgLunpaWhWhWho;
      MESSAGE_SET(msg);
      MESSAGE_PUT(20, 0);
      OBJ_EMOTE(20, 0x102, 30);
      MESSAGE_SET(msg + 1);
      MESSAGE_PUT(20, 0);
      WAIT(30);
      OBJ_MODE(20, 4);
      WAIT(30);
    }
    msg2 = MsgLunpaDontLookNearly;
    MESSAGE_SET(msg2);
    MESSAGE_PUT(20, 0);
    OBJ_EMOTE(20, 0x101, 40);
    MESSAGE_SET(msg2 + 1);
    MESSAGE_OPEN(20, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      MESSAGE_SET(msg2 + 2);
      MESSAGE_OPEN(20, 0);
      FLAG_SET(0x226);
    } else {
      MESSAGE_SET(msg2 + 3);
      MESSAGE_OPEN(20, 0);
    }
    FLAG_SET(0x227);
    EV_END();
  }
}

static void EV20_END(void) {

  if (Engine_GameFlagIsSet(0x226)) {
    MESSAGE_SET(MsgLunpaMaybeMerchantReason);
    MESSAGE_PUT(20, 0);
  } else {
    s16 *q = &gEventWork->psynergyRequest;

    *q = 0;
    Engine_PsynergyCancel();
    EV20_MAIN();
  }
}

static void NoOpActorCallback(void) {}

static void EV17_TALK(void) {
  s32 msg = MsgLunpaWrongTurnOver;

  MESSAGE_SET(msg);
  MESSAGE_PUT(0x800d, 0);
  if (Engine_PartyInventoryFindOwner(234) != -1) {
    Engine_MessageShowCentered(msg + 2, 1);
  }
}

static void EV17_LOAD(void) {
  MESSAGE_SET(MsgLunpaCantBelieveWhen);
  MESSAGE_PUT(13, 0);
}

s32 FieldScene_DispatchActorUpdate(void) {
  struct FieldActor *actor;

  gLunpaFortressRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
  if (gGameState.scene == (s32)&SceneId_LunpaFortress1) {
    Engine_MapSetWorkFlagBits9To11(0xe00);
    FieldScene_InstallSceneTasks();
  }
  if (gGameState.scene == (s32)&SceneId_LunpaFortress2)
    FieldScene_SetupActorsForScene();
  if (gGameState.scene == (s32)&SceneId_LunpaFortress3)
    FieldScene_RestoreActorsFromFlags();
  if (gGameState.scene == (s32)&SceneId_LunpaFortress4) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_ActorSetSpriteFlags(Actor_Get(12), 0);
    OBJ_DIR(12, 0, 0);
    OBJ_MODE(12, 0);
    ActivateFiveActorGroupFromFlags();
    actor = Actor_Get(8);
    if (actor != 0)
      Engine_ActorSetSpriteFlags(actor, 0);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(9);
    if (actor != 0)
      Engine_ActorSetSpriteFlags(actor, 0);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor = Actor_Get(10);
    if (actor != 0)
      Engine_ActorSetSpriteFlags(actor, 0);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_MapSetWorkFlagBits9To11(0xe00);
    if (gGameState.entrance == 4) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      FieldScene_RunMainScriptSequence();
    }
    if (gGameState.entrance == 3) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      if (FLAG_CHECK(0x941) != 0) {
        OBJ_POS_SET(12, 0, 0);
        OBJ_POS_SET(16, 432, 344);
        OBJ_DIR(16, 0x5000, 0);
        OBJ_POS_SET(13, 456, 288);
        OBJ_DIR(13, 0x5000, 0);
        OBJ_POS_SET(17, 456, 320);
        Engine_ActorSetSpriteFlags(Actor_Get(17), 0);
      }
    }
    actor = Actor_Get(15);
    if (actor != 0)
      Engine_ActorSetSpriteFlags(actor, 0);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->scaleX = 0xcccc;
  }
  return 0;
}

static void FieldScene_InstallSceneTasks(void) {
  FieldScene_ActivateThreeActorGroup();
  switch (gGameState.entrance) {
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Task_AddCallback((s32)TriggerSceneStage95FromActor12, 3200);
    Task_AddCallback((s32)FieldScene_RunScene3bfSequenceB, 3200);
    Task_AddCallback((s32)FieldScene_RunScene3bfSequenceC, 3200);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  case 12:
  case 19:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_MapSetWorkFlagBits9To11(0xc00);
    break;
  case 16:
  case 17:
  case 18:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Task_AddCallback((s32)FieldScene_UpdateActorEighteenInteraction, 3200);
    Task_AddCallback((s32)TriggerScene41AtVillagePath, 3200);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Map_CopyCells(101, 9, 10, 8, 110, 9);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  case 13:
  case 14:
  case 15:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Task_AddCallback((s32)FieldScene_RunScene3bfSequenceA, 3200);
    break;
  default:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  }
  Engine_ActorSetChildValue(18, 1);
  Engine_ActorSetChildValue(17, 1);
  Engine_ActorSetChildValue(21, 1);
  Engine_ActorSetChildValue(12, 1);
  Engine_ActorSetChildValue(13, 1);
  Engine_TaskWait(1);
}

static void FieldScene_SetupActorsForScene(void) {
  struct FieldActor *actor;

  FieldScene_ActivateTwoActorGroup();
  Engine_ActorSetChildValue(9, 1);
  Engine_ActorSetChildValue(10, 1);
  Engine_ActorSetChildValue(17, 1);
  if (FLAG_CHECK(0x94c)) {
    OBJ_POS_SET(15, 0, 0);
  }
  if (FLAG_CHECK(0x949)) {
    OBJ_POS_SET(11, 0, 0);
  }
  if (FLAG_CHECK(0x94b)) {
    OBJ_POS_SET(16, 0, 0);
  }
  if (FLAG_CHECK(0xf2e)) {
    OBJ_POS_SET(8, 0, 0);
  }
  switch (gGameState.entrance) {
  case 1:
  case 2:
  case 3:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    Task_AddCallback(FieldScene_UpdateActorPairInteraction, 3200);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    break;
  case 10:
  case 13:
  case 20:
  case 23:
  case 24:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_MapSetWorkFlagBits9To11(0xc00);
    Engine_ActorSetSpriteFlags(Actor_Get(24), 0);
    if (FLAG_CHECK(0x314)) {
      OBJ_POS_SET(25, 872, 120);
    }
    break;
  case 21:
  case 22:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    Task_AddCallback(FieldScene_UpdateActorSeventeenInteraction, 3200);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    break;
  case 11:
  case 12:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    if (FLAG_CHECK(0x94a)) {
      FieldScene_RunSequenceTail();
    }
    break;
  case 31:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    FieldScene_RunSequenceTail();
    break;
  case 14:
  case 15:
  case 16:
    Task_AddCallback(TriggerScene40AtVillagePath, 3200);
    break;
  default:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  }
  actor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
  Engine_ActorSetSpritePriority(8, 1);
  actor->scaleX = 0xc000;
  actor->scaleY = 0xc000;
}

static void FieldScene_RestoreActorsFromFlags(void) {
  struct FieldActor *actor;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  FieldScene_ActivateAlternateActorGroup();
  if (FLAG_CHECK(0x943)) {
    PlaceActorTwelveAndFinishScene();
  }
  FLAG_SET(0x217);
  FLAG_SET(0x218);
  if (FLAG_CHECK(0x944)) {
    OBJ_POS_SET(8, 0, 0);
    Engine_GameFlagClear(0x217);
  }
  if (FLAG_CHECK(0x945)) {
    OBJ_POS_SET(9, 0, 0);
    ConfigureInteractionRegionC();
  }
  if (FLAG_CHECK(0x946)) {
    OBJ_POS_SET(10, 0, 0);
    Engine_GameFlagClear(0x218);
  }
  if (FLAG_CHECK(0x947)) {
    ConfigureInteractionRegionA();
  }
  if (FLAG_CHECK(0x948)) {
    ConfigureInteractionRegionB();
  }
  EV_BEGIN();
  actor = Actor_Get(8);
  if (actor != 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  actor = Actor_Get(9);
  if (actor != 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  actor = Actor_Get(10);
  if (actor != 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  actor = Actor_Get(11);
  if (actor != 0) {
    Engine_ActorSetSpriteFlags(actor, 0);
  }
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  actor = Actor_Get(12);
  if (actor != 0) {
    actor->collisionFlags |= 0x10;
  }
  Engine_ActorSetSpriteFlags(Actor_Get(11), 0);
  EV_END();
  Engine_MapSetWorkFlagBits9To11(0xe00);
}

static void FieldScene_ActivateThreeActorGroup(void) {
  if (Engine_GameFlagIsSet(0x35a)) {
    PlaceSceneObjectPairFromTableC(0);
  }
  if (Engine_GameFlagIsSet(0x35b)) {
    PlaceSceneObjectPairFromTableC(1);
  }
  if (Engine_GameFlagIsSet(0x35c)) {
    PlaceSceneObjectPairFromTableC(2);
  }
}

static void FieldScene_ActivateTwoActorGroup(void) {
  if (Engine_GameFlagIsSet(0x358)) {
    PlaceSceneObjectPairFromTableB(0);
  }
  if (Engine_GameFlagIsSet(0x359)) {
    PlaceSceneObjectPairFromTableB(1);
  }
}

static void FieldScene_ActivateAlternateActorGroup(void) {
  if (Engine_GameFlagIsSet(0x355)) {
    FieldScene_SetPositionPairs(0);
  }
  if (Engine_GameFlagIsSet(0x356)) {
    FieldScene_SetPositionPairs(1);
  }
  if (Engine_GameFlagIsSet(0x357)) {
    FieldScene_SetPositionPairs(2);
  }
}

static void ActivateFiveActorGroupFromFlags(void) {
  if (Engine_GameFlagIsSet(0x350)) {
    PlaceSceneObjectPairFromTableA(0);
  }
  if (Engine_GameFlagIsSet(0x351)) {
    PlaceSceneObjectPairFromTableA(1);
  }
  if (Engine_GameFlagIsSet(0x352)) {
    PlaceSceneObjectPairFromTableA(2);
  }
  if (Engine_GameFlagIsSet(0x353)) {
    PlaceSceneObjectPairFromTableA(3);
  }
  if (Engine_GameFlagIsSet(0x354)) {
    PlaceSceneObjectPairFromTableA(4);
  }
}

// 共通ソースと field.h の宣言は const でないが、この三つの表は ROM では .rodata
// にある 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

/* FAKEMATCH: the three section attributes below place non-const tables in
 * .rodata, where the ROM has them; without them, tbs-en differs, and const
 * conflicts with field.h */
s32 gStagedActor_DirectionSteps[] __attribute__((section(".rodata"))) = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] __attribute__((section(".rodata"))) = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] __attribute__((section(".rodata"))) = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gLunpaFortressRandomPick = 0;

s32 gLunpaFortressPlacements2_1[] = {
    SC_SET(SW_ACCEL, 0x11999),
    SC_SET(SW_SPEED, 0x11999),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(288)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(288)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLunpaFortressPlacements2_0[] = {
    SC_SET(SW_ACCEL, 0x11999),
    SC_SET(SW_SPEED, 0x11999),
    SC_ARG(2),
    SC_MOVE(PIXELS(192), 0, PIXELS(192)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(88), 0, PIXELS(192)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLunpaFortressPlacements2_3[] = {
    SC_ARG(5),
    SC_END,
};

s32 gLunpaFortressPlacements1_Data[] = {
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x9999),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(25),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(712), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(25),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(520)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(40),
    SC_ARG(2),
    SC_MOVE(PIXELS(808), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(20),
    SC_ARG(2),
    SC_MOVE(PIXELS(744), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gLunpaFortressPlacements2_2[] = {
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_SPEED, 0x9999),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(592)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(840), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(648)),
    SC_SYNC,
    SC_ARG(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(920), 0, PIXELS(592)),
    SC_SYNC,
    SC_ARG(1),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gLunpaFortressEntrances1[] = {
    { 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 160, 0, 168, FACING_NORTH, 0, 40, 24, 296, 280, 0 },
    { 2, CONDITION_ALWAYS, 160, 0, 104, FACING_SOUTH, 0, 40, 24, 296, 280, 0 },
    { 3, CONDITION_ALWAYS, 272, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { 4, CONDITION_ALWAYS, 376, 0, 896, FACING_NORTH, 0, 24, 616, 536, 936, 0 },
    { 5, CONDITION_ALWAYS, 248, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { 6, CONDITION_ALWAYS, 488, 0, 680, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { 7, CONDITION_ALWAYS, 72, 0, 696, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { 8, CONDITION_ALWAYS, 96, 0, 856, FACING_SOUTH, 0, 24, 616, 536, 936, 0 },
    { 9, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { 10, CONDITION_ALWAYS, 504, 0, 88, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { 11, CONDITION_ALWAYS, 440, 0, 104, FACING_SOUTH, 0, 328, 8, 568, 232, 0 },
    { 12, CONDITION_ALWAYS, 136, 0, 512, FACING_NORTH, 0, 40, 328, 280, 552, 0 },
    { 13, CONDITION_ALWAYS, 888, 0, 280, FACING_NORTH, 0, 632, 40, 952, 328, 0 },
    { 14, CONDITION_ALWAYS, 696, 0, 264, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { 15, CONDITION_ALWAYS, 824, 0, 120, FACING_SOUTH, 0, 632, 40, 952, 328, 0 },
    { 16, CONDITION_ALWAYS, 664, 0, 632, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { 17, CONDITION_ALWAYS, 888, 0, 520, FACING_NORTH, 0, 616, 344, 936, 696, 0 },
    { 18, CONDITION_ALWAYS, 696, 0, 432, FACING_SOUTH, 0, 616, 344, 936, 696, 0 },
    { 19, CONDITION_ALWAYS, 440, 0, 448, FACING_NORTH, 0, 320, 312, 560, 504, 0 },
    { 20, CONDITION_ALWAYS, 696, 0, 824, FACING_NORTH, 0, 616, 744, 856, 952, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressEntrances2[] = {
    { 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 136, 0, 392, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { 2, CONDITION_ALWAYS, 136, 0, 80, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { 3, CONDITION_ALWAYS, 264, 0, 416, FACING_SOUTH, 0, 40, 8, 308, 448, 0 },
    { 4, CONDITION_ALWAYS, 376, 0, 232, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { 5, CONDITION_ALWAYS, 504, 0, 104, FACING_NORTH, 0, 328, 8, 568, 272, 0 },
    { 6, CONDITION_ALWAYS, 376, 0, 176, FACING_SOUTH, 0, 328, 8, 568, 272, 0 },
    { 7, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { 8, CONDITION_ALWAYS, 456, 0, 352, FACING_SOUTH, 0, 328, 272, 584, 512, 0 },
    { 9, CONDITION_ALWAYS, 504, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { 10, CONDITION_ALWAYS, 136, 0, 640, FACING_NORTH, 0, 16, 472, 256, 688, 0 },
    { 11, CONDITION_ALWAYS, 648, 0, 72, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { 12, CONDITION_ALWAYS, 744, 0, 80, FACING_SOUTH, 0, 568, 16, 808, 176, 0 },
    { 13, CONDITION_ALWAYS, 696, 0, 376, FACING_NORTH, 0, 576, 264, 816, 432, 0 },
    { 14, CONDITION_ALWAYS, 200, 0, 848, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { 15, CONDITION_ALWAYS, 264, 0, 776, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { 16, CONDITION_ALWAYS, 376, 0, 824, FACING_SOUTH, 0, 152, 680, 424, 992, 0 },
    { 17, CONDITION_ALWAYS, 536, 0, 696, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { 18, CONDITION_ALWAYS, 680, 0, 832, FACING_SOUTH, 0, 488, 584, 760, 896, 0 },
    { 19, CONDITION_ALWAYS, 712, 0, 856, FACING_NORTH, 0, 488, 584, 760, 896, 0 },
    { 20, CONDITION_ALWAYS, 912, 0, 192, FACING_NORTH, 0, 784, 24, 1024, 240, 0 },
    { 21, CONDITION_ALWAYS, 840, 0, 664, FACING_NORTH, 0, 768, 440, 1008, 704, 0 },
    { 22, CONDITION_ALWAYS, 952, 0, 528, FACING_SOUTH, 0, 768, 440, 1008, 704, 0 },
    { 23, CONDITION_ALWAYS, 872, 0, 376, FACING_NORTH, 0, 768, 264, 1016, 432, 0 },
    { 24, CONDITION_ALWAYS, 904, 0, 112, FACING_SOUTH, 0, 784, 24, 1024, 240, 0 },
    { 30, CONDITION_ALWAYS, 408, 0, 456, FACING_NORTH, 0, 328, 272, 584, 512, 0 },
    { 31, CONDITION_ALWAYS, 648, 0, 88, FACING_NORTH, 0, 568, 16, 808, 176, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressEntrances3[] = {
    { 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 424, 0, 584, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { 2, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 200, 488, 480, 648, 0 },
    { 3, CONDITION_ALWAYS, 312, 0, 448, FACING_NORTH, 0, 8, 8, 568, 480, 0 },
    { 4, CONDITION_ALWAYS, 440, 0, 296, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { 5, CONDITION_ALWAYS, 312, 0, 248, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { 6, CONDITION_ALWAYS, 104, 0, 184, FACING_SOUTH, 0, 8, 8, 568, 480, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gLunpaFortressEntrancesOther[] = {
    { 0, CONDITION_ALWAYS, 168, 0, 632, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 56, 0, 328, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { 2, CONDITION_ALWAYS, 184, 0, 368, FACING_NORTH, 0, 8, 8, 264, 424, 0 },
    { 3, CONDITION_ALWAYS, 328, 0, 344, FACING_SOUTH, 0, 288, 32, 576, 432, 0 },
    { 4, CONDITION_ALWAYS, 440, 0, 224, FACING_NORTH, 0, 288, 32, 576, 432, 0 },
    { SCENE_TABLE_END },
};

u32 gLunpaFortressExitsOther[] = {
    SCENE_EXITS(106),
    SCENE_EXIT(1, 2, 49),
    SCENE_EXIT(2, 104, 3),
    SCENE_EXIT(3, 99, 11),
    SCENE_EXIT(0, 160, 0),
    SCENE_EXIT(1, 159, 1),
    SCENE_EXIT(60, 159, 4),
    SCENE_EXIT(2, 160, 3),
    SCENE_EXIT(3, 160, 2),
    SCENE_EXIT(4, 160, 9),
    SCENE_EXIT(5, 160, 12),
    SCENE_EXIT(6, 160, 13),
    SCENE_EXIT(7, 160, 16),
    SCENE_EXIT(8, 161, 1),
    SCENE_EXIT(9, 160, 4),
    SCENE_EXIT(10, 161, 16),
    SCENE_EXIT(11, 162, 1),
    SCENE_EXIT(12, 160, 5),
    SCENE_EXIT(13, 160, 6),
    SCENE_EXIT(14, 160, 17),
    SCENE_EXIT(15, 160, 19),
    SCENE_EXIT(16, 160, 7),
    SCENE_EXIT(17, 160, 14),
    SCENE_EXIT(18, 160, 20),
    SCENE_EXIT(19, 160, 15),
    SCENE_EXIT(20, 160, 18),
    SCENE_EXITS_END,
};

u32 gLunpaFortressExits2[] = {
    SCENE_EXITS(161),
    SCENE_EXIT(60, 159, 4),
    SCENE_EXIT(1, 160, 8),
    SCENE_EXIT(2, 161, 4),
    SCENE_EXIT(3, 161, 7),
    SCENE_EXIT(4, 161, 2),
    SCENE_EXIT(5, 161, 11),
    SCENE_EXIT(6, 161, 13),
    SCENE_EXIT(7, 161, 3),
    SCENE_EXIT(8, 161, 10),
    SCENE_EXIT(9, 161, 14),
    SCENE_EXIT(10, 161, 8),
    SCENE_EXIT(11, 161, 5),
    SCENE_EXIT(12, 161, 17),
    SCENE_EXIT(13, 161, 6),
    SCENE_EXIT(14, 161, 9),
    SCENE_EXIT(15, 161, 21),
    SCENE_EXIT(16, 160, 10),
    SCENE_EXIT(17, 161, 12),
    SCENE_EXIT(18, 161, 20),
    SCENE_EXIT(19, 161, 22),
    SCENE_EXIT(20, 161, 18),
    SCENE_EXIT(21, 161, 15),
    SCENE_EXIT(22, 161, 19),
    SCENE_EXIT(23, 161, 24),
    SCENE_EXIT(24, 161, 23),
    SCENE_EXITS_END,
};

u32 gLunpaFortressExits3And4[] = {
    SCENE_EXITS(162),
    SCENE_EXIT(1, 160, 11),
    SCENE_EXIT(2, 162, 3),
    SCENE_EXIT(3, 162, 2),
    SCENE_EXIT(4, 163, 2),
    SCENE_EXIT(5, 163, 1),
    SCENE_EXIT(6, 163, 3),
    SCENE_EXIT(60, 159, 4),
    SCENE_EXIT(0, 163, 0),
    SCENE_EXIT(1, 162, 5),
    SCENE_EXIT(2, 162, 4),
    SCENE_EXIT(5, 162, 6),
    SCENE_EXIT(4, 159, 3),
    SCENE_EXIT(60, 159, 4),
    SCENE_EXITS_END,
};

struct ScenePlacement gLunpaFortressPlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacementsWestLunpaCave[] = {
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(344), 0, PIXELS(252), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(552), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x44, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacements1[] = {
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x14, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(760), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(104), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(432), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(232), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, (s32)gLunpaFortressPlacements1_Data,
        PIXELS(736), 0, PIXELS(648), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(192), 0, PIXELS(464), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(424), 0, PIXELS(392), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(432), 0, PIXELS(768), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacements2[] = {
    { 0xf5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(264), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x85, CONDITION_ALWAYS, (s32)gLunpaFortressPlacements2_0,
        PIXELS(88), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, (s32)gLunpaFortressPlacements2_1,
        PIXELS(88), 0, PIXELS(288), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(88), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(704), 0, PIXELS(104), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(128), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(312), 0, PIXELS(840), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(680), 0, PIXELS(824), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x85, CONDITION_ALWAYS, (s32)gLunpaFortressPlacements2_2,
        PIXELS(920), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(616), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(560), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x66, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(148), 0, PIXELS(616), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(560), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(664), 0, PIXELS(360), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2c, CONDITION_ALWAYS, (s32)gLunpaFortressPlacements2_3,
        PIXELS(944), 0, PIXELS(122), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(904), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x5b, 0x49, ACTOR_STAND,
        PIXELS(872), 0, PIXELS(336), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacements3[] = {
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(576), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(440), 0, PIXELS(282), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(176), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(504), 0, PIXELS(168), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(248), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gLunpaFortressPlacements4[] = {
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(56), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(72), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0xbb, CONDITION_ALWAYS, 7,
        PIXELS(472), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x31, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(528), 0, PIXELS(216), FACING_WEST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x32, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x2c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
    { 0x120, CONDITION_ALWAYS, 7,
        PIXELS(464), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x16, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gLunpaFortressNoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { 0x21, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { 0x21, 10, CONDITION_ALWAYS, 10 },
    { 0x31, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { EVENT_EXIT, 16, CONDITION_ALWAYS, 16 },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_JAIL) },
    { 0xc402, 40, 0x35a, EVENT_SCRIPT(EV40_C) },
    { 0x4402, 40, 0x35a, EVENT_SCRIPT(EV40_C) },
    { 0xc402, 41, 0x35b, EVENT_SCRIPT(EV40_C) },
    { 0x4402, 41, 0x35b, EVENT_SCRIPT(EV40_C) },
    { 0xc402, 42, 0x35c, EVENT_SCRIPT(EV40_C) },
    { 0x4402, 42, 0x35c, EVENT_SCRIPT(EV40_C) },
    { EVENT_TOUCH, 50, 0x224, EVENT_SCRIPT(EV50_MAIN) },
    { EVENT_TOUCH, 51, 0x225, EVENT_SCRIPT(EV51_MAIN) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaResting) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLunpaWhereIsDodonpa) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetBuysDrink) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetMoneyTree) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_MAIN) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_MAIN) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLunpaWhoFoughtDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLunpaWifeSeeksDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLunpaKalayBuysDrink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetKeepsEarnings) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_END) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_MESSAGE) },
    { EVENT_RAISED, 93, CONDITION_ALWAYS, EVENT_SCRIPT(EV93_ON) },
    { EVENT_RAISED, 94, CONDITION_ALWAYS, EVENT_SCRIPT(EV94_MAIN) },
    { EVENT_RAISED, 95, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_MAIN) },
    { EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(EV51_MAIN) },
    { SEARCH(SEARCH_CHEST), 100, 0xfa2, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_CHEST), 102, 0xfa4, FIND_ITEM(0xaa) },
    { SEARCH(SEARCH_CHEST), 103, 0xfa5, FIND_COINS(100) },
    { SEARCH(SEARCH_BARREL), 104, 0xfa6, FIND_ITEM(ITEM_VIAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEvents2[] = {
    { 0x31, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
    { EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
    { EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
    { EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
    { EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
    { EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
    { EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
    { 0x31, 16, CONDITION_ALWAYS, 16 },
    { EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
    { EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
    { EVENT_EXIT, 19, CONDITION_ALWAYS, 19 },
    { EVENT_EXIT, 20, CONDITION_ALWAYS, 20 },
    { EVENT_EXIT, 21, CONDITION_ALWAYS, 21 },
    { EVENT_EXIT, 22, CONDITION_ALWAYS, 22 },
    { EVENT_EXIT, 23, CONDITION_ALWAYS, 23 },
    { EVENT_EXIT, 24, CONDITION_ALWAYS, 24 },
    { EVENT_TOUCH, 30, 0x949, EVENT_SCRIPT(EV30_TREASURE_CHEST) },
    { EVENT_TOUCH, 31, 0x94a, EVENT_SCRIPT(EV31_GUARD) },
    { EVENT_TOUCH, 32, 0x94b, EVENT_SCRIPT(EV32_TOUCH) },
    { EVENT_TOUCH, 33, 0x94c, EVENT_SCRIPT(EV33_CHECK) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_JAIL) },
    { 0x4402, 40, 0x358, EVENT_SCRIPT(EV40_D) },
    { 0xc402, 41, 0x359, EVENT_SCRIPT(EV40_D) },
    { 0x4402, 41, 0x359, EVENT_SCRIPT(EV40_D) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_KEY) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_KEY) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_SET) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLunpaChildMustGrowUp) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgLunpaPapaWatchesLadies) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgLunpaChildTooResponsible) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaExhausted) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgLunpaWomenWantHome) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_SET) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_SET) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_SET) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_DONPA) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_MAIN) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_SET) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaSuccessor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaProtectsFortress) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLunpaDaughterWillNotMarryPapa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaJudges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgLunpaFearWithoutDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgLunpaHopeDonpaRecovers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLunpaShouldNotHaveFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetInCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLunpaHiddenDoorSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SELECT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SELECT) },
    { EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(EV91_SET) },
    { EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(EV92_ON) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEvents3[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x31, 6, CONDITION_ALWAYS, 6 },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_JAIL) },
    { SEARCH(SEARCH_UNNAMED), 98, CONDITION_ALWAYS, EVENT_SCRIPT(EV98_DOOR) },
    { 0xc402, 40, 0x355, EVENT_SCRIPT(EV40_B) },
    { 0xc402, 41, 0x356, EVENT_SCRIPT(EV40_B) },
    { 0xc402, 42, 0x357, EVENT_SCRIPT(EV40_B) },
    { 0x4402, 42, 0x357, EVENT_SCRIPT(EV40_B) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS, EVENT_SCRIPT(EV50_START) },
    { 0x202, 70, CONDITION_ALWAYS, EVENT_SCRIPT(EV70_INIT) },
    { SEARCH(SEARCH_UNNAMED), 51, 0x218, EVENT_SCRIPT(EV51_SUB) },
    { SEARCH(SEARCH_UNNAMED), 52, 0x217, EVENT_SCRIPT(EV52_WELL) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EV00_REVEAL1) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EV00_REVEAL2) },
    { PSYNERGY_ON_ACTOR(0x4e), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_WARDROBE) },
    { PSYNERGY_ON_ACTOR(0x4e), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_FIREWOOD) },
    { PSYNERGY_ON_ACTOR(0x4e), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_BOOK) },
    { PSYNERGY_ON_ACTOR(0x8c), 12, 0x943, EVENT_SCRIPT(EV12_MOVE) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 5, CONDITION_ALWAYS, 5 },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(EV99_JAIL) },
    { 0xc402, 40, 0x350, EVENT_SCRIPT(EV40_A) },
    { 0xc402, 41, 0x351, EVENT_SCRIPT(EV40_A) },
    { 0xc402, 42, 0x352, EVENT_SCRIPT(EV40_A) },
    { 0xc402, 43, 0x353, EVENT_SCRIPT(EV40_A) },
    { 0xc402, 44, 0x354, EVENT_SCRIPT(EV40_A) },
    { EVENT_TOUCH, 20, 0x270, EVENT_SCRIPT(EV20_CUTSCENE) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_LOAD) },
    { SEARCH(SEARCH_CHEST), 101, 0xfa7, FIND_ITEM(0xbf) },
    { SCENE_EVENTS_END },
};

u32 gLunpaFortressPairTableA[] = {
    13, 56, 8, 56, 3, 56, 28, 85,
    28, 65,
};

u32 gLunpaFortressSupportPairs[] = {
    31, 61, 15, 57, 6, 61,
};

u32 gLunpaFortressPairTableB[] = {
    16, 119, 16, 113,
};

u32 gLunpaFortressPairTableC[] = {
    28, 72, 43, 92,
};
// clang-format on
