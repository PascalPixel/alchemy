/// @file
/// The Yampi Desert, in twelve rooms that share this overlay
/// (SceneId_YampiDesert1 to SceneId_YampiDesert12): its travelers, the
/// scorpion in its way, the stakes Pound sinks and the sand slides Sand slips
/// under, and its tables of entrances, placements and triggers by room.
#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

/* VIEW(placements): routine PartyInventory_GiveItem as yampi_desert/scene.h
 * declared it; the joined file declares it otherwise. */
void PartyInventory_GiveItem__placements(s32 item, s32 flags) __asm__(
    "PartyInventory_GiveItem");
#define PartyInventory_GiveItem PartyInventory_GiveItem__placements
#include "types.h"
#include "item_ids.h"
#include "system/io_reg.h"
#include "flags.h"
#include "system/task.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "scene_ids.h"
#include "system/frame.h"

extern s16 gYampiDesert_Room1Stakes[];
extern s16 gYampiDesert_Room3Stakes[];
extern s16 gYampiDesert_Room4Stakes[];
extern s16 gYampiDesert_Room5Stakes[];
extern s32 gYampiDesert_Room12Stakes[];
extern s32 gYampiDesert_RaisedEvent;

struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Get(s32 flag);
void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
void FieldMessage_Show(s32 message, s32 position);
void Actor_SetSpriteColor(struct Actor *object, s32 attribute);
void Actor_SetWideSprite(s32 actor, s32 value);
void FieldPsynergy_RunHandler(s32 type);

void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_OpenLadderPit(void);
void YampiDesert_SetSlideActorsShown(s32 show);
void YampiDesert_HideSlideActors(void);
void YampiDesert_SetActorsHidden(s32 show, s32 first, u32 count);
void YampiDesert_UpdateSandSlide(void);
void YampiDesert_TrackUpperLevel(void);
void YampiDesert_OpenScoopHole(void);
void Actor_Hide(s32 id);
void Scene_RunUnlessFlag(void);
void YampiDesert_SetUpRoom12SlideActors(void);
void YampiDesert_ClimbOut(struct Actor *object, s32 x, s32 z, s32 mode);
void YampiDesert_MatchStakePriority(void);
void YampiDesert_PlaceStakes(const void *script);
void Pound_SinkTarget(u16 *list, s32 mode, s32 count);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 actor, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actor, s32 x, s32 z,
                                                    s32 angle);
s32 Event_AskYesNoAndShowResponse(s32 speaker, s32 flags);
void Actor_DisableById(s32 actor);
void Field_RequestDjinnBattle(s32 a, s32 b);
void FieldEffect_StartBlankOverlay(s32 value);
void BattleEffect_LoadActionEffectResources(s32 a, s32 b);
void FieldPsynergy_SetupActorPair(s32 a, s32 b);
void BattleEffect_StopMainActor(void);
void BattleEffect_CleanupSceneObjects(void);
void Scene_QueueMusic(s32 cue);
void Rope_Initialize(void);
void Rope_Place(s32 a, s32 b, s32 c, s32 d);
s32 Actor_GetCellIndex(struct Actor *object);
void YampiDesert_ElderRefusesToQuit(void);
void YampiDesert_BlockPoundedStakes(u16 *list);
void YampiDesert_UnblockStakes(u16 *list);
void Actor_FaceActorById(s32 id, s32 target, s32 flags);
void Actor_ArmCallback(s32 id, s32 angle, s32 flags);
void FieldCamera_FollowActor(s32 id, s32 mode);
void Actor_WatchAndSetScript(s32 id, s32 target);
void FieldEvent_Wait(s32 frames);
void PartyInventory_GiveItem(s32 item, s32 flags);
struct Actor *YampiDesert_CreateScoopGem(void);
void Actor_Release(struct Actor *object);
void Sound_Play(s32 cue);
extern struct Actor *gYampiDesert_SandRipple;
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 actor, s32 x, s32 z);
void Actor_SetSpriteFlags(struct Actor *object, u32 flags);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetMoveTarget(struct Actor *object, s32 x, s32 y, s32 z);
void Actor_WaitForMove(struct Actor *object);
void Actor_FaceDirectionById(s32 actor, s32 angle);
void Import_EventSetExitRequest(s32 mode);
void Camera_SetSpeedParameters(s32 a, s32 b);
void FieldCamera_MoveWithinMapBounds(s32 x, s32 a, s32 z, s32 b);
void Actor_MoveToAndWaitById(u32 objectId, s32 x, s32 z);
void Actor_Stop(s32 id);
void YampiDesert_ClearRaisedEvent(void);
void YampiDesert_PlayScorpionRumble(void);
void YampiDesert_SetUpRoom4Scorpion(void);
void YampiDesert_ReadyStake(s32 id);
void YampiDesert_TrackUpperLevelFromBelow(void);
void Actor_SetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

static inline void Scene_TaskAdd(void (*func)(void), s32 priority) {
  Task_Add(func, priority);
}

static inline s32 Scene_FlagTest(s32 flag) { return GameFlag_Get(flag); }

static inline void Scene_ArmCallback(s32 id, s32 angle, s32 flags) {
  Actor_ArmCallback(id, angle, flags);
}

static inline void Scene_Call4098(s32 actor, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(actor, x, z, angle);
}

static inline void Scene_Call41d8(s32 a, s32 b, s32 c, s32 d) {
  Rope_Place(a, b, c, d);
}

static inline void Scene_Call4178(s32 a, s32 b, s32 c) {
  Actor_ReturnHome(a, b, c);
}

#include "types.h"
#include "module/tla.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "map.h"
#include "system/field_effect.h"
#include "field/field_sequence_event.h"
#include "flag_call.h"
#include "system/heap_release.h"
#include "item_icon.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/field_effect.h"
#include "module/scene/scene.h"
#include "graphics/resource.h"
#include "call.h"
#include "system/input.h"
#include "system/frame.h"
#include "system/task.h"
#include "fixed_math.h"
#include "version.h"
#include "system/iwram_call.h"
#include "macros.h"

void YampiDesert_RippleFollowsScorpion(void);

extern u32 gYampiDesert_SandCloudCells[];

extern u32 gYampiDesert_SandCloudDrifts[];

void YampiDesert_ScorpionUsesScoop(void);
// Const: the cell helper only computes from its argument.
s32 YampiDesert_CellCenter(s32 cell) __attribute__((const));
extern u32 gYampiDesert_SlideDust[];
void YampiDesert_SetUpRoom1(void);
void YampiDesert_SetUpRoom2(void);
void YampiDesert_SetUpRoom3(void);
void YampiDesert_SetUpRoom4(void);
void YampiDesert_SetUpRoom5(void);

void YampiDesert_SetUpRoom6(void);

void YampiDesert_SetUpRoom8(void);
void YampiDesert_SetUpRoom9(void);
void YampiDesert_SetUpRoom10(void);
void YampiDesert_SetUpRoom11(void);
void YampiDesert_SetUpRoom12(void);

void YampiDesert_DropScoopGem(void);

void YampiDesert_TalkToElder(void);

void YampiDesert_ReadSignpost(void);

void YampiDesert_StepOffStakeTops(void);

void YampiDesert_CastSand(void);

void YampiDesert_EndSand(void);

void YampiDesert_WatchScorpionStop(void);

void YampiDesert_StirScorpion(void);

void YampiDesert_StirScorpionAgain(void);

void YampiDesert_CornerScorpion(void);

void YampiDesert_TrapKingScorpion(void);

void YampiDesert_RevealRockHint(void);

void YampiDesert_HideRockHint(void);

void YampiDesert_RevealMedalSpot(void);

void YampiDesert_ShowSlideActors(void);

void YampiDesert_ClimbRevealedLadder(void);
void YampiDesert_ClimbOutOfRoom11(void);

extern u32 gYampiDesert_TravelerLooksAround[];

extern u32 gYampiDesert_OtherTravelerLooksAround[];

s32 MapGrid_GetTerrainKind(s32 layer, s32 x, s32 z);
s32 MapCollision_FindOrAddTag(s32 layer, s32 shape, s32 height);

void YampiDesert_StartRisingSand(s32 unused, s32 value);

void Icon_DrawItem(s32 item);
void FieldEffect_SetAttached(s32 actor, s32 a);
void FieldCamera_WaitForArrival();

extern struct SceneEntrance gYampiDesert_Entrances[];
extern struct SceneEntrance gYampiDesert_Room11Entrances[];
extern u32 gYampiDesert_Room10Regions[];
extern s16 gYampiDesert_Room11Regions[];

void YampiDesert_FightBlitz(void) { Field_RequestDjinnBattle(19, 68); }

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert11)
    return gYampiDesert_Room11Entrances;
  return gYampiDesert_Entrances;
}

const u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_YampiDesert10)
    return gYampiDesert_Room10Regions;
  if (scene == (s32)&SceneId_YampiDesert11)
    return gYampiDesert_Room11Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"

extern struct ActorPlacement gYampiDesert_Placements[];
extern struct ActorPlacement gYampiDesert_Room1Placements[];
extern struct ActorPlacement gYampiDesert_Room2Placements[];
extern struct ActorPlacement gYampiDesert_Room2PlacementsAfterMeeting[];
extern struct ActorPlacement gYampiDesert_Room3Placements[];
extern struct ActorPlacement gYampiDesert_Room4Placements[];
extern struct ActorPlacement gYampiDesert_Room5Placements[];
extern struct ActorPlacement gYampiDesert_Room8Placements[];
extern struct ActorPlacement gYampiDesert_Room10Placements[];
extern struct ActorPlacement gYampiDesert_Room12Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_YampiDesert1)
    return gYampiDesert_Room1Placements;
  if (scene == (s32)&SceneId_YampiDesert2) {
    if (GameFlag_Get(0x8f0))
      return gYampiDesert_Room2PlacementsAfterMeeting;
    return gYampiDesert_Room2Placements;
  }
  if (scene == (s32)&SceneId_YampiDesert3)
    return gYampiDesert_Room3Placements;
  if (scene == (s32)&SceneId_YampiDesert4)
    return gYampiDesert_Room4Placements;
  if (scene == (s32)&SceneId_YampiDesert5)
    return gYampiDesert_Room5Placements;
  if (scene == (s32)&SceneId_YampiDesert8)
    return gYampiDesert_Room8Placements;
  if (scene == (s32)&SceneId_YampiDesert10)
    return gYampiDesert_Room10Placements;
  if (scene == (s32)&SceneId_YampiDesert12)
    return gYampiDesert_Room12Placements;
  return gYampiDesert_Placements;
}

void YampiDesert_ScoopTreasurePit(s32 a) { Scene_Call4178(a, 64, 0x302); }

extern struct Trigger gYampiDesert_Triggers[];
extern struct Trigger gYampiDesert_Room1Triggers[];
extern struct Trigger gYampiDesert_Room2Triggers[];
extern struct Trigger gYampiDesert_Room3Triggers[];
extern struct Trigger gYampiDesert_Room4Triggers[];
extern struct Trigger gYampiDesert_Room5Triggers[];
extern struct Trigger gYampiDesert_Room6Triggers[];
extern struct Trigger gYampiDesert_Room7Triggers[];
extern struct Trigger gYampiDesert_Room8Triggers[];
extern struct Trigger gYampiDesert_Room9Triggers[];
extern struct Trigger gYampiDesert_Room10Triggers[];
extern struct Trigger gYampiDesert_Room11Triggers[];
extern struct Trigger gYampiDesert_Room12Triggers[];

/// Returns the current room's table.
// As ☀️'s Scene_GetEntrances picks one.
const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_YampiDesert1)
    return gYampiDesert_Room1Triggers;
  if (scene == (s32)&SceneId_YampiDesert2)
    return gYampiDesert_Room2Triggers;
  if (scene == (s32)&SceneId_YampiDesert3)
    return gYampiDesert_Room3Triggers;
  if (scene == (s32)&SceneId_YampiDesert4)
    return gYampiDesert_Room4Triggers;
  if (scene == (s32)&SceneId_YampiDesert5)
    return gYampiDesert_Room5Triggers;
  if (scene == (s32)&SceneId_YampiDesert6)
    return gYampiDesert_Room6Triggers;
  if (scene == (s32)&SceneId_YampiDesert7)
    return gYampiDesert_Room7Triggers;
  if (scene == (s32)&SceneId_YampiDesert8)
    return gYampiDesert_Room8Triggers;
  if (scene == (s32)&SceneId_YampiDesert9)
    return gYampiDesert_Room9Triggers;
  if (scene == (s32)&SceneId_YampiDesert10)
    return gYampiDesert_Room10Triggers;
  if (scene == (s32)&SceneId_YampiDesert11)
    return gYampiDesert_Room11Triggers;
  if (scene == (s32)&SceneId_YampiDesert12)
    return gYampiDesert_Room12Triggers;
  return gYampiDesert_Triggers;
}

void YampiDesert_TalkToTraveler(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgWeFollowedThe);
  Event_AskYesNoAndShowResponse(8, 0);
  Event_End();
}

void YampiDesert_ElderRefusesToQuit(void) {
  Event_SetMessageId((s32)MsgCanTWe);
  Actor_SetModeById(12, 4);
  Event_ShowMessageAndWait(12, 0);
  Scene_ArmCallback(13, FACING_WEST, 20);
  Actor_SetModeById(13, 4);
  Event_ShowMessageAndWait(13, 0);
  Actor_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 20);
  Actor_ArmCallback(13, 0, 20);
  Scene_ArmCallback(13, 0x3000, 0);
  Event_ShowMessageAndWait(13, 0);
}

void YampiDesert_TalkToElderAboutWay(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(0x88f))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(12, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgAsYouCan);
  Event_ShowMessageAndWait(12, 0);
  GameFlag_Set(0x88f);
  Event_End();
}

void YampiDesert_TalkToElder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (!Scene_FlagTest(0x88f))
    YampiDesert_ElderRefusesToQuit();
  Actor_FaceActorById(13, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgThisIsWhere);
  Event_ShowMessageAndWait(13, 0);
  GameFlag_Set(0x88f);
  Event_End();
}

void YampiDesert_ReadSignpost(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Event_End();
}

void YampiDesert_PoundRoom1Stake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesert_Room1Stakes, a, b);
}

void YampiDesert_PoundRoom3Stake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesert_Room3Stakes, a, b);
}

#define DEFINE_POUND_STAKE_STOPPING_SCORPION(name, data1)                      \
  void name(s32 a, s32 b) {                                                    \
    Pound_SinkTarget(data1, a, b);                                             \
    if (a == 1) {                                                              \
      if (gYampiDesert_RaisedEvent == 0)                                       \
        gYampiDesert_RaisedEvent = 210;                                        \
    }                                                                          \
  }

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundRoom4Stake,
                                     gYampiDesert_Room4Stakes)

void YampiDesert_PoundRoom12Stake(s32 a, s32 b) {
  Pound_SinkTarget(gYampiDesert_Room12Stakes, a, b);
}

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundRoom5Stake,
                                     gYampiDesert_Room5Stakes)

void YampiDesert_StepOntoStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert3)
    list = gYampiDesert_Room3Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert4)
    list = gYampiDesert_Room4Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert5)
    list = gYampiDesert_Room5Stakes;
  if (list == NULL)
    return;
  while (*list != 0xffff) {
    struct Actor *object = Actor_GetById(*list);

    list += 2;
    if (!GameFlag_Get(*list)) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      s32 index = Actor_GetCellIndex(object);
      s32 shape = MapGrid_GetTerrainKind(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);

      cells += index;
      shapes += index;
      object->motionFlags = 0;
      cells->collisionCode = 0;
      cells->attributeB |= 128;
      *shapes = MapCollision_FindOrAddTag(layer, shape, (height >> 19) + 4);
    }
    list++;
  }
  GameFlag_Set(FLAG_SCENE_TEMP_0B);
}

void YampiDesert_StepOffStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert3)
    list = gYampiDesert_Room3Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert4)
    list = gYampiDesert_Room4Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert5)
    list = gYampiDesert_Room5Stakes;
  if (list == NULL)
    return;
  if (*list == 0xffff)
    goto done;
next:
  {
    struct Actor *object = Actor_GetById(*list);
    s32 set;

    list += 2;
    set = GameFlag_Get(*list);
    if (!set) {
      s32 layer = object->terrainId;
      struct MapCell *cells = map->layers[layer].cells;
      u8 *shapes =
          (u8 *)((((s32)cells - (s32)Ram_MapCellBuffer) >> 2) + (s32)grid);
      /* FAKEMATCH(pin): the index in r10. */
      register s32 index asm("r10") = Actor_GetCellIndex(object);
      s32 shape = MapGrid_GetTerrainKind(layer, object->x, object->z);
      s32 height = Map_GetTerrainHeight(layer, object->x, object->z);
      struct MapCell *cell = cells + index;

      object->motionFlags = set;
      cell->collisionCode = 255;
      cell->attributeB &= 127;
      shapes[index] =
          MapCollision_FindOrAddTag(layer, shape, (height >> 19) - 4);
    }
  }
  list++;
  if (*list != 0xffff)
    goto next;
done:
  GameFlag_Clear(FLAG_SCENE_TEMP_0B);
}

/// For each six-byte record (actor, unused, flag) up to an actor of 0xffff:
/// when the record's flag is set, the cell under the actor in map layer 2 takes
/// collision code 255.
void YampiDesert_BlockPoundedStakes(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  /* FAKEMATCH(pin): the limit in r2. */
  register s32 limit asm("r2");

  goto next;
  do {
    /* FAKEMATCH(barrier): the record pointer passed through an empty asm. */
    asm("" : "+l"(list));
    object = Actor_GetById(list[0]);
    if (GameFlag_Get(list[2]))
      *(u8 *)((u32)map->layers[2].cells + (Actor_GetCellIndex(object) << 2) +
              2) = 255;
    list += 3;
  next:
    limit = 0xff;
  } while (list[0] != (limit << 8) + 0xff);
}

/// For each six-byte record up to an actor of 0xffff, the cell under the actor
/// in map layer 2 takes collision code 232.
void YampiDesert_UnblockStakes(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapCell *cell;
  s32 collisionCode;

  if (*list != 0xffff) {
  loop:
    cell = &map->layers[2].cells[Actor_GetCellIndex(Actor_GetById(*list))];
    collisionCode = 232;
    /* FAKEMATCH(dead): the advance in a once-only block. */
    do {
      list += 3;
    } while (0);
    cell->collisionCode = collisionCode;
    if (*list != 0xffff)
      goto loop;
  }
}

void YampiDesert_CastSand(void) {
  s16 scene;

  Actor_GetById(gMainRoutineWork.currentOwner)->terrainId = 2;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_YampiDesert1)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room1Stakes);
  else if (scene == (s32)&SceneId_YampiDesert3)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room3Stakes);
  else if (scene == (s32)&SceneId_YampiDesert4)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room4Stakes);
  else if (scene == (s32)&SceneId_YampiDesert5)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room5Stakes);
}

void YampiDesert_EndSand(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s16 scene;

  leader->terrainId = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_YampiDesert1)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room1Stakes);
  else if (scene == (s32)&SceneId_YampiDesert3)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room3Stakes);
  else if (scene == (s32)&SceneId_YampiDesert4)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room4Stakes);
  else if (scene == (s32)&SceneId_YampiDesert5)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room5Stakes);
}

void YampiDesert_WatchScorpionStop(void) {
  s16 scene;

  Event_Start();
  Event_ResolvePendingActions(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_YampiDesert4) {
    FieldCamera_FollowActor(16, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  } else if (scene == (s32)&SceneId_YampiDesert5) {
    FieldCamera_FollowActor(21, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  }
  while (gYampiDesert_RaisedEvent == 210)
    wait(1);
  Event_End();
}

#undef PartyInventory_GiveItem
#include "../field/field_effect/advance_motion_priority.c"

#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef DEFINE_POUND_STAKE_STOPPING_SCORPION
// The Yampi Desert, between Osenia's south and Alhafra: twelve rooms of sand.
// Pound sinks its wooden stakes and Sand lets the party slip under them; a
// group of Madra's travelers hunting Briggs is stuck halfway (Room2); the King
// Scorpion moves under the sand through Room4 and Room5, stirred out by Pound
// and trapped between stakes, until it Scoops the leader down into its pit
// (Room10), where its battle is fought, the Scoop Gem is won, and a column of
// rising sand carries the party out. Rooms 8 and 12 are the cave's sand
// slides, and Reveal shows the rocks' hints and a hidden Lucky Medal.

#include "text/msg_ids.h"

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Oam_AddSpriteAtDepthClamped(u32 *oam, s32 depth);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
s32 Trig_Sin(s32 angle);

void YampiDesert_BurstSand(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  u32 i;

  options.update = Effect_AdvanceMotionAtLeaderPriority;
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 14);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 14);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((u32)(Random16() << 17) >> 16) + -0x10000;
    velocity[2] += (u16)Random16() + -0x8000;
    FieldEffect_Spawn(x, y, z, velocity[0], velocity[1], velocity[2],
                      0x01000001, &options);
  }
}

// Pound on the first screen's wooden column: the scorpion moving under the
// sand bursts out and flees to the next screen (flag 0x889).
void YampiDesert_StirScorpion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveToFixed(px(300), -1, px(248), 1);
  Actor_MoveToAndWaitById(16, 254, 248);
  Actor_MoveToAndWaitById(16, 352, 240);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  FieldEvent_Wait(10);
  Task_Remove(YampiDesert_RippleFollowsScorpion);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(17, 0, 0);
  FieldEvent_Wait(40);
  YampiDesert_BurstSand(0x1600000, 0, 0xf80000);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Actor_GetById(16)->velocityY = px(6);
  Actor_SetSpriteColor(Actor_GetById(16), 0);
  Actor_SetSpriteFlags(Actor_GetById(16), 1);
  Actor_MoveToAndWaitById(16, 380, 248);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeedFixed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(16, 428, 248);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
  GameFlag_Set(0x889);
  Event_End();
}

// As YampiDesert_StirScorpion, on the second screen (flag 0x88a).
void YampiDesert_StirScorpionAgain(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveWithinMapBounds(px(246), -1, px(616), 1);
  Actor_MoveToAndWait(21, 248, 648);
  Actor_MoveToAndWait(21, 248, 576);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  FieldEvent_Wait(10);
  Task_Remove(YampiDesert_RippleFollowsScorpion);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  FieldEvent_Wait(20);
  YampiDesert_BurstSand(0xf80000, 0, 0x2400000);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  Actor_GetById(21)->velocityY = px(6);
  Actor_SetSpriteColor(Actor_GetById(21), 0);
  Actor_SetSpriteFlags(Actor_GetById(21), 1);
  Actor_MoveToAndWaitById(21, 256, 560);
  FieldEvent_Wait(20);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Sound_Play(SE_WATER_MOVE);
  Actor_SetSpeedFixed(21, 0xcccc, 0x6666);
  Actor_WalkToAndWait(21, 248, 496);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  GameFlag_Set(0x88a);
  Event_End();
}

void YampiDesert_CornerScorpion(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  YampiDesert_ClearRaisedEvent();
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveToFixed(px(232), -1, px(328), 1);
  Actor_MoveToAndWait(21, 232, 328);
  FieldCamera_MoveWithinMapBounds(px(232), -1, px(200), 1);
  Actor_MoveToAndWait(21, 232, 280);
  Actor_MoveToAndWaitById(21, 232, 168);
  Task_Remove(YampiDesert_PlayScorpionRumble);
  Actor_ArmCallback(21, FACING_SOUTH, 0);
  Actor_Stop(gMainRoutineWork.currentOwner);
  FieldEvent_Wait(20);
  GameFlag_Set(0x88b);
  Event_End();
}

void Actor_StartScript(struct Actor *object, const void *script);

// Spawns an object of kind 285 at each of the fifteen cells of
// gYampiDesert_SandCloudCells, twice, each moved by -2 to +1 pixels at random.
void YampiDesert_RaiseSandCloud(void) {
  u32 pass;
  u32 i;

  for (pass = 0; pass <= 1; pass++) {
    u16 *cell;

    cell = (u16 *)gYampiDesert_SandCloudCells;
    for (i = 0; i <= 14; i++) {
      s32 x = *cell++ + ((Random16() << 2) >> 16);
      s32 z = *cell++ + ((Random16() << 2) >> 16);
      struct Actor *object =
          Import_ActorCreate(285, (x - 2) << 16, 0, (z - 2) << 16);

      if (object != NULL) {
        object->speedLimit = px(9) / 8;
        object->acceleration = px(1) / 2;
        Actor_SetSpriteFlags((struct Actor *)object, 0);
        Actor_SelectAnimation((struct Actor *)object, 1);
        Actor_StartScript(object, gYampiDesert_SandCloudDrifts);
      }
    }
  }
  MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
}

#define OBJ(p) ((struct Actor *)(p))

// The trapped scorpion uses Scoop, the leader falls with it, and the King
// Scorpion's battle begins.
void YampiDesert_TrapKingScorpion(void) {
  struct Actor *leader = OBJ(Actor_GetById(gMainRoutineWork.currentOwner));
  struct Actor *partner = OBJ(Actor_GetById(21));
  u32 i;
  s32 angle;

  if (!GameFlag_Get(0x88b))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
loop_1:
  OBJ(gYampiDesert_SandRipple)->scaleX += -0x147a;
  OBJ(gYampiDesert_SandRipple)->scaleY += -0x147a;
  wait(1);
  if (0xcccb < OBJ(gYampiDesert_SandRipple)->scaleX)
    goto loop_1;
loop_2:
  OBJ(gYampiDesert_SandRipple)->scaleX += 0x1999;
  OBJ(gYampiDesert_SandRipple)->scaleY += 0x1999;
  wait(1);
  if (0x1cccc >= OBJ(gYampiDesert_SandRipple)->scaleX)
    goto loop_2;
loop_3:
  OBJ(gYampiDesert_SandRipple)->scaleX += -0xa3d;
  OBJ(gYampiDesert_SandRipple)->scaleY += -0xa3d;
  wait(1);
  if (0x3332 < OBJ(gYampiDesert_SandRipple)->scaleX)
    goto loop_3;
  Actor_SetHorizontalPositionWithTerrain(22, 0, 0);
  FieldEvent_Wait(20);
  FieldCamera_MoveToFixed(px(232), -1, px(196), 1);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                      EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_SetSpeedFixed(gMainRoutineWork.currentOwner, 0xcccc, 0x6666);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 232, 164);
  FieldEvent_Wait(20);
  angle = 0x4000;
  Scene_Call4098(21, 232 << 16, 184 << 16, angle);
  wait(1);
  YampiDesert_BurstSand(232 << 16, 0, 184 << 16);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  partner->velocityY = px(6);
  partner->priorityFlags &= ~2;
  Actor_SetSpritePriority(21, 2);
  Actor_SetSpriteColor(Actor_GetById(21), 0);
  Actor_SetSpriteFlags(Actor_GetById(21), 1);
  Actor_SetSpeedFixed(21, 0x19999, 0xcccc);
  Actor_MoveToAndWaitById(21, 216, 184);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, angle, 0);
  Actor_FaceDirectionById(21, FACING_EAST);
  FieldEvent_Wait(20);
  YampiDesert_ScorpionUsesScoop();
  Sound_Play(SE_WHOOSH);
  YampiDesert_RaiseSandCloud();
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  FieldEvent_Wait(20);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 27);
  FieldEvent_Wait(30);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(10);
  Sound_Play(SE_DUST_PUFF);
  i = 0;
step_13:
  leader->scaleX += -0x400;
  leader->scaleY += -0x400;
  leader->y += -px(2);
  wait(1);
  i++;
  if (i <= 13)
    goto step_13;
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Sound_Play(SE_HIGH_JUMP);
  partner->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(21), 0);
  Actor_MoveToAndWaitById(21, 232, 168);
  FieldEvent_Wait(3);
  Sound_Play(SE_DUST_PUFF);
  i = 0;
step_5:
  partner->scaleX += -0x800;
  partner->scaleY += -0x800;
  partner->y += -px(7);
  wait(1);
  i++;
  if (i <= 5)
    goto step_5;
  Actor_SetHorizontalPositionWithTerrain(21, 0, 0);
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

// Scoop on the sand pit between the four rocks (flag 0x8ed).
void YampiDesert_ScoopCenterPit(s32 mode) {
  struct Actor *object;
  s32 height;

  if (mode == 1) {
    object = Actor_GetById(9);
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPositionFixed(9, px(232), px(200));
    height = Map_GetTerrainHeight(0, object->x, object->z);
    object->y = height;
    object->terrainHeight = height;
    GameFlag_Set(0x8ed);
  }
}

s32 YampiDesert_CellCenter(s32 cell) { return cell * 16 + 8; }

// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    s32 x_ = YampiDesert_CellCenter(column);                                   \
    s32 z_ = YampiDesert_CellCenter(row);                                      \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,      \
                                           x_ << 16, z_ << 16);                \
  }

void YampiDesert_RevealRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(64);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (object != NULL)
    Actor_SetPositionFixed(64, 154 << 18, 222 << 17);
  MapGrid_CopyCellPictureIndicesRect(82, 82, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 82, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 82, 36, 25, 5, 5);
  MapGrid_CopyCellAttributes(88, 82, 5, 5, 36, 25);
  if (column == 15) {
    if (row == 41)
      MOVE_LEADER_TO(14, 41)
    else if (row == 43)
      MOVE_LEADER_TO(16, 43)
  } else if (column == 16) {
    if (row == 40)
      MOVE_LEADER_TO(16, 39)
    else if (row == 41)
      MOVE_LEADER_TO(16, 42)
  } else if (column == 18) {
    if (row == 40)
      MOVE_LEADER_TO(18, 41)
    else if (row == 43)
      MOVE_LEADER_TO(17, 43)
  }
}

// Moves the leader to the centre of a cell, in pixels.
#define MOVE_LEADER_TO(column, row)                                            \
  {                                                                            \
    struct MainRoutineWork *work = &gMainRoutineWork;                          \
    s32 x = YampiDesert_CellCenter(column);                                    \
    s32 z = YampiDesert_CellCenter(row);                                       \
    Actor_SetHorizontalPositionWithTerrain(work->currentOwner, x << 16,        \
                                           z << 16);                           \
  }

void YampiDesert_HideRockHint(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 column = leader->x >> 20;
  s32 row = leader->z >> 20;

  if (Actor_GetById(64) != NULL)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  MapGrid_CopyCellPictureIndicesRect(82, 89, 15, 40, 5, 6);
  MapGrid_CopyCellAttributes(82, 89, 5, 6, 15, 40);
  MapGrid_CopyCellPictureIndicesRect(88, 89, 36, 25, 5, 5);
  MapGrid_CopyCellAttributes(88, 89, 5, 5, 36, 25);
  if (column == 15 && row == 44)
    MOVE_LEADER_TO(15, 43)
  else if (column == 17 && row == 44)
    MOVE_LEADER_TO(17, 43)
  else if (column == 19) {
    if (row == 40)
      MOVE_LEADER_TO(19, 41)
    else if (row == 42)
      MOVE_LEADER_TO(18, 42)
  }
}

void YampiDesert_RevealMedalSpot(void) {
  struct Actor *object = Actor_GetById(64);
  struct Actor *leader;

  if (object != 0) {
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    Actor_SetPositionFixed(64, px(288), px(240));
    if ((leader->x + px(8)) >> 20 == (object->x + px(8)) >> 20 &&
        (leader->z + px(8)) >> 20 == (object->z + px(8)) >> 20)
      Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner,
                                             px(288), px(256));
  }
}

void YampiDesert_HideMedalSpot(void) {
  if (Actor_GetById(64))
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
}

void YampiDesert_OpenLadderPit(void) {
  MapGrid_CopyCellPictureIndicesRect(64, 64, 72, 32, 1, 1);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 32);
  Map_CopyCellAttributeRect(64, 64, 1, 1, 8, 96);
}

// Scoop on the sand pit that hides the ladder (flag 0x88d).
void YampiDesert_ScoopLadderPit(s32 mode) {
  if (mode == 1) {
    YampiDesert_OpenLadderPit();
    GameFlag_Set(0x88d);
  }
}

void YampiDesert_SetSlideActorsShown(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_YampiDesert8)
    YampiDesert_SetActorsHidden(show, 11, 2);
  else if (scene == (s32)&SceneId_YampiDesert12)
    YampiDesert_SetActorsHidden(show, 9, 7);
}

void YampiDesert_ShowSlideActors(void) {
  YampiDesert_SetSlideActorsShown(1);
  GameFlag_Set(FLAG_SCENE_TEMP_0D);
}

void YampiDesert_HideSlideActors(void) {
  YampiDesert_SetSlideActorsShown(0);
  GameFlag_Clear(FLAG_SCENE_TEMP_0D);
}

#include "shared/actors_set_hidden.h"

/* VIEW(yampi_desert): routine gYampiDesert_FinderRunsAhead as this part
 * declared it; the joined file has another; tried as the definition's extern
 * LATE_DATA u32 gYampiDesert_FinderRunsAhead[], it does not compile in tla-ja
 * (yampi_desert.c): games/tla/src/scene/yampi_desert.c:1069:
 * `gYampiDesert_FinderRunsAhead' undeclared (first use in this function). */
extern u8 gYampiDesert_FinderRunsAhead__yampi_desert[] __asm__(
    "gYampiDesert_FinderRunsAhead");
#define gYampiDesert_FinderRunsAhead gYampiDesert_FinderRunsAhead__yampi_desert
/* VIEW(yampi_desert): routine gYampiDesert_TravelersFollow as this part
 * declared it; the joined file has another; tried as the definition's extern
 * LATE_DATA u32 gYampiDesert_TravelersFollow[], it does not compile in tla-ja
 * (yampi_desert.c): games/tla/src/scene/yampi_desert.c:1116:
 * `gYampiDesert_TravelersFollow' undeclared (first use in this function). */
extern u8 gYampiDesert_TravelersFollow__yampi_desert[] __asm__(
    "gYampiDesert_TravelersFollow");
#define gYampiDesert_TravelersFollow gYampiDesert_TravelersFollow__yampi_desert
extern u32 gYampiDesert_LastTravelersFollow[];

void YampiDesert_SeeTravelersAbove(s32 p0) {
  s32 *owner;
  s32 angle;

  Call1((void (*)())Event_Start, p0);
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgHeyLookUp);
  Actor_Stop(9);
  wait(1);
  Actor_ShowEmote(9, 0x100, 0);
  Actor_FaceDirection(9, 0, 20);
  Event_ShowMessageAndWait(9, 0);
  Actor_Stop(8);
  wait(1);
  Actor_FaceDirection(8, 0, 0);
  Actor_FaceDirection(10, 0, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_FaceDirection(14, 0xd000, 0);
  owner = (s32 *)&gMainRoutineWork;
  owner += 133;
  FieldActor_ShowEmote(*owner, 0x100, 0);
  Actor_FaceDirection(*owner, 0x8000, 0);
  ((struct Actor *)FieldCamera_GetActor())->motionFlags = 0;
  FieldCamera_SetSpeedFixed(0x19999, 0x3333);
  FieldCamera_MoveToFixed(0xd40000, 0, 0xd40000, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(12, 0);
  Actor_FaceDirection(11, 0, 10);
  Event_ShowMessageAndWait(11, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(14, 0x5000, 20);
  Actor_Jump(14, 4, 40);
  Motion_SetModeAndWaitAnimation(13, 3);
  Motion_SetModeAndWaitAnimation(14, 3);
  Actor_WatchAndSetScript(8, 14);
  Actor_WatchAndSetScript(9, 14);
  Actor_WatchAndSetScript(10, 14);
  Actor_WatchAndSetScript(11, 14);
  Actor_WatchAndSetScript(12, 14);
  Actor_WatchAndSetScript(13, 14);
  Actor_SetSpeedFixed(13, 0x10000, 0x8000);
  Actor_SetMovementTypeById(14, (s32)gYampiDesert_FinderRunsAhead);
  FieldCamera_SetSpeedFixed(0xcccc, 0x1999);
  FieldCamera_MoveToFixed(0xf00000, -1, 0x1040000, 1);
  FieldCamera_WaitForArrival();
  Actor_Stop(8);
  Actor_Stop(9);
  Actor_Stop(10);
  Actor_Stop(11);
  Actor_Stop(12);
  Actor_Stop(13);
  FieldCamera_SetSpeedFixed(0x26666, 0x4ccc);
  FieldCamera_MoveToFixed(0xd40000, -1, 0xd40000, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(20);
  Actor_FaceDirection(8, 0xd000, 0);
  Actor_FaceDirection(9, 0x5000, 20);
  Event_ShowMessageAndWait(9, 0);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_ShowMessageAndWait(8, 0);
  Actor_Jump(10, 4, 0);
  Actor_FaceDirection(10, 0, 0);
  Actor_SetModeById(10, 4);
  Event_ShowMessageAndWait(10, 0);
  Actor_FaceDirection(8, 0x8000, 0);
  Actor_FaceDirection(9, 0x8000, 0);
  Actor_ShowEmote(8, 0x102, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
  Event_ShowMessageAndWait(14, 0);
  Actor_FaceDirection(8, 0xd000, 0);
  Actor_FaceDirection(10, 0x3000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(12, 0, 0);
  Actor_FaceDirection(13, 0x8000, 20);
  Actor_SetModeById(8, 3);
  Actor_SetModeById(9, 3);
  Actor_SetModeById(10, 3);
  Actor_SetModeById(11, 3);
  Actor_SetModeById(12, 3);
  Motion_SetModeAndWaitAnimation(13, 3);
  Actor_SetSpeedFixed(8, 0x19999, 0xcccc);
  Actor_SetSpeedFixed(9, 0x19999, 0xcccc);
  Actor_SetSpeedFixed(10, 0x19999, 0xcccc);
  Actor_SetSpeedFixed(11, 0x19999, 0xcccc);
  Actor_SetSpeedFixed(12, 0x19999, 0xcccc);
  Actor_SetSpeedFixed(13, 0x19999, 0xcccc);
  Actor_WatchAndSetScript(*owner, 13);
  {
    s32 path = (s32)gYampiDesert_TravelersFollow;

    Actor_SetMovementTypeById(13, path);
    FieldEvent_Wait(10);
    Actor_SetMovementTypeById(12, path);
    FieldCamera_SetSpeedFixed(0x6666, 0xccc);
    FieldCamera_MoveToFixed(0xf00000, -1, 0x1180000, 1);
    Actor_SetMovementTypeById(8, path);
    FieldEvent_Wait(10);
    Actor_SetMovementTypeById(9, path);
  }
  {
    s32 path = (s32)gYampiDesert_LastTravelersFollow;

    Actor_SetMovementTypeById(10, path);
    FieldEvent_Wait(20);
    Actor_SetMovementTypeAndWaitById(11, path);
  }
  Actor_Stop(*owner);
  wait(1);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
  Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  angle = 0xb000;
  Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 8,
        0x2080000, 0xf20000, angle);
  {
    s32 facing = 0x8000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 9,
          0x2150000, 0x10f0000, facing);
  }
  {
    s32 facing = 0x3000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 10,
          0x1b30000, 0xe80000, facing);
  }
  {
    s32 facing = 0x5000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 11,
          0x1d30000, 0xf80000, facing);
  }
  {
    s32 facing = 0xd000;

    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 12,
          0x1ce0000, 0x1540000, facing);
    Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 13,
          0x1e20000, 0x1540000, facing);
  }
  Call4((void (*)())Actor_SetHorizontalPositionAndAngleWithTerrain, 14,
        0x1e90000, 0x1400000, angle);
  Actor_SetModeById(8, 1);
  Actor_SetModeById(9, 1);
  Actor_SetModeById(10, 1);
  Actor_SetModeById(11, 1);
  Actor_SetModeById(12, 1);
  Actor_SetModeById(13, 1);
  Actor_SetModeById(14, 1);
  GameFlag_Set(0x8f0);
  GameFlag_Set(0x88f);
  Event_End();
}

// The Scoop Gem (item 209's icon) as an actor, after the King Scorpion's
// battle (flag 0x88c).
struct Actor *YampiDesert_CreateScoopGem(void) {
  struct Actor *object =
      Import_ActorCreate(489, 232 << 16, 32 << 16, 200 << 16);
  u8 *buffer = 0;

  struct YampiDesertVisual *visual;
  s32 mask;

  if (object == 0)
    return 0;
  {
    visual = object->sprite;

    {
      s32 keep = 33;

      keep = -keep;
      visual->flags05 &= keep;
    }
    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;
    object->motionFlags = 0;
    object->unk5c[0] = 1;
    visual->value1a = 0;
    visual->value1b = 0;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(ITEM_SCOOP_GEM);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
  return object;
}

void YampiDesert_DropScoopGem(void) {
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  object = YampiDesert_CreateScoopGem();
  PartyInventory_GiveItem(ITEM_SCOOP_GEM, 0);
  if (object != 0) {
    Actor_SetPositionAndResetMotion((struct Actor *)object, 0, 0, 0);
    wait(1);
    Actor_Release(object);
  }
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  GameFlag_Set(0x88c);
  Event_End();
}

void YampiDesert_RippleFollowsScorpion(void) {
  struct Actor *object;
  s16 scene;

  gFieldWork->raisedTrigger = gYampiDesert_RaisedEvent;
  if (gYampiDesert_SandRipple != 0) {
    scene = gMainRoutineWork.scene;
    if (scene == (s32)&SceneId_YampiDesert4)
      object = Actor_GetById(16);
    else if (scene == (s32)&SceneId_YampiDesert5)
      object = Actor_GetById(21);
    else
      return;
    gYampiDesert_SandRipple->x = object->x;
    gYampiDesert_SandRipple->z = object->z;
    gYampiDesert_SandRipple->facing = object->facing;
  }
}
#undef OBJ
#undef MOVE_LEADER_TO

#define FIELD32(object, offset) (*(s32 *)((u8 *)(object) + (offset)))

struct Actor *Actor_GetById(u32 objectId);
void Actor_StartScript(struct Actor *object, const void *script);
s32 Trig_Sin(s32 angle);

void YampiDesert_SlideLeader(struct Actor *object, s32 dx, s32 dz) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  if (gMainRoutineWork.renderMode != 5) {
    u32 odd = gFrameCount & 1;

    if (odd == 0) {
      struct Actor *dust = (struct Actor *)Value4(
          (s32 (*)())Import_ActorCreate, 285, object->x, object->y, object->z);

      if (dust != NULL) {
        struct DustSprite *sprite;

        dust->motionFlags = odd;
        dust->collisionFlags = odd;
        Actor_SetSpriteFlags(dust, 0);
        Actor_SelectAnimation(dust, 1);
        Actor_StartScript(dust, gYampiDesert_SlideDust);
        sprite = dust->sprite;
        sprite->mode = 1;
        /* FAKEMATCH(dead): an empty once-only block after the sprite store
         * keeps gInput's address load after it. */
        do {
        } while (0);
        if (gInput.held != 0) {
          FIELD32(dust, 68) = Trig_Cos(((Random16() << 3) >> 16) << 14) + dx;
          FIELD32(dust, 76) = Trig_Sin(((Random16() << 3) >> 16) << 14) + dz;
          if (gInput.held & KEY_B) {
            FIELD32(dust, 68) += dx;
            FIELD32(dust, 76) += dz;
          }
        } else {
          FIELD32(dust, 68) =
              (Trig_Cos(((Random16() << 3) >> 16) << 14) >> 1) + dx;
          FIELD32(dust, 76) =
              (Trig_Sin(((Random16() << 3) >> 16) << 14) >> 1) + dz;
        }
        FIELD32(dust, 72) = 0;
        dust->timer = ((Random16() << 9) >> 16) - 256;
        dust->speedLimit = 0;
        dust->acceleration = 0;
        dust->update = Effect_AdvanceMotionAtLeaderPriority;
      }
    }
  }
  if (dx != 0) {
    object->x += dx;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 z = object->z >> 16;

      object->z += ((8 - z % 16) << 16) >> 3;
    }
  } else {
    object->z += dz;
    if (!(gInput.held & KEYS_DPAD)) {
      s32 x = object->x >> 16;

      object->x += ((8 - x % 16) << 16) >> 3;
    }
  }
}

void TilesetAnim_SuspendAll(void);
void TilesetAnim_ResumeAll(void);

void YampiDesert_UpdateSandSlide(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 speed = 0x18000;
  struct MapWork *map;
  struct MapCell *here, *ahead, *under;
  s32 index, column, row;
  struct MapCell *cells;

  if (work->psynergy != 0 || work->fieldMenu != 0 || work->pauseMenu != 0) {
    Actor_SetAnimationSpeed(leader, 0);
    TilesetAnim_SuspendAll();
    return;
  }
  map = Ram_HeapSlots->mapWork;
  index = Actor_GetCellIndex(leader);
  column = leader->x / 0x100000;
  row = (leader->z + px(4)) / 0x100000;
  cells = map->layers[0].cells;
  here = cells + index;
  ahead = cells + (column + row * 128);
  under = map->layers[1].cells + index;
  Actor_SetAnimationSpeed(leader, 16);
  TilesetAnim_ResumeAll();
  if ((u8)(under->collisionCode - 10) <= 4) {
    if (gMainRoutineWork.renderMode == 5)
      speed = 0;
    else
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 1);
  }
  if (ahead->collisionCode == 255)
    speed = 0;
  switch (here->collisionCode) {
  case 10:
    if (gInput.held & KEY_UP) {
      YampiDesert_SlideLeader(leader, 0, -speed >> 2);
      return;
    }
    break;
  case 11:
    if (gInput.held & KEY_DOWN) {
      YampiDesert_SlideLeader(leader, 0, speed >> 2);
      return;
    }
    break;
  case 12:
    if (gInput.held & KEY_LEFT) {
      YampiDesert_SlideLeader(leader, -speed >> 2, 0);
      return;
    }
    break;
  case 13:
    if (gInput.held & KEY_RIGHT) {
      YampiDesert_SlideLeader(leader, speed >> 2, 0);
      return;
    }
    break;
  }
  switch (under->collisionCode) {
  case 10:
    YampiDesert_SlideLeader(leader, 0, -speed);
    break;
  case 11:
    YampiDesert_SlideLeader(leader, 0, speed);
    break;
  case 12:
    YampiDesert_SlideLeader(leader, -speed, 0);
    break;
  case 13:
    YampiDesert_SlideLeader(leader, speed, 0);
    break;
  }
}

void YampiDesert_TrackUpperLevel(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 x;
  s32 y;
  s32 z;

  y = leader->y;
  if (y > 0x100000)
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
  if (gMainRoutineWork.scene == (s32)&SceneId_YampiDesert3) {
    Actor_SetSpritePriority(18,
                            ((struct FieldSprite *)leader->sprite)->priority);
    x = leader->x;
    if (x > 0x1d80000) {
      z = leader->z;
      if (z <= 0x1e80000)
        return;
      if (leader->y > (s32)0xfff00000)
        gMainRoutineWork.unk2c3 = 1;
      else
        gMainRoutineWork.unk2c3 = 0;
    }
  }
}

void YampiDesert_TrackUpperLevelFromBelow(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);

  if (leader->y > -px(16))
    gMainRoutineWork.unk2c3 = 1;
  else
    gMainRoutineWork.unk2c3 = 0;
}

void YampiDesert_TrackUpperLevelOnRamp(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index = Actor_GetCellIndex(leader);
  struct MapCell *cell = map->layers[0].cells + index;
  struct MainRoutineWork *party;
  s32 y;

  if (cell->collisionCode == 20) {
    y = leader->y;
    if (y > 0x100000) {
      gMainRoutineWork.unk2c3 = 1;
      return;
    }
  }
  party = &gMainRoutineWork;
  party->unk2c3 = 0;
}

void YampiDesert_TrackLeaderInPitArea(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 x = leader->x >> 20;
  u32 z = leader->z >> 20;

  if (x > 12 && z > 10 && x <= 15 && z <= 13)
    GameFlag_Set(FLAG_MENUS_RAISE_TRIGGER);
  else
    GameFlag_Clear(FLAG_MENUS_RAISE_TRIGGER);
}

void YampiDesert_PlayScorpionRumble(void) {
  if (gFieldWork->psynergy == 0 && gFrameCount % 5 == 0) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->renderMode != 5 || gInput.held == 0)
      Sound_Play(SE_596);
  }
}

void YampiDesert_ClearRaisedEvent(void) {
  gFieldWork->raisedTrigger = 0;
  gYampiDesert_RaisedEvent = 0;
}

s32 YampiDesert_HasArrived(struct Actor *object) {
  s32 x = object->targetX;

  if (x == TARGET_UNSET && object->targetZ == x)
    return 1;
  return 0;
}

void YampiDesert_ScorpionRoamsRoom4(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
  case 0:
    FieldActor_SetMoveTargetFixed((s32)object, 140 << 17, 0, 200 << 16);
    *state = 1;
    break;
  case 1:
    if (YampiDesert_HasArrived(object))
      *state = 2;
    break;
  case 2:
    FieldActor_SetMoveTargetFixed((s32)object, 140 << 17, 0, 132 << 17);
    *state = 3;
    break;
  case 3:
    if (YampiDesert_HasArrived(object))
      *state = 4;
    break;
  case 4:
    FieldActor_SetMoveTargetFixed((s32)object, 184 << 16, 0, 132 << 17);
    *state = 5;
    break;
  case 5:
    if (YampiDesert_HasArrived(object))
      *state = 6;
    break;
  case 6:
    FieldActor_SetMoveTargetFixed((s32)object, 184 << 16, 0, 248 << 16);
    *state = 7;
    break;
  case 7:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_03)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = 200;
      } else {
        *state = 8;
      }
    }
    break;
  case 8:
    FieldActor_SetMoveTargetFixed((s32)object, 184 << 16, 0, 200 << 16);
    *state = 9;
    break;
  case 9:
    if (YampiDesert_HasArrived(object))
      *state = 0;
    break;
  }
}

// Readies actors 16 and 17 and the room's callbacks; actor 17 becomes the
// followed actor (gYampiDesert_SandRipple).
void YampiDesert_SetUpRoom4Scorpion(void) {
  struct Actor *leader = Actor_GetById(16);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(YampiDesert_PlayScorpionRumble, 0x480);
  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(17);
  object->priorityFlags |= 2;
  *followed = (struct Actor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Actor_DisableById(17);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(17, 3);
  Actor_SetSpeedFixed(16, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(16, 3);
  Actor_DisableById(16);
  Actor_GetById(16)->motionFlags = 3;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    *((u8 *)leader + 98) = 0;
    leader->update = YampiDesert_ScorpionRoamsRoom4;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

// Sends the object toward a point and moves to the next state.
#define WALK(state_, x, z)                                                     \
  case state_:                                                                 \
    FieldActor_SetMoveTargetFixed((s32)object, x, 0, z);                       \
    *state = state_ + 1;                                                       \
    break;

// Waits for the object to arrive before moving to the given state.
#define ARRIVE(state_, next)                                                   \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(object))                                        \
      *state = next;                                                           \
    break;

void YampiDesert_ScorpionRoamsRoom5(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 166 << 18)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 182 << 18)
    ARRIVE(3, 4)
    WALK(4, 246 << 16, 182 << 18)
  case 5:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = 201;
      } else {
        *state = 6;
      }
    }
    break;
    WALK(6, 184 << 16, 182 << 18)
    ARRIVE(7, 8)
    WALK(8, 184 << 16, 178 << 18)
    ARRIVE(9, 10)
    WALK(10, 140 << 17, 178 << 18)
    ARRIVE(11, 12)
    WALK(12, 140 << 17, 190 << 18)
    ARRIVE(13, 14)
    WALK(14, 232 << 16, 190 << 18)
    ARRIVE(15, 16)
    WALK(16, 232 << 16, 186 << 18)
  case 17:
    if (YampiDesert_HasArrived(object)) {
      if (!GameFlag_Get(FLAG_SCENE_TEMP_00))
        *state = 18;
      else
        *state = 32;
    }
    break;
    WALK(18, 232 << 16, 166 << 18)
    ARRIVE(19, 0)
  case 32:
    YampiDesert_ClearRaisedEvent();
    FieldActor_SetMoveTargetFixed((s32)object, 148 << 17, 0, 186 << 18);
    *state = 33;
    break;
    ARRIVE(33, 34)
    WALK(34, 148 << 17, 190 << 18)
    ARRIVE(35, 36)
    WALK(36, 232 << 16, 190 << 18)
    ARRIVE(37, 38)
    WALK(38, 232 << 16, 186 << 18)
    ARRIVE(39, 32)
  }
}

// YampiDesert_SetUpRoom4Scorpion's counterpart for actors 21 and 22: actor 22
// becomes the followed actor, and until flag 0x109 actor 21 is placed and
// given the update YampiDesert_ScorpionRoamsRoom5.
void YampiDesert_SetUpRoom5Scorpion(void) {
  struct Actor *leader = Actor_GetById(21);
  struct Actor *object;
  struct Actor **followed;

  Scene_TaskAdd(YampiDesert_PlayScorpionRumble, 0x480);
  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(22);
  object->priorityFlags |= 2;
  *followed = (struct Actor *)object;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Actor_DisableById(22);
  (*followed)->motionFlags = 3;
  Actor_SetSpritePriority(22, 3);
  Actor_SetSpeedFixed(21, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Actor_DisableById(21);
  Actor_GetById(21)->motionFlags = 3;
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    Scene_Call4098(21, 232 << 16, 664 << 16, 0);
    *((u8 *)leader + 98) = 0;
    leader->update = YampiDesert_ScorpionRoamsRoom5;
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

// Sends the object toward a point and moves to the next state.

// As WALK, after YampiDesert_ClearRaisedEvent.
#define WALK_ON(state_, x, z)                                                  \
  case state_:                                                                 \
    YampiDesert_ClearRaisedEvent();                                            \
    FieldActor_SetMoveTargetFixed((s32)object, x, 0, z);                       \
    *state = state_ + 1;                                                       \
    break;

// Waits for the object to arrive before moving to the given state.

// Waits for the object to arrive, then turns off by a flag.
#define ARRIVE_OR(state_, flag, turn, next)                                    \
  case state_:                                                                 \
    if (YampiDesert_HasArrived(object)) {                                      \
      if (GameFlag_Get(flag))                                                  \
        *state = turn;                                                         \
      else                                                                     \
        *state = next;                                                         \
    }                                                                          \
    break;

void YampiDesert_ScorpionRoamsToTrap(struct Actor *object) {
  u8 *state = (u8 *)object + 98;

  switch (*state) {
    WALK(0, 156 << 17, 132 << 17)
    ARRIVE(1, 2)
    WALK(2, 156 << 17, 172 << 17)
    ARRIVE(3, 4)
    WALK(4, 148 << 17, 172 << 17)
    ARRIVE_OR(5, 0x202, 32, 6)
    WALK(6, 168 << 16, 172 << 17)
    ARRIVE(7, 8)
    WALK(8, 168 << 16, 148 << 17)
    ARRIVE(9, 10)
    WALK(10, 184 << 16, 148 << 17)
    ARRIVE_OR(11, 0x201, 48, 12)
    WALK(12, 140 << 17, 148 << 17)
    ARRIVE(13, 14)
    WALK(14, 140 << 17, 164 << 17)
  case 15:
    if (YampiDesert_HasArrived(object)) {
      if (GameFlag_Get(FLAG_SCENE_TEMP_02)) {
        object->update = NULL;
        gYampiDesert_RaisedEvent = 202;
      } else {
        *state = 16;
      }
    }
    break;
    WALK(16, 140 << 17, 180 << 17)
    ARRIVE(17, 18)
    WALK(18, 200 << 16, 180 << 17)
    ARRIVE(19, 20)
    WALK(20, 200 << 16, 156 << 17)
    ARRIVE_OR(21, 0x201, 34, 22)
    WALK(22, 200 << 16, 132 << 17)
    ARRIVE(23, 0)
    WALK_ON(32, 148 << 17, 156 << 17)
    ARRIVE(33, 34)
    WALK_ON(34, 164 << 17, 156 << 17)
    ARRIVE(35, 36)
    WALK(36, 164 << 17, 188 << 17)
    ARRIVE(37, 38)
    WALK(38, 148 << 17, 188 << 17)
    ARRIVE(39, 32)
    WALK_ON(48, 184 << 16, 172 << 17)
    ARRIVE(49, 50)
    WALK(50, 168 << 16, 172 << 17)
    ARRIVE(51, 52)
    WALK(52, 168 << 16, 148 << 17)
    ARRIVE(53, 54)
    WALK(54, 184 << 16, 148 << 17)
    ARRIVE(55, 48)
  }
}

// YampiDesert_SetUpRoom4Scorpion's counterpart for actors 21 and 22 in Room 5:
// after flag 0x88b actor 21 is placed facing 0x4000; otherwise, until flag
// 0x109, it is placed and given the update YampiDesert_ScorpionRoamsToTrap.
void YampiDesert_SetUpTrappedScorpion(void) {
  struct Actor *leader = Actor_GetById(21);
  struct Actor *object;
  struct Actor **followed;
  s32 angle;

  followed = &gYampiDesert_SandRipple;
  object = Actor_GetById(22);
  object->priorityFlags |= 2;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  *followed = (struct Actor *)object;
  Actor_DisableById(22);
  Actor_SetSpritePriority(22, 3);
  (*followed)->motionFlags = 3;
  Actor_SetSpeedFixed(21, 0x19999, 0xcccc);
  Actor_SetSpriteColor((struct Actor *)leader, 15);
  Actor_SetSpritePriority(21, 3);
  Actor_DisableById(21);
  Actor_GetById(21)->motionFlags = 3;
  if (GameFlag_Get(0x88b)) {
    angle = 0x4000;
    Scene_Call4098(21, 232 << 16, 168 << 16, angle);
  } else {
    Scene_TaskAdd(YampiDesert_PlayScorpionRumble, 0x480);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Scene_Call4098(21, 200 << 16, 264 << 16, 0);
      *((u8 *)leader + 98) = 0;
      leader->update = YampiDesert_ScorpionRoamsToTrap;
    }
  }
  YampiDesert_ClearRaisedEvent();
  Task_Add(YampiDesert_RippleFollowsScorpion, TASK_PASS_VBLANK);
}

void YampiDesert_OpenScoopHole(void) {
  if (*(s16 *)((u8 *)Ram_HeapSlots->effectWork + 0x7c4) != 0)
    MapGrid_CopyCellPictureIndicesRect(73, 76, 78, 11, 1, 1);
}

void YampiDesert_ScorpionUsesScoop(void) {
  BattleEffect_LoadActionEffectResources(136, 1);
  Scene_TaskAdd(YampiDesert_OpenScoopHole, 0x480);
  FieldPsynergy_SetupActorPair(21, -1);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(YampiDesert_OpenScoopHole);
  BattleEffect_CleanupSceneObjects();
}

#include "shared/actor_hide.h"

#include "shared/scene_run_unless_flag.h"

void YampiDesert_SetUpRoom8SlideActors(void) {
  Actor_Hide(11);
  Actor_Hide(12);
  Scene_RunUnlessFlag();
}

#include "shared/actors_for_range.h"

void YampiDesert_ClimbOut(struct Actor *object, s32 x, s32 z, s32 mode) {
  struct MainRoutineWork *party;
  s32 zero;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  flags = &object->motionFlags;
  zero = 0;
  party = &gMainRoutineWork;
  *flags = zero;
  Actor_SetSpeed(party->currentOwner, 1);
  Actor_WalkToAndWaitById(party->currentOwner, x, z);
  Actor_FaceDirectionById(party->currentOwner, FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(party->currentOwner), 0);
  Actor_SetModeById(party->currentOwner, 13);
  Actor_SetMoveTarget(object, object->x, object->y - px(10), object->z);
  Actor_WaitForMove(object);
  Actor_SetModeById(party->currentOwner, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(mode);
}

void YampiDesert_ClimbRevealedLadder(void) {
  struct Actor *object;
  s32 x;
  s32 z;

  if (GameFlag_Get(0x88d)) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    x = object->x >> 20;
    z = object->z >> 20;
    if (x == 8) {
      if (z == 31 && !(gInput.held & KEY_DOWN))
        return;
      if (z == 33 && !(gInput.held & KEY_UP))
        return;
    } else {
      if (z != 32)
        return;
      if (x == 7 && !(gInput.held & KEY_RIGHT))
        return;
      if (x == 9 && !(gInput.held & KEY_LEFT))
        return;
    }
    YampiDesert_ClimbOut(object, 136, 516, 5);
  }
}

void YampiDesert_ClimbOutOfRoom11(void) {
  YampiDesert_ClimbOut(Actor_GetById(gMainRoutineWork.currentOwner), 408, 432,
                       2);
}

void Event_SetAfterBattleDestination(s32 scene, s32 entrance);
void YampiDesert_BurstSand(s32 x, s32 y, s32 z);

void YampiDesert_LandAndFightKingScorpion(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;
  struct Actor *leader = Actor_GetById(party->currentOwner);
  struct Actor *actor8 = Actor_GetById(8);
  /* FAKEMATCH(wrapper): the camera, walk and speed calls forwarded through
   * call.h's Call3 and Call4. */

  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SOUND_FADE_OUT_BGM);
  FieldCamera_MoveToFixed(-1, -1, -1, 0);
  FieldCamera_MoveToFixed(px(232), -1, px(184), 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  Actor_SetSpriteFlags(Actor_GetById(party->currentOwner), 0);
  leader->motionFlags = 0;
  leader->y = px(130);
  leader->gravity = Q16_ONE / 2;
  leader->bounce = 0;
  Actor_SetSpriteFlags(Actor_GetById(8), 0);
  {
    // The angle is held in a local; as a literal argument, its constant is
    // built last.
    s32 angle = TRIG_TURN / 4;

    Call4(Actor_SetHorizontalPositionAndAngleWithTerrain, 8, px(232), px(148),
          angle);
  }
  actor8->motionFlags = 0;
  actor8->y = px(130);
  actor8->gravity = Q16_ONE / 2;
  actor8->bounce = 0;
  wait(1);
  gFieldWork->transition = 0x203;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = 3;
  Actor_SetSpriteFlags(Actor_GetById(party->currentOwner), 1);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(leader->x, leader->y, leader->z);
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(party->currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Motion_SetModeAndWaitAnimation(party->currentOwner, 49);
  Sound_Play(SE_DUST_PUFF);
  actor8->motionFlags = 3;
  Actor_SetSpriteFlags(Actor_GetById(8), 1);
  FieldEvent_Wait(24);
  YampiDesert_BurstSand(actor8->x, actor8->y, actor8->z);
  Sound_Play(SE_THUMP);
  Actor_ArmCallback(party->currentOwner, TRIG_TURN * 3 / 4, 0);
  FieldEvent_Wait(40);
  Sound_Play(SE_BEAST_CRY);
  Actor_SetModeById(8, 3);
  FieldEvent_Wait(20);
  actor8->velocityY = Q16_ONE * 8;
  actor8->gravity = 0xe666;
  Actor_SetSpeedFixed(8, 0x19999, 0xcccc);
  Actor_MoveToAndWaitById(8, 232, 184);
  party->battleRule24b = 2;
  Event_SetAfterBattleDestination((s32)&SceneId_YampiDesert10, 2);
  Field_RequestBattleFromEncounterTable(10, 0);
}

// Clears the scorpion's sand ripple and runs the entered room's setup; Room7
// has none.
s32 Scene_Initialize(void) {
  s16 scene;

  YampiDesert_ClearRaisedEvent();
  gYampiDesert_SandRipple = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_YampiDesert1)
    YampiDesert_SetUpRoom1();
  else if (scene == (s32)&SceneId_YampiDesert2)
    YampiDesert_SetUpRoom2();
  else if (scene == (s32)&SceneId_YampiDesert3)
    YampiDesert_SetUpRoom3();
  else if (scene == (s32)&SceneId_YampiDesert4)
    YampiDesert_SetUpRoom4();
  else if (scene == (s32)&SceneId_YampiDesert5)
    YampiDesert_SetUpRoom5();
  else if (scene == (s32)&SceneId_YampiDesert6)
    YampiDesert_SetUpRoom6();
  else if (scene == (s32)&SceneId_YampiDesert8)
    YampiDesert_SetUpRoom8();
  else if (scene == (s32)&SceneId_YampiDesert9)
    YampiDesert_SetUpRoom9();
  else if (scene == (s32)&SceneId_YampiDesert10)
    YampiDesert_SetUpRoom10();
  else if (scene == (s32)&SceneId_YampiDesert11)
    YampiDesert_SetUpRoom11();
  else if (scene == (s32)&SceneId_YampiDesert12)
    YampiDesert_SetUpRoom12();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void YampiDesert_SetUpRoom1(void) {
  YampiDesert_PlaceStakes(gYampiDesert_Room1Stakes);
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.entrance == 1) {
    GameFlag_Set(0xa29);
    GameFlag_Clear(0xa2a);
    GameFlag_Clear(0xa2b);
  }
}

// Clears actors 8 to 14 after flag 0x8b3, or places them after flag 0x8f0.
void YampiDesert_SetUpRoom2(void) {
  s32 angleA;
  s32 angleB;
  s32 angle;

  if (GameFlag_Get(FLAG_ALHAFRA_MADRANS_ARRIVED)) {
    Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(13, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(14, 0, 0);
  } else if (GameFlag_Get(0x8f0)) {
    angleA = 0xb000;
    Scene_Call4098(8, 520 << 16, 242 << 16, angleA);
    angle = 0x8000;
    Scene_Call4098(9, 533 << 16, 271 << 16, angle);
    angle = 0x3000;
    Scene_Call4098(10, 435 << 16, 232 << 16, angle);
    angle = 0x5000;
    Scene_Call4098(11, 467 << 16, 248 << 16, angle);
    angleB = 0xd000;
    Scene_Call4098(12, 462 << 16, 340 << 16, angleB);
    Scene_Call4098(13, 482 << 16, 340 << 16, angleB);
    Scene_Call4098(14, 489 << 16, 320 << 16, angleA);
  }
  if (GameFlag_Get(0x88d))
    YampiDesert_OpenLadderPit();
  Task_Add(YampiDesert_TrackUpperLevel, TASK_PASS_VBLANK);
}

void YampiDesert_MatchStakePriority(void) {
  s32 variant =
      ((struct FieldSprite *)Actor_GetById(gMainRoutineWork.currentOwner)
           ->sprite)
          ->priority;

  Actor_SetSpritePriority(13, variant);
  Actor_SetSpritePriority(14, variant);
}

void YampiDesert_SetUpRoom3(void) {
  YampiDesert_PlaceStakes(gYampiDesert_Room3Stakes);
  if (GameFlag_Get(FLAG_SCENE_TEMP_0B)) {
    YampiDesert_StepOntoStakeTops();
    wait(10);
  }
  Scene_TaskAdd(YampiDesert_TrackUpperLevel, 0x480);
  Scene_TaskAdd(YampiDesert_MatchStakePriority, 0x480);
}

void YampiDesert_ReadyStake(s32 id) {
  Actor_GetById(id)->priorityFlags |= 2;
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
  Actor_SetModeById(id, 2);
}

void YampiDesert_SetUpRoom4(void) {
  FieldEffect_StartBlankOverlay(0);
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4) {
    if (Scene_FlagTest(0x889))
      Actor_SetHorizontalPositionWithTerrain(16, 0, 0);
    else
      YampiDesert_SetUpRoom4Scorpion();
  }
  YampiDesert_PlaceStakes(gYampiDesert_Room4Stakes);
  if (Scene_FlagTest(0x20b)) {
    YampiDesert_StepOntoStakeTops();
    wait(10);
  }
  YampiDesert_ReadyStake(12);
  YampiDesert_ReadyStake(13);
  YampiDesert_ReadyStake(14);
  YampiDesert_ReadyStake(15);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    Task_Add(YampiDesert_TrackUpperLevelFromBelow, TASK_PASS_VBLANK);
}

// Room 5's setup: from entrances other than 1 and 2 it places actors 11 to
// 20 and copies the map cells flags 0x88c and 0x8ed select.
void YampiDesert_SetUpRoom5(void) {
  FieldEffect_StartBlankOverlay(0);
  YampiDesert_PlaceStakes(gYampiDesert_Room5Stakes);
  YampiDesert_ReadyStake(11);
  YampiDesert_ReadyStake(12);
  YampiDesert_ReadyStake(13);
  YampiDesert_ReadyStake(14);
  YampiDesert_ReadyStake(15);
  YampiDesert_ReadyStake(16);
  YampiDesert_ReadyStake(17);
  YampiDesert_ReadyStake(18);
  YampiDesert_ReadyStake(19);
  YampiDesert_ReadyStake(20);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    if (GameFlag_Get(0x889) && !GameFlag_Get(0x88a))
      YampiDesert_SetUpRoom5Scorpion();
  } else {
    Actor_SetPositionFixed(11, 200 << 16, 248 << 16);
    Actor_SetPositionFixed(12, 168 << 16, 280 << 16);
    Actor_SetPositionFixed(13, 152 << 16, 344 << 16);
    Actor_SetPositionFixed(14, 184 << 16, 360 << 16);
    Actor_SetPositionFixed(15, 328 << 16, 264 << 16);
    Actor_SetPositionFixed(16, 296 << 16, 296 << 16);
    Actor_SetPositionFixed(17, 344 << 16, 312 << 16);
    Actor_SetPositionFixed(18, 312 << 16, 360 << 16);
    Actor_SetPositionFixed(19, 280 << 16, 376 << 16);
    Actor_SetHorizontalPositionWithTerrain(20, 328 << 16, 392 << 16);
    if (GameFlag_Get(0x88c)) {
      if (GameFlag_Get(0x8ed)) {
        MapGrid_CopyCellPictureIndicesRect(68, 77, 13, 9, 3, 3);
        MapGrid_CopyCellAttributes(68, 77, 3, 3, 13, 9);
      } else {
        MapGrid_CopyCellPictureIndicesRect(74, 71, 77, 9, 3, 3);
        MapGrid_CopyCellAttributes(74, 71, 3, 3, 13, 9);
      }
      MapGrid_CopyCellAttributes(63, 77, 3, 3, 13, 73);
    } else if (GameFlag_Get(0x88a)) {
      YampiDesert_SetUpTrappedScorpion();
    }
  }
}

// A halfword of gMainRoutineWork that has no field name yet.
#define MAIN_ROUTINE_WORK_U16(offset)                                          \
  (*(u16 *)((u8 *)&gMainRoutineWork + (offset)))

void YampiDesert_SetUpRoom6(void) {
  FieldEffect_StartBlankOverlay(0);
  if (GameFlag_Get(FLAG_AREA_TEMP_02) == 0)
    Actor_SetWideSprite(64, 0);
}

static inline void Scene_StoreU16(u16 *field, s32 value) {
  /* FAKEMATCH(wrapper): through the inline the value is a word constant built
   * after the field's address. */
  *field = value;
}

// Room 8's setup. Entered at entrance 4 (not in the Japanese edition), it
// sets gMainRoutineWork's pair at 0x260 to the room whose flag is set.
void YampiDesert_SetUpRoom8(void) {
  struct Actor *object = Actor_GetById(64);
  s32 scene;

  Scene_QueueMusic(SE_137);
  Rope_Initialize();
  Scene_Call41d8(0, 524, 8, 9);
  if (object != NULL)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  object = Actor_GetById(10);
  object->scaleX = 0xb333;
  object->scaleY = 0xb333;
  object->radius = 8;
  YampiDesert_SetUpRoom8SlideActors();
  Task_Add(YampiDesert_UpdateSandSlide, TASK_PASS_VBLANK);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_Clear(0xa29);
    GameFlag_Set(0xa2a);
    GameFlag_Clear(0xa2b);
  }
#if TLA_PATCH_1
  else if (gMainRoutineWork.entrance == 4) {
    if (GameFlag_Get(0xa29)) {
      scene = (s32)&SceneId_YampiDesert1;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 1);
    } else if (GameFlag_Get(0xa2a)) {
      scene = (s32)&SceneId_YampiDesert8;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    } else if (GameFlag_Get(0xa2b)) {
      scene = (s32)&SceneId_YampiDesert9;
      MAIN_ROUTINE_WORK_U16(0x260) = scene;
      Scene_StoreU16(&MAIN_ROUTINE_WORK_U16(0x262), 2);
    }
  }
#endif
}

void YampiDesert_SetUpRoom9(void) {
  if (Actor_GetById(64) != 0)
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  if (gMainRoutineWork.entrance == 2) {
    GameFlag_Clear(0xa29);
    GameFlag_Clear(0xa2a);
    GameFlag_Set(0xa2b);
  }
}

struct RideEntry *YampiDesert_AddRisingSand(s32 slot, s32 id, s32 timer,
                                            s32 low, s32 high);

// Room 10's setup.
void YampiDesert_SetUpRoom10(void) {
  gFieldWork->transition = 0x209;
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  if (gMainRoutineWork.entrance == 1 && !GameFlag_Get(0x88c)) {
    YampiDesert_LandAndFightKingScorpion();
  } else {
    if (gMainRoutineWork.entrance == 2 && !GameFlag_Get(0x88c))
      YampiDesert_DropScoopGem();
  }
  if (GameFlag_Get(0x8ed)) {
    MapGrid_CopyCellPictureIndicesRect(35, 39, 13, 11, 3, 3);
    Actor_SetPositionFixed(9, 232 << 16, 200 << 16);
  }
  YampiDesert_StartRisingSand(0, 240);
  YampiDesert_AddRisingSand(0, 9, 0, 0, 64);
  wait(10);
}

void YampiDesert_SetUpRoom11(void) { gFieldWork->transition = 0x204; }

void YampiDesert_SetUpRoom12(void) {
  gFieldWork->transition = 0x204;
  Scene_QueueMusic(SE_137);
  YampiDesert_PlaceStakes(gYampiDesert_Room12Stakes);
  YampiDesert_SetUpRoom12SlideActors();
  Scene_TaskAdd(YampiDesert_UpdateSandSlide, 0x480);
}

void YampiDesert_PlaceStakes(const void *script) {
  const u16 *entry;

  if (*(const u16 *)script == 0xffff)
    return;
  entry = script;
loop:
  {
    s32 id = entry[0];
    struct Actor *object = Actor_GetById(id);
    s32 kind = (s16)entry[1];
    s32 flag = (s16)entry[2];

    object->collisionFlags &= 0xfe;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetCellOpen(id, (u16)kind, (u16)flag, object->terrainId);
    entry += 3;
  }
  if (entry[0] != 0xffff)
    goto loop;
}
#undef FIELD32
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR
#undef MAIN_ROUTINE_WORK_U16

// Finds the current target's table row and shrinks it.
#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

// The whole-pixel half of a 16.16 coordinate.
/* VIEW(s3910): macro PIXELS as PIXELS__s3910; another part or header has
 * another definition; tried with the definition the rest of the object has,
 * YampiDesert_UpdateRisingSand moves: 33 of 212 instructions in tla-ja
 * (yampi_desert.c). */
#define PIXELS__s3910(v) (((s16 *)&(v))[1])

// One rising platform and what rides it.
struct RideEntry {
  s16 id;
  s16 timer;
  s16 height;
  s16 period;
  s16 riseAt;
  s16 fallAt;
  s16 max;
  s16 min;
  s16 unk10;
  s16 step;
  s16 unk14;
  s16 phase;
  struct Actor *rider;
};

struct RideWork {
  s16 entry;
  s16 value;
  struct RideEntry entries[16];
  void *block;
};

s32 Trig_Sin(s32 angle);

// The rising platforms callback: for each of the 16 entries it raises or lowers
// the platform on its timer, writes its height to the collision and cell maps,
// and carries its rider with a bob.
void YampiDesert_UpdateRisingSand(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct RideEntry *entry = work->entries;
  s32 value = work->value;
  u8 *terrain = map->layers[0].terrainIds;
  struct MapCell *cells = map->layers[0].cells;
  s32 i = 0;
  s32 *collision = (s32 *)Ram_MapCollision + value;

  for (; i <= 15; collision++, entry++, i++) {
    struct Actor *object;
    struct Actor *rider;
    struct MapCell *cell;
    s32 index;

    if (entry->id == 0)
      continue;
    object = Actor_GetById(entry->id);
    if (object->x == 0 && object->z == 0)
      continue;
    if (entry->timer == entry->riseAt) {
      Sound_Play(SE_CURSE_FALL);
      entry->step = 4;
    }
    if (entry->timer == entry->fallAt) {
      Sound_Play(SOUND_FADE_OUT_SE5);
      entry->step = -8;
    }
    if (entry->step != 0) {
      entry->height += entry->step;
      if (entry->height >= entry->max) {
        entry->height = entry->max;
        entry->step = 0;
        entry->phase = 0;
      }
      if (entry->height <= entry->min) {
        entry->height = entry->min;
        entry->step = 0;
        entry->phase = 0;
      }
    }
    if (!GameFlag_Get(0x104)) {
      if (++entry->timer >= entry->period)
        entry->timer = 0;
    }
    index = (object->x >> 20) + (object->z >> 20) * 128;
    terrain[index] = (u8)work->value + i;
    *collision = ((PIXELS__s3910(object->y) + entry->height) / 8) << 8;
    cell = &cells[index];
    if (entry->step != 0)
      cell->attributeB = 0;
    else
      cell->attributeB |= 128;
    if (entry->height != 0)
      cell->attributeB |= 16;
    else
      cell->attributeB = 0;
    rider = entry->rider;
    if (rider == NULL)
      continue;
    if (entry->height == 0) {
      rider->y = object->y + -0x100000;
    } else {
      s32 wave;

      entry->phase += 0x1000;
      wave = Iwram_MulQ16(0x20000, Trig_Sin(entry->phase)) + -0xe0000;
      rider->y = object->y + (entry->height << 16) + wave;
    }
    rider->x = object->x;
    rider->z = object->z;
  }
}

// Draws the rising platforms' columns as sprites, one 16-pixel piece per step
// of height and a bobbing top, up to 80 of them, each an OAM entry queued by
// Oam_AddSpriteAtDepthClamped.
void YampiDesert_DrawRisingSand(void) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = work->entries;
  s32 i;
  u32 tile = gResourceTableEntries[work->entry].offset >> 5;
  s32 frame;
  u32 *oam = work->block;
  s32 count = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX, viewZ, originY;
  struct Actor *object;
  s32 h, x, y, sy, z, depth;
  u32 *p;

  viewX = view[0];
  viewX &= 0xffff0000;
  viewZ = view[1] & 0xffff0000;
  originY = map->origin[1];
  for (i = 15; i >= 0; i--, entry++) {
    if (entry->id == 0 || entry->height == 0)
      continue;
    object = Actor_GetById(entry->id);
    frame = (gFrameCount % 12) / 3 * 8 + 32;
    h = 0;
    if (h >= entry->height)
      goto top;
  again:
    if (count > 79)
      goto next;
    x = object->x - viewX;
    y = object->y + (h << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    sy = (sy >> 16) - 16;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      goto next;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  next:
    h += 16;
    if (h < entry->height)
      goto again;
  top:
    if (count > 79)
      continue;
    x = object->x - viewX;
    y = object->y + (entry->height << 16) - originY;
    z = object->z - viewZ - originY;
    sy = z - y;
    x = (x >> 16) - 8;
    /* FAKEMATCH(wrapper): forwarded. */
    sy = ((sy + Math_MulQ16(-0x20000, Trig_Sin(entry->phase)) + -0x20000) >>
          16) -
         4;
    depth = ((y + z) >> 16) + 58;
    if ((u32)(x + 15) > 334 || sy <= -16 || sy > 239)
      continue;
    x &= 511;
    sy &= 255;
    p = oam;
    *p++ = 0;
    *p++ = sy | x << 16 | 0x40002000;
    *p = (tile + frame - 32) | 0x800;
    Oam_AddSpriteAtDepthClamped(oam, depth);
    oam += 3;
    count++;
  }
}

void YampiDesert_StartRisingSand(s32 unused, s32 value) {
  struct YampiDesertRideWork *work =
      (struct YampiDesertRideWork *)(gInventorySnapshot + 0x188);
  void *buffer;

  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWords((s32)work, 456);
  buffer = (void *)Value1((s32 (*)())Heap_AllocEwram, 0x800);
  Decompress_TaggedLz(gField_Tiles64D, buffer);
  work->entry = VramCache_FindFreeEntry();
  VramCache_Load(work->entry, 0x800, buffer);
  Heap_Release(buffer);
  work->block = Heap_AllocEwram(0x3c0);
  work->value = value;
  Task_Add((s32)YampiDesert_UpdateRisingSand, 0x48d);
  Task_Add((s32)YampiDesert_DrawRisingSand, 0x480);
  Actor_GetById(Party_GetLeaderActor())->priorityFlags |= 0x40;
}

struct RideEntry *YampiDesert_AddRisingSand(s32 slot, s32 id, s32 timer,
                                            s32 low, s32 high) {
  struct RideWork *work = (struct RideWork *)(gInventorySnapshot + 0x188);
  struct RideEntry *entry = &work->entries[slot];
  struct Actor *object = Actor_GetById(id);
  struct Actor *rider;

  Actor_GetById(id);
  Actor_SetSpriteFlags(object, 0);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (high < low) {
      s32 swap = low;

      low = high;
      high = swap;
    }
    entry->id = id;
    entry->height = 0;
    entry->timer = timer;
    entry->period = 360;
    entry->max = high;
    entry->riseAt = 60;
    entry->fallAt = 240;
    entry->min = low;
    entry->unk10 = 1;
    object->motionFlags = 0;
    object->collisionFlags = 8;
    entry->step = 0;
    entry->phase = 0;
    entry->unk14 = 0;
    object->priorityFlags |= 2;
  }
  rider = Import_ActorCreate(0x8000, object->x, object->y, object->z);
  rider->collisionFlags = 0;
  rider->motionFlags = 0;
  rider->priorityFlags |= 128;
  entry->rider = rider;
  return entry;
}

#undef PIXELS__s3910

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gYampiDesert_SandCloudCells[] = {
    0x9800e0, 0x9800e8, 0x9800f0, 0xa000d8, 0xa000e0, 0xa000e8,
    0xa000f0, 0xa000f8, 0xa800d8, 0xa800e0, 0xa800e8, 0xa800f0,
    0xa800f8, 0xb000d8, 0xb000f8,
};

u32 gYampiDesert_SlideDust[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};
// clang-format on

#undef OBJ
#undef MOVE_LEADER_TO
#undef FIELD32
#undef WALK
#undef ARRIVE
#undef WALK_ON
#undef ARRIVE_OR
#undef MAIN_ROUTINE_WORK_U16
#undef PIXELS__s3910
#undef Motion_SetModeAndWaitAnimation
#undef Actor_SetMovementTypeAndWaitById
#undef gYampiDesert_FinderRunsAhead
#undef gYampiDesert_TravelersFollow

// The Yampi Desert's data: its effect and travelers' scripts, stake lists,
// rooms' tables and triggers.

#include "text/msg_ids.h"

void YampiDesert_FightBlitz(void);
void YampiDesert_TalkToTraveler(void);
void YampiDesert_TalkToElderAboutWay(void);
void YampiDesert_StepOntoStakeTops(void);
void YampiDesert_HideMedalSpot(void);
void YampiDesert_HideSlideActors(void);

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

// clang-format off
LATE_DATA u32 gYampiDesert_TravelerLooksAround[] = {
    22, 3, 0xb000, 0, 80, 22,
    3, 0, 0, 80, 22, 3,
    0xd000, 0, 20, 22, 3, 0,
    0, 40, 13, 0xffff, 0,
};

LATE_DATA u32 gYampiDesert_OtherTravelerLooksAround[] = {
    22, 3, 0xd000, 0, 20, 22,
    3, 0, 0, 40, 22, 3,
    0xb000, 0, 80, 22, 3, 0,
    0, 80, 13, 0xffff, 0,
};

LATE_DATA u32 gYampiDesert_FinderRunsAhead[] = {
    39, 2, 4, 0xf80000, 0, 0xf80000,
    1, 4, 0xf80000, 0, 0x1180000, 1,
    4, 0x1080000, 0, 0x1900000, 1, 39,
    1, 17,
};

LATE_DATA u32 gYampiDesert_TravelersFollow[] = {
    39, 2, 4, 0xf00000, 0, 0x1180000,
    1, 4, 0x1080000, 0, 0x1900000, 1,
    39, 1, 17,
};

LATE_DATA u32 gYampiDesert_LastTravelersFollow[] = {
    39, 2, 4, 0xd80000, 0, 0xe80000,
    1, 4, 0xf00000, 0, 0x1180000, 1,
    4, 0x1080000, 0, 0x1900000, 1, 39,
    1, 17,
};

LATE_DATA s16 gYampiDesert_Room1Stakes[] = {
    8, 0, 512, 9, 0, 513, 10, 0,
    514, 11, 0, 515, 12, 0, 516, 13,
    0, 517, -1,
};

LATE_DATA s16 gYampiDesert_Room3Stakes[] = {
    8, 0, 512, 9, 0, 513, 10, 0,
    514, 11, 0, 515, 12, 0, 516, 13,
    0, 517, 14, 0, 518, 15, 0, 519,
    16, 0, 520, 17, 0, 521, 18, 0,
    522, -1,
};

LATE_DATA s16 gYampiDesert_Room4Stakes[] = {
    8, 0, 512, 9, 0, 513, 10, 0,
    514, 11, 0, 515, -1,
};

LATE_DATA s16 gYampiDesert_Room5Stakes[] = {
    8, 0, 512, 9, 0, 513, 10, 0,
    514, -1,
};

LATE_DATA s32 gYampiDesert_Room12Stakes[] = {
    8, -0xfe00,
};

LATE_DATA s32 gYampiDesert_RaisedEvent = 0;

LATE_DATA struct Actor *gYampiDesert_SandRipple = NULL;

LATE_DATA struct SceneEntrance gYampiDesert_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA struct SceneEntrance gYampiDesert_Room11Entrances[] = {
    {1, CONDITION_ALWAYS, 408, 0, 480, FACING_NORTH, 0, 288, 376, 528, 536, 0},
    {2, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 288, 376, 528, 536, 0},
    {3, CONDITION_ALWAYS, 120, 0, 424, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {4, CONDITION_ALWAYS, 376, 0, 88, FACING_SOUTH, 0, 0, 16, 448, 496, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gYampiDesert_Room10Regions[] = {
    0x2c00d8, 0xf800b8, 0xd8004c, 0x1ffff, 0xffff, 0,
    0, 0,
};

LATE_DATA s16 gYampiDesert_Room11Regions[] = {
    112, 24, 416, 128, 40, 432, -1, 3,
    368, 24, 80, 384, 40, 96, -1, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(YampiDesert1),
    SCENE_EXIT(1, WorldMap, 17),
    SCENE_EXIT(2, YampiDesert2, 1),
    SCENE_EXITS(YampiDesert2),
    SCENE_EXIT(1, YampiDesert1, 2),
    SCENE_EXIT(2, YampiDesert6, 1),
    SCENE_EXIT(3, YampiDesert11, 1),
    SCENE_EXIT(4, YampiDesert3, 1),
    SCENE_EXIT(5, YampiDesert11, 3),
    SCENE_EXITS(YampiDesert3),
    SCENE_EXIT(1, YampiDesert2, 4),
    SCENE_EXIT(2, YampiDesert4, 1),
    SCENE_EXITS(YampiDesert4),
    SCENE_EXIT(1, YampiDesert3, 2),
    SCENE_EXIT(2, YampiDesert4, 3),
    SCENE_EXIT(3, YampiDesert4, 2),
    SCENE_EXIT(4, YampiDesert5, 1),
    SCENE_EXITS(YampiDesert5),
    SCENE_EXIT(1, YampiDesert4, 4),
    SCENE_EXIT(2, YampiDesert5, 3),
    SCENE_EXIT(3, YampiDesert5, 2),
    SCENE_EXIT(4, YampiDesert10, 1),
    SCENE_EXITS(YampiDesert6),
    SCENE_EXIT(1, YampiDesert2, 2),
    SCENE_EXIT(2, YampiDesert12, 1),
    SCENE_EXITS(YampiDesert7),
    SCENE_EXIT(1, YampiDesert12, 2),
    SCENE_EXIT(2, YampiDesert8, 1),
    SCENE_EXITS(YampiDesert8),
    SCENE_EXIT(1, YampiDesert7, 2),
    SCENE_EXIT(2, WorldMap, 18),
    SCENE_EXIT(3, YampiDesert9, 1),
    SCENE_EXIT(4, YampiDesertCave1, 1),
    SCENE_EXITS(YampiDesert9),
    SCENE_EXIT(1, YampiDesert8, 3),
    SCENE_EXIT(2, WorldMap, 19),
    SCENE_EXITS(YampiDesert10),
    SCENE_EXIT(1, YampiDesert5, 4),
    SCENE_EXITS(YampiDesert11),
    SCENE_EXIT(1, YampiDesert2, 3),
    SCENE_EXIT(2, YampiDesert11, 4),
    SCENE_EXIT(3, YampiDesert2, 5),
    SCENE_EXIT(4, YampiDesert11, 2),
    SCENE_EXITS(YampiDesert12),
    SCENE_EXIT(1, YampiDesert6, 2),
    SCENE_EXIT(2, YampiDesert7, 1),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gYampiDesert_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room1Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room2Placements[] = {
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, (s32)gYampiDesert_TravelerLooksAround, 200, 0, 216, 0xd000, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, (s32)gYampiDesert_OtherTravelerLooksAround, 216, 0, 200, 0xd000, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 200, 0x3000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 162, 0, 232, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 212, 0, 246, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 230, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 230, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room2PlacementsAfterMeeting[] = {
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 242, 0xb000, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(88, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 533, 0, 271, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 435, 0, 323, 0x3000, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(93, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 467, 0, 248, 0x5000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 462, 0, 340, 0xd000, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 482, 0, 340, 0xd000, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 489, 0, 320, 0xb000, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room3Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(246, 0x74, MOVEMENT_TYPE_STAND, 504, 0, 568, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room4Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(251, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room5Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(251, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(354, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room8Placements[] = {
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(407, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 768, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room10Placements[] = {
    ACTOR_PLACEMENT(103, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gYampiDesert_Room12Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, 0x203, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x204, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x205, EVENT(YampiDesert_PoundRoom1Stake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {STEP_TRIGGER_WALKING, 5, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbRevealedLadder)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, 0x88d, EVENT(YampiDesert_ScoopLadderPit)},
    {TRIGGER_TALK, 8, 0x8f0, EVENT(YampiDesert_TalkToTraveler)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAtLastWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8f0, MSGNO(MsgBriggsAndThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgOhNotAgain)},
    {TRIGGER_TALK, 9, 0x8f0, MSGNO(MsgIfThatAccursed)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgIWasCertain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8f0, MSGNO(MsgThatIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgMyJobIs)},
    {TRIGGER_TALK, 10, 0x8f0, MSGNO(MsgOooohThisDesert)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgIMJust)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x8f0, MSGNO(MsgSureItS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgMaybeWeShould)},
    {TRIGGER_TALK, 11, 0x8f0, MSGNO(MsgWaterMyThroat)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgOhManThis)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x8f0, MSGNO(MsgThereSA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgIDonTWant)},
    {TRIGGER_TALK, 12, 0x8f0, EVENT(YampiDesert_TalkToElderAboutWay)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgOseniaIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1036, 0x88f, EVENT(YampiDesert_TalkToElderAboutWay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x8f0, MSGNO(MsgWeVeReached)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHowMuchLonger)},
    {TRIGGER_TALK, 13, 0x8f0, EVENT(YampiDesert_TalkToElder)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgMySonHas)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1037, 0x88f, EVENT(YampiDesert_TalkToElder)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x8f0, MSGNO(MsgMySonIs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgRegardlessOfWhat)},
    {TRIGGER_TALK, 14, 0x8f0, MSGNO(MsgFatherHowAm)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgIfWeRemain)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x8f0, MSGNO(MsgMyFatherS)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgHoooICan)},
    {TRIGGER_STEP, 15, 0x8f0, EVENT(YampiDesert_SeeTravelersAbove)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, 0x203, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 12, 0x204, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 13, 0x205, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 14, 0x206, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 15, 0x207, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 16, 0x208, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 17, 0x209, EVENT(YampiDesert_PoundRoom3Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, 0x20a, EVENT(YampiDesert_PoundRoom3Stake)},
    {TRIGGER_STEP, 11, 0x20b, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, 10, 0x120b, EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, EVENT(YampiDesert_FightBlitz)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(YampiDesert_PoundRoom4Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(YampiDesert_PoundRoom4Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202, EVENT(YampiDesert_PoundRoom4Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, 0x203, EVENT(YampiDesert_PoundRoom4Stake)},
    {TRIGGER_STEP, 11, 0x20b, EVENT(YampiDesert_StepOntoStakeTops)},
    {TRIGGER_STEP, 10, 0x120b, EVENT(YampiDesert_StepOffStakeTops)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, 210, CONDITION_ALWAYS, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, 200, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpion)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(YampiDesert_PoundRoom5Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 9, 0x201, EVENT(YampiDesert_PoundRoom5Stake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 10, 0x202, EVENT(YampiDesert_PoundRoom5Stake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_RAISED, 210, 0x88b, EVENT(YampiDesert_WatchScorpionStop)},
    {TRIGGER_RAISED, 201, CONDITION_ALWAYS, EVENT(YampiDesert_StirScorpionAgain)},
    {TRIGGER_RAISED, 202, CONDITION_ALWAYS, EVENT(YampiDesert_CornerScorpion)},
    {TRIGGER_STEP, 10, 0x88c, EVENT(YampiDesert_TrapKingScorpion)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 12, 0x302, EVENT(YampiDesert_ScoopTreasurePit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room7Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room8Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 8, 0x20c, 0},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealRockHint)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HideRockHint)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(YampiDesert_ReadSignpost)},
    {TRIGGER_STEP, 7, 0x120d, EVENT(YampiDesert_HideSlideActors)},
    {TRIGGER_STEP, 8, 0x20d, EVENT(YampiDesert_ShowSlideActors)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room9Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(YampiDesert_RevealMedalSpot)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_HideMedalSpot)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room10Triggers[] = {
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 10, 0x8ed, EVENT(YampiDesert_ScoopCenterPit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room11Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(YampiDesert_ClimbOutOfRoom11)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gYampiDesert_Room12Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, 0x200, EVENT(YampiDesert_PoundRoom12Stake)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(YampiDesert_CastSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(YampiDesert_EndSand)},
    {TRIGGER_TABLE_END},
};

LATE_DATA u32 gYampiDesert_SandCloudDrifts[] = {
    0, 1, 4, 0xe80000, 0, 0xba0000,
    1, 38,
};
// clang-format on
