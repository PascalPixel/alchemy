/// @file
/// The Kandorean Temple of Master Poi, south-west of Daila, in nine rooms that
/// share this overlay (SceneId_KandoreanTempleLifts to
/// SceneId_KandoreanTemplePoi).
///
/// Its gate is shut for the meditation (KandoreanTemple_StartGate): Kraden
/// spots a suspicious wall and the party finds a hidden way in through a cave
/// (KandoreanTemple_FindTheHiddenPassage). Inside, Poi tests three students
/// (KandoreanTemple_TestTheStudents), and the party follows the one who can
/// levitate through the temple's trials: a hot floor, steam that pushes the
/// leader along, two lifts, two air vents whose platforms a pillar holds down
/// (KandoreanTemple_RunLeftVent, KandoreanTemple_RunRightVent), and three
/// Chestbeaters (KandoreanTemple_FightChestbeaters). Poi then gives the party
/// the Lash Pebble (KandoreanTemple_ReceiveLash), and Lash reaches Fog, the
/// Mercury Djinni (KandoreanTemple_FightFog).

#include "field/actor.h"
#include "scene/event_layer.h"
#include "field/actor_view.h"
#include "djinn_ids.h"
#include "field/field_sprite.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "system/math_table.h"
#include "macros.h"
#include "field/map_grid.h"
#include "field/actor_script.h"
#include "field/scene_tables.h"
#include "types.h"
#include "version.h"
#include "battle/battle.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_sequence_event.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "field/event_commands.h"
#include "field/field_effect.h"
#include "field/object.h"
#include "scene/scene.h"
#include "field/call.h"
#include "shared/tables.h"
#include "sound/sound.h"
#include "system/field_effect.h"
#include "system/frame.h"
#include "system/input.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/task.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// The Kandorean Temple's rooms: their tables' getters, the steam, vents and
/// lifts.

/// The lift room's actors (SceneId_KandoreanTempleLifts), numbering the rows of
/// gKandoreanTemple_LiftRoomPlacements.
enum KandoreanTempleLiftRoomActorId {
  /// The poles of the two tightropes, a pair to each rope.
  ACTOR_FIRST_TIGHTROPE_POLE = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_TIGHTROPE_OTHER_POLE,
  ACTOR_SECOND_TIGHTROPE_POLE,
  ACTOR_SECOND_TIGHTROPE_OTHER_POLE,
  /// The pillar Move pushes along (gKandoreanTemple_BlockActors).
  ACTOR_LIFT_ROOM_PILLAR,
  ACTOR_WEST_LIFT,
  ACTOR_EAST_LIFT,
  /// The columns that grow under the lifts as they rise.
  ACTOR_WEST_LIFT_COLUMN,
  ACTOR_EAST_LIFT_COLUMN
};

/// The Chestbeaters' room's actors (SceneId_KandoreanTempleChestbeaters),
/// numbering the rows of gKandoreanTemple_ChestbeaterRoomPlacements.
enum KandoreanTempleChestbeaterRoomActorId {
  /// The pot of boiling water Move chucks aside
  /// (gKandoreanTemple_PillarActors).
  ACTOR_BOILING_POT = ACTOR_FIRST_PLACED,
  /// A second actor in the pot's sprite at the same place, in front of it, kept
  /// disabled in its animation 2.
  /// XXX: what it shows; a render of the room with the actor enabled would
  /// settle it.
  ACTOR_SECOND_BOILING_POT,
  /// The sign by the hot floor, which tells of Master Poi's trial.
  ACTOR_POIS_SIGN,
  ACTOR_FIRST_CHESTBEATER,
  ACTOR_WEST_CHESTBEATER,
  ACTOR_EAST_CHESTBEATER
};

/// The vent room's actors (SceneId_KandoreanTemplePillar), numbering the rows
/// of gKandoreanTemple_VentRoomPlacements.
enum KandoreanTempleVentRoomActorId {
  /// The pillar Move pushes onto either platform to hold it down.
  ACTOR_VENT_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The platforms over the two air vents, which the steam lifts.
  ACTOR_WEST_PLATFORM,
  ACTOR_EAST_PLATFORM,
  /// The tops of the columns that grow under the platforms as they rise.
  ACTOR_WEST_COLUMN_TOP,
  ACTOR_EAST_COLUMN_TOP,
  /// The west column's pieces below its top, from the highest down. A platform
  /// whose pillar holds the other one down rises on all four; otherwise on the
  /// upper two.
  ACTOR_WEST_COLUMN_UPPER_PIECE,
  ACTOR_WEST_COLUMN_MIDDLE_PIECE,
  ACTOR_WEST_COLUMN_LOWER_PIECE,
  ACTOR_WEST_COLUMN_FOOT,
  ACTOR_EAST_COLUMN_UPPER_PIECE,
  ACTOR_EAST_COLUMN_MIDDLE_PIECE,
  ACTOR_EAST_COLUMN_LOWER_PIECE,
  ACTOR_EAST_COLUMN_FOOT
};

/// Fog's room's actors (SceneId_KandoreanTempleFog), numbering the rows of
/// gKandoreanTemple_FogRoomPlacements.
enum KandoreanTempleFogRoomActorId {
  /// The poles of the rope Lash ties across to Fog's ledge.
  ACTOR_FOG_ROPE_POLE = ACTOR_FIRST_PLACED,
  ACTOR_FOG_ROPE_OTHER_POLE,
  /// The student who has given up on the test: "It seems I may not have been
  /// ready after all..." He levitates until the party has met Poi.
  ACTOR_STUDENT_WHO_GAVE_UP,
  /// Fog, the Mercury Djinni.
  ACTOR_FOG
};

/// The temple gate's actors (SceneId_KandoreanTempleGate), numbering the rows
/// of gKandoreanTemple_GatePlacements.
enum KandoreanTempleGateActorId {
  /// The bush over the cave, which Whirlwind blows away.
  ACTOR_BUSH_OVER_THE_CAVE = ACTOR_FIRST_PLACED,
  /// "Have you come to see Master Poi?"
  ACTOR_STUDENT_ASKING_ABOUT_POI,
  /// "Once the period of meditation has begun, none may enter the temple
  /// grounds."
  ACTOR_STUDENT_ON_GUARD_DUTY,
  /// The three students meditating in the courtyard.
  ///@{
  ACTOR_STUDENT_WARY_OF_OUTSIDERS,
  ACTOR_STUDENT_ON_THE_FAILED_TEST,
  ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
  ///@}
  ACTOR_GATE_KRADEN
};

/// Poi's hall's actors (SceneId_KandoreanTemplePoi), numbering the rows of
/// gKandoreanTemple_PoisHallPlacements.
enum KandoreanTemplePoisHallActorId {
  /// The poles of the hall's two ropes: Lash ties the loose end of each.
  ACTOR_SOUTH_ROPE_WEST_POLE = ACTOR_FIRST_PLACED,
  ACTOR_SOUTH_ROPE_EAST_POLE,
  ACTOR_NORTH_ROPE_EAST_POLE,
  ACTOR_NORTH_ROPE_WEST_POLE,
  ACTOR_POI,
  /// The three students Poi tests, in turn; only the third can levitate well
  /// enough to go on to the final test.
  ///@{
  ACTOR_FIRST_TESTED_STUDENT,
  ACTOR_SECOND_TESTED_STUDENT,
  ACTOR_THIRD_TESTED_STUDENT,
  ///@}
  ACTOR_HALL_KRADEN
};

/// Poi's hall's story entrances: finding the rope still lying on the floor
/// (KandoreanTemple_FindTheRopeOnTheFloor), and coming in through the cave to
/// meet Poi (KandoreanTemple_MeetPoi).
enum { ENTRANCE_ROPE_ON_THE_FLOOR = 13, ENTRANCE_THROUGH_THE_CAVE = 14 };

/// The temple's scene and area flags, by the rooms that read them.
enum {
  /// The leader stands on the lift room's west or east lift, or on the vent
  /// room's west or east platform.
  FLAG_ON_WEST_LIFT = FLAG_SCENE_TEMP_08,
  FLAG_ON_EAST_LIFT = FLAG_SCENE_TEMP_09,
  /// Move has lifted the vent room's pillar off a platform.
  FLAG_PILLAR_LIFTED_OFF_PLATFORM = FLAG_SCENE_TEMP_0A,
  /// Move is carrying the vent room's pillar.
  FLAG_PILLAR_MOVING = FLAG_SCENE_TEMP_0B,
  /// Steam blows from the vent room's west or east vent.
  FLAG_WEST_VENT_BLOWING = FLAG_SCENE_TEMP_0C,
  FLAG_EAST_VENT_BLOWING = FLAG_SCENE_TEMP_0D,
  /// The vents' rush of steam is playing (SE_WAVE).
  FLAG_VENT_SOUND_PLAYING = FLAG_SCENE_TEMP_0E,
  /// Lash has tied the rope across to Fog, or the south rope of Poi's hall.
  FLAG_FOG_ROPE_TIED = FLAG_SCENE_TEMP_0F,
  FLAG_SOUTH_ROPE_TIED = FLAG_SCENE_TEMP_0F,
  /// The north rope of Poi's hall is tied across.
  FLAG_NORTH_ROPE_TIED = FLAG_SCENE_TEMP_10,
  /// The lift room's pillar blocks the shaft of steam, so stepping over it no
  /// longer sinks the leader into the steam.
  FLAG_PILLAR_BLOCKS_SHAFT = FLAG_SCENE_TEMP_11,
  /// The trapdoor in Poi's hall, or the passage by the gate's well, is open.
  FLAG_TRAPDOOR_OPEN = FLAG_SCENE_TEMP_12,
  FLAG_WELL_PASSAGE_OPEN = FLAG_SCENE_TEMP_12,
  /// Kraden has eyed the suspicious wall by the gate on this visit.
  FLAG_KRADEN_EYED_THE_WALL = FLAG_SCENE_TEMP_30,
  /// Where the vent room's pillar stands, which also decides which of the lift
  /// room's lifts runs.
  FLAG_PILLAR_ON_WEST_PLATFORM = FLAG_AREA_TEMP_00,
  FLAG_PILLAR_ON_EAST_PLATFORM = FLAG_AREA_TEMP_01,
  /// Where Move has chucked the Chestbeaters' room's pot of boiling water.
  FLAG_POT_PUSHED_WEST = FLAG_AREA_TEMP_02,
  FLAG_POT_PUSHED_EAST = FLAG_AREA_TEMP_03,
  /// The pot is out of the way, and the floor no longer burns.
  FLAG_POT_MOVED = FLAG_AREA_TEMP_04
};

s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);
void Actor_DisableById(s32 id);
extern s32 gKandoreanTemple_LeftVentBlowing;
extern s32 gKandoreanTemple_RightVentBlowing;
extern s32 gKandoreanTemple_SteamCounter;
extern s32 gKandoreanTemple_SteamScript[];
extern u32 gKandoreanTemple_PuffScript[];
extern u32 gKandoreanTemple_StreamPuffScript[];

void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void FieldEffect_StartBlankOverlay(s32 a);
void KandoreanTemple_StartGate(void);
void Actor_MoveToObject(s32 id, s32 target);
extern struct Actor *gKandoreanTemple_LashPebble;
extern s32 gKandoreanTemple_MeditateScript[];
extern s32 gKandoreanTemple_LevitateScript[];

extern u32 gKandoreanTemple_LookAroundScript[];
extern s16 gKandoreanTemple_BlockActors[];
extern s16 gKandoreanTemple_PillarActors[];

/// Sets an actor's sprite flags (Actor_SetSpriteFlags).
static __inline__ void Actor_SetSingleChildFlags_(struct Actor *object,
                                                  s32 flags) {
  Actor_SetSpriteFlags(object, flags);
}

/// Creates an actor of the given sprite at a 16.16 position.
static __inline__ struct Actor *KandoreanTemple_CreateActor(s32 type, s32 x,
                                                            s32 y, s32 z) {
  return ((struct Actor *)Import_ActorCreate(type, x, y, z));
}

/// The Kandorean Temple's story, from the hidden passage Kraden suspects to
/// the ladder that unrolls once the Chestbeaters are beaten.
#define FLAG_HIDDEN_PASSAGE_FOUND 0x842
#define FLAG_ASKED_ABOUT_POI 0x845
#define FLAG_TOLD_OF_THE_MEDITATION 0x846
#define FLAG_STUDENTS_TESTED 0x848
#define FLAG_MET_POI 0x849
#define FLAG_LASH_RECEIVED 0x84a
#define FLAG_LADDER_LOWERED 0x92d

/// The map work as the stream and steam rooms write it: MapWork.groundEffects
/// as its two fields, the footprints and the kind of wading, which they set
/// to 1.
struct MapWorkGroundEffects {
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  u16 flags;
  u8 wideTiles;
  u8 footprints : 2;
  u8 wading : 2;
};

extern u32 gKandoreanTemple_StreamRoomRegions[];
extern struct ActorPlacement gKandoreanTemple_GatePlacements[];
extern struct ActorPlacement gKandoreanTemple_PoisHallPlacements[];
extern s16 gKandoreanTemple_Room2Events[];
void KandoreanTemple_ArriveOnEastLift(void);

void KandoreanTemple_TestTheStudents(void);
extern u32 gKandoreanTemple_PoiWalksOutScript[];
extern u32 gKandoreanTemple_StudentWalksAsideScript[];
extern u32 gKandoreanTemple_StudentWalksAwayScript[];

void KandoreanTemple_FindTheHiddenPassage(void);
void KandoreanTemple_FightChestbeaters(void);

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];

extern s32 gKandoreanTemple_LevitateHighScript[];
extern s32 gKandoreanTemple_LevitateLowScript[];
extern s32 gKandoreanTemple_StandStillScript[];

void KandoreanTemple_ArriveOnWestLift(void);

extern s32 gKandoreanTemple_LeftTimer;
extern u16 gKandoreanTemple_LeftBobPhase;
extern u16 gKandoreanTemple_RightBobPhase;
void Tightrope_Initialize(void);
void Actor_PlaceBlocks(const void *data);

extern s32 gKandoreanTemple_RightTimer;

/// Talking to Fog starts its battle.
void KandoreanTemple_FightFog(void) {
  Field_RequestDjinnBattle(ACTOR_FOG, DJINNI_FOG);
}

/// A puff of steam's script step: counts down its life and reports whether any
/// is left.
u32 KandoreanTemple_CountDownSteam(struct Actor *object) {
  s32 left;

  left = --*(s16 *)&object->userValue;
  return NONZERO(left);
}

/// A meditating student's update: now and then, every four to eight seconds, an
/// ellipsis over his head.
s32 KandoreanTemple_MeditateRestlessly(struct Actor *object) {
  if (object->timer != 0) {
    object->timer--;
  } else {
    Actor_ShowEmoteById((s16)object->userValue, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
    object->timer = (Random_Below(240)) + 240;
  }
  return 1;
}

u32 __umodsi3(u32 a, u32 b);

/// A levitating student's update: every second or two his sprite squashes and
/// springs back.
s32 KandoreanTemple_LevitateStudentBob(struct Actor *object) {
  switch (object->timer) {
  case 6:
    object->scaleX -= FIXED_ONE / 4;
    {
      s32 scale = object->scaleY;

      object->scaleY = scale + 0x2000;
    }
    break;
  case 4:
    object->scaleX += FIXED_ONE / 8;
    object->scaleY -= FIXED_ONE / 16;
    break;
  case 2:
    object->scaleX += FIXED_ONE / 16;
    object->scaleY -= FIXED_ONE / 32;
    break;
  case 0:
    object->scaleX = FIXED_ONE;
    object->scaleY = FIXED_ONE;
    object->timer = Random16() % 90 + 60;
    break;
  }
  object->timer--;
  return 1;
}

/// Sets up a levitating student: held off the ground, at four fifths of his
/// size, with no shadow.
s32 KandoreanTemple_PlaceLevitatingStudent(struct Actor *object) {
  object->motionFlags = 0;
  object->scaleX = FIXED_ONE * 4 / 5;
  object->scaleY = FIXED_ONE * 4 / 5;
  object->collisionFlags |= 8;
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern u32 gKandoreanTemple_ChestbeaterRoomRegions[];

/// The regions of the rooms whose exits are rectangles: the Chestbeaters' room
/// and the stream room.
u32 *Scene_GetExitRegions(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
    return gKandoreanTemple_ChestbeaterRoomRegions;
  if (scene == SCENE_KANDOREAN_TEMPLE_STREAM)
    return gKandoreanTemple_StreamRoomRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gKandoreanTemple_Placements[];
extern struct ActorPlacement gKandoreanTemple_LiftRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_ChestbeaterRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_VentRoomPlacements[];
extern struct ActorPlacement gKandoreanTemple_FogRoomPlacements[];

/// The current room's actors. Once the party has met Poi, the gate's meditating
/// students no longer turn to talk, and in Poi's hall two of the tested
/// students stand elsewhere.
void *Scene_GetActorPlacements(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
    return gKandoreanTemple_LiftRoomPlacements;
  if (scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
    return gKandoreanTemple_ChestbeaterRoomPlacements;
  if (scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
    return gKandoreanTemple_VentRoomPlacements;
  if (scene == SCENE_KANDOREAN_TEMPLE_FOG)
    return gKandoreanTemple_FogRoomPlacements;
  if (scene == SCENE_KANDOREAN_TEMPLE_GATE) {
    if (GameFlag_Get(FLAG_MET_POI)) {
      gKandoreanTemple_GatePlacements[3].talkFacing = 0;
      gKandoreanTemple_GatePlacements[4].talkFacing = 0;
      gKandoreanTemple_GatePlacements[5].talkFacing = 0;
    }
    return gKandoreanTemple_GatePlacements;
  }
  if (scene == SCENE_KANDOREAN_TEMPLE_POI) {
    if (GameFlag_Get(FLAG_MET_POI)) {
      gKandoreanTemple_PoisHallPlacements[5].talkFacing = 3;
      gKandoreanTemple_PoisHallPlacements[6].x = toFIXED(368);
      gKandoreanTemple_PoisHallPlacements[6].z = toFIXED(694);
      gKandoreanTemple_PoisHallPlacements[7].talkFacing = 1;
    }
    return gKandoreanTemple_PoisHallPlacements;
  }
  return gKandoreanTemple_Placements;
}

/// By the gate, until the hidden passage is found, Kraden walks up to the
/// leader and points out the suspicious wall.
void KandoreanTemple_KradenSuspectsTheWall(void) {
  struct Actor *object;

  if (GameFlag_Get(FLAG_HIDDEN_PASSAGE_FOUND) == 0) {
    GameFlag_Set(FLAG_KRADEN_EYED_THE_WALL);
    Event_Start();
    Event_ResolvePendingActions(0);
    Actor_WalkToAndWaitById(ACTOR_FELIX, 104, 252);
    Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
    FieldActor_LaunchFromLeader(ACTOR_GATE_KRADEN, -16, -16, FACING_NORTH);
    Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
    Event_Wait(20);
    Event_SetMessageId((s32)&MsgKandoreanTempleSuspiciousWall);
    // "This wall looks rather suspicious, wouldn't you say, Felix?"
    Event_ShowMessageAndWait(ACTOR_GATE_KRADEN, 0);
    Actor_FaceDirectionAndRefreshById(ACTOR_GATE_KRADEN, FACING_SOUTHEAST);
    Event_Wait(30);
    Actor_SetAnimationById(ACTOR_GATE_KRADEN, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_GATE_KRADEN, 1.2);
    Actor_SetAnimationById(ACTOR_GATE_KRADEN, ANIM_WALK);
    object = Actor_GetById(ACTOR_FELIX);
    if (object)
      Actor_SetDestinationById(ACTOR_GATE_KRADEN, object->x.part[0].integer,
                               object->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
    Actor_SetPosition(ACTOR_GATE_KRADEN, 0, 0);
    Event_End();
  }
}

extern struct Trigger gKandoreanTemple_Events[];
extern struct Trigger gKandoreanTemple_LiftRoomEvents[];
extern struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[];
extern struct Trigger gKandoreanTemple_VentRoomEvents[];
extern struct Trigger gKandoreanTemple_Room5Events[];
extern struct Trigger gKandoreanTemple_Room6Events[];
extern struct Trigger gKandoreanTemple_FogRoomEvents[];
extern struct Trigger gKandoreanTemple_GateEvents[];
extern struct Trigger gKandoreanTemple_PoisHallEvents[];

/// The current room's triggers.
void *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
    return gKandoreanTemple_LiftRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_MIMIC)
    return gKandoreanTemple_Room2Events;
  if (scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
    return gKandoreanTemple_ChestbeaterRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
    return gKandoreanTemple_VentRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_STREAM)
    return gKandoreanTemple_Room5Events;
  if (scene == SCENE_KANDOREAN_TEMPLE_STEAM)
    return gKandoreanTemple_Room6Events;
  if (scene == SCENE_KANDOREAN_TEMPLE_FOG)
    return gKandoreanTemple_FogRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_GATE)
    return gKandoreanTemple_GateEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_POI)
    return gKandoreanTemple_PoisHallEvents;
  return gKandoreanTemple_Events;
}

/// The leader starts trailing puffs of steam as it walks.
void KandoreanTemple_StartSteamTrail(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  KandoreanTemple_TrailSteam(leader, 0);
}

/// Counts an actor's steps through the steam, faster when it runs, and puffs a
/// cloud of steam every so often.
void KandoreanTemple_TrailSteam(struct Actor *object, s32 step) {
  s32 speed = (object->velocityX + object->velocityZ) >> 1;
  s32 *counter = &gKandoreanTemple_SteamCounter;

  if (speed < 0)
    speed = -speed;
  if ((u32)speed > 0xbb80)
    *counter += 4;
  else if (speed != 0)
    *counter += 2;
  gKandoreanTemple_SteamCounter += step;
  if (gKandoreanTemple_SteamCounter > 40) {
    KandoreanTemple_PuffSteam(object);
    gKandoreanTemple_SteamCounter = 0;
  }
}

/// Puffs a cloud of steam at an actor's feet.
void KandoreanTemple_PuffSteam(struct Actor *object) {
  struct Actor *steam = KandoreanTemple_CreateActor(
      0x10d, object->x.fixed, object->y.fixed, object->z.fixed);
  struct FieldSprite *sprite;

  if (steam) {
    sprite = steam->sprite;
    Actor_StartScript(steam, gKandoreanTemple_PuffScript);
    steam->motionFlags = 0;
    steam->layer = 1;
    steam->priorityFlags = 2;
    if (sprite) {
      Sprite_StartAnimation(sprite, 2);
      sprite->blendMode = 1;
      sprite->nextFrame = 0;
      sprite->priority = 3;
    }
  }
}

/// The current of steam pushes the leader north, with a puff of steam every
/// eight frames.
void KandoreanTemple_PushThroughTheSteam(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *steam;

  if ((gFrameCount & 7) == 0) {
    steam = ((struct Actor *)Import_ActorCreate(
        14, leader->x.fixed, leader->y.fixed + toFIXED(6), leader->z.fixed));
    if (steam) {
      Actor_SetSpriteFlags(steam, 0);
      Actor_SelectAnimation(steam, 1);
      Actor_StartScript(steam, gKandoreanTemple_StreamPuffScript);
    }
  }
  KandoreanTemple_PushNorth(leader, 55705);
}

/// Moves an actor and the camera north by dz, unless the map stops it.
void KandoreanTemple_PushNorth(struct Actor *object, s32 dz) {
  struct Actor *focus = gFieldWork->camera;
  s32 position[3];

  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed + dz;
  MapGrid_CheckCollision(object, position);
  position[0] = object->x.fixed;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x.fixed + 0x5b333;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  position[0] = object->x.fixed - 0x5b333;
  position[1] = object->y.fixed;
  position[2] = object->z.fixed + 0x5b333;
  if (MapGrid_CheckCollision(object, position) > 0)
    return;
  focus->z.fixed += dz;
  object->z.fixed += dz;
}

/// Whether actor a is within twelve pixels across and eight deep of actor b.
s32 KandoreanTemple_IsOverActor(struct Actor *a, struct Actor *b) {
  s32 left = b->x.fixed - toFIXED(12);
  s32 right = b->x.fixed + toFIXED(12);

  if (a->x.fixed > left && a->x.fixed < right &&
      a->z.fixed > b->z.fixed - toFIXED(8) &&
      a->z.fixed < b->z.fixed + toFIXED(8))
    return 1;
  return 0;
}

/// A cloud of the vents' steam: drifts and slows, grows and turns, and rises
/// instead of drifting on where the leader or the pillar stands over it.
void KandoreanTemple_UpdateVentSteam(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *other = Actor_GetById(ACTOR_VENT_ROOM_PILLAR);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x.fixed += vx;
  object->y.fixed += object->gravity;
  vz = object->riseBaseY;
  object->z.fixed += vz;
  object->bounce = vx - vx / 18;
  object->riseBaseY = vz - vz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  sprite->rotation += object->timer;
  if (leader->y.fixed == 0) {
    if (KandoreanTemple_IsOverActor(object, leader)) {
      object->z.fixed -= object->riseBaseY;
      object->gravity += object->riseBaseY >> 2;
      object->riseBaseY = 0;
    }
    if (leader->y.fixed == 0 && KandoreanTemple_IsOverActor(object, other)) {
      object->z.fixed -= object->riseBaseY;
      object->gravity += object->riseBaseY >> 2;
      object->riseBaseY = 0;
    }
  }
}

/// Sets up a cloud of steam on the given sprite priority: translucent, no
/// shadow, its puff script.
void KandoreanTemple_InitializeSteam(struct Actor *object, s32 priority) {
  struct FieldSprite *sprite;

  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  sprite = object->sprite;
  sprite->priority = priority;
  Actor_SetSpriteFlags(object, 0);
  Actor_SelectAnimation(object, 2);
  Actor_StartScript(object, gKandoreanTemple_SteamScript);
  Actor_SetSpriteColor(object, 9);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

/// Blows a cloud of steam from a vent at position, rising at the given speed.
void KandoreanTemple_BlowSteam(s32 *position, s32 value) {
  struct Actor *object = KandoreanTemple_CreateActor(
      0x11d, position[0], position[1], position[2] - toFIXED(2));

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos((Random_Below(0x3000)) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() * 2) + value;
    timer = (u16)Random16() - 0x8000;
    object->timer = timer;
    KandoreanTemple_InitializeSteam(object, 3);
    object->update = (void (*)(void))KandoreanTemple_UpdateVentSteam;
    Actor_SetBlendMode(object, 1);
  }
}

/// A cloud of the lift room's stream of steam: as the vents' clouds, held down
/// by the leader or the lift room's pillar.
void KandoreanTemple_UpdateStreamSteam(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *other = Actor_GetById(ACTOR_LIFT_ROOM_PILLAR);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x.fixed += vx;
  object->y.fixed += object->gravity;
  vz = object->riseBaseY;
  object->z.fixed += vz;
  object->bounce = vx - vx / 18;
  object->riseBaseY = vz - vz / 16;
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->sprite;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  sprite->rotation += object->timer;
  if (KandoreanTemple_IsOverActor(object, leader)) {
    ((struct FieldSprite *)object->sprite)->priority = 3;
    object->z.fixed -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 2;
    object->riseBaseY = 0;
  }
  if (KandoreanTemple_IsOverActor(object, other)) {
    ((struct FieldSprite *)object->sprite)->priority = 3;
    object->z.fixed -= object->riseBaseY;
    object->gravity += object->riseBaseY >> 3;
    object->riseBaseY = 0;
  }
}

/// Blows a cloud of steam from the lift room's stream.
void KandoreanTemple_BlowStreamSteam(void) {
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      SPRITE_DUST_CLOUD, toFIXED(696), toFIXED(28), toFIXED(156)));

  if (object) {
    s32 timer;

    object->bounce = Trig_Cos((Random_Below(0x3000)) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() * 2) + 0x58000;
    timer = (u16)Random16() - 0x8000;
    object->timer = timer;
    KandoreanTemple_InitializeSteam(object, 1);
    object->update = (void (*)(void))KandoreanTemple_UpdateStreamSteam;
    Actor_SetBlendMode(object, 1);
  }
}

/// Shows the leader's sprite flags again.
void KandoreanTemple_ShowLeader(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
}

/// Sets a lift or platform at rest: on the ground, in front, in its lowered
/// animation, disabled.
void KandoreanTemple_PrepareLift(s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->riseCounter = 1;
  object->priorityFlags |= 2;
  Actor_SetAnimationById(id, 1);
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
}

/// Sets a lift's column or a piece of one at rest, disabled.
void KandoreanTemple_PrepareLiftBase(s32 id) {
  Actor_GetById(id)->riseCounter = 1;
  Actor_SetSpritePriority(id, 2);
  Actor_DisableById(id);
}

/// Poi casts Lash on the north rope's east pole.
void KandoreanTemple_CastLash(void) {
  FieldPsynergy_LoadResources(ABILITY_LASH, 1);
  FieldPsynergy_SetupActorPair(ACTOR_POI, ACTOR_NORTH_ROPE_EAST_POLE);
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  BattleEffect_StopMainActor();
  FieldPsynergy_End();
}

#define DEFINE_KANDOREAN_TEMPLE_VENT(name, blowing, period, x, z, strength)    \
  void name(void) {                                                            \
    s32 position[3];                                                           \
                                                                               \
    if (blowing != 0 && gFrameCount % period == 0) {                           \
      SET_VECTOR(position, x, 0, z);                                           \
      KandoreanTemple_BlowSteam(position, strength);                           \
    }                                                                          \
  }

DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowLeftVent,
                             gKandoreanTemple_LeftVentBlowing, 9, toFIXED(296),
                             toFIXED(176), toFIXED(2))

DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowRightVent,
                             gKandoreanTemple_RightVentBlowing, 5, toFIXED(344),
                             toFIXED(160), toFIXED(4))

/// The lift room's task: a cloud of steam from the stream every fifth frame.
void KandoreanTemple_BlowStream(void) {
  if (gFrameCount % 5 == 0)
    KandoreanTemple_BlowStreamSteam();
}

/// Lowers a piece of the west column flat under its platform.
void KandoreanTemple_LowerLeftColumn(s32 id) {
  Actor_GetById(id)->scaleY = FIXED_ONE / 32;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(id), toFIXED(216), 0,
                                       toFIXED(216));
}

/// Lowers a piece of the east column flat under its platform.
void KandoreanTemple_LowerRightColumn(s32 id) {
  Actor_GetById(id)->scaleY = FIXED_ONE / 32;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(id), toFIXED(376), 0,
                                       toFIXED(248));
}

/// Grows a column piece by scale and raises it by dy.
void KandoreanTemple_GrowColumn(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY += scale;
  object->y.fixed += dy;
  object->z.fixed -= dy;
}

/// Shrinks a column piece by scale and lowers it by dy.
void KandoreanTemple_ShrinkColumn(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY -= scale;
  object->y.fixed -= dy;
  object->z.fixed += dy;
}

/// Whether the west vent stays shut: while Move carries the pillar off a
/// platform, or while the pillar weighs the west platform down.
s32 KandoreanTemple_IsLeftVentBlocked(void) {
  if (GameFlag_Get(FLAG_PILLAR_LIFTED_OFF_PLATFORM) ||
      GameFlag_Get(FLAG_PILLAR_ON_WEST_PLATFORM))
    return 1;
  return 0;
}

/// Whether the east vent stays shut: while Move carries the pillar off a
/// platform, or while the pillar weighs the east platform down.
s32 KandoreanTemple_IsRightVentBlocked(void) {
  if (GameFlag_Get(FLAG_PILLAR_LIFTED_OFF_PLATFORM) ||
      GameFlag_Get(FLAG_PILLAR_ON_EAST_PLATFORM))
    return 1;
  return 0;
}

/// The west vent starts blowing: its column and platform ready to rise,
/// carrying the leader if it stands there.
void KandoreanTemple_StartLeftVent(void) {
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_TOP, 2);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_UPPER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_LOWER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_FOOT, 3);
  Actor_SetAnimationById(ACTOR_WEST_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_WEST_PLATFORM, 2);
  Actor_GetById(ACTOR_WEST_PLATFORM)->priorityFlags &= ~2;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_PLATFORM),
                                       toFIXED(216), toFIXED(8), toFIXED(216));
  if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
    Actor_SetAnimationById(ACTOR_WEST_PLATFORM, 4);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(gMainRoutineWork.leaderActorId), toFIXED(216),
        toFIXED(12), toFIXED(216));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.leaderActorId),
                               0);
  }
  Actor_GetById(ACTOR_WEST_COLUMN_TOP)->scaleY = FIXED_ONE;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_COLUMN_TOP),
                                       toFIXED(216), 0, toFIXED(216));
  KandoreanTemple_LowerLeftColumn(ACTOR_WEST_COLUMN_UPPER_PIECE);
  KandoreanTemple_LowerLeftColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE);
  if (GameFlag_Get(FLAG_PILLAR_ON_EAST_PLATFORM)) {
    KandoreanTemple_LowerLeftColumn(ACTOR_WEST_COLUMN_LOWER_PIECE);
    KandoreanTemple_LowerLeftColumn(ACTOR_WEST_COLUMN_FOOT);
  }
  gKandoreanTemple_LeftVentBlowing = 1;
  Event_SetFlag(FLAG_WEST_VENT_BLOWING);
}

/// The east vent starts blowing: its column and platform ready to rise,
/// carrying the leader if it stands there.
void KandoreanTemple_StartRightVent(void) {
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_TOP, 2);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_UPPER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_LOWER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_FOOT, 3);
  Actor_SetAnimationById(ACTOR_EAST_PLATFORM, 0);
  Actor_SetSpritePriority(ACTOR_EAST_PLATFORM, 2);
  Actor_GetById(ACTOR_EAST_PLATFORM)->priorityFlags &= ~2;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_PLATFORM),
                                       toFIXED(376), toFIXED(8), toFIXED(248));
  if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
    Actor_SetAnimationById(ACTOR_EAST_PLATFORM, 4);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(gMainRoutineWork.leaderActorId), toFIXED(376),
        toFIXED(12), toFIXED(248));
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.leaderActorId),
                               0);
  }
  Actor_GetById(ACTOR_EAST_COLUMN_TOP)->scaleY = FIXED_ONE;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_COLUMN_TOP),
                                       toFIXED(376), 0, toFIXED(248));
  KandoreanTemple_LowerRightColumn(ACTOR_EAST_COLUMN_UPPER_PIECE);
  KandoreanTemple_LowerRightColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE);
  if (GameFlag_Get(FLAG_PILLAR_ON_WEST_PLATFORM)) {
    KandoreanTemple_LowerRightColumn(ACTOR_EAST_COLUMN_LOWER_PIECE);
    KandoreanTemple_LowerRightColumn(ACTOR_EAST_COLUMN_FOOT);
  }
  gKandoreanTemple_RightVentBlowing = 1;
  Event_SetFlag(FLAG_EAST_VENT_BLOWING);
}

/// The west vent's task, a timer through its cycle: after five seconds it blows
/// and its platform rises on the column, higher when the pillar holds the east
/// platform down, bobs on the steam, then sinks back and waits again.
void KandoreanTemple_RunLeftVent(void) {
  s32 *timer = &gKandoreanTemple_LeftTimer;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 timerEnd;
  s32 t;
  s32 flag;

  if (*timer < 300) {
    if (KandoreanTemple_IsLeftVentBlocked() != 0)
      return;
    goto count;
  }
  if (*timer == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 63, 13);
    KandoreanTemple_StartLeftVent();
    goto count;
  }
  timerEnd = 2999;
  if (*timer <= timerEnd) {
    statue = Actor_GetById(ACTOR_WEST_PLATFORM);
    flag = GameFlag_Get(FLAG_PILLAR_ON_EAST_PLATFORM);
    t = *timer;
    if (flag) {
      if (t <= 316) {
        statue->y.fixed += toFIXED(9) / 2;
        limit = 0x1000;
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, 0x20000, limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, 0x18000,
                                   limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0x10000,
                                   limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_LOWER_PIECE, 32768, limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
      } else if (t < 600) {
        if (GameFlag_Get(FLAG_ON_WEST_LIFT) && *timer == 400) {
          MapGrid_CopyCellAttributes(22, 0, 1, 1, 13, 13);
          Task_Remove(KandoreanTemple_RunLeftVent);
        }
        phase = &gKandoreanTemple_LeftBobPhase;
        statue->y.fixed += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gKandoreanTemple_LeftBobPhase;
        if (*phase == 0) {
          gKandoreanTemple_LeftVentBlowing = 0;
          *timer = 999;
          GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
        }
        statue->y.fixed += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y.fixed += -toFIXED(27) / 4;
        limit = 0x1800;
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_TOP, 0x30000, limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, 0x24000,
                                     limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0x18000,
                                     limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_LOWER_PIECE, 49152,
                                     limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
        if (Actor_GetById(ACTOR_WEST_COLUMN_FOOT)->scaleY <= limit)
          *timer = timerEnd;
      }
    } else {
      if (t <= 316) {
        statue->y.fixed += 0x27999;
        limit = 0x1000;
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, 0x10000, limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, 32768, limit);
        KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, limit);
      } else if (t < 600) {
        phase = &gKandoreanTemple_LeftBobPhase;
        statue->y.fixed += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else if (t <= 999) {
        phase = &gKandoreanTemple_LeftBobPhase;
        if (*phase == 0) {
          gKandoreanTemple_LeftVentBlowing = 0;
          *timer = 999;
          GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
        }
        statue->y.fixed += Trig_Sin(*phase) >> 1;
        *phase += 0x1000;
      } else {
        statue->y.fixed += -0x3b666;
        limit = 0x1800;
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_TOP, 0x18000, limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, 49152,
                                     limit);
        KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, limit);
        if (Actor_GetById(ACTOR_WEST_COLUMN_MIDDLE_PIECE)->scaleY <= limit)
          *timer = timerEnd;
      }
    }
    if (GameFlag_Get(FLAG_ON_WEST_LIFT) == 0)
      goto count;
    leader = Actor_GetById(gMainRoutineWork.leaderActorId);
    leader->y.fixed = statue->y.fixed + toFIXED(4);
    leader->targetY = TARGET_UNSET;
    leader->velocityY = 0;
    goto count;
  }
  if (*timer == 3000) {
    MapGrid_CopyCellAttributes(8, 0, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(58, 0, 1, 1, 63, 13);
    Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
    Actor_SetAnimationById(ACTOR_WEST_PLATFORM, 1);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_PLATFORM),
                                         toFIXED(216), 0, toFIXED(216));
    Actor_GetById(ACTOR_WEST_PLATFORM)->priorityFlags |= 2;
    Actor_SetSpritePriority(ACTOR_WEST_PLATFORM, 3);
    if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
      leader = Actor_GetById(gMainRoutineWork.leaderActorId);
      Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0,
                                      leader->z.fixed);
      Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
    }
    goto count;
  }
  {
    s32 none = KandoreanTemple_IsLeftVentBlocked();

    if (none != 0)
      return;
    *timer += (Random16() << 7) >> 16;
    if ((u32)*timer > (Random_Below(10000)) + 10000)
      *timer = none;
  }
count:
  gKandoreanTemple_LeftTimer++;
}

/// Bob the statue on a sine.
#define BOB(statue, phase)                                                     \
  do {                                                                         \
    (statue)->y.fixed += Trig_Sin(*(phase)) >> 1;                              \
    *(phase) += 0x1000;                                                        \
  } while (0)

/// The east vent's task, as KandoreanTemple_RunLeftVent with the platforms
/// swapped.
void KandoreanTemple_RunRightVent(void) {
  s32 *timer = &gKandoreanTemple_RightTimer;
  struct Actor *statue;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 raised;
  s32 last;
  s32 t;

  t = *timer;
  if (t < 300) {
    if (KandoreanTemple_IsRightVentBlocked() != 0)
      return;
  } else if (t == 300) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 23, 15);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 73, 15);
    KandoreanTemple_StartRightVent();
  } else {
    last = 2999;
    if (t <= last) {
      statue = Actor_GetById(ACTOR_EAST_PLATFORM);
      raised = GameFlag_Get(FLAG_PILLAR_ON_WEST_PLATFORM);
      t = *timer;
      if (raised) {
        if (t <= 316) {
          statue->y.fixed += toFIXED(9) / 2;
          limit = 0x1000;
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, 0x20000, limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, 0x18000,
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0x10000,
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_LOWER_PIECE, 32768,
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
        } else if (t < 600) {
          if (GameFlag_Get(FLAG_ON_EAST_LIFT) && *timer == 400) {
            MapGrid_CopyCellAttributes(23, 0, 1, 1, 23, 15);
            Task_Remove(KandoreanTemple_RunRightVent);
          }
          phase = &gKandoreanTemple_RightBobPhase;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gKandoreanTemple_RightBobPhase;
          if (*phase == 0) {
            gKandoreanTemple_RightVentBlowing = 0;
            *timer = 999;
            GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
          }
          BOB(statue, phase);
        } else {
          statue->y.fixed += -toFIXED(27) / 4;
          limit = 0x1800;
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_TOP, 0x30000, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, 0x24000,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0x18000,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_LOWER_PIECE, 49152,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
          if (Actor_GetById(ACTOR_EAST_COLUMN_FOOT)->scaleY <= limit)
            *timer = last;
        }
      } else {
        if (t <= 316) {
          statue->y.fixed += 0x27999;
          limit = 0x1000;
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, 0x10000, limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, 32768,
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, limit);
        } else if (t < 600) {
          phase = &gKandoreanTemple_RightBobPhase;
          BOB(statue, phase);
        } else if (t <= 999) {
          phase = &gKandoreanTemple_RightBobPhase;
          if (*phase == 0) {
            gKandoreanTemple_RightVentBlowing = 0;
            *timer = 999;
            GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
          }
          BOB(statue, phase);
        } else {
          statue->y.fixed += -0x3b666;
          limit = 0x1800;
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_TOP, 0x18000, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, 49152,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0,
                                       limit);
          if (Actor_GetById(ACTOR_EAST_COLUMN_MIDDLE_PIECE)->scaleY <= limit)
            *timer = last;
        }
      }
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        leader->y.fixed = statue->y.fixed + toFIXED(4);
        leader->targetY = TARGET_UNSET;
        leader->velocityY = 0;
      }
    } else if (t == 3000) {
      MapGrid_CopyCellAttributes(8, 0, 1, 1, 23, 15);
      MapGrid_CopyCellAttributes(58, 0, 1, 1, 73, 15);
      Actor_SetPosition(ACTOR_EAST_COLUMN_TOP, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_FOOT, 0, 0);
      Actor_SetAnimationById(ACTOR_EAST_PLATFORM, 1);
      FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_PLATFORM),
                                           toFIXED(376), 0, toFIXED(248));
      Actor_GetById(ACTOR_EAST_PLATFORM)->priorityFlags |= 2;
      Actor_SetSpritePriority(ACTOR_EAST_PLATFORM, 3);
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0,
                                        leader->z.fixed);
        Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
      }
    } else {
      s32 none = KandoreanTemple_IsRightVentBlocked();

      if (none != 0)
        return;
      *timer += (Random16() << 7) >> 16;
      if ((u32)*timer > (Random_Below(10000)) + 10000)
        *timer = none;
    }
  }
  gKandoreanTemple_RightTimer++;
}

/// The lift room's west lift, a timer through its cycle: rises on its column,
/// bobs, and sinks again unless the leader rides it.
void KandoreanTemple_RunWestLift(void) {
  struct Actor *lift = Actor_GetById(ACTOR_WEST_LIFT);
  struct Actor *base = Actor_GetById(ACTOR_WEST_LIFT_COLUMN);
  s32 *timer = &gKandoreanTemple_LeftTimer;

  if (*timer == 300) {
    Scene_QueueMusic(SE_WAVE);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_LIFT),
                                         toFIXED(440), toFIXED(-20),
                                         toFIXED(204));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_LIFT_COLUMN),
                                    toFIXED(440), toFIXED(-20), toFIXED(200));
    Actor_SetSpritePriority(ACTOR_WEST_LIFT, 3);
    Actor_SetSpritePriority(ACTOR_WEST_LIFT_COLUMN, 3);
    base->priorityFlags |= 2;
    base->scaleY = FIXED_ONE / 64;
    Actor_SetAnimationById(ACTOR_WEST_LIFT, 0);
    Actor_SetAnimationById(ACTOR_WEST_LIFT_COLUMN, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y.fixed += toFIXED(4);
    KandoreanTemple_GrowColumn(ACTOR_WEST_LIFT_COLUMN, 19660, 0x4ccc);
    if (*(long *)&lift->y.fixed > -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_WEST_LIFT, 2);
    if (lift->y.fixed >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 27, 12);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gKandoreanTemple_LeftBobPhase;

    lift->y.fixed += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(27, 10, 1, 1, 27, 12);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
        MapGrid_CopyCellAttributes(21, 0, 1, 1, 27, 12);
        Task_Remove(KandoreanTemple_RunWestLift);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y.fixed += toFIXED(-4);
    KandoreanTemple_ShrinkColumn(ACTOR_WEST_LIFT_COLUMN, 19660, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y.fixed < -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_WEST_LIFT, 3);
    if (lift->y.fixed <= -toFIXED(20)) {
      Scene_QueueMusic(SE_AMBIENT_STREAM);
      Actor_SetPosition(ACTOR_WEST_LIFT, 0, 0);
      Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
      gKandoreanTemple_LeftTimer = 0;
    }
  }
  gKandoreanTemple_LeftTimer++;
}

/// The lift room's east lift, as KandoreanTemple_RunWestLift.
void KandoreanTemple_RunEastLift(void) {
  struct Actor *lift = Actor_GetById(ACTOR_EAST_LIFT);
  struct Actor *base = Actor_GetById(ACTOR_EAST_LIFT_COLUMN);
  s32 *timer = &gKandoreanTemple_RightTimer;

  if (*timer == 300) {
    Scene_QueueMusic(SE_WAVE);
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_LIFT),
                                         toFIXED(600), toFIXED(-20),
                                         toFIXED(236));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_LIFT_COLUMN),
                                    toFIXED(600), toFIXED(-20), toFIXED(232));
    Actor_SetSpritePriority(ACTOR_EAST_LIFT, 3);
    Actor_SetSpritePriority(ACTOR_EAST_LIFT_COLUMN, 3);
    base->priorityFlags |= 2;
    base->scaleY = FIXED_ONE / 64;
    Actor_SetAnimationById(ACTOR_EAST_LIFT, 0);
    Actor_SetAnimationById(ACTOR_EAST_LIFT_COLUMN, 2);
  } else if ((u32)(*timer - 301) <= 1698) {
    lift->y.fixed += toFIXED(4);
    KandoreanTemple_GrowColumn(ACTOR_EAST_LIFT_COLUMN, 19660, 0x4ccc);
    if (*(long *)&lift->y.fixed > -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_EAST_LIFT, 2);
    if (lift->y.fixed >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 14);
      *timer = 2000;
    }
  } else if (*timer < 2800) {
    u16 *phase = &gKandoreanTemple_RightBobPhase;

    lift->y.fixed += Trig_Sin(*phase) >> 1;
    *phase += 0x1000;
    if (*timer == 2280)
      MapGrid_CopyCellAttributes(37, 12, 1, 1, 37, 14);
    if (*timer > 2300 && *phase == 0) {
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        MapGrid_CopyCellAttributes(22, 0, 1, 1, 37, 14);
        Task_Remove(KandoreanTemple_RunEastLift);
      } else {
        *timer = 2800;
      }
    }
  } else {
    lift->y.fixed += toFIXED(-4);
    KandoreanTemple_ShrinkColumn(ACTOR_EAST_LIFT_COLUMN, 19660, 0x4ccc);
    // y read as long, as above.
    if (*(long *)&lift->y.fixed < -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_EAST_LIFT, 3);
    if (lift->y.fixed <= -toFIXED(20)) {
      Scene_QueueMusic(SE_AMBIENT_STREAM);
      Actor_SetPosition(ACTOR_EAST_LIFT, 0, 0);
      Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
      gKandoreanTemple_RightTimer = 0;
    }
  }
  gKandoreanTemple_RightTimer++;
}

/// The vent room's task: notes whether the leader stands on a platform (where
/// it cannot save), lets a blowing vent push it north, and starts or stops the
/// vents' sound.
void KandoreanTemple_WatchVentPlatforms(void) {
  struct Actor *leader;
  s32 cellX, cellZ, onCell;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  cellX = FIELD_CELL(leader->x.fixed);
  cellZ = FIELD_CELL(leader->z.fixed);
  onCell = 0;
  if (cellX == 13 && cellZ == 13) {
    Event_SetFlag(FLAG_ON_WEST_LIFT);
    onCell = 1;
  } else {
    Event_ClearFlag(FLAG_ON_WEST_LIFT);
  }
  if (cellX == 23 && cellZ == 15) {
    Event_SetFlag(FLAG_ON_EAST_LIFT);
    onCell = 1;
  } else {
    Event_ClearFlag(FLAG_ON_EAST_LIFT);
  }
  if (onCell)
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
  else
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  if (gKandoreanTemple_LeftVentBlowing != 0 && leader->y.fixed == 0 &&
      leader->x.fixed > toFIXED(284) && leader->x.fixed < toFIXED(308) &&
      leader->z.fixed < toFIXED(200) && !Event_GetFlag(FLAG_PILLAR_MOVING))
    KandoreanTemple_PushNorth(leader, 0x26666);
  if (gKandoreanTemple_RightVentBlowing != 0 && leader->y.fixed == 0 &&
      leader->x.fixed > toFIXED(330) && leader->x.fixed <= 0x165ffff &&
      leader->z.fixed < toFIXED(216) && !Event_GetFlag(FLAG_PILLAR_MOVING))
    KandoreanTemple_PushNorth(leader, 0x39999);
  if (Event_GetFlag(FLAG_VENT_SOUND_PLAYING)) {
    if (!Event_GetFlag(FLAG_WEST_VENT_BLOWING) &&
        !Event_GetFlag(FLAG_EAST_VENT_BLOWING)) {
      Scene_QueueMusic(-1);
      Event_ClearFlag(FLAG_VENT_SOUND_PLAYING);
    }
  } else if (Event_GetFlag(FLAG_WEST_VENT_BLOWING) ||
             Event_GetFlag(FLAG_EAST_VENT_BLOWING)) {
    Scene_QueueMusic(SE_WAVE);
    Event_SetFlag(FLAG_VENT_SOUND_PLAYING);
  }
}

/// The lift room's task: a leader stepping onto a lift rides it, and cannot
/// save while it does.
void KandoreanTemple_WatchLifts(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = leader->x.fixed;
  s32 cellX = FIELD_CELL(x);
  s32 z = leader->z.fixed;
  s32 cellZ = FIELD_CELL(z);
  s32 riding = 0;

  if (leader->y.fixed > toFIXED(16))
    return;
  if ((u32)(x - toFIXED(400)) > toFIXED(240))
    return;
  if ((u32)(z - toFIXED(180)) > toFIXED(100))
    return;
  if (cellX == 27 && cellZ == 12) {
    struct Actor *lift = Actor_GetById(ACTOR_WEST_LIFT);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetAnimationById(ACTOR_WEST_LIFT, 4);
    leader->motionFlags = riding;
    leader->y.fixed = lift->y.fixed;
    GameFlag_Set(FLAG_ON_WEST_LIFT);
    riding = 1;
  }
  if (cellX == 37 && cellZ == 14) {
    struct Actor *lift = Actor_GetById(ACTOR_EAST_LIFT);
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
    Actor_SetAnimationById(ACTOR_EAST_LIFT, 4);
    leader->motionFlags = 0;
    leader->y.fixed = lift->y.fixed;
    GameFlag_Set(FLAG_ON_EAST_LIFT);
    riding = 1;
  }
  if (riding) {
    GameFlag_Set(FLAG_NO_PLACE_TO_SAVE);
    return;
  }
  Actor_SetAnimationById(ACTOR_WEST_LIFT, 0);
  Actor_SetAnimationById(ACTOR_EAST_LIFT, 0);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  leader->motionFlags = 3;
  GameFlag_Clear(FLAG_ON_WEST_LIFT);
  GameFlag_Clear(FLAG_ON_EAST_LIFT);
  GameFlag_Clear(FLAG_NO_PLACE_TO_SAVE);
}

/// Keeps the lifts and the west column in front of or behind the leader as the
/// leader is.
void KandoreanTemple_MatchLiftPriority(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 priority = ((struct FieldSprite *)leader->sprite)->priority;

  Actor_SetSpritePriority(ACTOR_WEST_LIFT, priority);
  Actor_SetSpritePriority(ACTOR_EAST_LIFT, priority);
  Actor_SetSpritePriority(ACTOR_WEST_LIFT_COLUMN, priority);
}

/// The vent room's task: a leader standing over a cell the map marks low drops
/// to the ground.
void KandoreanTemple_DropOffLowCells(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct MapCell *cell = &map->layers[1].cells[Actor_GetCellIndex(leader)];

  if (cell->collisionCode != 0) {
    leader->y.fixed = 0;
    leader->groundHeight = 0;
  }
}

/// Reads the sign by the hot floor.
void KandoreanTemple_ReadPoisSign(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "Extinguish your desire, and the fire will cool. Empty your mind, and..."
  FieldMessage_Show((s32)&MsgKandoreanTempleExtinguishYourDesire, 1);
  Event_End();
}

/// Tries the temple's locked door.
void KandoreanTemple_TryTheLockedDoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  // "The door is tightly locked."
  FieldMessage_Show((s32)&MsgKandoreanTempleDoorLocked, 1);
  Event_End();
}

/// The student by the gate asks whether the party has come to see Master Poi.
void KandoreanTemple_AskAboutPoi(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_Stop(ACTOR_STUDENT_ASKING_ABOUT_POI);
  wait(1);
  Actor_FaceActorById(ACTOR_STUDENT_ASKING_ABOUT_POI,
                      gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)&MsgKandoreanTempleComeToSeePoi);
  // "Have you come to see Master Poi?"
  Event_AskYesNoAndShowResponse(ACTOR_STUDENT_ASKING_ABOUT_POI, 0);
  Actor_GetById(ACTOR_STUDENT_ASKING_ABOUT_POI)->facing =
      FACING_SOUTH_SOUTHEAST;
  wait(1);
  Actor_SetMovementTypeById(ACTOR_STUDENT_ASKING_ABOUT_POI,
                            gKandoreanTemple_LookAroundScript);
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_ASKED_ABOUT_POI);
}

/// The student on guard tells the party that none may enter during the
/// meditation.
void KandoreanTemple_TellOfTheMeditation(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActorById(ACTOR_STUDENT_ON_GUARD_DUTY,
                      gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)&MsgKandoreanTempleMeditationBegun);
  // "Once the period of meditation has begun, none may enter the temple..."
  Event_ShowMessageAndWait(ACTOR_STUDENT_ON_GUARD_DUTY, 0);
  Actor_GetById(ACTOR_STUDENT_ON_GUARD_DUTY)->facing = FACING_SOUTH_SOUTHWEST;
  wait(1);
  Event_End();
  GameFlag_Set(FLAG_TOLD_OF_THE_MEDITATION);
}

/// The first tested student asks whether the party passed the test.
void KandoreanTemple_AskWhoPassedTheTest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgKandoreanTemplePassedTheTest);
  // "You were the ones who passed the test?"
  Event_AskYesNoAndShowResponse(ACTOR_FIRST_TESTED_STUDENT, 0);
  Event_End();
}

/// Mind Read on the student by the gate startles him into asking about Poi.
void KandoreanTemple_ReadTheGateStudent(void) {
  Actor_ShowEmoteById(ACTOR_STUDENT_ASKING_ABOUT_POI,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  KandoreanTemple_AskAboutPoi();
}

/// Mind Read on the student on guard startles him into telling of the
/// meditation.
void KandoreanTemple_ReadTheGuard(void) {
  Actor_ShowEmoteById(ACTOR_STUDENT_ON_GUARD_DUTY,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  KandoreanTemple_TellOfTheMeditation();
}

/// Climbs down the well by the gate, when the leader pushes toward it.
void KandoreanTemple_ClimbDownTheWell(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(leader->x.fixed);
  s32 z = FIELD_CELL(leader->z.fixed);

  if (z == 5) {
    if (x == 9 && (gInput.held & KEY_RIGHT) == 0)
      return;
    if (x == 11 && (gInput.held & KEY_LEFT) == 0)
      return;
  } else if (x == 10 && z == 6 && (gInput.held & KEY_UP) == 0) {
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  leader->motionFlags = 0;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 168, 80);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 13);
  FieldActor_SetMoveTarget(leader, 168, -12, 80);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 10);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(8);
}

/// Opens the trapdoor in Poi's hall.
void KandoreanTemple_OpenTheTrapdoor(void) {
  MapGrid_CopyCellPictureIndicesRect(39, 39, 24, 38, 2, 2);
  wait(10);
  MapGrid_CopyCellPictureIndicesRect(39, 41, 24, 38, 2, 2);
  wait(20);
  GameFlag_Set(FLAG_TRAPDOOR_OPEN);
}

/// The leader walks onto the trapdoor in Poi's hall, opening it if it is shut,
/// and drops through.
void KandoreanTemple_DropThroughTheTrapdoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 400, 648);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  if (GameFlag_Get(FLAG_TRAPDOOR_OPEN) == 0) {
    Sound_Play(SE_THUMP);
    KandoreanTemple_OpenTheTrapdoor();
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  Actor_WalkToById(gMainRoutineWork.leaderActorId, 400, 640);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(9);
}

/// The leader squeezes north into the crack in the wall.
void KandoreanTemple_SlipIntoTheCrack(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 416, 288);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  for (i = 0; i < 8; i++) {
    leader->z.fixed += toFIXED(1);
    wait(1);
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 13);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= toFIXED(1);
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(4);
}

/// Move lifts the lift room's pillar.
void KandoreanTemple_StartMovingBlock(void) {
  Actor_LiftTargetBlock(gKandoreanTemple_BlockActors);
}

/// Sets the lift room's pillar down, and notes whether it blocks the shaft of
/// steam.
void KandoreanTemple_FinishMovingBlock(void) {
  s32 x = FIELD_CELL(Actor_GetById(ACTOR_LIFT_ROOM_PILLAR)->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 43)
    GameFlag_Set(FLAG_PILLAR_BLOCKS_SHAFT);
  else
    GameFlag_Clear(FLAG_PILLAR_BLOCKS_SHAFT);
  if (x != 39)
    MapGrid_CopyCellAttributes(39, 0, 1, 1, 39, 10);
}

/// Move lifts the vent room's pillar, noting whether it leaves a platform.
void KandoreanTemple_StartMovingPillar(void) {
  if ((u32)(FIELD_CELL(Actor_GetById(ACTOR_VENT_ROOM_PILLAR)->x.fixed) - 15) >
      6)
    GameFlag_Set(FLAG_PILLAR_LIFTED_OFF_PLATFORM);
  GameFlag_Set(FLAG_PILLAR_MOVING);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

/// Sets the vent room's pillar down, and notes which platform, if either, it
/// weighs down.
void KandoreanTemple_FinishMovingPillar(void) {
  struct Actor *object = Actor_GetById(ACTOR_VENT_ROOM_PILLAR);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);

  Actor_PlaceMovedBlock();
  if (x == 13 && z == 13)
    GameFlag_Set(FLAG_PILLAR_ON_WEST_PLATFORM);
  else
    GameFlag_Clear(FLAG_PILLAR_ON_WEST_PLATFORM);
  if (x == 23 && z == 15)
    GameFlag_Set(FLAG_PILLAR_ON_EAST_PLATFORM);
  else
    GameFlag_Clear(FLAG_PILLAR_ON_EAST_PLATFORM);
  GameFlag_Clear(FLAG_PILLAR_LIFTED_OFF_PLATFORM);
  GameFlag_Clear(FLAG_PILLAR_MOVING);
}

/// Move lifts the pot of boiling water.
void KandoreanTemple_StartMovingPot(void) {
  GameFlag_Clear(FLAG_POT_PUSHED_WEST);
  GameFlag_Clear(FLAG_POT_PUSHED_EAST);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

/// Sets the pot down; chucked to the west or east of its place, it is out of
/// the way and the floor it covered cools.
void KandoreanTemple_FinishMovingPot(void) {
  s32 x = FIELD_CELL(Actor_GetById(ACTOR_BOILING_POT)->x.fixed);

  Actor_PlaceMovedBlock();
  if (x == 19) {
    GameFlag_Set(FLAG_POT_PUSHED_WEST);
    GameFlag_Set(FLAG_POT_MOVED);
    Map_CopyCellAttributeRect(0, 0, 3, 3, x, 28);
  } else if (x == 21) {
    GameFlag_Set(FLAG_POT_PUSHED_EAST);
    GameFlag_Set(FLAG_POT_MOVED);
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
}

/// Stepping onto the hot floor, the leader jumps back with a yelp.
void KandoreanTemple_TouchTheHotFloor(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Sound_Play(SE_WATER);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 10);
  leader->velocityY = toFIXED(6);
  wait(20);
  while (leader->velocityY != 0)
    wait(1);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  Event_End();
}

/// Opens the passage by the gate's well.
void KandoreanTemple_OpenTheWellPassage(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
  MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  wait(10);
  GameFlag_Set(FLAG_WELL_PASSAGE_OPEN);
  Event_End();
}

/// Whirlwind has blown the bush away: the passage behind it opens and Kraden
/// follows the leader in.
void KandoreanTemple_FindTheHiddenPassage(void) {
  struct Actor *object;

  MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  GameFlag_Set(FLAG_HIDDEN_PASSAGE_FOUND);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(ACTOR_FELIX, 89, 237);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_LaunchFromLeader(ACTOR_GATE_KRADEN, 16, 16, FACING_NORTHWEST);
  Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
  Event_Wait(20);
  Event_SetMessageId((s32)&MsgKandoreanTempleWatchfulEye);
  // "Fantastic work, Felix! Keep a watchful eye for any strange-looking..."
  Event_ShowMessageAndWait(ACTOR_GATE_KRADEN, 0);
  Event_Wait(30);
  Actor_SetAnimationById(ACTOR_GATE_KRADEN, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_GATE_KRADEN, 1.2);
  Actor_SetAnimationById(ACTOR_GATE_KRADEN, ANIM_WALK);
  object = Actor_GetById(ACTOR_FELIX);
  if (object)
    Actor_SetDestinationById(ACTOR_GATE_KRADEN, object->x.part[0].integer,
                             object->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
  Actor_SetPosition(ACTOR_GATE_KRADEN, 0, 0);
  Event_End();
}

/// Examining the Lash Pebble in Poi's hall: Poi gives the party Lash and tells
/// how to use it.
void KandoreanTemple_ReceiveLash(void) {
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.leaderActorId)->facing;
  struct Actor *object;
  struct FieldSprite *sprite;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_GetActor()->motionFlags = 0;
  ((void (*)())Actor_FaceDirectionById)(ACTOR_POI, FACING_EAST);
  Event_SetMessageId((s32)&MsgKandoreanTempleSecretOfLash);
  // "This is the power and the secret of our temple!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_FaceActor(gMainRoutineWork.leaderActorId, ACTOR_POI, 0);
  Actor_SetAnimation(ACTOR_POI, ANIM_NOD);
  // "If you are worthy, you will use it with ease!"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_SetAnimation(ACTOR_POI, ANIM_NOD);
  // "When you see a rope, use Lash on it..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_SetAnimation(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "Perhaps you will find this useful on your travels."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_POI, ANIM_NOD);
  // "Take this power with you, and do good."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, (u16)angle, 10);
  if (gKandoreanTemple_LashPebble != 0)
    ItemDisplay_ShowPicture(gKandoreanTemple_LashPebble, 3);
  Party_GiveItem(ITEM_LASH_PEBBLE, 0);
  if (gKandoreanTemple_LashPebble != 0)
    Actor_Release(gKandoreanTemple_LashPebble);
  Actor_SetAnimation(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "I hope that someday, my students will be worthy of this honor, too."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(424, 0, 400, 1);
  object = Actor_GetById(ACTOR_POI);
  sprite = object->sprite;
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 424, 376);
  FieldActor_SetSpritePriority(ACTOR_POI, 1);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  FieldActor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 444, 376);
  Actor_SetAnimation(ACTOR_POI, 7);
  object->scaleX = -FIXED_ONE;
  sprite->rotation = TRIG_TURN * 7 / 8;
  object->motionFlags = 0;
  object->y.fixed += toFIXED(4);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpeed(ACTOR_POI, 0.4);
  FieldActor_SetMoveTarget(object, 492, 8, 376);
  FieldActor_WaitForMoveById(ACTOR_POI);
  object->motionFlags = 3;
  object->groundHeight = 0;
  object->scaleX = FIXED_ONE;
  sprite->rotation = 0;
  Actor_SetAnimation(ACTOR_POI, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 1);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  FieldActor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 504, 376);
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 496, 394);
  Actor_WalkToAndWait(ACTOR_POI, 432, 394);
  FieldActor_WatchAndSetScript(ACTOR_POI, gMainRoutineWork.leaderActorId);
  FieldActor_SetSpritePriority(ACTOR_POI, 2);
  Event_SetFlag(FLAG_LASH_RECEIVED);
  Event_End();
}

/// Stepping behind a rope's pole, the leader is drawn behind it.
void KandoreanTemple_StepBehindThePole(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
}

/// Stepping out from behind a rope's pole, the leader is drawn in front again.
void KandoreanTemple_StepOutFromThePole(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
}

/// Coming up to the Chestbeaters, the three leap down and their battle starts.
void KandoreanTemple_FightChestbeaters(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 174, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 174);
  Actor_RunRepeatedMotion(ACTOR_FIRST_CHESTBEATER, 2);
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetAnimationById(ACTOR_FIRST_CHESTBEATER, 2);
  Actor_GetById(ACTOR_FIRST_CHESTBEATER)->velocityY = toFIXED(4);
  Event_Wait(10);
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetAnimationById(ACTOR_WEST_CHESTBEATER, 2);
  Actor_SetAnimationById(ACTOR_EAST_CHESTBEATER, 2);
  Actor_GetById(ACTOR_WEST_CHESTBEATER)->velocityY = toFIXED(6);
  Actor_GetById(ACTOR_EAST_CHESTBEATER)->velocityY = toFIXED(6);
  Actor_SetDestinationById(ACTOR_WEST_CHESTBEATER, 300, 164);
  Actor_MoveToAndWaitById(ACTOR_EAST_CHESTBEATER, 356, 164);
  Event_Wait(40);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Event_SetAfterBattleDestination(SCENE_KANDOREAN_TEMPLE_CHESTBEATERS, 3);
  Field_RequestBattleFromEncounterTable(9, 4);
  Event_End();
}

/// Cyclone blows away the first of five patches by the gate.
void KandoreanTemple_CycloneFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 2, 2, 73, 14, 1);
}

/// Cyclone blows away the second of five patches by the gate.
void KandoreanTemple_CycloneSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 75, 16, 1);
}

/// Cyclone blows away the third of five patches by the gate.
void KandoreanTemple_CycloneThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 74, 18, 1);
}

/// Cyclone blows away the fourth of five patches by the gate.
void KandoreanTemple_CycloneFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 80, 18, 1);
}

/// Cyclone blows away the fifth of five patches by the gate.
void KandoreanTemple_CycloneFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 87, 15, 1);
}

/// Sets up the room being entered.
s32 Scene_Initialize(void) {
  s32 scene;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
    KandoreanTemple_StartLiftRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_MIMIC)
    KandoreanTemple_StartMimicRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
    KandoreanTemple_StartChestbeaterRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
    KandoreanTemple_StartVentRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_STREAM)
    KandoreanTemple_StartStreamRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_STEAM)
    KandoreanTemple_StartSteamRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_FOG)
    KandoreanTemple_StartFogRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_GATE)
    KandoreanTemple_StartGate();
  else if (scene == SCENE_KANDOREAN_TEMPLE_POI)
    KandoreanTemple_StartPoisHall();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the lift room: its tightropes, the pillar, the stream of steam and
/// the lifts, and the lift the leader arrives on.
void KandoreanTemple_StartLiftRoom(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_FIRST_TIGHTROPE_POLE,
                  ACTOR_FIRST_TIGHTROPE_OTHER_POLE);
  Tightrope_Place(1, ACTOR_SECOND_TIGHTROPE_POLE,
                  ACTOR_SECOND_TIGHTROPE_OTHER_POLE);
  Actor_SetAnimationById(ACTOR_FIRST_TIGHTROPE_POLE, 4);
  Actor_SetAnimationById(ACTOR_FIRST_TIGHTROPE_OTHER_POLE, 4);
  Actor_SetAnimationById(ACTOR_SECOND_TIGHTROPE_POLE, 4);
  Actor_SetAnimationById(ACTOR_SECOND_TIGHTROPE_OTHER_POLE, 4);
  Actor_PlaceBlocks(gKandoreanTemple_BlockActors);
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Scene_QueueMusic(SE_AMBIENT_STREAM);
  Task_Add(KandoreanTemple_BlowStream, TASK_PASS_VBLANK);
  Actor_SetPosition(ACTOR_WEST_LIFT, 0, 0);
  Actor_SetPosition(ACTOR_EAST_LIFT, 0, 0);
  Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
  Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
  KandoreanTemple_PrepareLift(ACTOR_WEST_LIFT);
  KandoreanTemple_PrepareLift(ACTOR_EAST_LIFT);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_LIFT_COLUMN);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_LIFT_COLUMN);
  gKandoreanTemple_LeftTimer = 0;
  gKandoreanTemple_RightTimer = 0;
  gKandoreanTemple_LeftBobPhase = 0;
  gKandoreanTemple_RightBobPhase = 0;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance == 5)
      KandoreanTemple_ArriveOnWestLift();
    else if (gMainRoutineWork.entrance == 6)
      KandoreanTemple_ArriveOnEastLift();
  }
  if (GameFlag_Get(FLAG_PILLAR_ON_EAST_PLATFORM))
    Task_Add(KandoreanTemple_RunWestLift, TASK_PASS_VBLANK);
  else if (GameFlag_Get(FLAG_PILLAR_ON_WEST_PLATFORM))
    Task_Add(KandoreanTemple_RunEastLift, TASK_PASS_VBLANK);
  Task_Add(KandoreanTemple_WatchLifts, TASK_PASS_VBLANK);
  FieldEffect_StartBlankOverlay(0);
}

/// Sets up the second room.
void KandoreanTemple_StartMimicRoom(void) {
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 1)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the Chestbeaters' room: the pot where Move left it, the sign, and
/// the Chestbeaters gone once the ladder is lowered.
void KandoreanTemple_StartChestbeaterRoom(void) {
  struct Actor *object = Actor_GetById(ACTOR_SECOND_BOILING_POT);

  Actor_DisableById(ACTOR_SECOND_BOILING_POT);
  object->priorityFlags |= 2;
  Actor_SetAnimationById(ACTOR_SECOND_BOILING_POT, 2);
  object = Actor_GetById(ACTOR_BOILING_POT);
  object->motionFlags = 0;
  if (GameFlag_Get(FLAG_POT_PUSHED_WEST)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_BOILING_POT),
                                         toFIXED(312), toFIXED(32),
                                         toFIXED(472));
    MapGrid_CopyCellAttributes(0, 0, 3, 3, 19, 28);
  } else if (GameFlag_Get(FLAG_POT_PUSHED_EAST)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_BOILING_POT),
                                         toFIXED(344), toFIXED(32),
                                         toFIXED(472));
    MapGrid_CopyCellAttributes(0, 4, 3, 3, 19, 28);
  }
  Actor_PlaceBlocks(gKandoreanTemple_PillarActors);
  object = Actor_GetById(ACTOR_POIS_SIGN);
  object->scaleX = FIXED_ONE * 7 / 10;
  object->scaleY = FIXED_ONE * 7 / 10;
  object->collisionFlags = 0;
  FieldEffect_StartBlankOverlay(0);
  if (Event_GetFlag(FLAG_LADDER_LOWERED)) {
    {
      s32 e = 1;
      s32 f = 4;

      MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, e, f);
    }
    Actor_SetPosition(ACTOR_FIRST_CHESTBEATER, 0, 0);
    Actor_SetPosition(ACTOR_WEST_CHESTBEATER, 0, 0);
    Actor_SetPosition(ACTOR_EAST_CHESTBEATER, 0, 0);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance == 2) {
      Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed = toFIXED(64);
    } else if (gMainRoutineWork.entrance == 3 &&
               Event_GetFlag(FLAG_LADDER_LOWERED) == 0) {
      Actor_SetPosition(ACTOR_FIRST_CHESTBEATER, 0, 0);
      Actor_SetPosition(ACTOR_WEST_CHESTBEATER, 0, 0);
      Actor_SetPosition(ACTOR_EAST_CHESTBEATER, 0, 0);
      KandoreanTemple_LowerTheLadder();
    }
  }
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 2)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the vent room: the platforms and columns at rest, the pillar where
/// Move left it, the vents' steam and the tasks that run them, and the ride
/// down or the fall the leader arrives by.
void KandoreanTemple_StartVentRoom(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_PLATFORM),
                                         toFIXED(216), 0, toFIXED(216));
    Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_PLATFORM),
                                    toFIXED(376), 0, toFIXED(248));
    Actor_SetPosition(ACTOR_EAST_COLUMN_TOP, 0, 0);
    Actor_SetPosition(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_EAST_COLUMN_FOOT, 0, 0);
    GameFlag_Clear(FLAG_VENT_SOUND_PLAYING);
    GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
    GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
  }
  if (GameFlag_Get(FLAG_PILLAR_ON_WEST_PLATFORM))
    Actor_SetPosition(ACTOR_VENT_ROOM_PILLAR, 216, 216);
  else if (GameFlag_Get(FLAG_PILLAR_ON_EAST_PLATFORM))
    Actor_SetPosition(ACTOR_VENT_ROOM_PILLAR, 376, 248);
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  gKandoreanTemple_LeftVentBlowing = 0;
  gKandoreanTemple_RightVentBlowing = 0;
  Task_AddFrame(KandoreanTemple_BlowLeftVent, TASK_PASS_VBLANK);
  Task_AddFrame(KandoreanTemple_BlowRightVent, TASK_PASS_VBLANK);
  Task_Add(KandoreanTemple_DropOffLowCells, TASK_PASS_VBLANK);
  KandoreanTemple_PrepareLift(ACTOR_WEST_PLATFORM);
  KandoreanTemple_PrepareLift(ACTOR_EAST_PLATFORM);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_COLUMN_TOP);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_COLUMN_UPPER_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_COLUMN_MIDDLE_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_COLUMN_LOWER_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_WEST_COLUMN_FOOT);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_COLUMN_TOP);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_COLUMN_UPPER_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_COLUMN_MIDDLE_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_COLUMN_LOWER_PIECE);
  KandoreanTemple_PrepareLiftBase(ACTOR_EAST_COLUMN_FOOT);
  Actor_PlaceBlocks(gKandoreanTemple_PillarActors);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (gMainRoutineWork.entrance == 3)
      KandoreanTemple_RiseOnLeftVent();
    else if (gMainRoutineWork.entrance == 4)
      KandoreanTemple_RiseOnRightVent();
  }
  gKandoreanTemple_LeftTimer = 0;
  gKandoreanTemple_RightTimer = 4000;
  gKandoreanTemple_LeftBobPhase = 0;
  gKandoreanTemple_RightBobPhase = 0;
  Task_AddFrame(KandoreanTemple_RunLeftVent, TASK_PASS_VBLANK);
  Task_Add(KandoreanTemple_RunRightVent, TASK_PASS_VBLANK);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && gMainRoutineWork.entrance == 5)
    KandoreanTemple_LandFromAbove();
  Task_AddFrame(KandoreanTemple_WatchVentPlatforms, TASK_PASS_VBLANK);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  FieldEffect_StartBlankOverlay(0);
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KANDOREAN_TEMPLE_VENT
#undef BOB
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
// The Kandorean Temple's events, from the vents' rides to Poi's test, and its
// tables.

extern s32 gKandoreanTemple_RightTimer;

/// The leader drops into the vent room from above and lands in a puff of dust
/// that shakes the screen.
void KandoreanTemple_LandFromAbove(void) {
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
  leader->motionFlags = 3;
  Event_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (i = 0; i <= 16; i++) {
    s32 velocity[3];

    velocity[0] = Trig_Cos(i << 12);
    velocity[1] = 0;
    velocity[2] = Trig_Sin(i << 12);
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -0x6000;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -0x3000;
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
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

/// Arriving from above by the west vent: the platform rises to meet the leader
/// and sinks with it to the ground.
void KandoreanTemple_RiseOnLeftVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *statue = Actor_GetById(ACTOR_WEST_PLATFORM);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_Set(FLAG_ON_WEST_LIFT);
  GameFlag_Clear(FLAG_ON_EAST_LIFT);
  GameFlag_Set(FLAG_PILLAR_ON_EAST_PLATFORM);
  GameFlag_Clear(FLAG_PILLAR_ON_WEST_PLATFORM);
  Sound_Play(SE_WAVE);
  KandoreanTemple_StartLeftVent();
  i = 0;
  do {
    leader->y.fixed += toFIXED(9) / 2;
    statue->y.fixed += toFIXED(9) / 2;
    ride->y.fixed += toFIXED(9) / 5;
    limit = 0x1000;
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, 0x20000, limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, 0x18000, limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0x10000, limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_LOWER_PIECE, 32768, limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
#define SINK_PLATFORM(top, upper, middle, lower, foot)                         \
  do {                                                                         \
    leader->y.fixed += -toFIXED(27) / 4;                                       \
    statue->y.fixed += -toFIXED(27) / 4;                                       \
    ride->y.fixed += -toFIXED(27) / 10;                                        \
    limit = 0x1800;                                                            \
    KandoreanTemple_ShrinkColumn(top, 0x30000, limit);                         \
    KandoreanTemple_ShrinkColumn(upper, 0x24000, limit);                       \
    KandoreanTemple_ShrinkColumn(middle, 0x18000, limit);                      \
    KandoreanTemple_ShrinkColumn(lower, 49152, limit);                         \
    KandoreanTemple_ShrinkColumn(foot, 0, limit);                              \
    wait(1);                                                                   \
  } while (Actor_GetById(foot)->scaleY > limit);

  SINK_PLATFORM(ACTOR_WEST_COLUMN_TOP, ACTOR_WEST_COLUMN_UPPER_PIECE,
                ACTOR_WEST_COLUMN_MIDDLE_PIECE, ACTOR_WEST_COLUMN_LOWER_PIECE,
                ACTOR_WEST_COLUMN_FOOT)
  gKandoreanTemple_LeftVentBlowing = 0;
  GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
  Actor_SetAnimationById(ACTOR_WEST_PLATFORM, 1);
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_PLATFORM),
                                       toFIXED(216), 0, toFIXED(216));
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(ACTOR_WEST_PLATFORM, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0, leader->z.fixed);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

/// Arriving from above by the east vent: the platform rises to meet the leader
/// and sinks with it to the ground.
void KandoreanTemple_RiseOnRightVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *statue = Actor_GetById(ACTOR_EAST_PLATFORM);
  struct Actor *ride = FieldCamera_GetActor();
  s16 i;
  u16 next;
  s32 limit;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  leader->motionFlags = 0;
  wait(1);
  GameFlag_Clear(FLAG_ON_WEST_LIFT);
  GameFlag_Set(FLAG_ON_EAST_LIFT);
  GameFlag_Clear(FLAG_PILLAR_ON_EAST_PLATFORM);
  GameFlag_Set(FLAG_PILLAR_ON_WEST_PLATFORM);
  Sound_Play(SE_WAVE);
  KandoreanTemple_StartRightVent();
  i = 0;
  do {
    leader->y.fixed += toFIXED(9) / 2;
    statue->y.fixed += toFIXED(9) / 2;
    ride->y.fixed += toFIXED(9) / 5;
    limit = 0x1000;
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, 0x20000, limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, 0x18000, limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0x10000, limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_LOWER_PIECE, 32768, limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next <= 15);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  SINK_PLATFORM(ACTOR_EAST_COLUMN_TOP, ACTOR_EAST_COLUMN_UPPER_PIECE,
                ACTOR_EAST_COLUMN_MIDDLE_PIECE, ACTOR_EAST_COLUMN_LOWER_PIECE,
                ACTOR_EAST_COLUMN_FOOT)
  gKandoreanTemple_RightVentBlowing = 0;
  GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_EAST_COLUMN_TOP, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_FOOT, 0, 0);
  Actor_SetAnimationById(ACTOR_EAST_PLATFORM, 1);
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_PLATFORM),
                                       toFIXED(376), 0, toFIXED(248));
  statue->priorityFlags |= 2;
  Actor_SetSpritePriority(ACTOR_EAST_PLATFORM, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0, leader->z.fixed);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 1);
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Event_End();
}

/// The leader rises out of the room on the steam.
void KandoreanTemple_LeaveBySteam(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Rises on the west vent's steam to the room above.
void KandoreanTemple_LeaveByWestVent(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(3);
}

/// Rises on the east vent's steam to the room above.
void KandoreanTemple_LeaveByEastVent(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(4);
}

/// The leader slips into the lift room's shaft of steam and is carried away.
void KandoreanTemple_SinkIntoTheSteam(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 i;
  struct MainRoutineWork *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  object->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  object->facing = 0;
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 16);
  for (i = 0; i <= 7; i++) {
    Actor_SetPositionAndResetMotion(object, object->x.fixed, object->y.fixed,
                                    object->z.fixed + toFIXED(3));
    wait(1);
  }
  Event_Wait(20);
  party = &gMainRoutineWork;
  object->facing = FACING_SOUTH;
  FieldEffect_SetAttached(party->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimationById(party->leaderActorId, 27);
  Event_Wait(30);
  Actor_SetAnimationById(party->leaderActorId, 28);
  Event_Wait(10);
  Actor_SetSpritePriority(party->leaderActorId, 3);
  for (i = 0; i <= 39; i++) {
    {
      s32 scale = object->scaleX;

      object->scaleX = scale + -0x400;
    }
    object->scaleY += -0x400;
    object->y.fixed += -toFIXED(2);
    wait(1);
    if (i == 10)
      Sound_Play(SE_DUST_PUFF);
    if (i == 20) {
      gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
      Event_FinishScreenTransition();
    }
  }
  Import_EventSetExitRequest(5);
}

/// The leader rises into view on a lift while its column grows under it.
#define DEFINE_KANDOREAN_TEMPLE_RISE(name, liftId, columnId, x, z, liftZ,      \
                                     cellX, cellZ, timer)                      \
  void name(void) {                                                            \
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);      \
    struct Actor *first = Actor_GetById(liftId);                               \
    struct Actor *second = Actor_GetById(columnId);                            \
                                                                               \
    Event_Start();                                                             \
    Event_ResolvePendingActions(0);                                            \
    Scene_QueueMusic(SE_WAVE);                                                 \
    FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);              \
    wait(1);                                                                   \
    leader->motionFlags = 0;                                                   \
    FieldActor_SetPositionAndResetMotion(                                      \
        Actor_GetById(gMainRoutineWork.leaderActorId), toFIXED(x),             \
        toFIXED(-20), toFIXED(z));                                             \
    Actor_SetSingleChildFlags_(Actor_GetById(gMainRoutineWork.leaderActorId),  \
                               0);                                             \
    FieldActor_SetPositionAndResetMotion(Actor_GetById(liftId), toFIXED(x),    \
                                         toFIXED(-20), toFIXED(liftZ));        \
    FieldActor_SetPositionAndResetMotion(Actor_GetById(columnId), toFIXED(x),  \
                                         toFIXED(-20), toFIXED(z));            \
    Actor_SetSpritePriority(liftId, 2);                                        \
    Actor_SetSpritePriority(columnId, 3);                                      \
    {                                                                          \
      u8 *flags = &second->priorityFlags;                                      \
      s32 c = 2 | *flags;                                                      \
                                                                               \
      *flags = c;                                                              \
    }                                                                          \
    second->scaleY = FIXED_ONE / 64;                                           \
    Actor_SetAnimationById(liftId, 4);                                         \
    Actor_SetAnimationById(columnId, 2);                                       \
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);           \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
  rise:                                                                        \
    first->y.fixed += toFIXED(4);                                              \
    leader->y.fixed += toFIXED(4);                                             \
    KandoreanTemple_GrowColumn(columnId, 19660, 0x4ccc);                       \
    wait(1);                                                                   \
    if (first->y.fixed < 0)                                                    \
      goto rise;                                                               \
    MapGrid_CopyCellAttributes(0, 0, 1, 1, cellX, cellZ);                      \
    timer = 2000;                                                              \
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);           \
    Event_End();                                                               \
  }

DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnWestLift, ACTOR_WEST_LIFT,
                             ACTOR_WEST_LIFT_COLUMN, 440, 200, 204, 27, 12,
                             gKandoreanTemple_LeftTimer)

// As KandoreanTemple_ArriveOnWestLift, for the east lift.
DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnEastLift, ACTOR_EAST_LIFT,
                             ACTOR_EAST_LIFT_COLUMN, 600, 232, 236, 37, 14,
                             gKandoreanTemple_RightTimer)

/// The west lift rises out of view on its growing column, then the scene moves
/// on.
void KandoreanTemple_RideWestLiftUp(void) {
  struct Actor *object = Actor_GetById(ACTOR_WEST_LIFT);

  Event_Start();
  Event_ResolvePendingActions(0);
  do {
    object->y.fixed -= toFIXED(4);
    KandoreanTemple_ShrinkColumn(ACTOR_WEST_LIFT_COLUMN, 19660, 0x4ccc);
    wait(1);
  } while (object->y.fixed > -toFIXED(20));
  Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(6);
}

/// As KandoreanTemple_RideWestLiftUp, for actors 14 and 16.
void KandoreanTemple_RideEastLiftUp(void) {
  struct Actor *object = Actor_GetById(ACTOR_EAST_LIFT);

  Event_Start();
  Event_ResolvePendingActions(0);
  do {
    object->y.fixed -= toFIXED(4);
    KandoreanTemple_ShrinkColumn(ACTOR_EAST_LIFT_COLUMN, 19660, 0x4ccc);
    wait(1);
  } while (object->y.fixed > -toFIXED(20));
  Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(7);
}

/// Sets up the stream room, where the party wades and the current of steam
/// pushes it.
void KandoreanTemple_StartStreamRoom(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;

  gKandoreanTemple_SteamCounter = 0;
  map->wading = 1;
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the steam room, with the rush of its steam.
void KandoreanTemple_StartSteamRoom(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;

  gKandoreanTemple_SteamCounter = 0;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  map->wading = 1;
  FieldEffect_StartBlankOverlay(0);
}

/// Sets up Fog's room: the rope to Fog, and the student who gave up levitating
/// until the party has met Poi.
void KandoreanTemple_StartFogRoom(void) {
  Rope_Initialize();
  Rope_Place(0, FLAG_FOG_ROPE_TIED, ACTOR_FOG_ROPE_POLE,
             ACTOR_FOG_ROPE_OTHER_POLE);
  if (GameFlag_Get(FLAG_MET_POI)) {
    Actor_SetPosition(ACTOR_STUDENT_WHO_GAVE_UP, 0, 0);
  } else {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_STUDENT_WHO_GAVE_UP), 0);
    Actor_SetAnimationById(ACTOR_STUDENT_WHO_GAVE_UP, 6);
    Actor_SetMovementTypeById(ACTOR_STUDENT_WHO_GAVE_UP,
                              gKandoreanTemple_LevitateScript);
  }
  FieldEffect_StartBlankOverlay(0);
}

/// A student sits meditating, his first ellipsis after base frames and up to
/// 400 more.
void KandoreanTemple_StartMeditating(s32 id, s32 base) {
  struct Actor *object = Actor_GetById(id);

  Actor_SetAnimationById(id, 5);
  object->timer = base + (Random_Below(400));
  object->userValue = id;
  Actor_SetMovementTypeById(id, gKandoreanTemple_MeditateScript);
}

/// Sets up the temple gate: the students meditating or standing by, the opened
/// well passage, and Kraden.
void KandoreanTemple_StartGate(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_HIDDEN_PASSAGE_FOUND)) {
    Actor_SetPosition(ACTOR_BUSH_OVER_THE_CAVE, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  } else {
    Actor_DisableById(ACTOR_BUSH_OVER_THE_CAVE);
  }
  if (GameFlag_Get(FLAG_MET_POI)) {
    MapGrid_CopyCellAttributes(2, 0, 2, 3, 17, 24);
    Actor_SetPosition(ACTOR_STUDENT_ASKING_ABOUT_POI, 0, 0);
    Actor_SetPosition(ACTOR_STUDENT_ON_GUARD_DUTY, 0, 0);
    Actor_SetPositionAndFacing(ACTOR_STUDENT_WARY_OF_OUTSIDERS, toFIXED(374),
                               toFIXED(282), FACING_EAST);
    {
      s32 angle = FACING_NORTH_NORTHEAST;

      Actor_SetPositionAndFacing(ACTOR_STUDENT_ON_THE_FAILED_TEST, toFIXED(272),
                                 toFIXED(296), angle);
    }
    {
      s32 angle = FACING_SOUTH_SOUTHEAST;

      Actor_SetPositionAndFacing(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
                                 toFIXED(216), toFIXED(264), angle);
    }
  } else {
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_WARY_OF_OUTSIDERS, 0);
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_ON_THE_FAILED_TEST, 240);
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, 480);
  }
  if (GameFlag_Get(FLAG_WELL_PASSAGE_OPEN)) {
    MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
    MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 4)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Before the party has Lash, Poi waits by the north rope, which he has tied
/// across (FLAG_NORTH_ROPE_TIED) the first time on each visit.
void KandoreanTemple_PoiWaitsByTheNorthRope(void) {
  if (Event_GetFlag(FLAG_NORTH_ROPE_TIED) == 0) {
    Event_SetFlag(FLAG_NORTH_ROPE_TIED);
    Call4(Rope_Place, 1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POLE,
          ACTOR_NORTH_ROPE_WEST_POLE);
  }
  {
    s32 angle = FACING_SOUTH_SOUTHEAST;

    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(392), toFIXED(360), angle);
  }
}

/// Sets up Poi's hall: its ropes, the tested students, Poi and the trapdoor, by
/// the entrance the party takes.
void KandoreanTemple_StartPoisHall(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Rope_Initialize();
  Call4(Rope_Place, 0, FLAG_SOUTH_ROPE_TIED, ACTOR_SOUTH_ROPE_WEST_POLE,
        ACTOR_SOUTH_ROPE_EAST_POLE);
  Rope_Place(1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POLE,
             ACTOR_NORTH_ROPE_WEST_POLE);
  if (Event_GetFlag(FLAG_MET_POI) == 0) {
    Actor_SetAnimationById(ACTOR_FIRST_TESTED_STUDENT, 5);
    Actor_SetAnimationById(ACTOR_THIRD_TESTED_STUDENT, 5);
  }
  Actor_SetAnimationById(ACTOR_SECOND_TESTED_STUDENT, 5);
  Task_Add(KandoreanTemple_MatchLiftPriority, TASK_PASS_VBLANK);
  if ((gMainRoutineWork.entrance >= 4 && gMainRoutineWork.entrance <= 6) ||
      gMainRoutineWork.entrance == ENTRANCE_THROUGH_THE_CAVE) {
    if (GameFlag_Get(FLAG_LASH_RECEIVED)) {
      s32 angle = FACING_SOUTH_SOUTHWEST;

      Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(432), toFIXED(394), angle);
    } else {
      KandoreanTemple_CreateLashPebble();
      if (GameFlag_Get(FLAG_MET_POI))
        KandoreanTemple_PoiWaitsByTheNorthRope();
    }
  }
  if ((gMainRoutineWork.entrance >= 1 && gMainRoutineWork.entrance <= 3) ||
      gMainRoutineWork.entrance == 9) {
    if (GameFlag_Get(FLAG_TRAPDOOR_OPEN))
      KandoreanTemple_OpenTheTrapdoor();
    if (GameFlag_Get(FLAG_MET_POI)) {
      Actor_SetPosition(ACTOR_POI, 0, 0);
      {
        s32 facing = FACING_NORTH_NORTHWEST;

        Actor_SetPositionAndFacing(ACTOR_THIRD_TESTED_STUDENT, toFIXED(408),
                                   toFIXED(664), facing);
      }
    } else if (GameFlag_Get(FLAG_STUDENTS_TESTED)) {
      Actor_SetPosition(ACTOR_POI, 0, 0);
      Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
    } else {
      KandoreanTemple_TestTheStudents();
    }
  } else if (gMainRoutineWork.entrance == 6) {
    if (GameFlag_Get(FLAG_MET_POI) == 0)
      KandoreanTemple_PraiseTheLevitator();
  } else if (gMainRoutineWork.entrance == ENTRANCE_ROPE_ON_THE_FLOOR) {
    KandoreanTemple_FindTheRopeOnTheFloor();
  } else if (gMainRoutineWork.entrance == ENTRANCE_THROUGH_THE_CAVE) {
    if (GameFlag_Get(FLAG_MET_POI) == 0)
      KandoreanTemple_MeetPoi();
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance >= 8 && gMainRoutineWork.entrance <= 9)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Turns a sprite by step each frame for the given frames.
void KandoreanTemple_Wobble(struct FieldSprite *sprite, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    Event_Wait(1);
  }
}

/// Turns an actor's sprite by step each frame for the given frames.
void KandoreanTemple_Sway(struct Actor *object, u32 frames, s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    object->x.fixed += step;
    Event_Wait(1);
  }
}

/// Poi tests his three students in turn: the first two fall, and the third
/// levitates and passes.
void KandoreanTemple_TestTheStudents(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u32 i;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 400, 796);
  wait(1);
  Event_StartScreenTransition();
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 400, 752);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(436, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_Jump(ACTOR_POI, 4, 0);
  Event_SetMessageId((s32)&MsgKandoreanTempleFocusYourMind);
  // "You there! Focus your mind!"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  FieldCamera_MoveTo(456, 0, 688, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_FIRST_TESTED_STUDENT, 2);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_TESTED_STUDENT, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_TESTED_STUDENT, 0);
  Event_Wait(20);
  object = Actor_GetById(ACTOR_FIRST_TESTED_STUDENT);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
  do {
    object->y.fixed += 0xccc;
    Event_Wait(1);
    i++;
  } while (i <= 63);
  KandoreanTemple_Wobble(sprite, 32, 32);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_FIRST_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  KandoreanTemple_Wobble(sprite, 32, -64);
  KandoreanTemple_Wobble(sprite, 32, 64);
  KandoreanTemple_Wobble(sprite, 64, 32);
  FieldEffect_SetAttached(ACTOR_FIRST_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(SE_DOOR_CLOSE);
  object->velocityY = toFIXED(3);
  sprite->rotation = 0;
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_FIRST_TESTED_STUDENT, 0);
  FieldCamera_MoveTo(436, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 476, 664);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POI, 0);
  Actor_WalkToAndWait(ACTOR_POI, 474, 664);
  Actor_WalkToAndWait(ACTOR_POI, 472, 648);
  Actor_SetPosition(ACTOR_POI, 0, 0);
  FieldCamera_SetSpeed(1.2);
  FieldCamera_MoveTo(364, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_SetPosition(ACTOR_POI, 328, 648);
  wait(1);
  Actor_WalkToAndWait(ACTOR_POI, 326, 664);
  Actor_WalkToAndWait(ACTOR_POI, 320, 664);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 0);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(342, 0, 688, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_SECOND_TESTED_STUDENT, 2);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_ConfigureSpriteColor(ACTOR_SECOND_TESTED_STUDENT, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_SECOND_TESTED_STUDENT, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  object = Actor_GetById(ACTOR_SECOND_TESTED_STUDENT);
  object->motionFlags = 0;
  i = 0;
  do {
    object->y.fixed += toFIXED(3) / 20;
    Event_Wait(1);
    i++;
  } while (i <= 63);
  KandoreanTemple_Sway(object, 64, 4096);
  KandoreanTemple_Sway(object, 128, -4096);
  KandoreanTemple_Sway(object, 128, 4096);
  KandoreanTemple_Sway(object, 64, -4096);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  object->motionFlags = 3;
  Event_Wait(5);
  Sound_Play(SE_DOOR_CLOSE);
  object->velocityY = toFIXED(3);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_SECOND_TESTED_STUDENT, 0);
  FieldCamera_MoveTo(364, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 10);
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  FieldCamera_MoveTo(400, 0, 688, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 2);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_ConfigureSpriteColor(ACTOR_THIRD_TESTED_STUDENT, 0x100);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_THIRD_TESTED_STUDENT, 0);
  Event_Wait(20);
  object = Actor_GetById(ACTOR_THIRD_TESTED_STUDENT);
  sprite = object->sprite;
  object->motionFlags = 0;
  i = 0;
  do {
    object->y.fixed += toFIXED(1) / 5;
    Event_Wait(1);
    i++;
  } while (i <= 63);
  KandoreanTemple_Wobble(sprite, 32, 64);
  sprite->rotation = 0;
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
  Event_Wait(60);
  KandoreanTemple_Wobble(sprite, 32, -64);
  sprite->rotation = 0;
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
  Event_Wait(60);
  i = 0;
  do {
    object->y.fixed += -toFIXED(1) / 5;
    Event_Wait(1);
    i++;
  } while (i <= 63);
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_THIRD_TESTED_STUDENT, EMOTE_IN_FRONT | EMOTE_SMILE,
                      0);
  Event_ShowMessageThenWait(ACTOR_THIRD_TESTED_STUDENT, 0, 20);
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_THIRD_TESTED_STUDENT,
                      EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_SetAnimationById(ACTOR_THIRD_TESTED_STUDENT, ANIM_STAND);
  Actor_Jump(ACTOR_THIRD_TESTED_STUDENT, 4, 0);
  Actor_FaceDirection(ACTOR_THIRD_TESTED_STUDENT, FACING_NORTH_NORTHWEST, 20);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_THIRD_TESTED_STUDENT,
                           0);
  FieldCamera_MoveTo(364, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Event_ShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Event_ShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_WalkToAndWait(ACTOR_POI, 326, 664);
  Actor_WalkToAndWaitById(ACTOR_POI, 328, 648);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_POI), 15);
  Actor_SetPosition(ACTOR_POI, 440, 600);
  FieldCamera_MoveTo(400, 0, 688, 1);
  Actor_SetSpeed(ACTOR_THIRD_TESTED_STUDENT, 0.8);
  Actor_WalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 644);
  Event_Wait(20);
  Sound_Play(SE_THUMP);
  KandoreanTemple_OpenTheTrapdoor();
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Task_Remove(KandoreanTemple_MatchLiftPriority);
  Actor_SetAnimationAndWait(ACTOR_THIRD_TESTED_STUDENT, ANIM_NOD);
  Actor_SetSpritePriority(ACTOR_THIRD_TESTED_STUDENT, 3);
  Actor_WalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 640);
  Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
  Actor_SetPosition(ACTOR_POI, 0, 0);
  FieldCamera_MoveTo(400, 0, 752, 1);
  FieldCamera_WaitForArrival();
  Task_Add(KandoreanTemple_MatchLiftPriority, TASK_PASS_VBLANK);
  Event_End();
  GameFlag_Set(FLAG_STUDENTS_TESTED);
}

/// Poi praises the student who passed, then sees the party and asks how they
/// came so far.
void KandoreanTemple_PraiseTheLevitator(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 328, 360);
  {
    s32 facing = FACING_SOUTH_SOUTHWEST;

    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(472), toFIXED(408), facing);
  }
  Actor_GetById(ACTOR_HALL_KRADEN)->facing = FACING_NORTH;
  wait(1);
  Event_StartScreenTransition();
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 328, 392);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(408, CAMERA_KEEP, 400, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 376, 408);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_POI, 1);
  Event_SetMessageId((s32)&MsgKandoreanTempleTestPassed);
  // "You have passed a very difficult test. You have pleased your teacher!"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 20);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_WalkToAndWait(ACTOR_POI, 456, 408);
  // "Wait... You're not one of my students! Who are you!?"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_MoveToObject(ACTOR_HALL_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_HALL_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_HALL_KRADEN, 376, 392);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 20);
  Actor_SetAttachedEffect(ACTOR_POI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "How did you... come this far?"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 20);
  // "Ah, yes... How indeed?"
  Event_ShowMessageAndWait(ACTOR_HALL_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_POI, 468, 428);
  Actor_WalkToAndWait(ACTOR_POI, 472, 454);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(13);
}

/// Finding the rope still lying on the floor.
void KandoreanTemple_FindTheRopeOnTheFloor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_POI, 0, 0);
  Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Actor_SetPosition(ACTOR_POI, 472, 648);
  wait(1);
  Actor_WalkToAndWait(ACTOR_POI, 472, 664);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 20);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(456, 40, 664, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(40);
  Event_SetMessageId((s32)&MsgKandoreanTempleRopeOnTheFloor);
  // "The rope is still on the floor."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_WalkToAndWait(ACTOR_POI, 472, 648);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(14);
}

/// Poi walks up to the north rope and speaks from above.
void KandoreanTemple_PoiWalksUp(void) {
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_WalkToAndWait(ACTOR_POI, 496, 392);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
}

#define WORK gFieldWork

/// Poi meets the party who came through the cave, and must give them the secret
/// of Lash.
void KandoreanTemple_MeetPoi(void) {
  struct Actor *object;
  struct FieldSprite *sprite;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetPositionAndFacing(ACTOR_HALL_KRADEN, toFIXED(376), toFIXED(392),
                             FACING_EAST);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.leaderActorId, toFIXED(376), toFIXED(408), FACING_EAST);
  {
    s32 angle = FACING_NORTH_NORTHWEST;
    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(472), toFIXED(454), angle);
  }
  WORK->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Event_StartScreenTransition();
  Actor_SetSpeed(ACTOR_POI, 1.2);
  Actor_WalkToAndWait(ACTOR_POI, 464, 428);
  Actor_WalkToAndWait(ACTOR_POI, 432, 408);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 0);
  FieldEffect_SetAttached(ACTOR_POI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_SetMessageId((s32)&MsgKandoreanTempleThroughTheCave);
  // "You must have come through the cave..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  // "That means..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirectionById(ACTOR_POI, FACING_EAST, 40);
  // "I must give you... the secret of Lash!"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  // "Master Poi... You seem perturbed."
  Event_ShowMessageAndWait(ACTOR_HALL_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_POI, 2);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "Oh, no... Not at all..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  // "Felix, am I imagining it, or does he seem perturbed?" Yes: "I am not
  // perturbed. Merely...surprised." No: "No one from outside the temple has
  // ever passed..."
  Event_OpenMessage(ACTOR_HALL_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
    // "I am not perturbed. Merely...surprised."
    Event_ShowMessageAndWait(ACTOR_POI, 0);
    WORK->message++;
  } else {
    WORK->message++;
    Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
    // "No one from outside the temple has ever passed the test. It's
    // amazing..."
    Event_ShowMessageAndWait(ACTOR_POI, 0);
  }
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What is this "Lash" you mentioned a moment ago?"
  Event_ShowMessageAndWait(ACTOR_HALL_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_POI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Lash is a sacred power, passed down for years to the followers of..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_SetAnimationById(ACTOR_HALL_KRADEN, ANIM_NOD);
  // "A sacred power? And it's called Lash, you say?"
  Event_ShowMessageThenWait(ACTOR_HALL_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 40);
  // "And now, I must pass it on to you. It is our way."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "It is a gift given to all those who pass the test, as it has been..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_ShowEmoteById(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  // "What type of power is this Lash?"
  Event_ShowMessageAndWait(ACTOR_HALL_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "Allow me to show you."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(424, 16, 384, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_HALL_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gKandoreanTemple_PoiWalksOutScript);
  Actor_SetMovementTypeById(ACTOR_HALL_KRADEN,
                            gKandoreanTemple_StudentWalksAsideScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_POI,
                                   gKandoreanTemple_StudentWalksAwayScript);
  Event_Wait(40);
  KandoreanTemple_CastLash();
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 20);
  // "Lash looks quite a bit like Psynergy, doesn't it?"
  Event_OpenMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_HALL_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    KandoreanTemple_PoiWalksUp();
    WORK->message++;
  } else {
    WORK->message++;
    KandoreanTemple_PoiWalksUp();
  }
  Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_HALL_KRADEN, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION,
                      40);
  Event_ShowMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_HALL_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  Event_OpenMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_HALL_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
    Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
    Actor_ShowEmoteById(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_HALL_KRADEN, 0);
    WORK->message += 2;
  } else {
    WORK->message += 2;
    Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
    Actor_FaceDirectionById(ACTOR_HALL_KRADEN, FACING_EAST, 0);
    Actor_ShowEmoteById(ACTOR_HALL_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(
        SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_HALL_KRADEN, 0);
  }
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  FieldCamera_MoveTo(424, 0, 400, 1);
  FieldCamera_SetSpeed(0.6);
  Actor_SetSpeed(ACTOR_POI, 0.6);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_POI);
  Actor_WatchAndSetScript(ACTOR_HALL_KRADEN, ACTOR_POI);
  Actor_WalkToAndWait(ACTOR_POI, 488, 416);
  Actor_WalkToAndWait(ACTOR_POI, 472, 432);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_HALL_KRADEN, 0, 80);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 20);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  FieldCamera_MoveTo(424, 16, 384, 1);
  Actor_SetSpeed(ACTOR_POI, 1.2);
  Actor_WalkToAndWait(ACTOR_POI, 488, 416);
  Actor_WalkToAndWait(ACTOR_POI, 504, 376);
  Actor_FaceDirectionById(ACTOR_POI, FACING_WEST, 20);
  object = Actor_GetById(ACTOR_POI);
  sprite = object->sprite;
  Actor_SetSpritePriority(ACTOR_POI, 1);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 492, 376);
  Actor_SetAnimationById(ACTOR_POI, 7);
  object->scaleX = -FIXED_ONE;
  sprite->rotation = TRIG_TURN * 7 / 8;
  object->motionFlags = 0;
  object->y.fixed += toFIXED(4);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpeed(ACTOR_POI, 0.4);
  FieldActor_SetMoveTarget((s32)object, 444, 40, 376);
  Actor_WaitForMoveById(ACTOR_POI);
  object->motionFlags = 3;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 1);
  sprite->rotation = 0;
  object->scaleX = FIXED_ONE;
  object->groundHeight = toFIXED(32);
  Actor_SetAnimationById(ACTOR_POI, ANIM_STAND);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 424, 376);
  FieldCamera_MoveTo(408, 16, 400, 1);
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 392, 360);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpritePriority(ACTOR_POI, 2);
  Actor_Stop(ACTOR_HALL_KRADEN);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_FaceDirection(ACTOR_HALL_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 10);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_HALL_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_HALL_KRADEN, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_HALL_KRADEN, leader->x.part[0].integer,
                               leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_HALL_KRADEN);
  Actor_SetPosition(ACTOR_HALL_KRADEN, 0, 0);
  GameFlag_Set(FLAG_MET_POI);
#if TLA_PATCH_1
  WORK->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
#endif
  Event_End();
}

/// Creates the Lash Pebble's actor on Poi's table, drawn from its item icon.
void KandoreanTemple_CreateLashPebble(void) {
  struct Actor **slot = &gKandoreanTemple_LashPebble;
  s32 zero = 0;
  struct Actor *object = ((struct Actor *)Import_ActorCreate(
      0x1e9, toFIXED(408), toFIXED(40), toFIXED(344)));

  *slot = object;
  if (object) {
    struct FieldSprite *sprite = object->sprite;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->retainSpriteCache = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(ITEM_LASH_PEBBLE);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}

/// Back in the Chestbeaters' room after their battle, a ladder unrolls down
/// the north wall, rung by rung.
void KandoreanTemple_LowerTheLadder(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Sound_Play(SE_SCUFFLE);
  MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 1, 20, 1, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 2, 20, 2, 1, 1);
  Event_Wait(10);
  MapGrid_CopyCellPictureIndicesRect(41, 3, 20, 3, 1, 1);
  Event_Wait(10);
  GameFlag_Set(FLAG_LADDER_LOWERED);
  Event_End();
}

void KandoreanTemple_FightFog(void);
void KandoreanTemple_ReceiveLash(void);

extern u32 gFieldEffect_LongScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

s32 gKandoreanTemple_SteamScript[] = {
    SC_WAIT(44),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
    SC_CALL((s32)KandoreanTemple_CountDownSteam),
    SC_SET(SW_HOOK, 0),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

s16 gKandoreanTemple_BlockActors[] = {
    ACTOR_LIFT_ROOM_PILLAR, -1,
};

s16 gKandoreanTemple_PillarActors[] = {
    ACTOR_VENT_ROOM_PILLAR, -1,
};

u32 gKandoreanTemple_PuffScript[] = {
    SC_WAIT(24),
    SC_RELEASE,
};

u32 gKandoreanTemple_StreamPuffScript[] = {
    SC_WAIT(15),
    SC_POSITION_ORIGIN,
    SC_WAIT(1),
    SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

u32 gKandoreanTemple_LookAroundScript[] = {
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
    SC_WAIT(80),
    SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
    SC_WAIT(80),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gKandoreanTemple_MeditateScript[] = {
    SC_CALL((s32)KandoreanTemple_MeditateRestlessly),
};

s32 gKandoreanTemple_LevitateScript[] = {
    SC_CALL(KandoreanTemple_LevitateStudentBob),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gKandoreanTemple_PoiWalksOutScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(456, 0, 408),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gKandoreanTemple_StudentWalksAsideScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(440, 0, 392),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 gKandoreanTemple_StudentWalksAwayScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(496, 0, 392),
    SC_SYNC,
    SC_MOVE(504, 0, 376),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gKandoreanTemple_LevitateHighScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_SET(SW_Y, toFIXED(88)),
    SC_END,
};

s32 gKandoreanTemple_LevitateLowScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_SET(SW_Y, toFIXED(44)),
    SC_END,
};

s32 gKandoreanTemple_StandStillScript[] = {
    SC_CALL(KandoreanTemple_PlaceLevitatingStudent),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gKandoreanTemple_ChestbeaterRoomRegions[] = {
    0x4a0140, 0x1500060, 0x70005a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gKandoreanTemple_StreamRoomRegions[] = {
    0x2a0200, 0x2100110, 0x120003a, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(KandoreanTempleLifts),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_STEAM, KandoreanTempleSteam, KANDOREAN_TEMPLE_STEAM_FROM_KANDOREAN_TEMPLE_LIFTS),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_A, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_B, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_SOUTHEAST, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_SOUTHEAST),
    SCENE_EXITS(KandoreanTempleMimic),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_SOUTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_NORTH),
    SCENE_EXITS(KandoreanTempleChestbeaters),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXITS(KandoreanTemplePillar),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_STEAM, KandoreanTempleSteam, KANDOREAN_TEMPLE_STEAM_FROM_KANDOREAN_TEMPLE_PILLAR),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_WEST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_A),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_SOUTHEAST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_B),
    SCENE_EXITS(KandoreanTempleStream),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_EAST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_EAST),
    SCENE_EXITS(KandoreanTempleSteam),
    SCENE_EXIT(KANDOREAN_TEMPLE_STEAM_TO_KANDOREAN_TEMPLE_LIFTS, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_STEAM),
    SCENE_EXIT(KANDOREAN_TEMPLE_STEAM_TO_KANDOREAN_TEMPLE_PILLAR, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_STEAM),
    SCENE_EXITS(KandoreanTempleFog),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_FOG_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_FOG_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_FOG_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_FOG_SOUTH),
    SCENE_EXITS(KandoreanTempleGate),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_KANDOREAN_TEMPLE_GATE),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_GATE),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST, KandoreanTempleStream, KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_EAST, KandoreanTempleStream, KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_EAST),
    SCENE_EXITS(KandoreanTemplePoi),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(2, KandoreanTemplePoi, 4),
    SCENE_EXIT(3, KandoreanTemplePoi, 5),
    SCENE_EXIT(4, KandoreanTemplePoi, 2),
    SCENE_EXIT(5, KandoreanTemplePoi, 3),
    SCENE_EXIT(6, KandoreanTemplePoi, 7),
    SCENE_EXIT(7, KandoreanTemplePoi, 6),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(13, KandoreanTemplePoi, 13),
    SCENE_EXIT(14, KandoreanTemplePoi, 14),
    SCENE_EXITS_END,
};

struct ActorPlacement gKandoreanTemple_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_LiftRoomPlacements[] = {                                             
      {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},                
      {407, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 3},                
      {407, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 3},                
      {272, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {351, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gKandoreanTemple_ChestbeaterRoomPlacements[] = {
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {368, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(450, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(184, CONDITION_ALWAYS, 7, 328, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(184, CONDITION_ALWAYS, 7, 296, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(184, CONDITION_ALWAYS, 7, 360, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_VentRoomPlacements[] = {
    {272, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {351, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_WEST, 2, 2},
    {351, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(351, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_FogRoomPlacements[] = {
    {407, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {407, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 340, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_FOG), MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_GatePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_IVY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 224, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, (s32)gKandoreanTemple_LookAroundScript, 264, 0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 334, 0, 317, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 284, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 318, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_PoisHallPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(21, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 664, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(91, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateHighScript, 439, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateHighScript, 359, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_LevitateLowScript, 412, 0, 324, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_StandStillScript, 90, 0, 230, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_StandStillScript, 174, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gKandoreanTemple_Events[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_LiftRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {TRIGGER_STEP, 10, FLAG_PILLAR_BLOCKS_SHAFT, EVENT(KandoreanTemple_SinkIntoTheSteam)},
    {TRIGGER_STEP, 12, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideWestLiftUp)},
    {TRIGGER_STEP, 13, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideEastLiftUp)},
    {TRIGGER_TABLE_END},
};

s16 gKandoreanTemple_Room2Events[] = {
    1, 0, 1, -1, 1, 0, 49, 0,
    2, -1, 2, 0, 33, 0, 3, -1,
    3, 0, 49, 0, 4, -1, 4, 0,
    -1, -1, 0, 0, 0, 0,
};

struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(KandoreanTemple_StartMovingPot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(KandoreanTemple_FinishMovingPot)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 10, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 11, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {TRIGGER_TALK, ACTOR_POIS_SIGN, CONDITION_ALWAYS, EVENT(KandoreanTemple_ReadPoisSign)},
    {TRIGGER_STEP, 6, FLAG_LADDER_LOWERED, EVENT(KandoreanTemple_FightChestbeaters)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_VentRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveByWestVent)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveByEastVent)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_Room5Events[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gKandoreanTemple_Room6Events[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_PushThroughTheSteam)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_FogRoomEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FOG_ROPE_POLE, FLAG_FOG_ROPE_TIED,
     0},
    {TRIGGER_TALK, ACTOR_FOG, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_FightFog)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepBehindThePole)},
    {TRIGGER_STEP, 11, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepOutFromThePole)},
    {TRIGGER_TALK, ACTOR_STUDENT_WHO_GAVE_UP, CONDITION_ALWAYS,
     MSGNO(MsgFieldItSeemsIMayNotHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_GateEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 6, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_SlipIntoTheCrack)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgKandoreanTemplePassageInTheWell))},
    {TRIGGER_STEP, 20, FLAG_KRADEN_EYED_THE_WALL,
     EVENT(KandoreanTemple_KradenSuspectsTheWall)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 4, FLAG_WELL_PASSAGE_OPEN,
     EVENT(KandoreanTemple_OpenTheWellPassage)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 5, FLAG_WELL_PASSAGE_OPEN,
     EVENT(KandoreanTemple_OpenTheWellPassage)},
    {TRIGGER_EXAMINE, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TryTheLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_FindTheHiddenPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_CycloneFifthPatch)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(KandoreanTemple_AskAboutPoi)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TellOfTheMeditation)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9 | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_ASKED_ABOUT_POI, EVENT(KandoreanTemple_ReadTheGateStudent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10 | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_TOLD_OF_THE_MEDITATION, EVENT(KandoreanTemple_ReadTheGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWasHopingToHearMaster)},
    {TRIGGER_TALK, 11, FLAG_MET_POI, MSGNO(MsgFieldIHopeToEnterTheTemple)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
    {TRIGGER_TALK, 12, FLAG_MET_POI,
     MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
    {TRIGGER_TALK, 13, FLAG_MET_POI, MSGNO(MsgFieldIAmNotJustMeditatingI)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgFieldSomedayImGoingToBeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_MET_POI,
     MSGNO(MsgFieldIStillCantBelieveThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldGahIKeptMyEyesShut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_MET_POI,
     MSGNO(MsgFieldMyMindIsEmptyMyBody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_MET_POI,
     MSGNO(MsgFieldEmptyTheMindToLightenThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
    {TRIGGER_TABLE_END},
};

// clang-format off
struct Trigger gKandoreanTemple_PoisHallEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {STEP_TRIGGER_WALKING, 8, CONDITION_ALWAYS, EVENT(KandoreanTemple_ClimbDownTheWell)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT(KandoreanTemple_DropThroughTheTrapdoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SOUTH_ROPE_WEST_POLE, FLAG_SOUTH_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_NORTH_ROPE_EAST_POLE, FLAG_NORTH_ROPE_TIED, 0},
    {TRIGGER_TALK, ACTOR_FIRST_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldHeWentInBeforeMeWhat)},
    {TRIGGER_TALK, ACTOR_FIRST_TESTED_STUDENT, CONDITION_ALWAYS, EVENT(KandoreanTemple_AskWhoPassedTheTest)},
    {TRIGGER_TALK, ACTOR_SECOND_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldEmptyMindEmptyBodyByClearing)},
    {TRIGGER_TALK, ACTOR_SECOND_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldIHearThatSomeonePassedThe)},
    {TRIGGER_TALK, ACTOR_THIRD_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldWhenIAwokeILearnedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldTheresNoWayImRootingFor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldTheMasterSeemedSoDisappointedThat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TESTED_STUDENT, FLAG_MET_POI, MSGNO(MsgFieldArrghIWasSoCloseI)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldIKnewItItShouldHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_TESTED_STUDENT, CONDITION_ALWAYS, MSGNO(MsgFieldBeforeIPassedOutISaw)},
    {TRIGGER_EXAMINE, 16, FLAG_LASH_RECEIVED, EVENT(KandoreanTemple_ReceiveLash)},
    {TRIGGER_TALK, ACTOR_POI, FLAG_LASH_RECEIVED, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {TRIGGER_TALK, ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGER_TABLE_END},
};
// clang-format on
