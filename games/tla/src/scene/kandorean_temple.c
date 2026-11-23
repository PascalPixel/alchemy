/// @file
/// The Kandorean Temple of Master Poi, south-west of Daila, in nine rooms that
/// share this overlay (SceneId_KandoreanTempleLifts to
/// SceneId_KandoreanTemplePoi).
///
/// Its gate is shut for the meditation (KandoreanTemple_StartGate): Kraden
/// spots a suspicious wall, and Whirlwind on the bush in the courtyard's north
/// west corner opens a hidden way in through a cave of shallow water
/// (KandoreanTemple_FindTheHiddenPassage). Inside, Poi tests three students
/// (KandoreanTemple_TestTheStudents), and the party follows the one who can
/// levitate, through the door below Poi's statue, into the caverns of his
/// trials: the Mimic's chest, the student who gave up, a sheet of water whose
/// current pushes Felix south (KandoreanTemple_PushAlongTheCurrent), two vents
/// whose round covers rise on gushing columns unless the pillar holds them down
/// (KandoreanTemple_RunWestVent, KandoreanTemple_RunEastVent), the lifts those
/// columns push up into the room above, and its shaft the pillar there can
/// block; then a pot boiling over a hot floor, and three Chestbeaters
/// (KandoreanTemple_FightChestbeaters), after which a ladder comes down to
/// Poi's hall. Poi gives the party the Lash Pebble
/// (KandoreanTemple_ReceiveLash), and Lash ties the rope across to Fog, the
/// Mercury Djinni (KandoreanTemple_FightFog). Once Poi has met the party, the
/// temple's door in the courtyard opens as Felix walks up to it
/// (KandoreanTemple_OpenTheTempleDoor).

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
#include "field/field_sequence.h"
#include "field/field_sequence_event.h"
#include "game/game.h"
#include "graphics/glyph.h"
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

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

/// The Kandorean Temple's rooms: their tables' getters, the steam, vents and
/// lifts.

/// The lift room's actors (SceneId_KandoreanTempleLifts), numbering the rows of
/// gKandoreanTemple_LiftRoomPlacements.
enum KandoreanTempleLiftRoomActorId {
  /// The posts of the two tightropes, a pair to each rope.
  ACTOR_WEST_ROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_WEST_ROPE_EAST_POST,
  ACTOR_EAST_ROPE_WEST_POST,
  ACTOR_EAST_ROPE_EAST_POST,
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
  /// The red embers the pot boils over: an actor in the pot's sprite at the
  /// pot's place, in its animation 2, the sheet's last picture alone. It is
  /// drawn underfoot with no shadow, hidden under the pot, and stays glowing on
  /// the floor where the pot stood when the pot is chucked aside.
  ACTOR_EMBERS_UNDER_THE_POT,
  /// The sign by the hot floor, which tells of Master Poi's trial.
  ACTOR_POIS_SIGN,
  ACTOR_MIDDLE_CHESTBEATER,
  ACTOR_WEST_CHESTBEATER,
  ACTOR_EAST_CHESTBEATER
};

/// The vent room's actors (SceneId_KandoreanTemplePillar), numbering the rows
/// of gKandoreanTemple_VentRoomPlacements.
enum KandoreanTempleVentRoomActorId {
  /// The pillar Move sets on either vent's cover to hold it down.
  ACTOR_VENT_ROOM_PILLAR = ACTOR_FIRST_PLACED,
  /// The round stone covers of the two vents, which their columns of water
  /// lift.
  ACTOR_WEST_VENT_COVER,
  ACTOR_EAST_VENT_COVER,
  /// The gushing tops of the columns of water that grow under the covers as
  /// they rise.
  ACTOR_WEST_COLUMN_TOP,
  ACTOR_EAST_COLUMN_TOP,
  /// The pieces of the west column below its top, from the highest down. A
  /// cover rises on all four while the pillar holds the other vent down;
  /// otherwise on the upper two.
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
  /// The posts of the rope Lash ties across to Fog's ledge.
  ACTOR_FOG_ROPE_POST = ACTOR_FIRST_PLACED,
  ACTOR_FOG_ROPE_OTHER_POST,
  /// The student who has given up on the test: "It seems I may not have been
  /// ready after all..." He sits on the floor, squashing and stretching now
  /// and then, until the party has met Poi.
  ACTOR_STUDENT_WHO_GAVE_UP,
  /// Fog, the Mercury Djinni.
  ACTOR_FOG
};

/// The temple gate's actors (SceneId_KandoreanTempleGate), numbering the rows
/// of gKandoreanTemple_GatePlacements.
enum KandoreanTempleGateActorId {
  /// The weeds over the mouth of the cave, which Whirlwind scatters.
  ACTOR_WEEDS_OVER_THE_CAVE = ACTOR_FIRST_PLACED,
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

/// Poi's room's actors (SceneId_KandoreanTemplePoi), numbering the rows of
/// gKandoreanTemple_PoisRoomPlacements.
enum KandoreanTemplePoisRoomActorId {
  /// The posts of the room's two ropes: Lash ties the loose end of each.
  ACTOR_SOUTH_ROPE_WEST_POST = ACTOR_FIRST_PLACED,
  ACTOR_SOUTH_ROPE_EAST_POST,
  ACTOR_NORTH_ROPE_EAST_POST,
  ACTOR_NORTH_ROPE_WEST_POST,
  ACTOR_POI,
  /// The three students Poi tests, in turn; only the third can levitate well
  /// enough to go on to the final test.
  ///@{
  ACTOR_FIRST_TESTED_STUDENT,
  ACTOR_SECOND_TESTED_STUDENT,
  ACTOR_THIRD_TESTED_STUDENT,
  ///@}
  ACTOR_POIS_ROOM_KRADEN
};

/// Poi's room's story exits, each leading back into the room by the entrance of
/// its number: finding the rope still lying on the floor
/// (KandoreanTemple_FindTheRopeOnTheFloor), and coming in through the cave to
/// meet Poi (KandoreanTemple_MeetPoi).
enum {
  KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR = 13,
  KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE = 14
};

/// The ways between the three parts of Poi's hall, each exit leading in by the
/// entrance on its far side: the test hall in the south, where Poi tests his
/// students before the door below his statue; the upper hall north of it, with
/// Poi's table and the north rope, reached by two openings in the test hall's
/// north wall; and the passage down the west side, from the ladder up out of
/// the Chestbeaters' room to the upper hall's west doorway.
enum {
  KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST = 2,
  KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE,
  KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL
};
/// The entrances, one for each exit above.
enum {
  KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_WEST = 2,
  KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_EAST,
  KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE,
  KANDOREAN_TEMPLE_POI_WEST_PASSAGE_FROM_UPPER_HALL
};

/// The rooms' own cell behaviors, named by room.
enum KandoreanTempleCellBehavior {
  /// The lift room's shaft, cells (43, 10) to (43, 13).
  CELL_BEHAVIOR_SHAFT = 10,
  /// The west and east lifts while risen, cells (27, 12) and (37, 14).
  CELL_BEHAVIOR_WEST_LIFT = 12,
  CELL_BEHAVIOR_EAST_LIFT,
  /// The Chestbeaters' room: the cells west, south and east of the hot floor
  /// under the pot, (19, 29), (20, 30) and (21, 29), and the floor before the
  /// Chestbeaters, (19, 12) to (21, 12).
  CELL_BEHAVIOR_WEST_OF_HOT_FLOOR = 10,
  CELL_BEHAVIOR_SOUTH_OF_HOT_FLOOR,
  CELL_BEHAVIOR_EAST_OF_HOT_FLOOR,
  CELL_BEHAVIOR_BEFORE_CHESTBEATERS = 6,
  /// The vent room's covers while risen, which carry Felix up.
  CELL_BEHAVIOR_WEST_VENT_COVER = 20,
  CELL_BEHAVIOR_EAST_VENT_COVER,
  /// The room of the current: the sheet of water the current runs through.
  CELL_BEHAVIOR_CURRENT = 10,
  /// Fog's room: behind and beside the rope's posts.
  CELL_BEHAVIOR_BEHIND_POST = 10,
  CELL_BEHAVIOR_BESIDE_POST,
  /// The courtyard: the cells just inside and just before the temple's door,
  /// kept at (2, 0) and (2, 2) in the rock of the top left corner and laid at
  /// (17, 24) and (17, 26) once Poi has been met; the well, cells (25, 17) to
  /// (26, 18); the west wall
  /// Kraden suspects, (5, 15) and (6, 15); the temple's door, (17, 25) and
  /// (18, 25); and the five clumps of weeds Cyclone scatters.
  CELL_BEHAVIOR_INSIDE_TEMPLE_DOOR = 4,
  CELL_BEHAVIOR_BEFORE_TEMPLE_DOOR,
  CELL_BEHAVIOR_COURTYARD_WELL,
  CELL_BEHAVIOR_SUSPICIOUS_WALL = 20,
  CELL_BEHAVIOR_TEMPLE_DOOR = 10,
  CELL_BEHAVIOR_FIRST_WEED_CLUMP = 30,
  CELL_BEHAVIOR_SECOND_WEED_CLUMP,
  CELL_BEHAVIOR_THIRD_WEED_CLUMP,
  CELL_BEHAVIOR_FOURTH_WEED_CLUMP,
  CELL_BEHAVIOR_FIFTH_WEED_CLUMP,
  /// Poi's hall: round the opening down to the Chestbeaters' room, cells
  /// (9, 5), (11, 5) and (10, 6); before the door below Poi's statue, (24, 40)
  /// and (25, 40); and Poi's table with the Lash Pebble, (25, 21).
  CELL_BEHAVIOR_BY_THE_LADDER = 8,
  CELL_BEHAVIOR_CAVE_DOOR,
  CELL_BEHAVIOR_POIS_TABLE = 16
};

/// The Chestbeaters' room's entrance after their battle, which no exit leads
/// to: the party is back before the north wall, and the ladder comes down.
enum { ENTRANCE_AFTER_CHESTBEATER_BATTLE = 3 };

/// The temple's scene and area flags, by the rooms that read them.
enum {
  /// The leader stands on the lift room's west or east lift, or on the vent
  /// room's west or east cover.
  FLAG_ON_WEST_LIFT = FLAG_SCENE_TEMP_08,
  FLAG_ON_EAST_LIFT = FLAG_SCENE_TEMP_09,
  /// Move has lifted the vent room's pillar off a vent's cover.
  FLAG_PILLAR_LIFTED_OFF_VENT = FLAG_SCENE_TEMP_0A,
  /// Move is carrying the vent room's pillar.
  FLAG_PILLAR_MOVING = FLAG_SCENE_TEMP_0B,
  /// The vent room's west or east vent blows, its column of water rising out
  /// of its hole under its cover.
  FLAG_WEST_VENT_BLOWING = FLAG_SCENE_TEMP_0C,
  FLAG_EAST_VENT_BLOWING = FLAG_SCENE_TEMP_0D,
  /// The vents' rush of steam is playing (SE_WAVE).
  FLAG_VENT_SOUND_PLAYING = FLAG_SCENE_TEMP_0E,
  /// Lash has tied the rope across to Fog, or the south rope of Poi's room.
  FLAG_FOG_ROPE_TIED = FLAG_SCENE_TEMP_0F,
  FLAG_SOUTH_ROPE_TIED = FLAG_SCENE_TEMP_0F,
  /// The north rope of Poi's room is tied across.
  FLAG_NORTH_ROPE_TIED = FLAG_SCENE_TEMP_10,
  /// The lift room's pillar blocks the shaft, so stepping over it no longer
  /// sinks Felix into it.
  FLAG_PILLAR_BLOCKS_SHAFT = FLAG_SCENE_TEMP_11,
  /// The door to the cave below Poi's statue, or the temple's door in the
  /// courtyard, is open.
  FLAG_CAVE_DOOR_OPEN = FLAG_SCENE_TEMP_12,
  FLAG_TEMPLE_DOOR_OPEN = FLAG_SCENE_TEMP_12,
  /// Kraden has pointed out the suspicious wall by the gate on this visit.
  FLAG_KRADEN_SAW_THE_WALL = FLAG_SCENE_TEMP_30,
  /// Where the vent room's pillar stands, which also decides which of the lift
  /// room's lifts runs.
  FLAG_PILLAR_ON_WEST_VENT = FLAG_AREA_TEMP_00,
  FLAG_PILLAR_ON_EAST_VENT = FLAG_AREA_TEMP_01,
  /// Where Move has chucked the Chestbeaters' room's pot of boiling water.
  FLAG_POT_PUSHED_WEST = FLAG_AREA_TEMP_02,
  FLAG_POT_PUSHED_EAST = FLAG_AREA_TEMP_03,
  /// The pot is out of the way, and the floor no longer burns.
  FLAG_POT_MOVED = FLAG_AREA_TEMP_04
};

s32 MapGrid_CheckCollision(struct Actor *actor, s32 *position);
void Actor_DisableById(s32 id);
s32 gKandoreanTemple_SplashRingCounter BSS_DATA;
s32 gKandoreanTemple_WestVentBlowing BSS_DATA;
s32 gKandoreanTemple_EastVentBlowing BSS_DATA;
s32 gKandoreanTemple_WestTimer BSS_DATA; ///< frames into its cycle
s32 gKandoreanTemple_EastTimer BSS_DATA; ///< frames into its cycle
u16 gKandoreanTemple_WestFloatPhase BSS_DATA;
u16 gKandoreanTemple_EastFloatPhase BSS_DATA;
struct Actor *gKandoreanTemple_LashPebble BSS_DATA;
extern s32 gKandoreanTemple_SteamScript[];
extern u32 gKandoreanTemple_SplashRingScript[];
extern u32 gKandoreanTemple_SplashScript[];

void Actor_LiftTargetBlock(const void *data);
void Actor_PlaceMovedBlock(void);
void TileCopies_CopyTouchedCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                 s32 g);
void FieldEffect_StartBlankOverlay(s32 a);
void KandoreanTemple_StartGate(void);
void Actor_PlaceOnActor(s32 id, s32 target);
extern s32 gKandoreanTemple_MeditateScript[];
extern s32 gKandoreanTemple_LevitateScript[];

extern u32 gKandoreanTemple_LookAroundScript[];
extern s16 gKandoreanTemple_BlockActors[];
extern s16 gKandoreanTemple_PillarActors[];

/// Creates an actor of the given sprite at a 16.16 position.
static __inline__ struct Actor *KandoreanTemple_CreateActor(s32 type, FIXED x,
                                                            FIXED y, FIXED z) {
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

/// The map work as the two water rooms write it: MapWork.groundEffects
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

extern struct SceneRegion gKandoreanTemple_StreamRoomRegions[];
extern struct ActorPlacement gKandoreanTemple_GatePlacements[];
extern struct ActorPlacement gKandoreanTemple_PoisRoomPlacements[];
extern struct Trigger gKandoreanTemple_MimicRoomEvents[];
void KandoreanTemple_ArriveOnEastLift(void);

void KandoreanTemple_TestTheStudents(void);
extern u32 gKandoreanTemple_LeaderStepsAsideScript[];
extern u32 gKandoreanTemple_KradenStepsAsideScript[];
extern u32 gKandoreanTemple_PoiWalksToTheRopeScript[];

void KandoreanTemple_FindTheHiddenPassage(void);
void KandoreanTemple_FightChestbeaters(void);

extern u32 gFieldEffect_ShortScript[];

extern u32 gFieldEffect_MediumScript[];

extern s32 gKandoreanTemple_HighFlameScript[];
extern s32 gKandoreanTemple_LowFlameScript[];
extern s32 gKandoreanTemple_FlameScript[];

void KandoreanTemple_ArriveOnWestLift(void);
void KandoreanTemple_LeaveSplashRings(struct Actor *actor, s32 step);
void KandoreanTemple_SpawnSplashRing(struct Actor *actor);
void KandoreanTemple_PushSouth(struct Actor *actor, FIXED dz);

void Tightrope_Initialize(void);
void Actor_PlaceBlocks(const void *data);

/// Talking to Fog starts its battle.
void KandoreanTemple_FightFog(void) {
  Field_RequestDjinnBattle(ACTOR_FOG, DJINNI_FOG);
}

/// A cloud of steam's script call: counts down its life and reports whether any
/// is left.
u32 KandoreanTemple_CountDownSteam(struct Actor *actor) {
  s32 left;

  left = --*(s16 *)&actor->userValue;
  return NONZERO(left);
}

/// A meditating student's update: now and then, every four to eight seconds, an
/// ellipsis over his head.
s32 KandoreanTemple_Meditate(struct Actor *actor) {
  if (actor->timer != 0) {
    actor->timer--;
  } else {
    Actor_ShowEmoteById((s16)actor->userValue, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
                        0);
    actor->timer = (Random_Below(240)) + 240;
  }
  return 1;
}

/// An actor script call, each frame: every 60 to 149 frames the actor squashes
/// and stretches back to its own shape over six frames.
s32 KandoreanTemple_SquashAndStretch(struct Actor *actor) {
  switch (actor->timer) {
  case 6:
    actor->scaleX -= FIXED_ONE / 4;
    actor->scaleY += FIXED_ONE / 8;
    break;
  case 4:
    actor->scaleX += FIXED_ONE / 8;
    actor->scaleY -= FIXED_ONE / 16;
    break;
  case 2:
    actor->scaleX += FIXED_ONE / 16;
    actor->scaleY -= FIXED_ONE / 32;
    break;
  case 0:
    actor->scaleX = FIXED_ONE;
    actor->scaleY = FIXED_ONE;
    actor->timer = Random16() % 90 + 60;
    break;
  }
  actor->timer--;
  return 1;
}

/// Sets up a torch's flame in Poi's room: still in the air, at four fifths of
/// its size, with no shadow, never a talk or Psynergy target.
s32 KandoreanTemple_PlaceFlame(struct Actor *actor) {
  actor->motionFlags = 0;
  actor->scaleX = FIXED_ONE * 4 / 5;
  actor->scaleY = FIXED_ONE * 4 / 5;
  actor->collisionFlags |= ACTOR_NOT_TARGETED;
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern struct SceneRegion gKandoreanTemple_ChestbeaterRoomRegions[];

/// The regions of the rooms whose exits are rectangles: the Chestbeaters' room
/// and the stream room.
const struct SceneRegion *Scene_GetExitRegions(void) {
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
/// students no longer turn to talk, and in Poi's room two of the tested
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
      gKandoreanTemple_PoisRoomPlacements[5].talkFacing = 3;
      gKandoreanTemple_PoisRoomPlacements[6].x = toFIXED(368);
      gKandoreanTemple_PoisRoomPlacements[6].z = toFIXED(694);
      gKandoreanTemple_PoisRoomPlacements[7].talkFacing = 1;
    }
    return gKandoreanTemple_PoisRoomPlacements;
  }
  return gKandoreanTemple_Placements;
}

/// Felix walks past the courtyard's west wall, cells (5, 15) and (6, 15),
/// before the hidden passage is found: Kraden walks up beside him and says the
/// wall looks rather suspicious, and the two nod.
void KandoreanTemple_KradenSuspectsTheWall(void) {
  struct Actor *actor;

  if (GameFlag_Get(FLAG_HIDDEN_PASSAGE_FOUND) == 0) {
    GameFlag_Set(FLAG_KRADEN_SAW_THE_WALL);
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
    actor = Actor_GetById(ACTOR_FELIX);
    if (actor)
      Actor_SetDestinationById(ACTOR_GATE_KRADEN, actor->x.part[0].integer,
                               actor->z.part[0].integer);
    Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
    Actor_SetPosition(ACTOR_GATE_KRADEN, 0, 0);
    Event_End();
  }
}

extern struct Trigger gKandoreanTemple_Events[];
extern struct Trigger gKandoreanTemple_LiftRoomEvents[];
extern struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[];
extern struct Trigger gKandoreanTemple_VentRoomEvents[];
extern struct Trigger gKandoreanTemple_StreamRoomEvents[];
extern struct Trigger gKandoreanTemple_CurrentRoomEvents[];
extern struct Trigger gKandoreanTemple_FogRoomEvents[];
extern struct Trigger gKandoreanTemple_GateEvents[];
extern struct Trigger gKandoreanTemple_PoisRoomEvents[];

/// The current room's triggers.
void *Scene_GetTriggers(void) {
  s32 scene = gMainRoutineWork.next;

  if (scene == SCENE_KANDOREAN_TEMPLE_LIFTS)
    return gKandoreanTemple_LiftRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_MIMIC)
    return gKandoreanTemple_MimicRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_CHESTBEATERS)
    return gKandoreanTemple_ChestbeaterRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_PILLAR)
    return gKandoreanTemple_VentRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_STREAM)
    return gKandoreanTemple_StreamRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_CURRENT)
    return gKandoreanTemple_CurrentRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_FOG)
    return gKandoreanTemple_FogRoomEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_GATE)
    return gKandoreanTemple_GateEvents;
  if (scene == SCENE_KANDOREAN_TEMPLE_POI)
    return gKandoreanTemple_PoisRoomEvents;
  return gKandoreanTemple_Events;
}

/// Each call of KandoreanTemple_LeaveSplashRings on a moving wader counts two
/// toward the next ring on the water, or four when half its speeds across and
/// down together pass SPLASH_RING_FAST_SPEED (a 16.16 speed, just under three
/// quarters of a pixel a frame); past SPLASH_RING_STRIDE a ring spreads at its
/// feet.
#define SPLASH_RING_FAST_SPEED 48000
#define SPLASH_RING_STRIDE 40

/// Unused: takes the leader's shadow away and counts one step of its wading
/// (KandoreanTemple_LeaveSplashRings).
UNUSED void KandoreanTemple_StartSplashRings(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  KandoreanTemple_LeaveSplashRings(leader, 0);
}

/// Counts one step of an actor through the water, more when it moves fast,
/// and spreads a ring on the water at its feet once enough are counted.
void KandoreanTemple_LeaveSplashRings(struct Actor *actor, s32 step) {
  s32 speed = (actor->velocityX + actor->velocityZ) >> 1;
  s32 *counter = &gKandoreanTemple_SplashRingCounter;

  if (speed < 0)
    speed = -speed;
  if ((u32)speed > SPLASH_RING_FAST_SPEED)
    *counter += 4;
  else if (speed != 0)
    *counter += 2;
  gKandoreanTemple_SplashRingCounter += step;
  if (gKandoreanTemple_SplashRingCounter > SPLASH_RING_STRIDE) {
    KandoreanTemple_SpawnSplashRing(actor);
    gKandoreanTemple_SplashRingCounter = 0;
  }
}

/// Spreads a translucent ring on the water at an actor's feet, which goes after
/// 24 frames.
void KandoreanTemple_SpawnSplashRing(struct Actor *actor) {
  struct Actor *ring = KandoreanTemple_CreateActor(
      SPRITE_SPLASH_RING, actor->x.fixed, actor->y.fixed, actor->z.fixed);
  struct FieldSprite *sprite;

  if (ring) {
    sprite = ring->sprite;
    Actor_StartScript(ring, gKandoreanTemple_SplashRingScript);
    ring->motionFlags = 0;
    ring->layer = 1;
    ring->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    if (sprite) {
      Sprite_StartAnimation(sprite, 2);
      sprite->blendMode = 1;
      sprite->flags = 0;
      sprite->priority = 3;
    }
  }
}

/// Felix wades into the sheet of water that pours south from the waterfall in
/// the room of the current, any of its cells: the current pushes him south,
/// splashing at his feet, unless he runs against it.
void KandoreanTemple_PushAlongTheCurrent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *splash;

  if ((gFrameCount & 7) == 0) {
    splash = ((struct Actor *)Import_ActorCreate(
        SPRITE_WADING_SPLASH, leader->x.fixed, leader->y.fixed + toFIXED(6),
        leader->z.fixed));
    if (splash) {
      Actor_SetSpriteFlags(splash, 0);
      Actor_SelectAnimation(splash, 1);
      Actor_StartScript(splash, gKandoreanTemple_SplashScript);
    }
  }
  KandoreanTemple_PushSouth(leader, toFIXED(0.85));
}

/// Moves an actor and the camera south by dz, unless the map stops it eight
/// pixels ahead or ahead to either side.
void KandoreanTemple_PushSouth(struct Actor *actor, FIXED dz) {
  struct Actor *focus = gFieldWork->camera;
  FIXED position[3];

  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + dz;
  MapGrid_CheckCollision(actor, position);
  position[0] = actor->x.fixed;
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + toFIXED(8);
  if (MapGrid_CheckCollision(actor, position) > 0)
    return;
  position[0] = actor->x.fixed + toFIXED(5.7);
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + toFIXED(5.7);
  if (MapGrid_CheckCollision(actor, position) > 0)
    return;
  position[0] = actor->x.fixed - toFIXED(5.7);
  position[1] = actor->y.fixed;
  position[2] = actor->z.fixed + toFIXED(5.7);
  if (MapGrid_CheckCollision(actor, position) > 0)
    return;
  focus->z.fixed += dz;
  actor->z.fixed += dz;
}

/// Whether actor a is within twelve pixels across and eight deep of actor b.
s32 KandoreanTemple_IsOverActor(struct Actor *a, struct Actor *b) {
  FIXED left = b->x.fixed - toFIXED(12);
  FIXED right = b->x.fixed + toFIXED(12);

  if (a->x.fixed > left && a->x.fixed < right &&
      a->z.fixed > b->z.fixed - toFIXED(8) &&
      a->z.fixed < b->z.fixed + toFIXED(8))
    return 1;
  return 0;
}

/// A cloud of the vents' steam: drifts and slows, grows and turns, and rises
/// instead of drifting on where the leader or the pillar stands over it.
void KandoreanTemple_UpdateVentSteam(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *other = Actor_GetById(ACTOR_VENT_ROOM_PILLAR);
  FIXED vx = actor->bounce;
  FIXED vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  actor->x.fixed += vx;
  actor->y.fixed += actor->gravity;
  vz = actor->riseBaseY;
  actor->z.fixed += vz;
  actor->bounce = vx - vx / FIELD_EFFECT_DRAG_X;
  actor->riseBaseY = vz - vz / FIELD_EFFECT_DRAG_Z;
  rate = actor->speedLimit;
  scale = actor->scaleX;
  sprite = actor->sprite;
  actor->scaleX = scale + rate;
  actor->scaleY += actor->acceleration;
  sprite->rotation += actor->timer;
  if (leader->y.fixed == 0) {
    if (KandoreanTemple_IsOverActor(actor, leader)) {
      actor->z.fixed -= actor->riseBaseY;
      actor->gravity += actor->riseBaseY >> 2;
      actor->riseBaseY = 0;
    }
    if (leader->y.fixed == 0 && KandoreanTemple_IsOverActor(actor, other)) {
      actor->z.fixed -= actor->riseBaseY;
      actor->gravity += actor->riseBaseY >> 2;
      actor->riseBaseY = 0;
    }
  }
}

/// Sets up a cloud of steam on the given sprite priority: translucent, no
/// shadow, its script.
void KandoreanTemple_InitializeSteam(struct Actor *actor, s32 priority) {
  struct FieldSprite *sprite;

  actor->motionFlags = 0;
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  sprite = actor->sprite;
  sprite->priority = priority;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SelectAnimation(actor, 2);
  Actor_StartScript(actor, gKandoreanTemple_SteamScript);
  Actor_SetSpriteColor(actor, SPRITE_COLOR_BLUE_TINT);
  actor->timer = 0;
  actor->speedLimit = 0;
  actor->acceleration = 0;
}

/// Blows a cloud of steam from a vent at position, rising at the given speed.
void KandoreanTemple_BlowSteam(s32 *position, s32 value) {
  struct Actor *actor = KandoreanTemple_CreateActor(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));

  if (actor) {
    s32 timer;

    actor->bounce =
        Trig_Cos((Random_Below(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->riseBaseY = (u16)(Random16() * 2) + value;
    timer = (u16)Random16() - 32768;
    actor->timer = timer;
    KandoreanTemple_InitializeSteam(actor, 3);
    actor->update = (ObjectUpdate)KandoreanTemple_UpdateVentSteam;
    Actor_SetBlendMode(actor, 1);
  }
}

/// A cloud of the lift room's stream of steam: as the vents' clouds, held down
/// by the leader or the lift room's pillar.
void KandoreanTemple_UpdateStreamSteam(struct Actor *actor) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *other = Actor_GetById(ACTOR_LIFT_ROOM_PILLAR);
  FIXED vx = actor->bounce;
  FIXED vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  actor->x.fixed += vx;
  actor->y.fixed += actor->gravity;
  vz = actor->riseBaseY;
  actor->z.fixed += vz;
  actor->bounce = vx - vx / FIELD_EFFECT_DRAG_X;
  actor->riseBaseY = vz - vz / FIELD_EFFECT_DRAG_Z;
  rate = actor->speedLimit;
  scale = actor->scaleX;
  sprite = actor->sprite;
  actor->scaleX = scale + rate;
  actor->scaleY += actor->acceleration;
  sprite->rotation += actor->timer;
  if (KandoreanTemple_IsOverActor(actor, leader)) {
    ((struct FieldSprite *)actor->sprite)->priority = 3;
    actor->z.fixed -= actor->riseBaseY;
    actor->gravity += actor->riseBaseY >> 2;
    actor->riseBaseY = 0;
  }
  if (KandoreanTemple_IsOverActor(actor, other)) {
    ((struct FieldSprite *)actor->sprite)->priority = 3;
    actor->z.fixed -= actor->riseBaseY;
    actor->gravity += actor->riseBaseY >> 3;
    actor->riseBaseY = 0;
  }
}

/// Blows a cloud of steam from the lift room's stream.
void KandoreanTemple_BlowStreamSteam(void) {
  struct Actor *actor = ((struct Actor *)Import_ActorCreate(
      SPRITE_DUST_CLOUD, toFIXED(696), toFIXED(28), toFIXED(156)));

  if (actor) {
    s32 timer;

    actor->bounce =
        Trig_Cos((Random_Below(TRIG_TURN * 3 / 16)) + TRIG_TURN * 3 / 16);
    actor->gravity = (u16)Random16();
    actor->riseBaseY = (u16)(Random16() * 2) + toFIXED(11) / 2;
    timer = (u16)Random16() - 32768;
    actor->timer = timer;
    KandoreanTemple_InitializeSteam(actor, 1);
    actor->update = (ObjectUpdate)KandoreanTemple_UpdateStreamSteam;
    Actor_SetBlendMode(actor, 1);
  }
}

/// Shows the leader's sprite flags again.
void KandoreanTemple_ShowLeader(void) {
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
}

/// Sets a lift or a vent's cover at rest: on the ground, in front, in its
/// lowered animation, disabled.
void KandoreanTemple_PrepareLift(s32 id) {
  struct Actor *actor = Actor_GetById(id);

  actor->riseCounter = 1;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimationById(id, 1);
  Actor_SetSpritePriority(id, 3);
  Actor_DisableById(id);
}

/// Sets a lift's column or a piece of one at rest, disabled.
void KandoreanTemple_PrepareColumn(s32 id) {
  Actor_GetById(id)->riseCounter = 1;
  Actor_SetSpritePriority(id, 2);
  Actor_DisableById(id);
}

/// Poi casts Lash on the north rope's east post.
void KandoreanTemple_CastLash(void) {
  FieldPsynergy_LoadResources(ABILITY_LASH, 1);
  FieldPsynergy_SetupActorPair(ACTOR_POI, ACTOR_NORTH_ROPE_EAST_POST);
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

/// The west vent's task: while it blows, a cloud of steam every ninth frame.
DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowWestVent,
                             gKandoreanTemple_WestVentBlowing, 9, toFIXED(296),
                             toFIXED(176), toFIXED(2))

/// The east vent's task: while it blows, a stronger cloud every fifth frame.
DEFINE_KANDOREAN_TEMPLE_VENT(KandoreanTemple_BlowEastVent,
                             gKandoreanTemple_EastVentBlowing, 5, toFIXED(344),
                             toFIXED(160), toFIXED(4))

/// The lift room's task: a cloud of steam from the stream every fifth frame.
void KandoreanTemple_BlowStream(void) {
  if (gFrameCount % 5 == 0)
    KandoreanTemple_BlowStreamSteam();
}

/// Lowers a piece of the west column flat under its cover.
void KandoreanTemple_LowerWestColumn(s32 id) {
  Actor_GetById(id)->scaleY = FIXED_ONE / 32;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(id), toFIXED(216), 0,
                                       toFIXED(216));
}

/// Lowers a piece of the east column flat under its cover.
void KandoreanTemple_LowerEastColumn(s32 id) {
  Actor_GetById(id)->scaleY = FIXED_ONE / 32;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(id), toFIXED(376), 0,
                                       toFIXED(248));
}

/// Grows a column piece by scale and raises it by dy.
void KandoreanTemple_GrowColumn(s32 id, FIXED dy, s32 scale) {
  struct Actor *actor = Actor_GetById(id);

  actor->scaleY += scale;
  actor->y.fixed += dy;
  actor->z.fixed -= dy;
}

/// Shrinks a column piece by scale and lowers it by dy.
void KandoreanTemple_ShrinkColumn(s32 id, FIXED dy, s32 scale) {
  struct Actor *actor = Actor_GetById(id);

  actor->scaleY -= scale;
  actor->y.fixed -= dy;
  actor->z.fixed += dy;
}

/// Whether the west vent stays shut: while Move carries the pillar off a
/// cover, or while the pillar weighs the west cover down.
s32 KandoreanTemple_IsWestVentBlocked(void) {
  if (GameFlag_Get(FLAG_PILLAR_LIFTED_OFF_VENT) ||
      GameFlag_Get(FLAG_PILLAR_ON_WEST_VENT))
    return 1;
  return 0;
}

/// Whether the east vent stays shut: while Move carries the pillar off a
/// cover, or while the pillar weighs the east cover down.
s32 KandoreanTemple_IsEastVentBlocked(void) {
  if (GameFlag_Get(FLAG_PILLAR_LIFTED_OFF_VENT) ||
      GameFlag_Get(FLAG_PILLAR_ON_EAST_VENT))
    return 1;
  return 0;
}

/// The west vent starts blowing: its column and cover ready to rise,
/// carrying the leader if it stands there.
void KandoreanTemple_StartWestVent(void) {
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_TOP, 2);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_UPPER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_LOWER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_WEST_COLUMN_FOOT, 3);
  Actor_SetAnimationById(ACTOR_WEST_VENT_COVER, 0);
  Actor_SetSpritePriority(ACTOR_WEST_VENT_COVER, 2);
  Actor_GetById(ACTOR_WEST_VENT_COVER)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_VENT_COVER),
                                       toFIXED(216), toFIXED(8), toFIXED(216));
  if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
    Actor_SetAnimationById(ACTOR_WEST_VENT_COVER, 4);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(gMainRoutineWork.leaderActorId), toFIXED(216),
        toFIXED(12), toFIXED(216));
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  }
  Actor_GetById(ACTOR_WEST_COLUMN_TOP)->scaleY = FIXED_ONE;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_COLUMN_TOP),
                                       toFIXED(216), 0, toFIXED(216));
  KandoreanTemple_LowerWestColumn(ACTOR_WEST_COLUMN_UPPER_PIECE);
  KandoreanTemple_LowerWestColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE);
  if (GameFlag_Get(FLAG_PILLAR_ON_EAST_VENT)) {
    KandoreanTemple_LowerWestColumn(ACTOR_WEST_COLUMN_LOWER_PIECE);
    KandoreanTemple_LowerWestColumn(ACTOR_WEST_COLUMN_FOOT);
  }
  gKandoreanTemple_WestVentBlowing = 1;
  Event_SetFlag(FLAG_WEST_VENT_BLOWING);
}

/// The east vent starts blowing: its column and cover ready to rise,
/// carrying the leader if it stands there.
void KandoreanTemple_StartEastVent(void) {
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_TOP, 2);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_UPPER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_LOWER_PIECE, 3);
  Actor_SetAnimationById(ACTOR_EAST_COLUMN_FOOT, 3);
  Actor_SetAnimationById(ACTOR_EAST_VENT_COVER, 0);
  Actor_SetSpritePriority(ACTOR_EAST_VENT_COVER, 2);
  Actor_GetById(ACTOR_EAST_VENT_COVER)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_VENT_COVER),
                                       toFIXED(376), toFIXED(8), toFIXED(248));
  if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
    Actor_SetAnimationById(ACTOR_EAST_VENT_COVER, 4);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(gMainRoutineWork.leaderActorId), toFIXED(376),
        toFIXED(12), toFIXED(248));
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  }
  Actor_GetById(ACTOR_EAST_COLUMN_TOP)->scaleY = FIXED_ONE;
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_COLUMN_TOP),
                                       toFIXED(376), 0, toFIXED(248));
  KandoreanTemple_LowerEastColumn(ACTOR_EAST_COLUMN_UPPER_PIECE);
  KandoreanTemple_LowerEastColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE);
  if (GameFlag_Get(FLAG_PILLAR_ON_WEST_VENT)) {
    KandoreanTemple_LowerEastColumn(ACTOR_EAST_COLUMN_LOWER_PIECE);
    KandoreanTemple_LowerEastColumn(ACTOR_EAST_COLUMN_FOOT);
  }
  gKandoreanTemple_EastVentBlowing = 1;
  Event_SetFlag(FLAG_EAST_VENT_BLOWING);
}

/// A vent's cycle, in frames of its timer. Shut, it counts only while nothing
/// holds it shut; at VENT_BLOW it blows, walls off its cover's cell and lifts
/// the cover on its column until VENT_RISE_END, higher when the pillar holds
/// the other vent down. The cover floats on the column until VENT_FLOAT_END and
/// on until its float comes round, then sinks from VENT_SINK; once the column
/// is flat the timer jumps to VENT_SHUT, which lowers everything and frees the
/// cell, and the vent rests a random while before its timer starts again. A
/// leader riding the risen cover at VENT_CARRY_LEADER is carried up to the
/// room above.
enum VentFrame {
  VENT_BLOW = 300,
  VENT_RISE_END = 316,
  VENT_CARRY_LEADER = 400,
  VENT_FLOAT_END = 600,
  VENT_SINK = 1000,
  VENT_SHUT = 3000
};

/// The west vent's task, its timer through its cycle.
void KandoreanTemple_RunWestVent(void) {
  s32 *timer = &gKandoreanTemple_WestTimer;
  struct Actor *cover;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 timerEnd;
  s32 t;
  s32 flag;

  if (*timer < VENT_BLOW) {
    if (KandoreanTemple_IsWestVentBlocked() != 0)
      return;
  } else if (*timer == VENT_BLOW) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 13, 13);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 63, 13);
    KandoreanTemple_StartWestVent();
  } else {
    timerEnd = VENT_SHUT - 1;
    if (*timer <= timerEnd) {
      cover = Actor_GetById(ACTOR_WEST_VENT_COVER);
      flag = GameFlag_Get(FLAG_PILLAR_ON_EAST_VENT);
      t = *timer;
      if (flag) {
        if (t <= VENT_RISE_END) {
          cover->y.fixed += toFIXED(9) / 2;
          limit = FIXED_ONE / 16;
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, toFIXED(2), limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE,
                                     toFIXED(3) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, toFIXED(1),
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_LOWER_PIECE,
                                     toFIXED(1) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
        } else if (t < VENT_FLOAT_END) {
          if (GameFlag_Get(FLAG_ON_WEST_LIFT) && *timer == VENT_CARRY_LEADER) {
            MapGrid_CopyCellAttributes(22, 0, 1, 1, 13, 13);
            Task_Remove(KandoreanTemple_RunWestVent);
          }
          phase = &gKandoreanTemple_WestFloatPhase;
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else if (t < VENT_SINK) {
          phase = &gKandoreanTemple_WestFloatPhase;
          if (*phase == 0) {
            gKandoreanTemple_WestVentBlowing = 0;
            *timer = VENT_SINK - 1;
            GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
          }
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else {
          cover->y.fixed += -toFIXED(27) / 4;
          limit = FIXED_ONE * 3 / 32;
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_TOP, toFIXED(3),
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_UPPER_PIECE,
                                       toFIXED(9) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE,
                                       toFIXED(3) / 2, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_LOWER_PIECE,
                                       toFIXED(3) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
          if (Actor_GetById(ACTOR_WEST_COLUMN_FOOT)->scaleY <= limit)
            *timer = timerEnd;
        }
      } else {
        if (t <= VENT_RISE_END) {
          cover->y.fixed += toFIXED(2.475);
          limit = FIXED_ONE / 16;
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, toFIXED(1), limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE,
                                     toFIXED(1) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, limit);
        } else if (t < VENT_FLOAT_END) {
          phase = &gKandoreanTemple_WestFloatPhase;
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else if (t < VENT_SINK) {
          phase = &gKandoreanTemple_WestFloatPhase;
          if (*phase == 0) {
            gKandoreanTemple_WestVentBlowing = 0;
            *timer = VENT_SINK - 1;
            GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
          }
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else {
          cover->y.fixed += -toFIXED(3.7125);
          limit = FIXED_ONE * 3 / 32;
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_TOP, toFIXED(3) / 2,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_UPPER_PIECE,
                                       toFIXED(3) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0,
                                       limit);
          if (Actor_GetById(ACTOR_WEST_COLUMN_MIDDLE_PIECE)->scaleY <= limit)
            *timer = timerEnd;
        }
      }
      if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        leader->y.fixed = cover->y.fixed + toFIXED(4);
        leader->targetY = ACTOR_NO_TARGET;
        leader->velocityY = 0;
      }
    } else if (*timer == VENT_SHUT) {
      MapGrid_CopyCellAttributes(8, 0, 1, 1, 13, 13);
      MapGrid_CopyCellAttributes(58, 0, 1, 1, 63, 13);
      Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
      Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
      Actor_SetAnimationById(ACTOR_WEST_VENT_COVER, 1);
      FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_VENT_COVER),
                                           toFIXED(216), 0, toFIXED(216));
      Actor_GetById(ACTOR_WEST_VENT_COVER)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpritePriority(ACTOR_WEST_VENT_COVER, 3);
      if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0,
                                        leader->z.fixed);
        Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                             SPRITE_FLAG_SHADOW);
      }
    } else {
      s32 none = KandoreanTemple_IsWestVentBlocked();

      if (none != 0)
        return;
      *timer += (Random16() << 7) >> 16;
      if ((u32)*timer > (Random_Below(10000)) + 10000)
        *timer = none;
    }
  }
  gKandoreanTemple_WestTimer++;
}

/// The east vent's task, as KandoreanTemple_RunWestVent with the vents
/// swapped.
void KandoreanTemple_RunEastVent(void) {
  s32 *timer = &gKandoreanTemple_EastTimer;
  struct Actor *cover;
  struct Actor *leader;
  u16 *phase;
  s32 limit;
  s32 raised;
  s32 last;
  s32 t;

  t = *timer;
  if (t < VENT_BLOW) {
    if (KandoreanTemple_IsEastVentBlocked() != 0)
      return;
  } else if (t == VENT_BLOW) {
    MapGrid_CopyCellAttributes(15, 15, 1, 1, 23, 15);
    MapGrid_CopyCellAttributes(65, 15, 1, 1, 73, 15);
    KandoreanTemple_StartEastVent();
  } else {
    last = VENT_SHUT - 1;
    if (t <= last) {
      cover = Actor_GetById(ACTOR_EAST_VENT_COVER);
      raised = GameFlag_Get(FLAG_PILLAR_ON_WEST_VENT);
      t = *timer;
      if (raised) {
        if (t <= VENT_RISE_END) {
          cover->y.fixed += toFIXED(9) / 2;
          limit = FIXED_ONE / 16;
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, toFIXED(2), limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE,
                                     toFIXED(3) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, toFIXED(1),
                                     limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_LOWER_PIECE,
                                     toFIXED(1) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
        } else if (t < VENT_FLOAT_END) {
          if (GameFlag_Get(FLAG_ON_EAST_LIFT) && *timer == VENT_CARRY_LEADER) {
            MapGrid_CopyCellAttributes(23, 0, 1, 1, 23, 15);
            Task_Remove(KandoreanTemple_RunEastVent);
          }
          phase = &gKandoreanTemple_EastFloatPhase;
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else if (t < VENT_SINK) {
          phase = &gKandoreanTemple_EastFloatPhase;
          if (*phase == 0) {
            gKandoreanTemple_EastVentBlowing = 0;
            *timer = VENT_SINK - 1;
            GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
          }
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else {
          cover->y.fixed += -toFIXED(27) / 4;
          limit = FIXED_ONE * 3 / 32;
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_TOP, toFIXED(3),
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_UPPER_PIECE,
                                       toFIXED(9) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE,
                                       toFIXED(3) / 2, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_LOWER_PIECE,
                                       toFIXED(3) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
          if (Actor_GetById(ACTOR_EAST_COLUMN_FOOT)->scaleY <= limit)
            *timer = last;
        }
      } else {
        if (t <= VENT_RISE_END) {
          cover->y.fixed += toFIXED(2.475);
          limit = FIXED_ONE / 16;
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, toFIXED(1), limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE,
                                     toFIXED(1) / 2, limit);
          KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, limit);
        } else if (t < VENT_FLOAT_END) {
          phase = &gKandoreanTemple_EastFloatPhase;
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else if (t < VENT_SINK) {
          phase = &gKandoreanTemple_EastFloatPhase;
          if (*phase == 0) {
            gKandoreanTemple_EastVentBlowing = 0;
            *timer = VENT_SINK - 1;
            GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
          }
          cover->y.fixed += Trig_Sin(*phase) >> 1;
          *phase += TRIG_TURN / 16;
        } else {
          cover->y.fixed += -toFIXED(3.7125);
          limit = FIXED_ONE * 3 / 32;
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_TOP, toFIXED(3) / 2,
                                       limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_UPPER_PIECE,
                                       toFIXED(3) / 4, limit);
          KandoreanTemple_ShrinkColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0,
                                       limit);
          if (Actor_GetById(ACTOR_EAST_COLUMN_MIDDLE_PIECE)->scaleY <= limit)
            *timer = last;
        }
      }
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        leader->y.fixed = cover->y.fixed + toFIXED(4);
        leader->targetY = ACTOR_NO_TARGET;
        leader->velocityY = 0;
      }
    } else if (t == VENT_SHUT) {
      MapGrid_CopyCellAttributes(8, 0, 1, 1, 23, 15);
      MapGrid_CopyCellAttributes(58, 0, 1, 1, 73, 15);
      Actor_SetPosition(ACTOR_EAST_COLUMN_TOP, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
      Actor_SetPosition(ACTOR_EAST_COLUMN_FOOT, 0, 0);
      Actor_SetAnimationById(ACTOR_EAST_VENT_COVER, 1);
      FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_VENT_COVER),
                                           toFIXED(376), 0, toFIXED(248));
      Actor_GetById(ACTOR_EAST_VENT_COVER)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetSpritePriority(ACTOR_EAST_VENT_COVER, 3);
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        leader = Actor_GetById(gMainRoutineWork.leaderActorId);
        Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0,
                                        leader->z.fixed);
        Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                             SPRITE_FLAG_SHADOW);
      }
    } else {
      s32 none = KandoreanTemple_IsEastVentBlocked();

      if (none != 0)
        return;
      *timer += (Random16() << 7) >> 16;
      if ((u32)*timer > (Random_Below(10000)) + 10000)
        *timer = none;
    }
  }
  gKandoreanTemple_EastTimer++;
}

/// A lift's cycle, in frames of its timer: at LIFT_RISE it starts up its column
/// from LIFT_SUNK_HEIGHT below the floor until it stands level with it, frees
/// its cell, and its timer jumps to LIFT_RISEN. It floats on the column, walls
/// its cell off again at LIFT_WALL_OFF, and once its float comes round after
/// LIFT_FLOAT_END it takes a leader standing on it down to the vent room, or
/// with no one on it sinks from LIFT_SINK.
enum LiftFrame {
  LIFT_RISE = 300,
  LIFT_RISEN = 2000,
  LIFT_WALL_OFF = 2280,
  LIFT_FLOAT_END = 2300,
  LIFT_SINK = 2800
};

/// How far below the lift room's floor a lift starts and ends, in pixels.
#define LIFT_SUNK_HEIGHT (-20)

/// The lift room's west lift, the top of the west vent's column, its timer
/// through its cycle.
void KandoreanTemple_RunWestLift(void) {
  struct Actor *lift = Actor_GetById(ACTOR_WEST_LIFT);
  struct Actor *base = Actor_GetById(ACTOR_WEST_LIFT_COLUMN);
  s32 *timer = &gKandoreanTemple_WestTimer;

  if (*timer == LIFT_RISE) {
    Scene_QueueMusic(SE_WAVE);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_WEST_LIFT), toFIXED(440), toFIXED(LIFT_SUNK_HEIGHT),
        toFIXED(204));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_LIFT_COLUMN),
                                    toFIXED(440), toFIXED(LIFT_SUNK_HEIGHT),
                                    toFIXED(200));
    Actor_SetSpritePriority(ACTOR_WEST_LIFT, 3);
    Actor_SetSpritePriority(ACTOR_WEST_LIFT_COLUMN, 3);
    base->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    base->scaleY = FIXED_ONE / 64;
    Actor_SetAnimationById(ACTOR_WEST_LIFT, 0);
    Actor_SetAnimationById(ACTOR_WEST_LIFT_COLUMN, 2);
  } else if (*timer > LIFT_RISE && *timer < LIFT_RISEN) {
    lift->y.fixed += toFIXED(4);
    KandoreanTemple_GrowColumn(ACTOR_WEST_LIFT_COLUMN, toFIXED(0.3),
                               toFIXED(0.3));
    if (lift->y.fixed > -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_WEST_LIFT, 2);
    if (lift->y.fixed >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 27, 12);
      *timer = LIFT_RISEN;
    }
  } else if (*timer < LIFT_SINK) {
    u16 *phase = &gKandoreanTemple_WestFloatPhase;

    lift->y.fixed += Trig_Sin(*phase) >> 1;
    *phase += TRIG_TURN / 16;
    if (*timer == LIFT_WALL_OFF)
      MapGrid_CopyCellAttributes(27, 10, 1, 1, 27, 12);
    if (*timer > LIFT_FLOAT_END && *phase == 0) {
      if (GameFlag_Get(FLAG_ON_WEST_LIFT)) {
        MapGrid_CopyCellAttributes(21, 0, 1, 1, 27, 12);
        Task_Remove(KandoreanTemple_RunWestLift);
      } else {
        *timer = LIFT_SINK;
      }
    }
  } else {
    lift->y.fixed += toFIXED(-4);
    KandoreanTemple_ShrinkColumn(ACTOR_WEST_LIFT_COLUMN, toFIXED(0.3),
                                 toFIXED(0.3));
    if (lift->y.fixed < -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_WEST_LIFT, 3);
    if (lift->y.fixed <= toFIXED(LIFT_SUNK_HEIGHT)) {
      Scene_QueueMusic(SE_AMBIENT_STREAM);
      Actor_SetPosition(ACTOR_WEST_LIFT, 0, 0);
      Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
      gKandoreanTemple_WestTimer = 0;
    }
  }
  gKandoreanTemple_WestTimer++;
}

/// The lift room's east lift, the top of the east vent's column, as
/// KandoreanTemple_RunWestLift.
void KandoreanTemple_RunEastLift(void) {
  struct Actor *lift = Actor_GetById(ACTOR_EAST_LIFT);
  struct Actor *base = Actor_GetById(ACTOR_EAST_LIFT_COLUMN);
  s32 *timer = &gKandoreanTemple_EastTimer;

  if (*timer == LIFT_RISE) {
    Scene_QueueMusic(SE_WAVE);
    FieldActor_SetPositionAndResetMotion(
        Actor_GetById(ACTOR_EAST_LIFT), toFIXED(600), toFIXED(LIFT_SUNK_HEIGHT),
        toFIXED(236));
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_LIFT_COLUMN),
                                    toFIXED(600), toFIXED(LIFT_SUNK_HEIGHT),
                                    toFIXED(232));
    Actor_SetSpritePriority(ACTOR_EAST_LIFT, 3);
    Actor_SetSpritePriority(ACTOR_EAST_LIFT_COLUMN, 3);
    base->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    base->scaleY = FIXED_ONE / 64;
    Actor_SetAnimationById(ACTOR_EAST_LIFT, 0);
    Actor_SetAnimationById(ACTOR_EAST_LIFT_COLUMN, 2);
  } else if (*timer > LIFT_RISE && *timer < LIFT_RISEN) {
    lift->y.fixed += toFIXED(4);
    KandoreanTemple_GrowColumn(ACTOR_EAST_LIFT_COLUMN, toFIXED(0.3),
                               toFIXED(0.3));
    if (lift->y.fixed > -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_EAST_LIFT, 2);
    if (lift->y.fixed >= 0) {
      MapGrid_CopyCellAttributes(0, 0, 1, 1, 37, 14);
      *timer = LIFT_RISEN;
    }
  } else if (*timer < LIFT_SINK) {
    u16 *phase = &gKandoreanTemple_EastFloatPhase;

    lift->y.fixed += Trig_Sin(*phase) >> 1;
    *phase += TRIG_TURN / 16;
    if (*timer == LIFT_WALL_OFF)
      MapGrid_CopyCellAttributes(37, 12, 1, 1, 37, 14);
    if (*timer > LIFT_FLOAT_END && *phase == 0) {
      if (GameFlag_Get(FLAG_ON_EAST_LIFT)) {
        MapGrid_CopyCellAttributes(22, 0, 1, 1, 37, 14);
        Task_Remove(KandoreanTemple_RunEastLift);
      } else {
        *timer = LIFT_SINK;
      }
    }
  } else {
    lift->y.fixed += toFIXED(-4);
    KandoreanTemple_ShrinkColumn(ACTOR_EAST_LIFT_COLUMN, toFIXED(0.3),
                                 toFIXED(0.3));
    if (lift->y.fixed < -toFIXED(5))
      Actor_SetSpritePriority(ACTOR_EAST_LIFT, 3);
    if (lift->y.fixed <= toFIXED(LIFT_SUNK_HEIGHT)) {
      Scene_QueueMusic(SE_AMBIENT_STREAM);
      Actor_SetPosition(ACTOR_EAST_LIFT, 0, 0);
      Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
      gKandoreanTemple_EastTimer = 0;
    }
  }
  gKandoreanTemple_EastTimer++;
}

/// The vent room's task: notes whether the leader stands on a cover (where
/// it cannot save), lets a blowing vent's steam push it south, and starts or
/// stops the vents' sound.
void KandoreanTemple_WatchVents(void) {
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
  if (gKandoreanTemple_WestVentBlowing != 0 && leader->y.fixed == 0 &&
      leader->x.fixed > toFIXED(284) && leader->x.fixed < toFIXED(308) &&
      leader->z.fixed < toFIXED(200) && !Event_GetFlag(FLAG_PILLAR_MOVING))
    KandoreanTemple_PushSouth(leader, toFIXED(2.4));
  if (gKandoreanTemple_EastVentBlowing != 0 && leader->y.fixed == 0 &&
      leader->x.fixed > toFIXED(330) && leader->x.fixed <= toFIXED(358) - 1 &&
      leader->z.fixed < toFIXED(216) && !Event_GetFlag(FLAG_PILLAR_MOVING))
    KandoreanTemple_PushSouth(leader, toFIXED(3.6));
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
  FIXED x = leader->x.fixed;
  s32 cellX = FIELD_CELL(x);
  FIXED z = leader->z.fixed;
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
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  GameFlag_Clear(FLAG_ON_WEST_LIFT);
  GameFlag_Clear(FLAG_ON_EAST_LIFT);
  GameFlag_Clear(FLAG_NO_PLACE_TO_SAVE);
}

/// Poi's room's task: draws the three tested students at the leader's sprite
/// priority, in front of or behind the room's scenery as the leader is.
void KandoreanTemple_MatchStudentsToLeader(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  s32 priority = ((struct FieldSprite *)leader->sprite)->priority;

  Actor_SetSpritePriority(ACTOR_FIRST_TESTED_STUDENT, priority);
  Actor_SetSpritePriority(ACTOR_SECOND_TESTED_STUDENT, priority);
  Actor_SetSpritePriority(ACTOR_THIRD_TESTED_STUDENT, priority);
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

/// Felix checks the temple's door in the middle of the courtyard, cells
/// (17, 25) and (18, 25), before it is open: "The door is tightly locked."
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

/// Felix pushes towards the opening in the floor of Poi's hall, cell (10, 5),
/// from beside or below it: he climbs down the ladder into the Chestbeaters'
/// room.
void KandoreanTemple_ClimbDownTheLadder(void) {
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
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  FieldActor_SetMoveTarget(leader, 168, -12, 80);
  Actor_WaitForMove(leader);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_LEADER_CLIMB);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_CHESTBEATERS);
}

/// Opens the door to the cave below Poi's statue.
void KandoreanTemple_OpenTheCaveDoor(void) {
  MapGrid_CopyCellPictureIndicesRect(39, 39, 24, 38, 2, 2);
  wait(10);
  MapGrid_CopyCellPictureIndicesRect(39, 41, 24, 38, 2, 2);
  wait(20);
  GameFlag_Set(FLAG_CAVE_DOOR_OPEN);
}

/// Felix walks north into the door below Poi's statue, cells (24, 40) and
/// (25, 40): it opens with a thump if it is shut, and he goes through into the
/// caverns of the trials.
void KandoreanTemple_EnterTheCave(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1);
  Actor_WalkToAndWait(gMainRoutineWork.leaderActorId, 400, 648);
  Actor_FaceDirectionAndRefreshById(gMainRoutineWork.leaderActorId,
                                    FACING_NORTH);
  if (GameFlag_Get(FLAG_CAVE_DOOR_OPEN) == 0) {
    Sound_Play(SE_THUMP);
    KandoreanTemple_OpenTheCaveDoor();
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 3);
  Actor_WalkToById(gMainRoutineWork.leaderActorId, 400, 640);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_MIMIC);
}

/// Felix walks south into the stone well in the courtyard's east side, cells
/// (25, 17) to (26, 18): he climbs down into it, to the passageway at its
/// bottom that examining the well tells of, and comes out in the east end of
/// the cave of shallow water.
void KandoreanTemple_ClimbDownTheWell(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 frame;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 416, 288);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  leader->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  for (frame = 0; frame < 8; frame++) {
    leader->z.fixed += toFIXED(1);
    wait(1);
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId,
                         ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
  for (frame = 0; frame < 8; frame++) {
    leader->y.fixed -= toFIXED(1);
    wait(1);
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_EAST);
}

/// Move takes hold of the pillar in the lift room, or Felix pushes it.
void KandoreanTemple_StartMovingBlock(void) {
  Actor_LiftTargetBlock(gKandoreanTemple_BlockActors);
}

/// The pillar in the lift room comes to rest; pushed east as far as column
/// 43, over the shaft at cells (43, 10) to (43, 13), it blocks the shaft, and
/// Felix can walk across without sinking into it.
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

/// Move takes hold of the pillar in the vent room, or Felix pushes it; lifted
/// off a vent's cover, that vent is free to blow again.
void KandoreanTemple_StartMovingPillar(void) {
  if ((u32)(FIELD_CELL(Actor_GetById(ACTOR_VENT_ROOM_PILLAR)->x.fixed) - 15) >
      6)
    GameFlag_Set(FLAG_PILLAR_LIFTED_OFF_VENT);
  GameFlag_Set(FLAG_PILLAR_MOVING);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

/// The pillar in the vent room comes to rest. On the west vent's round cover,
/// cell (13, 13), or the east one's, cell (23, 15), it weighs that vent down,
/// and the other vent's column rises higher, up to the lift room.
void KandoreanTemple_FinishMovingPillar(void) {
  struct Actor *actor = Actor_GetById(ACTOR_VENT_ROOM_PILLAR);
  s32 x = FIELD_CELL(actor->x.fixed);
  s32 z = FIELD_CELL(actor->z.fixed);

  Actor_PlaceMovedBlock();
  if (x == 13 && z == 13)
    GameFlag_Set(FLAG_PILLAR_ON_WEST_VENT);
  else
    GameFlag_Clear(FLAG_PILLAR_ON_WEST_VENT);
  if (x == 23 && z == 15)
    GameFlag_Set(FLAG_PILLAR_ON_EAST_VENT);
  else
    GameFlag_Clear(FLAG_PILLAR_ON_EAST_VENT);
  GameFlag_Clear(FLAG_PILLAR_LIFTED_OFF_VENT);
  GameFlag_Clear(FLAG_PILLAR_MOVING);
}

/// Move takes hold of the pot boiling over the hot floor in the Chestbeaters'
/// room.
void KandoreanTemple_StartMovingPot(void) {
  GameFlag_Clear(FLAG_POT_PUSHED_WEST);
  GameFlag_Clear(FLAG_POT_PUSHED_EAST);
  Actor_LiftTargetBlock(gKandoreanTemple_PillarActors);
}

/// The pot of boiling water, chucked one cell west or east by Move, lands out
/// of the way, and the floor round where it stood, cells (19, 28) to (21, 30),
/// stops burning.
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

/// Felix walks onto the hot floor under the pot of boiling water, from the
/// west, south or east of it: he jumps back with a start, his sprite flushed
/// red.
void KandoreanTemple_TouchTheHotFloor(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmoteById(gMainRoutineWork.leaderActorId,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Sound_Play(SE_WATER);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_RED_TINT);
  leader->velocityY = toFIXED(6);
  wait(20);
  while (leader->velocityY != 0)
    wait(1);
  Actor_SetSpriteColor(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_COLOR_NORMAL);
  Event_End();
}

/// Once Felix has met Poi, the temple's wooden door in the middle of the
/// courtyard swings open with a thump as he walks up to it, from the courtyard
/// (cell (17, 26)) or from inside (cell (17, 24)). Its cells, (17, 25) and
/// (18, 25), stop answering "The door is tightly locked." and let him through.
void KandoreanTemple_OpenTheTempleDoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_THUMP);
  MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
  MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  wait(10);
  GameFlag_Set(FLAG_TEMPLE_DOOR_OPEN);
  Event_End();
}

/// Whirlwind has scattered the weeds: the passage behind them opens and Kraden
/// follows the leader in.
void KandoreanTemple_FindTheHiddenPassage(void) {
  struct Actor *actor;

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
  actor = Actor_GetById(ACTOR_FELIX);
  if (actor)
    Actor_SetDestinationById(ACTOR_GATE_KRADEN, actor->x.part[0].integer,
                             actor->z.part[0].integer);
  Actor_WaitForMoveById(ACTOR_GATE_KRADEN);
  Actor_SetPosition(ACTOR_GATE_KRADEN, 0, 0);
  Event_End();
}

/// Examining the Lash Pebble in Poi's room: Poi gives the party Lash and tells
/// how to use it.
void KandoreanTemple_ReceiveLash(void) {
  s16 angle = *(s16 *)&Actor_GetById(gMainRoutineWork.leaderActorId)->facing;
  struct Actor *actor;
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
    ItemDisplay_ShowPicture(gKandoreanTemple_LashPebble,
                            ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
  Party_GiveItem(ITEM_LASH_PEBBLE, 0);
  if (gKandoreanTemple_LashPebble != 0)
    Actor_Release(gKandoreanTemple_LashPebble);
  Actor_SetAnimation(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "I hope that someday, my students will be worthy of this honor, too."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  FieldCamera_SetSpeed(0.4);
  FieldCamera_MoveTo(424, 0, 400, 1);
  actor = Actor_GetById(ACTOR_POI);
  sprite = actor->sprite;
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 424, 376);
  FieldActor_SetSpritePriority(ACTOR_POI, 1);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  FieldActor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 444, 376);
  Actor_SetAnimation(ACTOR_POI, 7);
  actor->scaleX = -FIXED_ONE;
  sprite->rotation = TRIG_TURN * 7 / 8;
  actor->motionFlags = 0;
  actor->y.fixed += toFIXED(4);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpeed(ACTOR_POI, 0.4);
  FieldActor_SetMoveTarget(actor, 492, 8, 376);
  FieldActor_WaitForMoveById(ACTOR_POI);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  actor->groundHeight = 0;
  actor->scaleX = FIXED_ONE;
  sprite->rotation = 0;
  Actor_SetAnimation(ACTOR_POI, ANIM_STAND);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), SPRITE_FLAG_SHADOW);
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

/// Nothing visible happens as Felix steps behind a post of the rope across to
/// Fog; he is drawn behind the post (`Actor_SetSpritePriority`) while he is
/// there.
void KandoreanTemple_StepBehindThePost(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
}

/// Nothing visible happens as Felix steps out from behind the rope's post; he
/// is drawn in front of it again.
void KandoreanTemple_StepOutFromThePost(void) {
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags &=
      ~ACTOR_PRIORITY_IGNORES_RISES;
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
}

/// Felix walks up to the Chestbeaters at the north end of their room, cells
/// (19, 12) to (21, 12): the three leap down at him with a cry and their
/// battle starts.
void KandoreanTemple_FightChestbeaters(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(328, CAMERA_KEEP, 174, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 1.6);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 328, 174);
  Actor_RunRepeatedMotion(ACTOR_MIDDLE_CHESTBEATER, 2);
  Sound_Play(SE_ENEMY_CRY);
  Actor_SetAnimationById(ACTOR_MIDDLE_CHESTBEATER, 2);
  Actor_GetById(ACTOR_MIDDLE_CHESTBEATER)->velocityY = toFIXED(4);
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
  Event_SetAfterBattleDestination(SCENE_KANDOREAN_TEMPLE_CHESTBEATERS,
                                  ENTRANCE_AFTER_CHESTBEATER_BATTLE);
  Field_RequestBattleFromEncounterTable(9, 4);
  Event_End();
}

/// Felix casts Cyclone over the first of the five clumps of weeds in the
/// courtyard before the temple, at cells (13, 14) to (14, 15): leaves whirl up
/// and the clump is gone.
void KandoreanTemple_CycloneFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 2, 2, 73, 14, 1);
}

/// Felix casts Cyclone over the second of the five clumps of weeds in the
/// courtyard before the temple, at cell (15, 16): leaves whirl up and the clump
/// is gone.
void KandoreanTemple_CycloneSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 75, 16, 1);
}

/// Felix casts Cyclone over the third of the five clumps of weeds in the
/// courtyard before the temple, at cell (14, 18): leaves whirl up and the clump
/// is gone.
void KandoreanTemple_CycloneThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 74, 18, 1);
}

/// Felix casts Cyclone over the fourth of the five clumps of weeds in the
/// courtyard before the temple, at cell (20, 18): leaves whirl up and the clump
/// is gone.
void KandoreanTemple_CycloneFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 80, 18, 1);
}

/// Felix casts Cyclone over the fifth of the five clumps of weeds in the
/// courtyard before the temple, at cell (27, 15): leaves whirl up and the clump
/// is gone.
void KandoreanTemple_CycloneFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(76, 0, 1, 1, 87, 15, 1);
}

/// Sets up the room being entered.
s32 Scene_Initialize(void) {
  s32 scene;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
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
  else if (scene == SCENE_KANDOREAN_TEMPLE_CURRENT)
    KandoreanTemple_StartCurrentRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_FOG)
    KandoreanTemple_StartFogRoom();
  else if (scene == SCENE_KANDOREAN_TEMPLE_GATE)
    KandoreanTemple_StartGate();
  else if (scene == SCENE_KANDOREAN_TEMPLE_POI)
    KandoreanTemple_StartPoisRoom();
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Sets up the lift room: its tightropes, the pillar, the stream of steam and
/// the lifts, and the lift the leader arrives on.
void KandoreanTemple_StartLiftRoom(void) {
  Tightrope_Initialize();
  Tightrope_Place(0, ACTOR_WEST_ROPE_WEST_POST, ACTOR_WEST_ROPE_EAST_POST);
  Tightrope_Place(1, ACTOR_EAST_ROPE_WEST_POST, ACTOR_EAST_ROPE_EAST_POST);
  Actor_SetAnimationById(ACTOR_WEST_ROPE_WEST_POST, 4);
  Actor_SetAnimationById(ACTOR_WEST_ROPE_EAST_POST, 4);
  Actor_SetAnimationById(ACTOR_EAST_ROPE_WEST_POST, 4);
  Actor_SetAnimationById(ACTOR_EAST_ROPE_EAST_POST, 4);
  Actor_PlaceBlocks(gKandoreanTemple_BlockActors);
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  Scene_QueueMusic(SE_AMBIENT_STREAM);
  Task_Add(KandoreanTemple_BlowStream, TASK_PASS_FRAME);
  Actor_SetPosition(ACTOR_WEST_LIFT, 0, 0);
  Actor_SetPosition(ACTOR_EAST_LIFT, 0, 0);
  Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
  Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
  KandoreanTemple_PrepareLift(ACTOR_WEST_LIFT);
  KandoreanTemple_PrepareLift(ACTOR_EAST_LIFT);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_LIFT_COLUMN);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_LIFT_COLUMN);
  gKandoreanTemple_WestTimer = 0;
  gKandoreanTemple_EastTimer = 0;
  gKandoreanTemple_WestFloatPhase = 0;
  gKandoreanTemple_EastFloatPhase = 0;
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance ==
        KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT)
      KandoreanTemple_ArriveOnWestLift();
    else if (gMainRoutineWork.entrance ==
             KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT)
      KandoreanTemple_ArriveOnEastLift();
  }
  if (GameFlag_Get(FLAG_PILLAR_ON_EAST_VENT))
    Task_Add(KandoreanTemple_RunWestLift, TASK_PASS_FRAME);
  else if (GameFlag_Get(FLAG_PILLAR_ON_WEST_VENT))
    Task_Add(KandoreanTemple_RunEastLift, TASK_PASS_FRAME);
  Task_Add(KandoreanTemple_WatchLifts, TASK_PASS_FRAME);
  FieldEffect_StartBlankOverlay(0);
}

/// Sets up the Mimic's room. Outside the Japanese edition, coming into it
/// from Poi's hall does not show the area's name again.
void KandoreanTemple_StartMimicRoom(void) {
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance ==
      KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_POI)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the Chestbeaters' room: the pot where Move left it, the sign, and
/// the Chestbeaters gone once the ladder is lowered.
void KandoreanTemple_StartChestbeaterRoom(void) {
  struct Actor *actor = Actor_GetById(ACTOR_EMBERS_UNDER_THE_POT);

  Actor_DisableById(ACTOR_EMBERS_UNDER_THE_POT);
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetAnimationById(ACTOR_EMBERS_UNDER_THE_POT, 2);
  actor = Actor_GetById(ACTOR_BOILING_POT);
  actor->motionFlags = 0;
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
  actor = Actor_GetById(ACTOR_POIS_SIGN);
  actor->scaleX = FIXED_ONE * 7 / 10;
  actor->scaleY = FIXED_ONE * 7 / 10;
  actor->collisionFlags = 0;
  FieldEffect_StartBlankOverlay(0);
  if (Event_GetFlag(FLAG_LADDER_LOWERED)) {
    {
      s32 e = 1;
      s32 f = 4;

      MapGrid_CopyCellPictureIndicesRect(41, 0, 20, 0, e, f);
    }
    Actor_SetPosition(ACTOR_MIDDLE_CHESTBEATER, 0, 0);
    Actor_SetPosition(ACTOR_WEST_CHESTBEATER, 0, 0);
    Actor_SetPosition(ACTOR_EAST_CHESTBEATER, 0, 0);
  }
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    if (gMainRoutineWork.entrance ==
        KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI) {
      Actor_GetById(gMainRoutineWork.leaderActorId)->y.fixed = toFIXED(64);
    } else if (gMainRoutineWork.entrance == ENTRANCE_AFTER_CHESTBEATER_BATTLE &&
               Event_GetFlag(FLAG_LADDER_LOWERED) == 0) {
      Actor_SetPosition(ACTOR_MIDDLE_CHESTBEATER, 0, 0);
      Actor_SetPosition(ACTOR_WEST_CHESTBEATER, 0, 0);
      Actor_SetPosition(ACTOR_EAST_CHESTBEATER, 0, 0);
      KandoreanTemple_LowerTheLadder();
    }
  }
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance ==
      KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the vent room: the covers and columns at rest, the pillar where
/// Move left it, the vents' steam and the tasks that run them, and the ride
/// down or the fall the leader arrives by.
void KandoreanTemple_StartVentRoom(void) {
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_VENT_COVER),
                                         toFIXED(216), 0, toFIXED(216));
    Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
    Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
    Actor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_VENT_COVER),
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
  if (GameFlag_Get(FLAG_PILLAR_ON_WEST_VENT))
    Actor_SetPosition(ACTOR_VENT_ROOM_PILLAR, 216, 216);
  else if (GameFlag_Get(FLAG_PILLAR_ON_EAST_VENT))
    Actor_SetPosition(ACTOR_VENT_ROOM_PILLAR, 376, 248);
  REG_BLDALPHA = BLEND_ALPHA(8, 12);
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  gKandoreanTemple_WestVentBlowing = 0;
  gKandoreanTemple_EastVentBlowing = 0;
  Task_AddFrame(KandoreanTemple_BlowWestVent, TASK_PASS_FRAME);
  Task_AddFrame(KandoreanTemple_BlowEastVent, TASK_PASS_FRAME);
  Task_Add(KandoreanTemple_DropOffLowCells, TASK_PASS_FRAME);
  KandoreanTemple_PrepareLift(ACTOR_WEST_VENT_COVER);
  KandoreanTemple_PrepareLift(ACTOR_EAST_VENT_COVER);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_COLUMN_TOP);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_COLUMN_UPPER_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_COLUMN_LOWER_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_WEST_COLUMN_FOOT);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_COLUMN_TOP);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_COLUMN_UPPER_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_COLUMN_LOWER_PIECE);
  KandoreanTemple_PrepareColumn(ACTOR_EAST_COLUMN_FOOT);
  Actor_PlaceBlocks(gKandoreanTemple_PillarActors);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
    if (gMainRoutineWork.entrance ==
        KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT)
      KandoreanTemple_ArriveOnWestVent();
    else if (gMainRoutineWork.entrance ==
             KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT)
      KandoreanTemple_ArriveOnEastVent();
  }
  gKandoreanTemple_WestTimer = 0;
  // The east vent starts at rest, out of step with the west.
  gKandoreanTemple_EastTimer = VENT_SHUT + 1000;
  gKandoreanTemple_WestFloatPhase = 0;
  gKandoreanTemple_EastFloatPhase = 0;
  Task_AddFrame(KandoreanTemple_RunWestVent, TASK_PASS_FRAME);
  Task_Add(KandoreanTemple_RunEastVent, TASK_PASS_FRAME);
  if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_SHAFT)
    KandoreanTemple_LandFromAbove();
  Task_AddFrame(KandoreanTemple_WatchVents, TASK_PASS_FRAME);
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 2);
  FieldEffect_StartBlankOverlay(0);
}

#include "../field/field_effect_advance_motion.c"

#undef DEFINE_KANDOREAN_TEMPLE_VENT
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
// The Kandorean Temple's events, from the vents' rides to Poi's test, and its
// tables.

/// Felix drops into the vent room from the lift room's shaft above and lands in
/// a puff of dust that shakes the screen.
void KandoreanTemple_LandFromAbove(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct EffectOptions options;
  u32 particleIndex;

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
  options.startScaleX = FIXED_ONE * 4 / 5;
  options.startScaleY = FIXED_ONE * 4 / 5;
  for (particleIndex = 0; particleIndex <= DUST_RING_PUFFS; particleIndex++) {
    FIXED velocity[3];

    velocity[0] = Trig_Cos(particleIndex * (TRIG_TURN / DUST_RING_PUFFS));
    velocity[1] = 0;
    velocity[2] = Trig_Sin(particleIndex * (TRIG_TURN / DUST_RING_PUFFS));
    velocity[0] += velocity[0] >> 1;
    velocity[0] += ((Random16() * 3) << 14) >> 16;
    velocity[0] += -toFIXED(3) / 8;
    velocity[2] += (((Random16() * 3) << 13) >> 16) + -toFIXED(3) / 16;
    FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                      velocity[0], velocity[1], velocity[2],
                      EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE |
                          EFFECT_USE_PALETTE | 1,
                      &options);
  }
  Sound_Play(SE_THUMP);
  FieldEffect_SetAttached(gMainRoutineWork.leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_LEADER_SIT);
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

/// Coming down the west lift from the room above: the party arrives atop the
/// west vent's cover, raised high on its column, and sinks with it to the
/// ground.
void KandoreanTemple_ArriveOnWestVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *cover = Actor_GetById(ACTOR_WEST_VENT_COVER);
  struct Actor *camera = FieldCamera_GetActor();
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
  GameFlag_Set(FLAG_PILLAR_ON_EAST_VENT);
  GameFlag_Clear(FLAG_PILLAR_ON_WEST_VENT);
  Sound_Play(SE_WAVE);
  KandoreanTemple_StartWestVent();
  i = 0;
  do {
    leader->y.fixed += toFIXED(9) / 2;
    cover->y.fixed += toFIXED(9) / 2;
    camera->y.fixed += toFIXED(9) / 5;
    limit = FIXED_ONE / 16;
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_TOP, toFIXED(2), limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_UPPER_PIECE, toFIXED(3) / 2,
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_MIDDLE_PIECE, toFIXED(1),
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_LOWER_PIECE, toFIXED(1) / 2,
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_WEST_COLUMN_FOOT, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next < VENT_RISE_END - VENT_BLOW);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
#define SINK_COVER(top, upper, middle, lower, foot)                            \
  do {                                                                         \
    leader->y.fixed += -toFIXED(27) / 4;                                       \
    cover->y.fixed += -toFIXED(27) / 4;                                        \
    camera->y.fixed += -toFIXED(27) / 10;                                      \
    limit = FIXED_ONE * 3 / 32;                                                \
    KandoreanTemple_ShrinkColumn(top, toFIXED(3), limit);                      \
    KandoreanTemple_ShrinkColumn(upper, toFIXED(9) / 4, limit);                \
    KandoreanTemple_ShrinkColumn(middle, toFIXED(3) / 2, limit);               \
    KandoreanTemple_ShrinkColumn(lower, toFIXED(3) / 4, limit);                \
    KandoreanTemple_ShrinkColumn(foot, 0, limit);                              \
    wait(1);                                                                   \
  } while (Actor_GetById(foot)->scaleY > limit);

  SINK_COVER(ACTOR_WEST_COLUMN_TOP, ACTOR_WEST_COLUMN_UPPER_PIECE,
             ACTOR_WEST_COLUMN_MIDDLE_PIECE, ACTOR_WEST_COLUMN_LOWER_PIECE,
             ACTOR_WEST_COLUMN_FOOT)
  gKandoreanTemple_WestVentBlowing = 0;
  GameFlag_Clear(FLAG_WEST_VENT_BLOWING);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_WEST_COLUMN_TOP, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_UPPER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_MIDDLE_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_LOWER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_WEST_COLUMN_FOOT, 0, 0);
  Actor_SetAnimationById(ACTOR_WEST_VENT_COVER, 1);
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_WEST_VENT_COVER),
                                       toFIXED(216), 0, toFIXED(216));
  cover->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_WEST_VENT_COVER, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0, leader->z.fixed);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Coming down the east lift from the room above: the party arrives atop the
/// east vent's cover, raised high on its column, and sinks with it to the
/// ground.
void KandoreanTemple_ArriveOnEastVent(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  struct Actor *cover = Actor_GetById(ACTOR_EAST_VENT_COVER);
  struct Actor *camera = FieldCamera_GetActor();
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
  GameFlag_Clear(FLAG_PILLAR_ON_EAST_VENT);
  GameFlag_Set(FLAG_PILLAR_ON_WEST_VENT);
  Sound_Play(SE_WAVE);
  KandoreanTemple_StartEastVent();
  i = 0;
  do {
    leader->y.fixed += toFIXED(9) / 2;
    cover->y.fixed += toFIXED(9) / 2;
    camera->y.fixed += toFIXED(9) / 5;
    limit = FIXED_ONE / 16;
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_TOP, toFIXED(2), limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_UPPER_PIECE, toFIXED(3) / 2,
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_MIDDLE_PIECE, toFIXED(1),
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_LOWER_PIECE, toFIXED(1) / 2,
                               limit);
    KandoreanTemple_GrowColumn(ACTOR_EAST_COLUMN_FOOT, 0, limit);
    next = i;
    next++;
    i = next;
  } while (next < VENT_RISE_END - VENT_BLOW);
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  SINK_COVER(ACTOR_EAST_COLUMN_TOP, ACTOR_EAST_COLUMN_UPPER_PIECE,
             ACTOR_EAST_COLUMN_MIDDLE_PIECE, ACTOR_EAST_COLUMN_LOWER_PIECE,
             ACTOR_EAST_COLUMN_FOOT)
  gKandoreanTemple_EastVentBlowing = 0;
  GameFlag_Clear(FLAG_EAST_VENT_BLOWING);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  Actor_SetPosition(ACTOR_EAST_COLUMN_TOP, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_UPPER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_MIDDLE_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_LOWER_PIECE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_COLUMN_FOOT, 0, 0);
  Actor_SetAnimationById(ACTOR_EAST_VENT_COVER, 1);
  FieldActor_SetPositionAndResetMotion(Actor_GetById(ACTOR_EAST_VENT_COVER),
                                       toFIXED(376), 0, toFIXED(248));
  cover->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_SetSpritePriority(ACTOR_EAST_VENT_COVER, 3);
  Actor_SetPositionAndResetMotion(leader, leader->x.fixed, 0, leader->z.fixed);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId),
                       SPRITE_FLAG_SHADOW);
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Event_End();
}

/// Felix rises out of the vent room on a vent's column, the screen closing in
/// on him.
void KandoreanTemple_LeaveBySteam(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = 0;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}

/// Felix rides the west vent's round cover as its column lifts it, up through
/// the ceiling to the west lift in the room above.
void KandoreanTemple_LeaveByWestVent(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT);
}

/// Felix rides the east vent's round cover as its column lifts it, up through
/// the ceiling to the east lift in the room above.
void KandoreanTemple_LeaveByEastVent(void) {
  KandoreanTemple_LeaveBySteam();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT);
}

/// Felix steps onto the lift room's shaft, cells (43, 10) to (43, 13), while
/// the pillar does not block it: he slips back, throws up his arms and sinks
/// out of sight, dropping into the vent room below.
void KandoreanTemple_SinkIntoTheSteam(void) {
  struct Actor *actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  u32 frame;
  struct MainRoutineWork *party;

  Event_Start();
  Event_ResolvePendingActions(0);
  actor->motionFlags = 0;
  Actor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
  Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);
  actor->facing = 0;
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, 16);
  for (frame = 0; frame <= 7; frame++) {
    Actor_SetPositionAndResetMotion(actor, actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed + toFIXED(3));
    wait(1);
  }
  Event_Wait(20);
  party = &gMainRoutineWork;
  actor->facing = FACING_SOUTH;
  FieldEffect_SetAttached(party->leaderActorId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAnimationById(party->leaderActorId, ANIM_LEADER_WALK_BACKWARDS);
  Event_Wait(30);
  Actor_SetAnimationById(party->leaderActorId, ANIM_LEADER_ARMS_RAISED);
  Event_Wait(10);
  Actor_SetSpritePriority(party->leaderActorId, 3);
  for (frame = 0; frame <= 39; frame++) {
    {
      s32 scale = actor->scaleX;

      actor->scaleX = scale + -FIXED_ONE / 64;
    }
    actor->scaleY += -FIXED_ONE / 64;
    actor->y.fixed += -toFIXED(2);
    wait(1);
    if (frame == 10)
      Sound_Play(SE_DUST_PUFF);
    if (frame == 20) {
      gFieldWork->transition =
          SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
      Event_FinishScreenTransition();
    }
  }
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_SHAFT);
}

/// Felix rises into view on a lift while its column grows under it.
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
    Actor_SetSpriteFlags(Actor_GetById(gMainRoutineWork.leaderActorId), 0);    \
    FieldActor_SetPositionAndResetMotion(Actor_GetById(liftId), toFIXED(x),    \
                                         toFIXED(-20), toFIXED(liftZ));        \
    FieldActor_SetPositionAndResetMotion(Actor_GetById(columnId), toFIXED(x),  \
                                         toFIXED(-20), toFIXED(z));            \
    Actor_SetSpritePriority(liftId, 2);                                        \
    Actor_SetSpritePriority(columnId, 3);                                      \
    {                                                                          \
      u8 *flags = &second->priorityFlags;                                      \
      s32 c = ACTOR_PRIORITY_UNDERFOOT | *flags;                               \
                                                                               \
      *flags = c;                                                              \
    }                                                                          \
    second->scaleY = FIXED_ONE / 64;                                           \
    Actor_SetAnimationById(liftId, 4);                                         \
    Actor_SetAnimationById(columnId, 2);                                       \
    gFieldWork->transition =                                                   \
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);         \
    Event_StartScreenTransition();                                             \
    Event_WaitTransitionFrames();                                              \
    do {                                                                       \
      first->y.fixed += toFIXED(4);                                            \
      leader->y.fixed += toFIXED(4);                                           \
      KandoreanTemple_GrowColumn(columnId, toFIXED(0.3), toFIXED(0.3));        \
      wait(1);                                                                 \
    } while (first->y.fixed < 0);                                              \
    MapGrid_CopyCellAttributes(0, 0, 1, 1, cellX, cellZ);                      \
    timer = 2000;                                                              \
    gFieldWork->transition = SCENE_TRANSITION(                                 \
        TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);                 \
    Event_End();                                                               \
  }

/// Arriving on the west lift, cell (27, 12), from the vent room below: Felix
/// rises into the room on it as its column grows beneath, and the lift's cycle
/// goes on from LIFT_RISEN.
DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnWestLift, ACTOR_WEST_LIFT,
                             ACTOR_WEST_LIFT_COLUMN, 440, 200, 204, 27, 12,
                             gKandoreanTemple_WestTimer)

/// Arriving on the east lift, cell (37, 14), from the vent room below: Felix
/// rises into the room on it as its column grows beneath.
DEFINE_KANDOREAN_TEMPLE_RISE(KandoreanTemple_ArriveOnEastLift, ACTOR_EAST_LIFT,
                             ACTOR_EAST_LIFT_COLUMN, 600, 232, 236, 37, 14,
                             gKandoreanTemple_EastTimer)

/// Felix steps onto the west lift in the lift room, cell (27, 12), while it
/// stands risen: it sinks out of sight with him, down its column to the west
/// vent in the room below.
void KandoreanTemple_RideWestLiftDown(void) {
  struct Actor *actor = Actor_GetById(ACTOR_WEST_LIFT);

  Event_Start();
  Event_ResolvePendingActions(0);
  do {
    actor->y.fixed -= toFIXED(4);
    KandoreanTemple_ShrinkColumn(ACTOR_WEST_LIFT_COLUMN, toFIXED(0.3),
                                 toFIXED(0.3));
    wait(1);
  } while (actor->y.fixed > -toFIXED(20));
  Actor_SetPosition(ACTOR_WEST_LIFT_COLUMN, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT);
}

/// Felix steps onto the east lift in the lift room, cell (37, 14), while it
/// stands risen: it sinks out of sight with him, down its column to the east
/// vent in the room below.
void KandoreanTemple_RideEastLiftDown(void) {
  struct Actor *actor = Actor_GetById(ACTOR_EAST_LIFT);

  Event_Start();
  Event_ResolvePendingActions(0);
  do {
    actor->y.fixed -= toFIXED(4);
    KandoreanTemple_ShrinkColumn(ACTOR_EAST_LIFT_COLUMN, toFIXED(0.3),
                                 toFIXED(0.3));
    wait(1);
  } while (actor->y.fixed > -toFIXED(20));
  Actor_SetPosition(ACTOR_EAST_LIFT_COLUMN, 0, 0);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(
      KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT);
}

/// Sets up the cave of shallow water behind the bush by the gate, where the
/// party wades.
void KandoreanTemple_StartStreamRoom(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;

  gKandoreanTemple_SplashRingCounter = 0;
  map->wading = 1;
  FieldEffect_StartBlankOverlay(0);
#if TLA_PATCH_1
  if (gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_WEST ||
      gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_STREAM_FROM_KANDOREAN_TEMPLE_GATE_EAST)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Sets up the room of the current, with the rush of its water.
void KandoreanTemple_StartCurrentRoom(void) {
  struct MapWorkGroundEffects *map =
      (struct MapWorkGroundEffects *)Ram_HeapSlots->mapGrid;

  gKandoreanTemple_SplashRingCounter = 0;
  Scene_QueueMusic(SE_AMBIENT_WATER);
  map->wading = 1;
  FieldEffect_StartBlankOverlay(0);
}

/// Sets up Fog's room: the rope to Fog and, until the party has met Poi, the
/// student who gave up, sitting on the floor.
void KandoreanTemple_StartFogRoom(void) {
  Rope_Initialize();
  Rope_Place(0, FLAG_FOG_ROPE_TIED, ACTOR_FOG_ROPE_POST,
             ACTOR_FOG_ROPE_OTHER_POST);
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
void KandoreanTemple_StartMeditating(s32 id, s32 delay) {
  struct Actor *actor = Actor_GetById(id);

  Actor_SetAnimationById(id, 5);
  actor->timer = delay + (Random_Below(400));
  actor->userValue = id;
  Actor_SetMovementTypeById(id, gKandoreanTemple_MeditateScript);
}

/// Sets up the temple gate: the weeds over the hidden passage gone once
/// Whirlwind has scattered them, the students meditating or, once Poi has been
/// met, standing by with the cells that open the temple's door laid before it,
/// the door open if it was opened this visit, and Kraden.
void KandoreanTemple_StartGate(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  if (GameFlag_Get(FLAG_HIDDEN_PASSAGE_FOUND)) {
    Actor_SetPosition(ACTOR_WEEDS_OVER_THE_CAVE, 0, 0);
    MapGrid_CopyCellAttributes(0, 0, 1, 1, 5, 13);
  } else {
    Actor_DisableById(ACTOR_WEEDS_OVER_THE_CAVE);
  }
  if (GameFlag_Get(FLAG_MET_POI)) {
    MapGrid_CopyCellAttributes(2, 0, 2, 3, 17, 24);
    Actor_SetPosition(ACTOR_STUDENT_ASKING_ABOUT_POI, 0, 0);
    Actor_SetPosition(ACTOR_STUDENT_ON_GUARD_DUTY, 0, 0);
    Actor_SetPositionAndFacing(ACTOR_STUDENT_WARY_OF_OUTSIDERS, toFIXED(374),
                               toFIXED(282), FACING_EAST);
    {
      Actor_SetPositionAndFacing(ACTOR_STUDENT_ON_THE_FAILED_TEST, toFIXED(272),
                                 toFIXED(296), FACING_NORTH_NORTHEAST);
    }
    {
      Actor_SetPositionAndFacing(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
                                 toFIXED(216), toFIXED(264),
                                 FACING_SOUTH_SOUTHEAST);
    }
  } else {
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_WARY_OF_OUTSIDERS, 0);
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_ON_THE_FAILED_TEST, 240);
    KandoreanTemple_StartMeditating(ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, 480);
  }
  if (GameFlag_Get(FLAG_TEMPLE_DOOR_OPEN)) {
    MapGrid_CopyCellPictureIndicesRect(77, 33, 77, 24, 2, 2);
    MapGrid_CopyCellAttributes(32, 30, 2, 1, 17, 25);
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_WEST ||
      gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_EAST)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Before the party has Lash, Poi waits by the north rope, which he has tied
/// across (FLAG_NORTH_ROPE_TIED) the first time on each visit.
void KandoreanTemple_PoiWaitsByTheNorthRope(void) {
  if (Event_GetFlag(FLAG_NORTH_ROPE_TIED) == 0) {
    Event_SetFlag(FLAG_NORTH_ROPE_TIED);
    Call4(Rope_Place, 1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POST,
          ACTOR_NORTH_ROPE_WEST_POST);
  }
  {
    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(392), toFIXED(360),
                               FACING_SOUTH_SOUTHEAST);
  }
}

/// Sets up Poi's hall: its ropes, the tested students, Poi and the cave door,
/// by the entrance the party takes.
void KandoreanTemple_StartPoisRoom(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Rope_Initialize();
  Call4(Rope_Place, 0, FLAG_SOUTH_ROPE_TIED, ACTOR_SOUTH_ROPE_WEST_POST,
        ACTOR_SOUTH_ROPE_EAST_POST);
  Rope_Place(1, FLAG_NORTH_ROPE_TIED, ACTOR_NORTH_ROPE_EAST_POST,
             ACTOR_NORTH_ROPE_WEST_POST);
  if (Event_GetFlag(FLAG_MET_POI) == 0) {
    Actor_SetAnimationById(ACTOR_FIRST_TESTED_STUDENT, 5);
    Actor_SetAnimationById(ACTOR_THIRD_TESTED_STUDENT, 5);
  }
  Actor_SetAnimationById(ACTOR_SECOND_TESTED_STUDENT, 5);
  Task_Add(KandoreanTemple_MatchStudentsToLeader, TASK_PASS_FRAME);
  if ((gMainRoutineWork.entrance >=
           KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST &&
       gMainRoutineWork.entrance <=
           KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE) ||
      gMainRoutineWork.entrance == KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE) {
    if (GameFlag_Get(FLAG_LASH_RECEIVED)) {
      Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(432), toFIXED(394),
                                 FACING_SOUTH_SOUTHWEST);
    } else {
      KandoreanTemple_CreateLashPebble();
      if (GameFlag_Get(FLAG_MET_POI))
        KandoreanTemple_PoiWaitsByTheNorthRope();
    }
  }
  if ((gMainRoutineWork.entrance >=
           KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_GATE &&
       gMainRoutineWork.entrance <=
           KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST) ||
      gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC) {
    if (GameFlag_Get(FLAG_CAVE_DOOR_OPEN))
      KandoreanTemple_OpenTheCaveDoor();
    if (GameFlag_Get(FLAG_MET_POI)) {
      Actor_SetPosition(ACTOR_POI, 0, 0);
      {
        Actor_SetPositionAndFacing(ACTOR_THIRD_TESTED_STUDENT, toFIXED(408),
                                   toFIXED(664), FACING_NORTH_NORTHWEST);
      }
    } else if (GameFlag_Get(FLAG_STUDENTS_TESTED)) {
      Actor_SetPosition(ACTOR_POI, 0, 0);
      Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
    } else {
      KandoreanTemple_TestTheStudents();
    }
  } else if (gMainRoutineWork.entrance ==
             KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE) {
    if (GameFlag_Get(FLAG_MET_POI) == 0)
      KandoreanTemple_PassTheTest();
  } else if (gMainRoutineWork.entrance ==
             KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR) {
    KandoreanTemple_FindTheRopeOnTheFloor();
  } else if (gMainRoutineWork.entrance ==
             KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE) {
    if (GameFlag_Get(FLAG_MET_POI) == 0)
      KandoreanTemple_MeetPoi();
  }
#if TLA_PATCH_1 /* From patch 1. */
  if (gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_CHESTBEATERS ||
      gMainRoutineWork.entrance ==
          KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC)
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
#endif
}

/// Turns a sprite by step (of TRIG_TURN to the turn) each frame for the given
/// frames.
void KandoreanTemple_TurnSprite(struct FieldSprite *sprite, u32 frames,
                                s32 step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    sprite->rotation += step;
    Event_Wait(1);
  }
}

/// Moves an actor sideways by step each frame for the given frames.
void KandoreanTemple_Drift(struct Actor *actor, u32 frames, FIXED step) {
  u32 count = frames;
  u32 i;

  for (i = 0; i < count; i++) {
    actor->x.fixed += step;
    Event_Wait(1);
  }
}

/// Poi tests his three students in turn: the first two fall, and the third
/// levitates and passes.
void KandoreanTemple_TestTheStudents(void) {
  struct Actor *actor;
  struct FieldSprite *sprite;
  u32 frame;

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
  Actor_ConfigureSpriteColor(ACTOR_FIRST_TESTED_STUDENT, SPRITE_COLOR_BLINK);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_FIRST_TESTED_STUDENT, 0);
  Event_Wait(20);
  actor = Actor_GetById(ACTOR_FIRST_TESTED_STUDENT);
  sprite = actor->sprite;
  actor->motionFlags = 0;
  for (frame = 0; frame <= 63; frame++) {
    actor->y.fixed += toFIXED(0.05);
    Event_Wait(1);
  }
  KandoreanTemple_TurnSprite(sprite, 32, TRIG_TURN / 2048);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_FIRST_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  KandoreanTemple_TurnSprite(sprite, 32, -TRIG_TURN / 1024);
  KandoreanTemple_TurnSprite(sprite, 32, TRIG_TURN / 1024);
  KandoreanTemple_TurnSprite(sprite, 64, TRIG_TURN / 2048);
  FieldEffect_SetAttached(ACTOR_FIRST_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(5);
  Sound_Play(SE_DOOR_CLOSE);
  actor->velocityY = toFIXED(3);
  sprite->rotation = 0;
  Event_Wait(60);
  // "Did you see that, Master?"
  Event_ShowMessageAndWait(ACTOR_FIRST_TESTED_STUDENT, 0);
  FieldCamera_MoveTo(436, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 476, 664);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "No, no, no! You lack discipline!"
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
  // "Now, you!"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
  FieldCamera_SetSpeed(4.8);
  FieldCamera_MoveTo(342, 0, 688, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_SECOND_TESTED_STUDENT, 2);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_ConfigureSpriteColor(ACTOR_SECOND_TESTED_STUDENT, SPRITE_COLOR_BLINK);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_SECOND_TESTED_STUDENT, 0);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  actor = Actor_GetById(ACTOR_SECOND_TESTED_STUDENT);
  actor->motionFlags = 0;
  for (frame = 0; frame <= 63; frame++) {
    actor->y.fixed += toFIXED(3) / 20;
    Event_Wait(1);
  }
  KandoreanTemple_Drift(actor, 64, toFIXED(1) / 16);
  KandoreanTemple_Drift(actor, 128, -toFIXED(1) / 16);
  KandoreanTemple_Drift(actor, 128, toFIXED(1) / 16);
  KandoreanTemple_Drift(actor, 64, -toFIXED(1) / 16);
  Event_Wait(20);
  Actor_SetAttachedEffect(ACTOR_SECOND_TESTED_STUDENT,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_Wait(5);
  Sound_Play(SE_DOOR_CLOSE);
  actor->velocityY = toFIXED(3);
  Event_Wait(60);
  // "How was that, Master?"
  Event_ShowMessageAndWait(ACTOR_SECOND_TESTED_STUDENT, 0);
  FieldCamera_MoveTo(364, 72, 664, 1);
  FieldCamera_WaitForArrival();
  // "Improving...slowly! Continue your meditation!"
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 10);
  // "Your turn!"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  FieldCamera_MoveTo(400, 0, 688, 1);
  FieldCamera_WaitForArrival();
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 2);
  Sound_Play(SE_PSYNERGY_SPARKLE);
  Actor_ConfigureSpriteColor(ACTOR_THIRD_TESTED_STUDENT, SPRITE_COLOR_BLINK);
  Event_Wait(20);
  Actor_ConfigureSpriteColor(ACTOR_THIRD_TESTED_STUDENT, 0);
  Event_Wait(20);
  actor = Actor_GetById(ACTOR_THIRD_TESTED_STUDENT);
  sprite = actor->sprite;
  actor->motionFlags = 0;
  for (frame = 0; frame <= 63; frame++) {
    actor->y.fixed += toFIXED(1) / 5;
    Event_Wait(1);
  }
  KandoreanTemple_TurnSprite(sprite, 32, TRIG_TURN / 1024);
  sprite->rotation = 0;
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
  Event_Wait(60);
  KandoreanTemple_TurnSprite(sprite, 32, -TRIG_TURN / 1024);
  sprite->rotation = 0;
  Actor_RunRepeatedMotion(ACTOR_THIRD_TESTED_STUDENT, 1);
  Event_Wait(60);
  for (frame = 0; frame <= 63; frame++) {
    actor->y.fixed += -toFIXED(1) / 5;
    Event_Wait(1);
  }
  Event_Wait(20);
  Actor_ShowEmoteById(ACTOR_THIRD_TESTED_STUDENT, EMOTE_IN_FRONT | EMOTE_SMILE,
                      0);
  // "Master, have I done it?"
  Event_ShowMessageThenWait(ACTOR_THIRD_TESTED_STUDENT, 0, 20);
  // "Not bad... You might stand a chance to succeed..."
  Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_THIRD_TESTED_STUDENT,
                      EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
  Actor_SetAnimationById(ACTOR_THIRD_TESTED_STUDENT, ANIM_STAND);
  Actor_Jump(ACTOR_THIRD_TESTED_STUDENT, 4, 0);
  Actor_FaceDirection(ACTOR_THIRD_TESTED_STUDENT, FACING_NORTH_NORTHWEST, 20);
  // "I can do it, Master! Please, just let me try!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_THIRD_TESTED_STUDENT,
                           0);
  FieldCamera_MoveTo(364, 72, 664, 1);
  FieldCamera_WaitForArrival();
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
  // "Your skills lack polish... It may be dangerous. Do you still wish to
  // enter?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  // "Yes, I do! I do!"
  Event_ShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 40);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "If you fail, you may lose more than just your pride..."
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  // "I do not fear death, Master!"
  Event_ShowMessageAndWait(ACTOR_THIRD_TESTED_STUDENT, 0);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  // "Ho ho! Is that so?"
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "Very well, then! It is time for your trial, my son!"
  Event_ShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_POI, 0);
  Actor_WalkToAndWait(ACTOR_POI, 326, 664);
  Actor_WalkToAndWaitById(ACTOR_POI, 328, 648);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpriteColor(Actor_GetById(ACTOR_POI), SPRITE_COLOR_HIDDEN);
  Actor_SetPosition(ACTOR_POI, 440, 600);
  FieldCamera_MoveTo(400, 0, 688, 1);
  Actor_SetSpeed(ACTOR_THIRD_TESTED_STUDENT, 0.8);
  Actor_WalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 644);
  Event_Wait(20);
  Sound_Play(SE_THUMP);
  KandoreanTemple_OpenTheCaveDoor();
  // "Enter the cave, and ascend!"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Task_Remove(KandoreanTemple_MatchStudentsToLeader);
  Actor_SetAnimationAndWait(ACTOR_THIRD_TESTED_STUDENT, ANIM_NOD);
  Actor_SetSpritePriority(ACTOR_THIRD_TESTED_STUDENT, 3);
  Actor_WalkToAndWait(ACTOR_THIRD_TESTED_STUDENT, 400, 640);
  Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
  Actor_SetPosition(ACTOR_POI, 0, 0);
  FieldCamera_MoveTo(400, 0, 752, 1);
  FieldCamera_WaitForArrival();
  Task_Add(KandoreanTemple_MatchStudentsToLeader, TASK_PASS_FRAME);
  Event_End();
  GameFlag_Set(FLAG_STUDENTS_TESTED);
}

/// Felix comes out of the west passage into the upper hall of Poi's hall, the
/// way up from the Chestbeaters' room, and Poi praises him for passing the test
/// before he sees the party are not his students and asks how they came so far.
/// He then goes down into the test hall to look at the rope.
void KandoreanTemple_PassTheTest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  wait(1);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 328, 360);
  {
    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(472), toFIXED(408),
                               FACING_SOUTH_SOUTHWEST);
  }
  Actor_GetById(ACTOR_POIS_ROOM_KRADEN)->facing = FACING_NORTH;
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
  Actor_PlaceOnActor(ACTOR_POIS_ROOM_KRADEN, gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_SetSpeed(ACTOR_POIS_ROOM_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_POIS_ROOM_KRADEN, 376, 392);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 40);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 20);
  Actor_SetAttachedEffect(ACTOR_POI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "How did you... come this far?"
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
                      0);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 20);
  // "Ah, yes... How indeed?"
  Event_ShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
  Actor_WalkToAndWait(ACTOR_POI, 468, 428);
  Actor_WalkToAndWait(ACTOR_POI, 472, 454);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR);
}

/// Poi goes down into the test hall and looks about by the south rope's posts:
/// "The rope is still on the floor." Nobody has climbed up that way, so he
/// goes back up to the party sure that they came through the cave.
void KandoreanTemple_FindTheRopeOnTheFloor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetPosition(gMainRoutineWork.leaderActorId, 0, 0);
  Actor_SetPosition(ACTOR_POI, 0, 0);
  Actor_SetPosition(ACTOR_THIRD_TESTED_STUDENT, 0, 0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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
  Import_EventSetExitRequest(KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE);
}

/// Poi steps up beside the north rope's east post, turns west to the party and
/// answers Kraden.
void KandoreanTemple_PoiWalksUp(void) {
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_WalkToAndWait(ACTOR_POI, 496, 392);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 0);
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
}

/// Poi comes back up into the upper hall to the party: they must have come
/// through the cave, so he must give them the secret of Lash. He shows them
/// Lash on the north rope, then crosses it ahead of them to the raised floor by
/// his table, and waits there for them to follow.
void KandoreanTemple_MeetPoi(void) {
  struct Actor *actor;
  struct FieldSprite *sprite;
  u8 *flags;

  Event_Start();
  Event_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  FieldCamera_GetActor()->motionFlags = 0;
  wait(1);
  Actor_SetPositionAndFacing(ACTOR_POIS_ROOM_KRADEN, toFIXED(376), toFIXED(392),
                             FACING_EAST);
  Actor_SetHorizontalPositionAndAngleWithTerrain(
      gMainRoutineWork.leaderActorId, toFIXED(376), toFIXED(408), FACING_EAST);
  {
    Actor_SetPositionAndFacing(ACTOR_POI, toFIXED(472), toFIXED(454),
                               FACING_NORTH_NORTHWEST);
  }
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_NOISE);
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
  Event_ShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_POI, 2);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
  // "Oh, no... Not at all..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 10);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 0);
  // "Felix, am I imagining it, or does he seem perturbed?" Yes: "I am not
  // perturbed. Merely...surprised." No: "No one from outside the temple has
  // ever passed..."
  Event_OpenMessage(ACTOR_POIS_ROOM_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
    // "I am not perturbed. Merely...surprised."
    Event_ShowMessageAndWait(ACTOR_POI, 0);
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
    // "No one from outside the temple has ever passed the test. It's
    // amazing..."
    Event_ShowMessageAndWait(ACTOR_POI, 0);
  }
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  // "What is this "Lash" you mentioned a moment ago?"
  Event_ShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
  Actor_SetAttachedEffect(ACTOR_POI,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  // "Lash is a sacred power, passed down for years to the followers of..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_SetAnimationById(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
  // "A sacred power? And it's called Lash, you say?"
  Event_ShowMessageThenWait(ACTOR_POIS_ROOM_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 40);
  // "And now, I must pass it on to you. It is our way."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_POI, FACING_WEST, 20);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "It is a gift given to all those who pass the test, as it has been..."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_ShowEmoteById(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION,
                      0);
  // "What type of power is this Lash?"
  Event_ShowMessageAndWait(ACTOR_POIS_ROOM_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "Allow me to show you."
  Event_ShowMessageAndWait(ACTOR_POI, 0);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(424, 16, 384, 1);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.8);
  Actor_SetSpeed(ACTOR_POIS_ROOM_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_SetMovementTypeById(gMainRoutineWork.leaderActorId,
                            gKandoreanTemple_LeaderStepsAsideScript);
  Actor_SetMovementTypeById(ACTOR_POIS_ROOM_KRADEN,
                            gKandoreanTemple_KradenStepsAsideScript);
  Actor_SetMovementTypeAndWaitById(ACTOR_POI,
                                   gKandoreanTemple_PoiWalksToTheRopeScript);
  Event_Wait(40);
  KandoreanTemple_CastLash();
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 20);
  // "Lash looks quite a bit like Psynergy, doesn't it?"
  Event_OpenMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_POIS_ROOM_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    // "Psynergy, hm? We call it spiritual strength, but it seems the same."
    KandoreanTemple_PoiWalksUp();
    gFieldWork->message++;
  } else {
    gFieldWork->message++;
    // "I do not know of your Psynergy, but our power comes from within."
    KandoreanTemple_PoiWalksUp();
  }
  Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "This technique uses the spiritual to affect the physical."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
  // "One with a strong spirit can lift and move heavy objects farther."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_ShowEmoteById(ACTOR_POIS_ROOM_KRADEN,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "The distance changes with your level of power? Fascinating!"
  Event_ShowMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_POIS_ROOM_KRADEN, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "Those who cannot pass this cave's trials are not worthy of the
  // technique."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 0);
  // "Felix, do you think this cave was designed to test one's control of
  // Psynergy?"
  Event_OpenMessage(
      SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_POIS_ROOM_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(gMainRoutineWork.leaderActorId, 0) ==
      0) {
    Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
    // "Yes, yes! Well done, old man!"
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
    Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
    Actor_ShowEmoteById(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        20);
    // "Who are you calling an old man!? You're no spring chicken yourself!"
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
                                 ACTOR_POIS_ROOM_KRADEN,
                             0);
    gFieldWork->message += 2;
  } else {
    gFieldWork->message += 2;
    Actor_SetAnimationById(ACTOR_POI, ANIM_SHAKE_HEAD);
    // "You are quite mistaken... You should listen to the old man. He speaks
    // wisely."
    Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
    Actor_FaceDirectionById(ACTOR_POIS_ROOM_KRADEN, FACING_EAST, 0);
    Actor_ShowEmoteById(ACTOR_POIS_ROOM_KRADEN, EMOTE_IN_FRONT | EMOTE_SCOWL,
                        20);
    // "Who are you calling an old man? You're no schoolboy yourself!"
    Event_ShowMessageAndWait(SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE |
                                 ACTOR_POIS_ROOM_KRADEN,
                             0);
  }
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  FieldCamera_MoveTo(424, 0, 400, 1);
  FieldCamera_SetSpeed(0.6);
  Actor_SetSpeed(ACTOR_POI, 0.6);
  Actor_WatchAndSetScript(gMainRoutineWork.leaderActorId, ACTOR_POI);
  Actor_WatchAndSetScript(ACTOR_POIS_ROOM_KRADEN, ACTOR_POI);
  Actor_WalkToAndWait(ACTOR_POI, 488, 416);
  Actor_WalkToAndWait(ACTOR_POI, 472, 432);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHWEST, 0);
  // "I wondered if it would be right to allow an outsider to use this
  // power..."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  // "You're...not going to teach us?"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_POIS_ROOM_KRADEN, 0,
                            80);
  Actor_ShowEmoteById(ACTOR_POI, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Actor_FaceDirection(ACTOR_POI, FACING_NORTH_NORTHWEST, 20);
  // "But I like you, so I will let you learn our secret."
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  Actor_SetAnimationById(ACTOR_POI, ANIM_NOD);
  // "Please, follow me."
  Event_ShowMessageAndWait(SPEAKER_WINDOW_ABOVE | ACTOR_POI, 0);
  FieldCamera_MoveTo(424, 16, 384, 1);
  Actor_SetSpeed(ACTOR_POI, 1.2);
  Actor_WalkToAndWait(ACTOR_POI, 488, 416);
  Actor_WalkToAndWait(ACTOR_POI, 504, 376);
  Actor_FaceDirectionById(ACTOR_POI, FACING_WEST, 20);
  actor = Actor_GetById(ACTOR_POI);
  sprite = actor->sprite;
  Actor_SetSpritePriority(ACTOR_POI, 1);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 492, 376);
  Actor_SetAnimationById(ACTOR_POI, 7);
  actor->scaleX = -FIXED_ONE;
  sprite->rotation = TRIG_TURN * 7 / 8;
  actor->motionFlags = 0;
  actor->y.fixed += toFIXED(4);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), 0);
  Actor_SetSpeed(ACTOR_POI, 0.4);
  FieldActor_SetMoveTarget((s32)actor, 444, 40, 376);
  Actor_WaitForMoveById(ACTOR_POI);
  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_POI), SPRITE_FLAG_SHADOW);
  sprite->rotation = 0;
  actor->scaleX = FIXED_ONE;
  actor->groundHeight = toFIXED(32);
  Actor_SetAnimationById(ACTOR_POI, ANIM_STAND);
  Actor_SetSpeed(ACTOR_POI, 1.6);
  Actor_Jump(ACTOR_POI, 4, 0);
  Actor_MoveToAndWait(ACTOR_POI, 424, 376);
  FieldCamera_MoveTo(408, 16, 400, 1);
  Actor_SetSpeed(ACTOR_POI, 0.8);
  Actor_WalkToAndWait(ACTOR_POI, 392, 360);
  Actor_FaceDirection(ACTOR_POI, FACING_SOUTH_SOUTHEAST, 0);
  Actor_SetSpritePriority(ACTOR_POI, 2);
  Actor_Stop(ACTOR_POIS_ROOM_KRADEN);
  Actor_Stop(gMainRoutineWork.leaderActorId);
  wait(1);
  Actor_FaceDirection(ACTOR_POIS_ROOM_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_NORTHWEST, 10);
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_POIS_ROOM_KRADEN, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_POIS_ROOM_KRADEN, ANIM_WALK);
  {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.leaderActorId);

    if (leader != NULL)
      Actor_SetDestinationById(ACTOR_POIS_ROOM_KRADEN,
                               leader->x.part[0].integer,
                               leader->z.part[0].integer);
  }
  Actor_WaitForMoveById(ACTOR_POIS_ROOM_KRADEN);
  Actor_SetPosition(ACTOR_POIS_ROOM_KRADEN, 0, 0);
  GameFlag_Set(FLAG_MET_POI);
#if TLA_PATCH_1
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
#endif
  Event_End();
}

/// Creates the Lash Pebble's actor on Poi's table, drawn from its item icon.
void KandoreanTemple_CreateLashPebble(void) {
  struct Actor **slot = &gKandoreanTemple_LashPebble;
  u8 clear = 0;
  struct Actor *actor = ((struct Actor *)Import_ActorCreate(
      SPRITE_UNSEEN_MARKER, toFIXED(408), toFIXED(40), toFIXED(344)));

  *slot = actor;
  if (actor) {
    struct FieldSprite *sprite = actor->sprite;
    GlyphTransfer *glyph;

    sprite->fullColor = 0;
    sprite->palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    sprite->flags = clear;
    sprite->countAndShadow[0] = clear;
    glyph = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(ITEM_LASH_PEBBLE);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
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
#include "shared/field_effect_duration_scripts.c"

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

u32 gKandoreanTemple_SplashRingScript[] = {
    SC_WAIT(24),
    SC_RELEASE,
};

u32 gKandoreanTemple_SplashScript[] = {
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
    SC_CALL((s32)KandoreanTemple_Meditate),
};

s32 gKandoreanTemple_LevitateScript[] = {
    SC_CALL(KandoreanTemple_SquashAndStretch),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gKandoreanTemple_LeaderStepsAsideScript[] = {
    SC_ANIMATION(2),
    SC_MOVE(456, 0, 408),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
    SC_END,
};

u32 gKandoreanTemple_KradenStepsAsideScript[] = {                                                               
      SC_ANIMATION(2),   SC_MOVE(440, 0, 392),                         
      SC_SYNC,     SC_ANIMATION(1),                                                  
      SC_WAIT(1), SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),              
      SC_WAIT(1), SC_END,                                                     
  };

u32 gKandoreanTemple_PoiWalksToTheRopeScript[] = {
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

s32 gKandoreanTemple_HighFlameScript[] = {
    SC_CALL(KandoreanTemple_PlaceFlame),
    SC_SET(SW_Y, toFIXED(88)),
    SC_END,
};

s32 gKandoreanTemple_LowFlameScript[] = {
    SC_CALL(KandoreanTemple_PlaceFlame),
    SC_SET(SW_Y, toFIXED(44)),
    SC_END,
};

s32 gKandoreanTemple_FlameScript[] = {
    SC_CALL(KandoreanTemple_PlaceFlame),
    SC_END,
};

#include "shared/scene_entrances_default.c"

struct SceneRegion gKandoreanTemple_ChestbeaterRoomRegions[] = {
    {320, 74, 96, 336, 90, 112, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_POI},
    {SCENE_TABLE_END},
};

struct SceneRegion gKandoreanTemple_StreamRoomRegions[] = {
    {512, 42, 272, 528, 58, 288, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_EAST},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(KandoreanTempleLifts),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_LIFTS_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT, KandoreanTempleCurrent, KANDOREAN_TEMPLE_CURRENT_FROM_KANDOREAN_TEMPLE_LIFTS),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_SHAFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_SHAFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT),
    SCENE_EXITS(KandoreanTempleMimic),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_SOUTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_MIMIC),
    SCENE_EXIT(KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH, KandoreanTempleFog, KANDOREAN_TEMPLE_FOG_FROM_KANDOREAN_TEMPLE_MIMIC_NORTH),
    SCENE_EXITS(KandoreanTempleChestbeaters),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXIT(KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_POI, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_FROM_KANDOREAN_TEMPLE_CHESTBEATERS),
    SCENE_EXITS(KandoreanTemplePillar),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT, KandoreanTempleCurrent, KANDOREAN_TEMPLE_CURRENT_FROM_KANDOREAN_TEMPLE_PILLAR),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_NORTH),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_WEST_LIFT, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_WEST_LIFT),
    SCENE_EXIT(KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_EAST_LIFT, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_PILLAR_EAST_LIFT),
    SCENE_EXITS(KandoreanTempleStream),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_EAST, KandoreanTempleGate, KANDOREAN_TEMPLE_GATE_FROM_KANDOREAN_TEMPLE_STREAM_EAST),
    SCENE_EXITS(KandoreanTempleCurrent),
    SCENE_EXIT(KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS, KandoreanTempleLifts, KANDOREAN_TEMPLE_LIFTS_FROM_KANDOREAN_TEMPLE_CURRENT),
    SCENE_EXIT(KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR, KandoreanTemplePillar, KANDOREAN_TEMPLE_PILLAR_FROM_KANDOREAN_TEMPLE_CURRENT),
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
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_TEST_HALL_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_WEST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_TEST_HALL_FROM_UPPER_HALL_EAST),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_FROM_UPPER_HALL),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_UPPER_HALL_FROM_WEST_PASSAGE),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_CHESTBEATERS, KandoreanTempleChestbeaters, KANDOREAN_TEMPLE_CHESTBEATERS_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_MIMIC, KandoreanTempleMimic, KANDOREAN_TEMPLE_MIMIC_FROM_KANDOREAN_TEMPLE_POI),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_ROPE_ON_THE_FLOOR),
    SCENE_EXIT(KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE, KandoreanTemplePoi, KANDOREAN_TEMPLE_POI_THROUGH_THE_CAVE),
    SCENE_EXITS_END,
};

struct ActorPlacement gKandoreanTemple_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_LiftRoomPlacements[] = {                                             
      {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},                
      {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_SOUTH, 2, 3},                
      {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 4, 0, 0, FACING_SOUTH, 2, 3},                
      {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 5, 0, 0, FACING_SOUTH, 2, 2},                
      {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},                  
      {SCENE_TABLE_END},                                                       
  };

struct ActorPlacement gKandoreanTemple_ChestbeaterRoomPlacements[] = {
    {SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_SIGNAL_FIRE, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    ACTOR_PLACEMENT(SPRITE_SIGN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 328, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 296, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHESTBEATER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 360, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_VentRoomPlacements[] = {
    {SPRITE_STONE_PILLAR, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_WEST, 2, 2},
    {SPRITE_STEAM_VENT, CONDITION_ALWAYS, 1, 3, 0, 0, FACING_EAST, 2, 2},
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STEAM_VENT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_FogRoomPlacements[] = {
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 1, 0, 0, FACING_SOUTH, 2, 2},
    {SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, 1, 2, 0, 0, FACING_SOUTH, 2, 3},
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 340, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_FOG), MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_GatePlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WEEDS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 224, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, (s32)gKandoreanTemple_LookAroundScript, 264, 0, 424, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 334, 0, 317, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 284, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 246, 0, 318, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gKandoreanTemple_PoisRoomPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 504, 0, 676, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POI, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 664, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_BOY_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_HighFlameScript, 439, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_HighFlameScript, 359, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_LowFlameScript, 412, 0, 324, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_FlameScript, 90, 0, 230, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gKandoreanTemple_FlameScript, 174, 0, 72, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gKandoreanTemple_Events[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_LiftRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_FOG_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_CURRENT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_LIFTS_TO_KANDOREAN_TEMPLE_PILLAR_NORTH},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LIFT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingBlock)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SHAFT, FLAG_PILLAR_BLOCKS_SHAFT, EVENT(KandoreanTemple_SinkIntoTheSteam)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_LIFT, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideWestLiftDown)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_LIFT, CONDITION_ALWAYS, EVENT(KandoreanTemple_RideEastLiftDown)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_MimicRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_POI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_CHESTBEATERS},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_MIMIC_TO_KANDOREAN_TEMPLE_FOG_NORTH},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_ChestbeaterRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC, CONDITION_ALWAYS, KANDOREAN_TEMPLE_CHESTBEATERS_TO_KANDOREAN_TEMPLE_MIMIC},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(KandoreanTemple_StartMovingPot)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BOILING_POT, FLAG_POT_MOVED, EVENT(KandoreanTemple_FinishMovingPot)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_WEST_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_SOUTH_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_EAST_OF_HOT_FLOOR, FLAG_POT_MOVED, EVENT(KandoreanTemple_TouchTheHotFloor)},
    {TRIGGER_TALK, ACTOR_POIS_SIGN, CONDITION_ALWAYS, EVENT(KandoreanTemple_ReadPoisSign)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEFORE_CHESTBEATERS, FLAG_LADDER_LOWERED, EVENT(KandoreanTemple_FightChestbeaters)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_VentRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT, CONDITION_ALWAYS, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_CURRENT},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH, CONDITION_ALWAYS, KANDOREAN_TEMPLE_PILLAR_TO_KANDOREAN_TEMPLE_LIFTS_NORTH},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VENT_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(KandoreanTemple_FinishMovingPillar)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WEST_VENT_COVER, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveByWestVent)},
    {TRIGGER_STEP, CELL_BEHAVIOR_EAST_VENT_COVER, CONDITION_ALWAYS, EVENT(KandoreanTemple_LeaveByEastVent)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_StreamRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_STREAM_TO_KANDOREAN_TEMPLE_GATE_WEST},
    {TRIGGER_TABLE_END},
};
// clang-format on

struct Trigger gKandoreanTemple_CurrentRoomEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_LIFTS},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR,
     CONDITION_ALWAYS, KANDOREAN_TEMPLE_CURRENT_TO_KANDOREAN_TEMPLE_PILLAR},
    {TRIGGER_STEP, CELL_BEHAVIOR_CURRENT, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_PushAlongTheCurrent)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_FogRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST,
     CONDITION_ALWAYS, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_NORTH},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST,
     CONDITION_ALWAYS, KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_LIFTS_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP,
     KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_FOG_TO_KANDOREAN_TEMPLE_MIMIC_SOUTH},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_FOG_ROPE_POST, FLAG_FOG_ROPE_TIED,
     0},
    {TRIGGER_TALK, ACTOR_FOG, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_FightFog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BEHIND_POST, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepBehindThePost)},
    {TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_POST, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_StepOutFromThePost)},
    {TRIGGER_TALK, ACTOR_STUDENT_WHO_GAVE_UP, CONDITION_ALWAYS,
     MSGNO(MsgFieldItSeemsIMayNotHave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_GAVE_UP,
     CONDITION_ALWAYS, MSGNO(MsgFieldIsItReallyPossibleForAnyone)},
    {TRIGGER_TABLE_END},
};

struct Trigger gKandoreanTemple_GateEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_GATE_TO_WORLD_MAP},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI,
     CONDITION_ALWAYS, KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_POI},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN,
     KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST, CONDITION_ALWAYS,
     KANDOREAN_TEMPLE_GATE_TO_KANDOREAN_TEMPLE_STREAM_WEST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_COURTYARD_WELL,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_ClimbDownTheWell)},
    {EXAMINE_TRIGGER(EXAMINE_WELL), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgKandoreanTemplePassageInTheWell))},
    {TRIGGER_STEP, CELL_BEHAVIOR_SUSPICIOUS_WALL, FLAG_KRADEN_SAW_THE_WALL,
     EVENT(KandoreanTemple_KradenSuspectsTheWall)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_INSIDE_TEMPLE_DOOR,
     FLAG_TEMPLE_DOOR_OPEN, EVENT(KandoreanTemple_OpenTheTempleDoor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_BEFORE_TEMPLE_DOOR,
     FLAG_TEMPLE_DOOR_OPEN, EVENT(KandoreanTemple_OpenTheTempleDoor)},
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_TEMPLE_DOOR, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TryTheLockedDoor)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEEDS_OVER_THE_CAVE,
     CONDITION_ALWAYS, EVENT(KandoreanTemple_FindTheHiddenPassage)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIRST_WEED_CLUMP, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_SECOND_WEED_CLUMP, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_THIRD_WEED_CLUMP, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FOURTH_WEED_CLUMP, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
     CELL_BEHAVIOR_FIFTH_WEED_CLUMP, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_CycloneFifthPatch)},
    {TRIGGER_TALK, ACTOR_STUDENT_ASKING_ABOUT_POI, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_AskAboutPoi)},
    {TRIGGER_TALK, ACTOR_STUDENT_ON_GUARD_DUTY, CONDITION_ALWAYS,
     EVENT(KandoreanTemple_TellOfTheMeditation)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_STUDENT_ASKING_ABOUT_POI | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_ASKED_ABOUT_POI, EVENT(KandoreanTemple_ReadTheGateStudent)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ASKING_ABOUT_POI,
     CONDITION_ALWAYS, MSGNO(MsgFieldIDoNotUnderstandMasterPoi)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_STUDENT_ON_GUARD_DUTY | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_TOLD_OF_THE_MEDITATION, EVENT(KandoreanTemple_ReadTheGuard)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_GUARD_DUTY,
     CONDITION_ALWAYS, MSGNO(MsgFieldIWasHopingToHearMaster)},
    {TRIGGER_TALK, ACTOR_STUDENT_WARY_OF_OUTSIDERS, FLAG_MET_POI,
     MSGNO(MsgFieldIHopeToEnterTheTemple)},
    {TRIGGER_TALK, ACTOR_STUDENT_WARY_OF_OUTSIDERS, CONDITION_ALWAYS,
     MSGNO(MsgFieldHeyWhatsAnOutsiderDoingOn)},
    {TRIGGER_TALK, ACTOR_STUDENT_ON_THE_FAILED_TEST, FLAG_MET_POI,
     MSGNO(MsgFieldPleaseDontBreakMyConcentrationI)},
    {TRIGGER_TALK, ACTOR_STUDENT_ON_THE_FAILED_TEST, CONDITION_ALWAYS,
     MSGNO(MsgFieldLooksLikeNoneOfOurBrothers)},
    {TRIGGER_TALK, ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, FLAG_MET_POI,
     MSGNO(MsgFieldIAmNotJustMeditatingI)},
    {TRIGGER_TALK, ACTOR_STUDENT_WHO_WOULD_BE_A_MONK, CONDITION_ALWAYS,
     MSGNO(MsgFieldSomedayImGoingToBeA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WARY_OF_OUTSIDERS,
     FLAG_MET_POI, MSGNO(MsgFieldIStillCantBelieveThatThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WARY_OF_OUTSIDERS,
     CONDITION_ALWAYS, MSGNO(MsgFieldGahIKeptMyEyesShut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_THE_FAILED_TEST,
     FLAG_MET_POI, MSGNO(MsgFieldMyMindIsEmptyMyBody)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_ON_THE_FAILED_TEST,
     CONDITION_ALWAYS, MSGNO(MsgFieldIveBeenMeditatingAllDayBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
     FLAG_MET_POI, MSGNO(MsgFieldEmptyTheMindToLightenThe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUDENT_WHO_WOULD_BE_A_MONK,
     CONDITION_ALWAYS, MSGNO(MsgFieldWheneverTheMasterGoesToTown)},
    {TRIGGER_TABLE_END},
};

// clang-format off
struct Trigger gKandoreanTemple_PoisRoomEvents[] = {
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TO_KANDOREAN_TEMPLE_GATE},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_TEST_HALL_TO_UPPER_HALL_EAST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_WEST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_TEST_HALL_EAST},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_UPPER_HALL_TO_WEST_PASSAGE},
    {TRIGGER_EXIT, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL, CONDITION_ALWAYS, KANDOREAN_TEMPLE_POI_WEST_PASSAGE_TO_UPPER_HALL},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BY_THE_LADDER, CONDITION_ALWAYS, EVENT(KandoreanTemple_ClimbDownTheLadder)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_CAVE_DOOR, CONDITION_ALWAYS, EVENT(KandoreanTemple_EnterTheCave)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_SOUTH_ROPE_WEST_POST, FLAG_SOUTH_ROPE_TIED, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_NORTH_ROPE_EAST_POST, FLAG_NORTH_ROPE_TIED, 0},
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
    {TRIGGER_EXAMINE, CELL_BEHAVIOR_POIS_TABLE, FLAG_LASH_RECEIVED, EVENT(KandoreanTemple_ReceiveLash)},
    {TRIGGER_TALK, ACTOR_POI, FLAG_LASH_RECEIVED, MSGNO(MsgFieldTakeThePowerOfLashIt)},
    {TRIGGER_TALK, ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWishToPracticeThese)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_POI, CONDITION_ALWAYS, MSGNO(MsgFieldHoHoIFeelTheStrength)},
    {TRIGGER_TABLE_END},
};
// clang-format on
