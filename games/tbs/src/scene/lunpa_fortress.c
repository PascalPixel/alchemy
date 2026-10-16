// Lunpa Fortress events.
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
#include "flags.h"

// These roles apply to the rescue room; the other rooms reuse the same slots.
#define RESCUE_HAMMET 12
#define RESCUE_DODONPA 13
#define RESCUE_DONPA 14
#define RESCUE_MONSTER 11
#define RESCUE_MONSTER_PART 15

// The native paired patrol computes this layer offset in floating point.
#define PAIRED_PATROL_LAYER_ORIGIN_X (136.0 * 65536)

// Forward declarations.
static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId);
static void LunpaFortress_CheckSecondEntranceSentry(void);
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId);
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId);
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId);
static void LunpaFortress_ExpelPartyWithGuard(s32 actorId);
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId);
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId);
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId);
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId);
static void LunpaFortress_InitializeOuterRooms(void);
static void LunpaFortress_InitializeGuardRooms(void);
static void LunpaFortress_InitializeColumnRoom(void);
static void LunpaFortress_RestoreOuterDoors(void);
static void LunpaFortress_RestoreGuardRoomDoors(void);
static void LunpaFortress_RestoreColumnRoomDoors(void);
static void LunpaFortress_RestoreRescueRoomDoors(void);

// Shared source is included here in ROM order.
#include "common/object_staged_actor.h"

// Select entrance table for fortress rooms1..3, default other rooms.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressEntrances1;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressEntrances2;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressEntrances3;
  }
  return gLunpaFortressEntrancesOther;
}

// Return empty optional scene-data selector.
s32 GetEmptySceneData(void) { return 0; }

// Choose room 2 exits, shared rooms3/4 exits or default.
const u32 *Scene_GetExits(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressExits2;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress3 ||
      sceneId == (s32)&SceneId_LunpaFortress4) {
    return gLunpaFortressExits3And4;
  }
  return gLunpaFortressExitsOther;
}

// Select WestLunpaCave or fortress room actor placement table.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_WestLunpaCave) {
    return gLunpaFortressPlacementsWestLunpaCave;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressPlacements3;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressPlacements2;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressPlacements1;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress4) {
    return gLunpaFortressPlacements4;
  }
  return gLunpaFortressPlacementsOther;
}

// Select rooms 1 through 3 event dispatch table or common other-room events.
const struct SceneEvent *Scene_GetEvents(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_LunpaFortress1) {
    return gLunpaFortressEvents1;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress2) {
    return gLunpaFortressEvents2;
  }
  if (sceneId == (s32)&SceneId_LunpaFortress3) {
    return gLunpaFortressEvents3;
  }
  return gLunpaFortressEventsOther;
}

// Copy lowered-column cells and disable movement flags on its actor.
static void LunpaFortress_LowerMovableColumn(void) {

  struct FieldActor *columnActor;

  Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
  Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
  columnActor = Object_GetById(12);
  if (columnActor != 0) {
    Engine_ActorSetSpriteFlags(columnActor, 0);
    columnActor->motionFlags = 0;
    columnActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
}

// Forward movement trigger to staged-actor pair push operation.
static void LunpaFortress_PushAdjacentStagedActor(void) {
  StagedActor_AdvancePair();
}

// Walk movable column into lowered position then redraw and persist it.
static void LunpaFortress_LowerColumnAfterPushing(void) {

  OBJ_SPEED(12, 0x10000, 0x8000);
  Actor_SetDestination(12, 248, 376);
  Engine_ActorWaitForMove(12);
  SE(215);
  WAIT(60);
  LunpaFortress_LowerMovableColumn();
  FLAG_SET(0x943);
}

// After Move carries column past row22, shake/redraw it and persist lowered
// state.
static void LunpaFortress_SettleMovedColumn(void) {
  struct FieldActor *columnActor;

  columnActor = Actor_Get(12);
  if ((columnActor->z.fixed >> 20) > 22) {
    Map_SetShake(0x40000, 0x40000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    SE(144);
    Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
    Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
    columnActor = Actor_Get(12);
    if (columnActor != NULL) {
      Engine_ActorSetSpriteFlags(columnActor, 0);
      columnActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    }
    FLAG_SET(0x943);
  }
}

// Restore saved column position and map state.
static void LunpaFortress_RestoreLoweredColumn(void) {
  OBJ_POS_SET(12, 248, 376);
  LunpaFortress_LowerMovableColumn();
}

// Copy door-open cells and surrounding attribute into one indexed door
// location.
static void LunpaFortress_OpenRescueRoomDoor(s32 doorIndex) {

  s32 doorCellX = gLunpaFortressPairTableA[doorIndex * 2];
  s32 doorCellY = gLunpaFortressPairTableA[doorIndex * 2 + 1];

  Map_CopyCells(0, 0x4d, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(1, 0x4d, 1, 1, doorCellX + 1, doorCellY);
  Map_CopyCells(doorCellX, doorCellY - 0x30, 1, 1, doorCellX, doorCellY - 0x2e);
}

// Use Cell Key to open touched rescue-room door, except saved blocked fifth
// door.
static void LunpaFortress_UnlockRescueRoomDoor(void) {

  struct EventWork *fieldWork;
  s32 doorTrigger;
  s32 doorIndex;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    doorIndex = doorTrigger - 40;
    if (FLAG_CHECK(FLAG_LUNPA_TRADE_REOPENED) == 0 || doorIndex != 4) {
      LunpaFortress_OpenRescueRoomDoor(doorIndex);
      SE(157);
      Map_SetShake(0x30000, 0x30000, 0x10000);
      Map_SetShake(-1, -1, 0xe666);
      FLAG_SET(doorTrigger + 0x328);
    }
  }
}

// Open indexed column-room door and copy collision attributes, with extra
// middle-door patch.
static void LunpaFortress_OpenColumnRoomDoor(s32 doorIndex) {
  s32 doorCellX;
  s32 doorCellY;
  s32 collisionCellY;

  doorCellX = gLunpaFortressSupportPairs[doorIndex * 2];
  doorCellY = gLunpaFortressSupportPairs[doorIndex * 2 + 1];
  Map_CopyCells(0, 77, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(1, 77, 1, 1, doorCellX + 1, doorCellY);
  collisionCellY = doorCellY - 44;
  Map_CopyCellAttributes(doorCellX, doorCellY - 45, 1, 1, doorCellX,
                         collisionCellY);
  if (doorIndex == 1)
    Map_CopyCellAttributes(doorCellX, collisionCellY, 1, 1, doorCellX,
                           doorCellY - 43);
}

// Use Cell Key on touched column-room door and persist opening.
static void LunpaFortress_UnlockColumnRoomDoor(void) {

  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenColumnRoomDoor(doorTrigger - 40);
    SE(157);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + 0x32d);
  }
}

// Copy guard-room door-open cells and collision attribute for indexed door.
static void LunpaFortress_OpenGuardRoomDoor(s32 doorIndex) {

  s32 doorCellX = gLunpaFortressPairTableB[doorIndex * 2];
  s32 doorCellY = gLunpaFortressPairTableB[doorIndex * 2 + 1];

  Map_CopyCells(0x37, 0x79, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(0x38, 0x79, 1, 1, doorCellX + 1, doorCellY);
  Map_CopyCells(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX, doorCellY - 0x3e);
}

// Use Cell Key to open touched guard-room door and persist state.
static void LunpaFortress_UnlockGuardRoomDoor(void) {

  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenGuardRoomDoor(doorTrigger - 40);
    SE(157);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + 0x330);
  }
}

// Copy outer-room open-door cells and collision attribute for indexed location.
static void LunpaFortress_OpenOuterFortressDoor(s32 doorIndex) {

  // BUG: trigger42 and saved flag0x35c request index2, beyond these two pairs.
  s32 doorCellX = gLunpaFortressPairTableC[doorIndex * 2];
  s32 doorCellY = gLunpaFortressPairTableC[doorIndex * 2 + 1];

  Map_CopyCells(1, 0x50, 1, 3, doorCellX, doorCellY);
  Map_CopyCells(2, 0x50, 1, 1, doorCellX + 1, doorCellY);
  Engine_MapCopyCellAttributes(doorCellX, doorCellY - 0x3f, 1, 1, doorCellX,
                               doorCellY - 0x3e);
}

// Use Cell Key to open touched outer-fortress door and persist its trigger
// flag.
static void LunpaFortress_UnlockOuterFortressDoor(void) {

  struct EventWork *fieldWork;
  s32 doorTrigger;

  fieldWork = gEventWork;
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    doorTrigger = fieldWork->touchedTrigger;
    LunpaFortress_OpenOuterFortressDoor(doorTrigger - 40);
    SE(157);
    Map_SetShake(0x30000, 0x30000, 0x10000);
    Map_SetShake(-1, -1, 0xe666);
    FLAG_SET(doorTrigger + 0x332);
  }
}

// Show tightly locked door message when Cell Key absent.
static void LunpaFortress_CheckLockedCellDoor(void) {
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) == -1) {
    Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
  }
}

// Show tightly locked door message unconditionally.
static void LunpaFortress_ShowLockedDoor(void) {
  Engine_MessageShowCentered(MsgFieldDoorTightlyLocked, 1);
}

// Try item 8 from actor 8 and persist success.
static void UNUSED LunpaFortress_TryCollectActorEightItem(void) {
  if (LunpaFortress_TryCollectDisplayedItem(8, 8) != 0) {
    Engine_GameFlagSet(0xf2a);
  }
}

// Try item 7 from actor 9 and persist success.
static void UNUSED LunpaFortress_TryCollectActorNineItem(void) {
  if (LunpaFortress_TryCollectDisplayedItem(9, 7) != 0) {
    Engine_GameFlagSet(0xf2b);
  }
}

// Try item 6 from actor 10 and persist success.
static void UNUSED LunpaFortress_TryCollectActorTenItem(void) {
  if (LunpaFortress_TryCollectDisplayedItem(10, 6) != 0) {
    Engine_GameFlagSet(0xf2c);
  }
}

// Try item 5 from actor 11 and persist success.
static void UNUSED LunpaFortress_TryCollectActorElevenItem(void) {
  if (LunpaFortress_TryCollectDisplayedItem(11, 5) != 0) {
    Engine_GameFlagSet(0xf2d);
  }
}

// Show item effect at actor, grant item, choose success/failure animation then
// release effect.
static s32 LunpaFortress_TryCollectDisplayedItem(s32 actorId, s32 itemId) {
  s32 itemGiven = 0;
  s32 itemEffect;

  EV_BEGIN();
  itemEffect =
      Engine_BattleFxPlayCueAndStartEmitterOnTarget(0, actorId, itemId);
  if (Engine_PartyGiveItem(itemId, 0) != -1) {
    OBJ_MODE(actorId, 2);
    itemGiven = 1;
  } else {
    SE(0x7d);
    OBJ_MODE(actorId, 5);
  }
  Engine_ObjectDispatchRelease(itemEffect);
  EV_END();
  return itemGiven;
}

// Unused original scene callback performs no work.
static void UNUSED NoOpSceneCallbackA(void) {}

// Unused original scene callback performs no work.
static void UNUSED NoOpSceneCallbackB(void) {}

// Unused original scene callback performs no work.
static void UNUSED NoOpSceneCallbackC(void) {}

// Unused original scene callback performs no work.
static void UNUSED NoOpSceneCallbackD(void) {}

// Approach dangling key, display/grant it and remove its actor.
static void LunpaFortress_CollectCellKey(void) {

  Engine_ActorWalkTo(ACTOR_PARTY_LEADER, 264, 792);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  WAIT(10);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ItemShowFound(ITEM_CELL_KEY, 3);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_PartyGiveItem(ITEM_CELL_KEY, 0);
  Engine_GameFlagSet(0xf2e);
  Engine_ActorSetPosition(8, 0, 0);
}

// Test two tile regions that suppress paired patrol detection.
static s32 LunpaFortress_IsLeaderInPatrolSafeZone(void) {
  struct FieldActor *leader = Object_GetById(0);
  s32 leaderFixedZ = leader->z.fixed;
  s32 leaderFixedX;
  s32 leaderTileZ, leaderTileX;

  if (leaderFixedZ < 0) {
    leaderFixedZ += 0xfffff;
  }
  leaderFixedX = leader->x.fixed;
  leaderTileZ = leaderFixedZ >> 20;
  if (leaderFixedX < 0) {
    leaderFixedX += 0xfffff;
  }
  leaderTileX = leaderFixedX >> 20;
  if ((u32)(leaderTileZ - 5) <= 2 && leaderTileX <= 10) {
    return 1;
  }
  if ((u32)(leaderTileX - 8) <= 1 && leaderTileZ > 22) {
    return 1;
  }
  return 0;
}

// UB: these patrols formerly reached gEventWork through gMapWork[19] and
// treated its request members as a halfword array. Named owner access adds
// eight bytes to each of the three patrol callbacks in the EN trial.
#ifdef UBFIX
#define PATROL_PSYNERGY_REQUEST(work) ((work)->command)
#define PATROL_ITEM_REQUEST(work) ((work)->item)
#define PATROL_RAISED_TRIGGER(work) ((work)->event)
#else
#define PATROL_PSYNERGY_REQUEST(work) ((work)[191])
#define PATROL_ITEM_REQUEST(work) ((work)[192])
#define PATROL_RAISED_TRIGGER(work) ((work)[193])
#endif

// Jitter layer, pause patrol during requests, detect uncloaked leader or prompt
// while cloaked.
static void LunpaFortress_UpdatePairedGuardDetection(void) {
  struct FieldActor *firstGuard = Actor_Get(9);
  struct FieldActor *secondGuard = Actor_Get(10);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || PATROL_PSYNERGY_REQUEST(fieldWork) != 0 ||
      PATROL_ITEM_REQUEST(fieldWork) != 0) {
    firstGuard->paused = 1;
    secondGuard->paused = 1;
  } else if (!FLAG_CHECK(0x214)) {
    firstGuard->paused = 0;
    secondGuard->paused = 0;
    if (!FLAG_CHECK(0x214) && firstGuard->paused == 0) {
      patrolLayer->phaseX = PAIRED_PATROL_LAYER_ORIGIN_X - firstGuard->x.fixed;
    }
    if (!LunpaFortress_IsLeaderInPatrolSafeZone()) {
      if (gGameState.cloaked != 0) {
        if (LunpaFortress_IsLeaderNearGuard(9) && gGameState.cloaked != 0) {
          SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
          return;
        }
        if (LunpaFortress_IsLeaderNearGuard(10) && gGameState.cloaked != 0) {
          SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
          return;
        }
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(9)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
        if (LunpaFortress_CanGuardSeeLeader(10)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork), 91);
      }
    }
  }
}

// Stop first guard, face leader, then second guard expels party.
static void LunpaFortress_ConfrontCaughtPairedPatrol(void) {
  EV_BEGIN();
  Engine_ActorStop(9);
  Engine_ActorSetDestinationOffset(9, 0, 0);
  OBJ_MODE(9, 0);
  OBJ_FACE(9, ACTOR_PARTY_LEADER, 0);
  Engine_ActorShowEmote(9, EMOTE_IN_FRONT, 0);
  LunpaFortress_ExpelPartyWithGuard(10);
  EV_END();
}

// Recognize opposite-column passing positions where patrol should not detect.
static s32 LunpaFortress_IsLeaderPassingPatrollingGuard(void) {
  struct FieldActor *leader = Object_GetById(0);
  struct FieldActor *guard = Object_GetById(17);
  s32 leaderFixedX = leader->x.fixed;
  s32 guardFixedZ;
  s32 guardFixedX;
  s32 leaderTileX, guardTileZ, guardTileX;

  if (leaderFixedX < 0) {
    leaderFixedX += 0xfffff;
  }
  leaderTileX = leaderFixedX >> 20;
  guardFixedZ = guard->z.fixed;
  if (guardFixedZ < 0) {
    guardFixedZ += 0xfffff;
  }
  guardFixedX = guard->x.fixed;
  guardTileZ = guardFixedZ >> 20;
  if (guardFixedX < 0) {
    guardFixedX += 0xfffff;
  }
  guardTileX = guardFixedX >> 20;
  if (leaderTileX == 52 && guardTileX == 57 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  if (leaderTileX == 57 && guardTileX == 52 && guardTileZ > 34 &&
      guardTileZ <= 40) {
    return 1;
  }
  return 0;
}

// Jitter/offset layer and detect guard 17 unless leader passes safely.
static void LunpaFortress_UpdateHallGuardDetection(void) {

  struct FieldActor *guard = Actor_Get(17);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
#endif

  Engine_EventGetViewCenter();
  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || PATROL_PSYNERGY_REQUEST(fieldWork) != 0 ||
      PATROL_ITEM_REQUEST(fieldWork) != 0) {
    guard->paused = 1;
  } else if (!FLAG_CHECK(0x214)) {
    guard->paused = 0;
    if (!FLAG_CHECK(0x214) && guard->paused == 0) {
      patrolLayer->phaseX = 0x3400000 - guard->x.fixed;
      patrolLayer->phaseY = 0x2400000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderPassingPatrollingGuard()) {
      LunpaFortress_IsGuardNearViewCenter(17);
      if (LunpaFortress_IsLeaderNearGuard(17) && gGameState.cloaked != 0) {
        SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(17)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork), 92);
      }
    }
  }
}

// Run guard 17 detection-expulsion dialogue then end event.
static void LunpaFortress_ExpelPartyAfterHallDetection(void) {
  LunpaFortress_ExpelPartyWithGuard(17);
  EV_END();
}

// Test three tile rectangles exempting leader from guard 18 detection.
static s32 LunpaFortress_IsLeaderInOuterPatrolSafeZone(void) {
  struct FieldActor *leader = Object_GetById(0);
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;

  if ((u32)(leaderTileX - 41) <= 3 && leaderTileZ > 25 && leaderTileZ <= 28) {
    return 1;
  }
  if (leaderTileX == 41 && leaderTileZ > 37 && leaderTileZ <= 41) {
    return 1;
  }
  if ((u32)(leaderTileX - 54) <= 2 && leaderTileZ > 30 && leaderTileZ <= 40) {
    return 1;
  }
  return 0;
}

// Jitter/offset layer and detect guard 18 outside safe regions.
static void LunpaFortress_UpdateOuterGuardDetection(void) {

  struct FieldActor *guard = Actor_Get(18);
  struct MapLayerScroll *patrolLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[2];
#ifdef UBFIX
  struct FieldWork *fieldWork = (struct FieldWork *)gEventWork;
#else
  s16 *fieldWork = *(s16 **)&gMapWork[19];
#endif

  if (gFrameCount & 1) {
    patrolLayer->speedX = 1;
    patrolLayer->speedY = 1;
  } else {
    patrolLayer->speedX = -1;
    patrolLayer->speedY = -1;
  }
  if (FLAG_CHECK(0x106) || PATROL_PSYNERGY_REQUEST(fieldWork) != 0 ||
      PATROL_ITEM_REQUEST(fieldWork) != 0) {
    guard->paused = 1;
  } else if (!FLAG_CHECK(0x214)) {
    guard->paused = 0;
    if (!FLAG_CHECK(0x214) && guard->paused == 0) {
      patrolLayer->phaseX = 0x2f00000 - guard->x.fixed;
      patrolLayer->phaseY = 0x1f00000 - guard->z.fixed;
    }
    if (!LunpaFortress_IsLeaderInOuterPatrolSafeZone()) {
      if (LunpaFortress_IsLeaderNearGuard(18) && gGameState.cloaked != 0) {
        SetSceneValue(&PATROL_PSYNERGY_REQUEST(fieldWork), 0x2092);
        return;
      }
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(18)) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214)) {
        SetSceneValue(&PATROL_RAISED_TRIGGER(fieldWork), 93);
      }
    }
  }
}

// Run guard 18 detection-expulsion dialogue.
static void LunpaFortress_ExpelPartyAfterOuterDetection(void) {
  LunpaFortress_ExpelPartyWithGuard(18);
  EV_END();
}

// Return false only inside upper hall rectangle used for guard detection.
static s32 LunpaFortress_IsLeaderOutsideUpperHall(void) {
  struct FieldActor *leader = Object_GetById(0);
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;

  if (leaderTileX > 45 && leaderTileZ > 14 && leaderTileX <= 64 &&
      leaderTileZ <= 16) {
    return 0;
  }
  return 1;
}

// Detect uncloaked leader in upper hall, persist alert flags and raise94.
static void LunpaFortress_CheckUpperHallGuard(void) {

  struct EventWork *fieldWork;

  fieldWork = gEventWork;
  if (FLAG_CHECK(0x214) == 0) {
    if (LunpaFortress_IsLeaderOutsideUpperHall() == 0) {
      if (gGameState.cloaked == 0) {
        if (LunpaFortress_CanGuardSeeLeader(17) != 0) {
          FLAG_SET(0x215);
          FLAG_SET(0x214);
        }
      }
      if (FLAG_CHECK(0x214) != 0) {
        fieldWork->raisedTrigger = 94;
      }
    }
  }
}

// Run guard 17 detection-expulsion response for upper-hall trigger.
static void LunpaFortress_ExpelPartyAfterUpperHallDetection(void) {
  LunpaFortress_ExpelPartyWithGuard(17);
  EV_END();
}

// On uncloaked guard 12 detection, unregister watcher and raise95.
static void LunpaFortress_CheckEntranceGuard(void) {

  struct EventWork *fieldWork = gEventWork;

  if (LunpaFortress_CanGuardSeeLeader(12) != 0 && gGameState.cloaked == 0) {
    s16 *raisedTrigger;
    s32 caughtTrigger;

    Engine_TaskRemoveCallback(LunpaFortress_CheckEntranceGuard);
    raisedTrigger = &fieldWork->raisedTrigger;
    caughtTrigger = 95;
    *raisedTrigger = caughtTrigger;
  }
}

// Detect guard 13, set shared alert and remove both sentry watchers.
static void LunpaFortress_CheckFirstEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gEventWork;
  if (FLAG_CHECK(0x225) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(13) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(0x225);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        fieldWork->raisedTrigger = 96;
      }
    }
  }
}

// Detect guard 21, set shared alert and remove both sentry watchers.
static void LunpaFortress_CheckSecondEntranceSentry(void) {

  struct EventWork *fieldWork;

  fieldWork = gEventWork;
  if (FLAG_CHECK(0x225) == 0) {
    if (LunpaFortress_CanGuardSeeLeader(21) != 0) {
      if (gGameState.cloaked == 0) {
        FLAG_SET(0x225);
        Engine_TaskRemoveCallback(LunpaFortress_CheckSecondEntranceSentry);
        Engine_TaskRemoveCallback(LunpaFortress_CheckFirstEntranceSentry);
        fieldWork->raisedTrigger = 96;
      }
    }
  }
}

// Test same-column line of sight within six rows.
static s32 LunpaFortress_IsLeaderInGuardVerticalSight(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(0);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
  s32 zDistance = guardTileZ - leaderTileZ;

  if (zDistance >= -6 && zDistance <= 6 && guardTileX - 1 < leaderTileX &&
      guardTileX + 1 > leaderTileX) {
    return 1;
  }
  return 0;
}

// Test horizontal range6 and row difference at most1.
static s32 LunpaFortress_IsLeaderInGuardHorizontalSight(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(0);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
  s32 xDistance = guardTileX - leaderTileX;

  if (xDistance < -6 || xDistance > 6) {
    return 0;
  }
  if (guardTileZ - 2 < leaderTileZ && guardTileZ + 2 > leaderTileZ) {
    return 1;
  }
  return 0;
}

// Require camera trigger box then horizontal or vertical guard proximity.
static s32 LunpaFortress_CanGuardSeeLeader(s32 actorId) {
  if (LunpaFortress_IsGuardNearViewCenter(actorId) == 0) {
    return 0;
  }
  if (LunpaFortress_IsLeaderInGuardVerticalSight(actorId) != 0) {
    return 1;
  }
  {
    s32 inHorizontalSight =
        LunpaFortress_IsLeaderInGuardHorizontalSight(actorId);

    return (u32)(inHorizontalSight | -inHorizontalSight) >> 31;
  }
}

// Check Manhattan distance<=4 after advancing guard reference row by1.
static s32 LunpaFortress_IsLeaderNearGuard(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *leader = Object_GetById(0);
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 leaderTileZ = leader->z.fixed / 0x100000;
  s32 leaderTileX = leader->x.fixed / 0x100000;
  s32 xDistance = guardTileX - leaderTileX;
  s32 zDistance;

  guardTileZ += 1;
  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - leaderTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance + zDistance <= 4) {
    return 1;
  }
  return 0;
}

// Check guard within7 columns and5 rows of event view center.
static s32 LunpaFortress_IsGuardNearViewCenter(s32 actorId) {
  struct FieldActor *guard = Object_GetById(actorId);
  struct FieldActor *viewCenter = Engine_EventGetViewCenter();
  s32 guardTileX = guard->x.fixed / 0x100000;
  s32 guardTileZ = guard->z.fixed / 0x100000;
  s32 viewTileX = viewCenter->x.fixed / 0x100000;
  s32 viewTileZ = viewCenter->z.fixed / 0x100000;
  s32 xDistance = guardTileX - viewTileX;
  s32 zDistance;

  if (xDistance < 0) {
    xDistance = -xDistance;
  }
  zDistance = guardTileZ - viewTileZ;
  if (zDistance < 0) {
    zDistance = -zDistance;
  }
  if (xDistance > 7 || zDistance > 5) {
    return 0;
  }
  return 1;
}

// On column43 crossing, synthesize touch41 and run outer door unlock.
static void LunpaFortress_CheckOuterDoorApproach(void) {

  struct FieldActor *leader = Object_GetById(0);

  if (Engine_GameFlagIsSet(859) == 0) {
    s32 playerX = leader->x.fixed / 0x100000;
    s32 playerZ = leader->z.fixed / 0x100000;

    if (playerX == 43 && playerZ > 28 && playerZ <= 31) {
      s16 *touchedTrigger = &gEventWork->touchedTrigger;
      s32 doorTrigger = 41;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockOuterFortressDoor();
    }
  }
}

// On column16 crossing while east/west facing, synthesize40 and unlock
// guard-room door.
static void LunpaFortress_CheckGuardDoorApproach(void) {

  struct FieldActor *leader = Object_GetById(0);

  if (Engine_GameFlagIsSet(856) == 0) {
    s32 playerX = leader->x.fixed / 0x100000;
    s32 playerZ = leader->z.fixed / 0x100000;

    if (playerX == 16 && playerZ > 55 && playerZ <= 58 &&
        (leader->facing == 0xc000 || leader->facing == 0x4000)) {
      s16 *touchedTrigger = &gEventWork->touchedTrigger;
      s32 doorTrigger = 40;

      *touchedTrigger = doorTrigger;
      LunpaFortress_UnlockGuardRoomDoor();
    }
  }
}

// Stop guard 9, ask who party is, then close screen and request exit60.
static void UNUSED LunpaFortress_ExpelPartyWithPatrolGuard(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(9, 0, 0);
  Actor_SetBuiltinAction(9, ACTOR_ACTION_IDLE);
  Engine_ActorStop(9);
  OBJ_MODE(9, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(9, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(9, 0);
  Engine_EventRequestExit(60);
  Engine_EventCloseScreen();
  EV_END();
}

// Guard challenges leader, waits after fade and returns party to exit60.
static void LunpaFortress_ExpelPartyWithGuard(s32 actorId) {
  EV_BEGIN();
  EV_BEGIN();
  Engine_ActorShowEmote(actorId, EMOTE_IN_FRONT, 1);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Actor_SetBuiltinAction(actorId, ACTOR_ACTION_IDLE);
  OBJ_MODE(actorId, 0);
  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorSetDestinationOffset(actorId, 0, 0);
  Actor_SetBuiltinAction(actorId, ACTOR_ACTION_IDLE);
  Engine_ActorStop(actorId);
  OBJ_MODE(actorId, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(actorId, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 60);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(actorId, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
}

// Face guard and leader then choose one of four guard speeches.
static void LunpaFortress_RunGuardChallengeDialogue(s32 actorId) {

  OBJ_FACE(actorId, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, actorId, 0);
  switch (gLunpaFortressRandomPick & 3) {
  case 0:
    LunpaFortress_ActOutIntruderDialogue(actorId);
    break;
  case 1:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  case 2:
    LunpaFortress_ActOutGuardShiftDialogue(actorId);
    break;
  case 3:
    LunpaFortress_ActOutScoundrelDialogue(actorId);
    break;
  default:
    LunpaFortress_ActOutWhosThereDialogue(actorId);
    break;
  }
}

// Guard challenges the intruder to fight through.
static void LunpaFortress_ActOutIntruderDialogue(s32 actorId) {
  s32 messageBase;

  Engine_ActorRunRepeatedMotion(actorId, 1);
  messageBase = MsgLunpaIntruder;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  OBJ_EMOTE(actorId, EMOTE_IN_FRONT | 2, 60);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  messageBase += 2;
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
}

// Guard asks who's there then threatens the intruder.
static void LunpaFortress_ActOutWhosThereDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaGuardWhosThat;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(actorId, 0);
}

// Guard mistakes party for shift replacement before challenge.
static void LunpaFortress_ActOutGuardShiftDialogue(s32 actorId) {
  s32 messageBase;

  messageBase = MsgLunpaShiftAlready;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  WAIT(120);
  OBJ_EMOTE(actorId, EMOTE_IN_FRONT | 1, 60);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(actorId, 0);
}

// Guard scolds intruder through four lines and matching gestures.
static void LunpaFortress_ActOutScoundrelDialogue(s32 actorId) {
  s32 messageBase = MsgLunpaScoundrel;

  MESSAGE_SET(messageBase);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 4);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorRunRepeatedMotion(actorId, 1);
  MESSAGE_SET(messageBase + 2);
  MESSAGE_PUT(actorId, 0);
  Engine_ActorSetAnimationAndWait(actorId, 3);
  MESSAGE_SET(messageBase + 3);
  MESSAGE_PUT(actorId, 0);
}

// Challenge actor 15 then start guard battle and persist defeated flag.
static void LunpaFortress_StartWestWingGuardBattle(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(113);
  Engine_ActorShowEmote(15, EMOTE_IN_FRONT, 60);
  LunpaFortress_RunGuardChallengeDialogue(15);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(15, 0, 0);
  EV_END();
  Engine_GameFlagSet(2380);
}

// Challenge actor 11 and start guard battle, removing guard afterward.
static void LunpaFortress_StartHallGuardBattle(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(113);
  Engine_ActorShowEmote(11, EMOTE_IN_FRONT, 60);
  LunpaFortress_RunGuardChallengeDialogue(11);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(11, 0, 0);
  EV_END();
  Engine_GameFlagSet(2377);
}

// Act out three guards discovering party and set return room/entrance before
// battle.
static void LunpaFortress_StartThreeGuardBattle(void) {
  s32 messageBase;

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_MODE(12, 1);
  OBJ_MODE(13, 1);
  OBJ_MODE(14, 1);
  SE(113);
  OBJ_EMOTE(12, EMOTE_IN_FRONT, 0);
  WAIT(30);
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
  messageBase = MsgLunpaWho3;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(12, 0);
  OBJ_EMOTE(13, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(14, EMOTE_IN_FRONT, 0);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, 13, 0);
  WAIT(65);
  OBJ_DIR(13, FACING_SOUTH + FACING_STEP, 0);
  OBJ_DIR(14, FACING_NORTH + FACING_STEP, 0);
  MESSAGE_SET((messageBase + 1));
  MESSAGE_PUT(13, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((messageBase + 2));
  MESSAGE_PUT(14, 0);
  MESSAGE_SET((messageBase + 3));
  MESSAGE_PUT(12, 0);
  Engine_ActorRunRepeatedMotion(13, 1);
  MESSAGE_SET((messageBase + 4));
  MESSAGE_PUT(13, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  MESSAGE_SET((messageBase + 5));
  MESSAGE_PUT(14, 0);
  Engine_ActorSetAnimationAndWait(14, 3);
  WAIT(60);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(14, ACTOR_PARTY_LEADER, 0);
  WAIT(70);
  OBJ_WALK(12, 672, 88);
  Engine_ActorWaitForMove(12);
#if TBS_PATCH_1
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
#endif
  Engine_ActorSetAnimationAndWait(12, 3);
  WAIT(30);
  MESSAGE_SET((messageBase + 6));
  MESSAGE_PUT(12, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Engine_EventSetNextDestination((s32)&SceneId_LunpaFortress2, 31);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 3);
  OBJ_POS_SET(12, 0, 0);
  OBJ_POS_SET(13, 0, 0);
  OBJ_POS_SET(14, 0, 0);
  EV_END();
  FLAG_SET(0x94a);
}

// Place three defeated guards and reopen screen.
static void LunpaFortress_RestoreThreeDefeatedGuards(void) {
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

// Challenge actor 16, start guard battle and persist defeat.
static void LunpaFortress_StartEastWingGuardBattle(void) {

  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(113);
  Engine_ActorShowEmote(16, EMOTE_IN_FRONT, 60);
  LunpaFortress_RunGuardChallengeDialogue(16);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  Battle_SetStart(3);
  Engine_BattleFxSetWeightedResult(98, 2);
  Engine_ActorSetPosition(16, 0, 0);
  EV_END();
  Engine_GameFlagSet(2379);
}

// The entrance guard challenges the party, then sends it outside.
static void LunpaFortress_ExpelPartyAtEntrance(void) {
  EV_BEGIN();
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_FACE(12, ACTOR_PARTY_LEADER, 0);
  SE(113);
  Engine_ActorShowEmote(12, EMOTE_IN_FRONT, 60);
  {
    s32 challengeMessage = MsgLunpaWho2;

    MESSAGE_SET(challengeMessage);
    MESSAGE_PUT(12, 0);
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 50);
    MESSAGE_SET(challengeMessage + 1);
  }
  MESSAGE_PUT(12, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
  Engine_GameFlagSet(548);
}

// Two sentries spot leader and one challenges party before expulsion.
static void LunpaFortress_ExpelPartyAtSentries(void) {
  s32 challengeMessage;

  EV_BEGIN();
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetBuiltinAction(ACTOR_PARTY_LEADER, ACTOR_ACTION_IDLE);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  SE(113);
  OBJ_EMOTE(21, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(13, EMOTE_IN_FRONT, 60);
  OBJ_FACE(21, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(13, ACTOR_PARTY_LEADER, 0);
  challengeMessage = MsgLunpaWho2;
  MESSAGE_SET(challengeMessage);
  MESSAGE_PUT(13, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 30);
  MESSAGE_SET(challengeMessage + 1);
  MESSAGE_PUT(13, 0);
  Engine_EventCloseScreen();
  WAIT(60);
  Engine_EventRequestExit(60);
  EV_END();
  FLAG_SET(0x225);
}

// Copy first reveal-switch passage graphics and collision cells.
static void LunpaFortress_OpenFirstRevealPassage(void) {
  Map_CopyCells(2, 82, 1, 2, 21, 81);
  Map_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

// Copy second reveal-switch passage graphics and collision cells.
static void LunpaFortress_OpenSecondRevealPassage(void) {
  Map_CopyCells(2, 84, 1, 2, 6, 55);
  Map_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

// Clear passage exposed by Whirlwind on firewood actor.
static void LunpaFortress_OpenWhirlwindPassage(void) {
  Map_CopyCells(2, 86, 1, 2, 27, 62);
  Map_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

// Animate a revealed switch's two frames, then open its passage.
static __inline__ void
LunpaFortress_FlipHiddenSwitch(s32 cellX, s32 cellY,
                               void (*openPassage)(void)) {
  Map_CopyCells(6, 77, 1, 2, cellX, cellY);
  WAIT(5);
  Map_CopyCells(7, 77, 1, 2, cellX, cellY);
  WAIT(1);
  openPassage();
}

// While Reveal active, flip unactivated hidden switch and open first passage.
static void LunpaFortress_FlipFirstHiddenSwitch(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
    if (FLAG_CHECK(0x947) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(188);
      WAIT(1);
      LunpaFortress_FlipHiddenSwitch(17, 82,
                                     LunpaFortress_OpenFirstRevealPassage);
      FLAG_SET(0x947);
    }
  }
}

// While Reveal active, animate second switch and open second passage.
static void LunpaFortress_FlipSecondHiddenSwitch(void) {
  if (((struct FieldWork *)gEventWork)->revealActive != 0) {
    if (FLAG_CHECK(0x948) == 0) {
      Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
      SE(188);
      WAIT(1);
      LunpaFortress_FlipHiddenSwitch(3, 55,
                                     LunpaFortress_OpenSecondRevealPassage);
      FLAG_SET(0x948);
    }
  }
}

// Draw two revealed switches and enable their collision/search cells.
static void LunpaFortress_ShowHiddenSwitches(void) {
  Map_CopyCells(5, 77, 1, 2, 17, 82);
  Map_CopyCells(5, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(15, 33, 1, 1, 17, 35);
  Map_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

// Replace both switch graphics and restore surrounding attributes.
static void LunpaFortress_HideHiddenSwitches(void) {
  Map_CopyCells(8, 77, 1, 2, 17, 82);
  Map_CopyCells(8, 77, 1, 2, 3, 55);
  Map_CopyCellAttributes(18, 35, 1, 1, 17, 35);
  Map_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

// Persist Whirlwind interaction, clear temporary search flag and hide actor 8.
static void LunpaFortress_ClearWhirlwindWardrobe(void) {
  Engine_GameFlagSet(2372);
  Engine_GameFlagClear(535);
  Engine_ActorSetPosition(8, 0, 0);
}

// Persist removal, open passage and hide actor 9.
static void LunpaFortress_ClearWhirlwindFirewood(void) {
  Engine_GameFlagSet(2373);
  LunpaFortress_OpenWhirlwindPassage();
  Engine_ActorSetPosition(9, 0, 0);
}

// Persist actor 10 removal and clear second search flag.
static void LunpaFortress_ClearWhirlwindObstruction(void) {
  Engine_GameFlagSet(2374);
  Engine_GameFlagClear(536);
  Engine_ActorSetPosition(10, 0, 0);
}

// Unused original callback performs no work.
static void UNUSED NoOpInteractionCallback(void) {}

// Spawn companions at leader then arrange them around Hammet.
static void LunpaFortress_GatherPartyAroundHammet(void) {
  struct FieldActor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
  }
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != NULL) {
    Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
  }
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_WALK(IVAN, 456, 192);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(MIA, 440, 184);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_WALK(GARET, 448, 240);
  Engine_ActorWaitForMove(IVAN);
  OBJ_FACE(IVAN, RESCUE_HAMMET, 0);
  Engine_ActorWaitForMove(GARET);
  Engine_ActorWaitForMove(MIA);
  OBJ_FACE(GARET, RESCUE_HAMMET, 0);
  OBJ_FACE(MIA, RESCUE_HAMMET, 0);
  WAIT(15);
}

// Shake room and open Dodonpa's trap before party reactions.
static void LunpaFortress_ActivateLeverTrap(void) {
  Map_SetShake(0x40000, 0x40000, 0x10000);
  SE(141);
  WAIT(80);
  SE(0x120);
  WAIT(5);
  SE(145);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  Map_SetShake(-1, -1, 0xe666);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT, 0);
  WAIT(60);
}

// Pan to monster, show Dodonpa's threat and move monster parts into position.
static void LunpaFortress_RevealMonster(void) {
  Engine_CameraMoveToActor(RESCUE_MONSTER, 1);
  Engine_CameraWaitForMove();
  WAIT(60);
  MESSAGE_SET(MsgLunpaPrepareBecomeMonster);
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_SPEED(RESCUE_MONSTER, 0x10000, 0x8000);
  OBJ_SPEED(RESCUE_MONSTER_PART, 0x10000, 0x8000);
  OBJ_WALK(RESCUE_MONSTER, 472, 180);
  OBJ_WALK(RESCUE_MONSTER_PART, 472, 180);
  Engine_CameraFollowActor(RESCUE_MONSTER, 1);
  Engine_ActorWaitForMove(RESCUE_MONSTER);
  OBJ_MODE(RESCUE_MONSTER, 4);
  WAIT(30);
}

// Spread companions for monster battle and face party north.
static void LunpaFortress_PositionPartyForMonster(void) {
  OBJ_SPEED(IVAN, 0x10000, 0x8000);
  OBJ_WALK(IVAN, 504, 216);
  OBJ_SPEED(MIA, 0x10000, 0x8000);
  OBJ_WALK(MIA, 440, 232);
  OBJ_SPEED(GARET, 0x10000, 0x8000);
  OBJ_WALK(GARET, 480, 224);
  Engine_ActorWaitForMove(GARET);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, FACING_NORTH, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
}

// Introduce Hammet and Dodonpa or replay shortened failed-battle route,
// activate trap and battle.
static void LunpaFortress_RescueHammetAndStartMonsterBattle(void) {
  s32 messageBase;

  if (FLAG_CHECK(769) != 0)
    return;
  FLAG_SET(624);
  EV_BEGIN();
  // A retry skips the introduction but repeats Dodonpa's trap and battle.
  if (FLAG_CHECK(2370) != 0) {
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT, 60);
    OBJ_DIR(RESCUE_HAMMET, FACING_WEST, 0);
    Engine_ActorJump(RESCUE_HAMMET, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(RESCUE_HAMMET), 1);
    WAIT(30);
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(IVAN, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    OBJ_DIR(IVAN, FACING_SOUTH, 0);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    OBJ_DIR(MIA, FACING_SOUTH, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJ_POS_SET(RESCUE_DODONPA, 456, 320);
    Camera_SetSpeed(131072, 16384);
    messageBase = MsgLunpaBackMoreGuess;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_WALK(RESCUE_DODONPA, 458, 272);
    Engine_ActorWaitForMove(RESCUE_DODONPA);
    OBJ_DIR(RESCUE_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(40);
    Actor_SetDestinationOffset(RESCUE_DODONPA, -8, 8);
    Engine_ActorWaitForMove(RESCUE_DODONPA);
    WAIT(60);
    SE(155);
    Engine_MessageShowCentered(messageBase + 1, 1);
    Actor_SetDestinationOffset(RESCUE_DODONPA, 8, -8);
    LunpaFortress_ActivateLeverTrap();
    WAIT(120);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Engine_ActorStartRepeatedMotion(MIA, 2);
    WAIT(20);
    messageBase += 2;
    LunpaFortress_PositionPartyForMonster();
    OBJ_FACE(RESCUE_DODONPA, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    LunpaFortress_RevealMonster();
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    WAIT(1);
    Battle_SetStart(3);
    Engine_EventSetNextDestination((s32)&SceneId_LunpaFortress4, 4);
    Engine_BattleFxSetWeightedResult(98, 4);
  } else {
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
    SE(17);
    WAIT(30);
    // Ivan recognizes Hammet before Dodonpa interrupts the reunion.
    messageBase = MsgLunpaTimeEat;
    MESSAGE_SET(messageBase);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    WAIT(140);
    OBJ_DIR(RESCUE_HAMMET, FACING_WEST, 0);
    Engine_ActorJump(RESCUE_HAMMET, 4, 0);
    Engine_ActorSetSpriteFlags(Actor_Get(RESCUE_HAMMET), 1);
    MESSAGE_SET(messageBase + 1);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    OBJ_SPEED(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_HAMMET, 0);
    LunpaFortress_GatherPartyAroundHammet();
    OBJ_SPEED(IVAN, 45875, 22937);
    OBJ_WALK(IVAN, 464, 192);
    Engine_ActorWaitForMove(IVAN);
    WAIT(30);
    MESSAGE_SET(messageBase + 2);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT, 0);
    WAIT(110);
    SE(60);
    MESSAGE_SET(messageBase + 3);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    OBJ_MODE(IVAN, ANIM_STAND);
    Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 1);
    WAIT(20);
    OBJ_SPEED(RESCUE_HAMMET, 26214, 13107);
    OBJ_WALK(RESCUE_HAMMET, 520, 208);
    Engine_ActorWaitForMove(RESCUE_HAMMET);
    OBJ_MODE(RESCUE_HAMMET, 1);
    WAIT(20);
    OBJ_DIR(RESCUE_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(30);
    OBJ_DIR(RESCUE_HAMMET, FACING_SOUTH + FACING_STEP, 0);
    WAIT(30);
    OBJ_FACE(RESCUE_HAMMET, IVAN, 0);
    WAIT(20);
    MESSAGE_SET(messageBase + 4);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    WAIT(40);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(20);
    OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 8, 0);
    WAIT(120);
    MESSAGE_SET(messageBase + 5);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    WAIT(25);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    WAIT(30);
    Engine_ActorSetAnimationAndWait(RESCUE_HAMMET, 3);
    WAIT(40);
    OBJ_WALK(IVAN, 480, 200);
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorFaceEachOther(IVAN, 12, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 6);
    MESSAGE_PUT(IVAN, 0);
    WAIT(20);
    OBJ_MODE(RESCUE_HAMMET, 4);
    WAIT(80);
    MESSAGE_SET(messageBase + 7);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    // Dodonpa approaches from the south, then springs the lever trap.
    OBJ_POS_SET(RESCUE_DODONPA, 456, 320);
    SE(19);
    MESSAGE_SET(messageBase + 8);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
    OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
    OBJ_FACE(GARET, RESCUE_DODONPA, 0);
    WAIT(5);
    OBJ_DIR(MIA, FACING_SOUTH, 0);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
    WAIT(30);
    SE(61);
    Camera_SetSpeed(131072, 16384);
    Engine_CameraMoveToActor(RESCUE_DODONPA, 1);
    Engine_CameraWaitForMove();
    OBJ_SPEED(RESCUE_DODONPA, 52428, 26214);
    OBJ_WALK(RESCUE_DODONPA, 456, 304);
    Engine_CameraFollowActor(RESCUE_DODONPA, 1);
    Engine_ActorWaitForMove(RESCUE_DODONPA);
    Engine_CameraFollowActor(GARET, 1);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 2, 0);
    WAIT(60);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
    Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 2);
    WAIT(60);
    MESSAGE_SET(messageBase + 9);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    OBJ_FACE(RESCUE_DODONPA, RESCUE_HAMMET, 0);
    MESSAGE_SET(messageBase + 10);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    WAIT(60);
    OBJ_FACE(RESCUE_DODONPA, IVAN, 0);
    WAIT(30);
    MESSAGE_SET(messageBase + 11);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    Engine_ActorFaceEachOther(MIA, IVAN, 0);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
    OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
    OBJ_FACE(GARET, RESCUE_DODONPA, 0);
    OBJ_FACE(MIA, RESCUE_DODONPA, 0);
    Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 12);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 0);
    WAIT(60);
    OBJ_MODE(RESCUE_DODONPA, 4);
    MESSAGE_SET(messageBase + 13);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_WALK(GARET, 456, 248);
    Engine_ActorWaitForMove(GARET);
    OBJ_DIR(GARET, FACING_SOUTH, 0);
    MESSAGE_SET(messageBase + 14);
    MESSAGE_PUT(GARET, 0);
    OBJ_WALK(IVAN, 472, 216);
    Engine_ActorWaitForMove(IVAN);
    OBJ_DIR(IVAN, FACING_SOUTH, 0);
    WAIT(10);
    MESSAGE_SET(messageBase + 15);
    MESSAGE_PUT(IVAN, 0);
    OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 5, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 16);
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    OBJ_WALK(MIA, 440, 216);
    Engine_ActorWaitForMove(MIA);
    OBJ_FACE(MIA, RESCUE_DODONPA, 0);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    WAIT(10);
    MESSAGE_SET(messageBase + 17);
    MESSAGE_PUT(MIA, 0);
    OBJ_MODE(RESCUE_DODONPA, 4);
    MESSAGE_SET(messageBase + 18);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 2, 0);
    OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT | 8, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 19);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    WAIT(20);
    OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 0);
    WAIT(60);
    MESSAGE_SET(messageBase + 20);
    MESSAGE_PUT(GARET, 0);
    Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
    WAIT(60);
    MESSAGE_SET(messageBase + 21);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    OBJ_WALK(RESCUE_DODONPA, 456, 280);
    Engine_ActorWaitForMove(RESCUE_DODONPA);
    OBJ_DIR(RESCUE_DODONPA, FACING_SOUTH + FACING_STEP, 0);
    WAIT(80);
    Actor_SetDestinationOffset(RESCUE_DODONPA, -8, 8);
    Engine_ActorWaitForMove(RESCUE_DODONPA);
    WAIT(60);
    SE(155);
    Engine_MessageShowCentered(MsgLunpaDodonpaPulledLever, 1);
    Actor_SetDestinationOffset(RESCUE_DODONPA, 8, -8);
    OBJ_FACE(RESCUE_DODONPA, RESCUE_MONSTER, 0);
    LunpaFortress_ActivateLeverTrap();
    SE(52);
    MESSAGE_SET(messageBase + 23);
    MESSAGE_PUT(RESCUE_DODONPA, 0);
    WAIT(60);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_MONSTER, 0);
    OBJ_FACE(GARET, RESCUE_MONSTER, 0);
    OBJ_FACE(IVAN, RESCUE_MONSTER, 0);
    OBJ_FACE(MIA, RESCUE_MONSTER, 0);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_MONSTER, 0);
    LunpaFortress_PositionPartyForMonster();
    LunpaFortress_RevealMonster();
    FLAG_SET(2370);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    WAIT(1);
    Battle_SetStart(3);
    Engine_EventSetNextDestination((s32)&SceneId_LunpaFortress4, 4);
    Engine_BattleFxSetWeightedResult(98, 4);
  }
  EV_END();
}

// After monster battle lift monster off Dodonpa, hear Donpa's judgment, reunite
// party and leave with Hammet.
static void LunpaFortress_ResolveHammetRescueAndDonpaPunishment(void) {
  struct FieldActor *leader;
  s32 aftermathMessage;
  s32 punishmentMessage;
  s32 donpaMessage;

  FLAG_SET(0x301);
  FLAG_SET(FLAG_LUNPA_TRADE_REOPENED);
  Map_CopyCells(16, 75, 7, 4, 26, 55);
  LunpaFortress_OpenRescueRoomDoor(4);
  EV_BEGIN();
  leader = Actor_Get(RESCUE_HAMMET);
  Engine_ActorSetSpriteFlags(leader, 1);
  OBJ_POS_SET(ACTOR_PARTY_LEADER, 456, 184);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_POS_SET(GARET, 440, 192);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_POS_SET(MIA, 488, 184);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  OBJ_POS_SET(IVAN, 472, 184);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_POS_SET(RESCUE_HAMMET, 520, 224);
  OBJ_DIR(RESCUE_HAMMET, FACING_NORTHWEST + FACING_STEP, 0);
  OBJ_POS_SET(RESCUE_MONSTER, 448, 237);
  OBJ_DIR(RESCUE_MONSTER, FACING_WEST, 0);
  OBJ_POS_SET(RESCUE_MONSTER_PART, 448, 238);
  Engine_ActorSetSpritePriority(RESCUE_MONSTER_PART, 3);
  OBJ_MODE(RESCUE_MONSTER, 3);
  OBJ_POS_SET(RESCUE_DODONPA, 458, 243);
  OBJ_DIR(RESCUE_DODONPA, FACING_SOUTH, 0);
  OBJ_MODE(RESCUE_DODONPA, 5);
  Engine_EventOpenScreen();
  WAIT(120);

  // The fallen monster has pinned Dodonpa; decide whether to help him.
  aftermathMessage = MsgLunpaUhnnGetOff;
  MESSAGE_SET(aftermathMessage);
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 60);
  MESSAGE_SET((aftermathMessage + 1));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 1);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 1);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 1);
  WAIT(60);
  OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 2));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_HAMMET, 0);
  OBJ_FACE(GARET, RESCUE_HAMMET, 0);
  OBJ_FACE(IVAN, RESCUE_HAMMET, 0);
  OBJ_FACE(MIA, RESCUE_HAMMET, 0);
  WAIT(60);
  OBJ_WALK(RESCUE_HAMMET, 512, 232);
  OBJ_MODE(RESCUE_HAMMET, 4);
  WAIT(60);
  MESSAGE_SET((aftermathMessage + 3));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  WAIT(15);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 2);
  MESSAGE_SET((aftermathMessage + 4));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
  OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
  OBJ_FACE(MIA, RESCUE_DODONPA, 0);
  OBJ_FACE(GARET, RESCUE_DODONPA, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((aftermathMessage + 5));
  MESSAGE_PUT(IVAN, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 5, 0);
  OBJ_MODE(MIA, ANIM_SHAKE_HEAD);
  WAIT(80);
  OBJ_MODE(MIA, ANIM_STAND);
  MESSAGE_SET((aftermathMessage + 6));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 2, 65);
  Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 2);
  WAIT(100);
  MESSAGE_SET((aftermathMessage + 7));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  WAIT(10);
  MESSAGE_SET((aftermathMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((aftermathMessage + 9));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((aftermathMessage + 10));
  MESSAGE_OPEN(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    OBJ_MODE(IVAN, ANIM_NOD);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(MIA, ANIM_NOD);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
    WAIT(60);
  } else {
    MESSAGE_SET((aftermathMessage + 11));
    MESSAGE_PUT(MIA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_HAMMET, 0);
    OBJ_FACE(GARET, RESCUE_HAMMET, 0);
    OBJ_FACE(IVAN, RESCUE_HAMMET, 0);
    OBJ_FACE(MIA, RESCUE_HAMMET, 0);
    WAIT(20);
    OBJ_FACE(RESCUE_HAMMET, ACTOR_PARTY_LEADER, 0);
    WAIT(60);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
    WAIT(80);
    Engine_ActorSetAnimationAndWait(RESCUE_HAMMET, 3);
    WAIT(30);
    OBJ_FACE(RESCUE_HAMMET, ACTOR_PARTY_LEADER, 0);
    MESSAGE_SET((aftermathMessage + 12));
    MESSAGE_PUT(RESCUE_HAMMET, 0);
    WAIT(60);
    OBJ_FACE(IVAN, RESCUE_HAMMET, 0);
    OBJ_FACE(GARET, RESCUE_HAMMET, 0);
    OBJ_FACE(MIA, RESCUE_HAMMET, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_HAMMET, 0);
    OBJ_MODE(IVAN, ANIM_NOD);
    OBJ_MODE(GARET, ANIM_NOD);
    OBJ_MODE(MIA, ANIM_NOD);
    OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
    WAIT(80);
    OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
    OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
    OBJ_FACE(GARET, RESCUE_DODONPA, 0);
    OBJ_FACE(MIA, RESCUE_DODONPA, 0);
    OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
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
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_WALK(ACTOR_PARTY_LEADER, 440, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 456, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  OBJ_WALK(IVAN, 456, 216);
  Engine_ActorWaitForMove(IVAN);
  OBJ_DIR(IVAN, FACING_WEST, 0);
  Engine_ActorFaceEachOther(GARET, ACTOR_PARTY_LEADER, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(60);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  SE(226);
  OBJ_MODE(RESCUE_DODONPA, 7);
  // Move the party and both monster parts together to free Dodonpa.
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
  OBJ_SPEED(GARET, 0x1999, 0xccc);
  Actor_SetDestinationOffset(GARET, -24, 0);
  OBJ_SPEED(MIA, 0x1999, 0xccc);
  Actor_SetDestinationOffset(MIA, -24, 0);
  OBJ_SPEED(IVAN, 0x1999, 0xccc);
  Actor_SetDestinationOffset(IVAN, -24, 0);
  OBJ_SPEED(RESCUE_MONSTER, 0x1999, 0xccc);
  OBJ_SPEED(RESCUE_MONSTER_PART, 0x1999, 0xccc);
  Actor_SetDestinationOffset(RESCUE_MONSTER, -24, 0);
  Actor_SetDestinationOffset(RESCUE_MONSTER_PART, -24, 0);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  SE(0x120);
  WAIT(60);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_DIR(GARET, FACING_NORTH, 0);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  OBJ_DIR(MIA, FACING_NORTH, 0);
  WAIT(60);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
  OBJ_SPEED(GARET, 0xb333, 0x5999);
  OBJ_SPEED(IVAN, 0xb333, 0x5999);
  OBJ_SPEED(MIA, 0xb333, 0x5999);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 184);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_STAND);
  OBJ_DIR(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  OBJ_WALK(GARET, 464, 256);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 480, 248);
  Engine_ActorWaitForMove(GARET);
  OBJ_WALK(GARET, 440, 192);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  OBJ_DIR(GARET, FACING_SOUTH, 0);
  OBJ_WALK(MIA, 488, 248);
  Engine_ActorWaitForMove(MIA);
  OBJ_WALK(MIA, 488, 184);
  OBJ_WALK(IVAN, 472, 184);
  Engine_ActorWaitForMove(IVAN);
  OBJ_MODE(IVAN, ANIM_STAND);
  OBJ_DIR(IVAN, FACING_SOUTH, 0);
  Engine_ActorWaitForMove(MIA);
  OBJ_MODE(MIA, ANIM_STAND);
  OBJ_DIR(MIA, FACING_SOUTH, 0);
  WAIT(30);
  WAIT(60);
  // Debate Dodonpa's punishment before his father arrives.
  punishmentMessage = MsgLunpaAbleGet;
  MESSAGE_SET(punishmentMessage);
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(RESCUE_DODONPA, 6);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 2);
  WAIT(60);
  OBJ_MODE(RESCUE_DODONPA, 7);
  MESSAGE_SET((punishmentMessage + 1));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  WAIT(20);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(10);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 80);
  OBJ_FACE(MIA, RESCUE_DODONPA, 0);
  OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 2));
  MESSAGE_PUT(MIA, 0);
  OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT | 2, 70);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 2);
  WAIT(60);
  OBJ_MODE(RESCUE_DODONPA, 5);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 3));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_MODE(RESCUE_DODONPA, 7);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 8, 40);
  MESSAGE_SET((punishmentMessage + 4));
  MESSAGE_PUT(IVAN, 0);
  Engine_ActorSetAnimationAndWait(RESCUE_HAMMET, 3);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 3, 60);
  OBJ_WALK(GARET, 440, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  MESSAGE_SET((punishmentMessage + 5));
  MESSAGE_PUT(GARET, 0);
  WAIT(20);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 1, 0);
  WAIT(70);
  OBJ_FACE(GARET, IVAN, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 2, 75);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
  MESSAGE_SET((punishmentMessage + 6));
  MESSAGE_PUT(GARET, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 1, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 7));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_WALK(GARET, 448, 208);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  OBJ_FACE(GARET, RESCUE_HAMMET, 0);
  MESSAGE_SET((punishmentMessage + 8));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 1);
  WAIT(60);
  OBJ_FACE(GARET, RESCUE_HAMMET, 0);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 9));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_MODE(GARET, ANIM_SHAKE_HEAD);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 10));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 11));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, MIA, 0);
  WAIT(20);
  MESSAGE_SET((punishmentMessage + 12));
  MESSAGE_PUT(GARET, 0);
  WAIT(30);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 80);
  OBJ_FACE(MIA, GARET, 0);
  MESSAGE_SET((punishmentMessage + 13));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(MIA, RESCUE_DODONPA, 0);
  MESSAGE_SET((punishmentMessage + 14));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
  OBJ_FACE(IVAN, RESCUE_DODONPA, 0);
  OBJ_FACE(GARET, RESCUE_DODONPA, 0);
  OBJ_FACE(MIA, RESCUE_DODONPA, 0);
  WAIT(120);
  OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT | 2, 30);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
  WAIT(120);
  Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((punishmentMessage + 15));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 7, 110);
  OBJ_WALK(GARET, 456, 212);
  Engine_ActorWaitForMove(GARET);
  OBJ_MODE(GARET, ANIM_STAND);
  MESSAGE_SET((punishmentMessage + 16));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(IVAN, GARET, 0);
  WAIT(60);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  MESSAGE_SET((punishmentMessage + 17));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(GARET, RESCUE_DODONPA, 0);
  WAIT(80);
  OBJ_FACE(GARET, IVAN, 0);
  WAIT(60);
  OBJ_FACE(GARET, RESCUE_DODONPA, 0);
  WAIT(30);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DODONPA, 0);
  OBJ_FACE(MIA, RESCUE_DODONPA, 0);
  OBJ_FACE(RESCUE_DODONPA, RESCUE_DODONPA, 0);
  OBJ_FACE(RESCUE_HAMMET, RESCUE_DODONPA, 0);
  WAIT(80);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  WAIT(30);
  MESSAGE_SET((punishmentMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 2);
  WAIT(70);
  MESSAGE_SET((punishmentMessage + 19));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 60);
  MESSAGE_SET((punishmentMessage + 20));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(80);
  MESSAGE_SET((punishmentMessage + 21));
  MESSAGE_OPEN(IVAN, 0);
  OBJ_FACE(GARET, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(MIA, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  OBJ_FACE(RESCUE_HAMMET, ACTOR_PARTY_LEADER, 0);
  OBJ_POS_SET(RESCUE_DONPA, 456, 304);
  OBJ_SPEED(RESCUE_DONPA, 0x8000, 0x4000);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    SE(19);
    MESSAGE_SET((punishmentMessage + 22));
    MESSAGE_PUT(RESCUE_DONPA, 0);
  } else {
    SE(19);
    MESSAGE_SET((punishmentMessage + 23));
    MESSAGE_PUT(RESCUE_DONPA, 0);
  }
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DONPA, 0);
  OBJ_FACE(GARET, RESCUE_DONPA, 0);
  OBJ_FACE(MIA, RESCUE_DONPA, 0);
  OBJ_FACE(IVAN, RESCUE_DONPA, 0);
  OBJ_FACE(RESCUE_HAMMET, RESCUE_DONPA, 0);
  OBJ_FACE(RESCUE_DODONPA, RESCUE_DONPA, 0);
  Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
  WAIT(60);
  SE(8);
  // Donpa takes charge and orders his son to remain imprisoned.
  donpaMessage = MsgLunpaDad;
  MESSAGE_SET(donpaMessage);
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_WALK(RESCUE_DONPA, 456, 280);
  Engine_ActorWaitForMove(RESCUE_DONPA);
  OBJ_WALK(RESCUE_DONPA, 440, 256);
  Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
  MESSAGE_SET((donpaMessage + 1));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorWaitForMove(RESCUE_DONPA);
  OBJ_DIR(RESCUE_DONPA, FACING_NORTH + FACING_STEP, 0);
  OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT | 2, 80);
  OBJ_MODE(RESCUE_DONPA, 4);
  WAIT(89);
  MESSAGE_SET((donpaMessage + 2));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorRunRepeatedMotion(RESCUE_DODONPA, 2);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  MESSAGE_SET((donpaMessage + 3));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT, 80);
  OBJ_MODE(RESCUE_DONPA, 4);
  WAIT(80);
  OBJ_DIR(RESCUE_DONPA, FACING_SOUTHEAST + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 4));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
  MESSAGE_SET((donpaMessage + 5));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  WAIT(30);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  OBJ_FACE(RESCUE_DONPA, RESCUE_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 6));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_DODONPA, 1);
  OBJ_EMOTE(RESCUE_DODONPA, EMOTE_IN_FRONT | 2, 80);
  MESSAGE_SET((donpaMessage + 7));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_EMOTE(RESCUE_DONPA, EMOTE_IN_FRONT | 3, 60);
  MESSAGE_SET((donpaMessage + 8));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  MESSAGE_SET((donpaMessage + 9));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  WAIT(20);
  Actor_SetAttachedEffect(RESCUE_DODONPA, 0x101);
  WAIT(80);
  Actor_SetAttachedEffect(RESCUE_DODONPA, 0);
  OBJ_WALK(RESCUE_DONPA, 496, 240);
  Engine_ActorWaitForMove(RESCUE_DONPA);
  OBJ_FACE(RESCUE_DONPA, RESCUE_HAMMET, 0);
  WAIT(20);
  OBJ_FACE(RESCUE_HAMMET, RESCUE_DONPA, 0);
  MESSAGE_SET((donpaMessage + 10));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 1);
  MESSAGE_SET((donpaMessage + 11));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  WAIT(40);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 12));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_MODE(RESCUE_DONPA, 3);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 2, 60);
  MESSAGE_SET((donpaMessage + 13));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_EMOTE(RESCUE_DONPA, EMOTE_IN_FRONT, 70);
  MESSAGE_SET((donpaMessage + 14));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_MODE(RESCUE_HAMMET, 3);
  WAIT(140);
  OBJ_MODE(RESCUE_DONPA, 3);
  WAIT(120);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 14, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(120);
  OBJ_EMOTE(RESCUE_DONPA, EMOTE_IN_FRONT | 8, 180);
  MESSAGE_SET((donpaMessage + 15));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT | 1, 80);
  MESSAGE_SET((donpaMessage + 16));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GARET, 0);
  Engine_ActorFaceEachOther(MIA, IVAN, 0);
  WAIT(80);
  OBJ_FACE(RESCUE_DONPA, ACTOR_PARTY_LEADER, 0);
  WAIT(60);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DONPA, 0);
  OBJ_FACE(GARET, RESCUE_DONPA, 0);
  OBJ_FACE(IVAN, RESCUE_DONPA, 0);
  OBJ_FACE(MIA, RESCUE_DONPA, 0);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 17));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 18));
  MESSAGE_PUT(GARET, 0);
  Engine_ActorStartRepeatedMotion(IVAN, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 19));
  MESSAGE_PUT(IVAN, 0);
  OBJ_FACE(RESCUE_DONPA, IVAN, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 20));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT | 1, 0);
  WAIT(120);
  OBJ_MODE(RESCUE_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 21));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(RESCUE_DONPA, EMOTE_IN_FRONT | 2, 90);
  MESSAGE_SET((donpaMessage + 22));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(RESCUE_HAMMET, 3);
  WAIT(80);
  OBJ_MODE(RESCUE_DONPA, 4);
  WAIT(120);
  MESSAGE_SET((donpaMessage + 23));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(RESCUE_HAMMET, EMOTE_IN_FRONT, 60);
  MESSAGE_SET((donpaMessage + 24));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_FACE(RESCUE_DONPA, RESCUE_HAMMET, 0);
  WAIT(20);
  Engine_ActorStartRepeatedMotion(RESCUE_DONPA, 1);
  WAIT(50);
  MESSAGE_SET((donpaMessage + 25));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_MODE(GARET, ANIM_NOD);
  WAIT(70);
  MESSAGE_SET((donpaMessage + 26));
  MESSAGE_PUT(GARET, 0);
  OBJ_MODE(MIA, ANIM_NOD);
  MESSAGE_SET((donpaMessage + 27));
  MESSAGE_PUT(MIA, 0);
  OBJ_FACE(RESCUE_DONPA, ACTOR_PARTY_LEADER, 0);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 28));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(IVAN, EMOTE_IN_FRONT | 1, 90);
  MESSAGE_SET((donpaMessage + 29));
  MESSAGE_PUT(IVAN, 0);
  OBJ_DIR(RESCUE_DONPA, FACING_SOUTH + FACING_STEP, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 30));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorStartRepeatedMotion(RESCUE_HAMMET, 1);
  WAIT(60);
  MESSAGE_SET((donpaMessage + 31));
  MESSAGE_PUT(RESCUE_HAMMET, 0);
  OBJ_FACE(RESCUE_DONPA, RESCUE_HAMMET, 0);
  WAIT(20);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 32));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(GARET, EMOTE_IN_FRONT, 70);
  MESSAGE_SET((donpaMessage + 33));
  MESSAGE_PUT(GARET, 0);
  OBJ_FACE(RESCUE_DONPA, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  MESSAGE_SET((donpaMessage + 34));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_EMOTE(MIA, EMOTE_IN_FRONT, 80);
  MESSAGE_SET((donpaMessage + 35));
  MESSAGE_PUT(MIA, 0);
  Engine_ActorSetAnimationAndWait(RESCUE_DONPA, 3);
  MESSAGE_SET((donpaMessage + 36));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  OBJ_FACE(IVAN, ACTOR_PARTY_LEADER, 0);
  WAIT(30);
  OBJ_MODE(IVAN, ANIM_NOD);
  WAIT(100);
  MESSAGE_SET((donpaMessage + 37));
  MESSAGE_OPEN(IVAN, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    MESSAGE_SET((donpaMessage + 38));
    MESSAGE_PUT(RESCUE_DONPA, 0);
  }
  WAIT(30);
  OBJ_MODE(GARET, ANIM_NOD);
  OBJ_MODE(IVAN, ANIM_NOD);
  OBJ_MODE(MIA, ANIM_NOD);
  WAIT(100);
  Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  OBJ_MODE(RESCUE_HAMMET, 3);
  WAIT(100);
  // Rejoin the companions, then escort Hammet out of the fortress.
  OBJ_MODE(GARET, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(GARET, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  OBJ_POS_SET(GARET, 0, 0);
  OBJ_MODE(IVAN, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(IVAN, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  OBJ_POS_SET(IVAN, 0, 0);
  OBJ_MODE(MIA, ANIM_WALK);
  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  OBJ_POS_SET(MIA, 0, 0);
  WAIT(30);
  OBJ_SPEED(RESCUE_HAMMET, 0x6666, 0x3333);
  OBJ_WALK(RESCUE_HAMMET, 472, 184);
  Engine_ActorWaitForMove(RESCUE_HAMMET);
  OBJ_MODE(RESCUE_HAMMET, 1);
  OBJ_DIR(RESCUE_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJ_FACE(RESCUE_DONPA, ACTOR_PARTY_LEADER, 0);
  OBJ_MODE(RESCUE_DONPA, 3);
  WAIT(20);
  OBJ_FACE(RESCUE_DONPA, RESCUE_DODONPA, 0);
  WAIT(20);
  MESSAGE_SET((donpaMessage + 39));
  MESSAGE_PUT(RESCUE_DONPA, 0);
  Engine_ActorRunRepeatedMotion(RESCUE_DODONPA, 2);
  MESSAGE_SET((donpaMessage + 40));
  MESSAGE_PUT(RESCUE_DODONPA, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(30);
  OBJ_SPEED(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  OBJ_SPEED(RESCUE_HAMMET, 0x10000, 0x8000);
  Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
  OBJ_WALK(ACTOR_PARTY_LEADER, 480, 248);
  WAIT(40);
  Engine_ActorSetSpritePriority(RESCUE_HAMMET, 0);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 0);
  OBJ_WALK(RESCUE_HAMMET, 480, 216);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_FACE(ACTOR_PARTY_LEADER, RESCUE_DONPA, 0);
  Engine_ActorWaitForMove(RESCUE_HAMMET);
  OBJ_FACE(RESCUE_HAMMET, RESCUE_DONPA, 0);
  OBJ_DIR(RESCUE_HAMMET, FACING_SOUTHEAST + FACING_STEP, 0);
  OBJ_MODE(ACTOR_PARTY_LEADER, ANIM_NOD);
  WAIT(5);
  OBJ_MODE(RESCUE_HAMMET, 3);
  WAIT(100);
  OBJ_MODE(RESCUE_DONPA, 3);
  WAIT(100);
  OBJ_WALK(RESCUE_HAMMET, 480, 248);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 248);
  Engine_ActorWaitForMove(RESCUE_HAMMET);
  OBJ_WALK(RESCUE_HAMMET, 456, 248);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 456, 360);
  Engine_ActorWaitForMove(RESCUE_HAMMET);
  OBJ_WALK(RESCUE_HAMMET, 456, 360);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  OBJ_WALK(ACTOR_PARTY_LEADER, 352, 360);
  Engine_ActorWaitForMove(RESCUE_HAMMET);
  OBJ_WALK(RESCUE_HAMMET, 352, 360);
  WAIT(20);
  SE(17);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);
  Engine_EventCloseScreen();
  WAIT(1);
  WAIT(210);
  Engine_EventRequestExit(4);
  EV_END();
}

// Choose guard 21 Mind Read thought from preselected random case.
static void LunpaFortress_ReadSentryThoughts(void) {

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
    Engine_ActorFaceDirection(21, FACING_NORTH + FACING_STEP, 0);
    WAIT(50);
    Engine_ActorFaceDirection(21, FACING_NORTHWEST + FACING_STEP, 0);
    WAIT(50);
    Engine_ActorFaceDirection(21, FACING_SOUTH + FACING_STEP, 0);
    WAIT(50);
    MESSAGE_SET(MsgLunpaTakeCareAnybody);
    MESSAGE_PUT(21, 0);
    break;
  }
}

// Guard looks around and asks about hearing someone.
static void LunpaFortress_TalkToListeningSentry(void) {

  s32 messageBase;

  OBJ_EMOTE(21, EMOTE_IN_FRONT | 1, 30);
  OBJ_DIR(21, FACING_NORTH + FACING_STEP, 0);
  WAIT(50);
  OBJ_DIR(21, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(50);
  OBJ_DIR(21, FACING_SOUTH + FACING_STEP, 0);
  WAIT(50);
  messageBase = MsgLunpaLeftGuardHearsSomeone;
  MESSAGE_SET(messageBase);
  MESSAGE_PUT(21, 0);
  OBJ_MODE(21, 4);
  WAIT(60);
  OBJ_DIR(21, FACING_NORTHWEST + FACING_STEP, 0);
  WAIT(40);
  MESSAGE_SET(messageBase + 1);
  MESSAGE_PUT(21, 0);
}

// Discuss Donpa/Dodonpa with attendant, with first/repeat/post-rescue paths.
static void LunpaFortress_TalkToDonpasAttendant(void) {
  s32 whisperMessage;
  s32 familyMessage;

  EV_BEGIN();
  if (FLAG_CHECK(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    MESSAGE_SET(MsgLunpaDonpaGrateful);
    MESSAGE_PUT(18, 0);
    EV_END();
  } else {
    if (FLAG_CHECK(0x313) != 0) {
      MESSAGE_SET(MsgLunpaMaybeDodonpasEyes);
      MESSAGE_OPEN(25, 0);
      EV_END();
    } else {
      OBJ_EMOTE(25, EMOTE_IN_FRONT | 2, 30);
      OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
      whisperMessage = MsgLunpaShhhPleaseDont;
      MESSAGE_SET(whisperMessage);
      MESSAGE_PUT(25, 0);
      OBJ_FACE(25, 24, 0);
      Engine_CameraMoveToActor(24, 1);
      Engine_CameraWaitForMove();
      WAIT(60);
      Engine_CameraMoveToActor(ACTOR_PARTY_LEADER, 1);
      WAIT(20);
      OBJ_EMOTE(25, EMOTE_IN_FRONT | 5, 60);
      MESSAGE_SET(whisperMessage + 1);
      MESSAGE_PUT(25, 0);
      OBJ_EMOTE(25, EMOTE_IN_FRONT | 7, 60);
      MESSAGE_SET(whisperMessage + 2);
      MESSAGE_PUT(25, 0);
      WAIT(70);
      OBJ_EMOTE(25, EMOTE_IN_FRONT, 60);
      OBJ_FACE(25, ACTOR_PARTY_LEADER, 0);
      MESSAGE_SET(whisperMessage + 3);
      MESSAGE_OPEN(25, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        MESSAGE_SET(whisperMessage + 4);
        MESSAGE_OPEN(25, 0);
      } else {
        MESSAGE_SET(whisperMessage + 5);
        MESSAGE_OPEN(25, 0);
      }
      WAIT(60);
      OBJ_EMOTE(25, EMOTE_IN_FRONT | 5, 60);
      familyMessage = MsgLunpaDonpaKnowsCoddled;
      MESSAGE_SET(familyMessage);
      MESSAGE_OPEN(25, 0);
      Engine_ActorRunRepeatedMotion(25, 1);
      MESSAGE_SET(familyMessage + 1);
      MESSAGE_OPEN(25, 0);
      Engine_ActorSetAnimationAndWait(25, 3);
      MESSAGE_SET(familyMessage + 2);
      MESSAGE_OPEN(25, 0);
      FLAG_SET(0x313);
      EV_END();
    }
  }
}

// Attendant thinks about punishing Dodonpa or Donpa's difficulty.
static void LunpaFortress_ReadAttendantThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED)) {
    MESSAGE_SET(MsgLunpaDifficultDonpaRight);
    MESSAGE_PUT(25, 0);
  } else {
    MESSAGE_SET(MsgLunpaSomeonePunishDodonpa);
    MESSAGE_PUT(25, 0);
  }
}

// Donpa wishes to forgive or free his son.
static void LunpaFortress_ReadDonpasThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED)) {
    MESSAGE_SET(MsgLunpaFatherStayAngry);
    MESSAGE_PUT(24, 0);
  } else {
    MESSAGE_SET(MsgLunpaFatherSorryDodonpa);
    MESSAGE_PUT(24, 0);
  }
}

// Before rescue Donpa sleeps; after rescue he asks attendant to reveal reward
// passage once.
static void LunpaFortress_OfferDonpasReward(void) {
  s32 rewardMessage;

  if (FLAG_CHECK(2369) != 0) {
    if (FLAG_CHECK(2382) == 0 && FLAG_CHECK(788) == 0) {
      rewardMessage = MsgLunpaThankHelpDodonpa;
      MESSAGE_SET(rewardMessage);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
      Engine_ActorRunRepeatedMotion(DONPA_ROOM_DONPA, 1);
      WAIT(30);
      OBJ_SPEED(DONPA_ROOM_DONPA, 6553, 3276);
      Actor_SetDestinationOffset(DONPA_ROOM_DONPA, -4, 0);
      Engine_ActorWaitForMove(DONPA_ROOM_DONPA);
      OBJ_MODE(DONPA_ROOM_DONPA, 3);
      WAIT(60);
      OBJ_SPEED(DONPA_ROOM_DONPA, 13107, 6553);
      Actor_SetDestinationOffset(DONPA_ROOM_DONPA, -6, 0);
      OBJ_FACE(DONPA_ROOM_DONPA, ACTOR_PARTY_LEADER, 0);
      Engine_ActorWaitForMove(DONPA_ROOM_DONPA);
      MESSAGE_SET(rewardMessage + 1);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
      Engine_ActorRunRepeatedMotion(DONPA_ROOM_DONPA, 1);
      OBJ_FACE(DONPA_ROOM_ATTENDANT, DONPA_ROOM_DONPA, 0);
      MESSAGE_SET(rewardMessage + 2);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
      WAIT(70);
      OBJ_MODE(DONPA_ROOM_ATTENDANT, 3);
      WAIT(60);
      OBJ_SPEED(DONPA_ROOM_ATTENDANT, 26214, 13107);
      OBJ_WALK(DONPA_ROOM_ATTENDANT, 880, 112);
      Engine_ActorWaitForMove(DONPA_ROOM_ATTENDANT);
      OBJ_DIR(DONPA_ROOM_ATTENDANT, FACING_NORTH + FACING_STEP, 0);
      MESSAGE_SET(rewardMessage + 3);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
      OBJ_MODE(DONPA_ROOM_DONPA, 3);
      WAIT(70);
      Actor_SetDestinationOffset(DONPA_ROOM_DONPA, 8, 0);
      Engine_ActorWaitForMove(DONPA_ROOM_DONPA);
      OBJ_MODE(DONPA_ROOM_DONPA, 5);
      MESSAGE_SET(rewardMessage + 4);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
      OBJ_WALK(ACTOR_PARTY_LEADER, 896, 120);
      Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
      Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, DONPA_ROOM_ATTENDANT, 0);
      WAIT(60);
      OBJ_MODE(DONPA_ROOM_ATTENDANT, 3);
      WAIT(30);
      FLAG_SET(788);
    } else {
      MESSAGE_SET(MsgLunpaWellWorriedDodonpa);
      MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
    }
  } else {
    MESSAGE_SET(MsgLunpaZZZZ);
    MESSAGE_PUT(DONPA_ROOM_DONPA, 0);
  }
}

// Run Djinn joining effect for actor 26 and persist received flag.
static void LunpaFortress_ReceiveFortressDjinni(void) {
  Engine_BattleFxRunPageEffectForSlot(26, 1, 5);
  Engine_GameFlagSet(0x94e);
}

// Beaten guard reacts and begs not to be hurt.
static void LunpaFortress_TalkToBeatenGuard(void) {
  Engine_ActorRunRepeatedMotion(14, 2);
  MESSAGE_SET(MsgLunpaOwwwDontHurt);
  MESSAGE_PUT(14, 0);
}

// Guard offers to give party what it wants.
static void LunpaFortress_TalkToYieldingGuard(void) {
  Engine_ActorRunRepeatedMotion(13, 2);
  MESSAGE_SET(MsgLunpaRightRightGive);
  MESSAGE_PUT(13, 0);
}

// Guard admits party is tougher than expected.
static void LunpaFortress_TalkToDefeatedGuard(void) {
  Engine_ActorRunRepeatedMotion(12, 2);
  MESSAGE_SET(MsgLunpaGuysTougherThought);
  MESSAGE_PUT(12, 0);
}

// Ask party whether it knows where Dodonpa is.
static void LunpaFortress_AskAboutDodonpasLocation(void) {
  MESSAGE_SET(MsgLunpaKnowWhereDodonpa);
  Engine_EventAskYesNo(18, 0);
}

// Resident is startled, asks why party entered, then promises silence when told
// about Hammet.
static void LunpaFortress_TalkToStartledResident(void) {
  s32 frightMessage;
  s32 purposeMessage;

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
      frightMessage = MsgLunpaWhWhWho;
      MESSAGE_SET(frightMessage);
      MESSAGE_PUT(20, 0);
      OBJ_EMOTE(20, EMOTE_IN_FRONT | 2, 30);
      MESSAGE_SET(frightMessage + 1);
      MESSAGE_PUT(20, 0);
      WAIT(30);
      OBJ_MODE(20, 4);
      WAIT(30);
    }
    purposeMessage = MsgLunpaDontLookNearly;
    MESSAGE_SET(purposeMessage);
    MESSAGE_PUT(20, 0);
    OBJ_EMOTE(20, EMOTE_IN_FRONT | 1, 40);
    MESSAGE_SET(purposeMessage + 1);
    MESSAGE_OPEN(20, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      MESSAGE_SET(purposeMessage + 2);
      MESSAGE_OPEN(20, 0);
      FLAG_SET(0x226);
    } else {
      MESSAGE_SET(purposeMessage + 3);
      MESSAGE_OPEN(20, 0);
    }
    FLAG_SET(0x227);
    EV_END();
  }
}

// If resident knows party purpose show merchant thought; otherwise cancel
// Psynergy and run conversation.
static void LunpaFortress_ReadStartledResidentThoughts(void) {

  if (Engine_GameFlagIsSet(0x226)) {
    MESSAGE_SET(MsgLunpaMaybeMerchantReason);
    MESSAGE_PUT(20, 0);
  } else {
    s16 *psynergyRequest = &gEventWork->psynergyRequest;

    *psynergyRequest = 0;
    Engine_PsynergyCancel();
    LunpaFortress_TalkToStartledResident();
  }
}

// Unused original actor callback performs no work.
static void UNUSED NoOpActorCallback(void) {}

// Dodonpa pleads with father, then Cell Key possession adds refusal message.
static void LunpaFortress_TalkToImprisonedDodonpa(void) {
  s32 pleaMessage = MsgLunpaWrongTurnOver;

  MESSAGE_SET(pleaMessage);
  MESSAGE_PUT(0x800d, 0);
  if (Engine_PartyInventoryFindOwner(ITEM_CELL_KEY) != -1) {
    Engine_MessageShowCentered(pleaMessage + 2, 1);
  }
}

// Read Dodonpa's revenge thought.
static void LunpaFortress_ReadImprisonedDodonpaThoughts(void) {
  MESSAGE_SET(MsgLunpaCantBelieveWhen);
  MESSAGE_PUT(13, 0);
}

// Initialize room-specific state and rescue aftermath on entry.
s32 FieldScene_DispatchActorUpdate(void) {
  struct FieldActor *sceneActor;

  gLunpaFortressRandomPick = (u32)Engine_RandomNext() * 7 >> 16;
  if (gGameState.scene == (s32)&SceneId_LunpaFortress1) {
    Engine_MapSetWorkFlagBits9To11(0xe00);
    LunpaFortress_InitializeOuterRooms();
  }
  if (gGameState.scene == (s32)&SceneId_LunpaFortress2)
    LunpaFortress_InitializeGuardRooms();
  if (gGameState.scene == (s32)&SceneId_LunpaFortress3)
    LunpaFortress_InitializeColumnRoom();
  if (gGameState.scene == (s32)&SceneId_LunpaFortress4) {
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
    Engine_ActorSetSpriteFlags(Actor_Get(12), 0);
    OBJ_DIR(12, FACING_EAST, 0);
    OBJ_MODE(12, 0);
    LunpaFortress_RestoreRescueRoomDoors();
    sceneActor = Actor_Get(8);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor = Actor_Get(9);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor = Actor_Get(10);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    Engine_MapSetWorkFlagBits9To11(0xe00);
    if (gGameState.entrance == 4) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      LunpaFortress_ResolveHammetRescueAndDonpaPunishment();
    }
    if (gGameState.entrance == 3) {
      Engine_MapSetWorkFlagBits9To11(0xc00);
      if (FLAG_CHECK(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        OBJ_POS_SET(12, 0, 0);
        OBJ_POS_SET(16, 432, 344);
        OBJ_DIR(16, FACING_SOUTH + FACING_STEP, 0);
        OBJ_POS_SET(13, 456, 288);
        OBJ_DIR(13, FACING_SOUTH + FACING_STEP, 0);
        OBJ_POS_SET(17, 456, 320);
        Engine_ActorSetSpriteFlags(Actor_Get(17), 0);
      }
    }
    sceneActor = Actor_Get(15);
    if (sceneActor != 0)
      Engine_ActorSetSpriteFlags(sceneActor, 0);
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    sceneActor->scaleX = 0xcccc;
  }
  return 0;
}

// Restore door states and schedule guard watchers by entrance.
static void LunpaFortress_InitializeOuterRooms(void) {
  LunpaFortress_RestoreOuterDoors();
  switch (gGameState.entrance) {
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Task_AddCallback(LunpaFortress_CheckEntranceGuard, 3200);
    Task_AddCallback(LunpaFortress_CheckFirstEntranceSentry, 3200);
    Task_AddCallback(LunpaFortress_CheckSecondEntranceSentry, 3200);
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
    Task_AddCallback(LunpaFortress_UpdateOuterGuardDetection, 3200);
    Task_AddCallback(LunpaFortress_CheckOuterDoorApproach, 3200);
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
    Task_AddCallback(LunpaFortress_CheckUpperHallGuard, 3200);
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

// Restore defeated guards/key pickup and set watchers/return animation by
// entrance.
static void LunpaFortress_InitializeGuardRooms(void) {
  struct FieldActor *keyActor;

  LunpaFortress_RestoreGuardRoomDoors();
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
    Task_AddCallback(LunpaFortress_UpdatePairedGuardDetection, 3200);
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
    Task_AddCallback(LunpaFortress_UpdateHallGuardDetection, 3200);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    break;
  case 11:
  case 12:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    if (FLAG_CHECK(0x94a)) {
      LunpaFortress_RestoreThreeDefeatedGuards();
    }
    break;
  case 31:
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, LUNPA_RETURN_TRANSITION_VARIANT);
    LunpaFortress_RestoreThreeDefeatedGuards();
    break;
  case 14:
  case 15:
  case 16:
    Task_AddCallback(LunpaFortress_CheckGuardDoorApproach, 3200);
    break;
  default:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Engine_MapSetWorkFlagBits9To11(0xe00);
    break;
  }
  keyActor = Actor_Get(8);
  Engine_ActorSetSpriteFlags(Actor_Get(8), 0);
  Engine_ActorSetSpritePriority(8, 1);
  keyActor->scaleX = 0xc000;
  keyActor->scaleY = 0xc000;
}

// Restore moved column, Whirlwind obstacles and Reveal switches then actor
// priority/collision.
static void LunpaFortress_InitializeColumnRoom(void) {
  struct FieldActor *sceneActor;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  LunpaFortress_RestoreColumnRoomDoors();
  if (FLAG_CHECK(0x943)) {
    LunpaFortress_RestoreLoweredColumn();
  }
  FLAG_SET(0x217);
  FLAG_SET(0x218);
  if (FLAG_CHECK(0x944)) {
    OBJ_POS_SET(8, 0, 0);
    Engine_GameFlagClear(0x217);
  }
  if (FLAG_CHECK(0x945)) {
    OBJ_POS_SET(9, 0, 0);
    LunpaFortress_OpenWhirlwindPassage();
  }
  if (FLAG_CHECK(0x946)) {
    OBJ_POS_SET(10, 0, 0);
    Engine_GameFlagClear(0x218);
  }
  if (FLAG_CHECK(0x947)) {
    LunpaFortress_OpenFirstRevealPassage();
  }
  if (FLAG_CHECK(0x948)) {
    LunpaFortress_OpenSecondRevealPassage();
  }
  EV_BEGIN();
  sceneActor = Actor_Get(8);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(9);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(10);
  if (sceneActor != 0) {
    sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  }
  sceneActor = Actor_Get(11);
  if (sceneActor != 0) {
    Engine_ActorSetSpriteFlags(sceneActor, 0);
  }
  sceneActor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  sceneActor = Actor_Get(12);
  if (sceneActor != 0) {
    sceneActor->collisionFlags |= 0x10;
  }
  Engine_ActorSetSpriteFlags(Actor_Get(11), 0);
  EV_END();
  Engine_MapSetWorkFlagBits9To11(0xe00);
}

// Replay opened outer-fortress doors from three flags.
static void LunpaFortress_RestoreOuterDoors(void) {
  if (Engine_GameFlagIsSet(0x35a)) {
    LunpaFortress_OpenOuterFortressDoor(0);
  }
  if (Engine_GameFlagIsSet(0x35b)) {
    LunpaFortress_OpenOuterFortressDoor(1);
  }
  if (Engine_GameFlagIsSet(0x35c)) {
    LunpaFortress_OpenOuterFortressDoor(2);
  }
}

// Replay two opened guard-room doors.
static void LunpaFortress_RestoreGuardRoomDoors(void) {
  if (Engine_GameFlagIsSet(0x358)) {
    LunpaFortress_OpenGuardRoomDoor(0);
  }
  if (Engine_GameFlagIsSet(0x359)) {
    LunpaFortress_OpenGuardRoomDoor(1);
  }
}

// Replay three opened column-room doors.
static void LunpaFortress_RestoreColumnRoomDoors(void) {
  if (Engine_GameFlagIsSet(0x355)) {
    LunpaFortress_OpenColumnRoomDoor(0);
  }
  if (Engine_GameFlagIsSet(0x356)) {
    LunpaFortress_OpenColumnRoomDoor(1);
  }
  if (Engine_GameFlagIsSet(0x357)) {
    LunpaFortress_OpenColumnRoomDoor(2);
  }
}

// Replay five opened rescue-room doors.
static void LunpaFortress_RestoreRescueRoomDoors(void) {
  if (Engine_GameFlagIsSet(0x350)) {
    LunpaFortress_OpenRescueRoomDoor(0);
  }
  if (Engine_GameFlagIsSet(0x351)) {
    LunpaFortress_OpenRescueRoomDoor(1);
  }
  if (Engine_GameFlagIsSet(0x352)) {
    LunpaFortress_OpenRescueRoomDoor(2);
  }
  if (Engine_GameFlagIsSet(0x353)) {
    LunpaFortress_OpenRescueRoomDoor(3);
  }
  if (Engine_GameFlagIsSet(0x354)) {
    LunpaFortress_OpenRescueRoomDoor(4);
  }
}

// The shared source and field.h declarations lack const, but these three
// tables occupy .rodata. Pixel steps pack X above Z in two halfwords.
// Unsigned packing also defines the entries with negative X steps.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))

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
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CheckLockedCellDoor) },
    { 0xc402, 40, 0x35a, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { 0x4402, 40, 0x35a, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { 0xc402, 41, 0x35b, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { 0x4402, 41, 0x35b, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { 0xc402, 42, 0x35c, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { 0x4402, 42, 0x35c, EVENT_SCRIPT(LunpaFortress_UnlockOuterFortressDoor) },
    { EVENT_TOUCH, 50, 0x224, EVENT_SCRIPT(LunpaFortress_ExpelPartyAtEntrance) },
    { EVENT_TOUCH, 51, 0x225, EVENT_SCRIPT(LunpaFortress_ExpelPartyAtSentries) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaResting) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLunpaWhereIsDodonpa) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetBuysDrink) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetMoneyTree) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToStartledResident) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToListeningSentry) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLunpaWhoFoughtDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLunpaWifeSeeksDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgLunpaKalayBuysDrink) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetKeepsEarnings) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReadStartledResidentThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReadSentryThoughts) },
    { EVENT_RAISED, 93, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ExpelPartyAfterOuterDetection) },
    { EVENT_RAISED, 94, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ExpelPartyAfterUpperHallDetection) },
    { EVENT_RAISED, 95, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ExpelPartyAtEntrance) },
    { EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ExpelPartyAtSentries) },
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
    { EVENT_TOUCH, 30, 0x949, EVENT_SCRIPT(LunpaFortress_StartHallGuardBattle) },
    { EVENT_TOUCH, 31, 0x94a, EVENT_SCRIPT(LunpaFortress_StartThreeGuardBattle) },
    { EVENT_TOUCH, 32, 0x94b, EVENT_SCRIPT(LunpaFortress_StartEastWingGuardBattle) },
    { EVENT_TOUCH, 33, 0x94c, EVENT_SCRIPT(LunpaFortress_StartWestWingGuardBattle) },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CheckLockedCellDoor) },
    { 0x4402, 40, 0x358, EVENT_SCRIPT(LunpaFortress_UnlockGuardRoomDoor) },
    { 0xc402, 41, 0x359, EVENT_SCRIPT(LunpaFortress_UnlockGuardRoomDoor) },
    { 0x4402, 41, 0x359, EVENT_SCRIPT(LunpaFortress_UnlockGuardRoomDoor) },
    { PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CollectCellKey) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CollectCellKey) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_AskAboutDodonpasLocation) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgLunpaChildMustGrowUp) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgLunpaPapaWatchesLadies) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgLunpaChildTooResponsible) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpaExhausted) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgLunpaWomenWantHome) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToDefeatedGuard) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToYieldingGuard) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToBeatenGuard) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToDonpasAttendant) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_OfferDonpasReward) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReceiveFortressDjinni) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaSuccessor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaProtectsFortress) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgLunpaDaughterWillNotMarryPapa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgLunpaDonpappaJudges) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgLunpaFearWithoutDonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgLunpaHopeDonpaRecovers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLunpaShouldNotHaveFought) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetInCave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgLunpaHiddenDoorSwitch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReadDonpasThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReadAttendantThoughts) },
    { EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ConfrontCaughtPairedPatrol) },
    { EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ExpelPartyAfterHallDetection) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEvents3[] = {
    { 0x21, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { 0x31, 6, CONDITION_ALWAYS, 6 },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CheckLockedCellDoor) },
    { SEARCH(SEARCH_UNNAMED), 98, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ShowLockedDoor) },
    { 0xc402, 40, 0x355, EVENT_SCRIPT(LunpaFortress_UnlockColumnRoomDoor) },
    { 0xc402, 41, 0x356, EVENT_SCRIPT(LunpaFortress_UnlockColumnRoomDoor) },
    { 0xc402, 42, 0x357, EVENT_SCRIPT(LunpaFortress_UnlockColumnRoomDoor) },
    { 0x4402, 42, 0x357, EVENT_SCRIPT(LunpaFortress_UnlockColumnRoomDoor) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_LowerColumnAfterPushing) },
    { 0x202, 70, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_PushAdjacentStagedActor) },
    { SEARCH(SEARCH_UNNAMED), 51, 0x218, EVENT_SCRIPT(LunpaFortress_FlipSecondHiddenSwitch) },
    { SEARCH(SEARCH_UNNAMED), 52, 0x217, EVENT_SCRIPT(LunpaFortress_FlipFirstHiddenSwitch) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ShowHiddenSwitches) },
    { PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_HideHiddenSwitches) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ClearWhirlwindWardrobe) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ClearWhirlwindFirewood) },
    { PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ClearWhirlwindObstruction) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x943, EVENT_SCRIPT(LunpaFortress_SettleMovedColumn) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gLunpaFortressEventsOther[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { 0x21, 5, CONDITION_ALWAYS, 5 },
    { SEARCH(SEARCH_UNNAMED), 99, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_CheckLockedCellDoor) },
    { 0xc402, 40, 0x350, EVENT_SCRIPT(LunpaFortress_UnlockRescueRoomDoor) },
    { 0xc402, 41, 0x351, EVENT_SCRIPT(LunpaFortress_UnlockRescueRoomDoor) },
    { 0xc402, 42, 0x352, EVENT_SCRIPT(LunpaFortress_UnlockRescueRoomDoor) },
    { 0xc402, 43, 0x353, EVENT_SCRIPT(LunpaFortress_UnlockRescueRoomDoor) },
    { 0xc402, 44, 0x354, EVENT_SCRIPT(LunpaFortress_UnlockRescueRoomDoor) },
    { EVENT_TOUCH, 20, 0x270, EVENT_SCRIPT(LunpaFortress_RescueHammetAndStartMonsterBattle) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLunpaHammetStillInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgLunpaDodonpaSecretlyInside) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgLunpaWrongTurnOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgLunpaCantBelieveWhen) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_TalkToImprisonedDodonpa) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, EVENT_SCRIPT(LunpaFortress_ReadImprisonedDodonpaThoughts) },
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
