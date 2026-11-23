/// @file
/// The Ankohl Ruins, where Felix, once Gaia Rock has given him Sand, climbs
/// through the old ruins to the Left Prong of the Trident of Ankohl lying on
/// their top.
/// Past the forest at the doorway, the halls of the stone faces come first:
/// Whirlwind clears their weeds, some of which hide the way on, and Move
/// pushes each of the seven stone faces in the north wall off its ledge to
/// open the doorway behind it. In the Thanatos Mace's rooms Move pushes the
/// left half of a stone face against its right half and sand pours down into
/// a slope. Move also pushes pillars aside, one by the Nut's chest, one in the
/// great face's rooms, three in the face halves' rooms and one in the Crystal
/// Powder's rooms. In each of the two sand rooms a switch makes the great
/// stone face in the north wall pour sand until the room is full, and a
/// button on the far ledge, reached on the sand, opens the way on. In the
/// face halves' rooms flowing sand carries Felix down a slide. In the great
/// face's rooms Move pushes the right half of the great face into place, its
/// eyes glow, and stepping into its corridor sets it sliding down at
/// Felix, who hides from it under the Sand. In the Crystal Powder's rooms
/// Reveal shows a gap that Move pushes a half face into; it lands in the face
/// halves' rooms, where Move pushes it against the other half and sand pours
/// into a slope. From the Crystal Powder's rooms a way leads out onto the top
/// of the ruins.
#include "../field/actor_set_sprite_priority.c"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/actor_view.h"
#include "field/event.h"
#include "field/field_sequence_event.h"
#include "scene/dehkan_plateau.h"
#include "macros.h"

#include "../field/actor_prepare.c"
#include "types.h"
#include "system/io_write.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "scene_ids.h"
#include "field/field_sprite.h"
#include "field/event_inline.h"

/// The seven stone faces Move pushes off their ledge in the halls of the stone
/// faces, one flag a face; the macro takes the face's actor.
#define FLAG_STONE_FACE_A_MOVED 0x9d2
#define FLAG_STONE_FACE_MOVED(face)                                            \
  (FLAG_STONE_FACE_A_MOVED - ACTOR_STONE_FACE_A + (face))
/// The ruins' weeds Whirlwind scatters, one flag a clump: the fourteen of the
/// halls of the stone faces from FLAG_STONE_FACES_WEEDS_A_SCATTERED, the
/// Thanatos Mace's rooms' three, then the face halves' rooms' seven; the macros
/// take the clump's actor.
#define FLAG_STONE_FACES_WEEDS_A_SCATTERED 0x990
#define FLAG_THANATOS_WEEDS_A_SCATTERED 0x99e
#define FLAG_FACE_HALVES_WEEDS_A_SCATTERED 0x9a1
#define FLAG_STONE_FACES_WEEDS_SCATTERED(weeds)                                \
  (FLAG_STONE_FACES_WEEDS_A_SCATTERED - ACTOR_STONE_FACES_WEEDS_A + (weeds))
#define FLAG_THANATOS_WEEDS_SCATTERED(weeds)                                   \
  (FLAG_THANATOS_WEEDS_A_SCATTERED - ACTOR_THANATOS_WEEDS_A + (weeds))
#define FLAG_FACE_HALVES_WEEDS_SCATTERED(weeds)                                \
  (FLAG_FACE_HALVES_WEEDS_A_SCATTERED - ACTOR_FACE_HALVES_WEEDS_A + (weeds))
/// The pillars Move has pushed to the end of their tracks: the one by the
/// Nut's chest, the great face's rooms' one, the face halves' rooms' three and
/// the Crystal Powder's rooms' one.
#define FLAG_NUT_CHEST_PILLAR_MOVED 0x9c6
#define FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED 0x9c7
#define FLAG_FACE_HALVES_PILLAR_MOVED 0x9c8
#define FLAG_FACE_HALVES_SECOND_PILLAR_MOVED 0x9c9
#define FLAG_FACE_HALVES_THIRD_PILLAR_MOVED 0x9ca
#define FLAG_CRYSTAL_POWDER_PILLAR_MOVED 0x9cb
/// The halves of stone faces pushed into place: the face halves' rooms' two,
/// the half pushed into the Crystal Powder's rooms' gap, the Thanatos Mace's
/// rooms' half, and the great face's right half.
#define FLAG_LEFT_FACE_HALF_PLACED 0x9d9
#define FLAG_RIGHT_FACE_HALF_PLACED 0x9da
#define FLAG_FACE_HALF_PUSHED_INTO_GAP 0x9db
#define FLAG_THANATOS_FACE_HALF_PLACED 0x9dc
#define FLAG_GREAT_FACE_HALF_PLACED 0x9dd
/// The doors the buttons on the sand rooms' far ledges open, the first room's
/// and the second's.
#define FLAG_FIRST_SAND_DOOR_OPENED 0x9e2
#define FLAG_SECOND_SAND_DOOR_OPENED 0x9e3
/// The sand rooms' switches pressed, for this visit: the sand is rising or
/// has risen in each.
#define FLAG_FIRST_SAND_SWITCH_PRESSED FLAG_SCENE_TEMP_02
#define FLAG_SECOND_SAND_SWITCH_PRESSED FLAG_SCENE_TEMP_03
/// Set while Reveal is up: the sand rooms stop trimming the sand's edge, and
/// from patch 1 the Crystal Powder's rooms' gap lies open to a pushed half
/// face.
#define FLAG_ANKOHL_RUINS_REVEAL_SHOWN FLAG_SCENE_TEMP_05

/// The cell behaviours the rooms' trigger tables answer: the foot of the great
/// face's corridor and the cell right in front of the face at its top; the
/// sand rooms' switches and their far ledges' buttons; and the pit in the
/// Crystal Powder's rooms Felix jumps down into the face halves' rooms.
enum {
  CELL_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT = 40,
  CELL_BEHAVIOR_BEFORE_GREAT_FACE = 41,
  CELL_BEHAVIOR_FIRST_SAND_SWITCH = 40,
  CELL_BEHAVIOR_FIRST_SAND_DOOR_BUTTON = 41,
  CELL_BEHAVIOR_SECOND_SAND_SWITCH = 42,
  CELL_BEHAVIOR_SECOND_SAND_DOOR_BUTTON = 43,
  CELL_BEHAVIOR_PIT_TO_FACE_HALVES = 13
};

/// The triggers the rooms' code raises: the flowing sand carrying Felix off a
/// ledge, and each sand room's next rise.
enum {
  TRIGGER_CARRIED_OFF_LEDGE = 66,
  TRIGGER_RAISE_FIRST_SAND = 68,
  TRIGGER_RAISE_SECOND_SAND = 69
};

/// The attribute bit that marks the cells whose sand flows south here
/// (AnkohlRuins_FlowWithSand); the field reads it elsewhere as
/// MAP_CELL_SECOND_ZONE.
#define MAP_CELL_FLOWING_SAND MAP_CELL_SECOND_ZONE

/// The cell column each pillar Move pushes stands in at its track's end, where
/// its flag is set and the set-up places it again.
enum {
  NUT_CHEST_PILLAR_END_COLUMN = 4,
  GREAT_FACE_ROOMS_PILLAR_END_COLUMN = 32,
  FACE_HALVES_PILLAR_END_COLUMN = 2,
  FACE_HALVES_SECOND_PILLAR_END_COLUMN = 39,
  FACE_HALVES_THIRD_PILLAR_END_COLUMN = 38,
  CRYSTAL_POWDER_PILLAR_END_COLUMN = 17
};

/// The row below the stone faces' ledge: a face pushed into it falls.
#define STONE_FACE_DROP_ROW 31

/// The cell of the Crystal Powder's rooms' gap the half face drops into.
#define GAP_COLUMN 26
#define GAP_ROW 35

/// The Thanatos Mace's rooms' entrances up the stairs from their south hall,
/// east and west, into the hall of the face half.
enum {
  THANATOS_FACE_HALL_FROM_EAST_STAIRS = 15,
  THANATOS_FACE_HALL_FROM_WEST_STAIRS = 17
};

/// The face halves' rooms' first entrance into the hall of the halves, where
/// the sand flows; the hall's entrances are numbered on from it.
#define FACE_HALVES_HALL_FIRST_ENTRANCE                                        \
  ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST

/// The moves a rise of the sand takes, and the sand rows' end marker.
#define SAND_RISE_MOVES 16
#define SAND_ROWS_END 99

/// The sprite the sand's edge masks the screen with: 16 by 16 pixels, four
/// tiles of colour 1.
#define SAND_MASK_BYTES (4 * TILE_SIZE_4BPP)

/// The halls of the stone faces (gAnkohlRuins_StoneFacesPlacements).
enum AnkohlRuinsStoneFacesActorId {
  /// The pillar beside the Nut's chest, in the room behind the westmost face.
  ACTOR_NUT_CHEST_PILLAR = ACTOR_FIRST_PLACED,
  /// The seven stone faces in the north wall, west to east.
  ///@{
  ACTOR_STONE_FACE_A = ACTOR_FIRST_PLACED + 2,
  ACTOR_STONE_FACE_B,
  ACTOR_STONE_FACE_C,
  /// Unused: placed at (0, 0), off the room; at its place in the row the
  /// wall's own face stands split open on a doorway.
  ACTOR_STONE_FACE_D,
  ACTOR_STONE_FACE_E,
  ACTOR_STONE_FACE_F,
  ACTOR_STONE_FACE_G,
  ///@}
  /// The fourteen clumps of weeds.
  ///@{
  ACTOR_STONE_FACES_WEEDS_A,
  ACTOR_STONE_FACES_WEEDS_B,
  ACTOR_STONE_FACES_WEEDS_C,
  ACTOR_STONE_FACES_WEEDS_D,
  ACTOR_STONE_FACES_WEEDS_E,
  ACTOR_STONE_FACES_WEEDS_F,
  ACTOR_STONE_FACES_WEEDS_G,
  ACTOR_STONE_FACES_WEEDS_H,
  ACTOR_STONE_FACES_WEEDS_I,
  ACTOR_STONE_FACES_WEEDS_J,
  ACTOR_STONE_FACES_WEEDS_K,
  ACTOR_STONE_FACES_WEEDS_L,
  ACTOR_STONE_FACES_WEEDS_M,
  ACTOR_STONE_FACES_WEEDS_N,
  ///@}
};

/// The Thanatos Mace's rooms (gAnkohlRuins_ThanatosMaceChestPlacements).
enum AnkohlRuinsThanatosMaceChestActorId {
  /// The left half of a stone face, pushed into place.
  ACTOR_THANATOS_FACE_HALF = ACTOR_FIRST_PLACED,
  /// Its three clumps of weeds.
  ///@{
  ACTOR_THANATOS_WEEDS_A,
  ACTOR_THANATOS_WEEDS_B,
  ACTOR_THANATOS_WEEDS_C
  ///@}
};

/// The great face's rooms (gAnkohlRuins_GreatFacePlacements).
enum AnkohlRuinsGreatFaceActorId {
  ACTOR_GREAT_FACE_ROOMS_PILLAR = ACTOR_FIRST_PLACED,
  /// The great stone face, which slides down its corridor once its right half
  /// is in place.
  ACTOR_GREAT_FACE,
  /// Its right half, which Move pushes against it.
  ACTOR_GREAT_FACE_HALF
};

/// The face halves' rooms (gAnkohlRuins_FaceHalvesPlacements).
enum AnkohlRuinsFaceHalvesActorId {
  ACTOR_FACE_HALVES_PILLAR = ACTOR_FIRST_PLACED,
  /// The two halves of a stone face: the left one stands in the room, the
  /// right one falls in through the Crystal Powder's rooms' gap.
  ///@{
  ACTOR_LEFT_FACE_HALF,
  ACTOR_RIGHT_FACE_HALF,
  ///@}
  /// Its seven clumps of weeds.
  ///@{
  ACTOR_FACE_HALVES_WEEDS_A,
  ACTOR_FACE_HALVES_WEEDS_B,
  ACTOR_FACE_HALVES_WEEDS_C,
  ACTOR_FACE_HALVES_WEEDS_D,
  ACTOR_FACE_HALVES_WEEDS_E,
  ACTOR_FACE_HALVES_WEEDS_F,
  ACTOR_FACE_HALVES_WEEDS_G,
  ///@}
  ACTOR_FACE_HALVES_SECOND_PILLAR,
  ACTOR_FACE_HALVES_THIRD_PILLAR
};

/// The Crystal Powder's rooms (gAnkohlRuins_CrystalPowderChestPlacements).
enum AnkohlRuinsCrystalPowderChestActorId {
  ACTOR_CRYSTAL_POWDER_PILLAR = ACTOR_FIRST_PLACED,
  /// The half face pushed into the gap Reveal shows.
  ACTOR_FACE_HALF_FOR_THE_GAP
};

/// The Left Prong, lying on the top of the ruins.
#define ACTOR_LEFT_PRONG_CHEST 64

void AnkohlRuins_SettleObject(s32 id, s32 cue);
void AnkohlRuins_RaiseFirstSand(void);
void AnkohlRuins_RaiseSecondSand(void);
void AnkohlRuins_SlideGreatFace(void);

/// Whether the game flag is set.
static inline s32 AnkohlRuins_FlagCheck(s32 flag) { return GameFlag_Get(flag); }

#include "field/field.h"
#include "flags.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event_commands.h"
#include "field/field_sequence.h"
#include "scene/scene.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/map_grid.h"
#include "scene_ids.h"
#include "graphics/vram_tab.h"
#include "system/iwram_call.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/task.h"
#include "system/frame.h"

#include "shared/scene_create_imported_actor.h"

#include "../field/field_effect_move.c"

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct AnkohlRuinsSpriteWork gAnkohlRuins_Sand BSS_DATA ALIGNED(16);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gAnkohlRuins_SandStepDelay;
extern s16 gAnkohlRuins_SandTimer;
extern s16 gAnkohlRuins_SandRiseDelay;
extern s32 gAnkohlRuins_SandStep;
extern s32 gAnkohlRuins_SandRoom;

extern struct SceneRegion gAnkohlRuins_CrystalPowderChestExitRegions[];
extern struct SceneRegion gAnkohlRuins_LeftProngExitRegions[];

const struct SceneRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    return gAnkohlRuins_CrystalPowderChestExitRegions;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
    return gAnkohlRuins_LeftProngExitRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Jumps actor `id` down a ledge (Actor_JumpDownLedge).
static __inline__ void AnkohlRuins_JumpActorDownLedge(s32 id, s32 pose,
                                                      s32 param) {
  Actor_JumpDownLedge(id, pose, param);
}

/// Felix walks south into the pit in the middle of a small room of the Crystal
/// Powder's rooms and jumps down it; the screen fades and he drops into the
/// face halves' rooms beside the left half of the stone face, landing in a
/// ring of dust (AnkohlRuins_LandFromFall).
void AnkohlRuins_JumpDownLedge(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  AnkohlRuins_JumpActorDownLedge(gMainRoutineWork.leaderActorId,
                                 ANIM_LEADER_SPRING,
                                 LEDGE_JUMP_NO_LANDING_WAIT);
  Event_PlaySound(SE_FALL);
  Event_SetFlag(FLAG_ARRIVE_FALLING);
  FieldEvent_SetExitRequest(
      ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTHWEST);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller AnkohlRuins_LandFromFall exists in the Japanese
// scene.
#if TLA_PATCH_1
#include "../field/field_effect_advance_motion.c"
#endif

#if TLA_PATCH_1

/// Felix drops into the face halves' rooms from the pit above, lands sitting
/// in a ring of dust and shakes the ground, then stands up. From patch 1.
void AnkohlRuins_LandFromFall(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 i;
  s32 angle;
  FIXED velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  MapGrid_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y.fixed = toFIXED(130);
  leader->gravity = toFIXED(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 16 / 25;
  options.startScaleY = FIXED_ONE * 16 / 25;
  i = 0;
  do {
    angle = i * (TRIG_TURN / DUST_RING_PUFFS);
    velocity[0] = Trig_Cos(angle);
    velocity[1] = 0;
    velocity[2] = Math_Sin(angle);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14 >> 16) - (toFIXED(3) / 8);
    velocity[2] += ((Random16() * 3) << 13 >> 16) - (toFIXED(3) / 16);
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
    i++;
  } while (i <= DUST_RING_PUFFS);
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_SIT);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = toFIXED(1);
  leader->bounce = FIXED_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

#endif

/// The call gAnkohlRuins_WeedsScript makes: clears the clump's sprite flags,
/// draws it at priority 2 and stands it four pixels lower.
s32 AnkohlRuins_SetUpWeeds(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  ((struct FieldSprite *)actor->sprite)->priority = 2;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->motionFlags = 0;
  actor->y.fixed -= toFIXED(4);
  return 0;
}

extern struct ActorPlacement gAnkohlRuins_Placements[];
extern struct ActorPlacement gAnkohlRuins_StoneFacesPlacements[];
extern struct ActorPlacement gAnkohlRuins_ThanatosMaceChestPlacements[];
extern struct ActorPlacement gAnkohlRuins_GreatFacePlacements[];
extern struct ActorPlacement gAnkohlRuins_FaceHalvesPlacements[];
extern struct ActorPlacement gAnkohlRuins_CrystalPowderChestPlacements[];
extern struct ActorPlacement gAnkohlRuins_SandDoorsPlacements[];
extern struct ActorPlacement gAnkohlRuins_LeftProngPlacements[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    return gAnkohlRuins_StoneFacesPlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
    return gAnkohlRuins_ThanatosMaceChestPlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
    return gAnkohlRuins_GreatFacePlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    return gAnkohlRuins_FaceHalvesPlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    return gAnkohlRuins_CrystalPowderChestPlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_SAND_DOORS)
    return gAnkohlRuins_SandDoorsPlacements;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
    return gAnkohlRuins_LeftProngPlacements;
  return gAnkohlRuins_Placements;
}

/// Felix sinks into the ground with Sand and moves on the sand layer, under
/// the ruins' blocks and pillars.
void AnkohlRuins_SinkIntoSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// Felix comes up out of the Sand and walks on the ground again.
void AnkohlRuins_RiseFromSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Nothing visible happens as Felix starts to cast Move on one of the ruins'
/// pillars; the game rewrites the floor cells around the pillar for the push.
/// It rewrites the cells beside the pillar by the Nut's chest, the great face's
/// rooms' pillar, the face halves' rooms' three and the Crystal Powder's rooms'
/// one. For the half face beside the Crystal Powder's rooms' gap it copies in
/// the gap's cells, from patch 1 only while Reveal shows the gap.
void AnkohlRuins_StartMovingPillar(s32 unused, s32 id) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES) {
    if (id == ACTOR_NUT_CHEST_PILLAR) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 4, 69);
      MapGrid_CopyCellAttributes(3, 68, 1, 1, 3, 70);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (id == ACTOR_FACE_HALVES_THIRD_PILLAR) {
      MapGrid_CopyCellAttributes(35, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 37, 73);
    }
    if (id == ACTOR_FACE_HALVES_PILLAR)
      MapGrid_CopyCellAttributes(2, 113, 1, 1, 3, 113);
    if (id == ACTOR_FACE_HALVES_SECOND_PILLAR)
      MapGrid_CopyCellAttributes(39, 93, 1, 1, 40, 93);
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (id == ACTOR_GREAT_FACE_ROOMS_PILLAR) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 32, 70);
      MapGrid_CopyCellAttributes(33, 69, 1, 1, 33, 71);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (id == ACTOR_CRYSTAL_POWDER_PILLAR) {
      MapGrid_CopyCellAttributes(17, 85, 1, 1, 17, 83);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 18, 82);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 19, 83);
    }
#if !TLA_PATCH_1
    if (id == ACTOR_FACE_HALF_FOR_THE_GAP)
#else
    if (id == ACTOR_FACE_HALF_FOR_THE_GAP &&
        Event_GetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN))
#endif
      MapGrid_CopyCells(35, 98, 3, 5, 25, 98);
  }
}

/// Felix pushes one of the ruins' pillars a cell along with Move, and pushed to
/// the end of its short track it stays there for good: the one beside the
/// Nut's chest slides east into its groove.
/// The game fixes the cells the pillar has left and entered, and at the
/// track's end sets the pillar's flag, so that the set-up puts it there again.
void AnkohlRuins_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (id == ACTOR_NUT_CHEST_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES) {
    if (FIELD_CELL(actor->x.fixed) == NUT_CHEST_PILLAR_END_COLUMN) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
      Event_SetFlag(FLAG_NUT_CHEST_PILLAR_MOVED);
    }
    MapGrid_CopyCellAttributes(3, 68, 1, 1, 4, 69);
    MapGrid_CopyCellAttributes(3, 69, 1, 1, 3, 70);
  }
  if (id == ACTOR_GREAT_FACE_ROOMS_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    MapGrid_CopyCellAttributes(31, 70, 1, 1, 32, 70);
    if (FIELD_CELL(actor->x.fixed) == GREAT_FACE_ROOMS_PILLAR_END_COLUMN) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (id == ACTOR_FACE_HALVES_PILLAR) {
      MapGrid_CopyCellAttributes(3, 114, 1, 1, 3, 113);
      if (FIELD_CELL(actor->x.fixed) == FACE_HALVES_PILLAR_END_COLUMN)
        Event_SetFlag(FLAG_FACE_HALVES_PILLAR_MOVED);
    }
    if (id == ACTOR_FACE_HALVES_SECOND_PILLAR) {
      MapGrid_CopyCellAttributes(40, 94, 1, 1, 40, 93);
      if (FIELD_CELL(actor->x.fixed) == FACE_HALVES_SECOND_PILLAR_END_COLUMN)
        Event_SetFlag(FLAG_FACE_HALVES_SECOND_PILLAR_MOVED);
    }
    if (id == ACTOR_FACE_HALVES_THIRD_PILLAR) {
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (FIELD_CELL(actor->x.fixed) == FACE_HALVES_THIRD_PILLAR_END_COLUMN) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(FLAG_FACE_HALVES_THIRD_PILLAR_MOVED);
      }
    }
  }
  if (id == ACTOR_CRYSTAL_POWDER_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (FIELD_CELL(actor->x.fixed) == CRYSTAL_POWDER_PILLAR_END_COLUMN) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(FLAG_CRYSTAL_POWDER_PILLAR_MOVED);
    }
  }
}

/// Felix pushes one of the seven stone faces in the north wall of the halls of
/// the stone faces south off its ledge with Move; it falls with a puff of dust
/// and is gone for good, leaving open the doorway it stood in front of.
void AnkohlRuins_PushStoneFaceOffLedge(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *actor = Actor_Get(id);
  s16 frames = 60;

  if (FIELD_CELL(actor->z.fixed) == STONE_FACE_DROP_ROW) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_SetSpritePriorityDirect(actor, 3);
    Event_PlaySound(SE_DUST_PUFF);
    while (frames != 0) {
      System_Wait(1);
      if (actor->velocityY == 0)
        break;
      frames--;
    }
    Actor_SetPosition(id, 0, 0);
    if (id == ACTOR_STONE_FACE_A)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A));
    if (id == ACTOR_STONE_FACE_B)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B));
    if (id == ACTOR_STONE_FACE_C)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C));
    if (id == ACTOR_STONE_FACE_D)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D));
    if (id == ACTOR_STONE_FACE_E)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E));
    if (id == ACTOR_STONE_FACE_F)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F));
    if (id == ACTOR_STONE_FACE_G)
      Event_SetFlag(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G));
  }
}

/// A pushed half face, or Felix, falls to the ground below and lands, the cue
/// sounding if there is one: it falls for at most a second, and ten frames
/// after it lands it stands still again. Gaia Rock's GaiaRock_DropPillar is
/// the same routine in every edition but the Japanese, whose
/// GaiaRock_DropPillar plays no cue and tests for landing before counting the
/// frame down.
void AnkohlRuins_SettleObject(s32 id, s32 cue) {
  s32 frames = 60;
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (actor->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  actor->motionFlags = 0;
}

/// Sand pours down from above: `rows` rows of `count` grains a cell apart,
/// each row two pixels further south, `frames` apart.
void AnkohlRuins_PourSand(s32 rows, s32 frames, s32 x, s32 y, s32 startZ,
                          s32 count) {
  struct EffectOptions params;
  s32 row;
  s32 left;
  s32 dx;
  s32 dy;
  s32 z;

  params.palette = 1;
  params.startScaleX = FIXED_ONE * 3 / 4;
  params.startScaleY = FIXED_ONE * 3 / 4;
  row = 0;
  if (row < rows) {
    z = startZ;
    do {
      for (left = 0; left < count; left++) {
        {
          s32 r = Random16();
          dx = 15;
          dx &= r;
        }
        dy = Random16() & 31;
        FieldEffect_Spawn(
            INTtoFIXED(x + left * MAPGRID_CELL_PIXELS) + INTtoFIXED(dx - 8),
            INTtoFIXED(y + dy), INTtoFIXED(z), 0, 0, toFIXED(1) / 256,
            EFFECT_USE_START_SCALE | 1, &params);
      }
      FieldEvent_Wait(frames);
      z += 2;
      row++;
    } while (row < rows);
  }
}

/// Felix pushes a half of the stone face in the face halves' rooms with Move,
/// and once it reaches its place, the left half at cell (12, 45) and the
/// right half beside it at (14, 45), it settles with a boom. When both halves
/// are in place the ground shakes, the camera looks at the face, and sand
/// pours down below it, row by row, into a slope.
/// The slope is the face's cells copied in from beside the room, a row at a
/// time, as in AnkohlRuins_PlaceThanatosFaceHalf.
void AnkohlRuins_PlaceFaceHalves(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 column = FIELD_CELL(actor->x.fixed);
  s32 row = FIELD_CELL(actor->z.fixed);
  s32 toColumn;
  s32 lowRow;
  s32 highRow;
  s32 height;
  s32 count;

  if (id == ACTOR_LEFT_FACE_HALF && column == 12 && row == 45) {
    Event_SetFlag(FLAG_LEFT_FACE_HALF_PLACED);
    AnkohlRuins_SettleObject(ACTOR_LEFT_FACE_HALF, SE_BOOM);
  }
  if (id == ACTOR_RIGHT_FACE_HALF && column == 14 && row == 45) {
    Event_SetFlag(FLAG_RIGHT_FACE_HALF_PLACED);
    AnkohlRuins_SettleObject(ACTOR_RIGHT_FACE_HALF, SE_BOOM);
  }
  if (!GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED) ||
      !GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED))
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(2, 2, 1);
  FieldCamera_MoveTo(216, CAMERA_KEEP, 770, 1);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_FLAME_FLARE);
  toColumn = 12;
  highRow = 111;
  MapGrid_CopyCellsLayered(12, 91, 3, 1, toColumn, highRow);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
  lowRow = 47;
  MapGrid_CopyCells(16, 27, 3, 2, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 2, toColumn, highRow);
  height = -40;
  AnkohlRuins_PourSand(3, 15, 200, height, 744, count = 3);
  MapGrid_CopyCells(16, 27, 3, 3, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 3, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, height, 760, count);
  MapGrid_CopyCells(16, 27, 3, 4, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 4, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, height, 776, count);
  MapGrid_CopyCells(16, 27, 3, 5, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 5, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, height, 792, count);
  MapGrid_CopyCells(16, 27, 3, 6, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 6, toColumn, highRow);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  AnkohlRuins_PourSand(3, 10, 200, height, 808, count);
  MapGrid_CopyCells(16, 27, 3, 13, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 13, toColumn, highRow);
  Event_End();
}

/// Felix pushes the left half of the stone face in the back room of the
/// Thanatos Mace's rooms east with Move, and when it meets the right half in
/// the wall, at cell (12, 5), it settles with a boom: the ground shakes, the
/// camera looks at the face, and sand pours down from it, row by row, and
/// fills the hall below it.
void AnkohlRuins_PlaceThanatosFaceHalf(s32 unused, s32 id) {
  struct Actor *pillar = Actor_GetById(id);
  s32 column = FIELD_CELL(pillar->x.fixed);
  s32 row = FIELD_CELL(pillar->z.fixed);

  if (id != ACTOR_THANATOS_FACE_HALF || column != 12 || row != 5)
    return;
  Event_SetFlag(FLAG_THANATOS_FACE_HALF_PLACED);
  AnkohlRuins_SettleObject(ACTOR_THANATOS_FACE_HALF, SE_BOOM);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_FLAME_FLARE);
  MapGrid_CopyCells(29, 70, 3, 1, 12, 71);
  FieldCamera_MoveTo(216, CAMERA_KEEP, 120, 1);
  MapGrid_SetShake(2, 2, 1);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
  MapGrid_CopyCells(35, 6, 7, 2, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 2, 10, 71);
  AnkohlRuins_PourSand(6, 10, 200, -40, 124, 3);
  MapGrid_CopyCells(35, 6, 7, 3, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 3, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 140, 5);
  MapGrid_CopyCells(35, 6, 7, 4, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 4, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 156, 5);
  MapGrid_CopyCells(35, 6, 7, 5, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 5, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 172, 5);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_CopyCells(35, 6, 7, 6, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 6, 10, 71);
  FieldEvent_Wait(15);
  MapGrid_CopyCells(35, 6, 7, 16, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 16, 10, 71);
  Event_End();
}

/// Felix pushes the right half of the great stone face west with Move, and when
/// it meets the face, at cell (15, 24), it sinks into place with a thump, the
/// face is whole, and its eyes glow green.
/// Now stepping into the face's corridor sets it sliding
/// (AnkohlRuins_StartGreatFaceSliding).
void AnkohlRuins_PlaceGreatFaceHalf(s32 unused, s32 id) {
  struct Actor *actor = Actor_GetById(id);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  if (id == ACTOR_GREAT_FACE_HALF && x == 15 && z == 24) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_GREAT_FACE_HALF), 0);
    actor->y.fixed -= toFIXED(2);
    FieldEvent_Wait(1);
    actor->y.fixed -= toFIXED(2);
    FieldEvent_Wait(1);
    actor->y.fixed -= toFIXED(4);
    FieldEvent_Wait(1);
    actor->y.fixed -= toFIXED(8);
    FieldEvent_Wait(1);
    Actor_SetAnimation(ACTOR_GREAT_FACE, 3);
    Actor_SetPosition(ACTOR_GREAT_FACE_HALF, 0, 0);
    Event_PlaySound(SE_THUMP);
    FieldEvent_Wait(45);
    Actor_SetAnimation(ACTOR_GREAT_FACE, 4);
    Event_PlaySound(SE_EYES_GLOW);
    Event_SetFlag(FLAG_GREAT_FACE_HALF_PLACED);
  }
}

extern struct MapCell gMapGridCellBuffer[];

/// Felix, standing on the sand that flows down the face halves' rooms' slide
/// or the Crystal Powder's rooms' sand, drifts south with it, and where it
/// runs over a drop it carries him off (AnkohlRuins_CarriedOffLedge).
/// It runs every frame from the room's set-up, and moves him and the camera
/// half a pixel south on cells marked MAP_CELL_FLOWING_SAND, unless he is
/// under the Sand.
void AnkohlRuins_FlowWithSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapGrid;
  struct Actor *companion = work->camera;
  struct MapCell *ground;
  struct MapCell *upper;
  s32 index;
  FIXED position[3];

  if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS))
    return;
  if (map == NULL) {
    upper = gMapGridCellBuffer;
    ground = gMapGridCellBuffer;
  } else {
    ground = map->layers[0].cells;
    upper = map->layers[2].cells;
  }
  index = FIELD_CELL(leader->x.fixed) + (FIELD_CELL(leader->z.fixed) << 7);
  ground += index;
  upper += index;
  if (((ground->attributeB & MAP_CELL_FLOWING_SAND) |
       (upper->collisionCode & CELL_BEHAVIOR_SAND)) !=
      (MAP_CELL_FLOWING_SAND | CELL_BEHAVIOR_SAND))
    return;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_SAND)
    return;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(leader, position) < 0)
    work->stepTrigger = TRIGGER_CARRIED_OFF_LEDGE;
  leader->z.fixed += toFIXED(1) / 2;
  companion->z.fixed += toFIXED(1) / 2;
}

/// Felix, carried by the flowing sand to the edge of a drop in the Crystal
/// Powder's rooms, slides over it, starts with a "!" and falls to the floor
/// below with a thump.
void AnkohlRuins_CarriedOffLedge(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STILL);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  actor->motionFlags = 0;
  actor->steeringFlags = 0;
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, 8);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SE_DUST_PUFF);
  AnkohlRuins_SettleObject(gMainRoutineWork.leaderActorId, SE_THUMP);
  actor->steeringFlags = ACTOR_FACES_MOTION;
  Event_End();
}

/// Felix pushes a face half or a pillar in the face halves' rooms by walking
/// into it: once he has leant on it a moment where the floor lets him push,
/// the field shoves it a cell ahead (Field_RunPushCell) and then this runs, the
/// halves going on as AnkohlRuins_PlaceFaceHalves and the pillars as
/// AnkohlRuins_FinishMovingPillar, as after Move. A push by Move does not come
/// here: its own rows run those two directly.
void AnkohlRuins_FinishPushing(s32 a, s32 id) {
  if (id == ACTOR_LEFT_FACE_HALF || id == ACTOR_RIGHT_FACE_HALF)
    AnkohlRuins_PlaceFaceHalves(a, id);
  else
    AnkohlRuins_FinishMovingPillar(a, id);
}

/// Felix casts Whirlwind ("Conjure wind to scatter weeds.") on a clump of weeds
/// in the halls of the stone faces, the Thanatos Mace's rooms or the face
/// halves' rooms: the leaves blow away and the clump is gone for good. Most
/// hide nothing; a few hide the way on, such as the one in the middle of the
/// entrance hall and the one at the upper left of the halls, over a stairway.
/// Whirlwind itself blows the clump away; this only sets its flag, so that
/// the set-up leaves it out next time.
void AnkohlRuins_ScatterWeeds(s32 a, s32 id) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    Event_SetFlag(FLAG_STONE_FACES_WEEDS_SCATTERED(id));
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
    Event_SetFlag(FLAG_THANATOS_WEEDS_SCATTERED(id));
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    Event_SetFlag(FLAG_FACE_HALVES_WEEDS_SCATTERED(id));
}

/// The rising sand of the sand rooms, drawn each frame: the sand's picture is
/// raised to its level, and its edges against the ledges are trimmed.
/// Until the sand is 32 pixels deep, and while Reveal is not up, eight sprites
/// drawn into the object window mask its edge, each moved by the level as its
/// kind says; map layer 1, the sand, is scrolled by the level over its base.
void AnkohlRuins_DrawSand(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  // DEAD: viewY, subtracted from both y and z, cancels in screenY; the ROM
  // keeps its load and its store to the stack.
  s32 viewY = map->origin[1];
  s32 tile = gVramCacheEntries[work->tile].offset >> 5;
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN) == 0 &&
      work->level < toFIXED(32)) {
    for (i = 0; i <= 7; i++) {
      u32 kind = sprite->kind;
      FIXED x = sprite->x;
      s32 y = sprite->y;
      s32 z = sprite->z;
      s32 screenX, screenY;

      switch (kind) {
      case 0:
        x -= work->level;
        y += work->level;
        if (work->level > toFIXED(16))
          x = -toFIXED(16);
        break;
      case 1:
        x -= work->level;
        y += work->level;
        break;
      case 2:
        x += work->level;
        y += work->level;
        break;
      case 3:
        x += work->level;
        y += work->level;
        if (work->level > toFIXED(16))
          x = -toFIXED(16);
        break;
      case 4:
        if (work->level < toFIXED(16))
          y += work->level;
        else
          x = -toFIXED(16);
        break;
      }
      x -= viewX;
      y -= viewY;
      z = z - viewZ - viewY;
      screenY = z - y;
      screenX = FIXEDtoINT(x) - 8;
      screenY = FIXEDtoINT(screenY) - 8;
      if (screenX >= -15 && screenX <= 319 && screenY >= -15 &&
          screenY <= 239) {
        screenX &= 511;
        screenY &= 255;
        sprite->oam[0] = 0;
        sprite->oam[1] = screenY | OBJ_X(screenX) | OBJ_SIZE_16X16 |
                         OBJ_MODE(OBJ_MODE_WINDOW);
        sprite->oam[2] = tile | OBJ_PRIORITY(1);
        Oam_AddSpriteAtDepthClamped(sprite->oam, OAM_DEPTH_FRONT);
      }
      sprite++;
    }
  }
  {
    struct MapLayer *layer = &map->layers[1];

    layer->offsetY = work->baseOffsetY + work->level;
  }
}

/// Sets up the sand rooms' rising sand on entering one: the tile of the masks
/// that trim its edge, the window they draw into, the task that draws it, and
/// the masks for the room the entrance leads into.
void AnkohlRuins_InitSand(s32 unused) {
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  u32 *tiles = Heap_Alloc(SAND_MASK_BYTES);
  u32 *pixel = tiles;
  struct MapLayer *layer =
      &((struct MapWork *)Ram_HeapSlots->mapGrid)->layers[1];
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Iwram_ClearWordsEntry(work, sizeof(*work));
  for (i = 0; i < SAND_MASK_BYTES / 4; i++) {
    *pixel++ = PIXELS_4BPP(1);
  }
  *(s16 *)&work->tile = VramCache_FindFreeEntry();
  Vram_Load(work->tile, SAND_MASK_BYTES, tiles);
  Heap_Release(tiles);
  work->baseOffsetX = layer->offsetX;
  work->baseOffsetY = layer->offsetY;
  Io_Write16(WINOUT_WIN01_BG0 | WINOUT_WIN01_BG1 | WINOUT_WIN01_BG2 |
                 WINOUT_WIN01_BG3 | WINOUT_WIN01_OBJ | WINOUT_WINOBJ_BG0 |
                 WINOUT_WINOBJ_BG1 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_OBJ,
             &REG_WINOUT);
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Task_AddFrame(AnkohlRuins_DrawSand, TASK_PASS_FRAME);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    sprite->x = toFIXED(40);
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = toFIXED(120);
    sprite++;
    sprite->x = toFIXED(56);
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = toFIXED(120);
    sprite++;
    sprite->x = toFIXED(216);
    sprite->y = 0;
    sprite->z = toFIXED(120);
    sprite->kind = 2;
    sprite++;
    sprite->x = toFIXED(232);
    sprite->y = 0;
    sprite->z = toFIXED(120);
    sprite->kind = 2;
    sprite++;
    sprite->x = toFIXED(184);
    sprite->z = toFIXED(88);
    sprite->y = 0;
    sprite->kind = 4;
    sprite++;
  }
  if (*(s16 *)&gMainRoutineWork.entrance == 3 ||
      *(s16 *)&gMainRoutineWork.entrance == 4) {
    sprite->x = toFIXED(40);
    sprite->z = toFIXED(440);
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = toFIXED(56);
    sprite->z = toFIXED(440);
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = toFIXED(216);
    sprite->y = 0;
    sprite->z = toFIXED(408);
    sprite->kind = 0;
    sprite++;
    sprite->x = toFIXED(232);
    sprite->y = 0;
    sprite->z = toFIXED(408);
    sprite->kind = 1;
    sprite++;
    sprite->x = toFIXED(72);
    sprite->z = toFIXED(376);
    sprite->y = 0;
    sprite->kind = 4;
  }
}

/// Counts the time to the sand's next rise after the switch; when it runs out,
/// the room's sand rises again (TRIGGER_RAISE_FIRST_SAND or
/// TRIGGER_RAISE_SECOND_SAND).
void AnkohlRuins_CountSandTime(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;

  if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
    if (++gAnkohlRuins_SandTimer >= gAnkohlRuins_SandRiseDelay) {
      FieldTask_Remove(AnkohlRuins_CountSandTime);
      if (gAnkohlRuins_SandRoom == 1)
        work->raisedTrigger = TRIGGER_RAISE_FIRST_SAND;
      else
        work->raisedTrigger = TRIGGER_RAISE_SECOND_SAND;
    }
  }
}

extern struct AnkohlRuinsSandRow gAnkohlRuins_FirstSandRows[];

/// The sand in the first sand room rises a step as the ground shakes and the
/// great stone face in the north wall pours, lifting Felix if he stands on it,
/// until the room is full and the door button on the far ledge can be reached.
/// A step takes SAND_RISE_MOVES moves; after it the room's cells, read from
/// gAnkohlRuins_FirstSandRows, follow the new level, and while the sand is
/// below 32 pixels the next rise is timed (AnkohlRuins_CountSandTime).
void AnkohlRuins_RaiseFirstSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapGrid;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s16 i;
  s16 j;
  s16 m;
  s32 row;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STILL);
  m = CELL_HEIGHT_STEPS(work->level);
  leader->motionFlags = 0;
  MapGrid_SetShake(4, 4, 1);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 1);
  for (i = 0; i < SAND_RISE_MOVES; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells +=
        leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
    if (cells->collisionCode == CELL_BEHAVIOR_SAND) {
      leader->y.fixed += gAnkohlRuins_SandStep;
      leader->groundHeight = leader->y.fixed;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 11, 68);
      MapGrid_CopyCellAttributes(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == SAND_RISE_MOVES - 1)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 11, 68);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(5);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (work->level < toFIXED(32)) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, TASK_PASS_FRAME);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 1);
  i = CELL_HEIGHT_STEPS(work->level);
  for (row = 0; gAnkohlRuins_FirstSandRows[row].level != SAND_ROWS_END; row++) {
    if (i >= gAnkohlRuins_FirstSandRows[row].level) {
      for (m = 0; m < gAnkohlRuins_FirstSandRows[row].height; m++) {
        for (n = 0; n < gAnkohlRuins_FirstSandRows[row].width; n++) {
          MapGrid_CopyCellAttributes(21, 14 - i, 1, 1,
                                     gAnkohlRuins_FirstSandRows[row].x + n,
                                     gAnkohlRuins_FirstSandRows[row].z + m);
          MapGrid_CopyCellAttributes(
              25, 14 - i, 1, 1, gAnkohlRuins_FirstSandRows[row].x + n,
              gAnkohlRuins_FirstSandRows[row].z + m + 64);
        }
      }
    }
  }
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 7);
  MapGrid_CopyCellAttributes(22, 14 - i, 2, 1, 13, 7);
  MapGrid_CopyCellAttributes(18, 14 - i, 1, 1, 7, 5);
  MapGrid_CopyCellAttributes(24, 14 - i, 1, 1, 9, 5);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 71);
  MapGrid_CopyCellAttributes(22, 14 - i, 2, 1, 13, 71);
  MapGrid_CopyCellAttributes(26, 14 - i, 1, 1, 7, 69);
  MapGrid_CopyCellAttributes(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    MapGrid_CopyCellAttributes(18, 8, 1, 1, 11, 5);
}

extern struct AnkohlRuinsSandRow gAnkohlRuins_SecondSandRows[];

/// The sand in the second sand room rises a step, as in the first
/// (AnkohlRuins_RaiseFirstSand), the shaking harder.
void AnkohlRuins_RaiseSecondSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapGrid;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  s16 m;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STILL);
  level = CELL_HEIGHT_STEPS(work->level);
  leader->motionFlags = 0;
  MapGrid_SetShake(5, 5, 1);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 19);
  for (i = 0; i < SAND_RISE_MOVES; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells +=
        leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
    if (cells->collisionCode == CELL_BEHAVIOR_SAND) {
      leader->y.fixed += gAnkohlRuins_SandStep;
      leader->groundHeight = leader->y.fixed;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 4, 86);
      MapGrid_CopyCellAttributes(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == SAND_RISE_MOVES - 1)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 4, 86);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(5);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (work->level < toFIXED(32)) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, TASK_PASS_FRAME);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 19);
  i = CELL_HEIGHT_STEPS(work->level);
  for (row = 0; gAnkohlRuins_SecondSandRows[row].level != SAND_ROWS_END;
       row++) {
    if (i >= gAnkohlRuins_SecondSandRows[row].level) {
      for (m = 0; m < gAnkohlRuins_SecondSandRows[row].height; m++) {
        for (n = 0; n < gAnkohlRuins_SecondSandRows[row].width; n++) {
          MapGrid_CopyCellAttributes(21, 14 - i, 1, 1,
                                     gAnkohlRuins_SecondSandRows[row].x + n,
                                     gAnkohlRuins_SecondSandRows[row].z + m);
          MapGrid_CopyCellAttributes(
              25, 14 - i, 1, 1, gAnkohlRuins_SecondSandRows[row].x + n,
              gAnkohlRuins_SecondSandRows[row].z + m + 64);
        }
      }
    }
  }
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 27);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 13, 25);
  MapGrid_CopyCellAttributes(18, 14 - i, 1, 1, 7, 23);
  MapGrid_CopyCellAttributes(24, 14 - i, 1, 1, 9, 23);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 91);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 13, 89);
  MapGrid_CopyCellAttributes(26, 14 - i, 1, 1, 7, 87);
  MapGrid_CopyCellAttributes(27, 14 - i, 1, 1, 9, 87);
  if (i == 2)
    MapGrid_CopyCellAttributes(18, 7, 1, 1, 4, 23);
}

/// Felix steps onto the grey switch on the west ledge of the first sand room
/// (cell (1, 7)): the great stone face in the north wall opens its mouth and
/// pours sand, which rises a step, then slowly another and another until the
/// room is full. Saving is barred while it rises.
void AnkohlRuins_PressFirstSandSwitch(void) {
  s32 done = AnkohlRuins_FlagCheck(FLAG_FIRST_SAND_SWITCH_PRESSED);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_BUTTON_PRESS);
    FieldEvent_Wait(15);
    Event_PlaySound(SE_FLAME_FLARE);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
    MapGrid_SetShake(2, 2, 1);
    FieldEvent_Wait(15);
    gAnkohlRuins_SandStepDelay = 7;
    gAnkohlRuins_SandTimer = done;
    gAnkohlRuins_SandRiseDelay = 200;
    gAnkohlRuins_SandStep = toFIXED(1) / 2;
    gAnkohlRuins_SandRoom = 1;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseFirstSand();
    Event_SetFlag(FLAG_FIRST_SAND_SWITCH_PRESSED);
  }
}

/// Defines a sand room's door button: the first time it is stepped on, under
/// flag n1, it clicks and the door at cell (n2, n3) slides open with a boom,
/// its cell taking (n4, n5)'s attributes.
#define DEFINE_ANKOHL_RUINS_OPEN_ONCE(name, n1, n2, n3, n4, n5)                \
  void name(void) {                                                            \
    if (AnkohlRuins_FlagCheck(n1) == 0) {                                      \
      Event_SetFlag(n1);                                                       \
      Event_PlaySound(SE_BUTTON_PRESS);                                        \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      MapGrid_CopyCells(26, 70, 1, 3, n2, n3);                                 \
      FieldEvent_Wait(20);                                                     \
      Event_PlaySound(SE_BOOM);                                                \
      MapGrid_CopyCells(26, 67, 1, 3, n2, n3);                                 \
      MapGrid_CopyCellAttributes(n4, n5, 1, 1, n2, n5);                        \
      Event_End();                                                             \
    }                                                                          \
  }

/// Felix steps on the first sand room's door button and the door in its
/// north-east wall, cell (14, 5), slides open. The button is the block 16
/// pixels high at cell (11, 5), east of the great stone face: once the sand
/// has risen two steps, to 16 pixels, AnkohlRuins_RaiseFirstSand gives that
/// cell the button's behaviour from cell (18, 8), which the map keeps east of
/// the rooms where no camera shows it, and the next rise takes it away again.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenFirstSandDoor,
                              FLAG_FIRST_SAND_DOOR_OPENED, 14, 65, 15, 5)

/// Felix steps onto the grey switch on the east ledge of the second sand room
/// (cell (12, 25)): the great stone face in the north wall pours sand, which
/// rises as in the first sand room but faster.
void AnkohlRuins_PressSecondSandSwitch(void) {
  s32 done = AnkohlRuins_FlagCheck(FLAG_SECOND_SAND_SWITCH_PRESSED);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_BUTTON_PRESS);
    FieldEvent_Wait(15);
    Event_PlaySound(SE_FLAME_FLARE);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 84);
    MapGrid_SetShake(2, 2, 1);
    FieldEvent_Wait(15);
    gAnkohlRuins_SandStepDelay = 3;
    gAnkohlRuins_SandTimer = done;
    gAnkohlRuins_SandRiseDelay = 100;
    gAnkohlRuins_SandStep = toFIXED(1) / 2;
    gAnkohlRuins_SandRoom = 2;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseSecondSand();
    Event_SetFlag(FLAG_SECOND_SAND_SWITCH_PRESSED);
  }
}

/// Felix steps on the second sand room's door button and the door in its
/// north-west wall, cell (2, 23), slides open. The button is the block 16
/// pixels high at cell (4, 23), two cells east of the door: once the sand has
/// risen two steps, to 16 pixels, AnkohlRuins_RaiseSecondSand gives that cell
/// the button's behaviour from cell (18, 7), which the map keeps east of the
/// rooms where no camera shows it, and the next rise takes it away again.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenSecondSandDoor,
                              FLAG_SECOND_SAND_DOOR_OPENED, 2, 83, 1, 23)

/// The great stone face slides south down its corridor, shoving Felix ahead
/// of it if he stands in its way and carrying him along if he stands right
/// behind it, unless he is under the Sand; it stops at the corridor's foot.
/// It moves four fifths of a pixel a frame. While it passes the sand patch
/// midway, rows 31 to 36, Sand cannot end (FLAG_SAND_CANNOT_END). Past row 41
/// it stops, the shaking dies away with a sound, and the six cells at the
/// corridor's foot where it comes to rest, (13, 41) to (15, 42), where the
/// corridor opens onto the hall below, become wall: Felix cannot walk through
/// the face there, and the cells that set the slide off are gone with them.
/// FLAG_SCENE_TEMP_01 keeps them so for this visit.
void AnkohlRuins_SlideGreatFace(void) {
  struct Actor *camera = gFieldWork->camera;
  struct Actor *face = Actor_GetById(ACTOR_GREAT_FACE);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  s32 column;
  s32 row;

  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) &&
      work->stepTrigger != CELL_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT)
    return;
  face->z.fixed += toFIXED(4) / 5;
  column = FIELD_CELL(leader->x.fixed);
  if (gMainRoutineWork.movementMode != LEADER_MOVEMENT_SAND && column > 12 &&
      column <= 15) {
    if (face->z.fixed + toFIXED(24) >= leader->z.fixed &&
        leader->z.fixed >= face->z.fixed) {
      leader->z.fixed = face->z.fixed + toFIXED(24);
      camera->z.fixed = face->z.fixed + toFIXED(24);
    }
    if (face->z.fixed >= leader->z.fixed &&
        leader->z.fixed >= face->z.fixed - toFIXED(16)) {
      leader->z.fixed = face->z.fixed - toFIXED(16);
      camera->z.fixed = face->z.fixed - toFIXED(16);
    }
  }
  row = FIELD_CELL(face->z.fixed);
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(FLAG_SAND_CANNOT_END)) {
    MapGrid_CopyCellAttributes(35, 83, 5, 4, 12, 96);
    Event_SetFlag(FLAG_SAND_CANNOT_END);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(FLAG_SAND_CANNOT_END)) {
    MapGrid_CopyCellAttributes(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(FLAG_SAND_CANNOT_END);
  }
  if (row > 41) {
    FieldTask_Remove(AnkohlRuins_SlideGreatFace);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_CopyCellAttributes(41, 48, 3, 2, 13, 41);
    Event_SetFlag(FLAG_SCENE_TEMP_01);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
    Event_ClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
    Event_PlaySound(SE_BANE_ESCAPE);
  }
}

/// Once the great face is whole, Felix stepping into the foot of its corridor
/// (cells (13, 42) to (15, 42)) sets it moving: there is a rush, the ground
/// shakes, the camera looks up the corridor at the face as it starts to slide
/// down towards him, and then follows Felix again.
/// Saving and battles are barred while it slides.
void AnkohlRuins_StartGreatFaceSliding(void) {
  s32 z = FIELD_CELL(Actor_Get(ACTOR_GREAT_FACE)->z.fixed);

  if (Event_GetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24) {
    s32 found = Task_Find(AnkohlRuins_SlideGreatFace);

    if (found == -1) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(SE_RUSH);
      MapGrid_SetShake(2, 2, 1);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(232, CAMERA_KEEP, 392, 1);
      FieldCamera_WaitForArrival();
      Task_AddFrame(AnkohlRuins_SlideGreatFace, TASK_PASS_FRAME);
      FieldEvent_Wait(40);
      EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
      FieldCamera_WaitForArrival();
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      Event_SetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
      Event_End();
    }
  }
}

/// Once the great face is whole, Felix stepping right in front of it at the top
/// of its corridor (cell (13, 26)) sets it sliding at once, the ground
/// shaking, with no look up the corridor.
void AnkohlRuins_StartGreatFaceSlidingUpClose(void) {
  s32 z = FIELD_CELL(Actor_Get(ACTOR_GREAT_FACE)->z.fixed);

  if (Event_GetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24) {
    void (*func)(void) = AnkohlRuins_SlideGreatFace;

    if (Task_Find(func) == -1) {
      MapGrid_SetShake(2, 2, 1);
      Task_AddFrame(func, TASK_PASS_FRAME);
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      Event_SetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
      Event_End();
    }
  }
}

extern struct Trigger gAnkohlRuins_Triggers[];
extern struct Trigger gAnkohlRuins_StoneFacesTriggers[];
extern struct Trigger gAnkohlRuins_ThanatosMaceChestTriggers[];
extern struct Trigger gAnkohlRuins_GreatFaceTriggers[];
extern struct Trigger gAnkohlRuins_FaceHalvesTriggers[];
extern struct Trigger gAnkohlRuins_CrystalPowderChestTriggers[];
extern struct Trigger gAnkohlRuins_SandDoorsTriggers[];
extern struct Trigger gAnkohlRuins_LeftProngTriggers[];

/// Felix pushes the half face in the room of brown and red blocks of the
/// Crystal Powder's rooms into the gap Reveal shows (cell (26, 35)): it falls
/// in, and the screen fades to the face halves' rooms below, where it drops
/// in from above with a thump, then back to Felix.
void AnkohlRuins_PushStatueIntoGap(void) {
  struct Actor *actor = Actor_Get(ACTOR_FACE_HALF_FOR_THE_GAP);

  if (FIELD_CELL(actor->x.fixed) == GAP_COLUMN &&
      FIELD_CELL(actor->z.fixed) == GAP_ROW) {
    Event_SetFlag(FLAG_FACE_HALF_PUSHED_INTO_GAP);
    Event_PlaySound(SE_DUST_PUFF);
    AnkohlRuins_SettleObject(ACTOR_FACE_HALF_FOR_THE_GAP, 0);
    Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(
        ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_C);
  }
#if !TLA_PATCH_1 /* The Japanese edition copies cells here. */
  MapGrid_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

/// Reveal in the Crystal Powder's rooms shows a gap in the floor of the room of
/// brown and red blocks, beside the half face (cells (25, 34) to (27, 38)).
void AnkohlRuins_RevealGap(void) {
#if TLA_PATCH_1 /* The Japanese edition leaves the flag alone. */
  Event_SetFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
#endif
  MapGrid_CopyCells(35, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(35, 98, 3, 3, 25, 98);
}

/// As Reveal fades, the gap in the room of brown and red blocks is hidden
/// again.
void AnkohlRuins_HideGap(void) {
#if TLA_PATCH_1
  Event_ClearFlag(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
#endif
  MapGrid_CopyCells(40, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(40, 98, 3, 3, 25, 98);
}

/// Nothing visible happens to the sand when Felix casts Reveal in the sand
/// rooms; the game stops trimming the sand's edge while Reveal is up.
void AnkohlRuins_StartReveal(void) {
  GameFlag_Set(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
}

/// As Reveal fades in the sand rooms the sand's edge is trimmed again.
void AnkohlRuins_EndReveal(void) {
  GameFlag_Clear(FLAG_ANKOHL_RUINS_REVEAL_SHOWN);
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    return gAnkohlRuins_StoneFacesTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
    return gAnkohlRuins_ThanatosMaceChestTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE)
    return gAnkohlRuins_GreatFaceTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    return gAnkohlRuins_FaceHalvesTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    return gAnkohlRuins_CrystalPowderChestTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_SAND_DOORS)
    return gAnkohlRuins_SandDoorsTriggers;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
    return gAnkohlRuins_LeftProngTriggers;
  return gAnkohlRuins_Triggers;
}

#include "shared/prong_sparkle.h"

void AnkohlRuins_RiseFromSand(void);
void AnkohlRuins_CarriedOffLedge(void);
void AnkohlRuins_PressFirstSandSwitch(void);
void AnkohlRuins_OpenFirstSandDoor(void);
void AnkohlRuins_PressSecondSandSwitch(void);
void AnkohlRuins_OpenSecondSandDoor(void);
void AnkohlRuins_StartGreatFaceSliding(void);
void AnkohlRuins_StartGreatFaceSlidingUpClose(void);
void AnkohlRuins_PushStatueIntoGap(void);
void AnkohlRuins_RevealGap(void);
void AnkohlRuins_StartReveal(void);
void AnkohlRuins_EndReveal(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAnkohlRuins_WeedsScript[];
extern u32 gAnkohlRuins_FaceHalfScript[];
extern u32 gAnkohlRuins_GreatFaceScript[];

struct AnkohlRuinsSpriteWork;

#if TLA_PATCH_1

#include "types.h"
#include "field/call.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "text/msg_ids.h"

void AnkohlRuins_FlowWithSand(void);
void FieldEffect_StartBlankOverlay();
void AnkohlRuins_LandFromFall(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->next;
  struct Actor *actor;

  if (scene == SCENE_ANKOHL_RUINS_STONE_FACES) {
    FieldEffect_StartBlankOverlay(0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A)))
      Actor_SetPosition(ACTOR_STONE_FACE_A, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B)))
      Actor_SetPosition(ACTOR_STONE_FACE_B, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C)))
      Actor_SetPosition(ACTOR_STONE_FACE_C, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D)))
      Actor_SetPosition(ACTOR_STONE_FACE_D, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E)))
      Actor_SetPosition(ACTOR_STONE_FACE_E, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F)))
      Actor_SetPosition(ACTOR_STONE_FACE_F, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G)))
      Actor_SetPosition(ACTOR_STONE_FACE_G, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_A)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_A, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_B)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_B, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_C)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_C, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_D)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_D, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_E)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_E, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_F)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_F, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_G)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_G, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_H)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_H, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_I)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_I, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_J)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_J, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_K)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_K, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_L)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_L, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_M)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_M, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_N)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_N, 0, 0);
    if (GameFlag_Get(FLAG_NUT_CHEST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_NUT_CHEST_PILLAR, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST) {
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_A)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_A, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_B)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_B, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_C)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_C, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_FACE_HALF_PLACED)) {
      s32 width;

      actor = Actor_GetById(ACTOR_THANATOS_FACE_HALF);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_THANATOS_FACE_HALF, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance ==
              ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_EAST ||
          work->entrance == THANATOS_FACE_HALL_FROM_WEST_STAIRS ||
          work->entrance == THANATOS_FACE_HALL_FROM_EAST_STAIRS)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (GameFlag_Get(FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_GREAT_FACE_ROOMS_PILLAR, 520, 104);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(FLAG_GREAT_FACE_HALF_PLACED)) {
      Actor_SetPosition(ACTOR_GREAT_FACE_HALF, 0, 0);
      Actor_SetAnimation(ACTOR_GREAT_FACE, 4);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_A)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_A, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_B)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_B, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_C)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_C, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_D)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_D, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_E)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_E, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_F)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_F, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_G)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_G, 0, 0);
    if (GameFlag_Get(FLAG_FACE_HALVES_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FACE_HALVES_PILLAR, 40, 776);
    if (GameFlag_Get(FLAG_FACE_HALVES_SECOND_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FACE_HALVES_SECOND_PILLAR, 632, 456);
    if (GameFlag_Get(FLAG_FACE_HALVES_THIRD_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FACE_HALVES_THIRD_PILLAR, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      actor = Actor_GetById(ACTOR_LEFT_FACE_HALF);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_LEFT_FACE_HALF, 200, 728);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED)) {
      actor = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 232, 728);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    } else if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP) &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED) &&
        GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - FACE_HALVES_HALL_FIRST_ENTRANCE) <=
          4)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance ==
            ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTHWEST) {
      AnkohlRuins_LandFromFall();
#if TLA_PATCH_4
      Event_ClearFlag(FLAG_ARRIVE_FALLING);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance ==
          ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->leaderActorId, 0, 0);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        actor = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
        actor->y.fixed += toFIXED(128);
        frames = 60;
        actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (actor->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(
            ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - FACE_HALVES_HALL_FIRST_ENTRANCE) <= 3)
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_FRAME);
  } else if (scene == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP))
      Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    if (GameFlag_Get(FLAG_CRYSTAL_POWDER_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_CRYSTAL_POWDER_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 ||
        work->entrance ==
            ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_LEFT_PRONG) {
      Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_FRAME);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_SAND_DOORS) {
    if (work->entrance <= 4)
      AnkohlRuins_InitSand(a0);
    if (GameFlag_Get(FLAG_FIRST_SAND_DOOR_OPENED)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(FLAG_SECOND_SAND_DOOR_OPENED)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        GameFlag_Get(FLAG_SCENE_TEMP_04)) {
      FIXED level = toFIXED(32);

      gAnkohlRuins_Sand.level = level;
      if ((u16)(work->entrance - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(work->entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      actor = Actor_GetById(work->leaderActorId);
      actor->groundHeight = level;
      actor->y.fixed = level;
      FieldEvent_Wait(1);
      MapGrid_CenterOnFollowedPoint();
    }
  } else if (scene == SCENE_ANKOHL_RUINS_LEFT_PRONG) {
    actor = Actor_GetById(ACTOR_LEFT_PRONG_CHEST);
    if (actor != NULL) {
      actor->motionFlags = 0;
      actor->groundHeight = toFIXED(72);
      actor->y.fixed = toFIXED(72);
      actor->update = (ObjectUpdate)Prong_Sparkle;
    }
  }
  return 0;
}

#else

// The Japanese release's older form.

#include "types.h"
#include "field/call.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "text/msg_ids.h"

void AnkohlRuins_FlowWithSand(void);
void FieldEffect_StartBlankOverlay();

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->next;
  struct Actor *actor;

  if (scene == SCENE_ANKOHL_RUINS_STONE_FACES) {
    FieldEffect_StartBlankOverlay(0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A)))
      Actor_SetPosition(ACTOR_STONE_FACE_A, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B)))
      Actor_SetPosition(ACTOR_STONE_FACE_B, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C)))
      Actor_SetPosition(ACTOR_STONE_FACE_C, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D)))
      Actor_SetPosition(ACTOR_STONE_FACE_D, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E)))
      Actor_SetPosition(ACTOR_STONE_FACE_E, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F)))
      Actor_SetPosition(ACTOR_STONE_FACE_F, 0, 0);
    if (GameFlag_Get(FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G)))
      Actor_SetPosition(ACTOR_STONE_FACE_G, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_A)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_A, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_B)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_B, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_C)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_C, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_D)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_D, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_E)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_E, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_F)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_F, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_G)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_G, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_H)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_H, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_I)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_I, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_J)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_J, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_K)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_K, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_L)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_L, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_M)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_M, 0, 0);
    if (GameFlag_Get(
            FLAG_STONE_FACES_WEEDS_SCATTERED(ACTOR_STONE_FACES_WEEDS_N)))
      Actor_SetPosition(ACTOR_STONE_FACES_WEEDS_N, 0, 0);
    if (GameFlag_Get(FLAG_NUT_CHEST_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_NUT_CHEST_PILLAR, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST) {
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_A)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_A, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_B)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_B, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_WEEDS_SCATTERED(ACTOR_THANATOS_WEEDS_C)))
      Actor_SetPosition(ACTOR_THANATOS_WEEDS_C, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_FACE_HALF_PLACED)) {
      s32 width;

      actor = Actor_GetById(ACTOR_THANATOS_FACE_HALF);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_THANATOS_FACE_HALF, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance ==
              ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_EAST ||
          work->entrance == THANATOS_FACE_HALL_FROM_WEST_STAIRS ||
          work->entrance == THANATOS_FACE_HALL_FROM_EAST_STAIRS)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (GameFlag_Get(FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_GREAT_FACE_ROOMS_PILLAR, 520, 104);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(FLAG_GREAT_FACE_HALF_PLACED)) {
      Actor_SetPosition(ACTOR_GREAT_FACE_HALF, 0, 0);
      Actor_SetAnimation(ACTOR_GREAT_FACE, 4);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_A)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_A, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_B)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_B, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_C)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_C, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_D)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_D, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_E)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_E, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_F)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_F, 0, 0);
    if (GameFlag_Get(
            FLAG_FACE_HALVES_WEEDS_SCATTERED(ACTOR_FACE_HALVES_WEEDS_G)))
      Actor_SetPosition(ACTOR_FACE_HALVES_WEEDS_G, 0, 0);
    if (GameFlag_Get(FLAG_FACE_HALVES_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FACE_HALVES_PILLAR, 40, 776);
    if (GameFlag_Get(FLAG_FACE_HALVES_SECOND_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FACE_HALVES_SECOND_PILLAR, 632, 456);
    if (GameFlag_Get(FLAG_FACE_HALVES_THIRD_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FACE_HALVES_THIRD_PILLAR, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      actor = Actor_GetById(ACTOR_LEFT_FACE_HALF);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_LEFT_FACE_HALF, 200, 728);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED)) {
      actor = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 232, 728);
      actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    } else if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP) &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED) &&
        GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - FACE_HALVES_HALL_FIRST_ENTRANCE) <=
          3)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance ==
          ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->leaderActorId, 0, 0);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        actor = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
        actor->y.fixed += toFIXED(128);
        frames = 60;
        actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (actor->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(
            ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - FACE_HALVES_HALL_FIRST_ENTRANCE) <= 3)
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_FRAME);
  } else if (scene == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP))
      Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    if (GameFlag_Get(FLAG_CRYSTAL_POWDER_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_CRYSTAL_POWDER_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 ||
        work->entrance ==
            ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_LEFT_PRONG) {
      Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_FRAME);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_SAND_DOORS) {
    if (work->entrance <= 4)
      AnkohlRuins_InitSand(a0);
    if (GameFlag_Get(FLAG_FIRST_SAND_DOOR_OPENED)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(FLAG_SECOND_SAND_DOOR_OPENED)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        GameFlag_Get(FLAG_SCENE_TEMP_04)) {
      FIXED level = toFIXED(32);

      gAnkohlRuins_Sand.level = level;
      if ((u16)(work->entrance - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(work->entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      actor = Actor_GetById(work->leaderActorId);
      actor->groundHeight = level;
      actor->y.fixed = level;
      FieldEvent_Wait(1);
      MapGrid_CenterOnFollowedPoint();
    }
  } else if (scene == SCENE_ANKOHL_RUINS_LEFT_PRONG) {
    actor = Actor_GetById(ACTOR_LEFT_PRONG_CHEST);
    if (actor != NULL) {
      actor->motionFlags = 0;
      actor->groundHeight = toFIXED(72);
      actor->y.fixed = toFIXED(72);
      actor->update = (ObjectUpdate)Prong_Sparkle;
    }
  }
  return 0;
}

#endif

s32 Scene_PrepareMap(void) { return 0; }

void AnkohlRuins_JumpDownLedge(void);
void AnkohlRuins_SinkIntoSand(void);
void AnkohlRuins_RaiseFirstSand(void);
void AnkohlRuins_RaiseSecondSand(void);
void AnkohlRuins_HideGap(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

/// The first sand room's cell blocks, each with the sand level that covers it,
/// ended by level 99.
struct AnkohlRuinsSandRow gAnkohlRuins_FirstSandRows[] = {
    {0, 15, 4, 1, 8},  {0, 3, 3, 4, 5},   {0, 6, 2, 7, 6},
    {0, 1, 1, 10, 5},  {0, 1, 1, 12, 5},  {2, 1, 1, 11, 5},
    {2, 15, 1, 1, 12}, {4, 15, 1, 1, 13}, {99},
};

/// The second sand room's cell blocks, each with the sand level that covers it,
/// ended by level 99.
struct AnkohlRuinsSandRow gAnkohlRuins_SecondSandRows[] = {
    {0, 15, 2, 1, 28}, {0, 12, 2, 4, 26}, {0, 8, 2, 4, 24},
    {0, 2, 1, 5, 23},  {0, 2, 1, 10, 23}, {0, 1, 1, 15, 25},
    {2, 1, 1, 4, 23},  {2, 15, 1, 1, 30}, {4, 15, 1, 1, 31},
    {99},
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 136, 0, 120, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

/// The Crystal Powder's rooms' way out onto the top of the ruins.
struct SceneRegion gAnkohlRuins_CrystalPowderChestExitRegions[] = {
    {819, 238, 852, 827, 246, 860, CONDITION_ALWAYS,
     ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_LEFT_PRONG},
    {SCENE_TABLE_END},
};

/// The way back down from the top of the ruins to the Crystal Powder's rooms.
struct SceneRegion gAnkohlRuins_LeftProngExitRegions[] = {
    {180, -26, 164, 188, -18, 172, CONDITION_ALWAYS,
     ANKOHL_RUINS_LEFT_PRONG_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AnkohlRuinsEntrance),
    SCENE_EXIT(ANKOHL_RUINS_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ANKOHL_RUINS_ENTRANCE),
    SCENE_EXIT(ANKOHL_RUINS_ENTRANCE_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_ENTRANCE),
    SCENE_EXITS(AnkohlRuinsStoneFaces),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_ENTRANCE, AnkohlRuinsEntrance, ANKOHL_RUINS_ENTRANCE_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(2, AnkohlRuinsStoneFaces, 11),
    SCENE_EXIT(3, AnkohlRuinsStoneFaces, 5),
    SCENE_EXIT(4, AnkohlRuinsStoneFaces, 6),
    SCENE_EXIT(5, AnkohlRuinsStoneFaces, 3),
    SCENE_EXIT(6, AnkohlRuinsStoneFaces, 4),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_SOUTHWEST),
    SCENE_EXIT(8, AnkohlRuinsStoneFaces, 12),
    SCENE_EXIT(9, AnkohlRuinsStoneFaces, 10),
    SCENE_EXIT(10, AnkohlRuinsStoneFaces, 9),
    SCENE_EXIT(11, AnkohlRuinsStoneFaces, 2),
    SCENE_EXIT(12, AnkohlRuinsStoneFaces, 8),
    SCENE_EXIT(13, AnkohlRuinsStoneFaces, 20),
    SCENE_EXIT(14, AnkohlRuinsStoneFaces, 21),
    SCENE_EXIT(15, AnkohlRuinsStoneFaces, 22),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(17, AnkohlRuinsStoneFaces, 24),
    SCENE_EXIT(18, AnkohlRuinsStoneFaces, 25),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_STONE_FACES),
    SCENE_EXIT(20, AnkohlRuinsStoneFaces, 13),
    SCENE_EXIT(21, AnkohlRuinsStoneFaces, 14),
    SCENE_EXIT(22, AnkohlRuinsStoneFaces, 15),
    SCENE_EXIT(24, AnkohlRuinsStoneFaces, 17),
    SCENE_EXIT(25, AnkohlRuinsStoneFaces, 18),
    SCENE_EXIT(27, AnkohlRuinsStoneFaces, 30),
    SCENE_EXIT(29, AnkohlRuinsStoneFaces, 31),
    SCENE_EXIT(30, AnkohlRuinsStoneFaces, 27),
    SCENE_EXIT(31, AnkohlRuinsStoneFaces, 29),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_EAST),
    SCENE_EXIT(ANKOHL_RUINS_STONE_FACES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_STONE_FACES_WEST),
    SCENE_EXITS(AnkohlRuinsThanatosMaceChest),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_SOUTHWEST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_WEST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_STONE_FACES_EAST, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_EAST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_SOUTHWEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_GREAT_FACE_WEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(11, AnkohlRuinsThanatosMaceChest, 12),
    SCENE_EXIT(12, AnkohlRuinsThanatosMaceChest, 11),
    SCENE_EXIT(14, AnkohlRuinsThanatosMaceChest, 15),
    SCENE_EXIT(15, AnkohlRuinsThanatosMaceChest, 14),
    SCENE_EXIT(16, AnkohlRuinsThanatosMaceChest, 17),
    SCENE_EXIT(17, AnkohlRuinsThanatosMaceChest, 16),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_WEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST),
    SCENE_EXIT(ANKOHL_RUINS_THANATOS_MACE_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_EAST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST),
    SCENE_EXITS(AnkohlRuinsGreatFace),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_NORTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_WEST),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_SOUTHWEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_SOUTHWEST),
    SCENE_EXIT(5, AnkohlRuinsGreatFace, 8),
    SCENE_EXIT(6, AnkohlRuinsGreatFace, 10),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_GREAT_FACE),
    SCENE_EXIT(8, AnkohlRuinsGreatFace, 5),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_GREAT_FACE_EAST),
    SCENE_EXIT(10, AnkohlRuinsGreatFace, 6),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_WEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_WEST),
    SCENE_EXIT(ANKOHL_RUINS_GREAT_FACE_TO_ANKOHL_RUINS_FACE_HALVES_EAST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_GREAT_FACE_EAST),
    SCENE_EXITS(AnkohlRuinsFaceHalves),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_NORTH, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_A, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH_A),
    SCENE_EXIT(3, AnkohlRuinsFaceHalves, 14),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_B, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH_B),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_WEST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_WEST),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_EAST, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_EAST),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_NORTH, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_NORTH),
    SCENE_EXIT(8, AnkohlRuinsFaceHalves, 11),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_GREAT_FACE_SOUTH, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_EAST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_EAST),
    SCENE_EXIT(11, AnkohlRuinsFaceHalves, 8),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_SAND_DOORS_SOUTH, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST_WEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_WEST),
    SCENE_EXIT(14, AnkohlRuinsFaceHalves, 3),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_A, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_A),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_B),
    SCENE_EXIT(ANKOHL_RUINS_FACE_HALVES_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_FACE_HALVES_SOUTH_A),
    SCENE_EXITS(AnkohlRuinsCrystalPowderChest),
    SCENE_EXIT(1, AnkohlRuinsCrystalPowderChest, 2),
    SCENE_EXIT(2, AnkohlRuinsCrystalPowderChest, 1),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_B, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_B),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_A, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_A),
    SCENE_EXIT(5, AnkohlRuinsCrystalPowderChest, 6),
    SCENE_EXIT(6, AnkohlRuinsCrystalPowderChest, 5),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_B, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_B),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_NORTH_A, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_NORTH_A),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_SAND_DOORS, AnkohlRuinsSandDoors, ANKOHL_RUINS_SAND_DOORS_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(10, AnkohlRuinsCrystalPowderChest, 11),
    SCENE_EXIT(11, AnkohlRuinsCrystalPowderChest, 10),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_LEFT_PRONG, AnkohlRuinsLeftProng, ANKOHL_RUINS_LEFT_PRONG_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTHWEST, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTHWEST),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH_C, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_SOUTH_C),
    SCENE_EXIT(ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST),
    SCENE_EXITS(AnkohlRuinsSandDoors),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_GREAT_FACE, AnkohlRuinsGreatFace, ANKOHL_RUINS_GREAT_FACE_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_SOUTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_SAND_DOORS_SOUTH),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_FACE_HALVES_NORTH, AnkohlRuinsFaceHalves, ANKOHL_RUINS_FACE_HALVES_FROM_ANKOHL_RUINS_SAND_DOORS_NORTH),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_STONE_FACES, AnkohlRuinsStoneFaces, ANKOHL_RUINS_STONE_FACES_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXIT(ANKOHL_RUINS_SAND_DOORS_TO_ANKOHL_RUINS_THANATOS_MACE_CHEST, AnkohlRuinsThanatosMaceChest, ANKOHL_RUINS_THANATOS_MACE_CHEST_FROM_ANKOHL_RUINS_SAND_DOORS),
    SCENE_EXITS(AnkohlRuinsLeftProng),
    SCENE_EXIT(ANKOHL_RUINS_LEFT_PRONG_TO_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST, AnkohlRuinsCrystalPowderChest, ANKOHL_RUINS_CRYSTAL_POWDER_CHEST_FROM_ANKOHL_RUINS_LEFT_PRONG),
    SCENE_EXITS_END,
};

/// The weeds' script: each clump stands half as tall again, and is set up by
/// AnkohlRuins_SetUpWeeds. Work value 11 sets the Y scale as SW_SCALE_Y does
/// (gActorScript_OperandHandlerTable gives both one handler), and a probe of
/// the halls of the stone faces (render probe --scene AnkohlRuinsStoneFaces)
/// shows their
/// fourteen clumps at a Y scale of 1.5 and an X scale of 1.
s32 gAnkohlRuins_WeedsScript[] = {
    SC_SET(SW_SCALE_Y_DUPLICATE, FIXED_ONE * 3 / 2),
    SC_CALL(AnkohlRuins_SetUpWeeds),
    SC_END,
};

/// The face halves' script: each stands still until pushed.
u32 gAnkohlRuins_FaceHalfScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// The great face's script: it does not collide.
u32 gAnkohlRuins_GreatFaceScript[] = {
    SC_SET(SW_COLLISION, 0),
    SC_END,
};

/// The forest at the ruins' doorway: no actors.
struct ActorPlacement gAnkohlRuins_Placements[] = {
    {SCENE_TABLE_END},
};

/// The halls of the stone faces' actors, in AnkohlRuinsStoneFacesActorId's
/// order.
struct ActorPlacement gAnkohlRuins_StoneFacesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 456, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 696, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 568, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 600, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 648, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 584, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 168, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 264, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 296, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 184, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 840, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 936, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 856, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Thanatos Mace's rooms' actors, in its enum's order.
struct ActorPlacement gAnkohlRuins_ThanatosMaceChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (s32)gAnkohlRuins_FaceHalfScript, 120, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 472, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 520, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 600, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The great face's rooms' actors, in AnkohlRuinsGreatFaceActorId's order.
struct ActorPlacement gAnkohlRuins_GreatFacePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE, CONDITION_ALWAYS, (s32)gAnkohlRuins_GreatFaceScript, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_FaceHalfScript, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The face halves' rooms' actors, in AnkohlRuinsFaceHalvesActorId's order.
struct ActorPlacement gAnkohlRuins_FaceHalvesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (s32)gAnkohlRuins_FaceHalfScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_FaceHalfScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 504, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 712, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 792, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 648, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 728, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, (s32)gAnkohlRuins_WeedsScript, 808, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Crystal Powder's rooms' actors, in
/// AnkohlRuinsCrystalPowderChestActorId's order.
struct ActorPlacement gAnkohlRuins_CrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_FaceHalfScript, 440, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The sand rooms' actors: none.
struct ActorPlacement gAnkohlRuins_SandDoorsPlacements[] = {
    {SCENE_TABLE_END},
};

/// The top of the ruins' actors: none.
struct ActorPlacement gAnkohlRuins_LeftProngPlacements[] = {
    {SCENE_TABLE_END},
};

/// Frames between the sand's moves as it rises.
s16 gAnkohlRuins_SandStepDelay = 10;

/// Frames since the sand last rose.
s16 gAnkohlRuins_SandTimer = 0;

/// Frames between the sand's rises.
s16 gAnkohlRuins_SandRiseDelay = 0;

/// How far the sand rises each move, in 16.16 pixels.
s32 gAnkohlRuins_SandStep = toFIXED(1) / 2;

/// Which sand room's switch was pressed: 1 or 2.
s32 gAnkohlRuins_SandRoom = 0;

/// The forest at the ruins' doorway: its two exits.
struct Trigger gAnkohlRuins_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

/// The halls of the stone faces' exits and stairs, Sand, Move on the stone
/// faces and the pillar, and Whirlwind on the weeds.
struct Trigger gAnkohlRuins_StoneFacesTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 29, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 31},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 32, CONDITION_ALWAYS, 32},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 33, CONDITION_ALWAYS, 33},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NUT_CHEST_PILLAR, FLAG_NUT_CHEST_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_NUT_CHEST_PILLAR, FLAG_NUT_CHEST_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_A, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_B, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_C, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_D, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_E, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_F, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_G, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G), EVENT(AnkohlRuins_PushStoneFaceOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_D, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_E, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_F, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_G, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_H, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_I, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_J, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_K, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_L, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_M, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_STONE_FACES_WEEDS_N, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The Thanatos Mace's rooms' exits and stairs, Sand, Move on the face half and
/// Whirlwind on the weeds.
struct Trigger gAnkohlRuins_ThanatosMaceChestTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 19, CONDITION_ALWAYS, 19},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceThanatosFaceHalf)},
    {TRIGGER_PUSHED, ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceThanatosFaceHalf)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_WEEDS_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The great face's rooms' exits and stairs, Sand, Move on the pillar and the
/// great face's right half, and the steps that set the face sliding.
struct Trigger gAnkohlRuins_GreatFaceTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, CELL_BEHAVIOR_GREAT_FACE_CORRIDOR_FOOT, CONDITION_ALWAYS, EVENT(AnkohlRuins_StartGreatFaceSliding)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_GREAT_FACE, CONDITION_ALWAYS, EVENT(AnkohlRuins_StartGreatFaceSlidingUpClose)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_GREAT_FACE_ROOMS_PILLAR, FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_FACE_ROOMS_PILLAR, FLAG_GREAT_FACE_ROOMS_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceGreatFaceHalf)},
    {TRIGGER_PUSHED, ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceGreatFaceHalf)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The face halves' rooms' exits and stairs, Sand, Move on the face halves and
/// the pillars, and Whirlwind on the weeds.
struct Trigger gAnkohlRuins_FaceHalvesTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_D, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_E, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_F, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FACE_HALVES_WEEDS_G, CONDITION_ALWAYS, EVENT(AnkohlRuins_ScatterWeeds)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_FinishPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_FACE_HALF, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlaceFaceHalves)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_FACE_HALF, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlaceFaceHalves)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_PILLAR, FLAG_FACE_HALVES_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_PILLAR, FLAG_FACE_HALVES_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_SECOND_PILLAR, FLAG_FACE_HALVES_SECOND_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_SECOND_PILLAR, FLAG_FACE_HALVES_SECOND_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALVES_THIRD_PILLAR, FLAG_FACE_HALVES_THIRD_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALVES_THIRD_PILLAR, FLAG_FACE_HALVES_THIRD_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The Crystal Powder's rooms' exits and stairs, the pit, the flowing sand's
/// drop, Sand, Move on the pillar and the half face, and Reveal on the gap.
struct Trigger gAnkohlRuins_CrystalPowderChestTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_PIT_TO_FACE_HALVES, CONDITION_ALWAYS, EVENT(AnkohlRuins_JumpDownLedge)},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_STEP, TRIGGER_CARRIED_OFF_LEDGE, CONDITION_ALWAYS, EVENT(AnkohlRuins_CarriedOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_CRYSTAL_POWDER_PILLAR, FLAG_CRYSTAL_POWDER_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRYSTAL_POWDER_PILLAR, FLAG_CRYSTAL_POWDER_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, EVENT(AnkohlRuins_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, EVENT(AnkohlRuins_PushStatueIntoGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RevealGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_HideGap)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The sand rooms' exits and stairs, Sand, Reveal, the switches, the door
/// buttons and the rising sand.
struct Trigger gAnkohlRuins_SandDoorsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_STEP, CELL_BEHAVIOR_FIRST_SAND_SWITCH, FLAG_FIRST_SAND_SWITCH_PRESSED, EVENT(AnkohlRuins_PressFirstSandSwitch)},
    {TRIGGER_STEP, CELL_BEHAVIOR_FIRST_SAND_DOOR_BUTTON, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenFirstSandDoor)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SECOND_SAND_SWITCH, FLAG_SECOND_SAND_SWITCH_PRESSED, EVENT(AnkohlRuins_PressSecondSandSwitch)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SECOND_SAND_DOOR_BUTTON, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenSecondSandDoor)},
    {TRIGGER_RAISED, TRIGGER_RAISE_FIRST_SAND, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseFirstSand)},
    {TRIGGER_RAISED, TRIGGER_RAISE_SECOND_SAND, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseSecondSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_EndReveal)},
    {TRIGGER_TABLE_END},
};

/// The top of the ruins' triggers: none.
struct Trigger gAnkohlRuins_LeftProngTriggers[] = {
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_ANKOHL_RUINS_OPEN_ONCE
