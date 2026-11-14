/// @file
/// The Dehkan Plateau, the cliffs and caves of Indra between Daila and the East
/// Indra Shore, in seven rooms that share this overlay. Its floors are cracked:
/// the first step on a crack opens it, the next drops the leader through to the
/// cave below (DehkanPlateau_StepOnCrack), and each room puts back the cracks
/// its flags remember (DehkanPlateau_RestoreRoom1Cracks). Grey pillars crumble
/// once stood on (CrumblingPillars_Step), and the Mars Djinni Cannon runs ahead
/// of the party room by room, cracking floors, crumbling pillars and casting
/// Pound (DehkanPlateau_CannonCrumblesPillars), until a fall lands the leader
/// beside it and it drops the Pound Cube (DehkanPlateau_CannonDropsPoundCube);
/// cornered at last behind a cone the party Pounds flat, it fights
/// (DehkanPlateau_FightCannon).

#include "types.h"
#include "item_ids.h"
#include "system/system.h"
#include "actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/field.h"
#include "map.h"
#include "field/field_sequence_event.h"
#include "module/field/event_commands.h"
#include "module/scene/dehkan_plateau.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/main_routine_work.h"
#include "field_sprite.h"
#include "system/iwram_call.h"
#include "system/field_effect.h"
#include "module/field/field_effect.h"
#include "shared/tundaria_tower.h"
#include "module/field/field.h"
#include "flags.h"
#include "djinn_ids.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "field/event.h"
#include "flag_call.h"
#include "scene_call.h"
#include "fixed_math.h"
#include "objcmd.h"
#include "gamepak/far_call/system.h"
#include "system/heap_release.h"
#include "system/task.h"

// Cannon's flight across the plateau, a flag for each stage, room by room.
#define FLAG_CANNON_FLED_OVER_CRACKS 0x86c ///< room 2, over the cracked floor
#define FLAG_CANNON_FLED_EAST 0x86d        ///< room 2, out to the east
#define FLAG_CANNON_LEAPT_AWAY 0x86e       ///< room 3, over the first gap
#define FLAG_CANNON_POUNDED_PILLAR 0x86f   ///< room 3, Pounding a pillar down
#define FLAG_CANNON_CRUMBLED_PILLARS                                           \
  0x870 ///< room 3, over the crumbling pillars
/// Room 5: Cannon drops the Pound Cube, and the party takes it.
#define FLAG_CANNON_DROPPED_POUND_CUBE 0x871
#define FLAG_POUND_CUBE_TAKEN 0x878
/// The pillars Moved into place in rooms 2 and 3.
#define FLAG_ROOM2_PILLAR_PLACED 0x840
#define FLAG_ROOM3_SECOND_PILLAR_PLACED 0x841

/// Each room's Cannon, and the pillars the party Moves.
enum DehkanPlateauActorId {
  ACTOR_ROOM1_PILLAR = ACTOR_FIRST_PLACED + 4,
  ACTOR_ROOM2_PILLAR = ACTOR_FIRST_PLACED + 15,
  ACTOR_ROOM2_CANNON,
  ACTOR_ROOM3_POUND_PILLAR = ACTOR_FIRST_PLACED + 10,
  ACTOR_ROOM3_PILLAR,
  ACTOR_ROOM3_SECOND_PILLAR,
  ACTOR_ROOM3_CANNON = ACTOR_FIRST_PLACED + 17,
  ACTOR_ROOM5_CANNON = ACTOR_FIRST_PLACED + 1,
  ACTOR_ROOM6_CANNON = ACTOR_FIRST_PLACED + 8
};

// A room's scenery actors hidden while the leader is behind them.
#define FLAG_ACTORS_HIDDEN FLAG_SCENE_TEMP_00
// The leader stands on the crack he has just opened; set until he steps off.
#define FLAG_STANDING_ON_OPENED_CRACK FLAG_SCENE_TEMP_01
// The leader stands in room 6's cell beside the cone.
#define FLAG_IN_CONE_CELL FLAG_SCENE_TEMP_02
// Each room's pillar Pounded into the ground, by its list's flag.
#define FLAG_ROOM1_PILLAR_POUNDED FLAG_SCENE_TEMP_03
#define FLAG_ROOM2_PILLAR_POUNDED FLAG_SCENE_TEMP_04
#define FLAG_ROOM3_PILLAR_POUNDED FLAG_SCENE_TEMP_05
#define FLAG_ROOM5_PILLAR_POUNDED FLAG_SCENE_TEMP_06
// Room 6's cone, Pounded flat so Cannon is cornered.
#define FLAG_CONE_POUNDED FLAG_SCENE_TEMP_07
#define FLAG_ROOM3_ROPE_LASHED FLAG_SCENE_TEMP_08
// Cannon met in room 6 on this visit, or already in the party.
#define FLAG_CANNON_MET FLAG_SCENE_TEMP_09
// The crack the party fell through, one flag for each landing entrance from
// FIRST_LANDING_ENTRANCE, for the landing drawn below it.
#define FLAG_FELL_THROUGH_FIRST FLAG_AREA_TEMP_20
#define FLAG_FELL_THROUGH_LAST FLAG_AREA_TEMP_5F
#define FIRST_LANDING_ENTRANCE 11

void Actor_DisableById(s32 id);
extern s16 gDehkanPlateau_Room1CrumblingPillars[];
/// The Pound Cube Cannon drops, while it lies on the ground.
extern struct Actor *gDehkanPlateau_PoundCube;
void DehkanPlateau_LeaveConeCell(void);
void DehkanPlateau_ForgetFall(void);
void Actor_StartScript(struct Actor *object, const void *script);

#include "shared/crumbling_pillars.h"

#include "../field/field_effect/advance_motion_priority.c"

#include "shared/rubble.h"

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
#include "shared/pillars_raise.h"

#include "shared/pillars_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

#include "../field/object/set_sprite_priority.c"

#include "../field/field_effect/move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect/spawn.c"

#undef EFFECT_SET_SCRIPT

void Actor_DisableById(s32 id);

extern u16 gDehkanPlateau_Room1CrackPictures[];
extern u16 gDehkanPlateau_Room1Cracks[][6];

extern s16 gDehkanPlateau_Room2CrumblingPillars[];
extern s16 gDehkanPlateau_Room3CrumblingPillars[];
extern s16 gDehkanPlateau_Room6CrumblingPillar[];
extern s16 gDehkanPlateau_Room1PoundPillars[];
extern s16 gDehkanPlateau_Room2PoundPillars[];
extern s16 gDehkanPlateau_Room3PoundPillars[];
extern s16 gDehkanPlateau_Room5PoundPillars[];
extern s16 gDehkanPlateau_Room6PoundPillars[];
extern s16 gDehkanPlateau_Room2MovablePillars[];
extern s16 gDehkanPlateau_Room3MovablePillars[];

void DehkanPlateau_FightCannon(void) {
  GameFlag_Set(FLAG_CANNON_MET);
  Field_RequestDjinnBattle(ACTOR_ROOM6_CANNON, DJINNI_CANNON);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gDehkanPlateau_Placements[];
extern struct ActorPlacement gDehkanPlateau_Room1Placements[];
extern struct ActorPlacement gDehkanPlateau_Room2Placements[];
extern struct ActorPlacement gDehkanPlateau_Room3Placements[];
extern struct ActorPlacement gDehkanPlateau_Room4Placements[];
extern struct ActorPlacement gDehkanPlateau_Room5Placements[];
extern struct ActorPlacement gDehkanPlateau_Room6Placements[];
extern struct Trigger gDehkanPlateau_Triggers[];
extern struct Trigger gDehkanPlateau_Room1Triggers[];
extern struct Trigger gDehkanPlateau_Room2Triggers[];
extern struct Trigger gDehkanPlateau_Room3Triggers[];
extern struct Trigger gDehkanPlateau_Room4Triggers[];
extern struct Trigger gDehkanPlateau_Room5Triggers[];
extern struct Trigger gDehkanPlateau_Room6Triggers[];

const u8 *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_DehkanPlateau1)
    return gDehkanPlateau_Room1Placements;
  if (scene == (s32)&SceneId_DehkanPlateau2)
    return gDehkanPlateau_Room2Placements;
  if (scene == (s32)&SceneId_DehkanPlateau3)
    return gDehkanPlateau_Room3Placements;
  if (scene == (s32)&SceneId_DehkanPlateau4)
    return gDehkanPlateau_Room4Placements;
  if (scene == (s32)&SceneId_DehkanPlateau5)
    return gDehkanPlateau_Room5Placements;
  if (scene == (s32)&SceneId_DehkanPlateau6)
    return gDehkanPlateau_Room6Placements;
  return gDehkanPlateau_Placements;
}

const struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_DehkanPlateau1)
    return gDehkanPlateau_Room1Triggers;
  if (scene == (s32)&SceneId_DehkanPlateau2)
    return gDehkanPlateau_Room2Triggers;
  if (scene == (s32)&SceneId_DehkanPlateau3)
    return gDehkanPlateau_Room3Triggers;
  if (scene == (s32)&SceneId_DehkanPlateau4)
    return gDehkanPlateau_Room4Triggers;
  if (scene == (s32)&SceneId_DehkanPlateau5)
    return gDehkanPlateau_Room5Triggers;
  if (scene == (s32)&SceneId_DehkanPlateau6)
    return gDehkanPlateau_Room6Triggers;
  return gDehkanPlateau_Triggers;
}

void DehkanPlateau_StepOnRoom1Pillars(void) {
  CrumblingPillars_Step(gDehkanPlateau_Room1CrumblingPillars);
}

// Gives the cells by room 2's four crumbling pillars still standing the
// attributes of cell (0, 0).
void DehkanPlateau_SetRoom2StandingPillarCells(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_13) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 18);
  if (GameFlag_Get(FLAG_SCENE_TEMP_15) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 11, 18);
  if (GameFlag_Get(FLAG_SCENE_TEMP_21) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 14, 28);
  if (GameFlag_Get(FLAG_SCENE_TEMP_23) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 16, 28);
}

void DehkanPlateau_StepOnRoom2Pillars(void) {
  s32 y;

  CrumblingPillars_StepWithStandIn(gDehkanPlateau_Room2CrumblingPillars);
  DehkanPlateau_SetRoom2StandingPillarCells();
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x80000) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

// The same for room 3's two pillars, from cell (63, 0).
void DehkanPlateau_SetRoom3StandingPillarCells(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_13) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 50, 20);
  if (GameFlag_Get(FLAG_SCENE_TEMP_15) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 52, 20);
}

void DehkanPlateau_StepOnRoom3Pillars(void) {
  CrumblingPillars_Step(gDehkanPlateau_Room3CrumblingPillars);
  DehkanPlateau_SetRoom3StandingPillarCells();
  if (Actor_GetById(gMainRoutineWork.currentOwner)->y < -px(24)) {
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
}

void DehkanPlateau_StepOnRoom6Pillar(void) {
  s32 y;

  CrumblingPillars_Step(gDehkanPlateau_Room6CrumblingPillar);
  y = Actor_GetById(gMainRoutineWork.currentOwner)->y;
  if (y < 0x180000)
    DehkanPlateau_LeaveConeCell();
}

void Actors_SetHidden(s32 show, s32 first, u32 count);

void DehkanPlateau_SetActorsHidden(s32 show) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_DehkanPlateau1)
    Actors_SetHidden(show, 13, 14);
  else if (scene == (s32)&SceneId_DehkanPlateau2)
    Actors_SetHidden(show, 25, 13);
  else if (scene == (s32)&SceneId_DehkanPlateau3)
    Actors_SetHidden(show, 26, 10);
  else if (scene == (s32)&SceneId_DehkanPlateau4)
    Actors_SetHidden(show, 8, 3);
  else if (scene == (s32)&SceneId_DehkanPlateau6)
    Actors_SetHidden(show, 10, 6);
}

void DehkanPlateau_HideActors(void) {
  DehkanPlateau_SetActorsHidden(1);
  GameFlag_Set(FLAG_ACTORS_HIDDEN);
}

void DehkanPlateau_ShowActors(void) {
  DehkanPlateau_SetActorsHidden(0);
  GameFlag_Clear(FLAG_ACTORS_HIDDEN);
}

void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~0x10;
    else
      actor->priorityFlags |= 0x10;
  }
}

void DehkanPlateau_PoundRoom1Pillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_Room1PoundPillars, a, b);
}

void DehkanPlateau_PoundRoom2Pillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_Room2PoundPillars, a, b);
}

void DehkanPlateau_PoundRoom3Pillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_Room3PoundPillars, a, b);
}

void DehkanPlateau_PoundRoom5Pillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_Room5PoundPillars, a, b);
}

void DehkanPlateau_PoundRoom6Pillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_Room6PoundPillars, a, b);
}

#include "shared/actor_drop_and_wait.h"

void DehkanPlateau_DropRoom1PillarIntoGap(void) {
  struct Actor *object = Actor_GetById(ACTOR_ROOM1_PILLAR);
  s32 x;

  x = object->x >> 20;
  if (x == 56 && (object->z >> 20) == 32) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_Set(FLAG_AREA_TEMP_01);
    Event_End();
  }
}

void DehkanPlateau_LiftRoom2Pillar(void) {
  Actor_LiftTargetBlock(gDehkanPlateau_Room2MovablePillars);
}

void DehkanPlateau_PlaceRoom2Pillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_ROOM2_PILLAR);
  s32 x = object->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 52) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 7, 1, 1, x, 7);
    GameFlag_Set(FLAG_ROOM2_PILLAR_PLACED);
    Event_End();
  }
}

void DehkanPlateau_LiftRoom3Pillar(void) {
  Actor_LiftTargetBlock(gDehkanPlateau_Room3MovablePillars);
}

void DehkanPlateau_PlaceRoom3Pillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_ROOM3_PILLAR);
  s32 x = object->x >> 20;

  Actor_PlaceMovedBlock();
  if (x == 47) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(ACTOR_ROOM3_PILLAR, 2);
    Event_End();
  } else if (x == 46) {
    MapGrid_CopyCellAttributes(44, 13, 1, 1, 48, 13);
  }
}

void DehkanPlateau_SetRoom3SecondPillarInPlace(void) {
  Actor_SetPositionFixed(ACTOR_ROOM3_SECOND_PILLAR, 150 << 18, 134 << 18);
  MapGrid_CopyCellAttributes(38, 32, 1, 1, 37, 33);
  MapGrid_CopyCellAttributes(30, 32, 1, 2, 38, 32);
  MapGrid_CopyCellAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_Set(FLAG_ROOM3_SECOND_PILLAR_PLACED);
}

void DehkanPlateau_MoveRoom3SecondPillar(void) {
  if ((Actor_GetById(ACTOR_ROOM3_SECOND_PILLAR)->x >> 20) == 37)
    DehkanPlateau_SetRoom3SecondPillarInPlace();
}

void DehkanPlateau_PushRoom3SecondPillar(void) {
  u8 *flags;

  if ((Actor_GetById(ACTOR_ROOM3_SECOND_PILLAR)->x >> 20) == 38) {
    flags = &Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags;
    Event_Start();
    Event_ResolvePendingActions(0);
    *flags = 0;
    wait(1);
    Leader_PushObjectAhead();
    DehkanPlateau_SetRoom3SecondPillarInPlace();
    wait(1);
    *flags = 3;
    Event_End();
  }
}

// Stepping off a crack opened on this visit lets it be stepped on again.
void DehkanPlateau_StepOffCrack(void) {
  GameFlag_Clear(FLAG_STANDING_ON_OPENED_CRACK);
}

// Forgets which crack the party fell through.
void DehkanPlateau_ForgetFall(void) {
  s32 flag;

  flag = FLAG_FELL_THROUGH_FIRST;
loop:
  GameFlag_Clear(flag);
  if (flag != FLAG_FELL_THROUGH_LAST) {
    flag++;
    goto loop;
  }
}

#define DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(name, data1, n1, data2)            \
  void name(void) {                                                            \
    const u16 *cell = &data1[0][0];                                            \
    s32 flag = 0x240;                                                          \
    u32 i;                                                                     \
    const u16 *dest;                                                           \
                                                                               \
    for (i = 0; i <= n1; i++) {                                                \
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {                       \
        if (GameFlag_Get(flag) != 0) {                                         \
          s32 x;                                                               \
          s32 y;                                                               \
                                                                               \
          cell = &data1[i][1];                                                 \
          x = *cell++;                                                         \
          y = *cell++;                                                         \
          /* FAKEMATCH(temporary): the destination table through a local set   \
           * here. */                                                          \
          dest = data2;                                                        \
          MapGrid_CopyCellPictureIndicesRect(dest[0], dest[1], x, y, 1, 1);    \
        }                                                                      \
      } else {                                                                 \
        if (*cell != 0)                                                        \
          GameFlag_Set(flag);                                                  \
        cell += 6;                                                             \
      }                                                                        \
      flag++;                                                                  \
    }                                                                          \
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0)                           \
      DehkanPlateau_ForgetFall();                                              \
  }

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(DehkanPlateau_RestoreRoom1Cracks,
                                    gDehkanPlateau_Room1Cracks, 13,
                                    gDehkanPlateau_Room1CrackPictures)

extern u16 gDehkanPlateau_Room2CrackPictures[];
extern u16 gDehkanPlateau_Room2Cracks[][6];

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(DehkanPlateau_RestoreRoom2Cracks,
                                    gDehkanPlateau_Room2Cracks, 7,
                                    gDehkanPlateau_Room2CrackPictures)

extern u16 gDehkanPlateau_Room3CrackPictures[];
extern u16 gDehkanPlateau_Room3Cracks[][6];

DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(DehkanPlateau_RestoreRoom3Cracks,
                                    gDehkanPlateau_Room3Cracks, 5,
                                    gDehkanPlateau_Room3CrackPictures)

void DehkanPlateau_RestoreRoom5Cracks(void) {
  u16 *record = &gDehkanPlateau_Room5Cracks[0][0];
  s32 flag = 0x240;
  u32 i;

  for (i = 0; i <= 15; flag++, i++) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (GameFlag_Get(flag)) {
        s32 x;
        s32 y;

        record = &gDehkanPlateau_Room5Cracks[i][1];
        x = *record++;
        y = *record++;
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room5CrackPictures[0],
                                           gDehkanPlateau_Room5CrackPictures[1],
                                           x, y, 1, 1);
      }
    } else {
      if (*record != 0)
        GameFlag_Set(flag);
      record += 6;
    }
  }
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);

extern s32 gDehkanPlateau_CrackZ;

#define SCENE(name) ((s32)&SceneId_##name)

// Steps onto a listed cell in rooms 1, 2, 3 and 5 (SceneId_DehkanPlateau1 to
// Room5): either falls through to exit work->stepTrigger or opens the cell and
// sets its flag. cellX is reused for the doubled index and the fall's frame
// count.
void DehkanPlateau_StepOnCrack(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 cellX = ((leader->x >> 20) << 1) + 1;
  s32 cellZ = ((leader->z >> 20) << 1) + 1;
  s32 n, flag;
  u16 *record, *next;

  if (gMainRoutineWork.scene == SCENE(DehkanPlateau1)) {
    n = work->stepTrigger - 11;
    flag = work->stepTrigger + 0x235;
    record = &gDehkanPlateau_Room1Cracks[n][1];
  } else if (gMainRoutineWork.scene == SCENE(DehkanPlateau2)) {
    n = work->stepTrigger - 14;
    flag = work->stepTrigger + 0x232;
    record = &gDehkanPlateau_Room2Cracks[n][1];
  } else if (gMainRoutineWork.scene == SCENE(DehkanPlateau3)) {
    n = work->stepTrigger - 12;
    flag = work->stepTrigger + 0x234;
    record = &gDehkanPlateau_Room3Cracks[n][1];
  } else if (gMainRoutineWork.scene == SCENE(DehkanPlateau5)) {
    n = work->stepTrigger - 18;
    flag = work->stepTrigger + 0x22e;
    record = &gDehkanPlateau_Room5Cracks[n][1];
    if (GameFlag_Get(FLAG_AREA_TEMP_3A))
      gDehkanPlateau_Room5Cracks[4][3] = 3;
  } else {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 4);
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, cellX << 3,
                          cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, cellX << 19, leader->y, cellZ << 19);
  Event_Wait(4);
  FieldCamera_GetActor()->y += -px(8);
  next = record + 1;
  if (GameFlag_Get(flag)) {
    s32 x, y, index, toX, toZ;

    Sound_Play(SE_SPLASH);
    x = *record;
    record = next;
    {
      s16 scene = gMainRoutineWork.scene;
      y = *record++;
      if (scene == SCENE(DehkanPlateau1))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room1CrackPictures[2],
                                           gDehkanPlateau_Room1CrackPictures[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau2))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room2CrackPictures[2],
                                           gDehkanPlateau_Room2CrackPictures[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau3))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room3CrackPictures[2],
                                           gDehkanPlateau_Room3CrackPictures[3],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau5))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room5CrackPictures[2],
                                           gDehkanPlateau_Room5CrackPictures[3],
                                           x, y, 1, 1);
    }
    index = *record++;
    cellX = index * 2;
    toX = record[0];
    toZ = record[1];
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(DehkanPlateau1)) {
        {
          s32 srcX = gDehkanPlateau_Room1HolePictures[cellX],
              srcY = gDehkanPlateau_Room1HolePictures[cellX + 1], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateau2)) {
        {
          s32 srcX = gDehkanPlateau_Room2HolePictures[cellX],
              srcY = gDehkanPlateau_Room2HolePictures[cellX + 1], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateau3)) {
        {
          s32 srcX = gDehkanPlateau_Room3HolePictures[cellX],
              srcY = gDehkanPlateau_Room3HolePictures[cellX + 1], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateau5)) {
        if ((n == 0 && GameFlag_Get(FLAG_SCENE_TEMP_41)) ||
            (n == 11 && GameFlag_Get(FLAG_SCENE_TEMP_4C))) {
          s32 srcX = gDehkanPlateau_Room5HolePictures[8],
              srcY = gDehkanPlateau_Room5HolePictures[9], width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        } else {
          s32 srcX = gDehkanPlateau_Room5HolePictures[cellX],
              srcY = gDehkanPlateau_Room5HolePictures[cellX + 1], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      }
    }
    Event_Wait(12);
    leader->facing = 0x4000;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(DehkanPlateau1)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      } else if (scene == SCENE(DehkanPlateau2) ||
                 scene == SCENE(DehkanPlateau3)) {
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      } else if (scene == SCENE(DehkanPlateau5)) {
        if (gDehkanPlateau_PoundCube != NULL)
          ((struct FieldSprite *)gDehkanPlateau_PoundCube->sprite)->priority =
              3;
        Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
      }
    }
    Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
    Event_Wait(16);
    for (cellX = 0; (u32)cellX <= 39; cellX++) {
      if (cellX == 5)
        Sound_Play(SE_DUST_PUFF);
      leader->scaleX += -0x400;
      leader->scaleY += -0x300;
      leader->y += -0x9999;
      wait(1);
    }
    gFieldWork->transition = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->stepTrigger);
  } else {
    s32 x, y;

    Sound_Play(SE_RATTLE);
    y = *next;
    x = *record;
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(DehkanPlateau1))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room1CrackPictures[0],
                                           gDehkanPlateau_Room1CrackPictures[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau2))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room2CrackPictures[0],
                                           gDehkanPlateau_Room2CrackPictures[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau3))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room3CrackPictures[0],
                                           gDehkanPlateau_Room3CrackPictures[1],
                                           x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateau5))
        MapGrid_CopyCellPictureIndicesRect(gDehkanPlateau_Room5CrackPictures[0],
                                           gDehkanPlateau_Room5CrackPictures[1],
                                           x, y, 1, 1);
    }
    Event_Wait(12);
    GameFlag_Set(flag);
    GameFlag_Set(FLAG_STANDING_ON_OPENED_CRACK);
    {
      s16 scene = gMainRoutineWork.scene;
      if (scene == SCENE(DehkanPlateau5) || scene == SCENE(DehkanPlateau3)) {
        gDehkanPlateau_CrackX = leader->x >> 20;
        gDehkanPlateau_CrackZ = leader->z >> 20;
      }
    }
    Event_End();
  }
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

void DehkanPlateau_ForgetCrackOnLeaving(void) {
  struct Actor *object;

  if (Flag_Test(FLAG_STANDING_ON_OPENED_CRACK) != 0) {
    object = Actor_GetById(gMainRoutineWork.currentOwner);
    if (gDehkanPlateau_CrackX != (object->x >> 20) ||
        gDehkanPlateau_CrackZ != (object->z >> 20))
      GameFlag_Clear(FLAG_STANDING_ON_OPENED_CRACK);
  }
}

#include "shared/actor_hide.h"

void Scene_RunUnlessFlag(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(FLAG_ACTORS_HIDDEN) == 0)
      DehkanPlateau_ShowActors();
  } else {
    GameFlag_Set(FLAG_ACTORS_HIDDEN);
  }
}

void DehkanPlateau_HideRoom1Actors(void) {
  u32 i;

  for (i = 0; i <= 13; i++)
    Actor_Hide(i + 13);
  Scene_RunUnlessFlag();
}

void DehkanPlateau_HideRoom2Actors(void) {
  u32 i;

  for (i = 0; i <= 12; i++)
    Actor_Hide(i + 25);
  Scene_RunUnlessFlag();
}

void DehkanPlateau_HideRoom3Actors(void) {
  u32 i;

  for (i = 0; i <= 9; i++)
    Actor_Hide(i + 26);
  Scene_RunUnlessFlag();
}

void DehkanPlateau_HideRoom4Actors(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Actor_Hide(i + 8);
  Scene_RunUnlessFlag();
}

void DehkanPlateau_HideRoom6Actors(void) {
  u32 i;

  for (i = 0; i <= 5; i++)
    Actor_Hide(i + 10);
  Scene_RunUnlessFlag();
}

/* VIEW(dehkan_plateau): routine HeldActor_RaiseCell as this part declared it;
 * the joined file has another; tried as the definition's void
 * HeldActor_RaiseCell(void), it does not compile in tla-ja (dehkan_plateau.c):
 * games/tla/src/scene/dehkan_plateau.c:729: too many arguments to function
 * `HeldActor_RaiseCell'. */
void HeldActor_RaiseCell__dehkan_plateau(struct Actor *object) __asm__(
    "HeldActor_RaiseCell");
#define HeldActor_RaiseCell HeldActor_RaiseCell__dehkan_plateau

void DehkanPlateau_PlaceRaisedPillar(struct Actor *object) {
  s32 x;
  u8 *cell;

  HeldActor_RaiseCell(object);
  x = object->x >> 20;
  if (x == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Event_End();
  } else if (x == 18) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Actor_GetCellIndex(object) * 4;
    cell[3] &= 0x7f;
  } else if (x == 54) {
    cell = ((struct MapCells *)Ram_HeapSlots->mapWork)->cells;
    cell += Actor_GetCellIndex(object) * 4;
    cell[3] &= 0x7f;
    cell[2] = 11;
  } else if (x == 55) {
    MapGrid_CopyCellAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

extern s16 gDehkanPlateau_Room3RaisedPillars[];

void Scene_RunIfHeld(void) {
  struct Actor *object;

  if (Leader_CanPushObjectAhead() != 0) {
    object = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.currentOwner));
    Pillars_LowerHeldCells(gDehkanPlateau_Room3RaisedPillars);
    Leader_PushObjectAhead();
    DehkanPlateau_PlaceRaisedPillar(object);
  }
}

void DehkanPlateau_LiftRoom3RaisedPillar(void) {
  Pillars_LowerHeldCells(gDehkanPlateau_Room3RaisedPillars);
}

void DehkanPlateau_FinishMovingRaisedPillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *active;

  active = work->movedBlock;
  if (active != 0)
    DehkanPlateau_PlaceRaisedPillar(active);
}

// Stepping beside room 6's cone before it is Pounded holds it still and changes
// the cell in front of it; stepping away puts the cell back.
void DehkanPlateau_EnterConeCell(void) {
  s32 set = GameFlag_Get(FLAG_CONE_POUNDED);

  if (set == 0) {
    Actor_GetById(8)->motionFlags = set;
    MapGrid_CopyCellAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_Set(FLAG_IN_CONE_CELL);
  }
}

void DehkanPlateau_LeaveConeCell(void) {
  if (GameFlag_Get(FLAG_CONE_POUNDED) == 0) {
    MapGrid_CopyCellAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_Clear(FLAG_IN_CONE_CELL);
  }
}

void DehkanPlateau_JoltCamera(void) {
  struct Actor *object = FieldCamera_GetActor();
  u32 i;

  object->y += -px(8);
  wait(1);
  for (i = 0; i <= 7; i++) {
    object->y += px(1);
    wait(1);
  }
}
#undef SCENE

void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);

void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
/* VIEW(events_g): routine Actor_SetHorizontalPositionAndAngleWithTerrain as
 * void Actor_SetHorizontalPositionAndAngleWithTerrain(u32 id, s32 x, s32 z, s16
 * angle); event.h has void Actor_SetHorizontalPositionAndAngleWithTerrain(s32
 * actorId, s32 x, s32 z, s32 angle); tried as the definition's void
 * Actor_SetHorizontalPositionAndAngleWithTerrain(s32 actorId, s32 x, s32 z, s16
 * angle), it does not compile in tla-ja (dehkan_plateau.c):
 * games/tla/src/scene/dehkan_plateau.c:825: conflicting types for
 * `Actor_SetHorizontalPositionAndAngleWithTerrain'. */
extern void Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(
    u32 id, s32 x, s32 z,
    s16 angle) __asm__("Actor_SetHorizontalPositionAndAngleWithTerrain");
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_g): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * rest of the object declares it, DehkanPlateau_CannonCastsPound moves: 1 of 20
 * instructions in tla-ja (dehkan_plateau.c). */
extern void Task_Add__events_g(void (*func)(void),
                               s32 priority) __asm__("Task_Add");

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain__events_g(id, x, z, angle);
}

void DehkanPlateau_CannonFleesOverCracks(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(658 << 16, -1, 164 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM2_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM2_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Actor_SetSpeed(ACTOR_ROOM2_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_ROOM2_CANNON, 664, 136);
  MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  GameFlag_Set(FLAG_SCENE_TEMP_47);
  Actor_WalkToAndWait(ACTOR_ROOM2_CANNON, 664, 104);
  MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
  Sound_Play(SE_SPLASH);
  DehkanPlateau_JoltCamera();
  Event_Wait(10);
  Actor_Jump(ACTOR_ROOM2_CANNON, 6, 16);
  Actor_PlaceFacing(ACTOR_ROOM2_CANNON, 222 << 18, 168 << 16, FACING_SOUTH);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_FLED_OVER_CRACKS);
  Event_End();
}

void DehkanPlateau_CannonFleesEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(245 << 18, -1, 168 << 16, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM2_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM2_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Actor_GetById(ACTOR_ROOM2_CANNON)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_ROOM2_CANNON, 1);
  Actor_SetSpeed(ACTOR_ROOM2_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_ROOM2_CANNON, 940, 168);
  Actor_WalkToAndWait(ACTOR_ROOM2_CANNON, 984, 152);
  Actor_WalkToAndWait(ACTOR_ROOM2_CANNON, 1040, 152);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM2_CANNON, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_FLED_EAST);
  Event_End();
}

void DehkanPlateau_CannonLeapsAway(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveWithinMapBounds(144 << 16, -1, 214 << 15, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM3_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM3_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 1.6);
  Actor_WalkToAndWaitById(ACTOR_ROOM3_CANNON, 168, 104);
  Event_Wait(10);
  Sound_Play(SE_HIGH_JUMP);
  Actor_GetById(ACTOR_ROOM3_CANNON)->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_ROOM3_CANNON, 200, 104);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 1);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 294, 104);
  Actor_PlaceFacing(ACTOR_ROOM3_CANNON, 212 << 17, 208 << 15, FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_LEAPT_AWAY);
  Event_End();
}

void DehkanPlateau_CannonPoundsPillar(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveToFixed(212 << 17, -1, 208 << 15, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM3_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM3_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
  FieldCamera_MoveWithinMapBounds(212 << 17, -1, 152 << 16, 1);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 424, 152);
  Actor_ArmCallback(ACTOR_ROOM3_CANNON, FACING_EAST, 20);
  DehkanPlateau_CannonCastsPound();
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 440, 152);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 456, 168);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 456, 184);
  GameFlag_Set(FLAG_SCENE_TEMP_41);
  MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 456, 216);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 584, 216);
  Actor_PlaceFacing(ACTOR_ROOM3_CANNON, 154 << 18, 140 << 17, FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_POUNDED_PILLAR);
  Event_End();
}

void DehkanPlateau_SinkPoundedPillar(void) {
  struct EffectWork7c4 *work = Ram_HeapSlots->effectWork;
  struct Actor *object = Actor_GetById(ACTOR_ROOM3_POUND_PILLAR);

  if (work->timer != -1 && work->timer <= 7)
    object->scaleY += -0x1f00;
  if (work->active != 0) {
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(ACTOR_ROOM3_POUND_PILLAR, 4);
    object->priorityFlags |= 2;
    ((u8 *)object)[89] &= 0xfe;
    object->scaleY = Q16_ONE;
    GameFlag_Set(FLAG_ROOM3_PILLAR_POUNDED);
  }
}

void DehkanPlateau_CannonCastsPound(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_g(DehkanPlateau_SinkPoundedPillar, 0x480);
  FieldPsynergy_SetupActorPair(25, 18);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_SinkPoundedPillar);
  BattleEffect_CleanupSceneObjects();
}

#include "../field/field_effect/advance_motion.c"

#undef DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS
#undef SCENE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef HeldActor_RaiseCell
void Actor_DisableById(s32 id);

extern u32 gDehkanPlateau_DustScript[];

// Flashes actor id, then raises eight puffs over actor center, then sets
// flag and waits 20 frames.
void DehkanPlateau_CrumblePillar(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;

  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(2);
  Actor_SetModeById(id, 0);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(4);
  Actor_SetSpriteColor(Actor_GetById(id), 0);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *object;
    struct EffectView *effect;
    s32 y;

    /* FAKEMATCH(temporary): the form kept. */
    y = leader->y - ((((Random16() * 7) << 1) >> 16) << 16);
    object = ((struct Actor *)Import_ActorCreate(
        285, (i << 17) + leader->x - Q16_ONE * 8, y - Q16_ONE * 2, leader->z));
    if (object != NULL) {
      effect = (struct EffectView *)object;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Actor_SelectAnimation(object, 2);
      Actor_StartScript(object, gDehkanPlateau_DustScript);
      object->scaleX = 0xb333;
      object->scaleY = 0xb333;
      effect->sprite->priority = 1;
      effect->velocityX = 0;
      effect->velocityZ = 0;
      effect->velocityY = -((((u32)Random16() << 14) >> 16) + Q16_ONE * 3 / 8);
      effect->spin = (((u32)Random16() << 9) >> 16) - 256;
      effect->scaleRateX = -(Q16_ONE / 32);
      effect->scaleRateY = -(Q16_ONE * 3 / 128);
      effect->update = FieldEffect_AdvanceMotion;
    }
  }
  GameFlag_Set(flag);
  Event_Wait(20);
}

void DehkanPlateau_CannonCrumblesPillars(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_ROOM3_CANNON);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 25);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(616, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM3_CANNON, gMainRoutineWork.currentOwner, 0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Actor_ShowEmoteById(ACTOR_ROOM3_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Actor_SetSpritePriority(ACTOR_ROOM3_CANNON, 1);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 632, 280);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 664, 280);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 1);
  DehkanPlateau_CrumblePillar(21, 25, 0x210);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 696, 280);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 1);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 712, 280);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveWithinMapBounds(px(712), -1, px(136), 1);
  MapGrid_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(ACTOR_ROOM3_CANNON, 1);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 712, 200);
  Actor_WalkToAndWaitById(ACTOR_ROOM3_CANNON, 712, 168);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Actor_FaceDirectionById(ACTOR_ROOM3_CANNON, FACING_EAST);
  Actor_SetSpeed(ACTOR_ROOM3_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 744, 168);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 1);
  DehkanPlateau_CrumblePillar(24, 25, 0x216);
  Actor_FaceDirectionById(ACTOR_ROOM3_CANNON, FACING_NORTH);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 744, 136);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM3_CANNON), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_ROOM3_CANNON, 744, 72);
  MapGrid_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(SE_SPLASH);
  DehkanPlateau_JoltCamera();
  Event_Wait(10);
  Actor_Jump(ACTOR_ROOM3_CANNON, 6, 16);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM3_CANNON, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_CRUMBLED_PILLARS);
  Event_End();
}

void DehkanPlateau_CannonPoundsLeaderThrough(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  {
    s32 facing = FACING_NORTH;

    Actor_SetHorizontalPositionAndAngleWithTerrain(
        gMainRoutineWork.currentOwner, px(248), px(136), facing);
  }
  MapGrid_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_Set(FLAG_SCENE_TEMP_45);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  Actor_FaceActorById(ACTOR_ROOM5_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM5_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      20);
  DehkanPlateau_CannonCastsPoundOnLeader();
  Event_Wait(20);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_ROOM5_CANNON, 3);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(SE_DUST_PUFF);
    leader->scaleX -= 0x400;
    leader->scaleY -= 0x300;
    leader->y -= 0x9999;
    wait(1);
  }
  gFieldWork->transition = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}

void DehkanPlateau_PoundLeaderThrough(void) {
  if (((struct EffectWork7c4 *)Ram_HeapSlots->effectWork)->active != 0) {
    Sound_Play(SE_SPLASH);
    MapGrid_CopyCellPictureIndicesRect(64, 64, 15, 8, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(64, 66, 15, 72, 1, 2);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
}

extern s16 gDehkanPlateau_Room2CrumblingPillars[];
extern s16 gDehkanPlateau_Room3CrumblingPillars[];
extern s16 gDehkanPlateau_Room6CrumblingPillar[];
extern s16 gDehkanPlateau_Room1PoundPillars[];
extern s16 gDehkanPlateau_Room2PoundPillars[];
extern s16 gDehkanPlateau_Room5PoundPillars[];
extern s16 gDehkanPlateau_Room6PoundPillars[];
extern s16 gDehkanPlateau_Room2MovablePillars[];
extern s16 gDehkanPlateau_Room3MovablePillars[];

void DehkanPlateau_LeaveConeCell(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void DehkanPlateau_ForgetFall(void);

void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);

s32 PartyInventory_GiveItem(s32 item, s32 unused);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_WalkToAndWaitById(s32 id, s32 x, s32 z);
void Actor_Jump(s32 id, s32 a, s32 b);
void Actor_WatchAndSetScript(s32 id, s32 target);
/* VIEW(events_i): routine Task_Add as void Task_Add(void(*func) (void), s32
 * priority); task.h has s32 Task_Add(TaskFunc func, s32 order); tried as the
 * rest of the object declares it, DehkanPlateau_CannonCastsPoundOnLeader moves:
 * 1 of 24 instructions in tla-ja (dehkan_plateau.c). */
extern void Task_Add__events_i(void (*func)(void),
                               s32 priority) __asm__("Task_Add");

void DehkanPlateau_CannonCastsPoundOnLeader(void) {
  BattleEffect_LoadActionEffectResources(134, 1);
  Task_Add__events_i(DehkanPlateau_PoundLeaderThrough, 0x480);
  FieldPsynergy_SetupActorPair(9, gMainRoutineWork.currentOwner);
  EventObject_Initialize();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_PoundLeaderThrough);
  BattleEffect_CleanupSceneObjects();
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);

void DehkanPlateau_CannonDropsPoundCube(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *helper = Actor_GetById(ACTOR_ROOM5_CANNON);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = 3;
  Event_Wait(24);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  DehkanPlateau_JoltCamera();
  if (gDehkanPlateau_PoundCube != NULL) {
    FieldActor_SetPositionAndResetMotion(gDehkanPlateau_PoundCube, px(248), 0,
                                         px(120));
    wait(1);
    gDehkanPlateau_PoundCube->speedLimit = 0x19999;
    gDehkanPlateau_PoundCube->acceleration = 0x19999;
    gDehkanPlateau_PoundCube->velocityY = px(8);
  }
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 1.6);
  Actor_SetSpeed(ACTOR_ROOM5_CANNON, 1.6);
  leader->velocityY = px(6);
  helper->velocityY = px(6);
  FieldActor_SetMoveTarget(leader, 216, 0, 120);
  FieldActor_SetMoveTarget(helper, 280, 0, 120);
  if (gDehkanPlateau_PoundCube != NULL)
    FieldActor_SetMoveTarget(gDehkanPlateau_PoundCube, 248, 0, 88);
  Actor_WaitForMove(leader);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_ROOM5_CANNON, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_ArmCallback(gMainRoutineWork.currentOwner, 0, 0);
  Actor_ArmCallback(ACTOR_ROOM5_CANNON, 0x8000, 60);
  Actor_WalkToAndWait(ACTOR_ROOM5_CANNON, 296, 104);
  Actor_WalkToAndWait(ACTOR_ROOM5_CANNON, 296, 80);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM5_CANNON, 0, 0);
  Event_Wait(20);
  if (gDehkanPlateau_PoundCube != NULL) {
    gDehkanPlateau_PoundCube->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_Set(FLAG_CANNON_DROPPED_POUND_CUBE);
  Event_End();
}

void DehkanPlateau_CannonCrumblesLastPillar(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_ROOM6_CANNON);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.currentOwner, 16);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveWithinMapBounds(px(760), px(32), px(204), 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_ROOM6_CANNON, gMainRoutineWork.currentOwner, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(ACTOR_ROOM6_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_ROOM6_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_ROOM6_CANNON, 760, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 1);
  Event_Wait(20);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_ROOM6_CANNON, 792, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 1);
  DehkanPlateau_CrumblePillar(9, 16, 0x210);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = px(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_ROOM6_CANNON, 824, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ROOM6_CANNON), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(ACTOR_ROOM6_CANNON, gMainRoutineWork.currentOwner);
  Actor_Stop(gMainRoutineWork.currentOwner);
  Event_Wait(20);
  Event_End();
}

void DehkanPlateau_CannonCornered(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(ACTOR_ROOM6_CANNON, gMainRoutineWork.currentOwner, 0);
  Actor_ShowEmoteById(ACTOR_ROOM6_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Actor_FaceDirectionById(ACTOR_ROOM6_CANNON, FACING_EAST);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(16, 3);
  Actor_FaceDirectionAndWait(ACTOR_ROOM6_CANNON, FACING_WEST);
  Actor_WatchAndSetScript(ACTOR_ROOM6_CANNON, gMainRoutineWork.currentOwner);
  FieldEffect_SetAttached(16, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_End();
}

void DehkanPlateau_MeetCannon(void) {
  if (GameFlag_Get(FLAG_CONE_POUNDED) != 0)
    DehkanPlateau_CannonCornered();
  else
    DehkanPlateau_CannonCrumblesLastPillar();
  GameFlag_Set(FLAG_CANNON_MET);
}

void DehkanPlateau_TakePoundCube(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  if (gDehkanPlateau_PoundCube != 0)
    ItemDisplay_ShowPicture(gDehkanPlateau_PoundCube, 3);
  PartyInventory_GiveItem(ITEM_POUND_CUBE, 0);
  if (gDehkanPlateau_PoundCube != 0)
    Actor_Release(gDehkanPlateau_PoundCube);
  MapGrid_CopyCellAttributes(1, 0, 1, 1, 15, 5);
  GameFlag_Set(FLAG_POUND_CUBE_TAKEN);
  Event_End();
}

void DehkanPlateau_LeaveRoom(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_DehkanPlateau1)
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 3);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_DehkanPlateau1)
    DehkanPlateau_InitializeRoom1();
  else if (scene == (s32)&SceneId_DehkanPlateau2)
    DehkanPlateau_InitializeRoom2();
  else if (scene == (s32)&SceneId_DehkanPlateau3)
    DehkanPlateau_InitializeRoom3();
  else if (scene == (s32)&SceneId_DehkanPlateau4)
    DehkanPlateau_InitializeRoom4();
  else if (scene == (s32)&SceneId_DehkanPlateau5)
    DehkanPlateau_InitializeRoom5();
  else if (scene == (s32)&SceneId_DehkanPlateau6)
    DehkanPlateau_InitializeRoom6();
  return 0;
}

// Room 1's task: draws object 64 at the leader sprite's priority each frame.
// XXX: which object 64 is in room 1; staging room 1 and reading the object
// table would settle it.
void DehkanPlateau_KeepPriorityWithLeader(void) {
  Actor_SetSpritePriority(64, ((struct AnimationFacing *)Actor_GetById(
                                   gMainRoutineWork.currentOwner)
                                   ->sprite)
                                  ->facing);
}

void DehkanPlateau_RestoreRoom1Cracks(void);

void DehkanPlateau_InitializeRoom1(void) {
  CrumblingPillars_Place(gDehkanPlateau_Room1CrumblingPillars);
  Actor_SetSpritePriority(8, 3);
  Pillars_Place(gDehkanPlateau_Room1PoundPillars);
  Actor_DisableById(ACTOR_ROOM1_PILLAR);
  if (GameFlag_Get(FLAG_AREA_TEMP_01) != 0) {
    MapGrid_CopyCellAttributes(54, 32, 2, 1, 56, 64);
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_ROOM1_PILLAR),
                                    226 << 18, 128 << 15, 130 << 18);
    wait(10);
  }
  DehkanPlateau_RestoreRoom1Cracks();
  DehkanPlateau_HideRoom1Actors();
  {
    s32 priority = 0x480;

    Task_Add(DehkanPlateau_KeepPriorityWithLeader, priority);
  }
}

void DehkanPlateau_RestoreRoom2Cracks(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z,
                                                    s32 angle);

void DehkanPlateau_InitializeRoom2(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    if (gMainRoutineWork.entrance == 5) {
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    }
  }
  CrumblingPillars_PlaceWithStandIns(gDehkanPlateau_Room2CrumblingPillars);
  DehkanPlateau_SetRoom2StandingPillarCells();
  Pillars_Place(gDehkanPlateau_Room2PoundPillars);
  if (GameFlag_Get(FLAG_ROOM2_PILLAR_PLACED) != 0) {
    Actor_DisableById(ACTOR_ROOM2_PILLAR);
    MapGrid_CopyCellAttributes(50, 7, 1, 1, 52, 7);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_ROOM2_PILLAR),
                                         210 << 18, 0, 240 << 15);
  } else {
    Actor_PlaceBlocks(gDehkanPlateau_Room2MovablePillars);
  }
  DehkanPlateau_RestoreRoom2Cracks();
  if (GameFlag_Get(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
    s32 angle = 0x4000;

    Actor_PlaceFacing(ACTOR_ROOM2_CANNON, 222 << 18, 168 << 16, angle);
    MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
    GameFlag_Set(FLAG_SCENE_TEMP_47);
  }
  if (GameFlag_Get(FLAG_CANNON_FLED_EAST) != 0)
    Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM2_CANNON, 0, 0);
  DehkanPlateau_HideRoom2Actors();
}

extern s16 gDehkanPlateau_Room3RaisedPillars[];
extern s32 gDehkanPlateau_CrackZ;

void DehkanPlateau_ForgetCrackOnLeaving(void);
/* VIEW(s3284): routine Actor_GetCellIndex as s32 Actor_GetCellIndex(void);
 * s2c14 has s32 Actor_GetCellIndex(struct Actor *object); tried as the
 * definition's s32 Actor_GetCellIndex(struct Actor *object), which the object
 * already declares, it does not compile in tla-ja (dehkan_plateau.c):
 * games/tla/src/scene/dehkan_plateau.c:1490: too few arguments to function
 * `Actor_GetCellIndex'. */
extern s32 Actor_GetCellIndex__room3(void) __asm__("Actor_GetCellIndex");

// The room entry written in the scene layer of DehkanPlateau_InitializeRoom2
// and DehkanPlateau_InitializeRoom5: sets actors' variants, links, cells and
// flags for the room.
void DehkanPlateau_InitializeRoom3(void) {
  struct Actor *object;
  s32 zero, value;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
  }
  Actor_SetSpritePriority(8, 1);
  Actor_SetSpritePriority(9, 1);
  Actor_SetSpritePriority(10, 1);
  Actor_SetSpritePriority(11, 1);
  /* FAKEMATCH(dead): an empty once-only block ends the argument loads' tie. */
  do {
  } while (0);
  Tightrope_Initialize();
  Tightrope_Place(0, 8, 9);
  Tightrope_Place(1, 10, 11);
  Actor_SetSpritePriority(12, 1);
  Actor_SetSpritePriority(13, 1);
  DehkanPlateau_InitializeRopes();
  Rope_Place(0, 520, 12, 13);
  Actor_PlaceBlocks(gDehkanPlateau_Room3MovablePillars);
  if (Actor_GetById(ACTOR_ROOM3_PILLAR)->y > (s32)0xfff80000)
    Actor_SetSpritePriority(ACTOR_ROOM3_PILLAR, 1);
  else
    Actor_SetSpritePriority(ACTOR_ROOM3_PILLAR, 2);
  Actor_DisableById(ACTOR_ROOM3_SECOND_PILLAR);
  Actor_GetById(ACTOR_ROOM3_SECOND_PILLAR)->priorityFlags |= 32;
  Actor_SetSpritePriority(ACTOR_ROOM3_SECOND_PILLAR, 2);
  if (GameFlag_Get(FLAG_ROOM3_SECOND_PILLAR_PLACED) != 0)
    DehkanPlateau_SetRoom3SecondPillarInPlace();
  else
    MapGrid_CopyCellAttributes(38, 32, 1, 1, 38, 33);
  Actor_GetById(14)->priorityFlags &= ~1;
  Actor_GetById(15)->priorityFlags &= ~1;
  Actor_GetById(16)->priorityFlags &= ~1;
  Actor_GetById(17)->priorityFlags &= ~1;
  Pillars_Raise(gDehkanPlateau_Room3RaisedPillars);
  value = Actor_GetById(17)->x;
  zero = 0;
  if ((value >> 20) == 54) {
    struct MapCell *cell = Ram_HeapSlots->mapWork->layers[0].cells;

    cell += Actor_GetCellIndex__room3();
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  }
  Actor_SetSpritePriority(14, 1);
  Actor_SetSpritePriority(15, 1);
  DehkanPlateau_RestoreRoom3Cracks();
  if (GameFlag_Get(FLAG_CANNON_LEAPT_AWAY) != 0) {
    s32 angle = 0x4000;

    Actor_SetPositionAndFacing(ACTOR_ROOM3_CANNON, 212 << 17, 208 << 15, angle);
  }
  Actor_SetModeById(ACTOR_ROOM3_POUND_PILLAR, 3);
  if (GameFlag_Get(FLAG_CANNON_POUNDED_PILLAR) != 0) {
    object = Actor_GetById(ACTOR_ROOM3_POUND_PILLAR);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetModeById(ACTOR_ROOM3_POUND_PILLAR, 4);
    object->priorityFlags |= 2;
    *((u8 *)object + 89) &= ~1;
    GameFlag_Set(FLAG_ROOM3_PILLAR_POUNDED);
    GameFlag_Set(FLAG_SCENE_TEMP_41);
    MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(ACTOR_ROOM3_CANNON, 154 << 18, 140 << 17,
                                 angle);
    }
  }
  if (GameFlag_Get(FLAG_CANNON_CRUMBLED_PILLARS) != 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_10);
    GameFlag_Set(FLAG_SCENE_TEMP_16);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM3_CANNON, 0, 0);
  }
  CrumblingPillars_Place(gDehkanPlateau_Room3CrumblingPillars);
  DehkanPlateau_SetRoom3StandingPillarCells();
  DehkanPlateau_HideRoom3Actors();
  {
    struct Actor *last = Actor_GetById(27);

    *((u8 *)last + 85) = zero;
    last->y = zero;
  }
  gDehkanPlateau_CrackX = zero;
  gDehkanPlateau_CrackZ = zero;
  Task_Add(DehkanPlateau_ForgetCrackOnLeaving, TASK_PASS_VBLANK);
}

extern u16 gDehkanPlateau_FallLandingCells[];

#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, n1, n2, n3, n4, n5, n6)      \
  void name(void) {                                                            \
    /* FAKEMATCH(pin): flag in r5, which leaves r6 to the entry. */            \
    register s32 flag asm("r5") = FLAG_FELL_THROUGH_FIRST;                     \
    s32 offset = 0;                                                            \
                                                                               \
    while (1) {                                                                \
      if (GameFlag_Get(flag)) {                                                \
        u16 *entry = (u16 *)((u8 *)gDehkanPlateau_FallLandingCells + offset);  \
        s32 y;                                                                 \
        s32 x;                                                                 \
        s32 one;                                                               \
                                                                               \
        flag -= FLAG_FELL_THROUGH_FIRST;                                       \
        x = *entry++;                                                          \
        y = *entry++;                                                          \
        one = 1;                                                               \
        MapGrid_CopyCellsTo(n1, n2, x, y, one, one);                           \
        x = entry[0];                                                          \
        y = entry[1];                                                          \
        MapGrid_CopyCellsTo(n3, n4, x, y, one, 5);                             \
        if (flag == 1)                                                         \
          MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);                         \
        x -= 64;                                                               \
        y += n5;                                                               \
        MapGrid_CopyCellsTo(n1, n6, x, y, one, 4);                             \
        return;                                                                \
      }                                                                        \
      if (flag == FLAG_FELL_THROUGH_LAST)                                      \
        return;                                                                \
      offset += 8;                                                             \
      flag++;                                                                  \
    }                                                                          \
  }

// Draws, in rooms 4 and 5, the cells where the party landed under the crack it
// fell through.
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawRoom4FallLanding, 3, 8,
                                    67, 3, 66, 69)

DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawRoom5FallLanding, 13, 5,
                                    77, 0, 65, 65)

void DehkanPlateau_InitializeRoom4(void) {
  gFieldWork->transition = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  DehkanPlateau_HideRoom4Actors();
  if (GameFlag_Get(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
    MapGrid_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    MapGrid_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - FIRST_LANDING_ENTRANCE) <= 28) {
      DehkanPlateau_ForgetFall();
      entrance <<= 16;
      entrance >>= 16;
      entrance += FLAG_FELL_THROUGH_FIRST - FIRST_LANDING_ENTRANCE;
      GameFlag_Set(entrance);
    }
  }
  DehkanPlateau_DrawRoom4FallLanding();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        DehkanPlateau_LandFromFall();
    }
  }
}

// The leader drops in from the crack above, landing in a ring of dust.
void DehkanPlateau_LandFromFall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  struct EffectOptions *opts;
  s32 spawn;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Map_CenterOnFollowedPoint();
  wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = 3;
  Event_Wait(24);
  opts = &options;
  opts->palette = 7;
  opts->update = FieldEffect_AdvanceMotion;
  opts->startScaleX = 0xa3d7;
  opts->startScaleY = 0xa3d7;
  opts = &options;
  spawn = 1;
  i = 0;
loop:
  {
    s32 angle;
    s32 velocity[3];
    s32 cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Trig_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += ((Random16() * 3) << 14) >> 16;
      velocity[0] += -0x6000;
      velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
      FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0],
                        velocity[1], velocity[2], 0x01090001, opts);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  Event_Wait(10);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 1);
  gFieldWork->transition = 0x204;
  Event_End();
}

void DehkanPlateau_InitializeRoom5(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 entrance;

  gFieldWork->transition = 0x204;
  owner->priorityFlags |= 32;
  if (GameFlag_Get(FLAG_POUND_CUBE_TAKEN) == 0) {
    DehkanPlateau_CreatePoundCube();
    if (GameFlag_Get(FLAG_CANNON_DROPPED_POUND_CUBE) != 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
      if (gDehkanPlateau_PoundCube != NULL) {
        FieldActor_SetPositionAndResetMotion(gDehkanPlateau_PoundCube,
                                             248 << 16, 0, 176 << 15);
        wait(1);
      }
    }
  }
  Pillars_Place(gDehkanPlateau_Room5PoundPillars);
  DehkanPlateau_RestoreRoom5Cracks();
  entrance = gMainRoutineWork.entrance;
  if (entrance > 10) {
    DehkanPlateau_ForgetFall();
    GameFlag_Set(entrance + FLAG_FELL_THROUGH_FIRST - FIRST_LANDING_ENTRANCE);
  }
  if (Flag_Test(FLAG_CANNON_DROPPED_POUND_CUBE) != 0)
    Actor_SetHorizontalPositionWithTerrain(ACTOR_ROOM5_CANNON, 0, 0);
  DehkanPlateau_DrawRoom5FallLanding();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 16) {
      if (gMainRoutineWork.entrance == 37 &&
          GameFlag_Get(FLAG_CANNON_DROPPED_POUND_CUBE) == 0)
        DehkanPlateau_CannonDropsPoundCube();
      else
        DehkanPlateau_LandFromFall();
    }
  }
  gDehkanPlateau_CrackX = 0;
  gDehkanPlateau_CrackZ = 0;
  Task_Add(DehkanPlateau_ForgetCrackOnLeaving, TASK_PASS_VBLANK);
}

// Creates the Pound Cube Cannon drops, drawn with the item's own icon.
void DehkanPlateau_CreatePoundCube(void) {
  struct Actor **slot = &gDehkanPlateau_PoundCube;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(489, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(68, 0x608);
    Icon_DrawItem(ITEM_POUND_CUBE);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void DehkanPlateau_InitializeRoom6(void) {
  gFieldWork->transition = 0x204;
  Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_CANNON)) != 0)
    GameFlag_Set(FLAG_CANNON_MET);
  Pillars_Place(gDehkanPlateau_Room6PoundPillars);
  CrumblingPillars_Place(gDehkanPlateau_Room6CrumblingPillar);
  if (GameFlag_Get(FLAG_CONE_POUNDED) == 0) {
    DehkanPlateau_EnterConeCell();
    wait(10);
  }
  DehkanPlateau_HideRoom6Actors();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        DehkanPlateau_LandFromFall();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#undef DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG

void DehkanPlateau_PlaceRoom3Pillar(void);
void DehkanPlateau_MoveRoom3SecondPillar(void);
void DehkanPlateau_LeaveConeCell(void);
void DehkanPlateau_CannonFleesEast(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gRubble_SprayScript[] = {
    SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

u32 gRubble_BurstScript[] = {
    SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_FREE,
};

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

u32 gDehkanPlateau_DustScript[] = {
    0, 33, 22, 38, 0, 3,
    0, 1, 38,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s16 gDehkanPlateau_Room1CrumblingPillars[] = {
    8, 528, 9, 530, 10, 532, -1,
};

s16 gDehkanPlateau_Room2CrumblingPillars[] = {
    11, 528, 12, 530, 13, 532, 14, 534,
    15, 536, 16, 538, 17, 540, 18, 542,
    19, 544, 20, 546, 21, 548, -1,
};

s16 gDehkanPlateau_Room3CrumblingPillars[] = {
    21, 528, 22, 530, 23, 532, 24, 534,
    -1,
};

s16 gDehkanPlateau_Room6CrumblingPillar[] = {
    9, 528, -1,
};

s16 gDehkanPlateau_Room1PoundPillars[] = {
    11, 1, 515, -1,
};

s16 gDehkanPlateau_Room2PoundPillars[] = {
    22, 1, 516, -1,
};

s16 gDehkanPlateau_Room3PoundPillars[] = {
    18, 1, 517, -1,
};

s16 gDehkanPlateau_Room5PoundPillars[] = {
    8, 1, 518, -1,
};

s16 gDehkanPlateau_Room6PoundPillars[] = {
    8, 1, 519, -1,
};

s16 gDehkanPlateau_Room2MovablePillars[] = {
    23, -1,
};

s16 gDehkanPlateau_Room3MovablePillars[] = {
    19, -1,
};

s16 gDehkanPlateau_Room3RaisedPillars[] = {
    14, 15, 16, 17, -1,
};

u16 gDehkanPlateau_Room1CrackPictures[] = {
    4, 33, 5, 33,
};

u16 gDehkanPlateau_Room2CrackPictures[] = {
    68, 33, 69, 33,
};

u16 gDehkanPlateau_Room3CrackPictures[] = {
    69, 51, 70, 51,
};

u16 gDehkanPlateau_Room5CrackPictures[] = {
    65, 64, 64, 64,
};

u16 gDehkanPlateau_Room1HolePictures[] = {
    1, 36, 2, 36, 3, 36, 4, 36,
    5, 36, 6, 36, 7, 36, 8, 36,
};

u16 gDehkanPlateau_Room2HolePictures[] = {
    65, 36, 66, 36, 67, 36, 68, 36,
    69, 36, 70, 36, 71, 36, 72, 36,
};

u16 gDehkanPlateau_Room3HolePictures[] = {
    66, 54, 67, 54, 68, 54, 69, 54,
    70, 54, 71, 54, 72, 54, 73, 54,
};

u16 gDehkanPlateau_Room5HolePictures[] = {
    64, 66, 65, 66, 66, 66, 67, 66,
    68, 66,
};

u16 gDehkanPlateau_Room1Cracks[][6] = {
    {0, 76, 12, 0, 76, 48},
    {0, 77, 13, 0, 77, 49},
    {1, 79, 12, 0, 79, 48},
    {0, 79, 14, 1, 79, 50},
    {0, 81, 12, 0, 81, 48},
    {0, 118, 17, 4, 118, 53},
    {1, 119, 16, 0, 119, 52},
    {0, 119, 18, 7, 119, 54},
    {1, 120, 12, 7, 120, 48},
    {0, 120, 14, 7, 120, 50},
    {0, 120, 17, 2, 120, 53},
    {0, 122, 12, 7, 122, 48},
    {1, 122, 16, 0, 122, 52},
    {0, 122, 18, 5, 122, 54},
};

u16 gDehkanPlateau_Room2Cracks[][6] = {
    {1, 105, 4, 0, 41, 36},
    {0, 109, 17, 0, 45, 49},
    {0, 110, 19, 3, 46, 51},
    {0, 111, 17, 3, 47, 49},
    {0, 112, 21, 0, 48, 53},
    {0, 113, 20, 3, 49, 52},
    {0, 114, 18, 0, 50, 50},
    {0, 105, 6, 0, 41, 38},
};

u16 gDehkanPlateau_Room3Cracks[][6] = {
    {1, 91, 11, 0, 27, 43},
    {0, 92, 11, 0, 28, 43},
    {1, 93, 9, 0, 29, 41},
    {1, 93, 12, 0, 29, 44},
    {1, 109, 7, 0, 45, 39},
    {1, 112, 6, 0, 48, 38},
};

u16 gDehkanPlateau_Room5Cracks[][6] = {
    {1, 13, 8, 2, 13, 72},
    {0, 13, 9, 0, 13, 73},
    {0, 13, 11, 1, 13, 75},
    {0, 13, 6, 0, 13, 70},
    {1, 15, 6, 0, 15, 70},
    {0, 15, 8, 0, 15, 72},
    {0, 15, 10, 1, 15, 74},
    {1, 14, 10, 0, 14, 74},
    {0, 14, 12, 0, 14, 76},
    {1, 15, 12, 0, 15, 76},
    {0, 16, 6, 0, 16, 70},
    {1, 16, 8, 2, 16, 72},
    {0, 16, 9, 0, 16, 73},
    {0, 16, 11, 0, 16, 75},
    {1, 18, 9, 0, 18, 73},
    {0, 18, 11, 0, 18, 75},
};

u16 gDehkanPlateau_FallLandingCells[] = {
    4, 7, 68, 2, 5, 8, 69, 3,
    7, 7, 71, 2, 7, 9, 71, 4,
    9, 7, 73, 2, 40, 12, 104, 7,
    41, 11, 105, 6, 41, 13, 105, 8,
    42, 5, 106, 0, 42, 9, 106, 4,
    42, 12, 106, 7, 44, 7, 108, 2,
    44, 11, 108, 6, 44, 13, 108, 8,
    11, 23, 75, 18, 12, 25, 76, 20,
    13, 23, 77, 18, 14, 27, 78, 22,
    15, 26, 79, 21, 16, 24, 80, 19,
    30, 35, 94, 30, 26, 9, 90, 4,
    27, 9, 91, 4, 28, 7, 92, 2,
    28, 10, 92, 5, 12, 8, 76, 3,
    15, 7, 79, 2, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 30, 33, 94, 28,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
};

struct Actor *gDehkanPlateau_PoundCube = NULL;

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 752, 0, 348, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(DehkanPlateau1),
    SCENE_EXIT(1, WorldMap, 6),
    SCENE_EXIT(2, DehkanPlateau4, 1),
    SCENE_EXIT(3, DehkanPlateau4, 2),
    SCENE_EXIT(4, DehkanPlateau4, 4),
    SCENE_EXIT(5, DehkanPlateau2, 1),
    SCENE_EXIT(6, DehkanPlateau1, 7),
    SCENE_EXIT(7, DehkanPlateau1, 6),
    SCENE_EXIT(11, DehkanPlateau4, 11),
    SCENE_EXIT(12, DehkanPlateau4, 12),
    SCENE_EXIT(13, DehkanPlateau4, 13),
    SCENE_EXIT(14, DehkanPlateau4, 14),
    SCENE_EXIT(15, DehkanPlateau4, 15),
    SCENE_EXIT(16, DehkanPlateau4, 16),
    SCENE_EXIT(17, DehkanPlateau4, 17),
    SCENE_EXIT(18, DehkanPlateau4, 18),
    SCENE_EXIT(19, DehkanPlateau4, 19),
    SCENE_EXIT(20, DehkanPlateau4, 20),
    SCENE_EXIT(21, DehkanPlateau4, 21),
    SCENE_EXIT(22, DehkanPlateau4, 22),
    SCENE_EXIT(23, DehkanPlateau4, 23),
    SCENE_EXIT(24, DehkanPlateau4, 24),
    SCENE_EXITS(DehkanPlateau2),
    SCENE_EXIT(1, DehkanPlateau1, 5),
    SCENE_EXIT(2, DehkanPlateau4, 6),
    SCENE_EXIT(3, DehkanPlateau4, 5),
    SCENE_EXIT(4, DehkanPlateau3, 1),
    SCENE_EXIT(5, DehkanPlateau2, 6),
    SCENE_EXIT(6, DehkanPlateau2, 5),
    SCENE_EXIT(15, DehkanPlateau4, 25),
    SCENE_EXIT(16, DehkanPlateau4, 26),
    SCENE_EXIT(17, DehkanPlateau4, 27),
    SCENE_EXIT(18, DehkanPlateau4, 28),
    SCENE_EXIT(19, DehkanPlateau4, 29),
    SCENE_EXIT(20, DehkanPlateau4, 30),
    SCENE_EXIT(21, DehkanPlateau4, 31),
    SCENE_EXIT(14, DehkanPlateau4, 60),
    SCENE_EXITS(DehkanPlateau3),
    SCENE_EXIT(1, DehkanPlateau2, 4),
    SCENE_EXIT(2, DehkanPlateau4, 3),
    SCENE_EXIT(3, DehkanPlateau5, 1),
    SCENE_EXIT(4, DehkanPlateau5, 4),
    SCENE_EXIT(5, DehkanPlateau5, 5),
    SCENE_EXIT(6, WorldMap, 7),
    SCENE_EXIT(7, DehkanPlateau3, 8),
    SCENE_EXIT(8, DehkanPlateau3, 7),
    SCENE_EXIT(12, DehkanPlateau4, 32),
    SCENE_EXIT(13, DehkanPlateau4, 33),
    SCENE_EXIT(14, DehkanPlateau4, 34),
    SCENE_EXIT(15, DehkanPlateau4, 35),
    SCENE_EXIT(16, DehkanPlateau5, 36),
    SCENE_EXIT(17, DehkanPlateau5, 37),
    SCENE_EXITS(DehkanPlateau4),
    SCENE_EXIT(1, DehkanPlateau1, 2),
    SCENE_EXIT(2, DehkanPlateau1, 3),
    SCENE_EXIT(3, DehkanPlateau3, 2),
    SCENE_EXIT(4, DehkanPlateau1, 4),
    SCENE_EXIT(5, DehkanPlateau2, 3),
    SCENE_EXIT(6, DehkanPlateau2, 2),
    SCENE_EXIT(40, DehkanPlateau4, 42),
    SCENE_EXIT(41, DehkanPlateau4, 43),
    SCENE_EXIT(42, DehkanPlateau4, 40),
    SCENE_EXIT(43, DehkanPlateau4, 41),
    SCENE_EXITS(DehkanPlateau5),
    SCENE_EXIT(1, DehkanPlateau3, 3),
    SCENE_EXIT(2, DehkanPlateau6, 1),
    SCENE_EXIT(3, DehkanPlateau6, 2),
    SCENE_EXIT(4, DehkanPlateau3, 4),
    SCENE_EXIT(5, DehkanPlateau3, 5),
    SCENE_EXIT(6, DehkanPlateau6, 3),
    SCENE_EXIT(7, DehkanPlateau5, 10),
    SCENE_EXIT(8, DehkanPlateau5, 9),
    SCENE_EXIT(9, DehkanPlateau5, 8),
    SCENE_EXIT(10, DehkanPlateau5, 7),
    SCENE_EXIT(18, DehkanPlateau6, 8),
    SCENE_EXIT(19, DehkanPlateau6, 9),
    SCENE_EXIT(20, DehkanPlateau6, 10),
    SCENE_EXIT(21, DehkanPlateau6, 11),
    SCENE_EXIT(22, DehkanPlateau6, 12),
    SCENE_EXIT(23, DehkanPlateau6, 13),
    SCENE_EXIT(24, DehkanPlateau6, 14),
    SCENE_EXIT(25, DehkanPlateau6, 15),
    SCENE_EXIT(26, DehkanPlateau6, 16),
    SCENE_EXIT(27, DehkanPlateau6, 17),
    SCENE_EXIT(28, DehkanPlateau6, 18),
    SCENE_EXIT(29, DehkanPlateau6, 19),
    SCENE_EXIT(30, DehkanPlateau6, 20),
    SCENE_EXIT(31, DehkanPlateau6, 21),
    SCENE_EXIT(32, DehkanPlateau6, 22),
    SCENE_EXIT(33, DehkanPlateau6, 23),
    SCENE_EXITS(DehkanPlateau6),
    SCENE_EXIT(1, DehkanPlateau5, 2),
    SCENE_EXIT(2, DehkanPlateau5, 3),
    SCENE_EXIT(3, DehkanPlateau5, 6),
    SCENE_EXIT(40, DehkanPlateau6, 41),
    SCENE_EXIT(41, DehkanPlateau6, 40),
    SCENE_EXIT(42, DehkanPlateau6, 43),
    SCENE_EXIT(43, DehkanPlateau6, 42),
    SCENE_EXITS_END,
};

struct ActorPlacement gDehkanPlateau_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room1Placements[] = {
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room2Placements[] = {
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {409, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 3},
    {406, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room3Placements[] = {
    {407, CONDITION_ALWAYS, 1, 0x1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x2, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x3, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x4, 0, 0, FACING_SOUTH, 2, 3},
    {407, CONDITION_ALWAYS, 1, 0x5, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 0x6, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0x7, 0, 0, FACING_SOUTH, 2, 2},
    {250, CONDITION_ALWAYS, 1, 0x8, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0x9, 0, 0, FACING_SOUTH, 2, 3},
    {250, CONDITION_ALWAYS, 1, 0xa, 0, 0, FACING_SOUTH, 2, 3},
    {406, CONDITION_ALWAYS, 1, 0xb, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xc, 0, 0, FACING_SOUTH, 2, 2},
    {272, CONDITION_ALWAYS, 1, 0xd, 0, 0, FACING_SOUTH, 2, 3},
    {409, CONDITION_ALWAYS, 1, 0xe, 0, 0, FACING_SOUTH, 2, 2},
    {409, CONDITION_ALWAYS, 1, 0xf, 0, 0, FACING_SOUTH, 2, 2},
    {409, CONDITION_ALWAYS, 1, 0x10, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room4Placements[] = {
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room5Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_Room6Placements[] = {
    ACTOR_PLACEMENT(406, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 1),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CANNON), MOVEMENT_TYPE_STAND, 728, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gDehkanPlateau_Triggers[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(DehkanPlateau_LeaveRoom)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom1Pillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 11, FLAG_ROOM1_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundRoom1Pillar)},
    {TRIGGER_PUSHED, 0, FLAG_AREA_TEMP_01, EVENT(DehkanPlateau_DropRoom1PillarIntoGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_AREA_TEMP_01, EVENT(DehkanPlateau_DropRoom1PillarIntoGap)},
    {TRIGGER_STEP, 6, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOffCrack)},
    {TRIGGER_STEP, 11, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 12, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 13, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 14, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 15, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 16, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 17, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 18, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 19, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 20, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 21, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 22, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 23, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 24, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(DehkanPlateau_LeaveRoom)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 30, FLAG_CANNON_FLED_OVER_CRACKS, EVENT(DehkanPlateau_CannonFleesOverCracks)},
    {TRIGGER_STEP, 31, FLAG_CANNON_FLED_EAST, EVENT(DehkanPlateau_CannonFleesEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 8, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom2Pillars)},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom2Pillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 22, FLAG_ROOM2_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundRoom2Pillar)},
    {TRIGGER_PUSH, 0, FLAG_ROOM2_PILLAR_PLACED, EVENT(DehkanPlateau_LiftRoom2Pillar)},
    {TRIGGER_PUSHED, 0, FLAG_ROOM2_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceRoom2Pillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 23, FLAG_ROOM2_PILLAR_PLACED, EVENT(DehkanPlateau_LiftRoom2Pillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, FLAG_ROOM2_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceRoom2Pillar)},
    {TRIGGER_STEP, 6, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOffCrack)},
    {TRIGGER_STEP, 14, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 15, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 16, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 17, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 18, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 19, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 20, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 21, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, FLAG_CANNON_LEAPT_AWAY, EVENT(DehkanPlateau_CannonLeapsAway)},
    {TRIGGER_STEP, 31, FLAG_CANNON_POUNDED_PILLAR, EVENT(DehkanPlateau_CannonPoundsPillar)},
    {TRIGGER_STEP, 32, 0x870, EVENT(DehkanPlateau_CannonCrumblesPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), 12, FLAG_ROOM3_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 18, FLAG_ROOM3_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundRoom3Pillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3Pillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceRoom3Pillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3Pillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceRoom3Pillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, FLAG_ROOM3_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_MoveRoom3SecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 9, FLAG_ROOM3_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_PushRoom3SecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT(Scene_RunIfHeld)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(Scene_RunIfHeld)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3RaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3RaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 16, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3RaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftRoom3RaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom3Pillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom3Pillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom3Pillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom3Pillars)},
    {TRIGGER_STEP, 7, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 8, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_STEP, 12, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 13, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 14, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 15, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 16, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 17, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room4Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 40, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 41, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {TRIGGER_STEP, 8, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 9, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room5Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_STEP, 23, FLAG_CANNON_DROPPED_POUND_CUBE, EVENT(DehkanPlateau_CannonPoundsLeaderThrough)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, FLAG_ROOM5_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundRoom5Pillar)},
    {TRIGGER_EXAMINE, 12, FLAG_POUND_CUBE_TAKEN, EVENT(DehkanPlateau_TakePoundCube)},
    {TRIGGER_STEP, 18, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 19, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 20, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 21, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 22, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 23, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 24, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 25, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 26, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 27, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 28, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 29, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 30, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 31, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 32, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, 33, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDehkanPlateau_Room6Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 40, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 41, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {STEP_TRIGGER_WALKING, 8, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnRoom6Pillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 8, FLAG_CONE_POUNDED, EVENT(DehkanPlateau_PoundRoom6Pillar)},
    {TRIGGER_STEP, 9, FLAG_IN_CONE_CELL, EVENT(DehkanPlateau_EnterConeCell)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_IN_CONE_CELL), EVENT(DehkanPlateau_LeaveConeCell)},
    {TRIGGER_STEP, 5, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 6, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_STEP, 11, FLAG_CANNON_MET, EVENT(DehkanPlateau_MeetCannon)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(DehkanPlateau_FightCannon)},
    {TRIGGER_TABLE_END},
};
