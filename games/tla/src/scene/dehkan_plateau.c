/// @file
/// The Dehkan Plateau, the cliffs and caves of Indra between Daila and the East
/// Indra Shore, in seven rooms that share this overlay. Its floors are cracked:
/// the first step on a crack opens it, the next drops the leader through to the
/// cave below (DehkanPlateau_StepOnCrack), and each room puts back the cracks
/// its flags remember (DehkanPlateau_RestoreFullMetalVestChestCracks). Gray
/// crumbling pillars crack under the leader and crumble at his next step
/// (CrumblingPillar_Step), and the Mars Djinni Cannon runs ahead of the party
/// room by room, cracking floors and pillars and casting Pound
/// (DehkanPlateau_CannonCracksPillars), until a fall lands the leader beside
/// it and it drops the Pound Cube (DehkanPlateau_CannonDropsPoundCube);
/// cornered at last once the party Pounds the stake before it into the
/// ground, it fights (DehkanPlateau_FightCannon).

#include "types.h"
#include "macros.h"
#include "item_ids.h"
#include "system/system.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/field.h"
#include "field/map_grid.h"
#include "field/field_sequence.h"
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
#include "graphics/glyph.h"
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
#define FLAG_CANNON_POUNDED_STAKE 0x86f    ///< room 3, Pounding a stake down
#define FLAG_CANNON_CRACKED_PILLARS 0x870  ///< room 3, cracking two pillars
/// Room 5: Cannon drops the Pound Cube, and the party takes it.
#define FLAG_CANNON_DROPPED_POUND_CUBE 0x871
#define FLAG_POUND_CUBE_TAKEN 0x878
/// The pillars Moved into place in rooms 2 and 3.
#define FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED 0x840
#define FLAG_TIGHTROPES_SECOND_PILLAR_PLACED 0x841

// Each room's actors, numbering its placement table. A room's gray crumbling
// pillars crack under the leader and crumble at his next step
// (CrumblingPillar_Step), its stakes sink when Pounded
// (Pound_SinkTarget), and its stone pillars are Moved. Its unseen blocks draw
// nothing, but while they are drawn the leader among them shows in front of
// the cliff there (DehkanPlateau_SetBlocksShown). Chests number from 64.

/// The first room, where cracks drop the party to the caves below.
enum DehkanPlateauFullMetalVestChestActorId {
  ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED,
  ACTOR_FULL_METAL_VEST_CHEST_STAKE = ACTOR_FIRST_PLACED + 3,
  /// The stone pillar Moved over the edge into the gap below.
  ACTOR_FULL_METAL_VEST_CHEST_PILLAR,
  ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK,
  ACTOR_FULL_METAL_VEST_CHEST = 64
};

/// The second room, where Cannon is first seen and flees over the cracks.
enum DehkanPlateauThemisAxeChestActorId {
  ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR = ACTOR_FIRST_PLACED + 3,
  ACTOR_THEMIS_AXE_CHEST_STAKE = ACTOR_FIRST_PLACED + 14,
  /// The stone pillar Moved into the gap.
  ACTOR_THEMIS_AXE_CHEST_PILLAR,
  ACTOR_THEMIS_AXE_CHEST_CANNON,
  ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK
};

/// The third room, of tightropes, a rope to Lash and the crumbling pillars
/// Cannon leaps across.
enum DehkanPlateauTightropesActorId {
  /// The posts at the ends of the two tightropes, and of the rope, whose low
  /// post Lash ties it from.
  ACTOR_TIGHTROPES_FIRST_TIGHTROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_TIGHTROPES_FIRST_TIGHTROPE_EAST_POST,
  ACTOR_TIGHTROPES_SECOND_TIGHTROPE_WEST_POST,
  ACTOR_TIGHTROPES_SECOND_TIGHTROPE_EAST_POST,
  ACTOR_TIGHTROPES_ROPE_LOW_POST,
  ACTOR_TIGHTROPES_ROPE_HIGH_POST,
  /// The four stone pillars wrapped in vines, which the leader climbs and
  /// pushes or Moves into the gaps.
  ACTOR_TIGHTROPES_FIRST_VINE_PILLAR,
  ACTOR_TIGHTROPES_SECOND_VINE_PILLAR,
  ACTOR_TIGHTROPES_THIRD_VINE_PILLAR,
  ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR,
  /// The stake Cannon Pounds down as he flees.
  ACTOR_TIGHTROPES_STAKE,
  /// The stone pillar Moved on the raised ground.
  ACTOR_TIGHTROPES_PILLAR,
  /// The stone pillar at the foot of the cliff, pushed off the vine behind it.
  ACTOR_TIGHTROPES_SECOND_PILLAR,
  ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
  ACTOR_TIGHTROPES_CANNON = ACTOR_FIRST_PLACED + 17,
  ACTOR_TIGHTROPES_FIRST_BLOCK
};

/// The fourth room, a passage.
enum DehkanPlateauMintChestActorId {
  ACTOR_MINT_CHEST_FIRST_BLOCK = ACTOR_FIRST_PLACED
};

/// The fifth room, where a fall lands the leader beside Cannon and he drops
/// the Pound Cube.
enum DehkanPlateauPoundCubeActorId {
  ACTOR_POUND_CUBE_STAKE = ACTOR_FIRST_PLACED,
  ACTOR_POUND_CUBE_CANNON
};

/// The sixth room, where Cannon is cornered behind the stake and
/// fights.
enum DehkanPlateauCannonActorId {
  ACTOR_CANNON_ROOM_STAKE = ACTOR_FIRST_PLACED,
  ACTOR_CANNON_ROOM_CRUMBLING_PILLAR,
  ACTOR_CANNON_ROOM_FIRST_BLOCK,
  ACTOR_CANNON = ACTOR_FIRST_PLACED + 8
};

/// The unseen blocks of each room (Actors_SetDrawn, Actor_Hide).
enum {
  FULL_METAL_VEST_CHEST_BLOCK_COUNT = 14,
  THEMIS_AXE_CHEST_BLOCK_COUNT = 13,
  TIGHTROPES_BLOCK_COUNT = 10,
  MINT_CHEST_BLOCK_COUNT = 3,
  CANNON_ROOM_BLOCK_COUNT = 6
};

/// For this visit: the room's unseen blocks drawn
/// (DehkanPlateau_RestoreBlocks).
#define FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN FLAG_SCENE_TEMP_00
/// The leader stands on the crack he has just opened; set until he steps off.
#define FLAG_STANDING_ON_OPENED_CRACK FLAG_SCENE_TEMP_01
/// Room 6's stake's cell holds the attributes
/// DehkanPlateau_EnterLowerPath gives it while the leader is on the lower path.
#define FLAG_STAKE_CELL_CHANGED FLAG_SCENE_TEMP_02
/// Each room's stake Pounded into the ground, by its list's flag.
#define FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED FLAG_SCENE_TEMP_03
#define FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED FLAG_SCENE_TEMP_04
#define FLAG_TIGHTROPES_STAKE_POUNDED FLAG_SCENE_TEMP_05
#define FLAG_POUND_CUBE_STAKE_POUNDED FLAG_SCENE_TEMP_06
/// Room 6's stake, Pounded into the ground so Cannon is cornered.
#define FLAG_CANNON_ROOM_STAKE_POUNDED FLAG_SCENE_TEMP_07
/// Room 3's rope, tied by Lash from its low post to its high one.
#define FLAG_TIGHTROPES_ROPE_LASHED FLAG_SCENE_TEMP_08
/// Cannon met in room 6 on this visit, or already in the party.
#define FLAG_CANNON_MET FLAG_SCENE_TEMP_09
/// The first room's stone pillar, Moved into the gap, where it stays.
#define FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP FLAG_AREA_TEMP_01
/// A room's crumbling pillars, two flags to a pillar in its list's order: the
/// first set once it cracks, the next once it crumbles (CrumblingPillar_Step).
#define FLAG_PILLAR_CRACKED(n) (FLAG_SCENE_TEMP_10 + 2 * (n))
#define FLAG_PILLAR_CRUMBLED(n) (FLAG_PILLAR_CRACKED(n) + 1)
/// The first of a room's crack flags, one a listed crack, set while the crack
/// is open (DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS).
#define FLAG_CRACK_OPENED_FIRST FLAG_SCENE_TEMP_40
#define FLAG_CRACK_OPENED(n) (FLAG_CRACK_OPENED_FIRST + (n))
/// The crack the party fell through, one flag for each landing entrance from
/// FIRST_LANDING_ENTRANCE, for the landing drawn below it.
#define FLAG_FELL_THROUGH_FIRST FLAG_AREA_TEMP_20
#define FLAG_FELL_THROUGH_LAST FLAG_AREA_TEMP_5F
#define FIRST_LANDING_ENTRANCE 11
#define LAST_LANDING_ENTRANCE 39
#define FLAG_FELL_THROUGH(entrance)                                            \
  (FLAG_FELL_THROUGH_FIRST + (entrance) - FIRST_LANDING_ENTRANCE)
/// Entrances from these up land a fall: the Pound Cube room's 36 and 37, and
/// Cannon's room's 8 to 23.
#define POUND_CUBE_LANDINGS_FROM 17
#define CANNON_ROOM_LANDINGS_FROM 6

/// The step code of each room's first crack, its list's first row; the rest
/// follow in order, and a fall leaves by the exit of its crack's code.
#define TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK 11
#define TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK 14
#define TRIGGER_TIGHTROPES_FIRST_CRACK 12
#define TRIGGER_POUND_CUBE_FIRST_CRACK 18
/// The fifth room's crack Cannon Pounds the leader through, its sixth.
#define TRIGGER_POUND_CUBE_CANNON_CRACK (TRIGGER_POUND_CUBE_FIRST_CRACK + 5)
/// The third room's step code that runs DehkanPlateau_StepOnTightropesPillars
/// on walking west, which a vine pillar pushed beside the crumbling pillars
/// takes.
#define TRIGGER_TIGHTROPES_PILLARS_WEST 11

/// The frames the leader takes to sink through a crack.
#define FALL_FRAMES 40

/// The cracks of each room's list.
enum {
  FULL_METAL_VEST_CHEST_CRACKS = 14,
  THEMIS_AXE_CHEST_CRACKS = 8,
  TIGHTROPES_CRACKS = 6,
  POUND_CUBE_CRACKS = 16
};

/// The fifth room's two cracks that have another just south of them, the next
/// in its list; once both are open, the hole drawn below is JOINED_HOLE of
/// gDehkanPlateau_PoundCubeHolePictures in place of its own.
enum {
  POUND_CUBE_WEST_UPPER_CRACK = 0,
  POUND_CUBE_EAST_UPPER_CRACK = 11,
  JOINED_HOLE = 4
};

void Actor_DisableById(s32 id);
extern s16 gDehkanPlateau_FullMetalVestChestCrumblingPillars[];
/// The Pound Cube Cannon drops, while it lies on the ground.
extern struct Actor *gDehkanPlateau_PoundCube;
void DehkanPlateau_LeaveLowerPath(void);
void DehkanPlateau_ForgetFall(void);
void Actor_StartScript(struct Actor *actor, const void *script);

#include "shared/crumbling_pillar.h"

#include "../field/field_effect_advance_motion_priority.c"

#include "shared/rubble.h"

#include "shared/actor_set_cell_shape.h"

#include "shared/leader_set_cell_shape.h"

#include "shared/pillar_raise.h"

#include "shared/pillar_lower_held_cells.h"

#include "shared/held_actor_raise_cell.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#undef EFFECT_SET_SCRIPT

extern u16 gDehkanPlateau_FullMetalVestChestCrackPictures[];
extern u16 gDehkanPlateau_FullMetalVestChestCracks[][6];

extern s16 gDehkanPlateau_ThemisAxeChestCrumblingPillars[];
extern s16 gDehkanPlateau_TightropesCrumblingPillars[];
extern s16 gDehkanPlateau_CannonCrumblingPillar[];
extern s16 gDehkanPlateau_FullMetalVestChestStakes[];
extern s16 gDehkanPlateau_ThemisAxeChestStakes[];
extern s16 gDehkanPlateau_TightropesStakes[];
extern s16 gDehkanPlateau_PoundCubeStakes[];
extern s16 gDehkanPlateau_CannonStakes[];
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

/// Stepping onto one of the first room's crumbling pillars: the first step
/// cracks it, the next crumbles it under the leader.
void DehkanPlateau_StepOnFullMetalVestChestPillars(void) {
  CrumblingPillar_Step(gDehkanPlateau_FullMetalVestChestCrumblingPillars);
}

/// Gives the cells of four of room 2's crumbling pillars, while each stands,
/// the attributes of cell (0, 0).
void DehkanPlateau_SetThemisAxeChestStandingPillarCells(void) {
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(1)) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 9, 18);
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(2)) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 11, 18);
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(8)) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 14, 28);
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(9)) == 0)
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 16, 28);
}

/// Stepping onto one of the second room's crumbling pillars; on the low ones
/// the leader is drawn behind the ledge.
void DehkanPlateau_StepOnThemisAxeChestPillars(void) {
  FIXED y;

  CrumblingPillar_StepWithStandIn(
      gDehkanPlateau_ThemisAxeChestCrumblingPillars);
  DehkanPlateau_SetThemisAxeChestStandingPillarCells();
  y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;
  if (y < toFIXED(8)) {
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// The same for two of room 3's crumbling pillars, from cell (63, 0).
void DehkanPlateau_SetTightropesStandingPillarCells(void) {
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(1)) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 50, 20);
  if (GameFlag_Get(FLAG_PILLAR_CRUMBLED(2)) == 0)
    MapGrid_CopyCellAttributes(63, 0, 1, 1, 52, 20);
}

/// Stepping onto one of the third room's crumbling pillars; below the ledge the
/// leader is drawn behind it.
void DehkanPlateau_StepOnTightropesPillars(void) {
  CrumblingPillar_Step(gDehkanPlateau_TightropesCrumblingPillars);
  DehkanPlateau_SetTightropesStandingPillarCells();
  if (Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed < -toFIXED(24)) {
    Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_AUTOMATIC;
  }
}

/// Stepping onto the sixth room's crumbling pillar; from up on it the leader is
/// off the lower path.
void DehkanPlateau_StepOnCannonPillar(void) {
  FIXED y;

  CrumblingPillar_Step(gDehkanPlateau_CannonCrumblingPillar);
  y = Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed;
  if (y < toFIXED(24))
    DehkanPlateau_LeaveLowerPath();
}

/// Draws (show 1) or stops drawing the unseen blocks of the room the party is
/// in.
void DehkanPlateau_SetBlocksShown(s32 show) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
    Actors_SetDrawn(show, ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK,
                    FULL_METAL_VEST_CHEST_BLOCK_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
    Actors_SetDrawn(show, ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK,
                    THEMIS_AXE_CHEST_BLOCK_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
    Actors_SetDrawn(show, ACTOR_TIGHTROPES_FIRST_BLOCK, TIGHTROPES_BLOCK_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_MINT_CHEST)
    Actors_SetDrawn(show, ACTOR_MINT_CHEST_FIRST_BLOCK, MINT_CHEST_BLOCK_COUNT);
  else if (scene == SCENE_DEHKAN_PLATEAU_CANNON)
    Actors_SetDrawn(show, ACTOR_CANNON_ROOM_FIRST_BLOCK,
                    CANNON_ROOM_BLOCK_COUNT);
}

/// Draws the room's unseen blocks again, and notes them shown.
void DehkanPlateau_ShowBlocks(void) {
  DehkanPlateau_SetBlocksShown(1);
  GameFlag_Set(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
}

/// Stops drawing the room's unseen blocks.
void DehkanPlateau_HideBlocks(void) {
  DehkanPlateau_SetBlocksShown(0);
  GameFlag_Clear(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
}

#include "shared/actors_set_hidden.c"

/// Pound on the first room's stake: it sinks into the ground.
void DehkanPlateau_PoundFullMetalVestChestStake(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_FullMetalVestChestStakes, a, b);
}

/// Pound on the second room's stake.
void DehkanPlateau_PoundThemisAxeChestStake(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_ThemisAxeChestStakes, a, b);
}

/// Pound on the third room's stake.
void DehkanPlateau_PoundTightropesStake(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_TightropesStakes, a, b);
}

/// Pound on the fifth room's stake.
void DehkanPlateau_PoundPoundCubeStake(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_PoundCubeStakes, a, b);
}

/// Pound on the sixth room's stake, which corners Cannon.
void DehkanPlateau_PoundCannonStake(s32 a, s32 b) {
  Pound_SinkTarget(gDehkanPlateau_CannonStakes, a, b);
}

#include "shared/actor_drop_and_wait.h"

/// Move on the first room's stone pillar: once it stands at the gap's edge it
/// drops in and stays (FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP).
void DehkanPlateau_DropFullMetalVestChestPillarIntoGap(void) {
  struct Actor *actor = Actor_GetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
  s32 x;

  x = FIELD_CELL(actor->x.fixed);
  if (x == 56 && FIELD_CELL(actor->z.fixed) == 32) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_DropAndWait(actor);
    Map_CopyCellAttributeRect(54, 32, 2, 1, x, 64);
    GameFlag_Set(FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP);
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
  struct Actor *actor = Actor_GetById(ACTOR_THEMIS_AXE_CHEST_PILLAR);
  s32 x = FIELD_CELL(actor->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 52) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(51, 7, 1, 1, x, 7);
    wait(1);
    Actor_DropAndWait(actor);
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
  struct Actor *actor = Actor_GetById(ACTOR_TIGHTROPES_PILLAR);
  s32 x = FIELD_CELL(actor->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 47) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(47, 12, 1, 1, x, 13);
    wait(1);
    Actor_DropAndWait(actor);
    Map_CopyCellAttributeRect(50, 16, 1, 1, x, 13);
    Actor_SetSpritePriority(ACTOR_TIGHTROPES_PILLAR, 2);
    Event_End();
  } else if (x == 46) {
    MapGrid_CopyCellAttributes(44, 13, 1, 1, 48, 13);
  }
}

/// Puts the third room's second stone pillar a column west, off the vine it
/// stood before, with the cells that let it be walked over and the vine
/// climbed.
void DehkanPlateau_SetTightropesSecondPillarInPlace(void) {
  Actor_SetPosition(ACTOR_TIGHTROPES_SECOND_PILLAR, 600, 536);
  MapGrid_CopyCellAttributes(38, 32, 1, 1, 37, 33);
  MapGrid_CopyCellAttributes(30, 32, 1, 2, 38, 32);
  MapGrid_CopyCellAttributes(40, 64, 4, 3, 36, 64);
  GameFlag_Set(FLAG_TIGHTROPES_SECOND_PILLAR_PLACED);
}

/// Move on the third room's second stone pillar: a column west, it settles
/// there.
void DehkanPlateau_MoveTightropesSecondPillar(void) {
  if (FIELD_CELL(Actor_GetById(ACTOR_TIGHTROPES_SECOND_PILLAR)->x.fixed) == 37)
    DehkanPlateau_SetTightropesSecondPillarInPlace();
}

/// Walking west into the third room's second stone pillar pushes it a column
/// west.
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
    *flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
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
  while (1) {
    GameFlag_Clear(flag);
    if (flag == FLAG_FELL_THROUGH_LAST)
      return;
    flag++;
  }
}

#define DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(name, cracks, count, pictures)    \
  void name(void) {                                                            \
    const u16 *cell = &cracks[0][0];                                           \
    s32 flag = FLAG_CRACK_OPENED_FIRST;                                        \
    u32 i;                                                                     \
                                                                               \
    for (i = 0; i < count; i++) {                                              \
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
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(
    DehkanPlateau_RestoreFullMetalVestChestCracks,
    gDehkanPlateau_FullMetalVestChestCracks, FULL_METAL_VEST_CHEST_CRACKS,
    gDehkanPlateau_FullMetalVestChestCrackPictures)

extern u16 gDehkanPlateau_ThemisAxeChestCrackPictures[];
extern u16 gDehkanPlateau_ThemisAxeChestCracks[][6];

/// Restores the second room's eight cracks, as the first room's.
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(DehkanPlateau_RestoreThemisAxeChestCracks,
                                     gDehkanPlateau_ThemisAxeChestCracks,
                                     THEMIS_AXE_CHEST_CRACKS,
                                     gDehkanPlateau_ThemisAxeChestCrackPictures)

extern u16 gDehkanPlateau_TightropesCrackPictures[];
extern u16 gDehkanPlateau_TightropesCracks[][6];

/// Restores the third room's six cracks, as the first room's.
DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS(DehkanPlateau_RestoreTightropesCracks,
                                     gDehkanPlateau_TightropesCracks,
                                     TIGHTROPES_CRACKS,
                                     gDehkanPlateau_TightropesCrackPictures)

/// As the other rooms' (DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS) for the fifth
/// room's sixteen cracks.
void DehkanPlateau_RestorePoundCubeCracks(void) {
  u16 *record = &gDehkanPlateau_PoundCubeCracks[0][0];
  s32 flag = FLAG_CRACK_OPENED_FIRST;
  u32 i;

  for (i = 0; i < POUND_CUBE_CRACKS; flag++, i++) {
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

s32 gDehkanPlateau_CrackZ BSS_DATA;
s32 gDehkanPlateau_CrackX BSS_DATA;

/// Stepping onto a crack in rooms 1, 2, 3 and 5: the leader is set in the
/// middle of its cell; an open crack gives way and he sinks through it to the
/// cave below, by the exit of its step code, while a whole one cracks open.
void DehkanPlateau_StepOnCrack(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 cellX = (FIELD_CELL(leader->x.fixed) << 1) + 1;
  s32 cellZ = (FIELD_CELL(leader->z.fixed) << 1) + 1;
  s32 n, flag;
  u16 *record, *next;

  if (gMainRoutineWork.next == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
    n = work->stepTrigger - TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK;
    flag = FLAG_CRACK_OPENED_FIRST + n;
    record = &gDehkanPlateau_FullMetalVestChestCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST) {
    n = work->stepTrigger - TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK;
    flag = FLAG_CRACK_OPENED_FIRST + n;
    record = &gDehkanPlateau_ThemisAxeChestCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
    n = work->stepTrigger - TRIGGER_TIGHTROPES_FIRST_CRACK;
    flag = FLAG_CRACK_OPENED_FIRST + n;
    record = &gDehkanPlateau_TightropesCracks[n][1];
  } else if (gMainRoutineWork.next == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
    n = work->stepTrigger - TRIGGER_POUND_CUBE_FIRST_CRACK;
    flag = FLAG_CRACK_OPENED_FIRST + n;
    record = &gDehkanPlateau_PoundCubeCracks[n][1];
    if (GameFlag_Get(FLAG_FELL_THROUGH(
            DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH)))
      gDehkanPlateau_PoundCubeCracks[4][3] = 3;
  } else {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 4);
  Actor_MoveToAndWaitById(gMainRoutineWork.leaderActorId, cellX << 3,
                          cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, toFIXED(cellX * 8), leader->y.fixed,
                                  toFIXED(cellZ * 8));
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
      if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_FullMetalVestChestCrackPictures[2],
            gDehkanPlateau_FullMetalVestChestCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_ThemisAxeChestCrackPictures[2],
            gDehkanPlateau_ThemisAxeChestCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_TightropesCrackPictures[2],
            gDehkanPlateau_TightropesCrackPictures[3], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
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
      if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
        {
          s32 srcX = gDehkanPlateau_FullMetalVestChestHolePictures[cellX],
              srcY = gDehkanPlateau_FullMetalVestChestHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST) {
        {
          s32 srcX = gDehkanPlateau_ThemisAxeChestHolePictures[cellX],
              srcY = gDehkanPlateau_ThemisAxeChestHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
        {
          s32 srcX = gDehkanPlateau_TightropesHolePictures[cellX],
              srcY = gDehkanPlateau_TightropesHolePictures[cellX + 1],
              width = 1, height = 2;
          MapGrid_CopyCellPictureIndicesRect(srcX, srcY, toX, toZ, width,
                                             height);
        }
      } else if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
        if ((n == POUND_CUBE_WEST_UPPER_CRACK &&
             GameFlag_Get(
                 FLAG_CRACK_OPENED(POUND_CUBE_WEST_UPPER_CRACK + 1))) ||
            (n == POUND_CUBE_EAST_UPPER_CRACK &&
             GameFlag_Get(
                 FLAG_CRACK_OPENED(POUND_CUBE_EAST_UPPER_CRACK + 1)))) {
          s32 srcX = gDehkanPlateau_PoundCubeHolePictures[2 * JOINED_HOLE],
              srcY = gDehkanPlateau_PoundCubeHolePictures[2 * JOINED_HOLE + 1],
              width = 1, height = 2;
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
      if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST) {
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
      } else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST ||
                 scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
      } else if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE) {
        if (gDehkanPlateau_PoundCube != NULL)
          ((struct FieldSprite *)gDehkanPlateau_PoundCube->sprite)->priority =
              3;
        Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
      }
    }
    Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                           ANIM_LEADER_ARMS_RAISED);
    Event_Wait(16);
    for (cellX = 0; (u32)cellX < FALL_FRAMES; cellX++) {
      if (cellX == 5)
        Sound_Play(SE_DUST_PUFF);
      leader->scaleX += -(FIXED_ONE / 64);
      leader->scaleY += -(FIXED_ONE * 3 / 256);
      leader->y.fixed += -(toFIXED(3) / 5);
      wait(1);
    }
    gFieldWork->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
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
      if (scene == SCENE_DEHKAN_PLATEAU_FULL_METAL_VEST_CHEST)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_FullMetalVestChestCrackPictures[0],
            gDehkanPlateau_FullMetalVestChestCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_THEMIS_AXE_CHEST)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_ThemisAxeChestCrackPictures[0],
            gDehkanPlateau_ThemisAxeChestCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_TightropesCrackPictures[0],
            gDehkanPlateau_TightropesCrackPictures[1], x, y, 1, 1);
      else if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE)
        MapGrid_CopyCellPictureIndicesRect(
            gDehkanPlateau_PoundCubeCrackPictures[0],
            gDehkanPlateau_PoundCubeCrackPictures[1], x, y, 1, 1);
    }
    Event_Wait(12);
    GameFlag_Set(flag);
    GameFlag_Set(FLAG_STANDING_ON_OPENED_CRACK);
    {
      s16 scene = gMainRoutineWork.next;
      if (scene == SCENE_DEHKAN_PLATEAU_POUND_CUBE ||
          scene == SCENE_DEHKAN_PLATEAU_TIGHTROPES) {
        gDehkanPlateau_CrackX = FIELD_CELL(leader->x.fixed);
        gDehkanPlateau_CrackZ = FIELD_CELL(leader->z.fixed);
      }
    }
    Event_End();
  }
}

/// The third and fifth rooms' task: once the leader steps off the crack he
/// opened, it can be stepped on again.
void DehkanPlateau_ForgetCrackOnLeaving(void) {
  struct Actor *actor;

  if (Event_GetFlag(FLAG_STANDING_ON_OPENED_CRACK) != 0) {
    actor = Actor_GetById(gMainRoutineWork.leaderActorId);
    if (gDehkanPlateau_CrackX != FIELD_CELL(actor->x.fixed) ||
        gDehkanPlateau_CrackZ != FIELD_CELL(actor->z.fixed))
      GameFlag_Clear(FLAG_STANDING_ON_OPENED_CRACK);
  }
}

#include "shared/actor_hide.h"

/// Back from a battle or a menu, stops drawing the room's unseen blocks again
/// unless they were shown; otherwise notes them shown.
void DehkanPlateau_RestoreBlocks(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) != 0) {
    if (GameFlag_Get(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN) == 0)
      DehkanPlateau_HideBlocks();
  } else {
    GameFlag_Set(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN);
  }
}

/// Hides the first room's unseen blocks as it opens.
void DehkanPlateau_HideFullMetalVestChestBlocks(void) {
  u32 i;

  for (i = 0; i < FULL_METAL_VEST_CHEST_BLOCK_COUNT; i++)
    Actor_Hide(i + ACTOR_FULL_METAL_VEST_CHEST_FIRST_BLOCK);
  DehkanPlateau_RestoreBlocks();
}

/// Hides the second room's unseen blocks as it opens.
void DehkanPlateau_HideThemisAxeChestBlocks(void) {
  u32 i;

  for (i = 0; i < THEMIS_AXE_CHEST_BLOCK_COUNT; i++)
    Actor_Hide(i + ACTOR_THEMIS_AXE_CHEST_FIRST_BLOCK);
  DehkanPlateau_RestoreBlocks();
}

/// Hides the third room's unseen blocks as it opens.
void DehkanPlateau_HideTightropesBlocks(void) {
  u32 i;

  for (i = 0; i < TIGHTROPES_BLOCK_COUNT; i++)
    Actor_Hide(i + ACTOR_TIGHTROPES_FIRST_BLOCK);
  DehkanPlateau_RestoreBlocks();
}

/// Hides the fourth room's unseen blocks as it opens.
void DehkanPlateau_HideMintChestBlocks(void) {
  u32 i;

  for (i = 0; i < MINT_CHEST_BLOCK_COUNT; i++)
    Actor_Hide(i + ACTOR_MINT_CHEST_FIRST_BLOCK);
  DehkanPlateau_RestoreBlocks();
}

/// Hides the sixth room's unseen blocks as it opens.
void DehkanPlateau_HideCannonBlocks(void) {
  u32 i;

  for (i = 0; i < CANNON_ROOM_BLOCK_COUNT; i++)
    Actor_Hide(i + ACTOR_CANNON_ROOM_FIRST_BLOCK);
  DehkanPlateau_RestoreBlocks();
}

/// Sets the cells under a vine pillar of the third room once it is pushed or
/// Moved: off the ledge at column 12 it drops to the ground below, and at
/// columns 18, 54 and 55 it bridges the way.
void DehkanPlateau_PlaceVinePillar(struct Actor *actor) {
  s32 x;
  struct MapCell *cell;

  HeldActor_RaiseCell(actor);
  x = FIELD_CELL(actor->x.fixed);
  if (x == 12) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Map_CopyCellAttributeRect(0, 32, 1, 1, x, 11);
    wait(1);
    Actor_DropAndWait(actor);
    Map_CopyCellAttributeRect(1, 32, 1, 1, x, 11);
    Event_End();
  } else if (x == 18) {
    cell = ((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[0].cells;
    cell += Actor_GetCellIndex(actor);
    cell->attributeB &= ~MAP_CELL_JUMP;
  } else if (x == 54) {
    cell = ((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[0].cells;
    cell += Actor_GetCellIndex(actor);
    cell->attributeB &= ~MAP_CELL_JUMP;
    cell->collisionCode = TRIGGER_TIGHTROPES_PILLARS_WEST;
  } else if (x == 55) {
    MapGrid_CopyCellAttributes(53, 20, 1, 1, 54, 20);
    Map_CopyCellAttributeRect(57, 20, 1, 1, 56, 20);
  }
}

extern s16 gDehkanPlateau_TightropesVinePillars[];

/// Walking into one of the third room's vine pillars pushes it a cell on.
void DehkanPlateau_PushVinePillar(void) {
  struct Actor *actor;

  if (Leader_CanPushObjectAhead() != 0) {
    actor = Actor_GetById(
        Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId));
    Pillar_LowerHeldCells(gDehkanPlateau_TightropesVinePillars);
    Leader_PushObjectAhead();
    DehkanPlateau_PlaceVinePillar(actor);
  }
}

/// Move takes hold of one of the third room's vine pillars.
void DehkanPlateau_LiftTightropesVinePillar(void) {
  Pillar_LowerHeldCells(gDehkanPlateau_TightropesVinePillars);
}

/// Move lets go of one of the third room's vine pillars.
void DehkanPlateau_FinishMovingVinePillar(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *active;

  active = work->movedBlock;
  if (active != 0)
    DehkanPlateau_PlaceVinePillar(active);
}

/// Stepping onto room 6's lower path before its stake is Pounded holds
/// the stake still and gives its cell the attributes of cell (51, 12);
/// stepping off the path puts the cell back.
void DehkanPlateau_EnterLowerPath(void) {
  s32 set = GameFlag_Get(FLAG_CANNON_ROOM_STAKE_POUNDED);

  if (set == 0) {
    Actor_GetById(ACTOR_CANNON_ROOM_STAKE)->motionFlags = set;
    MapGrid_CopyCellAttributes(51, 12, 1, 1, 47, 12);
    GameFlag_Set(FLAG_STAKE_CELL_CHANGED);
  }
}

/// Stepping off the sixth room's lower path puts the stake's cell back.
void DehkanPlateau_LeaveLowerPath(void) {
  if (GameFlag_Get(FLAG_CANNON_ROOM_STAKE_POUNDED) == 0) {
    MapGrid_CopyCellAttributes(48, 11, 1, 1, 47, 12);
    GameFlag_Clear(FLAG_STAKE_CELL_CHANGED);
  }
}

/// Jolts the camera down eight pixels and back over eight frames, as the ground
/// gives way.
void DehkanPlateau_JoltCamera(void) {
  struct Actor *actor = FieldCamera_GetActor();
  u32 i;

  actor->y.fixed -= toFIXED(8);
  wait(1);
  for (i = 0; i <= 7; i++) {
    actor->y.fixed += toFIXED(1);
    wait(1);
  }
}

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
  GameFlag_Set(FLAG_CRACK_OPENED(7));
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
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON),
                       SPRITE_FLAG_SHADOW);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 294, 104);
  Actor_PlaceFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(424), toFIXED(104),
                    FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_LEAPT_AWAY);
  Event_End();
}

/// In the third room Cannon casts Pound on the stake to get by.
void DehkanPlateau_CannonPoundsPillar(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId,
                          ACTOR_TIGHTROPES_CANNON);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(424, CAMERA_KEEP, 104, 1);
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
  GameFlag_Set(FLAG_CRACK_OPENED(1));
  MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
  Sound_Play(SE_RATTLE);
  DehkanPlateau_JoltCamera();
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 456, 216);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 584, 216);
  Actor_PlaceFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(616), toFIXED(280),
                    FACING_SOUTH);
  GameFlag_Set(FLAG_CANNON_POUNDED_STAKE);
  Event_End();
}

/// The task of Cannon's Pound in the third room: the stake sinks, and
/// once the Psynergy ends it is down for good.
void DehkanPlateau_SinkPoundedStake(void) {
  struct FieldEffectWork *work = Ram_HeapSlots->fieldEffectWork;
  struct Actor *actor = Actor_GetById(ACTOR_TIGHTROPES_STAKE);

  if (work->frame != -1 && work->frame <= 7)
    actor->scaleY += -(FIXED_ONE * 31 / 256);
  if (work->stage != 0) {
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetAnimationById(ACTOR_TIGHTROPES_STAKE, 4);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags &= ~ACTOR_SOLID;
    actor->scaleY = FIXED_ONE;
    GameFlag_Set(FLAG_TIGHTROPES_STAKE_POUNDED);
  }
}

/// Cannon casts Pound on the third room's stake.
void DehkanPlateau_CannonCastsPound(void) {
  FieldPsynergy_LoadResources(ABILITY_POUND, 1);
  FieldTask_AddFrame(DehkanPlateau_SinkPoundedStake, TASK_PASS_FRAME);
  FieldPsynergy_SetupActorPair(ACTOR_TIGHTROPES_CANNON, ACTOR_TIGHTROPES_STAKE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_SinkPoundedStake);
  FieldPsynergy_End();
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_DEHKAN_PLATEAU_RESTORE_CRACKS
#undef EFFECT_SET_MODE

extern u32 gDehkanPlateau_DustScript[];

/// Cracks a crumbling pillar as Cannon lands on it: it flashes white and takes
/// its cracked look, and eight puffs of dust rise about the one standing on it.
void DehkanPlateau_CrackPillar(s32 id, s32 center, s32 flag) {
  struct Actor *leader;
  s32 frame;
  u32 i;
  FIXED y;

  Sound_Play(SE_CHEST_CLOSE);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
  wait(2);
  Actor_SetAnimationById(id, 0);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_WHITE);
  wait(4);
  Actor_SetSpriteColor(Actor_GetById(id), SPRITE_COLOR_NORMAL);
  leader = Actor_GetById(center);
  frame = 0;
  for (i = 0; i <= 7; i++) {
    struct Actor *actor;
    struct FieldEffect *effect;

    y = leader->y.fixed - toFIXED(((Random16() * 7) << 1) >> 16);
    actor = ((struct Actor *)Import_ActorCreate(
        SPRITE_DUST_CLOUD, toFIXED(i * 2) + leader->x.fixed - toFIXED(8),
        y - toFIXED(2), leader->z.fixed));
    if (actor != NULL) {
      effect = (struct FieldEffect *)actor;
      frame = Sprite_ShareVramCacheEntry(effect->sprite, frame);
      actor->motionFlags = 0;
      actor->collisionFlags = 0;
      Actor_SetSpriteFlags(actor, 0);
      Actor_SelectAnimation(actor, 2);
      Actor_StartScript(actor, gDehkanPlateau_DustScript);
      actor->scaleX = FIXED_ONE * 7 / 10;
      actor->scaleY = FIXED_ONE * 7 / 10;
      effect->sprite->priority = 1;
      effect->velocity.x = 0;
      effect->velocity.z = 0;
      effect->velocity.y = -((((u32)Random16() << 14) >> 16) + toFIXED(3) / 8);
      effect->spin = (((u32)Random16() << 9) >> 16) - 256;
      effect->scaleRateX = -(FIXED_ONE / 32);
      effect->scaleRateY = -(FIXED_ONE * 3 / 128);
      effect->update = FieldEffect_AdvanceMotion;
    }
  }
  GameFlag_Set(flag);
  Event_Wait(20);
}

/// In the third room Cannon leaps across the crumbling pillars, cracking two of
/// them as he lands.
void DehkanPlateau_CannonCracksPillars(void) {
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
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON),
                       SPRITE_FLAG_SHADOW);
  DehkanPlateau_CrackPillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR,
                            ACTOR_TIGHTROPES_CANNON, FLAG_PILLAR_CRACKED(0));
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 696, 280);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON),
                       SPRITE_FLAG_SHADOW);
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
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON),
                       SPRITE_FLAG_SHADOW);
  DehkanPlateau_CrackPillar(ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 3,
                            ACTOR_TIGHTROPES_CANNON, FLAG_PILLAR_CRACKED(3));
  Actor_FaceDirectionAndRefreshById(ACTOR_TIGHTROPES_CANNON, FACING_NORTH);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON), 0);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 136);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_TIGHTROPES_CANNON),
                       SPRITE_FLAG_SHADOW);
  Event_Wait(10);
  Actor_WalkToAndWait(ACTOR_TIGHTROPES_CANNON, 744, 72);
  MapGrid_CopyCellPictureIndicesRect(66, 51, 110, 4, 1, 1);
  Sound_Play(SE_SPLASH);
  DehkanPlateau_JoltCamera();
  Event_Wait(10);
  Actor_Jump(ACTOR_TIGHTROPES_CANNON, 6, 16);
  Actor_SetPosition(ACTOR_TIGHTROPES_CANNON, 0, 0);
  Event_Wait(20);
  GameFlag_Set(FLAG_CANNON_CRACKED_PILLARS);
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
  GameFlag_Set(FLAG_CRACK_OPENED(5));
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
  for (i = 0; i < FALL_FRAMES; i++) {
    if (i == 5)
      Sound_Play(SE_DUST_PUFF);
    leader->scaleX -= FIXED_ONE / 64;
    leader->scaleY -= FIXED_ONE * 3 / 256;
    leader->y.fixed -= toFIXED(3) / 5;
    wait(1);
  }
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(TRIGGER_POUND_CUBE_CANNON_CRACK);
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

/// Cannon casts Pound on the leader in the fifth room.
void DehkanPlateau_CannonCastsPoundOnLeader(void) {
  FieldPsynergy_LoadResources(ABILITY_POUND, 1);
  FieldTask_AddFrame(DehkanPlateau_PoundLeaderThrough, TASK_PASS_FRAME);
  FieldPsynergy_SetupActorPair(ACTOR_POUND_CUBE_CANNON,
                               gMainRoutineWork.leaderActorId);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  Task_Remove(DehkanPlateau_PoundLeaderThrough);
  FieldPsynergy_End();
}

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

/// Meeting Cannon in the sixth room while the stake stands: he leaps
/// over the last crumbling pillar, cracking it, and waits on the far side.
void DehkanPlateau_CannonCracksLastPillar(void) {
  struct Actor *actor;

  actor = Actor_GetById(ACTOR_CANNON);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_CANNON);
  FieldCamera_GetActor()->motionFlags = 0;
  FieldCamera_MoveTo(760, 32, 204, 1);
  FieldCamera_WaitForArrival();
  Actor_FaceActorById(ACTOR_CANNON, gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmote(ACTOR_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_SetSpeed(ACTOR_CANNON, 2);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 760, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
  Event_Wait(20);
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 792, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
  DehkanPlateau_CrackPillar(ACTOR_CANNON_ROOM_CRUMBLING_PILLAR, ACTOR_CANNON,
                            FLAG_PILLAR_CRACKED(0));
  Sound_Play(SE_HIGH_JUMP);
  actor->velocityY = toFIXED(8);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), 0);
  Actor_WalkToAndWaitById(ACTOR_CANNON, 824, 200);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_CANNON), SPRITE_FLAG_SHADOW);
  Event_Wait(20);
  Actor_WatchAndSetScript(ACTOR_CANNON, gMainRoutineWork.leaderActorId);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  Event_Wait(20);
  Event_End();
}

/// Meeting Cannon in the sixth room once the stake is down: he finds
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
  if (GameFlag_Get(FLAG_CANNON_ROOM_STAKE_POUNDED) != 0)
    DehkanPlateau_CannonCornered();
  else
    DehkanPlateau_CannonCracksLastPillar();
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

/// Sets up the first room: its pillars, the stone pillar already in the gap,
/// its cracks and unseen blocks, and the chest's task.
void DehkanPlateau_InitializeFullMetalVestChest(void) {
  CrumblingPillar_Place(gDehkanPlateau_FullMetalVestChestCrumblingPillars);
  Actor_SetSpritePriority(ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR,
                          3);
  Pillar_Place(gDehkanPlateau_FullMetalVestChestStakes);
  Actor_DisableById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR);
  if (GameFlag_Get(FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP) != 0) {
    MapGrid_CopyCellAttributes(54, 32, 2, 1, 56, 64);
    Actor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_FULL_METAL_VEST_CHEST_PILLAR), toFIXED(904),
        toFIXED(64), toFIXED(520));
    wait(10);
  }
  DehkanPlateau_RestoreFullMetalVestChestCracks();
  DehkanPlateau_HideFullMetalVestChestBlocks();
  {
    s32 priority = TASK_PASS_FRAME;

    Task_Add(DehkanPlateau_KeepPriorityWithLeader, priority);
  }
}

/// Sets up the second room: its crumbling pillars, stake and stone pillar,
/// its cracks, and Cannon as far as he has fled.
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
  CrumblingPillar_PlaceWithStandIns(
      gDehkanPlateau_ThemisAxeChestCrumblingPillars);
  DehkanPlateau_SetThemisAxeChestStandingPillarCells();
  Pillar_Place(gDehkanPlateau_ThemisAxeChestStakes);
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
    GameFlag_Set(FLAG_CRACK_OPENED(7));
  }
  if (GameFlag_Get(FLAG_CANNON_FLED_EAST) != 0)
    Actor_SetPosition(ACTOR_THEMIS_AXE_CHEST_CANNON, 0, 0);
  DehkanPlateau_HideThemisAxeChestBlocks();
}

void DehkanPlateau_ForgetCrackOnLeaving(void);
void Tightrope_Initialize(void);

/// Sets up the third room: its tightropes and the rope to Lash, its stone, vine
/// and stakes and crumbling pillars, its cracks, and Cannon as far as he has
/// gone.
void DehkanPlateau_InitializeTightropes(void) {
  struct Actor *actor;
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
  Actor_GetById(ACTOR_TIGHTROPES_FIRST_VINE_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_SECOND_VINE_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_THIRD_VINE_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Actor_GetById(ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR)->priorityFlags &=
      ~ACTOR_PRIORITY_AUTOMATIC;
  Pillar_Raise(gDehkanPlateau_TightropesVinePillars);
  {
    struct Actor *pillar = Actor_GetById(ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR);

    value = pillar->x.fixed;
    zero = 0;
    if (FIELD_CELL(value) == 54) {
      struct MapCell *cell =
          ((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[0].cells;

      cell += Actor_GetCellIndex(pillar);
      cell->attributeB &= ~MAP_CELL_JUMP;
      cell->collisionCode = TRIGGER_TIGHTROPES_PILLARS_WEST;
    }
  }
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, 1);
  Actor_SetSpritePriority(ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, 1);
  DehkanPlateau_RestoreTightropesCracks();
  if (GameFlag_Get(FLAG_CANNON_LEAPT_AWAY) != 0) {
    Actor_SetPositionAndFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(424),
                               toFIXED(104), FACING_SOUTH);
  }
  Actor_SetAnimationById(ACTOR_TIGHTROPES_STAKE, 3);
  if (GameFlag_Get(FLAG_CANNON_POUNDED_STAKE) != 0) {
    actor = Actor_GetById(ACTOR_TIGHTROPES_STAKE);
    MapGrid_CopyCellAttributes(27, 8, 1, 1, 27, 9);
    Actor_SetAnimationById(ACTOR_TIGHTROPES_STAKE, 4);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    actor->collisionFlags &= ~ACTOR_SOLID;
    GameFlag_Set(FLAG_TIGHTROPES_STAKE_POUNDED);
    GameFlag_Set(FLAG_CRACK_OPENED(1));
    MapGrid_CopyCellPictureIndicesRect(69, 51, 92, 11, 1, 1);
    {
      Actor_SetPositionAndFacing(ACTOR_TIGHTROPES_CANNON, toFIXED(616),
                                 toFIXED(280), FACING_SOUTH);
    }
  }
  if (GameFlag_Get(FLAG_CANNON_CRACKED_PILLARS) != 0) {
    GameFlag_Set(FLAG_PILLAR_CRACKED(0));
    GameFlag_Set(FLAG_PILLAR_CRACKED(3));
    Actor_SetPosition(ACTOR_TIGHTROPES_CANNON, 0, 0);
  }
  CrumblingPillar_Place(gDehkanPlateau_TightropesCrumblingPillars);
  DehkanPlateau_SetTightropesStandingPillarCells();
  DehkanPlateau_HideTightropesBlocks();
  {
    struct Actor *block = Actor_GetById(ACTOR_TIGHTROPES_FIRST_BLOCK + 1);

    block->motionFlags = zero;
    block->y.fixed = zero;
  }
  gDehkanPlateau_CrackX = zero;
  gDehkanPlateau_CrackZ = zero;
  Task_Add(DehkanPlateau_ForgetCrackOnLeaving, TASK_PASS_FRAME);
}

extern u16 gDehkanPlateau_FallLandingCells[];

#define DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(name, sourceX, sourceZ,            \
                                            columnSourceX, columnSourceZ,      \
                                            belowOffsetZ, belowSourceZ)        \
  void name(void) {                                                            \
    s32 flag = FLAG_FELL_THROUGH_FIRST;                                        \
    s32 offset = 0;                                                            \
                                                                               \
    while (1) {                                                                \
      if (GameFlag_Get(flag)) {                                                \
        u16 *entry = gDehkanPlateau_FallLandingCells;                          \
        /* FAKEMATCH(temporary): the fall's number held in a local; as         \
         * flag - FLAG_FELL_THROUGH_FIRST at its test. */                      \
        s32 fall = flag - FLAG_FELL_THROUGH_FIRST;                             \
        s32 y;                                                                 \
        s32 x;                                                                 \
                                                                               \
        entry = (u16 *)((u8 *)entry + offset);                                 \
        x = *entry++;                                                          \
        y = *entry++;                                                          \
        {                                                                      \
          s32 width = 1;                                                       \
                                                                               \
          MapGrid_CopyCellsTo(sourceX, sourceZ, x, y, width, 1);               \
          x = entry[0];                                                        \
          y = entry[1];                                                        \
          MapGrid_CopyCellsTo(columnSourceX, columnSourceZ, x, y, width, 5);   \
          if (fall == 1)                                                       \
            MapGrid_CopyCellsTo(67, 4, 69, 3, width, 1);                       \
          /* FAKEMATCH(barrier): an empty asm on entry. */                     \
          asm("" : : "r"(entry));                                              \
          x -= 64;                                                             \
          y += belowOffsetZ;                                                   \
          MapGrid_CopyCellsTo(sourceX, belowSourceZ, x, y, width, 4);          \
        }                                                                      \
        return;                                                                \
      }                                                                        \
      if (flag == FLAG_FELL_THROUGH_LAST)                                      \
        return;                                                                \
      offset += 8;                                                             \
      flag++;                                                                  \
    }                                                                          \
  }

/// Draws, in the fourth room, the cells where the party landed under the crack
/// it fell through.
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawMintChestFallLanding, 3,
                                    8, 67, 3, 66, 69)

/// Draws, in the fifth room, the cells where the party landed under the crack
/// it fell through.
DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG(DehkanPlateau_DrawPoundCubeFallLanding, 13,
                                    5, 77, 0, 65, 65)

/// Sets up the fourth room, below the cracks: the floor Cannon opened, and the
/// landing of a fall.
void DehkanPlateau_InitializeMintChest(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  DehkanPlateau_HideMintChestBlocks();
  if (GameFlag_Get(FLAG_CANNON_FLED_OVER_CRACKS) != 0) {
    MapGrid_CopyCellPictureIndicesRect(3, 8, 30, 33, 1, 1);
    MapGrid_CopyCellPictureIndicesRect(67, 3, 94, 28, 1, 5);
    MapGrid_CopyCellPictureIndicesRect(3, 69, 30, 93, 1, 4);
  }
  {
    /* FAKEMATCH(temporary): the entrance read once as a halfword into a local
     * and its sign extended by hand; plainly, gMainRoutineWork.entrance in the
     * test and GameFlag_Set(FLAG_FELL_THROUGH(gMainRoutineWork.entrance)). */
    s32 entrance = *(u16 *)&gMainRoutineWork.entrance;

    if ((u16)(entrance - FIRST_LANDING_ENTRANCE) <=
        LAST_LANDING_ENTRANCE - FIRST_LANDING_ENTRANCE) {
      DehkanPlateau_ForgetFall();
      entrance <<= 16;
      entrance >>= 16;
      entrance += FLAG_FELL_THROUGH_FIRST - FIRST_LANDING_ENTRANCE;
      GameFlag_Set(entrance);
    }
  }
  DehkanPlateau_DrawMintChestFallLanding();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance >= FIRST_LANDING_ENTRANCE) {
      if (gMainRoutineWork.entrance <= LAST_LANDING_ENTRANCE)
        DehkanPlateau_LandFromFall();
    }
  }
}

/// The leader drops in from the crack above, landing in a ring of dust.
void DehkanPlateau_LandFromFall(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;

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
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  for (i = 0; i <= TRIG_TURN / FACING_STEP; i++) {
    s32 angle;
    FIXED velocity[3];

    angle = i * FACING_STEP;
    velocity[0] = Trig_Cos(angle);
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
                      &options);
  }
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
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Sets up the fifth room: the Pound Cube where Cannon dropped it, its cracks,
/// and the landing of a fall, beside Cannon the first time.
void DehkanPlateau_InitializePoundCube(void) {
  struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
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
  Pillar_Place(gDehkanPlateau_PoundCubeStakes);
  DehkanPlateau_RestorePoundCubeCracks();
  entrance = gMainRoutineWork.entrance;
  if (entrance >= FIRST_LANDING_ENTRANCE) {
    DehkanPlateau_ForgetFall();
    GameFlag_Set(FLAG_FELL_THROUGH(entrance));
  }
  if (Event_GetFlag(FLAG_CANNON_DROPPED_POUND_CUBE) != 0)
    Actor_SetPosition(ACTOR_POUND_CUBE_CANNON, 0, 0);
  DehkanPlateau_DrawPoundCubeFallLanding();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance >= POUND_CUBE_LANDINGS_FROM) {
      if (gMainRoutineWork.entrance ==
              DEHKAN_PLATEAU_POUND_CUBE_FROM_DEHKAN_PLATEAU_TIGHTROPES_NORTH &&
          GameFlag_Get(FLAG_CANNON_DROPPED_POUND_CUBE) == 0)
        DehkanPlateau_CannonDropsPoundCube();
      else
        DehkanPlateau_LandFromFall();
    }
  }
  gDehkanPlateau_CrackX = 0;
  gDehkanPlateau_CrackZ = 0;
  Task_Add(DehkanPlateau_ForgetCrackOnLeaving, TASK_PASS_FRAME);
}

/// Creates the Pound Cube Cannon drops, drawn with the item's own icon.
void DehkanPlateau_CreatePoundCube(void) {
  struct Actor **slot = &gDehkanPlateau_PoundCube;
  u8 clear = 0;
  struct Actor *actor =
      ((struct Actor *)Import_ActorCreate(SPRITE_UNSEEN_MARKER, 0, 0, 0));

  *slot = actor;
  if (actor != 0) {
    struct FieldSprite *sprite = actor->sprite;
    GlyphTransfer *glyph;

    sprite->fullColor = 0;
    sprite->palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    sprite->flags = clear;
    sprite->countAndShadow[0] = clear;
    glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_POUND_CUBE);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Sets up the sixth room: its stake and crumbling pillar, Cannon met
/// already if he is in the party, and the landing of a fall.
void DehkanPlateau_InitializeCannon(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  if (GameFlag_Get(FLAG_DJINNI(DJINNI_CANNON)) != 0)
    GameFlag_Set(FLAG_CANNON_MET);
  Pillar_Place(gDehkanPlateau_CannonStakes);
  CrumblingPillar_Place(gDehkanPlateau_CannonCrumblingPillar);
  if (GameFlag_Get(FLAG_CANNON_ROOM_STAKE_POUNDED) == 0) {
    DehkanPlateau_EnterLowerPath();
    wait(10);
  }
  DehkanPlateau_HideCannonBlocks();
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance >= CANNON_ROOM_LANDINGS_FROM) {
      if (gMainRoutineWork.entrance <= LAST_LANDING_ENTRANCE)
        DehkanPlateau_LandFromFall();
    }
  }
}

s32 Scene_PrepareMap(void) { return 0; }

#undef DEFINE_DEHKAN_PLATEAU_PLACE_BY_FLAG

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

#include "shared/field_effect_duration_scripts.c"

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

/// Each room's crumbling pillars, with the first of the two flags that remember
/// each one cracked and crumbled.
s16 gDehkanPlateau_FullMetalVestChestCrumblingPillars[] = {
    ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
    ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
    ACTOR_FULL_METAL_VEST_CHEST_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_ThemisAxeChestCrumblingPillars[] = {
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 3, FLAG_PILLAR_CRACKED(3),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 4, FLAG_PILLAR_CRACKED(4),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 5, FLAG_PILLAR_CRACKED(5),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 6, FLAG_PILLAR_CRACKED(6),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 7, FLAG_PILLAR_CRACKED(7),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 8, FLAG_PILLAR_CRACKED(8),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 9, FLAG_PILLAR_CRACKED(9),
    ACTOR_THEMIS_AXE_CHEST_FIRST_CRUMBLING_PILLAR + 10, FLAG_PILLAR_CRACKED(10),
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_TightropesCrumblingPillars[] = {
    ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0),
    ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 1, FLAG_PILLAR_CRACKED(1),
    ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 2, FLAG_PILLAR_CRACKED(2),
    ACTOR_TIGHTROPES_FIRST_CRUMBLING_PILLAR + 3, FLAG_PILLAR_CRACKED(3),
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_CannonCrumblingPillar[] = {
    ACTOR_CANNON_ROOM_CRUMBLING_PILLAR, FLAG_PILLAR_CRACKED(0), ACTOR_LIST_END,
};

/// Each room's stakes, with the flag that remembers each one Pounded.
s16 gDehkanPlateau_FullMetalVestChestStakes[] = {
    ACTOR_FULL_METAL_VEST_CHEST_STAKE, 1,
    FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED, ACTOR_LIST_END,
};

s16 gDehkanPlateau_ThemisAxeChestStakes[] = {
    ACTOR_THEMIS_AXE_CHEST_STAKE, 1, FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED,
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_TightropesStakes[] = {
    ACTOR_TIGHTROPES_STAKE, 1, FLAG_TIGHTROPES_STAKE_POUNDED,
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_PoundCubeStakes[] = {
    ACTOR_POUND_CUBE_STAKE, 1, FLAG_POUND_CUBE_STAKE_POUNDED,
    ACTOR_LIST_END,
};

s16 gDehkanPlateau_CannonStakes[] = {
    ACTOR_CANNON_ROOM_STAKE, 1, FLAG_CANNON_ROOM_STAKE_POUNDED,
    ACTOR_LIST_END,
};

/// The stone pillars Move takes hold of.
s16 gDehkanPlateau_ThemisAxeChestMovablePillars[] = {
    ACTOR_THEMIS_AXE_CHEST_PILLAR, ACTOR_LIST_END,
};

s16 gDehkanPlateau_TightropesMovablePillars[] = {
    ACTOR_TIGHTROPES_PILLAR, ACTOR_LIST_END,
};

/// The third room's vine pillars.
s16 gDehkanPlateau_TightropesVinePillars[] = {
    ACTOR_TIGHTROPES_FIRST_VINE_PILLAR,  ACTOR_TIGHTROPES_SECOND_VINE_PILLAR,
    ACTOR_TIGHTROPES_THIRD_VINE_PILLAR,  ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR,
    ACTOR_LIST_END,
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

/// Each room's actors (enum DehkanPlateauFullMetalVestChestActorId and the
/// rest); the rows at scene points stand where the map's points put them.
struct ActorPlacement gDehkanPlateau_FullMetalVestChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_ThemisAxeChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 10, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 840, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_TightropesPlacements[] = {
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 9, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_VINE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 10, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 11, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 12, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 13, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT | ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 14, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 15, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    {SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 16, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 920, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_MintChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_PoundCubePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 536, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gDehkanPlateau_CannonPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 760, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_COLUMN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 872, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_CANNON), MOVEMENT_TYPE_STAND, 728, 0, 200, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

/// The triggers of a room with none of its own.
struct Trigger gDehkanPlateau_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// Each room's exits, cracks, pillars, unseen blocks and Cannon's flight.
struct Trigger gDehkanPlateau_FullMetalVestChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(DehkanPlateau_LeaveRoom)},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING, 9, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnFullMetalVestChestPillars)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_FULL_METAL_VEST_CHEST_STAKE, FLAG_FULL_METAL_VEST_CHEST_STAKE_POUNDED, EVENT(DehkanPlateau_PoundFullMetalVestChestStake)},
    {TRIGGER_PUSHED, 0, FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP, EVENT(DehkanPlateau_DropFullMetalVestChestPillarIntoGap)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FULL_METAL_VEST_CHEST_PILLAR, FLAG_FULL_METAL_VEST_CHEST_PILLAR_IN_GAP, EVENT(DehkanPlateau_DropFullMetalVestChestPillarIntoGap)},
    {TRIGGER_STEP, 6, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, EVENT(DehkanPlateau_ShowBlocks)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), EVENT(DehkanPlateau_HideBlocks)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOffCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 8, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 9, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 10, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 11, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 12, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_FULL_METAL_VEST_CHEST_FIRST_CRACK + 13, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_THEMIS_AXE_CHEST_STAKE, FLAG_THEMIS_AXE_CHEST_STAKE_POUNDED, EVENT(DehkanPlateau_PoundThemisAxeChestStake)},
    {TRIGGER_PUSH, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_LiftThemisAxeChestPillar)},
    {TRIGGER_PUSHED, 0, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceThemisAxeChestPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_LiftThemisAxeChestPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THEMIS_AXE_CHEST_PILLAR, FLAG_THEMIS_AXE_CHEST_PILLAR_PLACED, EVENT(DehkanPlateau_PlaceThemisAxeChestPillar)},
    {TRIGGER_STEP, 6, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, EVENT(DehkanPlateau_ShowBlocks)},
    {TRIGGER_STEP, 7, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), EVENT(DehkanPlateau_HideBlocks)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOffCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_THEMIS_AXE_CHEST_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
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
    {TRIGGER_STEP, 31, FLAG_CANNON_POUNDED_STAKE, EVENT(DehkanPlateau_CannonPoundsPillar)},
    {TRIGGER_STEP, 32, FLAG_CANNON_CRACKED_PILLARS, EVENT(DehkanPlateau_CannonCracksPillars)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_TIGHTROPES_ROPE_LOW_POST, FLAG_TIGHTROPES_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_TIGHTROPES_STAKE, FLAG_TIGHTROPES_STAKE_POUNDED, EVENT(DehkanPlateau_PoundTightropesStake)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceTightropesPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_PlaceTightropesPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_PILLAR, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_MoveTightropesSecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 9, FLAG_TIGHTROPES_SECOND_PILLAR_PLACED, EVENT(DehkanPlateau_PushTightropesSecondPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT(DehkanPlateau_PushVinePillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(DehkanPlateau_PushVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_THIRD_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesVinePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_LiftTightropesVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FIRST_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_SECOND_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_THIRD_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingVinePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TIGHTROPES_FOURTH_VINE_PILLAR, CONDITION_ALWAYS, EVENT(DehkanPlateau_FinishMovingVinePillar)},
    {STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), TRIGGER_TIGHTROPES_PILLARS_WEST, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 26, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT(DehkanPlateau_StepOnTightropesPillars)},
    {TRIGGER_STEP, 7, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, EVENT(DehkanPlateau_ShowBlocks)},
    {TRIGGER_STEP, 8, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), EVENT(DehkanPlateau_HideBlocks)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_TIGHTROPES_FIRST_CRACK + 5, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
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
    {TRIGGER_STEP, 8, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, EVENT(DehkanPlateau_ShowBlocks)},
    {TRIGGER_STEP, 9, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), EVENT(DehkanPlateau_HideBlocks)},
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
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_CANNON_CRACK, FLAG_CANNON_DROPPED_POUND_CUBE, EVENT(DehkanPlateau_CannonPoundsLeaderThrough)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_POUND_CUBE_STAKE, FLAG_POUND_CUBE_STAKE_POUNDED, EVENT(DehkanPlateau_PoundPoundCubeStake)},
    {TRIGGER_EXAMINE, 12, FLAG_POUND_CUBE_TAKEN, EVENT(DehkanPlateau_TakePoundCube)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 1, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 2, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 3, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 4, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_CANNON_CRACK, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 6, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 7, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 8, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 9, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 10, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 11, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 12, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 13, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 14, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
    {TRIGGER_STEP, TRIGGER_POUND_CUBE_FIRST_CRACK + 15, FLAG_STANDING_ON_OPENED_CRACK, EVENT(DehkanPlateau_StepOnCrack)},
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
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_CANNON_ROOM_STAKE, FLAG_CANNON_ROOM_STAKE_POUNDED, EVENT(DehkanPlateau_PoundCannonStake)},
    {TRIGGER_STEP, 9, FLAG_STAKE_CELL_CHANGED, EVENT(DehkanPlateau_EnterLowerPath)},
    {TRIGGER_STEP, 10, IF_FLAG_SET(FLAG_STAKE_CELL_CHANGED), EVENT(DehkanPlateau_LeaveLowerPath)},
    {TRIGGER_STEP, 5, FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN, EVENT(DehkanPlateau_ShowBlocks)},
    {TRIGGER_STEP, 6, IF_FLAG_SET(FLAG_DEHKAN_PLATEAU_BLOCKS_SHOWN), EVENT(DehkanPlateau_HideBlocks)},
    {TRIGGER_STEP, 11, FLAG_CANNON_MET, EVENT(DehkanPlateau_MeetCannon)},
    {TRIGGER_TALK, ACTOR_CANNON, CONDITION_ALWAYS, EVENT(DehkanPlateau_FightCannon)},
    {TRIGGER_TABLE_END},
};
