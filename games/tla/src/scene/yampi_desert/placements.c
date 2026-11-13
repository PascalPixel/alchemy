/// @file
/// The Yampi Desert's tables of entrances, placements and triggers by room, its
/// travelers' talk, and the stakes Pound sinks and Sand slips under; linked
/// apart from yampi_desert.c, with the field's shared effect code between.

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../../field/field_effect/spawn.c"

#include "scene.h"
#include "module/scene/yampi_desert.h"
#include "module/tla.h"

extern const u8 gYampiDesert_Entrances[];
extern const u8 gYampiDesert_Room11Entrances[];
extern const u8 gYampiDesert_Room10Regions[];
extern const u8 gYampiDesert_Room11Regions[];

void YampiDesert_FightBlitz(void) { Field_RequestDjinnBattle(19, 68); }

const u8 *Scene_GetEntrances(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room11)
    return gYampiDesert_Room11Entrances;
  return gYampiDesert_Entrances;
}

const u8 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room10)
    return gYampiDesert_Room10Regions;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gYampiDesert_Room11Regions;
  return 0;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "text/msg_ids.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"

extern const u8 gYampiDesert_Placements[];
extern const u8 gYampiDesert_Room1Placements[];
extern const u8 gYampiDesert_Room2Placements[];
extern const u8 gYampiDesert_Room2PlacementsAfterMeeting[];
extern const u8 gYampiDesert_Room3Placements[];
extern const u8 gYampiDesert_Room4Placements[];
extern const u8 gYampiDesert_Room5Placements[];
extern const u8 gYampiDesert_Room8Placements[];
extern const u8 gYampiDesert_Room10Placements[];
extern const u8 gYampiDesert_Room12Placements[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gYampiDesert_Room1Placements;
  if (scene == (s32)&SceneId_Ov661Room2) {
    if (GameFlag_Get(0x8f0))
      return gYampiDesert_Room2PlacementsAfterMeeting;
    return gYampiDesert_Room2Placements;
  }
  if (scene == (s32)&SceneId_Ov661Room3)
    return gYampiDesert_Room3Placements;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gYampiDesert_Room4Placements;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gYampiDesert_Room5Placements;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gYampiDesert_Room8Placements;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gYampiDesert_Room10Placements;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gYampiDesert_Room12Placements;
  return gYampiDesert_Placements;
}

void YampiDesert_ScoopTreasurePit(s32 a) { Scene_Call4178(a, 64, 0x302); }

extern const u8 gYampiDesert_Triggers[];
extern const u8 gYampiDesert_Room1Triggers[];
extern const u8 gYampiDesert_Room2Triggers[];
extern const u8 gYampiDesert_Room3Triggers[];
extern const u8 gYampiDesert_Room4Triggers[];
extern const u8 gYampiDesert_Room5Triggers[];
extern const u8 gYampiDesert_Room6Triggers[];
extern const u8 gYampiDesert_Room7Triggers[];
extern const u8 gYampiDesert_Room8Triggers[];
extern const u8 gYampiDesert_Room9Triggers[];
extern const u8 gYampiDesert_Room10Triggers[];
extern const u8 gYampiDesert_Room11Triggers[];
extern const u8 gYampiDesert_Room12Triggers[];

/// Returns the current room's table.
// As ☀️'s Scene_GetEntrances picks one.
const u8 *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov661Room1)
    return gYampiDesert_Room1Triggers;
  if (scene == (s32)&SceneId_Ov661Room2)
    return gYampiDesert_Room2Triggers;
  if (scene == (s32)&SceneId_Ov661Room3)
    return gYampiDesert_Room3Triggers;
  if (scene == (s32)&SceneId_Ov661Room4)
    return gYampiDesert_Room4Triggers;
  if (scene == (s32)&SceneId_Ov661Room5)
    return gYampiDesert_Room5Triggers;
  if (scene == (s32)&SceneId_Ov661Room6)
    return gYampiDesert_Room6Triggers;
  if (scene == (s32)&SceneId_Ov661Room7)
    return gYampiDesert_Room7Triggers;
  if (scene == (s32)&SceneId_Ov661Room8)
    return gYampiDesert_Room8Triggers;
  if (scene == (s32)&SceneId_Ov661Room9)
    return gYampiDesert_Room9Triggers;
  if (scene == (s32)&SceneId_Ov661Room10)
    return gYampiDesert_Room10Triggers;
  if (scene == (s32)&SceneId_Ov661Room11)
    return gYampiDesert_Room11Triggers;
  if (scene == (s32)&SceneId_Ov661Room12)
    return gYampiDesert_Room12Triggers;
  return gYampiDesert_Triggers;
}

/// ★ Named from the game: "We followed the pirates here from Madra... Are you
/// going to Alhafra, too?" (MsgWeFollowedThe)
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

/// ★ Named from the game: "As you can see, the elder is quite insistent on
/// finding a way through." (MsgAsYouCan)
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

/// ★ Named from the game: "This is where my son must prove himself as mayor..."
/// (MsgThisIsWhere)
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

/// ★ Named from the game: "North: Alhafra South: Air's Rock Warning: Beyond
/// this point, there is no route back." (MsgNorthAlhafraSouth)
void YampiDesert_ReadSignpost(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldMessage_Show((s32)MsgNorthAlhafraSouth, 1);
  Event_End();
}

void YampiDesert_PoundRoom1Stake(s32 a, s32 b) {
  Statue_Sink(gYampiDesert_Room1Stakes, a, b);
}

void YampiDesert_PoundRoom3Stake(s32 a, s32 b) {
  Statue_Sink(gYampiDesert_Room3Stakes, a, b);
}

#define DEFINE_POUND_STAKE_STOPPING_SCORPION(name, data1)                      \
  void name(s32 a, s32 b) {                                                    \
    Statue_Sink(data1, a, b);                                                  \
    if (a == 1) {                                                              \
      if (gYampiDesert_RaisedEvent == 0)                                       \
        gYampiDesert_RaisedEvent = 210;                                        \
    }                                                                          \
  }

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundRoom4Stake,
                                     gYampiDesert_Room4Stakes)

void YampiDesert_PoundRoom12Stake(s32 a, s32 b) {
  Statue_Sink(gYampiDesert_Room12Stakes, a, b);
}

DEFINE_POUND_STAKE_STOPPING_SCORPION(YampiDesert_PoundRoom5Stake,
                                     gYampiDesert_Room5Stakes)

void YampiDesert_StepOntoStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gYampiDesert_Room3Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gYampiDesert_Room4Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
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
  GameFlag_Set(0x20b);
}

void YampiDesert_StepOffStakeTops(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u16 *list = NULL;
  u8 *grid = gMapShapeGrid;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room3)
    list = gYampiDesert_Room3Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room4)
    list = gYampiDesert_Room4Stakes;
  else if (gMainRoutineWork.scene == (s32)&SceneId_Ov661Room5)
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
      /* FAKEMATCH(pin): the index in r10; as a plain local, 32 of 130
       * instructions differ in tla-en. */
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
  GameFlag_Clear(0x20b);
}

/// For each six-byte record (actor, unused, flag) up to an actor of 0xffff:
/// when the record's flag is set, the cell under the actor in map layer 2 takes
/// collision code 255.
void YampiDesert_BlockPoundedStakes(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object;
  /* FAKEMATCH(pin): the limit in r2, so the next id takes r3; as a plain
   * local, 10 of 31 instructions differ in tla-en. */
  register s32 limit asm("r2");

  goto next;
  do {
    /* FAKEMATCH(barrier): the record pointer passed through an empty asm, so
     * the id is read again and the pointer outranks the actor for r5; without
     * it, 19 of 31 instructions differ in tla-en. */
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
    /* FAKEMATCH(dead): the advance in a once-only block, so the store waits for
     * it; as list += 3, 2 of 25 instructions differ in tla-en. */
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
  if (scene == (s32)&SceneId_Ov661Room1)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room1Stakes);
  else if (scene == (s32)&SceneId_Ov661Room3)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room3Stakes);
  else if (scene == (s32)&SceneId_Ov661Room4)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room4Stakes);
  else if (scene == (s32)&SceneId_Ov661Room5)
    YampiDesert_BlockPoundedStakes((u16 *)gYampiDesert_Room5Stakes);
}

void YampiDesert_EndSand(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s16 scene;

  leader->terrainId = 0;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room1)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room1Stakes);
  else if (scene == (s32)&SceneId_Ov661Room3)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room3Stakes);
  else if (scene == (s32)&SceneId_Ov661Room4)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room4Stakes);
  else if (scene == (s32)&SceneId_Ov661Room5)
    YampiDesert_UnblockStakes((u16 *)gYampiDesert_Room5Stakes);
}

void YampiDesert_WatchScorpionStop(void) {
  s16 scene;

  Event_Start();
  Event_ResolvePendingActions(0);
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov661Room4) {
    FieldCamera_FollowActor(16, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  } else if (scene == (s32)&SceneId_Ov661Room5) {
    FieldCamera_FollowActor(21, 1);
    Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 21);
  }
  // Entered at its test, which keeps the test single and the address load
  // inside the loop: as a while loop the test is copied above it and the
  // address kept in r5.
  goto test;
  do {
    wait(1);
  test:;
  } while (gYampiDesert_RaisedEvent == 210);
  Event_End();
}
