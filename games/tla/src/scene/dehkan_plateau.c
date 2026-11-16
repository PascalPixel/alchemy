/// @file
/// The Dehkan Plateau, the cliffs and caves of Indra between Daila and the East
/// Indra Shore, in seven rooms that share this overlay. Its floors are cracked:
/// the first step on a crack opens it, the next drops the leader through to the
/// cave below (DehkanPlateau_StepOnCrack), and each room puts back the cracks
/// its flags remember (DehkanPlateau_RestoreFullMetalVestChestCracks). Grey
/// pillars crumble once stood on (CrumblingPillars_Step), and the Mars Djinni
/// Cannon runs ahead of the party room by room, cracking floors, crumbling
/// pillars and casting Pound (DehkanPlateau_CannonCrumblesPillars), until a
/// fall lands the leader beside it and it drops the Pound Cube
/// (DehkanPlateau_CannonDropsPoundCube); cornered at last behind a cone the
/// party Pounds flat, it fights (DehkanPlateau_FightCannon).

#include "types.h"
#include "macros.h"
#include "item_ids.h"
#include "system/system.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/field.h"
#include "field/map_grid.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "scene/dehkan_plateau.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "field/field_sprite.h"
#include "system/iwram_call.h"
#include "system/field_effect.h"
#include "field/field_effect.h"
#include "field/field_psynergy_work.h"
#include "shared/tundaria_tower.h"
#include "flags.h"
#include "djinn_ids.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/event.h"
#include "field/flag_call.h"
#include "field/scene_call.h"
#include "fixed_math.h"
#include "field/actor_script.h"
#include "gamepak/far_call/system.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "scene/gabomba_statue.h"

/// Cannon's flight across the plateau, a flag for each stage, room by room.
#define FLAG_CANNON_FLED_OVER_CRACKS 0x86c ///< room 2, over the cracked floor
#define FLAG_CANNON_FLED_EAST 0x86d        ///< room 2, out to the east
#define FLAG_CANNON_LEAPT_AWAY 0x86e       ///< room 3, over the first gap
#define FLAG_CANNON_POUNDED_PILLAR 0x86f   ///< room 3, Pounding a pillar down
#define FLAG_CANNON_CRUMBLED_PILLARS 0x870 ///< room 3, the crumbling pillars
/// Room 5: Cannon drops the Pound Cube, and the party takes it.
#define FLAG_CANNON_DROPPED_POUND_CUBE 0x871
#define FLAG_POUND_CUBE_TAKEN 0x878
/// The pillars Moved into place in rooms 2 and 3.
#define FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED 0x840
#define FLAG_TIGHTROPES_SECOND_PILLAR_PLACED 0x841

// Each room's actors, numbering its placement table. A room's grey pillars
// crumble once stood on (CrumblingPillars_Step), its Pound pillars sink when
// Pounded (Pound_SinkTarget), and its stone pillars are Moved. The covers are
// blank actors over the scenery the leader walks behind, which
// DehkanPlateau_SetActorsHidden shows and hides. Chests number from 64.

/// The first room, where cracks drop the party to the caves below.
enum DehkanPlateauFullMetalVestChestActorId {
  ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FULL_METAL_VEST_CHEST_POUND_PILLAR = ACTOR_FIRST_PLACED + 3,
  /// The stone pillar Moved over the edge into the gap below.
  ACTOR_FULL_METAL_VEST_CHEST_PILLAR,
  ACTOR_FULL_METAL_VEST_CHEST_FIRST_COVER,
  ACTOR_FULL_METAL_VEST_CHEST = 64
};

/// The second room, where Cannon is first seen and flees over the cracks.
enum DehkanPlateauThemisAxeChestActorId {
  ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED + 3,
  ACTOR_THEMIS_AXE_CHEST_POUND_PILLAR = ACTOR_FIRST_PLACED + 14,
  /// The stone pillar Moved into the gap.
  ACTOR_THEMIS_AXE_CHEST_PILLAR,
  ACTOR_THEMIS_AXE_CHEST_CANNON,
  ACTOR_THEMIS_AXE_CHEST_FIRST_COVER
};

/// The third room, of tightropes, a rope to Lash and the crumbling pillars
/// Cannon leaps across.
enum DehkanPlateauTightropesActorId {
  /// The posts at the ends of the two tightropes and the rope.
  ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST,
  ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST,
  ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST,
  ACTOR_TIGHTROPES_ROPE_LOW_POST,
  ACTOR_TIGHTROPES_ROPE_HIGH_POST,
  /// The four pillars that stand on raised ground, pushed off it or left.
  ACTOR_TIGHTROPES_FIRST_RAISED_PILLAR,
  ACTOR_TIGHTROPES_SECOND_RAISED_PILLAR,
  ACTOR_TIGHTROPES_THIRD_RAISED_PILLAR,
  ACTOR_TIGHTROPES_FOURTH_RAISED_PILLAR,
  /// The Pound pillar Cannon Pounds down as he flees.
  ACTOR_TIGHTROPES_POUND_PILLAR,
  ACTOR_TIGHTROPES_PILLAR,
  /// The stone pillar pushed down into the gap below the tightropes.
  ACTOR_TIGHTROPES_SECOND_PILLAR,
  ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
  ACTOR_TIGHTROPES_CANNON = ACTOR_FIRST_PLACED + 17,
  ACTOR_TIGHTROPES_FIRST_COVER
};

/// The fourth room, a passage.
enum DehkanPlateauMintChestActorId {
  ACTOR_MINT_CHEST_FIRST_COVER = ACTOR_FIRST_PLACED
};

/// The fifth room, where a fall lands the leader beside Cannon and he drops
/// the Pound Cube.
enum DehkanPlateauPoundCubeActorId {
  ACTOR_POUND_CUBE_POUND_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_POUND_CUBE_CANNON
};

/// The sixth room, where Cannon is cornered behind the Pound pillar and
/// fights.
enum DehkanPlateauCannonActorId {
  ACTOR_CANNON_ROOM_POUND_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_CANNON_ROOM_CRUMBLING_PILLAR,
  ACTOR_CANNON_ROOM_FIRST_COVER,
  ACTOR_CANNON = ACTOR_FIRST_PLACED + 8
};

/// The covers of each room (Actors_SetHidden, Actor_Hide).
enum {
  FULL_METAL_VEST_CHEST_COVER_COUNT = 14,
  THEMIS_AXE_CHEST_COVER_COUNT = 13,
  TIGHTROPES_COVER_COUNT = 10,
  MINT_CHEST_COVER_COUNT = 3,
  CANNON_ROOM_COVER_COUNT = 6
};

/// A room's scenery actors hidden while the leader is behind them.
#define FLAG_ACTORS_HIDDEN FLAG_SCENE_TEMP_00
/// The leader stands on the crack he has just opened; set until he steps off.
#define FLAG_STANDING_ON_OPENED_CRACK FLAG_SCENE_TEMP_01
/// The leader stands in room 6's cell beside the cone.
#define FLAG_IN_CONE_CELL FLAG_SCENE_TEMP_02
/// Each room's pillar Pounded into the ground, by its list's flag.
#define FLAG_FULL_METAL_VEST_CHEST_PILLAR_POUNDED FLAG_SCENE_TEMP_03
#define FLAG_THEMIS_AXE_CHEST_PILLAR_POUNDED FLAG_SCENE_TEMP_04
#define FLAG_TIGHTROPES_PILLAR_POUNDED FLAG_SCENE_TEMP_05
#define FLAG_POUND_CUBE_PILLAR_POUNDED FLAG_SCENE_TEMP_06
/// Room 6's cone, Pounded flat so Cannon is cornered.
#define FLAG_CONE_POUNDED FLAG_SCENE_TEMP_07
#define FLAG_TIGHTROPES_ROPE_LASHED FLAG_SCENE_TEMP_08
/// Cannon met in room 6 on this visit, or already in the party.
#define FLAG_CANNON_MET FLAG_SCENE_TEMP_09
/// The first of a room's crack flags, one a listed crack, set while the crack
/// is open (DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS).
#define FLAG_CRACK_OPENED_FIRST FLAG_SCENE_TEMP_40
/// The crack the party fell through, one flag for each landing entrance from
/// FIRST_LANDING_ENTRANCE, for the landing drawn below it.
#define FLAG_FELL_THROUGH_FIRST FLAG_AREA_TEMP_20
#define FLAG_FELL_THROUGH_LAST FLAG_AREA_TEMP_5F
#define FIRST_LANDING_ENTRANCE 11

void Actor_DisableById(s32 id);
extern s16 gDehkanPlateau_FullMetalVestChestCrumblingPillars[];
/// The Pound Cube Cannon drops, while it lies on the ground.
extern struct Actor *gDehkanPlateau_PoundCube;
void DehkanPlateau_LeaveConeCell(void);
void DehkanPlateau_ForgetFall(void);
void Actor_StartScript(struct Actor *object, const void *script);

#include "shared/crumbling_pillars.h"

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/rubble.h"

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

// Raises each listed actor's cells: the shape grid and the cell flags under it,
// then lifts the leader if the new terrain is above the leader.
#include "shared/pillars_raise.h"

#include "shared/pillars_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

void Actor_DisableById(s32 id);

extern u16 gDehkanPlateau_FullMetalVestChestCrackPictures[];
extern u16 gDehkanPlateau_FullMetalVestChestCracks[][6];

extern s16 gDehkanPlateau_ThemisAxeChestCrumblingPillars[];
extern s16 gDehkanPlateau_TightropesCrumblingPillars[];
extern s16 gDehkanPlateau_CannonCrumblingPillar[];
extern s16 gDehkanPlateau_FullMetalVestChestPoundPillars[];
extern s16 gDehkanPlateau_ThemisAxeChestPoundPillars[];
extern s16 gDehkanPlateau_TightropesPoundPillars[];
extern s16 gDehkanPlateau_PoundCubePoundPillars[];
extern s16 gDehkanPlateau_CannonPoundPillars[];
extern s16 gDehkanPlateau_ThemisAxeChestMovablePillars[];
extern s16 gDehkanPlateau_TightropesMovablePillars[];

/// Talking to Cannon once he is cornered: the Djinni battle.
void DehkanPlateau_FightCannon(void) {
  GameFlag_Set(FLAG_CANNON_MET);
  Field_RequestDjinnBattle(ACTOR_CANNON, DJINNI_CANNON);
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gDehkanPlateau_Placements[];
extern struct ActorPlacement gDehkanPlateau_FullMetalVestChestPlacements[];
extern struct ActorPlacement gDehkanPlateau_ThemisAxeChestPlacements[];
extern struct ActorPlacement gDehkanPlateau_TightropesPlacements[];
extern struct ActorPlacement gDehkanPlateau_MintChestPlacements[];
extern struct ActorPlacement gDehkanPlateau_PoundCubePlacements[];
extern struct ActorPlacement gDehkanPlateau_CannonPlacements[];
extern struct Trigger gDehkanPlateau_Triggers[];
extern struct Trigger gDehkanPlateau_FullMetalVestChestTriggers[];
extern struct Trigger gDehkanPlateau_ThemisAxeChestTriggers[];
extern struct Trigger gDehkanPlateau_TightropesTriggers[];
extern struct Trigger gDehkanPlateau_MintChestTriggers[];
extern struct Trigger gDehkanPlateau_PoundCubeTriggers[];
extern struct Trigger gDehkanPlateau_CannonTriggers[];

const u8 *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    return gDehkanPlateau_FullMetalVestChestPlacements;
  if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
    return gDehkanPlateau_ThemisAxeChestPlacements;
  if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
    return gDehkanPlateau_TightropesPlacements;
  if (scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
    return gDehkanPlateau_MintChestPlacements;
  if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
    return gDehkanPlateau_PoundCubePlacements;
  if (scene == SCENE_DEHKAN_PLATEAU_CANNON)
    return gDehkanPlateau_CannonPlacements;
  return gDehkanPlateau_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    return gDehkanPlateau_FullMetalVestChestTriggers;
  if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
    return gDehkanPlateau_ThemisAxeChestTriggers;
  if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
    return gDehkanPlateau_TightropesTriggers;
  if (scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
    return gDehkanPlateau_MintChestTriggers;
  if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
    return gDehkanPlateau_PoundCubeTriggers;
  if (scene == SCENE_DEHKAN_PLATEAU_CANNON)
    return gDehkanPlateau_CannonTriggers;
  return gDehkanPlateau_Triggers;
}

/// Stepping onto one of the first room's grey pillars, each of which can be
/// stood on only once before it crumbles.
void DehkanPlateau_StepOnFullMetalVestChestPillars(void) {
  CrumblingPillars_Step(gDehkanPlateau_FullMetalVestChestCrumblingPillars);
}

/// Gives the cells by room 2's four crumbling pillars still standing the
/// attributes of cell (0, 0).
void DehkanPlateau_SetThemisAxeChestStandingPillarCells(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_13) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 18);
  if (GameFlag_Get(FLAG_SCENE_TEMP_15) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 11, 18);
  if (GameFlag_Get(FLAG_SCENE_TEMP_21) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 14, 28);
  if (GameFlag_Get(FLAG_SCENE_TEMP_23) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 16, 28);
}

/// Stepping onto one of the second room's grey pillars; on the low ones the
/// leader is drawn behind the ledge.
void DehkanPlateau_StepOnThemisAxeChestPillars(void) {
  FIXED y;

  CrumblingPillars_StepWithStandIn(
      gDehkanPlateau_ThemisAxeChestCrumblingPillars);
  DehkanPlateau_SetThemisAxeChestStandingPillarCells();
  y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;
  if (y < 0x80000) {
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// The same for room 3's two pillars, from cell (63, 0).
void DehkanPlateau_SetTightropesStandingPillarCells(void) {
  if (GameFlag_Get(FLAG_SCENE_TEMP_13) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 50, 20);
  if (GameFlag_Get(FLAG_SCENE_TEMP_15) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 52, 20);
}

/// Stepping onto one of the third room's grey pillars; below the ledge the
/// leader is drawn behind it.
void DehkanPlateau_StepOnTightropesPillars(void) {
  CrumblingPillars_Step(gDehkanPlateau_TightropesCrumblingPillars);
  DehkanPlateau_SetTightropesStandingPillarCells();
  if (Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed < -toFIXED(24)) {
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Stepping onto the sixth room's grey pillar; from up on it the leader is out
/// of the Pound pillar's cell.
void DehkanPlateau_StepOnCannonPillar(void) {
  FIXED y;

  CrumblingPillars_Step(gDehkanPlateau_CannonCrumblingPillar);
  y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;
  if (y < 0x180000)
    DehkanPlateau_LeaveConeCell();
}

/// Sets the covers of the room the party is in (Actors_SetHidden).
void DehkanPlateau_SetActorsHidden(s32 show) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    Actors_SetHidden(show, ACTOR_FULL_METAL_VEST_CHEST_FIRST_COVER,
                     FULL_METAL_VEST_CHEST_COVER_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
    Actors_SetHidden(show, ACTOR_THEMIS_AXE_CHEST_FIRST_COVER,
                     THEMIS_AXE_CHEST_COVER_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
    Actors_SetHidden(show, ACTOR_TIGHTROPES_FIRST_COVER,
                     TIGHTROPES_COVER_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
    Actors_SetHidden(show, ACTOR_MINT_CHEST_FIRST_COVER,
                     MINT_CHEST_COVER_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_CANNON)
    Actors_SetHidden(show, ACTOR_CANNON_ROOM_FIRST_COVER,
                     CANNON_ROOM_COVER_COUNT);
}

/// Stepping behind the scenery: the room's covers change, and the flag
/// remembers it.
void DehkanPlateau_HideActors(void) {
  DehkanPlateau_SetActorsHidden(1);
  GameFlag_Set(FLAG_ACTORS_HIDDEN);
}

/// Stepping back out from behind the scenery: the room's covers change back.
void DehkanPlateau_ShowActors(void) {
  DehkanPlateau_SetActorsHidden(0);
  GameFlag_Clear(FLAG_ACTORS_HIDDEN);
}

/// Sets or clears bit 0x10 of count actors' priority flags from first.
void Actors_SetHidden(s32 show, s32 first, u32 count) {
  u32 i;

  for (i = 0; i < count; i++) {
    struct Actor *actor = Actor_GetById(first + i);

    if (show)
      actor->priorityFlags &= ~ACTOR_PRIORITY_NOT_DRAWN;
    else
      actor->priorityFlags |= ACTOR_PRIORITY_NOT_DRAWN;
  }
}

/// Pound on the first room's Pound pillar: it sinks into the ground.
void DehkanPlateau_PoundFullMetalVestChestPillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_FullMetalVestChestPoundPillars, a, b);
}

/// Pound on the second room's Pound pillar.
void DehkanPlateau_PoundThemisAxeChestPillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_ThemisAxeChestPoundPillars, a, b);
}

/// Pound on the third room's Pound pillar.
void DehkanPlateau_PoundTightropesPillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_TightropesPoundPillars, a, b);
}

/// Pound on the fifth room's Pound pillar.
void DehkanPlateau_PoundPoundCubePillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_PoundCubePoundPillars, a, b);
}

/// Pound on the sixth room's Pound pillar, which corners Cannon.
void DehkanPlateau_PoundCannonPillar(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_CannonPoundPillars, a, b);
}

#include "shared/actor_drop_and_wait.h"

/// Move on the first room's stone pillar: once it stands at the gap's edge it
/// drops in and stays (FLAG_AREA_TEMP_01).
void DehkanPlateau_DropFullMetalVestChestPillarIntoGap(void) {
  struct Actor *object = Actor_GetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
  s32 x;

  x = FIELD_CELL(object->x.fixed);
  if (x == 56 && FIELD_CELL(object->z.fixed) == 32) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_Set(FLAG_AREA_TEMP_01);
    Event_End();
  }
}

/// Move takes hold of the second room's stone pillar.
void DehkanPlateau_LiftThemisAxeChestPillar(void) {
  Actor_LiftTargetBlock(gDehkanPlateau_ThemisAxeChestMovablePillars);
}

/// Move lets go of the second room's stone pillar: at the gap it drops in for
/// good (FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED).
void DehkanPlateau_PlaceThemisAxeChestPillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_THEMIS_AXE_CHEST_PILLAR);
  s32 x = FIELD_CELL(object->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 52) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 7, 1, 1, x, 7);
    GameFlag_Set(FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED);
    Event_End();
  }
}

/// Move takes hold of the third room's stone pillar.
void DehkanPlateau_LiftTightropesPillar(void) {
  Actor_LiftTargetBlock(gDehkanPlateau_TightropesMovablePillars);
}

/// Move lets go of the third room's stone pillar: at the gap it drops in.
void DehkanPlateau_PlaceTightropesPillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_TIGHTROPES_PILLAR);
  s32 x = FIELD_CELL(object->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 47) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 2);
    Event_End();
  } else if (x == 46) {
    MapGrid_CopyCellAttributes(44, 13, 1, 1, 48, 13);
  }
}

/// Puts the third room's second stone pillar down in the gap below the
/// tightropes, with the cells that let it be walked over.
void DehkanPlateau_SetTightropesSecondPillarInPlace(void) {
  Actor_SetPosition(ACTOR_TIGHTROPES_SECOND_PILLAR, 600, 536);
  MapGrid_CopyCellAttributes(38, 32, 1, 1, 37, 33);
  MapGrid_CopyCellAttributes(30, 32, 1, 2, 38, 32);
  MapGrid_CopyCellAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_Set(FLAG_TIGHTROPES_SECOND_PILLAR_PLACED);
}

/// Move on the third room's second stone pillar: one column on, it falls into
/// place.
void DehkanPlateau_MoveTightropesSecondPillar(void) {
  if (FIELD_CELL(Actor_GetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->x.fixed) == 37)
    DehkanPlateau_SetTightropesSecondPillarInPlace();
}

/// Walking into the third room's second stone pillar at the gap's edge pushes
/// it in.
void DehkanPlateau_PushTightropesSecondPillar(void) {
  u8 *flags;

  if (FIELD_CELL(Actor_GetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->x.fixed) ==
      38) {
    flags = &Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags;
    Event_Start();
    Event_ResolvePendingActions(0);
    *flags = 0;
    wait(1);
    Leader_PushObjectAhead();
    DehkanPlateau_SetTightropesSecondPillarInPlace();
    wait(1);
    *flags = 3;
    Event_End();
  }
}

/// Stepping off a crack opened on this visit lets it be stepped on again.
void DehkanPlateau_StepOffCrack(void) {
  GameFlag_Clear(FLAG_STANDING_ON_OPENED_CRACK);
}

/// Forgets which crack the party fell through.
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

#define DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(name, cracks, last, pictures)      \
  void name(void) {                                                            \
    const u16 *cell = &cracks[0][0];                                           \
    s32 flag = FLAG_CRACK_OPENED_FIRST;                                        \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i <= last; i++) {                                              \
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {                       \
        if (GameFlag_Get(flag) != 0) {                                         \
          s32 x;                                                               \
          s32 y;                                                               \
                                                                               \
          cell = &cracks[i][1];                                                \
          x = *cell++;                                                         \
          y = *cell++;                                                         \
          MapGrid_CopyCellPictureIndicesRect(pictures[0], pictures[1], x, y,   \
                                             1, 1);                            \
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

/// Restores the first room's fourteen cracks: on a return the opened ones are
/// drawn open, otherwise each listed crack starts its flag.
DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(
    DehkanPlateau_RestoreFullMetalVestChestCracks,
    gDehkanPlateau_FullMetalVestChestCracks, 13,
    gDehkanPlateau_FullMetalVestChestCrackPictures)

extern u16 gDehkanPlateau_ThemisAxeChestCrackPictures[];
extern u16 gDehkanPlateau_ThemisAxeChestCracks[][6];

/// Restores the second room's eight cracks, as the first room's.
DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(DehkanPlateau_RestoreThemisAxeChestCracks,
                                    gDehkanPlateau_ThemisAxeChestCracks, 7,
                                    gDehkanPlateau_ThemisAxeChestCrackPictures)

extern u16 gDehkanPlateau_TightropesCrackPictures[];
extern u16 gDehkanPlateau_TightropesCracks[][6];

/// Restores the third room's six cracks, as the first room's.
DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS(DehkanPlateau_RestoreTightropesCracks,
                                    gDehkanPlateau_TightropesCracks, 5,
                                    gDehkanPlateau_TightropesCrackPictures)

/// As the other rooms' (DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS) for the fifth
/// room's sixteen cracks: on a return the opened cracks are drawn open, and
/// otherwise each listed crack starts its flag.
void DehkanPlateau_RestorePoundCubeCracks(void) {
  u16 *record = &gDehkanPlateau_PoundCubeCracks[0][0];
  s32 flag = FLAG_CRACK_OPENED_FIRST;
  u32 i;

  for (i = 0; i <= 15; flag++, i++) {
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      if (GameFlag_Get(flag)) {
        s32 x;
        s32 y;

        record = &gDehkanPlateau_PoundCubeCracks[i][1];
        x = *record++;
        y = *record++;
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_PoundCubeCrackPictures[0],
            gDehkanPlateau_PoundCubeCrackPictures[1], x, y, 1, 1);
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

s32 gDehkanPlateau_CrackZ BSS_DATA;
s32 gDehkanPlateau_CrackX BSS_DATA;

#define SCENE(name) ((s32)&SceneId_##name)

/// Steps onto a listed cell in rooms 1, 2, 3 and 5
/// (SceneId_DehkanPlateauFullMetalVestChest to SceneId_DehkanPlateauPoundCube):
/// either falls through to exit work->stepTrigger or opens the cell and sets
/// its flag. cellX is reused for the doubled index and the fall's frame count.
void DehkanPlateau_StepOnCrack(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 cellX = (FIELD_CELL(leader->x.fixed) << 1) + 1;
  s32 cellZ = (FIELD_CELL(leader->z.fixed) << 1) + 1;
  s32 n, flag;
  u16 *record, *next;

  if (gMainRoutineWork.next == SCENE(DehkanPlateauFullMetalVestChest)) {
    n = work->stepTrigger - 11;
    flag = work->stepTrigger + 0x235;
    record = &gDehkanPlateau_FullMetalVestChestCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE(DehkanPlateauThemisAxeChest)) {
    n = work->stepTrigger - 14;
    flag = work->stepTrigger + 0x232;
    record = &gDehkanPlateau_ThemisAxeChestCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE(DehkanPlateauTightropes)) {
    n = work->stepTrigger - 12;
    flag = work->stepTrigger + 0x234;
    record = &gDehkanPlateau_TightropesCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE(DehkanPlateauPoundCube)) {
    n = work->stepTrigger - 18;
    flag = work->stepTrigger + 0x22e;
    record = &gDehkanPlateau_PoundCubeCracks[n][1];
    if (GameFlag_Get(FLAG_AREA_TEMP_3A))
      gDehkanPlateau_PoundCubeCracks[4][3] = 3;
  } else {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 4);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, cellX << 3,
                          cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, cellX << 19, leader->y.fixed,
                                  cellZ << 19);
  Event_Wait(4);
  FieldCamera_GetActor()->y.fixed -= toFIXED(8);
  next = record + 1;
  if (GameFlag_Get(flag)) {
    s32 x, y, index, toX, toZ;

    Sound_Play(SE_SPLASH);
    x = *record;
    record = next;
    {
      s16 scene = gMainRoutineWork.next;
      y = *record++;
      if (scene == SCENE(DehkanPlateauFullMetalVestChest))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_FullMetalVestChestCrackPictures[2],
            gDehkanPlateau_FullMetalVestChestCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauThemisAxeChest))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_ThemisAxeChestCrackPictures[2],
            gDehkanPlateau_ThemisAxeChestCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauTightropes))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_TightropesCrackPictures[2],
            gDehkanPlateau_TightropesCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauPoundCube))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_PoundCubeCrackPictures[2],
            gDehkanPlateau_PoundCubeCrackPictures[3], x, y, 1, 1);
    }
    index = *record++;
    cellX = index * 2;
    toX = record[0];
    toZ = record[1];
    {
      s16 scene = gMainRoutineWork.next;
      if (scene == SCENE(DehkanPlateauFullMetalVestChest)) {
        {
          s32 srcX = gDehkanPlateau_FullMetalVestChestHolePictures[cellX],
              srcY = gDehkanPlateau_FullMetalVestChestHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateauThemisAxeChest)) {
        {
          s32 srcX = gDehkanPlateau_ThemisAxeChestHolePictures[cellX],
              srcY = gDehkanPlateau_ThemisAxeChestHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateauTightropes)) {
        {
          s32 srcX = gDehkanPlateau_TightropesHolePictures[cellX],
              srcY = gDehkanPlateau_TightropesHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE(DehkanPlateauPoundCube)) {
        if ((n == 0 && GameFlag_Get(FLAG_SCENE_TEMP_41)) ||
            (n == 11 && GameFlag_Get(FLAG_SCENE_TEMP_4C))) {
          s32 srcX = gDehkanPlateau_PoundCubeHolePictures[8],
              srcY = gDehkanPlateau_PoundCubeHolePictures[9], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        } else {
          s32 srcX = gDehkanPlateau_PoundCubeHolePictures[cellX],
              srcY = gDehkanPlateau_PoundCubeHolePictures[cellX + 1], width = 1,
              height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      }
    }
    Event_Wait(12);
    leader->facing = FACING_SOUTH;
    leader->motionFlags = 0;
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    {
      s16 scene = gMainRoutineWork.next;
      if (scene == SCENE(DehkanPlateauFullMetalVestChest)) {
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
      } else if (scene == SCENE(DehkanPlateauThemisAxeChest) ||
                 scene == SCENE(DehkanPlateauTightropes)) {
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
      } else if (scene == SCENE(DehkanPlateauPoundCube)) {
        if (gDehkanPlateau_PoundCube != NULL)
          ((struct FieldSprite *)gDehkanPlateau_PoundCube->sprite)->priority =
              3;
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
      }
    }
    Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                           ANIM_LEADER_ARMS_RAISED);
    Event_Wait(16);
    for (cellX = 0; (u32)cellX <= 39; cellX++) {
      if (cellX == 5)
        Sound_Play(SE_DUST_PUFF);
      leader->scaleX += -(FIXED_ONE / 64);
      leader->scaleY += -(FIXED_ONE * 3 / 256);
      leader->y.fixed += -(toFIXED(3) / 5);
      wait(1);
    }
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Import_EventSetExitRequest(work->stepTrigger);
  } else {
    s32 x, y;

    Sound_Play(SE_RATTLE);
    y = *next;
    x = *record;
    {
      s16 scene = gMainRoutineWork.next;
      if (scene == SCENE(DehkanPlateauFullMetalVestChest))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_FullMetalVestChestCrackPictures[0],
            gDehkanPlateau_FullMetalVestChestCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauThemisAxeChest))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_ThemisAxeChestCrackPictures[0],
            gDehkanPlateau_ThemisAxeChestCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauTightropes))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_TightropesCrackPictures[0],
            gDehkanPlateau_TightropesCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE(DehkanPlateauPoundCube))
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_PoundCubeCrackPictures[0],
            gDehkanPlateau_PoundCubeCrackPictures[1], x, y, 1, 1);
    }
    Event_Wait(12);
    GameFlag_Set(flag);
    GameFlag_Set(FLAG_STANDING_ON_OPENED_CRACK);
    {
      s16 scene = gMainRoutineWork.next;
      if (scene == SCENE(DehkanPlateauPoundCube) ||
          scene == SCENE(DehkanPlateauTightropes)) {
        gDehkanPlateau_CrackX = FIELD_CELL(leader->x.fixed);
        gDehkanPlateau_CrackZ = FIELD_CELL(leader->z.fixed);
      }
    }
    Event_End();
  }
}

static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Get(flag); }

/// The third and fifth rooms' task: once the leader steps off the crack he
/// opened, it can be stepped on again.
void DehkanPlateau_ForgetCrackOnLeaving(void) {
  struct Actor *object;

  if (Flag_Test(FLAG_STANDING_ON_OPENED_CRACK) != 0) {
    object = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (gDehkanPlateau_CrackX != FIELD_CELL(object->x.fixed) ||
        gDehkanPlateau_CrackZ != FIELD_CELL(object->z.fixed))
      GameFlag_Clear(FLAG_STANDING_ON_OPENED_CRACK);
  }
}

#include "shared/actor_hide.h"

/// On a return to the room the covers are as the flag left them; otherwise the
/// flag starts set.
void DehkanPlateau_RestoreCovers(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(FLAG_ACTORS_HIDDEN) == 0)
      DehkanPlateau_ShowActors();
  } else {
    GameFlag_Set(FLAG_ACTORS_HIDDEN);
  }
}

/// Hides the first room's covers (Actor_Hide) as it opens.
void DehkanPlateau_HideFullMetalVestChestActors(void) {
  u32 i;

  for (i = 0; i <= 13; i++)
    Actor_Hide(i + ACTOR_FULL_METAL_VEST_CHEST_FIRST_COVER);
  DehkanPlateau_RestoreCovers();
}

/// Hides the second room's covers as it opens.
void DehkanPlateau_HideThemisAxeChestActors(void) {
  u32 i;

  for (i = 0; i <= 12; i++)
    Actor_Hide(i + ACTOR_THEMIS_AXE_CHEST_FIRST_COVER);
  DehkanPlateau_RestoreCovers();
}

/// Hides the third room's covers as it opens.
void DehkanPlateau_HideTightropesActors(void) {
  u32 i;

  for (i = 0; i <= 9; i++)
    Actor_Hide(i + ACTOR_TIGHTROPES_FIRST_COVER);
  DehkanPlateau_RestoreCovers();
}

/// Hides the fourth room's covers as it opens.
void DehkanPlateau_HideMintChestActors(void) {
  u32 i;

  for (i = 0; i <= 2; i++)
    Actor_Hide(i + ACTOR_MINT_CHEST_FIRST_COVER);
  DehkanPlateau_RestoreCovers();
}

/// Hides the sixth room's covers as it opens.
void DehkanPlateau_HideCannonActors(void) {
  u32 i;

  for (i = 0; i <= 5; i++)
    Actor_Hide(i + ACTOR_CANNON_ROOM_FIRST_COVER);
  DehkanPlateau_RestoreCovers();
}

/// Sets the cells under a raised pillar of the third room once it is pushed:
/// off the ledge at column 12 it drops to the ground below, and at columns 18,
/// 54 and 55 it bridges the way.
void DehkanPlateau_PlaceRaisedPillar(struct Actor *object) {
  s32 x;
  struct MapCell *cell;

  HeldActor_RaiseCell(object);
  x = FIELD_CELL(object->x.fixed);
  if (x == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Actor_DropAndWait(object);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Event_End();
  } else if (x == 18) {
    cell = Ram_HeapSlots->mapGrid->layers[0].cells;
    cell += Actor_GetCellIndex(object);
    cell->attributeB &= 0x7f;
  } else if (x == 54) {
    cell = Ram_HeapSlots->mapGrid->layers[0].cells;
    cell += Actor_GetCellIndex(object);
    cell->attributeB &= 0x7f;
    cell->collisionCode = 11;
  } else if (x == 55) {
    MapGrid_CopyCellAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

extern s16 gDehkanPlateau_TightropesRaisedPillars[];

/// Walking into one of the third room's raised pillars pushes it a cell on.
void DehkanPlateau_PushRaisedPillar(void) {
  struct Actor *object;

  if (Leader_CanPushObjectAhead() != 0) {
    object = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId));
    Pillars_LowerHeldCells(gDehkanPlateau_TightropesRaisedPillars);
    Leader_PushObjectAhead();
    DehkanPlateau_PlaceRaisedPillar(object);
  }
}

/// Move takes hold of one of the third room's raised pillars.
void DehkanPlateau_LiftTightropesRaisedPillar(void) {
  Pillars_LowerHeldCells(gDehkanPlateau_TightropesRaisedPillars);
}

/// Move lets go of one of the third room's raised pillars.
void DehkanPlateau_FinishMovingRaisedPillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *active;

  active = work->movedBlock;
  if (active != 0)
    DehkanPlateau_PlaceRaisedPillar(active);
}

/// Stepping beside room 6's cone before it is Pounded holds it still and
/// changes the cell in front of it; stepping away puts the cell back.
void DehkanPlateau_EnterConeCell(void) {
  s32 set = GameFlag_Get(FLAG_CONE_POUNDED);

  if (set == 0) {
    Actor_GetById(ACTOR_CANNON_ROOM_POUND_PILLAR)->motionFlags = set;
    MapGrid_CopyCellAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_Set(FLAG_IN_CONE_CELL);
  }
}

/// Stepping away from the sixth room's Pound pillar puts back the cell before
/// it.
void DehkanPlateau_LeaveConeCell(void) {
  if (GameFlag_Get(FLAG_CONE_POUNDED) == 0) {
    MapGrid_CopyCellAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_Clear(FLAG_IN_CONE_CELL);
  }
}

/// Jolts the camera down eight pixels and back over eight frames, as the ground
/// gives way.
void DehkanPlateau_JoltCamera(void) {
  struct Actor *object = FieldCamera_GetActor();
  u32 i;

  object->y.fixed -= toFIXED(8);
  wait(1);
  for (i = 0; i <= 7; i++) {
    object->y.fixed += toFIXED(1);
    wait(1);
  }
}
#undef SCENE

/// Places an actor on the ground at a 16.16 position, facing a heading.
static __inline__ void Actor_PlaceFacing(s32 id, FIXED x, FIXED z, u16 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

/// In the second room Cannon is first seen and flees from the party over the
/// cracked floor, which gives way behind him.
void DehkanPlateau_CannonFleesOverCracks(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(658, CAMERA_KEEP, 164, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_THEMIS_AXE_CHEST_CANNON,
                      gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmoteById(ACTOR_THEMIS_AXE_CHEST_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_THEMIS_AXE_CHEST_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 664, 136);
  MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  GameFlag_Set(FLAG_SCENE_TEMP_47);
  Actor_WalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 664, 104);
  MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
  Sound_Play(SE_SPLASH);
  DehkanPlateau_JoltCamera();
  Event_Wait(10);
  Actor_Jump(ACTOR_THEMIS_AXE_CHEST_CANNON, 6, 16);
  Actor_PlaceFacing(ACTOR_THEMIS_AXE_CHEST_CANNON, toFIXED(888), toFIXED(168),
                    FACING_SOUTH);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_FLED_OVER_CRACKS);
  Event_End();
}

/// In the second room Cannon runs off out to the east.
void DehkanPlateau_CannonFleesEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(980, CAMERA_KEEP, 168, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_THEMIS_AXE_CHEST_CANNON,
                      gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmoteById(ACTOR_THEMIS_AXE_CHEST_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_GetById(ACTOR_THEMIS_AXE_CHEST_CANNON)->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_THEMIS_AXE_CHEST_CANNON, 1);
  Actor_SetSpeed(ACTOR_THEMIS_AXE_CHEST_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 940, 168);
  Actor_WalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 984, 152);
  Actor_WalkToAndWait(ACTOR_THEMIS_AXE_CHEST_CANNON, 1040, 152);
  Actor_SetPosition(ACTOR_THEMIS_AXE_CHEST_CANNON, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_FLED_EAST);
  Event_End();
}

/// In the third room Cannon leaps away over the first gap.
void DehkanPlateau_CannonLeapsAway(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(144, CAMERA_KEEP, 107, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_TIGHTROPES_CANNON, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_TIGHTROPES_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
  Actor_WalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 168, 104);
  Event_Wait(10);
  Sound_Play(SE_HIGH_JUMP);
  Actor_GetById(ACTOR_TIGHTROPES_CANNON)->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 200, 104);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 1);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 294, 104);
  Actor_PlaceFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(424), toFIXED(104),
                    FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_LEAPT_AWAY);
  Event_End();
}

/// In the third room Cannon casts Pound on the Pound pillar to get by.
void DehkanPlateau_CannonPoundsPillar(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_TIGHTROPES_CANNON);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveToFX(212 << 17, -1, 208 << 15, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_TIGHTROPES_CANNON, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_TIGHTROPES_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  FieldCamera_MoveTo(424, CAMERA_KEEP, 152, 1);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 424, 152);
  Actor_FaceDirectionById(ACTOR_TIGHTROPES_CANNON, FACING_EAST, 20);
  DehkanPlateau_CannonCastsPound();
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 440, 152);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 168);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 184);
  GameFlag_Set(FLAG_SCENE_TEMP_41);
  MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 216);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 584, 216);
  Actor_PlaceFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(616), toFIXED(280),
                    FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_POUNDED_PILLAR);
  Event_End();
}

/// The task of Cannon's Pound in the third room: the Pound pillar sinks, and
/// once the Psynergy ends it is down for good.
void DehkanPlateau_SinkPoundedPillar(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *object = Actor_GetById(ACTOR_TIGHTROPES_POUND_PILLAR);

  if (work->frame != -1 && work->frame <= 7)
    object->scaleY += -(FIXED_ONE * 31 / 256);
  if (work->stage != 0) {
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetAnimationById(ACTOR_TIGHTROPES_POUND_PILLAR, 4);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    object->collisionFlags &= ~ACTOR_SOLID;
    object->scaleY = FIXED_ONE;
    GameFlag_Set(FLAG_TIGHTROPES_PILLAR_POUNDED);
  }
}

/// Cannon casts Pound on the third room's Pound pillar.
void DehkanPlateau_CannonCastsPound(void) {
  FieldPsynergy_LoadResources(ABILITY_POUND, 1);
  FieldTask_AddFrame(DehkanPlateau_SinkPoundedPillar, TASK_PASS_VBLANK);
  FieldPsynergy_SetupActorPair(ACTOR_TIGHTROPES_CANNON,
                               ACTOR_TIGHTROPES_POUND_PILLAR);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_SinkPoundedPillar);
  FieldPsynergy_End();
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_DEHKAN_PLATEAU_RESTORE_CELLS
#undef SCENE
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
void Actor_DisableById(s32 id);

extern u32 gDehkanPlateau_DustScript[];

/// Flashes actor id, then raises eight puffs over actor center, then sets
/// flag and waits 20 frames.
void DehkanPlateau_CrumblePillar(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;
  FIXED y;

  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(2);
  Actor_SetAnimationById(id, 0);
  Actor_SetSpriteColor(Actor_GetById(id), 7);
  wait(4);
  Actor_SetSpriteColor(Actor_GetById(id), 0);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *object;
    struct FieldEffect *effect;

    y = leader->y.fixed - toFIXED(((Random16() * 7) << 1) >> 16);
    object = ((struct Actor *)Import_ActorCreate(
        SPRITE_DUST_CLOUD, (i << 17) + leader->x.fixed - FIXED_ONE * 8,
        y - FIXED_ONE * 2, leader->z.fixed));
    if (object != NULL) {
      effect = (struct FieldEffect *)object;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      object->motionFlags = 0;
      object->collisionFlags = 0;
      Actor_SetSpriteFlags(object, 0);
      Actor_SelectAnimation(object, 2);
      Actor_StartScript(object, gDehkanPlateau_DustScript);
      object->scaleX = FIXED_ONE * 7 / 10;
      object->scaleY = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      effect->velocity.x = 0;
      effect->velocity.z = 0;
      effect->velocity.y =
          -((((u32)Random16() << 14) >> 16) + FIXED_ONE * 3 / 8);
      effect->spin = (((u32)Random16() << 9) >> 16) - 256;
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = FieldEffect_AdvanceMotion;
    }
  }
  GameFlag_Set(flag);
  Event_Wait(20);
}

/// In the third room Cannon leaps across the grey pillars, crumbling two of
/// them behind him.
void DehkanPlateau_CannonCrumblesPillars(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_TIGHTROPES_CANNON);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_TIGHTROPES_CANNON);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(616, CAMERA_KEEP, 280, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_TIGHTROPES_CANNON, gMainRoutineWork.leaderActorId,
                      0);
  // A direct call: as Call3 the actor id is loaded before the 0x100's shift.
  Actor_ShowEmoteById(ACTOR_TIGHTROPES_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_CANNON, 1);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 632, 280);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 664, 280);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 1);
  DehkanPlateau_CrumblePillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
                              ACTOR_TIGHTROPES_CANNON, FLAG_SCENE_TEMP_10);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 696, 280);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 1);
  Event_Wait(10);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 1.6);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 712, 280);
  FieldCamera_SetSpeed(2.4);
  FieldCamera_MoveTo(712, CAMERA_KEEP, 136, 1);
  MapGrid_CopyCellPictureIndicesRect(112, 6, 110, 4, 1, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_CANNON, 1);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 712, 200);
  Actor_WalkToAndWaitById(ACTOR_TIGHTROPES_CANNON, 712, 168);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Actor_FaceDirectionAndRefreshById(ACTOR_TIGHTROPES_CANNON, FACING_EAST);
  Actor_SetSpeed(ACTOR_TIGHTROPES_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 168);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 1);
  DehkanPlateau_CrumblePillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 3,
                              ACTOR_TIGHTROPES_CANNON, FLAG_SCENE_TEMP_16);
  Actor_FaceDirectionAndRefreshById(ACTOR_TIGHTROPES_CANNON, FACING_NORTH);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 136);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 1);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 72);
  MapGrid_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(SE_SPLASH);
  DehkanPlateau_JoltCamera();
  Event_Wait(10);
  Actor_Jump(ACTOR_TIGHTROPES_CANNON, 6, 16);
  Actor_SetPosition(ACTOR_TIGHTROPES_CANNON, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_CRUMBLED_PILLARS);
  Event_End();
}

/// In the fifth room Cannon Pounds the leader through the floor to the cave
/// below.
void DehkanPlateau_CannonPoundsLeaderThrough(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  {
    s32 facing = FACING_NORTH;

    Actor_SetHorizontalPositionAndAngleWithTerrain(
        gMainRoutineWork.leaderActorId, toFIXED(248), toFIXED(136), facing);
  }
  MapGrid_CopyCellPictureIndicesRect(65, 64, 15, 8, 1, 1);
  GameFlag_Set(FLAG_SCENE_TEMP_45);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  Actor_FaceActorById(ACTOR_POUND_CUBE_CANNON, gMainRoutineWork.leaderActorId,
                      0);
  Actor_ShowEmoteById(ACTOR_POUND_CUBE_CANNON,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  DehkanPlateau_CannonCastsPoundOnLeader();
  Event_Wait(20);
  leader->facing = FACING_SOUTH;
  leader->motionFlags = 0;
  Actor_SetSpritePriority(ACTOR_POUND_CUBE_CANNON, 3);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_ARMS_RAISED);
  Event_Wait(16);
  for (i = 0; i < 40; i++) {
    if (i == 5)
      Sound_Play(SE_DUST_PUFF);
    leader->scaleX -= FIXED_ONE / 64;
    leader->scaleY -= FIXED_ONE * 3 / 256;
    leader->y.fixed -= toFIXED(3) / 5;
    wait(1);
  }
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(23);
}

/// The task of Cannon's Pound on the leader: the floor under him opens.
void DehkanPlateau_PoundLeaderThrough(void) {
  if (((struct FieldEffectWork *)Ram_HeapSlots->fieldEffectWork)->stage != 0) {
    Sound_Play(SE_SPLASH);
    MapGrid_CopyCellPictureIndicesRect(64, 64, 15, 8, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(64, 66, 15, 72, 1, 2);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  }
}

extern s16 gDehkanPlateau_ThemisAxeChestCrumblingPillars[];
extern s16 gDehkanPlateau_TightropesCrumblingPillars[];
extern s16 gDehkanPlateau_CannonCrumblingPillar[];
extern s16 gDehkanPlateau_FullMetalVestChestPoundPillars[];
extern s16 gDehkanPlateau_ThemisAxeChestPoundPillars[];
extern s16 gDehkanPlateau_PoundCubePoundPillars[];
extern s16 gDehkanPlateau_CannonPoundPillars[];
extern s16 gDehkanPlateau_ThemisAxeChestMovablePillars[];
extern s16 gDehkanPlateau_TightropesMovablePillars[];

void DehkanPlateau_LeaveConeCell(void);
void DehkanPlateau_ForgetFall(void);

/// Cannon casts Pound on the leader in the fifth room.
void DehkanPlateau_CannonCastsPoundOnLeader(void) {
  FieldPsynergy_LoadResources(ABILITY_POUND, 1);
  FieldTask_AddFrame(DehkanPlateau_PoundLeaderThrough, TASK_PASS_VBLANK);
  FieldPsynergy_SetupActorPair(ACTOR_POUND_CUBE_CANNON,
                               gMainRoutineWork.leaderActorId);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_PoundLeaderThrough);
  FieldPsynergy_End();
}

void Actor_TakePlaceOf(struct Actor *object, struct Actor *other);
void Actor_MoveToObject(s32 id, s32 target);

/// The leader lands beside Cannon in the fifth room; startled, he drops the
/// Pound Cube and flees.
void DehkanPlateau_CannonDropsPoundCube(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *helper = Actor_GetById(ACTOR_POUND_CUBE_CANNON);

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  Sound_Play(SE_KOLIMA_BRIDGE_HOP);
  DehkanPlateau_JoltCamera();
  if (gDehkanPlateau_PoundCube != NULL) {
    FieldActor_SetPositionAndResetMotion(gDehkanPlateau_PoundCube, toFIXED(248),
                                         0, toFIXED(120));
    wait(1);
    gDehkanPlateau_PoundCube->speedLimit = toFIXED(8) / 5;
    gDehkanPlateau_PoundCube->acceleration = toFIXED(8) / 5;
    gDehkanPlateau_PoundCube->velocityY = toFIXED(8);
  }
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_SetSpeed(ACTOR_POUND_CUBE_CANNON, 1.6);
  leader->velocityY = toFIXED(6);
  helper->velocityY = toFIXED(6);
  FieldActor_SetMoveTarget(leader, 216, 0, 120);
  FieldActor_SetMoveTarget(helper, 280, 0, 120);
  if (gDehkanPlateau_PoundCube != NULL)
    FieldActor_SetMoveTarget(gDehkanPlateau_PoundCube, 248, 0, 88);
  Actor_WaitForMove(leader);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_POUND_CUBE_CANNON,
                      EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_POUND_CUBE_CANNON, FACING_WEST, 60);
  Actor_WalkToAndWait(ACTOR_POUND_CUBE_CANNON, 296, 104);
  Actor_WalkToAndWait(ACTOR_POUND_CUBE_CANNON, 296, 80);
  Actor_SetPosition(ACTOR_POUND_CUBE_CANNON, 0, 0);
  Event_Wait(20);
  if (gDehkanPlateau_PoundCube != NULL) {
    gDehkanPlateau_PoundCube->motionFlags = 0;
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
    wait(1);
  }
  GameFlag_Set(FLAG_CANNON_DROPPED_POUND_CUBE);
  Event_End();
}

/// Meeting Cannon in the sixth room while the Pound pillar stands: he leaps
/// over the last grey pillar, crumbling it, and waits on the far side.
void DehkanPlateau_CannonCrumblesLastPillar(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_CANNON);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_CANNON);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(760, 32, 204, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_CANNON, gMainRoutineWork.leaderActorId, 0);
  // Called for its value, which is unused: as Call3 the actor id is loaded
  // before the 0x100's shift.
  Actor_ShowEmote(ACTOR_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 760, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 1);
  Event_Wait(20);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 792, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 1);
  DehkanPlateau_CrumblePillar(ACTOR_CANNON_ROOM_CRUMBLING_PILLAR, ACTOR_CANNON,
                              FLAG_SCENE_TEMP_10);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 824, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 1);
  Event_Wait(20);
  Actor_WatchAndSetScript(ACTOR_CANNON, gMainRoutineWork.leaderActorId);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Event_Wait(20);
  Event_End();
}

/// Meeting Cannon in the sixth room once the Pound pillar is down: he finds
/// himself cornered and turns to fight.
void DehkanPlateau_CannonCornered(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(ACTOR_CANNON, gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmoteById(ACTOR_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirectionAndRefreshById(ACTOR_CANNON, FACING_EAST);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_CANNON, 3);
  Actor_FaceDirectionAndWait(ACTOR_CANNON, FACING_WEST);
  Actor_WatchAndSetScript(ACTOR_CANNON, gMainRoutineWork.leaderActorId);
  FieldEffect_SetAttached(ACTOR_CANNON,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_End();
}

/// Stepping toward Cannon in the sixth room, cornered or not.
void DehkanPlateau_MeetCannon(void) {
  if (GameFlag_Get(FLAG_CONE_POUNDED) != 0)
    DehkanPlateau_CannonCornered();
  else
    DehkanPlateau_CannonCrumblesLastPillar();
  GameFlag_Set(FLAG_CANNON_MET);
}

/// Picking up the Pound Cube Cannon dropped.
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

/// A room's exit: the leader walks out and the screen changes.
void DehkanPlateau_LeaveRoom(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  if (gMainRoutineWork.next == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(work->stepTrigger);
}

s32 Scene_Initialize(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    DehkanPlateau_InitializeFullMetalVestChest();
  else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
    DehkanPlateau_InitializeThemisAxeChest();
  else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
    DehkanPlateau_InitializeTightropes();
  else if (scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
    DehkanPlateau_InitializeMintChest();
  else if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
    DehkanPlateau_InitializePoundCube();
  else if (scene == SCENE_DEHKAN_PLATEAU_CANNON)
    DehkanPlateau_InitializeCannon();
  return 0;
}

/// The first room's task: draws the chest on the ledge at the leader's sprite
/// priority each frame, so it stays in front of or behind him with the ground.
void DehkanPlateau_KeepPriorityWithLeader(void) {
  Actor_SetSpritePriority(
      ACTOR_FULL_METAL_VEST_CHEST,
      ((struct SpriteAttr2High *)Actor_GetById(gMainRoutineWork.leaderActorId)
           ->sprite)
          ->priority);
}

void DehkanPlateau_RestoreFullMetalVestChestCracks(void);

/// Sets up the first room: its pillars, the stone pillar already in the gap,
/// its cracks and covers, and the chest's task.
void DehkanPlateau_InitializeFullMetalVestChest(void) {
  CrumblingPillars_Place(gDehkanPlateau_FullMetalVestChestCrumblingPillars);
  Actor_SetSpritePriority(ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR,
                          3);
  Pillars_Place(gDehkanPlateau_FullMetalVestChestPoundPillars);
  Actor_DisableById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
  if (GameFlag_Get(FLAG_AREA_TEMP_01) != 0) {
    MapGrid_CopyCellAttributes(54, 32, 2, 1, 56, 64);
    Actor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR), toFIXED(904),
        toFIXED(64), toFIXED(520));
    wait(10);
  }
  DehkanPlateau_RestoreFullMetalVestChestCracks();
  DehkanPlateau_HideFullMetalVestChestActors();
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_Add(DehkanPlateau_KeepPriorityWithLeader, priority);
  }
}

void DehkanPlateau_RestoreThemisAxeChestCracks(void);

/// Sets up the second room: its grey, Pound and stone pillars, its cracks, and
/// Cannon as far as he has fled.
void DehkanPlateau_InitializeThemisAxeChest(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    if (gMainRoutineWork.entrance == 5) {
      Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
      Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
          ACTOR_PRIORITY_AUTOMATIC;
    }
  }
  CrumblingPillars_PlaceWithStandIns(
      gDehkanPlateau_ThemisAxeChestCrumblingPillars);
  DehkanPlateau_SetThemisAxeChestStandingPillarCells();
  Pillars_Place(gDehkanPlateau_ThemisAxeChestPoundPillars);
  if (GameFlag_Get(FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED) != 0) {
    Actor_DisableById(ACTOR_THEMIS_AXE_CHEST_PILLAR);
    MapGrid_CopyCellAttributes(50, 7, 1, 1, 52, 7);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_THEMIS_AXE_CHEST_PILLAR), toFIXED(840), 0,
        toFIXED(120));
  } else {
    Actor_PlaceBlocks(gDehkanPlateau_ThemisAxeChestMovablePillars);
  }
  DehkanPlateau_RestoreThemisAxeChestCracks();
  if (GameFlag_Get(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
    s32 angle = FACING_SOUTH;

    Actor_PlaceFacing(ACTOR_THEMIS_AXE_CHEST_CANNON, toFIXED(888), toFIXED(168),
                      angle);
    MapGrid_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
    GameFlag_Set(FLAG_SCENE_TEMP_47);
  }
  if (GameFlag_Get(FLAG_CANNON_FLED_EAST) != 0)
    Actor_SetPosition(ACTOR_THEMIS_AXE_CHEST_CANNON, 0, 0);
  DehkanPlateau_HideThemisAxeChestActors();
}

extern s16 gDehkanPlateau_TightropesRaisedPillars[];

void DehkanPlateau_ForgetCrackOnLeaving(void);

/// The room entry written in the scene layer of
/// DehkanPlateau_InitializeThemisAxeChest and
/// DehkanPlateau_InitializePoundCube: sets actors' variants, links, cells and
/// flags for the room.
void DehkanPlateau_InitializeTightropes(void) {
  struct Actor *object;
  FIXED zero, value;

  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST, 1);
  /* FAKEMATCH(dead): the form kept; as no block, 2 of 276 instructions differ
   * in tla-en. */
  do {
  } while (0);
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST,
                  ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST);
  Tightrope_Place(1, ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST,
                  ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_ROPE_LOW_POST, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_ROPE_HIGH_POST, 1);
  DehkanPlateau_InitializeRopes();
  Rope_Place(0, FLAG_TIGHTROPES_ROPE_LASHED, ACTOR_TIGHTROPES_ROPE_LOW_POST,
             ACTOR_TIGHTROPES_ROPE_HIGH_POST);
  Actor_PlaceBlocks(gDehkanPlateau_TightropesMovablePillars);
  if (Actor_GetById(ACTOR_TIGHTROPES_PILLAR)->y.fixed > (s32)-toFIXED(8))
    Actor_SetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 1);
  else
    Actor_SetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 2);
  Actor_DisableById(ACTOR_TIGHTROPES_SECOND_PILLAR);
  Actor_GetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_SECOND_PILLAR, 2);
  if (GameFlag_Get(FLAG_TIGHTROPES_SECOND_PILLAR_PLACED) != 0)
    DehkanPlateau_SetTightropesSecondPillarInPlace();
  else
    MapGrid_CopyCellAttributes(38, 32, 1, 1, 38, 33);
  Actor_GetById(ACTOR_TIGHTROPES_FIRST_RAISED_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_SECOND_RAISED_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_THIRD_RAISED_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_FOURTH_RAISED_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Pillars_Raise(gDehkanPlateau_TightropesRaisedPillars);
  {
    struct Actor *pillar = Actor_GetById(ACTOR_TIGHTROPES_FOURTH_RAISED_PILLAR);

    value = pillar->x.fixed;
    zero = 0;
    if (FIELD_CELL(value) == 54) {
      struct MapCell *cell = Ram_HeapSlots->mapGrid->layers[0].cells;

      cell += Actor_GetCellIndex(pillar);
      cell->attributeB &= 0x7f;
      cell->collisionCode = 11;
    }
  }
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_FIRST_RAISED_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_SECOND_RAISED_PILLAR, 1);
  DehkanPlateau_RestoreTightropesCracks();
  if (GameFlag_Get(FLAG_CANNON_LEAPT_AWAY) != 0) {
    s32 angle = FACING_SOUTH;

    Actor_SetPositionAndFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(424),
                               toFIXED(104), angle);
  }
  Actor_SetAnimationById(ACTOR_TIGHTROPES_POUND_PILLAR, 3);
  if (GameFlag_Get(FLAG_CANNON_POUNDED_PILLAR) != 0) {
    object = Actor_GetById(ACTOR_TIGHTROPES_POUND_PILLAR);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetAnimationById(ACTOR_TIGHTROPES_POUND_PILLAR, 4);
    object->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    object->collisionFlags &= ~ACTOR_SOLID;
    GameFlag_Set(FLAG_TIGHTROPES_PILLAR_POUNDED);
    GameFlag_Set(FLAG_SCENE_TEMP_41);
    MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      s32 angle = FACING_SOUTH;

      Actor_SetPositionAndFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(616),
                                 toFIXED(280), angle);
    }
  }
  if (GameFlag_Get(FLAG_CANNON_CRUMBLED_PILLARS) != 0) {
    GameFlag_Set(FLAG_SCENE_TEMP_10);
    GameFlag_Set(FLAG_SCENE_TEMP_16);
    Actor_SetPosition(ACTOR_TIGHTROPES_CANNON, 0, 0);
  }
  CrumblingPillars_Place(gDehkanPlateau_TightropesCrumblingPillars);
  DehkanPlateau_SetTightropesStandingPillarCells();
  DehkanPlateau_HideTightropesActors();
  {
    struct Actor *last = Actor_GetById(ACTOR_TIGHTROPES_FIRST_COVER + 1);

    last->motionFlags = zero;
    last->y.fixed = zero;
  }
  gDehkanPlateau_CrackX = zero;
  gDehkanPlateau_CrackZ = zero;
  Task_Add(DehkanPlateau_ForgetCrackOnLeaving, TASK_PASS_VBLANK);
}

extern u16 gDehkanPlateau_FallLandingCells[];

#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, sourceX, sourceZ,            \
                                            columnSourceX, columnSourceZ,      \
                                            belowOffsetZ, belowSourceZ)        \
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
        MapGrid_CopyCellsTo(sourceX, sourceZ, x, y, one, one);                 \
        x = entry[0];                                                          \
        y = entry[1];                                                          \
        MapGrid_CopyCellsTo(columnSourceX, columnSourceZ, x, y, one, 5);       \
        if (flag == 1)                                                         \
          MapGrid_CopyCellsTo(67, 4, 69, 3, one, one);                         \
        x -= 64;                                                               \
        y += belowOffsetZ;                                                     \
        MapGrid_CopyCellsTo(sourceX, belowSourceZ, x, y, one, 4);              \
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
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawMintChestFallLanding, 3,
                                    8, 67, 3, 66, 69)

/// Draws, in room 5, the cells where the party landed.
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawPoundCubeFallLanding, 13,
                                    5, 77, 0, 65, 65)

/// Sets up the fourth room, below the cracks: the floor Cannon opened, and the
/// landing of a fall.
void DehkanPlateau_InitializeMintChest(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  DehkanPlateau_HideMintChestActors();
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
  DehkanPlateau_DrawMintChestFallLanding();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 10) {
      if (gMainRoutineWork.entrance <= 39)
        DehkanPlateau_LandFromFall();
    }
  }
}

/// The leader drops in from the crack above, landing in a ring of dust.
void DehkanPlateau_LandFromFall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;
  struct EffectOptions *opts;
  s32 spawn;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(24);
  opts = &options;
  opts->palette = 7;
  opts->update = FieldEffect_AdvanceMotion;
  opts->startScaleX = FIXED_ONE * 16 / 25;
  opts->startScaleY = FIXED_ONE * 16 / 25;
  opts = &options;
  spawn = 1;
  i = 0;
loop:
  {
    s32 angle;
    FIXED velocity[3];
    FIXED cosine;

    angle = i << 12;
    cosine = Trig_Cos(angle);
    if (spawn) {
      velocity[0] = cosine;
      velocity[1] = 0;
      velocity[2] = Trig_Sin(angle);
      velocity[0] += velocity[0] >> 1;
      velocity[0] += ((Random16() * 3) << 14) >> 16;
      velocity[0] += -(toFIXED(3) / 8);
      velocity[2] += (((Random16() * 3) << 13) >> 16) + -(toFIXED(3) / 16);
      FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                        velocity[0], velocity[1], velocity[2],
                        EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                            EFFECT_USE_PALETTE | 1,
                        opts);
    }
    i++;
  }
  if (i <= 16)
    goto loop;
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  Event_Wait(10);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_STAND);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

/// Sets up the fifth room: the Pound Cube where Cannon dropped it, its cracks,
/// and the landing of a fall, beside Cannon the first time.
void DehkanPlateau_InitializePoundCube(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 entrance;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  owner->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  if (GameFlag_Get(FLAG_POUND_CUBE_TAKEN) == 0) {
    DehkanPlateau_CreatePoundCube();
    if (GameFlag_Get(FLAG_CANNON_DROPPED_POUND_CUBE) != 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 15, 5);
      if (gDehkanPlateau_PoundCube != NULL) {
        FieldActor_SetPositionAndResetMotion(gDehkanPlateau_PoundCube,
                                             toFIXED(248), 0, toFIXED(88));
        wait(1);
      }
    }
  }
  Pillars_Place(gDehkanPlateau_PoundCubePoundPillars);
  DehkanPlateau_RestorePoundCubeCracks();
  entrance = gMainRoutineWork.entrance;
  if (entrance > 10) {
    DehkanPlateau_ForgetFall();
    GameFlag_Set(entrance + FLAG_FELL_THROUGH_FIRST - FIRST_LANDING_ENTRANCE);
  }
  if (Flag_Test(FLAG_CANNON_DROPPED_POUND_CUBE) != 0)
    Actor_SetPosition(ACTOR_POUND_CUBE_CANNON, 0, 0);
  DehkanPlateau_DrawPoundCubeFallLanding();
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

/// Creates the Pound Cube Cannon drops, drawn with the item's own icon.
void DehkanPlateau_CreatePoundCube(void) {
  struct Actor **slot = &gDehkanPlateau_PoundCube;
  s32 zero = 0;
  struct Actor *object =
      ((struct Actor *)Import_ActorCreate(SPRITE_UNSEEN_MARKER, 0, 0, 0));

  *slot = object;
  if (object != 0) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->retainSpriteCache = 1;
    sprite->flags = zero;
    sprite->countAndShadow[0] = zero;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_POUND_CUBE);
    VramCache_Load(sprite->slot, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Sets up the sixth room: its Pound pillar and grey pillar, Cannon met already
/// if he is in the party, and the landing of a fall.
void DehkanPlateau_InitializeCannon(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_CANNON)) != 0)
    GameFlag_Set(FLAG_CANNON_MET);
  Pillars_Place(gDehkanPlateau_CannonPoundPillars);
  CrumblingPillars_Place(gDehkanPlateau_CannonCrumblingPillar);
  if (GameFlag_Get(FLAG_CONE_POUNDED) == 0) {
    DehkanPlateau_EnterConeCell();
    wait(10);
  }
  DehkanPlateau_HideCannonActors();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance > 5) {
      if (gMainRoutineWork.entrance <= 39)
        DehkanPlateau_LandFromFall();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#undef DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG

void DehkanPlateau_PlaceTightropesPillar(void);
void DehkanPlateau_MoveTightropesSecondPillar(void);
void DehkanPlateau_LeaveConeCell(void);
void DehkanPlateau_CannonFleesEast(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gRubble_SprayScript[] = {
    SC_WAIT(33), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

u32 gRubble_BurstScript[] = {
    SC_WAIT(44), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),
    SC_RELEASE,
};

u32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

u32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
    SC_END,
};

/// The puffs of dust a crumbling pillar raises.
u32 gDehkanPlateau_DustScript[] = {
    SC_WAIT(33),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

/// Each room's grey pillars, with the flag that remembers each one crumbled.
s16 gDehkanPlateau_FullMetalVestChestCrumblingPillars[] = {
    8, 528, 9, 530, 10, 532, -1,
};

s16 gDehkanPlateau_ThemisAxeChestCrumblingPillars[] = {
    11, 528, 12, 530, 13, 532, 14, 534,
    15, 536, 16, 538, 17, 540, 18, 542,
    19, 544, 20, 546, 21, 548, -1,
};

s16 gDehkanPlateau_TightropesCrumblingPillars[] = {
    21, 528, 22, 530, 23, 532, 24, 534,
    -1,
};

s16 gDehkanPlateau_CannonCrumblingPillar[] = {
    9, 528, -1,
};

/// Each room's Pound pillars, with the flag that remembers each one Pounded.
s16 gDehkanPlateau_FullMetalVestChestPoundPillars[] = {
    11, 1, 515, -1,
};

s16 gDehkanPlateau_ThemisAxeChestPoundPillars[] = {
    22, 1, 516, -1,
};

s16 gDehkanPlateau_TightropesPoundPillars[] = {
    18, 1, 517, -1,
};

s16 gDehkanPlateau_PoundCubePoundPillars[] = {
    8, 1, 518, -1,
};

s16 gDehkanPlateau_CannonPoundPillars[] = {
    8, 1, 519, -1,
};

/// The stone pillars Move takes hold of.
s16 gDehkanPlateau_ThemisAxeChestMovablePillars[] = {
    23, -1,
};

s16 gDehkanPlateau_TightropesMovablePillars[] = {
    19, -1,
};

/// The third room's pillars on raised ground.
s16 gDehkanPlateau_TightropesRaisedPillars[] = {
    14, 15, 16, 17, -1,
};

/// The map pictures a room's crack is drawn with: cracked open, then as a hole.
u16 gDehkanPlateau_FullMetalVestChestCrackPictures[] = {
    4, 33, 5, 33,
};

u16 gDehkanPlateau_ThemisAxeChestCrackPictures[] = {
    68, 33, 69, 33,
};

u16 gDehkanPlateau_TightropesCrackPictures[] = {
    69, 51, 70, 51,
};

u16 gDehkanPlateau_PoundCubeCrackPictures[] = {
    65, 64, 64, 64,
};

/// The pictures of the hole under each crack, drawn as the leader falls.
u16 gDehkanPlateau_FullMetalVestChestHolePictures[] = {
    1, 36, 2, 36, 3, 36, 4, 36,
    5, 36, 6, 36, 7, 36, 8, 36,
};

u16 gDehkanPlateau_ThemisAxeChestHolePictures[] = {
    65, 36, 66, 36, 67, 36, 68, 36,
    69, 36, 70, 36, 71, 36, 72, 36,
};

u16 gDehkanPlateau_TightropesHolePictures[] = {
    66, 54, 67, 54, 68, 54, 69, 54,
    70, 54, 71, 54, 72, 54, 73, 54,
};

u16 gDehkanPlateau_PoundCubeHolePictures[] = {
    64, 66, 65, 66, 66, 66, 67, 66,
    68, 66,
};

/// Each room's cracks: whether it starts cracked, its cell, its hole picture and
/// where the hole is drawn below.
u16 gDehkanPlateau_FullMetalVestChestCracks[][6] = {
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

u16 gDehkanPlateau_ThemisAxeChestCracks[][6] = {
    {1, 105, 4, 0, 41, 36},
    {0, 109, 17, 0, 45, 49},
    {0, 110, 19, 3, 46, 51},
    {0, 111, 17, 3, 47, 49},
    {0, 112, 21, 0, 48, 53},
    {0, 113, 20, 3, 49, 52},
    {0, 114, 18, 0, 50, 50},
    {0, 105, 6, 0, 41, 38},
};

u16 gDehkanPlateau_TightropesCracks[][6] = {
    {1, 91, 11, 0, 27, 43},
    {0, 92, 11, 0, 28, 43},
    {1, 93, 9, 0, 29, 41},
    {1, 93, 12, 0, 29, 44},
    {1, 109, 7, 0, 45, 39},
    {1, 112, 6, 0, 48, 38},
};

u16 gDehkanPlateau_PoundCubeCracks[][6] = {
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

/// Where each fall lands in the rooms below.
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
    SCENE_EXITS(DehkanPlateauFullMetalVestChest),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_SOUTHEAST),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST),
    SCENE_EXIT(6, DehkanPlateauFullMetalVestChest, 7),
    SCENE_EXIT(7, DehkanPlateauFullMetalVestChest, 6),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_D, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_D),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_E, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_E),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_F, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_F),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_G, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_G),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_D, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_D),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_E, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_E),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_F, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_F),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_G, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_G),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_H, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_H),
    SCENE_EXIT(DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST_I, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_EAST_I),
    SCENE_EXITS(DehkanPlateauThemisAxeChest),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_EAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_EAST),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_TIGHTROPES, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST),
    SCENE_EXIT(5, DehkanPlateauThemisAxeChest, 6),
    SCENE_EXIT(6, DehkanPlateauThemisAxeChest, 5),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_WEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_WEST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHWEST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_NORTHWEST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_THEMIS_AXE_CHEST_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_C, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_C),
    SCENE_EXITS(DehkanPlateauTightropes),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTH, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTH, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_DEHKAN_PLATEAU_TIGHTROPES),
    SCENE_EXIT(7, DehkanPlateauTightropes, 8),
    SCENE_EXIT(8, DehkanPlateauTightropes, 7),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTHWEST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTH_A, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH_A),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_NORTHEAST, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_MINT_CHEST_SOUTH_B, DehkanPlateauMintChest, DEHKAN_PLATEAU_MINT_CHEST_FROM_DEHKAN_PLATEAU_TIGHTROPES_SOUTH_B),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTHWEST),
    SCENE_EXIT(DEHKAN_PLATEAU_TIGHTROPES_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTH, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH),
    SCENE_EXITS(DehkanPlateauMintChest),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_A, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_WEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_WEST_B, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_WEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_TIGHTROPES_NORTH, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_MINT_CHEST_NORTH),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_SOUTHEAST, DehkanPlateauFullMetalVestChest, DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_SOUTHEAST_A, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_SOUTHEAST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_MINT_CHEST_TO_DEHKAN_PLATEAU_THEMIS_AXE_CHEST_EAST, DehkanPlateauThemisAxeChest, DEHKAN_PLATEAU_THEMIS_AXE_CHEST_FROM_DEHKAN_PLATEAU_MINT_CHEST_EAST),
    SCENE_EXIT(40, DehkanPlateauMintChest, 42),
    SCENE_EXIT(41, DehkanPlateauMintChest, 43),
    SCENE_EXIT(42, DehkanPlateauMintChest, 40),
    SCENE_EXIT(43, DehkanPlateauMintChest, 41),
    SCENE_EXITS(DehkanPlateauPoundCube),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_NORTHEAST, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHEAST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_SOUTHWEST, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_TIGHTROPES_SOUTH, DehkanPlateauTightropes, DEHKAN_PLATEAU_TIGHTROPES_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHEAST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_A),
    SCENE_EXIT(7, DehkanPlateauPoundCube, 10),
    SCENE_EXIT(8, DehkanPlateauPoundCube, 9),
    SCENE_EXIT(9, DehkanPlateauPoundCube, 8),
    SCENE_EXIT(10, DehkanPlateauPoundCube, 7),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHWEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHWEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHWEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_WEST_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_WEST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHWEST_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHWEST_C),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTH_A, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH_A),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_C, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_C),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTH_D, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTH_D),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_NORTHEAST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTH_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTH_B),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_EAST, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_EAST),
    SCENE_EXIT(DEHKAN_PLATEAU_POUND_CUBE_TO_DEHKAN_PLATEAU_CANNON_SOUTHEAST_B, DehkanPlateauCannon, DEHKAN_PLATEAU_CANNON_FROM_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_B),
    SCENE_EXITS(DehkanPlateauCannon),
    SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_WEST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_WEST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_NORTHEAST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_NORTHEAST_A),
    SCENE_EXIT(DEHKAN_PLATEAU_CANNON_TO_DEHKAN_PLATEAU_POUND_CUBE_SOUTHEAST_A, DehkanPlateauPoundCube, DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_CANNON_SOUTHEAST_A),
    SCENE_EXIT(40, DehkanPlateauCannon, 41),
    SCENE_EXIT(41, DehkanPlateauCannon, 40),
    SCENE_EXIT(42, DehkanPlateauCannon, 43),
    SCENE_EXIT(43, DehkanPlateauCannon, 42),
    SCENE_EXITS_END,
};

/// The placements of a room with none of its own.
struct ActorPlacement gDehkanPlateau_Placements[] = {
    {SCENE_TABLE_END},
};

/// Each room's actors (enum DehkanPlateauFullMetalVestChestActorId and the rest).
struct ActorPlacement gDehkanPlateau_FullMetalVestChestPlacements[] = {
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_ThemisAxeChestPlacements[] = {
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
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_TightropesPlacements[] = {
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
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_MintChestPlacements[] = {
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_PoundCubePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_CannonPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(409, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(337, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CANNON), MOVEMENT_TYPE_STAND, 728, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The triggers of a room with none of its own.
struct Trigger gDehkanPlateau_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// Each room's exits, cracks, pillars and Cannon's flight.
struct Trigger gDehkanPlateau_FullMetalVestChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(DehkanPlateau_LeaveRoom)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnFullMetalVestChestPillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FULL_METAL_VEST_CHEST_POUND_PILLAR, FLAG_FULL_METAL_VEST_CHEST_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundFullMetalVestChestPillar)},
    {TRIGGER_PUSHED, 0, FLAG_AREA_TEMP_01, EVENT(DehkanPlateau_DropFullMetalVestChestPillarIntoGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FULL_METAL_VEST_CHEST_PILLAR, FLAG_AREA_TEMP_01, EVENT(DehkanPlateau_DropFullMetalVestChestPillarIntoGap)},
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

struct Trigger gDehkanPlateau_ThemisAxeChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(DehkanPlateau_LeaveRoom)},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 6},
    {TRIGGER_STEP, 30, FLAG_CANNON_FLED_OVER_CRACKS, EVENT(DehkanPlateau_CannonFleesOverCracks)},
    {TRIGGER_STEP, 31, FLAG_CANNON_FLED_EAST, EVENT(DehkanPlateau_CannonFleesEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 8, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnThemisAxeChestPillars)},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnThemisAxeChestPillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_THEMIS_AXE_CHEST_POUND_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundThemisAxeChestPillar)},
    {TRIGGER_PUSH, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_LiftThemisAxeChestPillar)},
    {TRIGGER_PUSHED, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceThemisAxeChestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_LiftThemisAxeChestPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceThemisAxeChestPillar)},
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

struct Trigger gDehkanPlateau_TightropesTriggers[] = {
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
    {TRIGGER_STEP, 32, FLAG_CANNON_CRUMBLED_PILLARS, EVENT(DehkanPlateau_CannonCrumblesPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TIGHTROPES_ROPE_LOW_POST, FLAG_TIGHTROPES_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_TIGHTROPES_POUND_PILLAR, FLAG_TIGHTROPES_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundTightropesPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceTightropesPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceTightropesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_PILLAR, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_MoveTightropesSecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 9, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_PushTightropesSecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT(DehkanPlateau_PushRaisedPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(DehkanPlateau_PushRaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FIRST_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesRaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SECOND_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesRaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_THIRD_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesRaisedPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FOURTH_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FIRST_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_THIRD_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FOURTH_RAISED_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingRaisedPillar)},
    {STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
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

struct Trigger gDehkanPlateau_MintChestTriggers[] = {
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

struct Trigger gDehkanPlateau_PoundCubeTriggers[] = {
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_POUND_CUBE_POUND_PILLAR, FLAG_POUND_CUBE_PILLAR_POUNDED, EVENT(DehkanPlateau_PoundPoundCubePillar)},
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

struct Trigger gDehkanPlateau_CannonTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 40, CONDITION_ALWAYS, 40},
    {TRIGGER_EXIT, 41, CONDITION_ALWAYS, 41},
    {TRIGGER_EXIT, 42, CONDITION_ALWAYS, 42},
    {TRIGGER_EXIT, 43, CONDITION_ALWAYS, 43},
    {STEP_TRIGGER_WALKING, 8, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnCannonPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_CANNON_ROOM_POUND_PILLAR, FLAG_CONE_POUNDED, EVENT(DehkanPlateau_PoundCannonPillar)},
    {TRIGGER_STEP, 9, FLAG_IN_CONE_CELL, EVENT(DehkanPlateau_EnterConeCell)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_IN_CONE_CELL), EVENT(DehkanPlateau_LeaveConeCell)},
    {TRIGGER_STEP, 5, FLAG_ACTORS_HIDDEN, EVENT(DehkanPlateau_HideActors)},
    {TRIGGER_STEP, 6, IF_FLAG_SET(FLAG_ACTORS_HIDDEN), EVENT(DehkanPlateau_ShowActors)},
    {TRIGGER_STEP, 11, FLAG_CANNON_MET, EVENT(DehkanPlateau_MeetCannon)},
    {TRIGGER_TALK, ACTOR_CANNON, CONDITION_ALWAYS, EVENT(DehkanPlateau_FightCannon)},
    {TRIGGER_TABLE_END},
};
