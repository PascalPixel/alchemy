/// @file
/// The Ankohl Ruins, whose old ruins hold the Left Prong behind the Sand
/// puzzles, in seven rooms that share this overlay
/// (SceneId_AnkohlRuinsStoneFaces to SceneId_AnkohlRuinsLeftProng).
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

/// The seven stone faces of the first room Move pushes off the ledge, one flag
/// a face; the macro takes the face's actor.
#define FLAG_STONE_FACE_A_MOVED 0x9d2
#define FLAG_STONE_FACE_MOVED(face)                                            \
  (FLAG_STONE_FACE_A_MOVED - ACTOR_STONE_FACE_A + (face))
/// The ruins' overgrowth Whirlwind clears, one flag a tuft: the first room's
/// fourteen from FLAG_FIRST_ROOM_IVY_A_CLEARED, the Thanatos Mace room's three,
/// then the fourth room's seven; the macros take the tuft's actor.
#define FLAG_FIRST_ROOM_IVY_A_CLEARED 0x990
#define FLAG_THANATOS_IVY_A_CLEARED 0x99e
#define FLAG_FOURTH_ROOM_IVY_A_CLEARED 0x9a1
#define FLAG_FIRST_ROOM_IVY_CLEARED(ivy)                                       \
  (FLAG_FIRST_ROOM_IVY_A_CLEARED - ACTOR_FIRST_ROOM_IVY_A + (ivy))
#define FLAG_THANATOS_IVY_CLEARED(ivy)                                         \
  (FLAG_THANATOS_IVY_A_CLEARED - ACTOR_THANATOS_IVY_A + (ivy))
#define FLAG_FOURTH_ROOM_IVY_CLEARED(ivy)                                      \
  (FLAG_FOURTH_ROOM_IVY_A_CLEARED - ACTOR_FOURTH_ROOM_IVY_A + (ivy))
/// The pillars Move has pushed into place: the first room's, the third's, and
/// the fourth room's three.
#define FLAG_FIRST_ROOM_PILLAR_MOVED 0x9c6
#define FLAG_THIRD_ROOM_PILLAR_MOVED 0x9c7
#define FLAG_FOURTH_ROOM_PILLAR_MOVED 0x9c8
#define FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED 0x9c9
#define FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED 0x9ca
#define FLAG_FIFTH_ROOM_PILLAR_MOVED 0x9cb
/// The halves of the stone face pushed into place in the fourth room, the
/// fifth room's half pushed into its gap, the Thanatos Mace room's half, and
/// the third room's.
#define FLAG_LEFT_FACE_HALF_PLACED 0x9d9
#define FLAG_RIGHT_FACE_HALF_PLACED 0x9da
#define FLAG_FACE_HALF_PUSHED_INTO_GAP 0x9db
#define FLAG_THANATOS_FACE_HALF_PLACED 0x9dc
#define FLAG_GREAT_FACE_HALF_PLACED 0x9dd
/// The sand doors opened by their switches in the sand rooms, the first and the
/// second.
#define FLAG_FIRST_SAND_DOOR_OPENED 0x9e2
#define FLAG_SECOND_SAND_DOOR_OPENED 0x9e3
/// Set while the raft passes rows 31 to 36, the cells under its path
/// shifted.
#define FLAG_RAFT_CELLS_SHIFTED 0x146
/// The sand rooms' switches pressed, the sand rising in each.
#define FLAG_FIRST_SAND_SWITCH_PRESSED FLAG_SCENE_TEMP_02
#define FLAG_SECOND_SAND_SWITCH_PRESSED FLAG_SCENE_TEMP_03

/// The first room (gAnkohlRuins_StoneFacesPlacements).
enum AnkohlRuinsFirstRoomActorId {
  ACTOR_FIRST_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The seven stone faces.
  ///@{
  ACTOR_STONE_FACE_A = ACTOR_FIRST_PLACED + 2,
  ACTOR_STONE_FACE_B,
  ACTOR_STONE_FACE_C,
  ACTOR_STONE_FACE_D,
  ACTOR_STONE_FACE_E,
  ACTOR_STONE_FACE_F,
  ACTOR_STONE_FACE_G,
  ///@}
  /// The fourteen tufts of overgrowth.
  ///@{
  ACTOR_FIRST_ROOM_IVY_A,
  ACTOR_FIRST_ROOM_IVY_B,
  ACTOR_FIRST_ROOM_IVY_C,
  ACTOR_FIRST_ROOM_IVY_D,
  ACTOR_FIRST_ROOM_IVY_E,
  ACTOR_FIRST_ROOM_IVY_F,
  ACTOR_FIRST_ROOM_IVY_G,
  ACTOR_FIRST_ROOM_IVY_H,
  ACTOR_FIRST_ROOM_IVY_I,
  ACTOR_FIRST_ROOM_IVY_J,
  ACTOR_FIRST_ROOM_IVY_K,
  ACTOR_FIRST_ROOM_IVY_L,
  ACTOR_FIRST_ROOM_IVY_M,
  ACTOR_FIRST_ROOM_IVY_N,
  ///@}
};

/// The Thanatos Mace chest's room (gAnkohlRuins_ThanatosMaceChestPlacements).
enum AnkohlRuinsThanatosMaceChestActorId {
  /// The left half of a stone face, pushed into place.
  ACTOR_THANATOS_FACE_HALF = ACTOR_FIRST_PLACED,
  /// Its three tufts of overgrowth.
  ///@{
  ACTOR_THANATOS_IVY_A,
  ACTOR_THANATOS_IVY_B,
  ACTOR_THANATOS_IVY_C
  ///@}
};

/// The third room (gAnkohlRuins_GreatFacePlacements).
enum AnkohlRuinsThirdRoomActorId {
  ACTOR_THIRD_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The great stone face the party rides down the sand.
  ACTOR_GREAT_FACE,
  /// The half face Move pushes beside it.
  ACTOR_GREAT_FACE_HALF
};

/// The fourth room (gAnkohlRuins_FaceHalvesPlacements).
enum AnkohlRuinsFourthRoomActorId {
  ACTOR_FOURTH_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The two halves of a stone face.
  ///@{
  ACTOR_LEFT_FACE_HALF,
  ACTOR_RIGHT_FACE_HALF,
  ///@}
  /// Its seven tufts of overgrowth.
  ///@{
  ACTOR_FOURTH_ROOM_IVY_A,
  ACTOR_FOURTH_ROOM_IVY_B,
  ACTOR_FOURTH_ROOM_IVY_C,
  ACTOR_FOURTH_ROOM_IVY_D,
  ACTOR_FOURTH_ROOM_IVY_E,
  ACTOR_FOURTH_ROOM_IVY_F,
  ACTOR_FOURTH_ROOM_IVY_G,
  ///@}
  ACTOR_FOURTH_ROOM_SECOND_PILLAR,
  ACTOR_FOURTH_ROOM_THIRD_PILLAR
};

/// The fifth room (gAnkohlRuins_CrystalPowderChestPlacements).
enum AnkohlRuinsFifthRoomActorId {
  ACTOR_FIFTH_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The half face pushed into the gap Reveal shows.
  ACTOR_FACE_HALF_FOR_THE_GAP
};

/// The Left Prong's chest, raised out of the sand.
#define ACTOR_LEFT_PRONG_CHEST 64

void AnkohlRuins_SettleObject(s32 id, s32 cue);
void AnkohlRuins_RaiseFirstSand(void);
void AnkohlRuins_RaiseSecondSand(void);
void AnkohlRuins_RideRaftDown(void);

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

/// Creates a hidden, still actor of `type` at a point, untargeted, in colour 15
/// and drawn underfoot.
struct Actor *AnkohlRuins_CreateActor(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = FieldActor_Create(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
    return object;
  }
  return NULL;
}

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect_spawn.c"

struct AnkohlRuinsSpriteWork gAnkohlRuins_Sand BSS_DATA ALIGNED(16);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gAnkohlRuins_SandStepDelay;
extern s16 gAnkohlRuins_SandTimer;
extern s16 gAnkohlRuins_SandTime;
extern s32 gAnkohlRuins_SandStep;
extern s32 gAnkohlRuins_SandRoom;

extern u32 gAnkohlRuins_StoneFacesRegions[];
extern u32 gAnkohlRuins_ThanatosMaceChestRegions[];

const struct ActorPlacement *Scene_GetExitRegions(void) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST)
    return (const struct ActorPlacement *)gAnkohlRuins_StoneFacesRegions;
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_LEFT_PRONG)
    return (const struct ActorPlacement *)gAnkohlRuins_ThanatosMaceChestRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// Jumps actor `id` down a ledge (Actor_JumpDownLedge).
static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

/// The leader jumps down the ledge and falls to the room below, arriving
/// falling.
void AnkohlRuins_JumpDownLedge(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  Actor_Start(gMainRoutineWork.leaderActorId, 6, 0x1000);
  Event_PlaySound(SE_FALL);
  Event_SetFlag(FLAG_ARRIVE_FALLING);
  FieldEvent_SetExitRequest(13);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller AnkohlRuins_LandFromFall exists in the Japanese
// scene.
#if TLA_PATCH_1
#include "../field/field_effect_advance_motion.c"
#endif

#if TLA_PATCH_1

/// From patch 1: the leader drops in from above, lands in a ring of dust and
/// shakes the ground.
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
    angle = i << 12;
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
  } while (i <= 16);
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 49);
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

/// The call gAnkohlRuins_ShrunkenSpriteScript makes: hides the tuft of ivy,
/// sets its sprite's priority to 2 and stands it four pixels lower.
s32 AnkohlRuins_ClearScriptedActorSpriteFlags(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  object->motionFlags = 0;
  object->y.fixed -= toFIXED(4);
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

/// Sand takes the leader into the sand layer.
void AnkohlRuins_SinkIntoSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 2;
}

/// Sand ends: the leader is back on the ground layer.
void AnkohlRuins_RiseFromSand(void) {
  Actor_Get(gMainRoutineWork.leaderActorId)->layer = 0;
}

/// Before Move: opens the cells under the pillar about to move in the rooms
/// that have one (the first, third, fourth and fifth); in the fifth, the half
/// face for the gap also copies the rows the gap opens (from patch 1, only
/// while FLAG_SCENE_TEMP_05 is set).
void AnkohlRuins_StartMovingFace(s32 unused, s32 id) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES) {
    if (id == ACTOR_FIRST_ROOM_PILLAR) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 4, 69);
      MapGrid_CopyCellAttributes(3, 68, 1, 1, 3, 70);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (id == ACTOR_FOURTH_ROOM_THIRD_PILLAR) {
      MapGrid_CopyCellAttributes(35, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 37, 73);
    }
    if (id == ACTOR_FOURTH_ROOM_PILLAR)
      MapGrid_CopyCellAttributes(2, 113, 1, 1, 3, 113);
    if (id == ACTOR_FOURTH_ROOM_SECOND_PILLAR)
      MapGrid_CopyCellAttributes(39, 93, 1, 1, 40, 93);
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (id == ACTOR_THIRD_ROOM_PILLAR) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 32, 70);
      MapGrid_CopyCellAttributes(33, 69, 1, 1, 33, 71);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (id == ACTOR_FIFTH_ROOM_PILLAR) {
      MapGrid_CopyCellAttributes(17, 85, 1, 1, 17, 83);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 18, 82);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 19, 83);
    }
#if !TLA_PATCH_1
    if (id == ACTOR_FACE_HALF_FOR_THE_GAP)
#else
    if (id == ACTOR_FACE_HALF_FOR_THE_GAP && Event_GetFlag(FLAG_SCENE_TEMP_05))
#endif
      MapGrid_CopyCells(35, 98, 3, 5, 25, 98);
  }
}

/// Copies the cells the object with the given id has pushed open in rooms 1,
/// 3, 4 and 5, and sets flags FLAG_FIRST_ROOM_PILLAR_MOVED to
/// FLAG_FIFTH_ROOM_PILLAR_MOVED when it stands at the end of its track.
void AnkohlRuins_FinishMovingFace(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == ACTOR_FIRST_ROOM_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES) {
    if (FIELD_CELL(object->x.fixed) == 4) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
      Event_SetFlag(FLAG_FIRST_ROOM_PILLAR_MOVED);
    }
    MapGrid_CopyCellAttributes(3, 68, 1, 1, 4, 69);
    MapGrid_CopyCellAttributes(3, 69, 1, 1, 3, 70);
  }
  if (id == ACTOR_THIRD_ROOM_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    MapGrid_CopyCellAttributes(31, 70, 1, 1, 32, 70);
    if (FIELD_CELL(object->x.fixed) == 32) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(FLAG_THIRD_ROOM_PILLAR_MOVED);
    }
  }
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES) {
    if (id == ACTOR_FOURTH_ROOM_PILLAR) {
      MapGrid_CopyCellAttributes(3, 114, 1, 1, 3, 113);
      if (FIELD_CELL(object->x.fixed) == 2)
        Event_SetFlag(FLAG_FOURTH_ROOM_PILLAR_MOVED);
    }
    if (id == ACTOR_FOURTH_ROOM_SECOND_PILLAR) {
      MapGrid_CopyCellAttributes(40, 94, 1, 1, 40, 93);
      if (FIELD_CELL(object->x.fixed) == 39)
        Event_SetFlag(FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED);
    }
    if (id == ACTOR_FOURTH_ROOM_THIRD_PILLAR) {
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (FIELD_CELL(object->x.fixed) == 38) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED);
      }
    }
  }
  if (id == ACTOR_FIFTH_ROOM_PILLAR &&
      gMainRoutineWork.next == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (FIELD_CELL(object->x.fixed) == 17) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(FLAG_FIFTH_ROOM_PILLAR_MOVED);
    }
  }
}

/// Move on a stone face to row 31: it falls off the ledge and is gone, its flag
/// set.
void AnkohlRuins_DropPillar(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *object = Actor_Get(id);
  s16 frames = 60;

  if (FIELD_CELL(object->z.fixed) == 31) {
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Actor_SetSpritePriorityDirect(object, 3);
    Event_PlaySound(SE_DUST_PUFF);
    while (frames != 0) {
      System_Wait(1);
      if (object->velocityY == 0)
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

/// Runs the cue, if any, and sets the object's motion flags to 3 while it
/// moves, for at most 60 frames until its vertical speed is 0, then clears
/// them ten frames later; GaiaRock_DropPillar of Gaia Rock is the same
/// function.
void AnkohlRuins_SettleObject(s32 id, s32 cue) {
  s32 frames = 60;
  struct Actor *object = Actor_Get(id);

  object->motionFlags = 3;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  object->motionFlags = 0;
}

/// Places count effects in a row, rows times, each row two pixels further down,
/// waiting between rows.
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
        FieldEffect_Spawn(INTtoFIXED(x + left * 16) + INTtoFIXED(dx - 8),
                          (y + dy) << 16, INTtoFIXED(z), 0, 0, 0x100,
                          EFFECT_USE_START_SCALE | 1, &params);
      }
      FieldEvent_Wait(frames);
      z += 2;
      row++;
    } while (row < rows);
  }
}

/// When the two face halves stand on cells (12, 45) and (14, 45),
/// each sets its flag (FLAG_LEFT_FACE_HALF_PLACED,
/// FLAG_RIGHT_FACE_HALF_PLACED); once both are set, shakes the screen, copies
/// the cells of rows 2 to 6 and 13 in two columns and has AnkohlRuins_PourSand
/// pour sand between them, as AnkohlRuins_PlaceFacePiece does for the
/// Thanatos Mace room's half.
void AnkohlRuins_PlacePillarPair(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = FIELD_CELL(object->x.fixed);
  s32 row = FIELD_CELL(object->z.fixed);
  s32 toColumn;
  s32 lowRow;
  s32 highRow;
  s32 rise;
  s32 kind;

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
  rise = -40;
  AnkohlRuins_PourSand(3, 15, 200, rise, 744, kind = 3);
  MapGrid_CopyCells(16, 27, 3, 3, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 3, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 760, kind);
  MapGrid_CopyCells(16, 27, 3, 4, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 4, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 776, kind);
  MapGrid_CopyCells(16, 27, 3, 5, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 5, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 792, kind);
  MapGrid_CopyCells(16, 27, 3, 6, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 6, toColumn, highRow);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  AnkohlRuins_PourSand(3, 10, 200, rise, 808, kind);
  MapGrid_CopyCells(16, 27, 3, 13, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 13, toColumn, highRow);
  Event_End();
}

/// When the Thanatos Mace room's face half stands on cell (12, 5): sets flag
/// FLAG_THANATOS_FACE_HALF_PLACED, shakes the screen, copies the cells of rows
/// 2 to 6 and 16 in two columns and has AnkohlRuins_PourSand place four objects
/// between them.
void AnkohlRuins_PlaceFacePiece(s32 unused, s32 id) {
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

/// Move on the great face's half to cell (15, 24): it settles, the great face's
/// eyes glow, and the face can be ridden down the sand.
void AnkohlRuins_PushRaftPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  if (id == ACTOR_GREAT_FACE_HALF && x == 15 && z == 24) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_GREAT_FACE_HALF), 0);
    object->y.fixed -= toFIXED(2);
    FieldEvent_Wait(1);
    object->y.fixed -= toFIXED(2);
    FieldEvent_Wait(1);
    object->y.fixed -= toFIXED(4);
    FieldEvent_Wait(1);
    object->y.fixed -= toFIXED(8);
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

/// Each frame on flowing sand: the leader and the camera drift south half a
/// pixel, and leave by trigger 66 when a wall stops them.
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
  if (((ground->attributeB & 4) | (upper->collisionCode & 232)) != 236)
    return;
  if (gMainRoutineWork.movementMode == LEADER_MOVEMENT_SAND)
    return;
  position[0] = leader->x.fixed;
  position[1] = leader->y.fixed;
  position[2] = leader->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(leader, position) < 0)
    work->stepTrigger = 66;
  leader->z.fixed += toFIXED(1) / 2;
  companion->z.fixed += toFIXED(1) / 2;
}

/// The leader, pushed off a ledge, slips forward, is startled and falls.
void AnkohlRuins_PushedOffLedge(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STILL);
  Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  object->motionFlags = 0;
  object->steeringFlags = 0;
  {
    s32 zero;

    zero = 0;
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, zero, 8);
  }
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Event_PlaySound(SE_DUST_PUFF);
  AnkohlRuins_SettleObject(gMainRoutineWork.leaderActorId, SE_THUMP);
  object->steeringFlags = ACTOR_FACES_MOTION;
  Event_End();
}

/// After a push: the fourth room's face halves go to
/// AnkohlRuins_PlacePillarPair, everything else to
/// AnkohlRuins_FinishMovingFace.
void AnkohlRuins_FinishPushing(s32 a, s32 id) {
  if (id == ACTOR_LEFT_FACE_HALF || id == ACTOR_RIGHT_FACE_HALF)
    AnkohlRuins_PlacePillarPair(a, id);
  else
    AnkohlRuins_FinishMovingFace(a, id);
}

/// Whirlwind on a tuft of ivy: its room's flag for that tuft is set.
void AnkohlRuins_WhirlwindOnOvergrowth(s32 a, s32 id) {
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_STONE_FACES)
    Event_SetFlag(FLAG_FIRST_ROOM_IVY_CLEARED(id));
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST)
    Event_SetFlag(FLAG_THANATOS_IVY_CLEARED(id));
  if (gMainRoutineWork.next == SCENE_ANKOHL_RUINS_FACE_HALVES)
    Event_SetFlag(FLAG_FOURTH_ROOM_IVY_CLEARED(id));
}

/// Draws eight sprites while FLAG_SCENE_TEMP_05 is clear and the sand is below
/// 32 pixels: each moves by the level as its kind says, is placed relative to
/// the view and is drawn when it is on screen. Then sets a word in map layer 1
/// to the level plus a base.
void AnkohlRuins_DrawSand(void) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & FIELD_PIXEL_MASK;
  s32 viewZ = view[1] & FIELD_PIXEL_MASK;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s32 *origin = map->origin;
  // Unused: gives the stack frame its 4 bytes; the original stores it there
  // and never reads it.
  volatile s32 kept = origin[1];
  s32 tile = gVramCacheEntries[work->tile].offset >> 5;
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_SCENE_TEMP_05) == 0 && work->level < toFIXED(32)) {
    for (i = 0; i <= 7; i++) {
      u32 kind = sprite->kind;
      FIXED x = sprite->x;
      s32 y = sprite->y;
      s32 z = sprite->z;
      s32 sx, sy;

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
      sy = z - viewZ - y;
      sx = (x >> 16) - 8;
      sy = (sy >> 16) - 8;
      if (sx >= -15 && sx <= 319 && sy >= -15 && sy <= 239) {
        sx &= 511;
        sy &= 255;
        sprite->oam[0] = 0;
        sprite->oam[1] =
            sy | (sx << 16) | OBJ_SIZE_16X16 | OBJ_MODE(OBJ_MODE_WINDOW);
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

/// Sets up the sand rooms' rising sand: its tile in VRAM, the window, the
/// drawing task, and the falling sand's sprites by the entrance.
void AnkohlRuins_InitSand(s32 unused) {
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  u32 *tiles = Heap_Alloc(128);
  u32 *pixel = tiles;
  struct MapLayer *layer = &Ram_HeapSlots->mapGrid->layers[1];
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Iwram_ClearWordsEntry(work, 240);
  i = 31;
  do {
    *pixel++ = 0x11111111;
    i--;
  } while (i >= 0);
  // Spelled through a pointer: a member store shares its zero mask.
  *(s16 *)&work->tile = VramCache_FindFreeEntry();
  Vram_Load(work->tile, 128, tiles);
  Heap_Release(tiles);
  work->baseOffsetX = layer->offsetX;
  work->baseOffsetY = layer->offsetY;
  Io_Write16(0x1b1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  Task_AddFrame(AnkohlRuins_DrawSand, TASK_PASS_VBLANK);
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
  // Spelled through a pointer: a member read is copied up into the arm above.
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

/// Counts the risen sand's time; when it runs out, the room's sand sinks again
/// by trigger 68 or 69.
void AnkohlRuins_CountSandTime(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;

  if (Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS) == 0) {
    if (++gAnkohlRuins_SandTimer >= gAnkohlRuins_SandTime) {
      FieldTask_Remove(AnkohlRuins_CountSandTime);
      if (gAnkohlRuins_SandRoom == 1)
        work->raisedTrigger = 68;
      else
        work->raisedTrigger = 69;
    }
  }
}

extern struct AnkohlRuinsSandRow gAnkohlRuins_FirstSandRows[];

/// The first sand room's sand rises a step in sixteen moves, lifting the leader
/// standing on it, and its cells follow the new level.
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
  m = work->level >> 19;
  leader->motionFlags = 0;
  MapGrid_SetShake(4, 4, 1);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 1);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells +=
        leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
    if (cells->collisionCode == 232) {
      leader->y.fixed += gAnkohlRuins_SandStep;
      leader->groundHeight = leader->y.fixed;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 11, 68);
      MapGrid_CopyCellAttributes(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 11, 68);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(5);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (work->level < toFIXED(32)) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, TASK_PASS_VBLANK);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gAnkohlRuins_FirstSandRows[row].level != 99; row++) {
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

/// The second sand room's sand rises a step in sixteen moves, lifting the
/// leader standing on it, and its cells follow the new level.
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
  level = work->level >> 19;
  leader->motionFlags = 0;
  MapGrid_SetShake(5, 5, 1);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 19);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells +=
        leader->x.fixed / toFIXED(16) + ((leader->z.fixed / toFIXED(16)) << 7);
    if (cells->collisionCode == 232) {
      leader->y.fixed += gAnkohlRuins_SandStep;
      leader->groundHeight = leader->y.fixed;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 4, 86);
      MapGrid_CopyCellAttributes(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 4, 86);
  }
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldEvent_Wait(5);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (work->level < toFIXED(32)) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, TASK_PASS_VBLANK);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  for (row = 0; gAnkohlRuins_SecondSandRows[row].level != 99; row++) {
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

/// The first sand room's switch: the sand rises slowly and sinks after 200
/// frames.
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
    gAnkohlRuins_SandTime = 200;
    gAnkohlRuins_SandStep = toFIXED(1) / 2;
    gAnkohlRuins_SandRoom = 1;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseFirstSand();
    Event_SetFlag(FLAG_FIRST_SAND_SWITCH_PRESSED);
  }
}

/// Defines a sand door's opening: once, under flag n1, a button sounds and the
/// door at (n2, n3) slides open, its cell taking (n4, n5)'s attributes.
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

/// Stepping on the first sand room's door button opens its door.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenFirstSandDoor,
                              FLAG_FIRST_SAND_DOOR_OPENED, 14, 65, 15, 5)

/// The second sand room's switch: the sand rises faster and sinks after 100
/// frames.
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
    gAnkohlRuins_SandTime = 100;
    gAnkohlRuins_SandStep = toFIXED(1) / 2;
    gAnkohlRuins_SandRoom = 2;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseSecondSand();
    Event_SetFlag(FLAG_SECOND_SAND_SWITCH_PRESSED);
  }
}

/// Stepping on the second sand room's door button opens its door.
DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenSecondSandDoor,
                              FLAG_SECOND_SAND_DOOR_OPENED, 2, 83, 1, 23)

/// Each frame, moves the raft (the great face) four fifths of a pixel down the
/// sand and carries the leader and the camera's actor with it while the leader
/// stands on it; shifts the cells under it in rows 31 to 37
/// (FLAG_RAFT_CELLS_SHIFTED), and past row 41 removes itself, stops the
/// shaking, opens the landing and sets FLAG_SCENE_TEMP_01.
void AnkohlRuins_RideRaftDown(void) {
  struct Actor *rider = gFieldWork->camera;
  struct Actor *raft = Actor_GetById(ACTOR_GREAT_FACE);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  s32 column;
  s32 row;

  if (GameFlag_Get(FLAG_FIELD_HANDLING_REQUESTS) && work->stepTrigger != 40)
    return;
  raft->z.fixed += toFIXED(4) / 5;
  column = FIELD_CELL(leader->x.fixed);
  if (gMainRoutineWork.movementMode != LEADER_MOVEMENT_SAND && column > 12 &&
      column <= 15) {
    if (raft->z.fixed + toFIXED(24) >= leader->z.fixed &&
        leader->z.fixed >= raft->z.fixed) {
      leader->z.fixed = raft->z.fixed + toFIXED(24);
      rider->z.fixed = raft->z.fixed + toFIXED(24);
    }
    if (raft->z.fixed >= leader->z.fixed &&
        leader->z.fixed >= raft->z.fixed - toFIXED(16)) {
      leader->z.fixed = raft->z.fixed - toFIXED(16);
      rider->z.fixed = raft->z.fixed - toFIXED(16);
    }
  }
  row = FIELD_CELL(raft->z.fixed);
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(FLAG_RAFT_CELLS_SHIFTED)) {
    MapGrid_CopyCellAttributes(35, 83, 5, 4, 12, 96);
    Event_SetFlag(FLAG_RAFT_CELLS_SHIFTED);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(FLAG_RAFT_CELLS_SHIFTED)) {
    MapGrid_CopyCellAttributes(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(FLAG_RAFT_CELLS_SHIFTED);
  }
  if (row > 41) {
    FieldTask_Remove(AnkohlRuins_RideRaftDown);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_CopyCellAttributes(41, 48, 3, 2, 13, 41);
    Event_SetFlag(FLAG_SCENE_TEMP_01);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
    Event_ClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
    Event_PlaySound(SE_BANE_ESCAPE);
  }
}

/// Stepping onto the great face, once its half is placed: the ground shakes and
/// it starts down the sand with the party.
void AnkohlRuins_BoardRaft(void) {
  s32 z = FIELD_CELL(Actor_Get(ACTOR_GREAT_FACE)->z.fixed);

  if (Event_GetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24) {
    s32 found = Task_Find(AnkohlRuins_RideRaftDown);

    if (found == -1) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(SE_RUSH);
      MapGrid_SetShake(2, 2, 1);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(232, CAMERA_KEEP, 392, 1);
      FieldCamera_WaitForArrival();
      Task_AddFrame(AnkohlRuins_RideRaftDown, TASK_PASS_VBLANK);
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

/// As AnkohlRuins_BoardRaft, without the camera's pan.
void AnkohlRuins_BoardRaftAgain(void) {
  s32 z = FIELD_CELL(Actor_Get(ACTOR_GREAT_FACE)->z.fixed);

  if (Event_GetFlag(FLAG_GREAT_FACE_HALF_PLACED) != 0 && z == 24) {
    void (*func)(void) = AnkohlRuins_RideRaftDown;

    if (Task_Find(func) == -1) {
      MapGrid_SetShake(2, 2, 1);
      Task_AddFrame(func, TASK_PASS_VBLANK);
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

/// Move on the fifth room's half face to cell (26, 35): it drops into the gap
/// and the scene reloads.
void AnkohlRuins_PushStatueIntoGap(void) {
  struct Actor *object = Actor_Get(ACTOR_FACE_HALF_FOR_THE_GAP);

  if (FIELD_CELL(object->x.fixed) == 26 && FIELD_CELL(object->z.fixed) == 35) {
    Event_SetFlag(FLAG_FACE_HALF_PUSHED_INTO_GAP);
    Event_PlaySound(SE_DUST_PUFF);
    AnkohlRuins_SettleObject(ACTOR_FACE_HALF_FOR_THE_GAP, 0);
    Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(20);
  }
#if !TLA_PATCH_1 /* The Japanese edition copies cells here. */
  MapGrid_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

/// Reveal shows the fifth room's gap.
void AnkohlRuins_RevealGap(void) {
#if TLA_PATCH_1 /* The Japanese edition does not touch FLAG_SCENE_TEMP_05. */
  Event_SetFlag(FLAG_SCENE_TEMP_05);
#endif
  MapGrid_CopyCells(35, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(35, 98, 3, 3, 25, 98);
}

/// Reveal ends: the gap is hidden again.
void AnkohlRuins_HideGap(void) {
#if TLA_PATCH_1
  Event_ClearFlag(FLAG_SCENE_TEMP_05);
#endif
  MapGrid_CopyCells(40, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(40, 98, 3, 3, 25, 98);
}

/// Reveal begins: FLAG_SCENE_TEMP_05 hides the sand's sprites.
void AnkohlRuins_StartReveal(void) { GameFlag_Set(FLAG_SCENE_TEMP_05); }

/// Reveal ends: the sand's sprites show again.
void AnkohlRuins_EndReveal(void) { GameFlag_Clear(FLAG_SCENE_TEMP_05); }

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

/// A dust puff every eighth frame near the actor, as the raft scrapes.
void AnkohlRuins_RaiseDust(struct Actor *object) {
  struct EffectOptions params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    params.type = SPRITE_SPARKLE;
    FieldEffect_Spawn(object->x.fixed + INTtoFIXED(dx - 8),
                      object->y.fixed + INTtoFIXED(dy - 8), object->z.fixed, 0,
                      toFIXED(1) / 2, phase, EFFECT_USE_TYPE, &params);
  }
}

void AnkohlRuins_RiseFromSand(void);
void AnkohlRuins_PushedOffLedge(void);
void AnkohlRuins_PressFirstSandSwitch(void);
void AnkohlRuins_OpenFirstSandDoor(void);
void AnkohlRuins_PressSecondSandSwitch(void);
void AnkohlRuins_OpenSecondSandDoor(void);
void AnkohlRuins_BoardRaft(void);
void AnkohlRuins_BoardRaftAgain(void);
void AnkohlRuins_PushStatueIntoGap(void);
void AnkohlRuins_RevealGap(void);
void AnkohlRuins_StartReveal(void);
void AnkohlRuins_EndReveal(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAnkohlRuins_ShrunkenSpriteScript[];
extern u32 gAnkohlRuins_ThanatosMaceChestScript[];
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
  struct Actor *object;

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
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_A)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_B)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_C)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_D)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_D, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_E)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_E, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_F)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_F, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_G)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_G, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_H)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_H, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_I)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_I, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_J)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_J, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_K)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_K, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_L)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_L, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_M)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_M, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_N)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_N, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FIRST_ROOM_PILLAR, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST) {
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_A)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_B)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_C)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_FACE_HALF_PLACED)) {
      s32 width;

      object = Actor_GetById(ACTOR_THANATOS_FACE_HALF);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_THANATOS_FACE_HALF, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (GameFlag_Get(FLAG_THIRD_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_THIRD_ROOM_PILLAR, 520, 104);
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
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_A)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_B)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_C)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_D)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_D, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_E)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_E, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_F)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_F, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_G)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_G, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_PILLAR, 40, 776);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_SECOND_PILLAR, 632, 456);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FOURTH_ROOM_THIRD_PILLAR, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      object = Actor_GetById(ACTOR_LEFT_FACE_HALF);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_LEFT_FACE_HALF, 200, 728);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED)) {
      object = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 232, 728);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    } else if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP) &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED) &&
        GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 17) {
      AnkohlRuins_LandFromFall();
#if TLA_PATCH_4
      Event_ClearFlag(FLAG_ARRIVE_FALLING);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->leaderActorId, 0, 0);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
        object->y.fixed += toFIXED(128);
        frames = 60;
        object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_VBLANK);
  } else if (scene == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP))
      Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    if (GameFlag_Get(FLAG_FIFTH_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FIFTH_ROOM_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_VBLANK);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_SAND_DOORS) {
    s16 *entrance = &work->entrance;

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
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsSpriteWork *)entrance)->unused - 1) <= 1) {
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
      if ((u16)(*entrance - 3) <= 1) {
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
      object = Actor_GetById(work->leaderActorId);
      object->groundHeight = level;
      object->y.fixed = level;
      FieldEvent_Wait(1);
      MapGrid_CenterOnFollowedPoint();
    }
  } else if (scene == SCENE_ANKOHL_RUINS_LEFT_PRONG) {
    object = Actor_GetById(ACTOR_LEFT_PRONG_CHEST);
    if (object != NULL) {
      object->motionFlags = 0;
      object->groundHeight = toFIXED(72);
      object->y.fixed = toFIXED(72);
      object->update = (void (*)(void))AnkohlRuins_RaiseDust;
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
  struct Actor *object;

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
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_A)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_B)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_C)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_D)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_D, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_E)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_E, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_F)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_F, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_G)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_G, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_H)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_H, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_I)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_I, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_J)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_J, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_K)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_K, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_L)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_L, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_M)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_M, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_IVY_CLEARED(ACTOR_FIRST_ROOM_IVY_N)))
      Actor_SetPosition(ACTOR_FIRST_ROOM_IVY_N, 0, 0);
    if (GameFlag_Get(FLAG_FIRST_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FIRST_ROOM_PILLAR, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_THANATOS_MACE_CHEST) {
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_A)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_B)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_IVY_CLEARED(ACTOR_THANATOS_IVY_C)))
      Actor_SetPosition(ACTOR_THANATOS_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_THANATOS_FACE_HALF_PLACED)) {
      s32 width;

      object = Actor_GetById(ACTOR_THANATOS_FACE_HALF);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_THANATOS_FACE_HALF, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_GREAT_FACE) {
    if (GameFlag_Get(FLAG_THIRD_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_THIRD_ROOM_PILLAR, 520, 104);
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
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_A)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_A, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_B)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_B, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_C)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_C, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_D)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_D, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_E)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_E, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_F)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_F, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_IVY_CLEARED(ACTOR_FOURTH_ROOM_IVY_G)))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_IVY_G, 0, 0);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_PILLAR, 40, 776);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED))
      Actor_SetPosition(ACTOR_FOURTH_ROOM_SECOND_PILLAR, 632, 456);
    if (GameFlag_Get(FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FOURTH_ROOM_THIRD_PILLAR, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      object = Actor_GetById(ACTOR_LEFT_FACE_HALF);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Actor_SetPosition(ACTOR_LEFT_FACE_HALF, 200, 728);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED)) {
      object = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 232, 728);
      object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    } else if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP) &&
               !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
    }
    if (GameFlag_Get(FLAG_RIGHT_FACE_HALF_PLACED) &&
        GameFlag_Get(FLAG_LEFT_FACE_HALF_PLACED)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
        Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->leaderActorId, 0, 0);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(ACTOR_RIGHT_FACE_HALF);
        Actor_SetPosition(ACTOR_RIGHT_FACE_HALF, 328, 760);
        object->y.fixed += toFIXED(128);
        frames = 60;
        object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_VBLANK);
  } else if (scene == SCENE_ANKOHL_RUINS_CRYSTAL_POWDER_CHEST) {
    if (GameFlag_Get(FLAG_FACE_HALF_PUSHED_INTO_GAP))
      Actor_SetPosition(ACTOR_FACE_HALF_FOR_THE_GAP, 0, 0);
    if (GameFlag_Get(FLAG_FIFTH_ROOM_PILLAR_MOVED)) {
      Actor_SetPosition(ACTOR_FIFTH_ROOM_PILLAR, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(SE_ANKOHL_RUINS_PIECE);
      Task_AddFrame(AnkohlRuins_FlowWithSand, TASK_PASS_VBLANK);
    }
  } else if (scene == SCENE_ANKOHL_RUINS_SAND_DOORS) {
    s16 *entrance = &work->entrance;

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
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsSpriteWork *)entrance)->unused - 1) <= 1) {
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
      if ((u16)(*entrance - 3) <= 1) {
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
      object = Actor_GetById(work->leaderActorId);
      object->groundHeight = level;
      object->y.fixed = level;
      FieldEvent_Wait(1);
      MapGrid_CenterOnFollowedPoint();
    }
  } else if (scene == SCENE_ANKOHL_RUINS_LEFT_PRONG) {
    object = Actor_GetById(ACTOR_LEFT_PRONG_CHEST);
    if (object != NULL) {
      object->motionFlags = 0;
      object->groundHeight = toFIXED(72);
      object->y.fixed = toFIXED(72);
      object->update = (void (*)(void))AnkohlRuins_RaiseDust;
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

/// The exit regions of the fifth room.
u32 gAnkohlRuins_StoneFacesRegions[] = {
    0xee0333, 0x33b0354, 0x35c00f6, 0xcffff, 0xffff, 0,
    0, 0,
};

/// The exit regions of the Left Prong's room.
u32 gAnkohlRuins_ThanatosMaceChestRegions[] = {
    0xffe600b4, 0xbc00a4, 0xacffee, 0x1ffff, 0xffff, 0,
    0, 0,
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

/// The tufts of ivy's script: sets work value 11 to 1.5 and hides each one
/// through AnkohlRuins_ClearScriptedActorSpriteFlags.
s32 gAnkohlRuins_ShrunkenSpriteScript[] = {
    // XXX: script work value 11 has no name in ActorScriptValue; a probe of
    // what the set changes on the ivy would name it.
    SC_SET(11, FIXED_ONE * 3 / 2),
    SC_CALL(AnkohlRuins_ClearScriptedActorSpriteFlags),
    SC_END,
};

/// The Thanatos Mace chest's script: it stands still.
u32 gAnkohlRuins_ThanatosMaceChestScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

/// The great face's script: it does not collide.
u32 gAnkohlRuins_GreatFaceScript[] = {
    SC_SET(SW_COLLISION, 0),
    SC_END,
};

/// The scene's other rooms' actors: none.
struct ActorPlacement gAnkohlRuins_Placements[] = {
    {SCENE_TABLE_END},
};

/// The first room's actors, in AnkohlRuinsFirstRoomActorId's order.
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
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 456, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 696, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 568, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 600, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 648, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 584, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 168, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 264, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 296, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 184, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 840, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 936, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 856, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The Thanatos Mace chest's room's actors, in its enum's order.
struct ActorPlacement gAnkohlRuins_ThanatosMaceChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (s32)gAnkohlRuins_ThanatosMaceChestScript, 120, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 472, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 520, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 600, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The third room's actors, in AnkohlRuinsThirdRoomActorId's order.
struct ActorPlacement gAnkohlRuins_GreatFacePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE, CONDITION_ALWAYS, (s32)gAnkohlRuins_GreatFaceScript, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_ThanatosMaceChestScript, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The fourth room's actors, in AnkohlRuinsFourthRoomActorId's order.
struct ActorPlacement gAnkohlRuins_FaceHalvesPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_A, CONDITION_ALWAYS, (s32)gAnkohlRuins_ThanatosMaceChestScript, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_ThanatosMaceChestScript, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 504, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 712, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 792, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 648, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 728, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 808, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The fifth room's actors, in AnkohlRuinsFifthRoomActorId's order.
struct ActorPlacement gAnkohlRuins_CrystalPowderChestPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_FACE_PILLAR_HALF_B, CONDITION_ALWAYS, (s32)gAnkohlRuins_ThanatosMaceChestScript, 440, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/// The sand rooms' actors: none.
struct ActorPlacement gAnkohlRuins_SandDoorsPlacements[] = {
    {SCENE_TABLE_END},
};

/// The Left Prong's room's actors: none.
struct ActorPlacement gAnkohlRuins_LeftProngPlacements[] = {
    {SCENE_TABLE_END},
};

/// Frames between the sand's moves as it rises.
s16 gAnkohlRuins_SandStepDelay = 10;

/// Frames the risen sand has stood.
s16 gAnkohlRuins_SandTimer = 0;

/// Frames the risen sand stands before it sinks.
s16 gAnkohlRuins_SandTime = 0;

/// How far the sand rises each move, in 16.16 pixels.
s32 gAnkohlRuins_SandStep = toFIXED(1) / 2;

/// Which sand room's switch was pressed: 1 or 2.
s32 gAnkohlRuins_SandRoom = 0;

/// The scene's other rooms' triggers: two exits.
struct Trigger gAnkohlRuins_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

/// The first room's exits and stairs, Sand, Move on its stone faces and pillar,
/// and Whirlwind on its ivy.
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROOM_PILLAR, FLAG_FIRST_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_ROOM_PILLAR, FLAG_FIRST_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_A, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_A), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_B, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_B), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_C, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_C), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_D, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_D), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_E, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_E), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_F, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_F), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STONE_FACE_G, FLAG_STONE_FACE_MOVED(ACTOR_STONE_FACE_G), EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_D, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_E, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_F, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_G, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_H, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_I, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_J, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_K, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_L, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_M, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_ROOM_IVY_N, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The Thanatos Mace chest's room's exits and stairs, Sand, Move on the face
/// half and Whirlwind on its ivy.
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceFacePiece)},
    {TRIGGER_PUSHED, ACTOR_THANATOS_FACE_HALF, FLAG_THANATOS_FACE_HALF_PLACED, EVENT(AnkohlRuins_PlaceFacePiece)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_IVY_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_IVY_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_THANATOS_IVY_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The third room's exits and stairs, Sand, Move on its pillar and the great
/// face's half, and the steps that board the raft.
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
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AnkohlRuins_BoardRaft)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AnkohlRuins_BoardRaftAgain)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_THIRD_ROOM_PILLAR, FLAG_THIRD_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_ROOM_PILLAR, FLAG_THIRD_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, EVENT(AnkohlRuins_PushRaftPillar)},
    {TRIGGER_PUSHED, ACTOR_GREAT_FACE_HALF, FLAG_GREAT_FACE_HALF_PLACED, EVENT(AnkohlRuins_PushRaftPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The fourth room's exits and stairs, Sand, Move on the face halves and its
/// pillars, and Whirlwind on its ivy.
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
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_A, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_B, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_C, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_D, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_E, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_F, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FOURTH_ROOM_IVY_G, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_FinishPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_FACE_HALF, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlacePillarPair)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_FACE_HALF, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlacePillarPair)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FOURTH_ROOM_PILLAR, FLAG_FOURTH_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_ROOM_PILLAR, FLAG_FOURTH_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FOURTH_ROOM_SECOND_PILLAR, FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_ROOM_SECOND_PILLAR, FLAG_FOURTH_ROOM_SECOND_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FOURTH_ROOM_THIRD_PILLAR, FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FOURTH_ROOM_THIRD_PILLAR, FLAG_FOURTH_ROOM_THIRD_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The fifth room's exits and stairs, its ledges, Sand, Move on its pillar and
/// the half face, and Reveal on the gap.
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
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT(AnkohlRuins_JumpDownLedge)},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_STEP, 66, CONDITION_ALWAYS, EVENT(AnkohlRuins_PushedOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIFTH_ROOM_PILLAR, FLAG_FIFTH_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIFTH_ROOM_PILLAR, FLAG_FIFTH_ROOM_PILLAR_MOVED, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FACE_HALF_FOR_THE_GAP, FLAG_FACE_HALF_PUSHED_INTO_GAP, EVENT(AnkohlRuins_PushStatueIntoGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RevealGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_HideGap)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

/// The sand rooms' exits and stairs, Sand, Reveal, the switches, the doors and
/// the rising sand.
struct Trigger gAnkohlRuins_SandDoorsTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_STEP, 40, FLAG_SCENE_TEMP_02, EVENT(AnkohlRuins_PressFirstSandSwitch)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenFirstSandDoor)},
    {TRIGGER_STEP, 42, FLAG_SCENE_TEMP_03, EVENT(AnkohlRuins_PressSecondSandSwitch)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenSecondSandDoor)},
    {TRIGGER_RAISED, 68, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseFirstSand)},
    {TRIGGER_RAISED, 69, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseSecondSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_EndReveal)},
    {TRIGGER_TABLE_END},
};

/// The Left Prong's room's triggers: none.
struct Trigger gAnkohlRuins_LeftProngTriggers[] = {
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_ANKOHL_RUINS_OPEN_ONCE
