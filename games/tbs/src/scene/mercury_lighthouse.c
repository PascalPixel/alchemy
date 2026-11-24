/// @file
/// Mercury Lighthouse, the lighthouse of water north of Imil that Mia and the
/// Mercury Clan guard: the ground outside its door
/// (SceneId_MercuryLighthouseExterior) and three of its upper scenes
/// (SceneId_MercuryLighthouseLower, SceneId_MercuryLighthouseMiddle and
/// SceneId_MercuryLighthouseUpper). The ground floor inside the door, with the
/// Lizard Man and the goddess's pool, is mercury_lighthouse_entrance.c, and the
/// beacon at the top, the Lighthouse Aerie, is mercury_lighthouse_aerie.c. The
/// four scenes here share one exit table, and every arrival sets
/// FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts regain PP faster.
/// Retreat leads out to the foot of the causeway
/// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP).
///
/// In the order the party meets them:
///
/// - Outside. The party comes from the World Map up a causeway between two
///   blue statues standing in the water, past the dry Fountain of Healing and
///   its sign, to the lighthouse door. Until the party has met Mia in Imil
///   (FLAG_IMIL_MET_MIA) the door glows and searching it shows "Strange forces
///   are at work...". Then Mia kneels before it, and a stone statue stands in
///   her way. Spoken to, she senses someone inside; Move pushes the statue
///   aside, she asks whether Isaac did that, learns the word Psynergy from
///   Garet and Ivan, and opens the door with Ply before going in alone
///   (FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA).
/// - SceneId_MercuryLighthouseLower: the floor above the goddess's pool. Its
///   great pool room is set with stepping squares, some marked with a cross.
///   Stairs from the platform in its middle lead to a ledge where Mia is stuck
///   again, before a blue statue that was not there before; Move drops it into
///   the gap at her feet and she joins the party with Fizz, a Mercury Djinni
///   (FLAG_MIA_JOINED_PARTY). In the pipe room three pipe sections pushed
///   across their grated pools join the channels (FLAG_PIPE_1_JOINED to
///   FLAG_PIPE_3_JOINED), and the water runs on into the rooms below
///   (FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN): before and after are
///   places/mercury-lighthouse-lower/map-4.png and
///   places/mercury-lighthouse-lower_mercury-lighthouse-channels-open/map-4.png.
///   In a small room a hole behind a blue crystal drops to the goddess's pool.
///   Once Ply has woken that pool (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), the
///   cross-marked squares here give Isaac golden rings of light, each of which
///   holds him up on the water for one hop
///   (shared/mercury_lighthouse_pillar.c). That is the way across the great
///   pool room, whose chest holds a Sleep Bomb.
/// - SceneId_MercuryLighthouseMiddle. In the big pipe room one pipe section,
///   joined, sets water spouting from four pipe ends (FLAG_SPOUTS_RUNNING); a
///   chest there holds a Psy Crystal. In the gate room a blue statue moved
///   onto the floor switch holds the sealed arch on the dais open for good
///   (FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN), where Isaac standing on the
///   switch holds it only while he stays; past it is the crystal room.
///   In the room with the Nut chest a pipe joined across its pool sends a jet
///   along the north wall and opens a doorway up to the room of six
///   waterfalls (FLAG_FOUNTAIN_RUNNING). Sleet, a Mercury Djinni, waits in a
///   room of its own and fights when spoken to.
/// - SceneId_MercuryLighthouseUpper. Walking into the third waterfall from the
///   west of the first room of six waterfalls leads down to Sleet. In the room
///   of jets a pipe pushed along its pool turns the water from one pipe end to
///   another, and each jet pushes a blue statue a step, until it slides into a
///   square hole and falls to the gate room below
///   (FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL). There Move sets it on the floor
///   switch, which holds the middle arch open
///   (FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN). Past it, five stone
///   statues stand along the wall; the third and the fifth hide doorways down
///   to the room with the Nut chest. In the second room of six waterfalls the
///   easternmost leads to a room of vines, where Move pushes a stone statue
///   off the doorway to the paths outside.
/// - Outside again, high up. The paths end at a waterfall under a rainbow,
///   whose riddle appears on arrival: "He who honors the goddess of rainbows
///   shall be guided to the heavens upon wings of fluid grace." Ply on the
///   blue statue beside it reverses the waterfall, and Mia, Isaac, Garet and
///   Ivan rise up the water one by one to the Lighthouse Aerie, where Saturos
///   waits (mercury_lighthouse_aerie.c). Once he is beaten
///   (FLAG_SATUROS_BEATEN_AT_MERCURY) the lift brings the party down from the
///   aerie to the fountain, which flows again. People come to drink from it,
///   and an Empty Bottle used there becomes Hermes' Water, once a visit
///   (FLAG_HERMES_WATER_TAKEN). The waterfall still carries the leader up,
///   alone.

#include "scene/mercury_lighthouse.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/facing.h"
#include "field/field.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/ground_mask.h"
#include "field/map_grid_scroll.h"
#include "field/pushed_actor.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "field/world_map_render_state.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "scene/mercury_lighthouse_lights.h"
#include "scene/mercury_lighthouse_pillar.h"
#include "scene/walk_into_leader.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/dma.h"
#include "system/heap.h"
#include "system/heap_release.h"
#include "system/input.h"
#include "system/io_write.h"
#include "system/iwram_call.h"
#include "system/main_routine_state.h"
#include "system/ram_buffer.h"
#include "system/system.h"
#include "system/system_call.h"
#include "window/screens.h"
#include "window/window.h"
#include "text/msg_ids.h"

extern s32 gMercuryLighthouse_RippleScript[];
extern s32 gGlint_TwinkleScript[];
extern s32 gMercuryLighthouse_StatueSlideWest[];
extern s32 gMercuryLighthouse_StatueSlideSouth[];
extern s32 gMercuryLighthouse_StatueSlideIntoHole[];
extern s32 gMercuryLighthouse_StatueStaysPut[];
extern u16 gMercuryLighthouse_GateCells[];
extern u16 gMercuryLighthouse_GateCloseCells[];
extern u16 gMercuryLighthouse_UpperGateCells[];
extern u16 gMercuryLighthouse_UpperGateCloseCells[];
extern u16 gMercuryLighthouse_ColumnCells[];
extern struct SceneEntrance gMercuryLighthouseLowerEntrances[];
extern struct SceneEntrance gMercuryLighthouseMiddleEntrances[];
extern struct SceneEntrance gMercuryLighthouseUpperEntrances[];
extern struct SceneEntrance gMercuryLighthouseExteriorEntrances[];
extern u32 gMercuryLighthouse_Exits[];
extern struct ActorPlacement gMercuryLighthouseNoActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseLowerActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseMiddleActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseUpperActorPlacements[];
extern struct ActorPlacement gMercuryLighthouseExteriorActorPlacements[];
extern struct Trigger gMercuryLighthouseLowerTriggers[];
extern struct Trigger gMercuryLighthouseMiddleTriggers[];
extern struct Trigger gMercuryLighthouseUpperTriggers[];
extern struct Trigger gMercuryLighthouseExteriorTriggers[];
extern s32 gMercuryLighthouse_SparkScript[];
extern s32 gMercuryLighthouse_GrowLightScript[];
extern s32 gMercuryLighthouse_ScaleCounterScript[];
extern s32 gMercuryLighthouse_PillarScript[];
extern u16 gMercuryLighthouse_MoveAngles[];
extern s32 gPaletteCycleRed;
extern s32 gPaletteCycleGreen;
extern s32 gPaletteCycleBlue;

/// How a spout carries Isaac back: from patch 1 the event waits until he is
/// there.
#if TBS_PATCH_1
#define MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(actorId, x, z)                    \
  Actor_MoveToAndWait(actorId, x, z)
#else
#define MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(actorId, x, z)                    \
  Actor_SetDestination(actorId, x, z)
#endif

// UB: this calls a void function through an s32 function type, the call the
// bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT Actor_SetAnimationAndWait
#else
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT                              \
  ((s32 (*)())Actor_SetAnimationAndWait)
#endif

static void MercuryLighthouse_OpenGateWithMovedStatue(void);
static void MercuryLighthouse_OpenUpperGateWithMovedStatue(void);
static void MercuryLighthouse_RecordRowStatues(void);
static void MercuryLighthouse_StartSpoutsWithSparkles(void);
static void MercuryLighthouse_StopSpoutsWithSparkles(void);
static void MercuryLighthouse_SparkleAroundLeader(void);
static void MercuryLighthouse_SparkleAt(s32 x, s32 y, s32 z);
static void MercuryLighthouse_SparkleNear(s32 x, s32 y, s32 z);
static void MercuryLighthouse_SparkleBurst(s32 x, s32 y, s32 z, s32 angle);
static void MercuryLighthouse_RideLift(void);
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void);
static void Inventory_ReplaceItem(s32 oldItem, s32 newItem);

/// Mia kneeling with her hands together, as she senses someone inside the
/// lighthouse (mia.png's frame 76).
enum { ANIM_MIA_KNEEL = 16 };

/// The cells the four scenes' triggers answer, named for where they are. Exit
/// cells are their exits' numbers past EXIT_CELL_BEHAVIOR_OFFSET.
enum {
  /// The great pool room: two cells of the floor beside the Sleep Bomb chest,
  /// (7, 15) and (9, 15), which put out Isaac's lights like the pool's plain
  /// squares but are no place to hop from.
  CELL_BEHAVIOR_SLEEP_BOMB_LEDGE = 32,
  /// The crystal room's hole behind its blue crystal, (29, 37), and the edge
  /// of the gate room's terrace above another blue crystal, (13, 19).
  CELL_BEHAVIOR_BY_CRYSTAL = 15,
  /// Beside the three pipe sections of the pipe room.
  CELL_BEHAVIOR_CHANNEL_PIPE_PUSH = 25,
  /// Beside the spout pipe of the big pipe room.
  CELL_BEHAVIOR_SPOUT_PIPE_PUSH = 1,
  /// The three spouts of the big pipe room once they run: the one shooting
  /// east from (10, 38), the one shooting south from (22, 41) and the one
  /// shooting west at (16, 49) to (18, 49).
  CELL_BEHAVIOR_EAST_SPOUT = 2,
  CELL_BEHAVIOR_SOUTH_SPOUT = 3,
  CELL_BEHAVIOR_WEST_SPOUT = 4,
  /// Beside the pipe across the pool in the room with the Nut chest.
  CELL_BEHAVIOR_FOUNTAIN_PIPE_PUSH = 45,
  /// The jet along that room's north wall once it runs, (43, 36) to
  /// (45, 36), copied there from (43, 55) to (45, 55).
  CELL_BEHAVIOR_FOUNTAIN_JET = 46,
  /// Both gate rooms: the floor round the floor switch, which is also where
  /// the gate statue is pushed from.
  CELL_BEHAVIOR_GATE_ROOM_FLOOR = 10,
  /// The floor switch of the gate room in the middle scene, (5, 13), and of
  /// the one in the upper scene, (10, 23).
  CELL_BEHAVIOR_GATE_FLOOR_SWITCH = 12,
  /// The three cells round the middle scene's floor switch, (5, 12), (4, 13)
  /// and (6, 13).
  CELL_BEHAVIOR_BESIDE_GATE_FLOOR_SWITCH = 13,
  /// The sealed arch of either gate room, searched from in front.
  CELL_BEHAVIOR_SEALED_GATE = 20,
  /// Beside the two pipes of the room of jets.
  CELL_BEHAVIOR_JET_ROOM_PIPE_PUSH = 40,
  /// Beside the stone statues along the wall, (39, 5) to (53, 5), (57, 5)
  /// and (59, 5).
  CELL_BEHAVIOR_ROW_STATUE_PUSH = 47,
  /// Outside: the lighthouse door while it is sealed, (52, 36).
  CELL_BEHAVIOR_SEALED_DOOR = 10,
  /// Outside: the open door, (52, 36) once Mia has opened it, and the way
  /// back to the World Map at the foot of the causeway, (51, 54) to (53, 54).
  CELL_BEHAVIOR_LIGHTHOUSE_DOOR = 44,
  CELL_BEHAVIOR_CAUSEWAY_FOOT = 43,
  /// Outside: the Fountain of Healing, (52, 47), and its sign, (52, 50).
  CELL_BEHAVIOR_HEALING_FOUNTAIN = 5,
  CELL_BEHAVIOR_FOUNTAIN_SIGN = 42
};

/// MercuryLighthouseLower: the statue in Mia's way, and the pipe room's pipe
/// sections and pipe end.
enum MercuryLighthouseLowerActorId {
  ACTOR_STATUE_BLOCKING_MIA = ACTOR_FIRST_PLACED,
  /// Isaac pushes these pipe sections, by walking into them, across their
  /// grated pools until each joins the pipes on either side
  /// (SPRITE_PIPE_WEST_EAST for the first two, SPRITE_PIPE_NORTH_SOUTH for the
  /// third).
  ACTOR_FIRST_PIPE,
  ACTOR_SECOND_PIPE,
  ACTOR_THIRD_PIPE,
  /// The rim of the fixed pipe the third section joins.
  /// It is SPRITE_PIPE_END, one still frame, 59 pixels south of where
  /// ACTOR_THIRD_PIPE comes to rest. Pushing it does nothing, and nothing else
  /// moves it.
  ACTOR_PIPE_END
};

/// MercuryLighthouseMiddle: the spout pipe of the big pipe room, the pipe in
/// the room with the Nut chest, the statue for the gate's floor switch and
/// Sleet, a Mercury Djinni.
enum MercuryLighthouseMiddleActorId {
  ACTOR_SPOUT_PIPE = ACTOR_FIRST_PLACED,
  /// It stands 59 pixels south of where ACTOR_SPOUT_PIPE rests once it is
  /// joined. Nothing moves it.
  ACTOR_SPOUT_PIPE_END,
  ACTOR_FOUNTAIN_PIPE,
  ACTOR_GATE_STATUE,
  ACTOR_SLEET
};

/// MercuryLighthouseUpper: the blue statue of the room of jets and where it
/// lands below, that room's two pipes and pipe ends, the five stone statues
/// along the wall and the stone statue of the room of vines.
enum MercuryLighthouseUpperActorId {
  /// The statue of ACTOR_SLIDING_STATUE once it has fallen to the gate room
  /// below.
  ACTOR_FALLEN_STATUE = ACTOR_FIRST_PLACED,
  /// At rest beside the jet that falls from the pipe end north of it. Each
  /// jet that hits it pushes it a step, and the last slides it east over the
  /// floor to the square hole at (840, 744), whose ground lies below the
  /// floor's, and it falls through.
  ACTOR_SLIDING_STATUE,
  /// The pipe section lying west to east, which only stands in the way.
  ACTOR_WEST_EAST_PIPE,
  /// The pipe section lying north to south that turns the water from one jet
  /// to another.
  ACTOR_JET_SWITCH_PIPE,
  /// Three in a row south of the pool the switch pipe slides along. Nothing
  /// moves them.
  ACTOR_FIRST_JET_ROOM_PIPE_END,
  /// Five in a row along the north wall.
  ACTOR_FIRST_ROW_STATUE = ACTOR_FIRST_JET_ROOM_PIPE_END + 3,
  ACTOR_LAST_ROW_STATUE = ACTOR_FIRST_ROW_STATUE + 4,
  ACTOR_GAP_STATUE
};

/// MercuryLighthouseExterior: Garet, Ivan and Mia in their own party slots, the
/// statues by the way in, the statue at the door, the Ply target, the statue by
/// the waterfall, the statue on the lift and the people at the fountain.
enum MercuryLighthouseExteriorActorId {
  /// A pair of statues either side of the way in from the World Map
  /// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP). The east one is drawn
  /// mirrored.
  ACTOR_WEST_APPROACH_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_EAST_APPROACH_STATUE,
  ACTOR_DOOR_STATUE,
  ACTOR_PLY_TARGET,
  ACTOR_WATERFALL_STATUE,
  ACTOR_LIFT_STATUE,
  /// The first of four people at the Fountain of Healing once Saturos is
  /// beaten. In order they are a young man, a woman with fair hair, an old man
  /// with a cane and an old woman.
  ACTOR_FIRST_VISITOR
};

/// Puzzle states kept until the party leaves the lighthouse.
enum {
  /// The big pipe room's spout pipe is joined and its spouts run.
  FLAG_SPOUTS_RUNNING = FLAG_AREA_TEMP_02,
  FLAG_FOUNTAIN_RUNNING = FLAG_AREA_TEMP_06,
  FLAG_FOUNTAIN_SHOWN =
      FLAG_AREA_TEMP_07, ///< the camera has shown it start once
  /// The pipe room's first pipe section has been pushed into place, joining
  /// the water pipes. With all three joined the water runs on below.
  FLAG_PIPE_1_JOINED = FLAG_AREA_TEMP_10,
  FLAG_PIPE_2_JOINED = FLAG_AREA_TEMP_11,
  FLAG_PIPE_3_JOINED = FLAG_AREA_TEMP_12,
  /// Unused: cleared with the three, but nothing sets or tests it.
  FLAG_UNUSED_PIPE_4_JOINED = FLAG_AREA_TEMP_13,
  FLAG_WEST_EAST_PIPE_PUSHED = FLAG_AREA_TEMP_18,
  /// Where the switch pipe rests in the room of jets, and so which jet runs
  /// (MercuryLighthouse_PushJetRoomPipes). At column 47 it is JET_WESTWARD,
  /// along the sliding statue's row.
  FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET = FLAG_AREA_TEMP_19,
  /// At column 48 it is JET_EASTWARD, which slides the statue into the hole.
  FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET = FLAG_AREA_TEMP_1A,
  /// At column 53 no jet runs. With none of the three set JET_SOUTHWARD runs.
  FLAG_SWITCH_PIPE_STOPS_JETS = FLAG_AREA_TEMP_1B,
  /// Two flags per stone statue of the row along the north wall, one for each
  /// column it can be pushed to, then the last statue's.
  FLAG_FIRST_ROW_STATUE = FLAG_AREA_TEMP_30,
  FLAG_LAST_ROW_STATUE_WEST = FLAG_AREA_TEMP_38,
  FLAG_LAST_ROW_STATUE_EAST = FLAG_AREA_TEMP_39,
  /// The stone statue of the room of vines has been moved west off the
  /// doorway out to the paths, for good.
  FLAG_PASSAGE_STATUE_MOVED = FLAG_AREA_TEMP_48
};

/// Scene flags, kept while the party stays in the scene.
enum {
  /// The Empty Bottle has been filled at the Fountain of Healing this visit.
  FLAG_HERMES_WATER_TAKEN = FLAG_SCENE_TEMP_00,
  FLAG_LEADER_ON_FLOOR_SWITCH = FLAG_SCENE_TEMP_56
};

/// The chests: a Sleep Bomb, a Psy Crystal and a Nut.
enum {
  FLAG_CHEST_SLEEP_BOMB = 0xf64,
  FLAG_CHEST_PSY_CRYSTAL = 0xf66,
  FLAG_CHEST_NUT = 0xf67
};

// UB: the fountain rows include negative pixel offsets. Their range is
// -23..0, so multiplication is defined and fits the fixed-point coordinate.
#ifdef UBFIX
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) ((value) * FIXED_ONE)
#else
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) INTtoFIXED(value)
#endif

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"
#include "shared/staged_actor.c"

/// Talking to Sleet, the Mercury Djinni in the small room that the third
/// waterfall from the west of the first room of six waterfalls leads to, starts
/// a battle with it.
static void MercuryLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SLEET, DJINNI_SLEET);
}

#if TBS_PATCH_3
/// The rows of MercuryLighthouseMiddle's gate room where the fade overlay lets
/// go of the leader walking on the ground layer.
#define OVERLAY_FREE_FIRST_ROW 13
#define OVERLAY_FREE_LAST_ROW 18

/// Task: the fade overlay stops following the leader while he crosses the
/// rows from OVERLAY_FREE_FIRST_ROW to OVERLAY_FREE_LAST_ROW on
/// MAPGRID_LAYER_GROUND.
static void MercuryLighthouse_UpdateLeaderEffectTarget(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct FadeOverlayState *effectState = gFadeOverlayState;

  if (leader->layer == 0 &&
      FIELD_CELL(leader->z.fixed) >= OVERLAY_FREE_FIRST_ROW &&
      FIELD_CELL(leader->z.fixed) <= OVERLAY_FREE_LAST_ROW)
    effectState->actor = NULL;
  else
    effectState->actor = leader;
}
#endif

#include "shared/mercury_lighthouse_sparks.c"
#include "shared/mercury_lighthouse_palette_cycle.c"

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
    return gMercuryLighthouseExteriorEntrances;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
    return gMercuryLighthouseUpperEntrances;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
    return gMercuryLighthouseMiddleEntrances;
  }
  return gMercuryLighthouseLowerEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

/// The four scenes share one exit table.
const u32 *Scene_GetExits(void) { return gMercuryLighthouse_Exits; }

/// Each scene's people, statues and pipes; the empty table is for no scene.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
    return gMercuryLighthouseLowerActorPlacements;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
    return gMercuryLighthouseMiddleActorPlacements;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
    return gMercuryLighthouseUpperActorPlacements;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
    return gMercuryLighthouseExteriorActorPlacements;
  }
  return gMercuryLighthouseNoActorPlacements;
}

/// Ripples spread round Isaac while he stands out on the open water of the
/// great pool room, held up by one of his lights.
///
/// The open water's cells are CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER only
/// once Ply has woken the goddess's pool below
/// (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), when `Scene_Initialize` copies the
/// pool's cells in.
static void MercuryLighthouse_RippleWhileInPool(void) {
  MercuryLighthouse_RippleAroundLeader();
}

/// Stepping onto a plain stepping square or the floor at the edge of the great
/// pool room puts out the golden rings of light over Isaac's head, which swell
/// and fade away.
///
/// The squares and the floor's edge are
/// CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP. Two cells beside the Sleep Bomb
/// chest, CELL_BEHAVIOR_SLEEP_BOMB_LEDGE, do the same but are no place to hop
/// from.
static void MercuryLighthouse_LoseLightsOnLower(void) {
  MercuryLighthouse_LoseLights();
}

/// In the great pool room, pushing against the water from a stepping square,
/// the floor's edge or the open water, Isaac hops two cells the way he faces,
/// to the next square or out onto the water.
///
/// A hop onto open water spends one of the golden rings over his head, which
/// flies ahead to hold him up where he lands; with none left he does not hop
/// out onto it (shared/mercury_lighthouse_pillar.c).
static void MercuryLighthouse_HopAcrossPoolOnLower(void) {
  MercuryLighthouse_HopAcrossPool();
}

/// Stepping onto one of the great pool room's stepping squares marked with a
/// cross puts golden rings of light over Isaac's head, one more every half
/// second up to three. Each lets him hop once onto the open water.
///
/// The cross-marked squares are CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART
/// once Ply has woken the goddess's pool below.
static void MercuryLighthouse_ChargeLightsOnLower(void) {
  MercuryLighthouse_ChargeLights();
}

/// Searching the lighthouse door before Mia has opened it, cell (52, 36), shows
/// "Strange forces are at work... It seems impossible to proceed."
///
/// The door glows until then (MercuryLighthouse_CycleEntrancePalette). Once Mia
/// opens it, the cell becomes the door's exit, CELL_BEHAVIOR_LIGHTHOUSE_DOOR.
static void MercuryLighthouse_InspectSealedDoor(void) {
  Event_Start();
  FieldMessage_Show(MsgMercuryLighthouseStrangeForcesBlockWay, 1);
  Event_End();
}

/// Each scene's triggers, the exterior's last.
const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineState.next;

  if (scene == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
    return gMercuryLighthouseLowerTriggers;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
    return gMercuryLighthouseMiddleTriggers;
  }
  if (scene == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
    return gMercuryLighthouseUpperTriggers;
  }
  return gMercuryLighthouseExteriorTriggers;
}

#include "shared/mercury_lighthouse_waterfall.c"

/// Walking north into the third of the six waterfalls from the west, in the
/// first room of waterfalls (cell (16, 7)), Isaac vanishes in a trail of
/// sparkles and comes out in Sleet's small room.
///
/// Its cell's behaviour is its exit's number,
/// MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, past
/// EXIT_CELL_BEHAVIOR_OFFSET.
static void MercuryLighthouse_WaterfallToSleetsRoom(void) {
  MercuryLighthouse_WalkIntoWaterfall(gFieldState->stepTrigger -
                                      EXIT_CELL_BEHAVIOR_OFFSET);
}

/// Walking north into the easternmost of the six waterfalls of the second room
/// of waterfalls (cell (28, 37)), Isaac vanishes in a trail of sparkles and
/// comes out at the foot of the room of vines, through
/// MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP.
static void MercuryLighthouse_WaterfallToGapRoom(void) {
  MercuryLighthouse_WalkIntoWaterfall(gFieldState->stepTrigger -
                                      EXIT_CELL_BEHAVIOR_OFFSET);
}

/// Walking north into the waterfall under the rainbow (cell (49, 13)), Isaac
/// vanishes in a trail of sparkles and comes out again on the path that led
/// there, through MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH.
static void MercuryLighthouse_WaterfallToUpperPath(void) {
  MercuryLighthouse_WalkIntoWaterfall(gFieldState->stepTrigger -
                                      EXIT_CELL_BEHAVIOR_OFFSET);
}

/// Once all three pipe sections of the pipe room are joined, the floor rumbles
/// and shakes and the screen fades out. It shows the ground floor's room that
/// the channels feed for two seconds
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE), with Isaac
/// hidden, and then the party is back in the pipe room.
///
/// From then on FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN chooses the running
/// channels' entrances in mercury_lighthouse_entrance.c.
static void MercuryLighthouse_ExitWhenChannelsOpen(void) {
  if (GameFlag_Get(FLAG_PIPE_1_JOINED) != 0 &&
      GameFlag_Get(FLAG_PIPE_2_JOINED) != 0 &&
      GameFlag_Get(FLAG_PIPE_3_JOINED) != 0) {
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
    Event_Wait(30);
    MapGrid_SetShake(1, 1, 1);
    Sound_Play(SE_RUMBLE);
    Event_Wait(60);
    gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
    MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
    MapGrid_WaitForShakeToSettle();
    Import_EventSetExitRequest(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE);
  } else {
    GameFlag_Clear(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
  }
}

/// Where each channel pipe joins its channel: the first two along their rows,
/// the third along its column.
#define FIRST_PIPE_JOINED_ROW 8
#define SECOND_PIPE_JOINED_ROW 12
#define THIRD_PIPE_JOINED_COLUMN 40

/// Walking into one of the pipe room's three pipe sections, Isaac pushes it
/// across its grated pool. Where it joins the pipes on either side, water
/// bursts from the next pipe end in a spray of sparkles; pushed away again, it
/// breaks the channel. With all three joined the water runs on below
/// (MercuryLighthouse_ExitWhenChannelsOpen).
///
/// The cells copied redraw the water in the joined pipes. The second pipe's
/// spray shows only once the first is joined, as the water reaches it through
/// the first.
static void MercuryLighthouse_PushChannelPipes(void) {
  struct PushedActorProbe probe;
  s32 column;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    // Each pipe controls one channel; restore the connected water tiles too.
    switch ((u32)probe.actorSlot) {
    case ACTOR_FIRST_PIPE:
      if (FIELD_CELL(probe.position.z) == FIRST_PIPE_JOINED_ROW) {
        Actor_MoveAndRedraw(probe);
        Event_Wait(20);
        MapGrid_CopyCellsTo(119, 9, 109, 11, 1, 1);
        MercuryLighthouse_SparkleBurst(toFIXED(726), 0, toFIXED(180),
                                       FACING_WEST);
        GameFlag_Set(FLAG_PIPE_1_JOINED);
      } else {
        MapGrid_CopyCellsTo(117, 9, 104, 7, 1, 1);
        MapGrid_CopyCellsTo(119, 8, 109, 11, 1, 1);
        MapGrid_CopyCellsTo(118, 8, 104, 13, 1, 1);
        Actor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_PIPE_1_JOINED);
      }
      break;
    case ACTOR_SECOND_PIPE:
      if (FIELD_CELL(probe.position.z) == SECOND_PIPE_JOINED_ROW) {
        Actor_MoveAndRedraw(probe);
        Event_Wait(10);
        if (Event_GetFlag(FLAG_PIPE_1_JOINED)) {
          MapGrid_CopyCellsTo(118, 9, 104, 13, 1, 1);
          MercuryLighthouse_SparkleBurst(toFIXED(644), 0, toFIXED(210),
                                         FACING_SOUTH);
        }
        GameFlag_Set(FLAG_PIPE_2_JOINED);
      } else {
        MapGrid_CopyCellsTo(119, 8, 109, 11, 1, 1);
        if (GameFlag_Get(FLAG_PIPE_1_JOINED)) {
          MapGrid_CopyCellsTo(119, 9, 109, 11, 1, 1);
          MapGrid_CopyCellsTo(118, 8, 104, 13, 1, 1);
        }
        Actor_MoveAndRedraw(probe);
        Event_ClearFlag(FLAG_PIPE_2_JOINED);
      }
      break;
    case ACTOR_THIRD_PIPE:
      column = probe.position.x;
      if (FIELD_CELL(column) == THIRD_PIPE_JOINED_COLUMN) {
        Actor_MoveAndRedraw(probe);
        Event_SetFlag(FLAG_PIPE_3_JOINED);
      } else {
        Actor_MoveAndRedraw(probe);
        Event_ClearFlag(FLAG_PIPE_3_JOINED);
      }
      break;
    }
    MercuryLighthouse_ExitWhenChannelsOpen();
  }
  Event_End();
}

/// Walking into one of the spouts of water that shoot from the pipe ends of the
/// big pipe room while its spout pipe is joined, Isaac is carried back out to
/// the spout's tip, standing, in a trail of sparkles: the spouts bar the way.
/// The jet along the north wall of the room with the Nut chest does the same
/// while it runs.
///
/// Each spout's cell behaviour picks the tip he is carried to. Every edition
/// but the Japanese carries him a step further back from the jet in the Nut
/// chest's room and pauses half a second (MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK
/// and TBS_PATCH_1).
static void MercuryLighthouse_PushLeaderOutOfSpout(void) {
  struct FieldState *state;

  state = gFieldState;
  Event_Start();
  Task_AddFrame(MercuryLighthouse_SparkleAroundLeader, TASK_PASS_FRAME);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2.5);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Sound_Play(SE_SPARKLING_PASSAGE);
  if (state->stepTrigger == CELL_BEHAVIOR_EAST_SPOUT) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 232, 616);
  } else if (state->stepTrigger == CELL_BEHAVIOR_SOUTH_SPOUT) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 360, 728);
  } else if (state->stepTrigger == CELL_BEHAVIOR_WEST_SPOUT) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 248, 792);
  } else {
    MERCURY_LIGHTHOUSE_CARRY_LEADER_BACK(ACTOR_PARTY_LEADER, 696, 592);
#if TBS_PATCH_1
    Actor_SetDestination(ACTOR_PARTY_LEADER, 696, 600);
    Event_Wait(30);
#endif
  }
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_Get(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Task_Remove(MercuryLighthouse_SparkleAroundLeader);
  Event_End();
}

/// In the gate room, walking into the blue statue on the terrace pushes it a
/// step while the sealed arch on the dais is shut, until it rests on the floor
/// switch at the terrace's west end
/// (MercuryLighthouse_OpenGateWithMovedStatue).
static void MercuryLighthouse_PushGateSwitchStatue(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  MercuryLighthouse_OpenGateWithMovedStatue();
}

/// Once the gate room's blue statue stands on the floor switch at the west end
/// of the terrace (cell (5, 13)), whether moved by Move or pushed, the switch
/// sinks under it and the sealed arch at the top of the ladder on the dais
/// opens for good.
///
/// The statue is lowered 2 pixels onto the sunken switch, and the arch's cell
/// (9, 10) takes the open doorway's behaviour from (9, 5).
static void MercuryLighthouse_OpenGateWithMovedStatue(void) {
  s32 column;
  s32 row;

  column = Actor_Get(ACTOR_GATE_STATUE)->x.fixed / toFIXED(16);
  row = Actor_Get(ACTOR_GATE_STATUE)->z.fixed / toFIXED(16);
  Event_Start();
  if (column == 5 && row == 13) {
    Actor_Get(ACTOR_GATE_STATUE)->y.fixed -= toFIXED(2);
    Actor_Get(ACTOR_GATE_STATUE)->targetY =
        Actor_Get(ACTOR_GATE_STATUE)->y.fixed;
    MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
    Sound_Play(SE_FLOOR_SWITCH);
    MapGrid_PlayMapEdit(gMercuryLighthouse_GateCells, 9, 7);
    MapGrid_CopyCellAttributes(9, 5, 1, 1, 9, 10);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN);
  }
  Event_End();
}

/// Standing on the gate room's floor switch, at the west end of the terrace
/// (cell (5, 13)), sinks it under Isaac and opens the sealed arch at the top of
/// the ladder on the dais, for as long as he stays on it.
///
/// The switch answers only once Isaac stands near its middle, the x and z range
/// checked here.
static void MercuryLighthouse_PressGateFloorSwitch(void) {
  s32 x;
  s32 z;

  if (Event_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) == 0) {
    x = Actor_Get(ACTOR_PARTY_LEADER)->x.part[0].integer;
    z = Actor_Get(ACTOR_PARTY_LEADER)->z.part[0].integer;
    if (x >= 84 && x <= 91 && z > 211 && z <= 219) {
      Event_Start();
      GameFlag_Set(FLAG_LEADER_ON_FLOOR_SWITCH);
      Event_Wait(5);
      Actor_Get(ACTOR_PARTY_LEADER)->y.fixed -= toFIXED(2);
      Actor_Get(ACTOR_PARTY_LEADER)->targetY =
          Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
      MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
      Sound_Play(SE_FLOOR_SWITCH);
      MapGrid_PlayMapEdit(gMercuryLighthouse_GateCells, 9, 7);
      Event_End();
    }
  }
}

/// Stepping off the gate room's floor switch lets it rise again, and the arch
/// at the top of the ladder seals itself.
static void MercuryLighthouse_ReleaseGateFloorSwitch(void) {
  if (Event_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    Event_Start();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed += toFIXED(2);
    Actor_Get(ACTOR_PARTY_LEADER)->targetY =
        Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
    Event_Wait(5);
    MapGrid_CopyCellsTo(7, 2, 5, 11, 1, 1);
    Sound_Play(SE_FLOOR_SWITCH);
    MapGrid_AnimateCells(gMercuryLighthouse_GateCloseCells, 9, 7);
    Event_End();
  }
}

/// Draws the jet along the north wall of the room with the Nut chest a step at
/// a time, the floor shaking at each step when delayFrames is not 0, and copies
/// in the doorway that opens in the wall above it,
/// MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, with
/// its cells.
static void MercuryLighthouse_AnimateFountainTiles(s32 delayFrames,
                                                   s32 firstStep, s32 endStep) {
  u32 step;
  s32 sourceColumn;

  if (delayFrames != 0) {
    Sound_Play(SE_GUSH);
  }
  for (step = firstStep; step < endStep; step++) {
    MapGrid_CopyCellsTo(45 - (step << 1), 32, 44 - (step << 1), 32, (step + 1),
                        6);
    MapGrid_CopyCellsTo(45 - step, 51, 45 - step, 32, 1, 6);
    sourceColumn = 109 - step;
    MapGrid_CopyCellsTo(sourceColumn, 32, 108 - step, 32, 1, 4);
    MapGrid_CopyCellsTo(sourceColumn, 51, sourceColumn, 32, 1, 4);
    if (delayFrames != 0) {
      MapGrid_SetShake(5, 5, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      Event_Wait(delayFrames);
    }
  }
  MapGrid_CopyCellAttributes(42, 52, 4, 5, 42, 33);
}

/// Takes the jet and its doorway away again a step at a time, the floor
/// shaking, as the pipe is pushed back.
static void MercuryLighthouse_RestoreFountainTiles(s32 delayFrames) {
  u32 step;
  s32 destinationColumn;
  s32 sourceColumn;
  s32 column;

  Sound_Play(SE_GUSH);
  {
    // The fountain's strips are six cells tall.
    s32 height = 6;

    step = 0;
    destinationColumn = 41;
    sourceColumn = 40;
    do {
      MapGrid_CopyCellsTo(sourceColumn, 32, destinationColumn, 32, 3 - step,
                          height);
      {
        s32 width = 2;

        MapGrid_CopyCellsTo(39, 51, sourceColumn, 32, 1, height);
        column = step + 106;
        {
          s32 stripHeight = 4;

          MapGrid_CopyCellPictureIndicesRect(105, 51, column, 32, width,
                                             stripHeight);
          if (delayFrames != 0) {
            MapGrid_SetShake(5, 5, 1);
            MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
            Event_Wait(delayFrames);
          }
          step++;
          destinationColumn += 2;
          sourceColumn += 2;
        }
      }
    } while (step <= 2);
  }
  Sound_Play(SE_STOP_ACTION_SE);
  MapGrid_CopyCellAttributes(106, 33, 4, 5, 42, 33);
  MapGrid_WaitForShakeToSettle();
}

/// Task: sparkles on the jet while it runs.
static void MercuryLighthouse_SparkleAtFountain(void) {
  MercuryLighthouse_SparkleAt(toFIXED(690), 0, toFIXED(584));
}

/// Starts the jet along the north wall of the room with the Nut chest, its
/// leading edge throwing up spray and sparkles, and keeps it sparkling.
static void MercuryLighthouse_OpenFountain(s32 delayFrames) {
  u32 row;
  s32 rowOffset;
  FIXED particleX;
  u32 particleIndex;
  s32 sparkleOffset;
  s32 rowShift;
  FIXED velocityX;
  FIXED velocityZ;
  u32 nextRow;
  struct EffectOptions options;

  MapGrid_CopyCellsTo(78, 59, 110, 36, 1, 1);
  MapGrid_CopyCellsTo(76, 59, 109, 36, 1, 1);
  options.palette = SPRITE_COLOR_WHITE;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  row = 0;
  // Each row's edge sparkles and throws up spray as it opens.
  do {
    rowOffset = row << 4;
    particleIndex = 0;
    rowShift = -(row * toFIXED(16));
    particleX = toFIXED(728) + rowShift;
    do {
      if ((particleIndex & 1) != 0) {
        velocityX = Random_Below(8) * (toFIXED(1) / 5) - toFIXED(4) / 5;
        sparkleOffset = -particleIndex;
        velocityZ = Random_Below(8) * (toFIXED(1) / 5) - toFIXED(4) / 5;
        FieldEffect_Spawn(particleX, 0, toFIXED(584), velocityX, 0, velocityZ,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                          &options);
        Event_Wait(1);
      } else {
        sparkleOffset = -particleIndex;
      }
      MercuryLighthouse_SparkleAt(
          MERCURY_LIGHTHOUSE_OFFSET_FIXED(sparkleOffset - rowOffset) +
              toFIXED(728),
          0, toFIXED(584));
      particleIndex++;
      particleX -= toFIXED(1);
    } while (particleIndex <= 7);
    MapGrid_CopyCellsTo(76, 59, 108 - row, 36, 2, 1);
    nextRow = row + 1;
    MercuryLighthouse_AnimateFountainTiles(delayFrames, row, nextRow);
    row = nextRow;
  } while (nextRow <= 1);
  Event_Wait(delayFrames);
  MercuryLighthouse_AnimateFountainTiles(0, nextRow, nextRow + 1);
  Sound_Play(SE_MERCURY_LIGHTHOUSE_MECHANISM);
  Task_AddFrame(MercuryLighthouse_SparkleAtFountain, TASK_PASS_FRAME);
  MapGrid_WaitForShakeToSettle();
}

/// As the pipe moves away (the push's callback): spray drifts off the jet and
/// its sparkles stop.
static void MercuryLighthouse_StopFountain(void) {
  u32 row;
  u32 particleIndex;
  FIXED offsetX;
  FIXED particleX;
  FIXED particleZ;
  struct EffectOptions options;

  MapGrid_CopyCellsTo(78, 58, 110, 36, 1, 1);
  options.palette = SPRITE_COLOR_BRG;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  for (row = 0; row <= 2; row++) {
    offsetX = -toFIXED(2);
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        particleZ = ((584 - Random_Below(5)) & 0xffff) << 16;
        particleX = (offsetX - row * toFIXED(8)) + toFIXED(728);
        FieldEffect_Spawn(particleX, 0, particleZ, -toFIXED(1) / 4, 0, 0,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                          &options);
        Event_Wait(1);
      }
      offsetX -= toFIXED(2);
    }
    MapGrid_CopyCellsTo(111, 35, (109 - row), 36, 1, 1);
  }
  Task_Remove(MercuryLighthouse_SparkleAtFountain);
}

/// The spout pipe joins the pipes above and below its pool at this column, and
/// the spouts run; anywhere else they stop.
#define SPOUT_PIPE_JOINED_COLUMN 11
/// The pipe in the room with the Nut chest joins the pipes either side of its
/// pool at the first row, and the jet runs; at the second it stops.
#define FOUNTAIN_PIPE_RUNNING_ROW 40
#define FOUNTAIN_PIPE_STOPPED_ROW 42

/// Walking into a pipe section, Isaac pushes it across its grated pool. In the
/// big pipe room the spout pipe, joined to the pipes above and below it, sets
/// water spouting from four pipe ends, one of them a tall spout in the room's
/// west; in the room with the Nut chest the pipe joined across its pool sends a
/// jet along the north wall and opens a doorway in the wall above it, the
/// camera turning to show it the first time. Pushed away again, each stops its
/// water.
///
/// Three of the spouts bar the way (MercuryLighthouse_PushLeaderOutOfSpout),
/// and so does the jet.
static void MercuryLighthouse_PushSpoutAndFountainPipes(void) {
  struct PushedActorProbe probe;
  s32 pillarId;
  s32 column;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    pillarId = probe.actorSlot;
    // The spout pipe starts or stops the spouts; the other starts or stops
    // the jet.
    if (pillarId == ACTOR_SPOUT_PIPE) {
      column = probe.position.x;
      if (FIELD_CELL(column) == SPOUT_PIPE_JOINED_COLUMN) {
        Actor_MoveAndRedraw(probe);
        Event_Wait(30);
        Sound_Play(SE_MERCURY_LIGHTHOUSE_MECHANISM);
        MercuryLighthouse_StartSpoutsWithSparkles();
        MapGrid_CopyCellsTo(76, 60, 74, 38, 3, 1);
        MapGrid_CopyCellsTo(77, 60, 76, 38, 2, 1);
        MapGrid_CopyCellsTo(75, 58, 86, 41, 1, 3);
        MapGrid_CopyCellsTo(75, 59, 86, 43, 1, 2);
        MapGrid_CopyCellsTo(76, 59, 80, 49, 2, 1);
        MapGrid_CopyCellsTo(77, 59, 82, 49, 2, 1);
        GameFlag_Set(FLAG_SPOUTS_RUNNING);
      } else {
        probe.callback = MercuryLighthouse_StopSpoutsWithSparkles;
        MapGrid_CopyCellsTo(75, 57, 86, 41, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 42, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 43, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 44, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 80, 49, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 81, 49, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 82, 49, 1, 1);
        MapGrid_CopyCellsTo(78, 58, 83, 49, 1, 1);
        Actor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_SPOUTS_RUNNING);
      }
    } else if (pillarId == ACTOR_FOUNTAIN_PIPE) {
      if (FIELD_CELL(probe.position.z) == FOUNTAIN_PIPE_RUNNING_ROW) {
        Actor_MoveAndRedraw(probe);
        if (Event_GetFlag(FLAG_FOUNTAIN_SHOWN) == 0) {
          FieldCamera_SetSpeed(1.5);
          FieldCamera_MoveTo(714, CAMERA_KEEP, 592, 1);
          FieldCamera_WaitForArrival();
          GameFlag_Set(FLAG_FOUNTAIN_SHOWN);
          MercuryLighthouse_OpenFountain(5);
          Event_Wait(50);
        } else {
          MercuryLighthouse_OpenFountain(5);
        }
        GameFlag_Set(FLAG_FOUNTAIN_RUNNING);
      } else if (FIELD_CELL(probe.position.z) == FOUNTAIN_PIPE_STOPPED_ROW) {
        probe.callback = MercuryLighthouse_StopFountain;
        Actor_MoveAndRedraw(probe);
        MercuryLighthouse_RestoreFountainTiles(5);
        GameFlag_Clear(FLAG_FOUNTAIN_RUNNING);
      }
    }
  }
  Event_End();
}

/// In the upper gate room, walking into the blue statue that fell from the room
/// of jets pushes it a step while the middle arch is shut, until it rests on
/// the floor switch in the middle of the room
/// (MercuryLighthouse_OpenUpperGateWithMovedStatue).
static void MercuryLighthouse_PushUpperGateSwitchStatue(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  MercuryLighthouse_OpenUpperGateWithMovedStatue();
}

/// Once the fallen blue statue stands on the floor switch in the middle of the
/// upper gate room (cell (10, 23)), whether moved by Move or pushed, the switch
/// sinks under it and the sealed middle arch of the three in the north wall
/// opens for good.
///
/// The statue is lowered 2 pixels onto the sunken switch, and the arch's cell
/// (10, 19) takes the open doorway's behaviour from (10, 16).
static void MercuryLighthouse_OpenUpperGateWithMovedStatue(void) {
  s32 column = Actor_Get(ACTOR_FALLEN_STATUE)->x.fixed / toFIXED(16);
  s32 row = Actor_Get(ACTOR_FALLEN_STATUE)->z.fixed / toFIXED(16);

  Event_Start();
  if (column == 10 && row == 23) {
    Actor_Get(ACTOR_FALLEN_STATUE)->y.fixed -= toFIXED(2);
    Actor_Get(ACTOR_FALLEN_STATUE)->targetY =
        Actor_Get(ACTOR_FALLEN_STATUE)->y.fixed;
    MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
    Sound_Play(SE_FLOOR_SWITCH);
    MapGrid_PlayMapEdit(gMercuryLighthouse_UpperGateCells, 10, 18);
    MapGrid_CopyCellAttributes(10, 16, 1, 1, column, 19);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN);
  }
  Event_End();
}

/// Standing on the floor switch in the middle of the upper gate room, cell
/// (10, 23), sinks it under Isaac and opens the sealed middle arch of the three
/// in the north wall, for as long as he stays on it.
static void MercuryLighthouse_PressUpperGateFloorSwitch(void) {
  s32 x;
  s32 z;

  if (Event_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0)
    return;
  x = Actor_Get(ACTOR_PARTY_LEADER)->x.part[0].integer;
  z = Actor_Get(ACTOR_PARTY_LEADER)->z.part[0].integer;
  if (x < 164 || x > 171)
    return;
  if (z < 372)
    return;
  if (z >= 380)
    return;
  Event_Start();
  GameFlag_Set(FLAG_LEADER_ON_FLOOR_SWITCH);
  Event_Wait(5);
  Actor_Get(ACTOR_PARTY_LEADER)->y.fixed -= toFIXED(2);
  Actor_Get(ACTOR_PARTY_LEADER)->targetY =
      Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
  MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
  Sound_Play(SE_FLOOR_SWITCH);
  MapGrid_PlayMapEdit(gMercuryLighthouse_UpperGateCells, 10, 18);
  Event_End();
}

/// Stepping off the upper gate room's floor switch lets it rise again, and the
/// middle arch seals itself.
static void MercuryLighthouse_ReleaseUpperGateFloorSwitch(void) {
  if (Event_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    Event_Start();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed += toFIXED(2);
    Actor_Get(ACTOR_PARTY_LEADER)->targetY =
        Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
    Event_Wait(5);
    MapGrid_CopyCellsTo(8, 29, 10, 23, 1, 1);
    Sound_Play(SE_FLOOR_SWITCH);
    MapGrid_AnimateCells(gMercuryLighthouse_UpperGateCloseCells, 10, 18);
    Event_End();
  }
}

/// The three jets of the room of jets, named for the way each pushes the blue
/// statue: JET_SOUTHWARD falls from the pipe end north of the statue's first
/// place, JET_EASTWARD shoots along the row of the square hole from the pipe
/// end at its west, and JET_WESTWARD shoots along the statue's first row from
/// the pipe end at its east.
enum { JET_SOUTHWARD = 0, JET_EASTWARD = 1, JET_WESTWARD = 2 };

/// Where the switch pipe stops along its pool, in cells: at the first column
/// the water runs to JET_WESTWARD, at the second to JET_EASTWARD, at the third
/// no jet runs, and anywhere else, as at the last, where it starts, the water
/// falls as JET_SOUTHWARD.
#define SWITCH_PIPE_WESTWARD_JET_COLUMN 47
#define SWITCH_PIPE_EASTWARD_JET_COLUMN 48
#define SWITCH_PIPE_NO_JET_COLUMN 53
#define SWITCH_PIPE_SOUTHWARD_JET_COLUMN 54
/// The row of the pipe lying west to east once it has been pushed south into
/// the pool (FLAG_WEST_EAST_PIPE_PUSHED).
#define WEST_EAST_PIPE_PUSHED_ROW 38

/// A random speed for the spray a jet throws up.
static inline s32 MercuryLighthouse_RandomSpraySpeed(void) {
  return Random_Below(8) * (toFIXED(1) / 5);
}

/// Starts one jet of the room of jets: the water reaches out from its pipe end
/// a cell at a time, throwing up spray.
static void MercuryLighthouse_StartJet(s32 jet) {
  struct EffectOptions options;
  u32 particleIndex;
  u32 row;

  Sound_Play(SE_MERCURY_LIGHTHOUSE_MECHANISM);
  if (jet == JET_SOUTHWARD) {
    MapGrid_CopyCellsTo(111, 57, 113, 42, 1, 1);
    MapGrid_CopyCellsTo(111, 59, 113, 43, 1, 1);
  } else if (jet == JET_EASTWARD) {
    MapGrid_CopyCellsTo(113, 58, 112, 46, jet, jet);
    MapGrid_CopyCellsTo(115, 58, 113, 46, jet, jet);
  } else {
    MapGrid_CopyCellsTo(115, 57, 116, 44, 1, 1);
    MapGrid_CopyCellsTo(113, 57, 115, 44, 1, 1);
  }
  options.palette = SPRITE_COLOR_WHITE;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (jet == JET_SOUTHWARD) {
          FieldEffect_Spawn(
              toFIXED(792), 0,
              toFIXED(704) + (row * 16 + particleIndex) * toFIXED(1),
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5, 0,
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        } else if (jet == JET_EASTWARD) {
          FieldEffect_Spawn(
              toFIXED(800) + (row * 16 + particleIndex) * toFIXED(1), 0,
              toFIXED(746),
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5, 0,
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        } else {
          FieldEffect_Spawn(
              toFIXED(812) + (row * 16 + particleIndex) * -toFIXED(1), 0,
              toFIXED(714),
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5, 0,
              MercuryLighthouse_RandomSpraySpeed() - toFIXED(4) / 5,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        }
        Event_Wait(1);
      }
    }
    if (jet == JET_SOUTHWARD) {
      MapGrid_CopyCellsTo(111, 58, 113, row + 43, 1, 1);
      MapGrid_CopyCellsTo(111, 59, 113, row + 44, 1, 1);
    } else if (jet == JET_EASTWARD) {
      MapGrid_CopyCellsTo(114, 58, row + 113, 46, jet, jet);
      MapGrid_CopyCellsTo(115, 58, row + 114, 46, jet, jet);
    } else {
      MapGrid_CopyCellsTo(114, 57, 115 - row, 44, 1, 1);
      MapGrid_CopyCellsTo(113, 57, 114 - row, 44, 1, 1);
    }
  }
}

/// Stops a jet: the water draws back into its pipe end a cell at a time, spray
/// drifting from it. The first cell of all three jets is put back as it starts.
static void MercuryLighthouse_StopJet(s32 jet) {
  struct EffectOptions options;
  u32 particleIndex, row;

  MapGrid_CopyCellsTo(112, 57, 113, 42, 1, 1);
  MapGrid_CopyCellsTo(117, 58, 112, 46, 1, 1);
  MapGrid_CopyCellsTo(117, 57, 116, 44, 1, 1);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  options.palette = SPRITE_COLOR_BRG;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  for (row = 0; row <= 2; row++) {
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        if (jet == JET_SOUTHWARD) {
          FieldEffect_Spawn(
              INTtoFIXED(793 - Random_Below(5)), 0,
              (row * 4 + particleIndex) * toFIXED(2) + toFIXED(695), 0, jet,
              toFIXED(1) / 4, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
              &options);
        } else if (jet == JET_EASTWARD) {
          FieldEffect_Spawn(
              (row * 4 + particleIndex) * toFIXED(2) + toFIXED(786), 0,
              INTtoFIXED(Random_Below(5)) + toFIXED(744), toFIXED(1) / 4, 0, 0,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        } else {
          FieldEffect_Spawn(
              toFIXED(824) - particleIndex * toFIXED(2) - row * toFIXED(8), 0,
              INTtoFIXED(Random_Below(5)) + toFIXED(712), toFIXED(1) / 4, 0, 0,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        }
        Event_Wait(1);
      }
    }
    if (jet == JET_SOUTHWARD)
      MapGrid_CopyCellsTo(112, 58, 113, row + 43, 1, 1);
    else if (jet == JET_EASTWARD)
      MapGrid_CopyCellsTo(112, 58, row + 113, 46, jet, jet);
    else
      MapGrid_CopyCellsTo(112, 58, 115 - row, 44, 1, 1);
  }
}

/// The blue statue's first row, along which JET_WESTWARD pushes it under the
/// pipe end of JET_SOUTHWARD.
#define SLIDING_STATUE_UNDER_JET_ROW 44

/// The camera turns to the blue statue of the room of jets, and the jet just
/// started pushes it a step if the statue stands where that jet hits it:
/// JET_WESTWARD from its first place, then JET_SOUTHWARD from under the falling
/// jet's pipe end, then JET_EASTWARD from the row of the hole, into the hole.
/// Returns 1 when the slide is that last one.
///
/// The flags read are the switch pipe's new place, set before this is called.
static s32 MercuryLighthouse_StartStatueSlide(void) {
  s32 cellX;
  s32 cellZ;

  cellX = Actor_Get(ACTOR_SLIDING_STATUE)->x.fixed / toFIXED(16);
  cellZ = Actor_Get(ACTOR_SLIDING_STATUE)->z.fixed / toFIXED(16);
  FieldCamera_SetSpeed(5);
  FieldCamera_MoveTo(816, CAMERA_KEEP, 712, 1);
  FieldCamera_WaitForArrival();
  // A slide answers only the jet that hits the statue where it stands.
  if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL)) {
    if (cellX == 50 && GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
      Actor_SetScript(ACTOR_SLIDING_STATUE, gMercuryLighthouse_StatueSlideWest);
    } else if (cellX == 49) {
      if (cellZ == SLIDING_STATUE_UNDER_JET_ROW &&
          !GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET) &&
          !GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET) &&
          !GameFlag_Get(FLAG_SWITCH_PIPE_STOPS_JETS)) {
        Actor_SetScript(ACTOR_SLIDING_STATUE,
                        gMercuryLighthouse_StatueSlideSouth);
      } else if (cellZ == SLIDING_STATUE_UNDER_JET_ROW &&
                 GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
        Actor_SetScript(ACTOR_SLIDING_STATUE,
                        gMercuryLighthouse_StatueStaysPut);
      } else if (cellZ == 46 &&
                 GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
        Actor_SetScript(ACTOR_SLIDING_STATUE,
                        gMercuryLighthouse_StatueSlideIntoHole);
        Event_Wait(30);
        return 1;
      }
    }
  }
  Event_Wait(30);
  return 0;
}

/// Sparkles rise from the sliding statue as it moves.
static void MercuryLighthouse_SparkleAboveStatue(void) {
  struct Actor *statue = Actor_Get(ACTOR_SLIDING_STATUE);
  FIXED particleY = statue->y.fixed + INTtoFIXED(Random_Below(4));
  FIXED particleZ = statue->z.fixed;

  MercuryLighthouse_SparkleAt(statue->x.fixed, particleY, particleZ);
}

/// The slide scripts start the sparkles as the statue sets off.
static s32 MercuryLighthouse_StartStatueSparkles(struct Actor *actor) {
  Task_AddFrame(MercuryLighthouse_SparkleAboveStatue, TASK_PASS_FRAME);
  return 0;
}

/// The sparkles stop when the statue halts; slide scripts and code call it.
/// An actor script passes its actor, which this one does not read.
#ifdef UBFIX
static s32 MercuryLighthouse_StopStatueSparkles(struct Actor *actor) {
#else
static s32 MercuryLighthouse_StopStatueSparkles(void) {
#endif
  Task_Remove(MercuryLighthouse_SparkleAboveStatue);
  return 0;
}

/// In the room of jets, walking into a pipe section, Isaac pushes it along its
/// pool. The pipe lying north to south turns the water to another pipe end
/// where it stops: the old jet stops, the new one starts, and the camera turns
/// to the blue statue, which the new jet may push a step. When JET_EASTWARD
/// slides the statue into the square hole, it falls through, and the view cuts
/// to the gate room below as it lands
/// (MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS). The pipe lying
/// west to east only moves.
static void MercuryLighthouse_PushJetRoomPipes(void) {
  struct PushedActorProbe probe;
  s32 destinationColumn;
  s32 startColumn;

  Event_Start();
  if (PushedActor_FindClearPosition(&probe) != 0) {
    switch (probe.actorSlot) {
    case ACTOR_WEST_EAST_PIPE:
      Actor_MoveAndRedraw(probe);
      if (FIELD_CELL(probe.position.z) == WEST_EAST_PIPE_PUSHED_ROW)
        GameFlag_Set(FLAG_WEST_EAST_PIPE_PUSHED);
      else
        GameFlag_Clear(FLAG_WEST_EAST_PIPE_PUSHED);
      break;
    case ACTOR_JET_SWITCH_PIPE:
      startColumn = FIELD_CELL(Actor_Get(ACTOR_JET_SWITCH_PIPE)->x.fixed);
      Actor_MoveAndRedraw(probe);
      destinationColumn = FIELD_CELL(probe.position.x);
      if (destinationColumn == SWITCH_PIPE_WESTWARD_JET_COLUMN) {
        GameFlag_Set(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STOPS_JETS);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == SWITCH_PIPE_SOUTHWARD_JET_COLUMN)
          MercuryLighthouse_StopJet(JET_SOUTHWARD);
        else if (startColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN)
          MercuryLighthouse_StopJet(JET_EASTWARD);
        MercuryLighthouse_StartJet(JET_WESTWARD);
        Event_Wait(60);
      } else if (destinationColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN) {
        GameFlag_Set(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STOPS_JETS);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
        if (MercuryLighthouse_StartStatueSlide() != 0) {
          FIXED x;

          MercuryLighthouse_StopJet(JET_WESTWARD);
          x = toFIXED(840);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSparkles(NULL);
#else
          MercuryLighthouse_StopStatueSparkles();
#endif
          MercuryLighthouse_StartJet(JET_EASTWARD);
          Actor_WaitForScriptEndById(ACTOR_SLIDING_STATUE);
          Actor_PrepareFX(x, 0, toFIXED(786), SPRITE_CRATE);
          Actor_PrepareFX(x, 0, toFIXED(818), SPRITE_CRATE);
          Actor_SetDestination(ACTOR_SLIDING_STATUE, 840, 744);
          Event_Wait(5);
          Sound_Play(SE_STONE_GRIND);
          Actor_WaitForMoveById(ACTOR_SLIDING_STATUE);
          Event_Wait(40);
          GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL);
          PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
          gFieldState->transition =
              SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
          Event_FinishScreenTransition();
          Event_WaitTransitionFrames();
          Import_EventSetExitRequest(
              MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS);
        } else {
          MercuryLighthouse_StopJet(JET_WESTWARD);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSparkles(NULL);
#else
          MercuryLighthouse_StopStatueSparkles();
#endif
          MercuryLighthouse_StartJet(JET_EASTWARD);
          Event_Wait(60);
        }
      } else if (destinationColumn == SWITCH_PIPE_NO_JET_COLUMN) {
        GameFlag_Set(FLAG_SWITCH_PIPE_STOPS_JETS);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
        MercuryLighthouse_StartStatueSlide();
        MercuryLighthouse_StopJet(JET_SOUTHWARD);
        Event_Wait(60);
      } else {
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STOPS_JETS);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == SWITCH_PIPE_WESTWARD_JET_COLUMN)
          MercuryLighthouse_StopJet(JET_WESTWARD);
        else if (startColumn == SWITCH_PIPE_EASTWARD_JET_COLUMN)
          MercuryLighthouse_StopJet(JET_EASTWARD);
        MercuryLighthouse_StartJet(JET_SOUTHWARD);
        Event_Wait(60);
      }
      break;
    }
  }
  Event_End();
}

/// Walking into one of the five stone statues along the north wall of the room
/// of statues pushes it a step along the wall. The third from the west and the
/// fifth, alone in its alcove to the east, stand in front of doorways to the
/// room with the Nut chest, which open when they are pushed aside
/// (MercuryLighthouse_RecordRowStatues).
static void MercuryLighthouse_PushRowStatue(void) {
  Event_Start();
  PushedActor_AdvancePair();
  MercuryLighthouse_RecordRowStatues();
  Event_End();
}

/// Move on one of the stone statues along the north wall of the room of
/// statues, or a push, moves it a step along the wall. Where each stands is
/// kept until the party leaves the lighthouse, so the statues stay where they
/// were pushed when Isaac comes back, and the fifth, pushed off its doorway,
/// lets him jump the gap in front of its alcove.
static void MercuryLighthouse_RecordRowStatues(void) {
  s32 statueIndex;
  s32 positionFlag;
  s32 column;

  Event_Start();
  // Four statues use paired flags for their two saved stopping columns.
  for (statueIndex = 0, positionFlag = FLAG_FIRST_ROW_STATUE; statueIndex <= 3;
       statueIndex++, positionFlag += 2) {
    column =
        Actor_Get(statueIndex + ACTOR_FIRST_ROW_STATUE)->x.fixed / toFIXED(16);
    if (column == (statueIndex << 2) + 39) {
      GameFlag_Set(positionFlag);
      GameFlag_Clear(positionFlag + 1);
    } else if (column == (statueIndex << 2) + 41) {
      GameFlag_Set(positionFlag + 1);
      GameFlag_Clear(positionFlag);
    } else {
      GameFlag_Clear(positionFlag);
      GameFlag_Clear(positionFlag + 1);
    }
  }
  // The last statue stands before the doorway to the room with the Nut
  // chest. Moved a column west or east it clears the doorway, and the cell at
  // (58, 7) becomes a jump spot: pushing north from it, the leader jumps the
  // cell between and walks up to the doorway. With the statue in place the cell
  // is plain ground and the leader stays south of it.
  column = Actor_Get(ACTOR_LAST_ROW_STATUE)->x.fixed / toFIXED(16);
  if (column == 57) {
    GameFlag_Set(FLAG_LAST_ROW_STATUE_WEST);
    GameFlag_Clear(FLAG_LAST_ROW_STATUE_EAST);
    MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
  } else if (column == 59) {
    GameFlag_Set(FLAG_LAST_ROW_STATUE_EAST);
    GameFlag_Clear(FLAG_LAST_ROW_STATUE_WEST);
    MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
  } else {
    GameFlag_Clear(FLAG_LAST_ROW_STATUE_WEST);
    GameFlag_Clear(FLAG_LAST_ROW_STATUE_EAST);
    MapGrid_CopyCellAttributes(53, 11, 1, 1, 58, 7);
  }
  Event_End();
}

/// Move on the stone statue standing in the doorway at the head of the room of
/// vines: pushed one step west, to column 28, it stays there for good
/// (FLAG_PASSAGE_STATUE_MOVED), and the doorway, which leads up to the paths
/// outside, can be reached.
///
/// The doorway's foot, cell (29, 20), is a jump square on the strip of floor
/// along the north wall, with a gap at (30, 20) east of it. Cell (31, 20)
/// takes the same behaviour, CELL_BEHAVIOR_JUMP_FORWARD, so that Isaac,
/// walking west along the strip from the room's east side, leaps the gap
/// onto the doorway's foot.
static void MercuryLighthouse_MoveGapStatueOffDoorway(void) {
  Event_Start();
  if (Actor_Get(ACTOR_GAP_STATUE)->x.fixed / toFIXED(16) == 28) {
    GameFlag_Set(FLAG_PASSAGE_STATUE_MOVED);
    {
      MapGrid_CopyCellAttributes(29, 20, 1, 1, 31, 20);
    }
  }
  Event_End();
}

/// Rotates the selected palette range once every eight frames.
static inline void MercuryLighthouse_RotatePaletteRange(u16 *colors,
                                                        u32 colorCount) {
  u32 colorIndex;
  u16 *destination;
  u16 *source;
  u32 firstColor;

  if ((gFrameCount & 7) != 0) {
    return;
  }

  destination = colors;
  firstColor = *destination;
  colorIndex = 0;
  colors[colorCount - 1] = firstColor;

  source = colors + 1;
  while (colorIndex <= colorCount - 2) {
    *destination++ = *source++;
    colorIndex++;
  }
}

/// While the lighthouse door is sealed its light shimmers: background palette
/// 2, colors 8 to 15, rotates every eight frames.
static void MercuryLighthouse_CycleEntrancePalette(void) {
  MercuryLighthouse_RotatePaletteRange((u16 *)&BG_PLTT_COLOR(2, 8), 8);
}

/// Talking to Mia, kneeling before the lighthouse door, or Mind Read on her,
/// the first time: "Somebody...is here." She is sure it can only be Alex, the
/// one other than her who can enter, and turns to the door to find the stone
/// statue in her way: "But this statue is blocking my way..."
static void MercuryLighthouse_MiaSensesSomeoneInside(void) {
  Event_Start();
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
  Event_SetMessageId(MsgImilSomebodyHere);
  // "Somebody...is here."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_WalkToAndWait(ACTOR_MIA, 840, 648);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceDirection(ACTOR_MIA, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
  Actor_Get(ACTOR_MIA)->scaleX = -FIXED_ONE;
  Event_Wait(20);
  // "Someone is here..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_Get(ACTOR_MIA)->scaleX = FIXED_ONE;
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_SOUTH, 20);
  // "Can you hear it, too?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    // "Alex is the only one other than I who can enter Mercury..."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    gFieldState->message += 1;
  } else {
    gFieldState->message += 1;
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    // "You can't hear it? Well, I can. Alex is the only one other..."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  }
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(840, CAMERA_KEEP, 632, 1);
  Actor_WalkToAndWait(ACTOR_MIA, 840, 632);
  FieldCamera_WaitForArrival();
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 2);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "But this statue is blocking my way..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  GameFlag_Set(FLAG_MIA_SENSED_SOMEONE_INSIDE);
  Event_End();
}

/// Talking to Mia again while the statue still blocks the door: she shakes her
/// head. "This statue is blocking the entrance."
static void MercuryLighthouse_MiaDiscussesBlockedEntrance(void) {
  Event_Start();
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Event_SetMessageId(MSGNO(MsgImilStatueBlocksEntrance));
  // "This statue is blocking the entrance."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Event_End();
}

/// The door's light brightens to white as it opens.
static void MercuryLighthouse_FadePaletteToWhite(void) {
  volatile u16 *palette;
  u32 colorIndex;
  u32 whiteColors;
  s32 red;
  s32 green;
  s32 blue;

  do {
    palette = &BG_PLTT_COLOR(2, 8);
    whiteColors = 0;
    for (colorIndex = 0; colorIndex <= 7; colorIndex++) {
      red = RGB_RED(*palette);
      green = RGB_GREEN(*palette);
      blue = RGB_BLUE(*palette);
      if (red == RGB_LEVEL_MAX && green == RGB_LEVEL_MAX &&
          blue == RGB_LEVEL_MAX) {
        whiteColors++;
      } else {
        if (red < RGB_LEVEL_MAX)
          red++;
        if (green < RGB_LEVEL_MAX)
          green++;
        if (blue < RGB_LEVEL_MAX)
          blue++;
        *palette = RGB(red, green, blue);
      }
      palette++;
    }
    wait(2);
  } while (whiteColors <= 7);
}

/// Move on the stone statue in front of the lighthouse door while Mia waits
/// there: once it has slid aside, to column 51, Mia turns and asks "Did you do
/// that?" She saw the force that pushed it, Garet and Ivan tell her of
/// Psynergy, and she opens the door with Ply in a flash of light, then jumps up
/// and goes in alone.
///
/// Garet and Ivan appear beside the leader for the talk and walk back into him
/// at the end.
static void MercuryLighthouse_MiaOpensEntranceWithPly(void) {
  struct Actor *actor;
  s32 column;

  column = Actor_Get(ACTOR_DOOR_STATUE)->x.fixed;
  if (column < 0)
    column += toFIXED(16) - 1;
  column = FIELD_CELL(column);
  Event_Start();
  if (column == 51) {
    // Mia turns: "Did you do that?"
    Actor_SetSpeed(ACTOR_MIA, 0.8);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 10);
    Event_SetMessageId(MsgMercuryDidThat);
    // "Did you do that?"
    Event_OpenMessage(ACTOR_MIA, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      gFieldState->message += 1;
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
      // "I knew it... I saw a ghostly force pushing it!"
      Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    } else {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
      // "No, I saw it! I'm certain that flow of force came from you."
      Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
      gFieldState->message += 1;
    }
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // Garet and Ivan step out to explain Psynergy; Mia tells of Ply.
    SetEffectRecordMode(Actor_Get(ACTOR_GARET), 1);
    SetEffectRecordMode(Actor_Get(ACTOR_IVAN), 1);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_SetPosition(ACTOR_GARET, 872, 600);
    Actor_SetPosition(ACTOR_IVAN, 872, 600);
    Actor_WalkTo(ACTOR_IVAN, 888, 632);
    Actor_WalkToAndWait(ACTOR_GARET, 880, 616);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    // "You... You could see our Psynergy?"
    Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    // "Psynergy... Is that what you call that power?"
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 20);
    // "We had thought you were using Psynergy when you healed that old..."
    Event_ShowMessageThenWait(ACTOR_IVAN, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    // "That's a special power, handed down to members of the Mercury Clan..."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_WalkTo(ACTOR_MIA, 840, 656);
    Event_Wait(5);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTH_SOUTHWEST, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_WaitForMoveById(ACTOR_MIA);
    Event_Wait(10);
    // "It is called Ply, a power generated through meditation."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 20);
    // "It was once much stronger..."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_GARET, 1);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTH, 20);
    // "This Mercury Clan sounds a little like people of Vale, don't you think?"
    Event_OpenMessage(ACTOR_GARET, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
      // "Sure... They're the sworn protectors of a special power, just..."
      Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
      gFieldState->message += 1;
    } else {
      gFieldState->message += 1;
      Event_Wait(20);
      Actor_SetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
      // "Oh, come on! Both of them are protectors of special powers! It's..."
      Event_ShowMessageThenWait(ACTOR_GARET, 0, 20);
    }
    Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
    Event_Wait(30);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    Event_Wait(20);
    // "I have to go... I have to find whoever entered the lighthouse."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_WalkToAndWait(ACTOR_MIA, 840, 632);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
    Event_Wait(30);
    // Mia casts Ply on the door: a flash, the light turns white and the door
    // opens.
    MercuryLighthouse_MiaCastsPlyOnEntrance();
    Event_Wait(50);
    Sound_Play(SE_PSYNERGY_SPARKLE);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 20, 31),
                            PALETTES_ALL);
    PaletteFade_StartInterpolation(10);
    wait(1);
    Sound_Play(SE_SHIMMER);
    wait(40);
    PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    wait(60);
    Sound_Play(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
    MercuryLighthouse_FadePaletteToWhite();
    MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
    MapGrid_PlayMapEdit(gMercuryLighthouse_ColumnCells, 116, 35);
    Task_Remove(MercuryLighthouse_CycleEntrancePalette);
    Event_Wait(20);
    // She nods, jumps up and goes in.
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_MIA, 3);
    Actor_Jump(ACTOR_MIA, 4, 0);
    Actor_WalkTo(ACTOR_MIA, 840, 600);
    Actor_WaitForMoveById(ACTOR_MIA);
    MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
    Actor_Prepare(840, 56, 608, SPRITE_CRATE);
    Actor_SetSpeed(ACTOR_MIA, 0.8);
    Actor_WalkToAndWait(ACTOR_MIA, 840, 560);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(20);
    // Garet and Ivan rejoin the leader.
    Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_GARET, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
    actor = Actor_Get(ACTOR_PARTY_LEADER);
    if (actor != NULL) {
      Actor_SetDestination(ACTOR_IVAN, actor->x.part[0].integer,
                           actor->z.part[0].integer);
    }
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_SetPosition(ACTOR_GARET, 0, 0);
    Actor_WaitForMoveById(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA);
  }
  Event_End();
}

/// The rainbow over the waterfall shimmers: background palette 6, colors 1 to
/// 7, rotates every eight frames.
static void MercuryLighthouse_CycleWaterfallPalette(void) {
  MercuryLighthouse_RotatePaletteRange((u16 *)&BG_PLTT_COLOR(6, 1), 7);
}

/// The rainbow's colours fade to black, so it vanishes, as Ply takes effect.
static void MercuryLighthouse_FadePaletteToBlack(void) {
  // Palette RAM is hardware, read through a volatile pointer.
  volatile u16 *palette;
  u32 colorIndex;
  s32 blackColors;
  s32 red;
  s32 green;
  s32 blue;

  do {
    palette = &BG_PLTT_COLOR(6, 1);
    blackColors = 0;
    for (colorIndex = 0; colorIndex <= 6; colorIndex++) {
      red = RGB_RED(*palette);
      green = RGB_GREEN(*palette);
      blue = RGB_BLUE(*palette);
      if (red > 0)
        red--;
      if (green > 0)
        green--;
      if (blue > 0)
        blue--;
      *palette = RGB(red, green, blue);
      if (*palette == 0)
        blackColors++;
      palette++;
    }
    wait(5);
  } while (blackColors != 7);
}

/// Raises one of the party up the reversed waterfall in a shower of sparks,
/// faster and faster, for 96 frames.
static void MercuryLighthouse_RaiseActorWithSparks(s32 actorId) {
  struct EffectOptions options;
  struct Actor *actor;
  u32 frame;
  FIXED particleX;
  FIXED particleY;
  FIXED riseSpeed;

  actor = Actor_Get(actorId);
  actor->motionFlags = 0;
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetSpriteColorById(actorId, SPRITE_COLOR_BLINK);
  Sound_Play(SE_RISE_WITH_SPARKS);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_GLINT;
  options.script = gGlint_TwinkleScript;
  for (frame = 0; frame <= 47; frame++) {
    if (frame <= 31)
      MercuryLighthouse_SparkleNear(actor->x.fixed, actor->y.fixed,
                                    actor->z.fixed);
    if (frame & 1) {
      Sound_Play(SE_GLITTER);
      particleX = actor->x.fixed + INTtoFIXED(Random_Below(24)) - toFIXED(12);
      particleY = actor->y.fixed + INTtoFIXED(Random_Below(32)) - toFIXED(16);
      riseSpeed = ((Random_Below(4)) << 15) + toFIXED(1) / 2;
      FieldEffect_Spawn(particleX, particleY, actor->z.fixed, 0, riseSpeed, 0,
                        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        &options);
    }
    actor->y.fixed += frame * (toFIXED(1) / 10);
    actor->targetY = actor->y.fixed;
    wait(2);
  }
}

/// The sparks of each of the two bursts over the waterfall, and the spray
/// thrown up from each row of it as it parts.
#define WATERFALL_BURST_SPARKS 16
#define WATERFALL_ROW_SPRAY 32

/// Ply on the blue statue beside the waterfall under the rainbow: the rainbow
/// fades, sparks gather over the water, the floor rumbles and the waterfall
/// turns to stream upward. The first time Mia steps in and rises with it, and
/// Isaac, Garet and Ivan follow her one by one; the screen turns white and the
/// party is shown the lighthouse's beacon from outside before landing at the
/// Lighthouse Aerie. Once Saturos is beaten, Isaac rises alone.
///
/// The first time the exit is
/// MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, whose World Map
/// view ends at the aerie; later it is
/// MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE.
static void MercuryLighthouse_PlyOpensWaterfallPassage(void) {
  s32 rowOffset;
  u32 wave;
  u32 particleIndex;
  FIXED particleX;
  FIXED particleVelocityX;
  struct EffectOptions options;

  Event_Start();
  Event_Wait(20);
  MercuryLighthouse_FadePaletteToBlack();
  Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
  MapGrid_CopyCellsTo(45, 77, 45, 73, 9, 4);
  Event_Wait(30);
  options.priority = 1;
  options.palette = SPRITE_COLOR_BRG;
  options.type = SPRITE_GLINT;
  options.script = gGlint_TwinkleScript;
  // Two bursts spread sparks across the water before it begins to part.
  for (wave = 0; wave < WATERFALL_BURST_SPARKS; wave++) {
    u32 particleX, z;

    if ((1 & wave) != 0) {
      Sound_Play(SE_GLITTER);
    }
    particleX = INTtoFIXED(Random_Below(48)) + toFIXED(768);
    z = INTtoFIXED(Random_Below(56)) + toFIXED(136);
    FieldEffect_Spawn(particleX, 0, z, 0, 0, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
    Event_Wait(2);
  }
  Event_Wait(40);
  for (wave = 0; wave < WATERFALL_BURST_SPARKS; wave++) {
    u32 particleX, z;
    s32 particleSpeed;

    if ((1 & wave) != 0) {
      Sound_Play(SE_GLITTER);
    }
    particleX = INTtoFIXED(Random_Below(48)) + toFIXED(768);
    z = INTtoFIXED(Random_Below(56)) + toFIXED(152);
    particleSpeed = -(Random_Below(10) * (toFIXED(1) / 5)) - toFIXED(1) / 5;
    FieldEffect_Spawn(particleX, 0, z, 0, 0, particleSpeed,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      &options);
    Event_Wait(2);
  }
  Event_Wait(60);
  Sound_Play(SE_RUMBLE);
  MapGrid_SetShake(5, 5, 1);
  Event_Wait(60);
  options.palette = SPRITE_COLOR_WHITE;
  options.startScaleX = 7 * FIXED_ONE / 10;
  options.startScaleY = 7 * FIXED_ONE / 10;
  options.targetScaleX = 6 * FIXED_ONE / 5;
  options.targetScaleY = 6 * FIXED_ONE / 5;
  for (wave = 0; wave <= 3; wave++) {
    FIXED particleSpeed;

    MapGrid_CopyCellsTo(59, (12 - wave), 48, (12 - wave), 3, 1);
    particleIndex = 0;
    rowOffset = (wave << 4);
    do {
      particleX = INTtoFIXED(Random_Below(48)) + toFIXED(768);
      particleVelocityX =
          ((toFIXED(1) / 10) * (Random_Below(8))) - 4 * (toFIXED(1) / 10);
      particleSpeed = (toFIXED(1) / 10) * (Random_Below(8));
      FieldEffect_Spawn(particleX, 0,
                        INTtoFIXED(-(particleIndex >> 1) - rowOffset) +
                            toFIXED(192),
                        particleVelocityX, 0, particleSpeed,
                        EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
                            EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                        &options);
      particleIndex++;
      Event_Wait(2);
    } while (particleIndex < WATERFALL_ROW_SPRAY);
  }
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  MapGrid_WaitForShakeToSettle();
  Scene_PlayQueuedMusic();
  Event_Wait(30);
  // After Saturos the leader goes alone; the first time Mia leads the party.
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 824, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 792, 200);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_PARTY_LEADER);
    Event_Wait(60);
    Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
    PaletteFade_StartInterpolation(120);
    Event_Wait(120);
    PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_Wait(60);
    Import_EventSetExitRequest(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE);
    Event_End();
  } else {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.8);
    Actor_SetSpeed(ACTOR_GARET, 0.8);
    Actor_SetSpeed(ACTOR_IVAN, 0.8);
    Actor_SetSpeed(ACTOR_MIA, 0.8);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 824, 240);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
    Actor_SetPosition(ACTOR_MIA, 824, 240);
    Actor_WalkToAndWait(ACTOR_MIA, 792, 232);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_MIA, FACING_SOUTHEAST, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(40);
    Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 20);
    Actor_WalkToAndWait(ACTOR_MIA, 792, 200);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_MIA);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_SetPosition(ACTOR_GARET, 792, 232);
    Actor_SetPosition(ACTOR_IVAN, 792, 232);
    Actor_WalkTo(ACTOR_GARET, 816, 224);
    Actor_WalkToAndWait(ACTOR_IVAN, 768, 224);
    Actor_WaitForMoveById(ACTOR_GARET);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_FaceDirection(ACTOR_GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 20);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Event_Wait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0.5);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 224);
    Actor_FaceDirection(ACTOR_GARET, FACING_WEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_EAST, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
    Actor_FaceDirection(ACTOR_GARET, FACING_NORTHWEST, 0);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 20);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
    Actor_Jump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 792, 200);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    Actor_StartRepeatedMotion(ACTOR_GARET, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_PARTY_LEADER);
    Event_Wait(60);
    Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    Actor_TurnToFaceEachOther(ACTOR_GARET, ACTOR_IVAN, 20);
    Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
    Actor_SetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
    Event_Wait(40);
    Actor_WalkToAndWait(ACTOR_GARET, 792, 216);
    Actor_FaceDirection(ACTOR_IVAN, FACING_NORTHEAST, 0);
    Actor_WalkToAndWait(ACTOR_GARET, 792, 200);
    Event_Wait(30);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_GARET);
    Actor_WalkToAndWait(ACTOR_IVAN, 792, 216);
    Actor_WalkToAndWait(ACTOR_IVAN, 792, 200);
    Event_Wait(30);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_IVAN);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
    Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
    PaletteFade_ApplyTarget(PALETTE_TINT_PALE, PALETTES_ALL);
    PaletteFade_StartInterpolation(120);
    Event_Wait(120);
    PaletteFade_ApplyTarget(RGB_WHITE, PALETTES_ALL);
    PaletteFade_StartInterpolation(60);
    Event_Wait(60);
    Event_End();
    Import_EventSetExitRequest(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT);
  }
}

/// Searching the sign in front of the Fountain of Healing, at the top of the
/// causeway (cell (52, 50)), shows "Fountain of Healing" and "Water of Hermes:
/// It brings rejuvenation to all that lives."
static void MercuryLighthouse_ReadHermesFountainSign(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  // "Fountain of Healing" "Water of Hermes: It brings rejuvenation to all that
  // lives."
  FieldMessage_Show(MSGNO(MsgImilHermesHealingWaterFountain), 1);
  Event_End();
}

/// Searching the sealed arch of either gate room while it is shut, the one at
/// the top of the ladder on the dais or the middle one of three: "The door is
/// tightly locked."
static void MercuryLighthouse_InspectLockedDoor(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Event_End();
}

/// Searching the Fountain of Healing at the top of the causeway, cell
/// (52, 47), until its water has been taken this visit: "The fountain seems
/// dry." before Saturos is beaten, "The fountain is flowing with water." after.
///
/// With an Empty Bottle in the party it then asks for the field menu
/// (gFieldState->fieldMenu), so the bottle can be used there.
static void MercuryLighthouse_InspectHealingFountain(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) == 0)
    FieldMessage_Show(MSGNO(MsgMercuryLighthouseFountainSeemsDry), 1);
  else
    FieldMessage_Show(MSGNO(MsgMercuryLighthouseFountainFlowsWithWater), 1);
  if (PartyInventory_FindMember(ITEM_EMPTY_BOTTLE) != -1) {
    // The field sequence handles this menu request after the inspection ends.
    gFieldState->fieldMenu = 1;
  }
  Event_End();
}

/// Using the Empty Bottle at the Fountain of Healing once Saturos is beaten
/// fills it: the bottle becomes Hermes' Water, once a visit.
static void MercuryLighthouse_FillHermesWaterBottle(void) {
  Event_Start();
  Sound_Play(MUS_TREASURE_FOUND);
  ItemDisplay_ShowItem(ITEM_HERMES_WATER, 3);
  Inventory_ReplaceItem(ITEM_EMPTY_BOTTLE, ITEM_HERMES_WATER);
  MessageArgument_Push(PartyInventory_FindMember(ITEM_HERMES_WATER),
                       MESSAGE_ARGUMENT_ACTOR);
  MessageArgument_Push(ITEM_HERMES_WATER, MESSAGE_ARGUMENT_ITEM);
  FieldMessage_Show(MSGNO(MsgMercuryLighthouseIsaacGot), 1);
  GameFlag_Set(FLAG_HERMES_WATER_TAKEN);
  Event_End();
}

/// Walking south into the hole behind the blue crystal of the crystal room
/// (cell (29, 37)), Isaac drops through the floor and lands in the goddess's
/// pool on the ground floor
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE).
///
/// In the gate room the same cell, at the terrace's edge above another blue
/// crystal (cell (13, 19)), only jumps him down to the floor below.
static void MercuryLighthouse_JumpDownPastCrystal(void) {
  Event_Start();
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 472, 600);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
    FieldCamera_MoveTo(464, CAMERA_KEEP, 656, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
    Actor_PrepareFX(Actor_Get(ACTOR_PARTY_LEADER)->x.fixed, 0, toFIXED(702),
                    SPRITE_CRATE);
    MapGrid_CopyCellsTo(92, 46, 92, 40, 3, 2);
    Actor_Get(ACTOR_PARTY_LEADER)->gravity = toFIXED(1) / 2;
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 2);
    FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, -1);
    gFieldState->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    Event_Wait(60);
    Import_EventSetExitRequest(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE);
  } else {
    FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, -1);
  }
  Event_End();
}

#include "shared/mercury_lighthouse_arrive.c"

/// Starts the tall spout in the west of the big pipe room: the water rises from
/// its pointed pipe end a cell at a time, two cells, sparkles rising along its
/// edge.
static void MercuryLighthouse_StartSpoutsWithSparkles(void) {
  struct EffectOptions options;
  u32 particleIndex, row;

  MapGrid_CopyCellsTo(74, 58, 70, 34, 1, 1);
  options.palette = SPRITE_COLOR_WHITE;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        FIXED velocityX = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
        FIXED velocityZ = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

        FieldEffect_Spawn(
            toFIXED(105), 0,
            INTtoFIXED(-particleIndex - (row << 4)) + toFIXED(544), velocityX,
            0, velocityZ, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
            &options);
        Event_Wait(1);
      }
    }
    MapGrid_CopyCellsTo(74, 59, 70, 34 - row, 1, 1);
    MapGrid_CopyCellsTo(74, 58, 70, 33 - row, 1, 1);
  }
}

/// Stops the spouts as the spout pipe moves away (the push's callback): the
/// tall spout and the one shooting east draw back a cell at a time, each in a
/// scatter of sparkles.
static void MercuryLighthouse_StopSpoutsWithSparkles(void) {
  struct EffectOptions options;
  u32 row;
  u32 particleIndex;
  FIXED offsetZ;

  MapGrid_CopyCellsTo(76, 61, 74, 38, 1, 1);
  options.palette = SPRITE_COLOR_BRG;
  options.startScaleX = FIXED_ONE / 2;
  options.startScaleY = FIXED_ONE / 2;
  for (row = 0; row <= 2; row++) {
    offsetZ = -toFIXED(2);
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (particleIndex & 2) {
          FieldEffect_Spawn(
              INTtoFIXED(105 - Random_Below(5)), 0,
              offsetZ - row * toFIXED(8) + toFIXED(558), 0, 0, -toFIXED(1) / 4,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        } else {
          FieldEffect_Spawn(
              (row * 4 + particleIndex) * toFIXED(2) + toFIXED(183), 0,
              INTtoFIXED(620 - Random_Below(5)), toFIXED(1) / 4, 0, 0,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        }
        Event_Wait(1);
      }
      offsetZ -= toFIXED(2);
    }
    MapGrid_CopyCellsTo(71, 59, 70, 34 - row, 1, 1);
    MapGrid_CopyCellsTo(71, 59, row + 75, 38, 1, 1);
  }
}

/// Task: a sparkle from the leader every fourth frame, as a spout carries him
/// back.
static void MercuryLighthouse_SparkleAroundLeader(void) {
  struct EffectOptions options;
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  s32 framePhase = gFrameCount & 3;

  if (framePhase == 0) {
    options.palette = SPRITE_COLOR_WHITE;
    if (((Random_Below(2)) & 1) == 0)
      options.palette = SPRITE_COLOR_BRG;
    options.startScaleX = 7 * FIXED_ONE / 10;
    options.startScaleY = 7 * FIXED_ONE / 10;
    {
      FIXED y = leader->y.fixed + INTtoFIXED(Random_Below(4));
      FIXED velocityX = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
      FIXED velocityY = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

      FieldEffect_Spawn(leader->x.fixed, y, leader->z.fixed, velocityX,
                        velocityY, framePhase,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
    }
  }
}

/// A sparkle at a point every fourth frame, from a task.
static void MercuryLighthouse_SparkleAt(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  s32 framePhase = gFrameCount & 3;

  if (framePhase == 0) {
    options.palette = SPRITE_COLOR_WHITE;
    if (((Random_Below(2)) & 1) == 0)
      options.palette = SPRITE_COLOR_BRG;
    options.startScaleX = 7 * FIXED_ONE / 10;
    options.startScaleY = 7 * FIXED_ONE / 10;
    {
      FIXED velocityX = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;
      FIXED velocityY = (Random_Below(8)) * (toFIXED(1) / 5) - toFIXED(4) / 5;

      FieldEffect_Spawn(x, y, z, velocityX, velocityY, framePhase,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
    }
  }
}

/// One still sparkle near a point, up to 8 pixels either side and 4 north or
/// south.
static void MercuryLighthouse_SparkleNear(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  FIXED particleX;
  FIXED particleZ;

  options.palette = SPRITE_COLOR_WHITE;
  options.priority = 1;
  options.startScaleX = 7 * FIXED_ONE / 10;
  options.startScaleY = 7 * FIXED_ONE / 10;
  particleX = x + INTtoFIXED(Random_Below(16)) - toFIXED(8);
  particleZ = z + INTtoFIXED(Random_Below(8)) - toFIXED(4);

  FieldEffect_Spawn(particleX, y, particleZ, 0, 0, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_PALETTE,
                    &options);
}

/// The sparkles of a burst, one every four frames.
#define BURST_SPARKLES 6

/// BURST_SPARKLES sparkles, spraying the way angle points and slowing one by
/// one, as the water runs through a joined pipe.
static void MercuryLighthouse_SparkleBurst(s32 x, s32 y, s32 z, s32 angle) {
  struct EffectOptions options;
  s32 velocity[3];
  u32 particleIndex;

  Sound_Play(SE_BURST);
  for (particleIndex = 0; particleIndex < BURST_SPARKLES * 2; particleIndex++) {
    if ((particleIndex & 1) != 0) {
      options.palette = SPRITE_COLOR_WHITE;
      if ((particleIndex & 2) != 0)
        options.palette = SPRITE_COLOR_BRG;
      options.startScaleX = 3 * FIXED_ONE / 5;
      options.startScaleY = 3 * FIXED_ONE / 5;
      SET_VECTOR(velocity, 0, 0, 0);
      Vector_AddPolarOffset((BURST_SPARKLES - (particleIndex >> 1)) *
                                (toFIXED(1) / 10),
                            angle, velocity);
      {
        FIXED particleX = x + INTtoFIXED(6 - Random_Below(6));
        FIXED particleZ = z + INTtoFIXED(6 - Random_Below(6));

        FieldEffect_Spawn(
            particleX, y, particleZ, velocity[0], velocity[1], velocity[2],
            EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
      }
    }
    wait(2);
  }
}

/// Restores each lighthouse scene's puzzle state and plays its
/// entrance's event.
s32 Scene_Initialize(void) {
  u32 statueIndex;
  s32 positionFlag;
  FIXED statueX;
  s32 keepPartyPosition;
  s32 alpha;

  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  gMainRoutineState.retreatEntrance =
      MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gMainRoutineState.retreatScene = SCENE_MERCURY_LIGHTHOUSE_EXTERIOR;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Io_Write16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
  alpha = BLEND_ALPHA(16, 16);
  REG_BLDALPHA = alpha;
  MercuryLighthouse_RestoreLights(
      CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER,
      (struct MercuryLighthouseLights *)gSceneLocalState);
  FieldEffect_StartFadeOverlay(0);
  if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    Task_AddFrame(MercuryLighthouse_CyclePalette, TASK_PASS_FRAME);
  else
    MercuryLighthouse_ClearPalette();
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_LOWER) {
    // The great pool room, the pipe room and Mia's ledge.
    switch (gMainRoutineState.entrance) {
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST:
    case MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE:
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
        MapGrid_CopyCellAttributes(66, 5, 27, 23, 2, 5);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE:
      SetEffectRecordMode(Actor_Get(ACTOR_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST:
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN)) {
        wait(1);
        Actor_SetPosition(ACTOR_FIRST_PIPE, 672, 128);
        Actor_SetPosition(ACTOR_SECOND_PIPE, 704, 192);
        Actor_SetPosition(ACTOR_THIRD_PIPE, 648, 240);
      } else if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        GameFlag_Clear(FLAG_PIPE_1_JOINED);
        GameFlag_Clear(FLAG_PIPE_2_JOINED);
        GameFlag_Clear(FLAG_PIPE_3_JOINED);
        GameFlag_Clear(FLAG_UNUSED_PIPE_4_JOINED);
      }
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_PIPE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_SECOND_PIPE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_THIRD_PIPE), 1);
      Scene_RedrawActorRectangle(ACTOR_FIRST_PIPE);
      Scene_RedrawActorRectangle(ACTOR_SECOND_PIPE);
      Scene_RedrawActorRectangle(ACTOR_THIRD_PIPE);
      SetEffectRecordMode(Actor_Get(ACTOR_PIPE_END), 1);
      // The cells MercuryLighthouse_PushChannelPipes copies as it sets the
      // flag, redrawn on arrival.
      if (GameFlag_Get(FLAG_PIPE_1_JOINED)) {
        MapGrid_CopyCellsTo(119, 9, 109, 11, 1, 1);
        // The cells MercuryLighthouse_PushChannelPipes copies as it sets the
        // flag, redrawn on arrival.
        if (GameFlag_Get(FLAG_PIPE_2_JOINED))
          MapGrid_CopyCellsTo(118, 9, 104, 13, 1, 1);
        MapGrid_CenterOnFollowedPoint();
        wait(1);
      }
      break;
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL:
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS:
      if (!GameFlag_Get(FLAG_MIA_JOINED_PARTY)) {
        Actor_SetPosition(ACTOR_MIA, 696, 632);
        Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
      } else {
        Actor_SetPosition(ACTOR_STATUE_BLOCKING_MIA, 776, 632);
        MapGrid_CopyCellAttributes(110, 39, 5, 1, 46, 39);
      }
      break;
    }
  }
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_MIDDLE) {
    switch (gMainRoutineState.entrance) {
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH:
    case MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE:
      SetEffectRecordMode(Actor_Get(ACTOR_SPOUT_PIPE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_SPOUT_PIPE_END), 1);
      // With the spout pipe pushed into its pool, joining the pipe above to
      // the one below, water runs on through the room's pipes and spouts from
      // their open ends. The pipe is put back in the pool, and the cells
      // copied here are that state's.
      if (GameFlag_Get(FLAG_SPOUTS_RUNNING)) {
        wait(1);
        Sound_Play(SE_MERCURY_LIGHTHOUSE_MECHANISM);
        Actor_SetPosition(ACTOR_SPOUT_PIPE, 184, 528);
        MapGrid_CopyCellAttributes(11, 31, 1, 4, 9, 31);
        MapGrid_CopyCellAttributes(7, 30, 1, 4, 11, 31);
        MapGrid_CopyCellsTo(74, 58, 70, 32, 1, 2);
        MapGrid_CopyCellsTo(74, 59, 70, 34, 1, 1);
        MapGrid_CopyCellsTo(76, 60, 74, 38, 3, 1);
        MapGrid_CopyCellsTo(77, 60, 76, 38, 2, 1);
        MapGrid_CopyCellsTo(75, 58, 86, 41, 1, 3);
        MapGrid_CopyCellsTo(75, 59, 86, 43, 1, 2);
        MapGrid_CopyCellsTo(76, 59, 80, 49, 2, 1);
        MapGrid_CopyCellsTo(77, 59, 82, 49, 2, 1);
      }
      break;
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH:
    case MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES:
      MapGrid_CenterOnFollowedPoint();
      wait(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          GameFlag_Get(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
        MapGrid_CopyCellsTo(9, 1, 9, 7, 1, 2);
      }
      // The statue stands on the floor switch west of the dais, which holds
      // the gate open: the sealed arch north of the dais is drawn as an open
      // doorway.
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN)) {
        Actor_SetPosition(ACTOR_GATE_STATUE, 88, 216);
        Actor_Get(ACTOR_GATE_STATUE)->y.fixed -= toFIXED(2);
        Actor_Get(ACTOR_GATE_STATUE)->targetY =
            Actor_Get(ACTOR_GATE_STATUE)->y.fixed;
        MapGrid_CopyCellsTo(9, 1, 9, 7, 1, 2);
        MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
        MapGrid_CopyCellAttributes(9, 5, 1, 1, 9, 10);
      }
#if TBS_PATCH_3
      Task_AddFrame(MercuryLighthouse_UpdateLeaderEffectTarget,
                    TASK_PASS_FRAME);
#endif
      break;
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST:
      SetEffectRecordMode(Actor_Get(ACTOR_FOUNTAIN_PIPE), 1);
      // The fountain pipe joined across the head of its pool, with a jet of
      // water crossing it.
      if (GameFlag_Get(FLAG_FOUNTAIN_RUNNING)) {
        MercuryLighthouse_OpenFountain(0);
        MapGrid_CopyCellAttributes(42, 41, 4, 1, 42, 39);
        MapGrid_CopyCellAttributes(42, 40, 4, 1, 42, 41);
        Actor_SetPosition(ACTOR_FOUNTAIN_PIPE, 704, 640);
      }
      break;
    }
  }
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_UPPER) {
    switch (gMainRoutineState.entrance) {
    case MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST:
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_ROW_STATUE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_ROW_STATUE + 1), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_ROW_STATUE + 2), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_ROW_STATUE + 3), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_LAST_ROW_STATUE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_PARTY_LEADER), 1);
      statueIndex = 0;
      statueX = toFIXED(632);
      positionFlag = FLAG_FIRST_ROW_STATUE;
      for (; statueIndex <= 3; statueIndex++) {
        if (GameFlag_Get(positionFlag))
          Actor_SetPositionFX(statueIndex + ACTOR_FIRST_ROW_STATUE, statueX,
                              toFIXED(88));
        else if (GameFlag_Get(positionFlag + 1))
          Actor_SetPositionFX(statueIndex + ACTOR_FIRST_ROW_STATUE,
                              statueX + toFIXED(32), toFIXED(88));
        statueX += toFIXED(64);
        positionFlag += 2;
      }
      // The cells MercuryLighthouse_RecordRowStatues copies as it sets the
      // flag, redrawn on arrival.
      if (GameFlag_Get(FLAG_LAST_ROW_STATUE_WEST)) {
        Actor_SetPosition(ACTOR_LAST_ROW_STATUE, 920, 88);
        MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      } else if (GameFlag_Get(FLAG_LAST_ROW_STATUE_EAST)) {
        Actor_SetPosition(ACTOR_LAST_ROW_STATUE, 952, 88);
        MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST:
    case MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS:
      // The gap statue one column west of the door it hid, and the jump spot
      // that reaches the door (MercuryLighthouse_MoveGapStatueOffDoorway).
      if (GameFlag_Get(FLAG_PASSAGE_STATUE_MOVED)) {
        Actor_SetPosition(ACTOR_GAP_STATUE, 456, 328);
        MapGrid_CopyCellAttributes(29, 20, 1, 1, 31, 20);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST:
    case MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP:
      MercuryLighthouse_ArriveWithSparks();
      // Fall through.
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST:
      Scene_QueueMusic(SE_AMBIENT_WATER);
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER:
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES:
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          GameFlag_Get(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        Actor_Get(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(2);
        Actor_Get(ACTOR_PARTY_LEADER)->targetY =
            Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
        MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
        MapGrid_CopyCellsTo(10, 28, 10, 18, 1, 2);
      }
      // The upper floors' gate held open the same way: the sealed middle arch
      // is drawn as an open doorway.
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN)) {
        Actor_SetPosition(ACTOR_FALLEN_STATUE, 168, 376);
        Actor_Get(ACTOR_FALLEN_STATUE)->y.fixed -= toFIXED(2);
        Actor_Get(ACTOR_FALLEN_STATUE)->targetY =
            Actor_Get(ACTOR_FALLEN_STATUE)->y.fixed;
        MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
        MapGrid_CopyCellsTo(10, 28, 10, 18, 1, 2);
        MapGrid_CopyCellAttributes(10, 16, 1, 1, 10, 19);
        MapGrid_CenterOnFollowedPoint();
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS:
      wait(1);
#if TBS_PATCH_2
      if (GameFlag_Get(FLAG_WEST_EAST_PIPE_PUSHED))
        Actor_SetPosition(ACTOR_WEST_EAST_PIPE, 816, 608);
      if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET))
        Actor_SetPosition(ACTOR_JET_SWITCH_PIPE, 776, 576);
      else if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET))
        Actor_SetPosition(ACTOR_JET_SWITCH_PIPE, 760, 576);
      else if (GameFlag_Get(FLAG_SWITCH_PIPE_STOPS_JETS))
        Actor_SetPosition(ACTOR_JET_SWITCH_PIPE, 856, 576);
#else
      Actor_SetPosition(ACTOR_WEST_EAST_PIPE, 816, 608);
      Actor_SetPosition(ACTOR_JET_SWITCH_PIPE, 776, 576);
#endif
      SetEffectRecordMode(Actor_Get(ACTOR_PARTY_LEADER), 1);
      MercuryLighthouse_StopJet(JET_SOUTHWARD);
#if TBS_PATCH_2
      if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
        MercuryLighthouse_StartJet(JET_WESTWARD);
        if (FIELD_CELL(Actor_Get(ACTOR_SLIDING_STATUE)->z.fixed) ==
            SLIDING_STATUE_UNDER_JET_ROW)
          Task_AddFrame(MercuryLighthouse_SparkleAboveStatue, TASK_PASS_FRAME);
      } else if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
        MercuryLighthouse_StartJet(JET_EASTWARD);
      } else if (!GameFlag_Get(FLAG_SWITCH_PIPE_STOPS_JETS)) {
        MercuryLighthouse_StartJet(JET_SOUTHWARD);
      }
#else
      MercuryLighthouse_StartJet(JET_EASTWARD);
#endif
      // Fall through.
    case MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE:
      if (gMainRoutineState.entrance == MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE)
        Sound_Play(SE_MERCURY_LIGHTHOUSE_MECHANISM);
      SetEffectRecordMode(Actor_Get(ACTOR_SLIDING_STATUE), 1);
      Actor_SetSpritePriority(ACTOR_WEST_EAST_PIPE, 2);
      Actor_Get(ACTOR_WEST_EAST_PIPE)->layer = 2;
      SetEffectRecordMode(Actor_Get(ACTOR_JET_SWITCH_PIPE), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_JET_ROOM_PIPE_END), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_JET_ROOM_PIPE_END + 1), 1);
      SetEffectRecordMode(Actor_Get(ACTOR_FIRST_JET_ROOM_PIPE_END + 2), 1);
      Scene_RedrawActorRectangle(ACTOR_WEST_EAST_PIPE);
      Scene_RedrawActorRectangle(ACTOR_JET_SWITCH_PIPE);
      if (gMainRoutineState.entrance != MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE)
        break;
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        GameFlag_Clear(FLAG_WEST_EAST_PIPE_PUSHED);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET);
        GameFlag_Clear(FLAG_SWITCH_PIPE_STOPS_JETS);
        break;
      }
      MercuryLighthouse_StopJet(JET_SOUTHWARD);
      if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_WESTWARD_JET)) {
        MercuryLighthouse_StartJet(JET_WESTWARD);
        if (FIELD_CELL(Actor_Get(ACTOR_SLIDING_STATUE)->z.fixed) ==
            SLIDING_STATUE_UNDER_JET_ROW)
          Task_AddFrame(MercuryLighthouse_SparkleAboveStatue, TASK_PASS_FRAME);
      } else if (GameFlag_Get(FLAG_SWITCH_PIPE_STARTS_EASTWARD_JET)) {
        MercuryLighthouse_StartJet(JET_EASTWARD);
      } else if (!GameFlag_Get(FLAG_SWITCH_PIPE_STOPS_JETS)) {
        MercuryLighthouse_StartJet(JET_SOUTHWARD);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS:
      Event_Start();
      SetEffectRecordMode(Actor_Get(ACTOR_FALLEN_STATUE), 1);
      Actor_SetSpriteColorById(ACTOR_PARTY_LEADER, SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
      Actor_Get(ACTOR_FALLEN_STATUE)->y.fixed = toFIXED(144);
      Actor_Get(ACTOR_FALLEN_STATUE)->motionFlags = 0;
      Actor_Get(ACTOR_FALLEN_STATUE)->bounce = 0;
      Actor_Get(ACTOR_FALLEN_STATUE)->gravity = toFIXED(3) / 10;
      gFieldState->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_Get(ACTOR_FALLEN_STATUE)->motionFlags =
          ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
      Sound_Play(SE_STONE_GRIND);
      Event_Wait(32);
      Sound_Play(SE_THUMP);
      SetEffectRecordMode(Actor_Get(ACTOR_FALLEN_STATUE), 2);
      MapGrid_SetShake(3, 3, 1);
      MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
      MapGrid_WaitForShakeToSettle();
      Event_Wait(60);
      Import_EventSetExitRequest(
          MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS_STATUE_FALLS);
      Event_End();
      break;
    }
  }
  if (gMainRoutineState.next == SCENE_MERCURY_LIGHTHOUSE_EXTERIOR) {
    // This scene contains Mia's entrance, the lift and the healing fountain.
    switch (gMainRoutineState.entrance) {
    case MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH:
      Task_AddFrame(MercuryLighthouse_CycleWaterfallPalette, TASK_PASS_FRAME);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        break;
      MercuryLighthouse_ArriveWithSparks();
      Scene_QueueMusic(SE_AMBIENT_WATER);
      PaletteFade_ApplySource(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_ApplyTarget(PALETTE_TINT_WARM, PALETTES_BG);
      PaletteFade_StartInterpolation(30);
      Event_WaitTransitionFrames();
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_Wait(30);
      FieldMessage_ShowCentered(MsgMercuryHonorsGoddessRainbows, 0, 0);
      PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
      PaletteFade_StartInterpolation(30);
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE:
      Actor_SetPosition(ACTOR_MIA, 0, 0);
      BlendScript_Suspend();
      Io_Write16(0, &REG_BLDCNT);
      Actor_Get(ACTOR_EAST_APPROACH_STATUE)->scaleX = -FIXED_ONE;
      Actor_SetSpritePriority(ACTOR_FIRST_VISITOR, 1);
      Actor_SetSpritePriority(ACTOR_FIRST_VISITOR + 1, 1);
      Actor_SetSpritePriority(ACTOR_FIRST_VISITOR + 2, 1);
      keepPartyPosition = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
      // The lift's place with the lift gone back up, the two copies
      // MercuryLighthouse_RideLift ends with; the arrivals from the entrance
      // and the World Map below draw the same.
      if (keepPartyPosition) {
        MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
        MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
        break;
      }
      Actor_SetPosition(ACTOR_DOOR_STATUE, 824, 600);
      MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
      Actor_Get(ACTOR_DOOR_STATUE)->motionFlags = keepPartyPosition;
      SetEffectRecordMode(Actor_Get(ACTOR_DOOR_STATUE), 1);
      Event_Start();
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      FieldCamera_GetActor()->motionFlags = keepPartyPosition;
      Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 1);
      Actor_SetSpritePriority(ACTOR_LIFT_STATUE, 1);
      MercuryLighthouse_RideLift();
      BlendScript_Resume();
      Event_End();
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE:
      Event_Wait(1);
      SetEffectRecordMode(Actor_Get(ACTOR_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP:
      MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
      MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
      if (!GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        MapGrid_CopyCellsTo(30, 45, 50, 45, 5, 6);
        MapGrid_CopyCellsTo(50, 105, 50, 109, 5, 3);
        MapGrid_CenterOnFollowedPoint();
        wait(1);
      } else {
        Actor_SetSpritePriority(ACTOR_FIRST_VISITOR, 1);
        Actor_SetSpritePriority(ACTOR_FIRST_VISITOR + 1, 1);
        Actor_SetSpritePriority(ACTOR_FIRST_VISITOR + 2, 1);
      }
      Actor_Get(ACTOR_EAST_APPROACH_STATUE)->scaleX = -FIXED_ONE;
      // Until Mia has hurried here from Imil (FLAG_IMIL_MET_MIA, set in
      // imil.c), she is not here and the door stays shut.
      if (!GameFlag_Get(FLAG_IMIL_MET_MIA)) {
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        Actor_SetPosition(ACTOR_DOOR_STATUE, 824, 600);
        Actor_SetSpritePriority(ACTOR_DOOR_STATUE, 1);
        MapGrid_CopyCellAttributes(116, 37, 3, 3, 52, 37);
        MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
        Task_AddFrame(MercuryLighthouse_CycleEntrancePalette, TASK_PASS_FRAME);
        break;
      }
      if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA)) {
        if (!GameFlag_Get(FLAG_MIA_SENSED_SOMEONE_INSIDE)) {
          Actor_FaceDirection(ACTOR_MIA, FACING_EAST, 0);
          Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
        } else {
          Actor_SetPosition(ACTOR_MIA, 840, 632);
          Actor_FaceDirection(ACTOR_MIA, FACING_NORTH, 0);
        }
        SetEffectRecordMode(Actor_Get(ACTOR_MIA), 1);
        MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
        Task_AddFrame(MercuryLighthouse_CycleEntrancePalette, TASK_PASS_FRAME);
      } else {
        Actor_SetPosition(ACTOR_MIA, 0, 0);
        Actor_SetPosition(ACTOR_DOOR_STATUE, 824, 600);
        MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
        Actor_Get(ACTOR_DOOR_STATUE)->collisionFlags = (u8)~ACTOR_SOLID;
        Event_Wait(1);
      }
      Actor_Get(ACTOR_DOOR_STATUE)->motionFlags = 0;
      SetEffectRecordMode(Actor_Get(ACTOR_DOOR_STATUE), 1);
      break;
    }
  }
  return 0;
}

/// Move on the blue statue in Mia's way, on the ledge up the stairs from the
/// great pool room: once it has dropped into the gap at her feet, at column 48,
/// Mia thanks the party ("You've saved me again, haven't you!"), asks their
/// business in the lighthouse until they own to it, and joins them, bringing
/// Fizz, a Mercury Djinni, already set.
///
/// The gap's row (46, 39) to (50, 39) takes the walkable cells of (110, 39) to
/// (114, 39).
static void MercuryLighthouse_MiaJoinsAfterRescue(void) {
  s32 firstMessage;
  s32 questionMessage;

  if (Actor_Get(ACTOR_STATUE_BLOCKING_MIA)->x.fixed / toFIXED(16) != 48) {
    return;
  }
  Event_Start();
  firstMessage = MsgMercurySavedMeAgain;
  Event_SetMessageId(firstMessage);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MIA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  // "You've saved me again, haven't you!"
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Event_Wait(60);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
  Event_Wait(50);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  // "Do you have business in the lighthouse, too?"
  Event_OpenMessage(ACTOR_MIA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
    // Each "no" brings "Please, don't hide anything." until the party says
    // yes.
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Event_Wait(20);
    // "Please tell me the truth!"
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    // "I can't believe you'd go so far to help someone you just met."
    Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
    Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
    Event_Wait(20);
    // "You have some errand in this lighthouse, don't you?"
    Event_OpenMessage(ACTOR_MIA, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
      Event_Wait(20);
      MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(ACTOR_MIA, ANIM_SHAKE_HEAD);
      Event_Wait(20);
      questionMessage = firstMessage + 5;
      for (;;) {
        Event_SetMessageId(questionMessage);
        Event_OpenMessage(ACTOR_MIA, 0);
        if (Event_AskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 1) {
          break;
        }
        Event_Wait(20);
        MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(ACTOR_MIA, ANIM_SHAKE_HEAD);
        Event_Wait(20);
        questionMessage = MsgMercuryDontHideTruth;
      }
    }
  }
  Event_SetMessageId(MsgMercuryThoughtSo);
  Actor_SetSpeed(ACTOR_MIA, 0.8);
  Actor_WalkToAndWait(ACTOR_MIA, 728, 632);
  Event_Wait(20);
  // "I thought so..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_MIA_KNEEL);
  // "I am sure there will be more places ahead that I cannot pass..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_SetAnimation(ACTOR_MIA, ANIM_STAND);
  Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 20);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  // "Only those of the Mercury Clan are permitted to enter this..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  Event_Wait(20);
  // "However, I sense something special about you. I feel that I..."
  Event_ShowMessageThenWait(ACTOR_MIA, 0, 20);
  Event_AddPartyMember(MIA, 1);
  // "The Adept Mia joined your party!" Fizz, a Mercury Djinni, comes with
  // her and is set; her stats are worked out again.
  GameFlag_Set(FLAG_DJINNI(DJINNI_FIZZ));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Unit_Calculate(MIA);
  Scene_WalkIntoLeader(ACTOR_MIA);
  MapGrid_CopyCellAttributes(110, 39, 5, 1, 46, 39);
  GameFlag_Set(FLAG_MIA_JOINED_PARTY);
  Event_End();
}

/// Coming down from the Lighthouse Aerie
/// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE), Isaac rides the
/// lift down beside its blue statue, slowing as it nears the bottom, hops off,
/// and the lift goes back up without him.
///
/// The lift is drawn on the overhead layer, scrolled with both riders over 137
/// pixels.
static void MercuryLighthouse_RideLift(void) {
  struct MapGridLayerScroll *liftLayer;
  FIXED liftSpeed;

  liftLayer = &((struct MapGridScrollState *)gMapGridState[0])
                   ->layers[MAPGRID_LAYER_OVERHEAD];
  liftSpeed = toFIXED(0.61);
  liftLayer->offsetY = toFIXED(1161);
  liftLayer->speedY = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->z.fixed -= toFIXED(137);
  Actor_Get(ACTOR_PARTY_LEADER)->targetZ =
      Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_LIFT_STATUE, 680, 440);
  Actor_Get(ACTOR_LIFT_STATUE)->z.fixed -= toFIXED(137);
  Actor_Get(ACTOR_LIFT_STATUE)->targetZ = Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
  MapGrid_CenterOnFollowedPoint();
  wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(60);
  BlendScript_Suspend();
  Sound_Play(SE_LIFT);
  // The lift layer scrolls down with both riders, 137 pixels, slowing as it
  // nears the landing.
  for (;;) {
    liftLayer->offsetY -= liftSpeed;
    Actor_Get(ACTOR_PARTY_LEADER)->z.fixed += liftSpeed;
    Actor_Get(ACTOR_PARTY_LEADER)->targetZ =
        Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;
    Actor_Get(ACTOR_LIFT_STATUE)->z.fixed += liftSpeed;
    Actor_Get(ACTOR_LIFT_STATUE)->targetZ =
        Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
    if (liftLayer->offsetY > toFIXED(1024)) {
      if ((gFrameCount & 15) == 0 && liftSpeed >= toFIXED(0.05))
        liftSpeed -= toFIXED(0.021);
      wait(1);
      continue;
    }
    break;
  }
  liftLayer->offsetY = toFIXED(1024);
  MapGrid_CenterOnFollowedPoint();
  wait(2);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags =
      ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(ACTOR_LIFT_STATUE)->z.fixed = toFIXED(440);
  Actor_Get(ACTOR_LIFT_STATUE)->targetZ = Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
  Event_Wait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 704, 584);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
  Actor_Get(ACTOR_PARTY_LEADER)->speedLimit = toFIXED(3);
  Actor_Get(ACTOR_PARTY_LEADER)->acceleration = toFIXED(2);
  Sound_Play(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = toFIXED(4);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 736, 584);
  Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = FIXED_ONE / 4;
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
  Event_Wait(6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Sound_Play(SE_LIFT);
  // The lift and its statue go back up without the leader.
  for (;;) {
    liftLayer->offsetY += liftSpeed;
    Actor_Get(ACTOR_LIFT_STATUE)->z.fixed -= liftSpeed;
    Actor_Get(ACTOR_LIFT_STATUE)->targetZ =
        Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
    if (liftLayer->offsetY >= toFIXED(1161))
      break;
    if ((gFrameCount & 7) == 0 && liftSpeed <= toFIXED(4) / 5)
      liftSpeed += toFIXED(1) / 10;
    wait(1);
  }
  liftLayer->offsetY = toFIXED(1024);
  MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
  MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
  MapGrid_CenterOnFollowedPoint();
  wait(2);
  Actor_SetPosition(ACTOR_LIFT_STATUE, 0, 0);
  Event_Wait(30);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
}

/// Mia casts Ply on the Ply target, moved onto the door.
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void) {
  struct FieldEffectState *effectState;
  u8 flags;

  effectState = gFieldEffectState;
  Actor_SetPosition(ACTOR_PLY_TARGET, 840, 600);
  FieldPsynergy_LoadResources(ABILITY_PLY, 1);
  FieldPsynergy_SetupActorPair(ACTOR_MIA, ACTOR_PLY_TARGET);
  flags = (u8)effectState->messageMode | FIELD_MESSAGE_LOWER;
  effectState->messageMode = flags;
  FieldPsynergy_StartCast();
  FieldPsynergy_RunHandler(1);
  FieldPsynergy_StopCasterUpdate();
}

#include "shared/hermes_water_bottle.c"

#include "shared/mercury_lighthouse_lights.c"

/// Spreads one blue ripple (SPRITE_RIPPLE) from an actor, for 30
/// frames.
static void MercuryLighthouse_SpawnRipple(struct Actor *source) {
  struct Actor *effect;
  struct Sprite *sprite;

  effect = FieldActor_Create(SPRITE_RIPPLE, source->x.fixed, source->y.fixed,
                             source->z.fixed);
  if (effect == NULL) {
    return;
  }

  sprite = effect->sprite;
  Actor_StartScript(effect, gMercuryLighthouse_RippleScript);

  effect->motionFlags = 0;
  effect->layer = 1;
  effect->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;

  if (sprite != NULL) {
    Sprite_StartAnimation(sprite, 2);
    sprite->flags = 0;
    sprite->image.priority = 3;
  }
}

#include "shared/mercury_lighthouse_pillar.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/pushed_actor_tables.c"
// clang-format off

s32 gMercuryLighthouse_RippleScript[] = {
    SC_WAIT(30),
    SC_RELEASE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(MercuryLighthouse_AdvanceAnchoredRiseFrame),
    SC_RELEASE,
};

s32 gMercuryLighthouse_GrowLightScript[] = {
    SC_CALL(MercuryLighthouse_GrowLight),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(Actor_AdvanceScaleCounter),
    SC_RELEASE,
};

#include "shared/mercury_lighthouse_pillar_tables.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "shared/glint_twinkle_script.c"

/// The blue statue's slides in the room of jets, each three quarters of a
/// second or a second after the jet starts: JET_WESTWARD pushes it west under
/// the falling jet's pipe end, JET_SOUTHWARD south to the row of the hole, and
/// JET_EASTWARD east into the hole.
s32 gMercuryLighthouse_StatueSlideWest[] = {
    SC_WAIT(45),
    SC_SPEED(0.8),
    SC_ACCELERATION(0.2),
    SC_CALL(MercuryLighthouse_StartStatueSparkles),
    SC_MOVE(792, 0, 712),
    SC_SYNC,
    SC_END,
};

s32 gMercuryLighthouse_StatueSlideSouth[] = {
    SC_WAIT(60),
    SC_SPEED(0.8),
    SC_ACCELERATION(0.2),
    SC_CALL(MercuryLighthouse_StartStatueSparkles),
    SC_MOVE(792, 0, 744),
    SC_SYNC,
    SC_CALL(MercuryLighthouse_StopStatueSparkles),
    SC_END,
};

s32 gMercuryLighthouse_StatueSlideIntoHole[] = {
    SC_WAIT(60),
    SC_SPEED(0.8),
    SC_ACCELERATION(0.2),
    SC_CALL(MercuryLighthouse_StartStatueSparkles),
    SC_MOVE(824, 0, 744),
    SC_SYNC,
    SC_CALL(MercuryLighthouse_StopStatueSparkles),
    SC_END,
};

/// JET_WESTWARD again once the statue is under the falling jet's pipe end: the
/// sparkles start, but the statue stays where it is.
s32 gMercuryLighthouse_StatueStaysPut[] = {
    SC_WAIT(45),
    SC_CALL(MercuryLighthouse_StartStatueSparkles),
    SC_END,
};

/// Unused, with gPaletteCycleGreen and gPaletteCycleBlue: nothing reads them or
/// changes them.
s32 gPaletteCycleRed = 0;
s32 gPaletteCycleGreen = 0;
s32 gPaletteCycleBlue = 0;

/// The gate room's sealed arch on the dais opening a column at a time, and
/// closing; then the upper gate room's middle arch, the same.
u16 gMercuryLighthouse_GateCells[] = {
    MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
u16 gMercuryLighthouse_GateCloseCells[] = {
    MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
u16 gMercuryLighthouse_UpperGateCells[] = {
    MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};
u16 gMercuryLighthouse_UpperGateCloseCells[] = {
    MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

/// The lighthouse door opening in five steps as Mia casts Ply on it.
u16 gMercuryLighthouse_ColumnCells[] = {
    MAPGRID_ANIMATION_STEP(126, 35, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 38, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 41, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 44, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 47, 1, 2, 2), MAPGRID_ANIMATION_STEPS_END,
};

struct SceneEntrance gMercuryLighthouseLowerEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, 56, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, CONDITION_ALWAYS, 248, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, 16, 16, 496, 448, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, CONDITION_ALWAYS, 760, 0, 344, FACING_NORTH, 0, 544, 16, 816, 352, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE, CONDITION_ALWAYS, 120, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, CONDITION_ALWAYS, 344, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_SOUTHWEST, CONDITION_ALWAYS, 120, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER, CONDITION_ALWAYS, 280, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL, CONDITION_ALWAYS, 776, 0, 472, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS, CONDITION_ALWAYS, 856, 0, 520, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, CONDITION_ALWAYS, 472, 0, 696, FACING_NORTH, 0, 352, 512, 592, 720, 0 },
    { MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 544, 16, 816, 352, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseMiddleEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH, CONDITION_ALWAYS, 120, 0, 472, FACING_SOUTH, 0, 32, 400, 480, 848, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE, CONDITION_ALWAYS, 152, 0, 824, FACING_NORTH, 0, 32, 400, 480, 848, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH, CONDITION_ALWAYS, 152, 0, 184, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES, CONDITION_ALWAYS, 488, 0, 136, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, 728, 0, 216, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, CONDITION_ALWAYS, 632, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, CONDITION_ALWAYS, 792, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, CONDITION_ALWAYS, 712, 0, 568, FACING_SOUTH, 0, 576, 480, 832, 736, 0 },
    { MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, CONDITION_ALWAYS, 888, 0, 440, FACING_NORTH, 0, 760, 304, 1040, 464, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseUpperEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, CONDITION_ALWAYS, 88, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, CONDITION_ALWAYS, 504, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, CONDITION_ALWAYS, 264, 0, 120, FACING_SOUTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseExteriorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP, CONDITION_ALWAYS, 840, 0, 856, FACING_NORTH, 0, 576, 328, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, 840, 0, 600, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_UNUSED, CONDITION_ALWAYS, 872, 0, 600, FACING_SOUTH, 0, 720, 544, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, 680, 0, 584, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
    { SCENE_TABLE_END },
};

u32 gMercuryLighthouse_Exits[] = {
    SCENE_EXITS(MercuryLighthouseLower),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE),
    SCENE_EXITS(MercuryLighthouseMiddle),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST),
    SCENE_EXITS(MercuryLighthouseUpper),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_ROW_STATUES),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_FROM_GATE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_JETS_STATUE_FALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_JETS_FROM_GATE_STATUE_FALLS),
    SCENE_EXITS(MercuryLighthouseExterior),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, WorldMap, WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, MercuryLighthouseAerie, MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXITS_END,
};

struct ActorPlacement gMercuryLighthouseNoActorPlacements[] = {
    { SCENE_TABLE_END },
};

struct ActorPlacement gMercuryLighthouseLowerActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 632, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        672, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        704, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        744, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        648, 0, 299, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMercuryLighthouseMiddleActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        152, 0, 528, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        184, 0, 587, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        704, 0, 672, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        216, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SLEET), MOVEMENT_TYPE_STAND,
        888, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMercuryLighthouseUpperActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL), MOVEMENT_TYPE_STAND,
        200, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL, MOVEMENT_TYPE_STAND,
        808, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        816, 0, 544, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        872, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        760, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        776, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        872, 0, 635, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        648, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        712, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        776, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        936, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        472, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

struct ActorPlacement gMercuryLighthouseExteriorActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 664, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        888, 0, 872, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        840, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        856, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        824, 0, 696, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
        760, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
        920, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        856, 0, 792, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

struct Trigger gMercuryLighthouseLowerTriggers[] = {
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER },
    { TRIGGER_STEP, CELL_BEHAVIOR_SLEEP_BOMB_LEDGE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_LoseLightsOnLower) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_STEP, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_LoseLightsOnLower) },
    { STEP_TRIGGER_WALKING | STEP_TRIGGER_FLAG_QUICK, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_WATER, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_RippleWhileInPool) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { TRIGGER_STEP, CELL_BEHAVIOR_MERCURY_LIGHTHOUSE_POOL_HEART, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT(MercuryLighthouse_ChargeLightsOnLower) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BY_CRYSTAL, CONDITION_ALWAYS, EVENT(MercuryLighthouse_JumpDownPastCrystal) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_CHANNEL_PIPE_PUSH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushChannelPipes) },
    { TRIGGER_TALK, ACTOR_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThePathIsBlockedAgainWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThisStatueWasntHereBeforeI) },
    { PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_BLOCKING_MIA, FLAG_MIA_JOINED_PARTY, EVENT(MercuryLighthouse_MiaJoinsAfterRescue) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_CHEST_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { TRIGGER_TABLE_END },
};

struct Trigger gMercuryLighthouseMiddleTriggers[] = {
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_BY_CRYSTAL, CONDITION_ALWAYS, EVENT(MercuryLighthouse_JumpDownPastCrystal) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_SPOUT_PIPE_PUSH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushSpoutAndFountainPipes) },
    { TRIGGER_STEP, CELL_BEHAVIOR_EAST_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), EVENT(MercuryLighthouse_PushLeaderOutOfSpout) },
    { TRIGGER_STEP, CELL_BEHAVIOR_SOUTH_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), EVENT(MercuryLighthouse_PushLeaderOutOfSpout) },
    { TRIGGER_STEP, CELL_BEHAVIOR_WEST_SPOUT, IF_FLAG_SET(FLAG_SPOUTS_RUNNING), EVENT(MercuryLighthouse_PushLeaderOutOfSpout) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT(MercuryLighthouse_PushGateSwitchStatue) },
    { TRIGGER_STEP, CELL_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT(MercuryLighthouse_PressGateFloorSwitch) },
    { TRIGGER_STEP, CELL_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT(MercuryLighthouse_ReleaseGateFloorSwitch) },
    { TRIGGER_STEP, CELL_BEHAVIOR_BESIDE_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT(MercuryLighthouse_ReleaseGateFloorSwitch) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_FOUNTAIN_PIPE_PUSH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushSpoutAndFountainPipes) },
    { TRIGGER_STEP, CELL_BEHAVIOR_FOUNTAIN_JET, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushLeaderOutOfSpout) },
    { TRIGGER_TALK, ACTOR_SLEET, CONDITION_ALWAYS, EVENT(MercuryLighthouse_StartDjinnBattle) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SEALED_GATE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_STATUE, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT(MercuryLighthouse_OpenGateWithMovedStatue) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_CHEST_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 102, FLAG_CHEST_NUT, FIND_ITEM(ITEM_NUT) },
    { TRIGGER_TABLE_END },
};

struct Trigger gMercuryLighthouseUpperTriggers[] = {
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, CONDITION_ALWAYS, EVENT(MercuryLighthouse_WaterfallToSleetsRoom) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_ROW_STATUES },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_JETS },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, CONDITION_ALWAYS, EVENT(MercuryLighthouse_WaterfallToGapRoom) },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_ROW_STATUES_TO_GATE },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_JETS_TO_GATE },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT(MercuryLighthouse_PushUpperGateSwitchStatue) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT(MercuryLighthouse_PushUpperGateSwitchStatue) },
    { TRIGGER_STEP, CELL_BEHAVIOR_GATE_FLOOR_SWITCH, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT(MercuryLighthouse_PressUpperGateFloorSwitch) },
    { TRIGGER_STEP, CELL_BEHAVIOR_GATE_ROOM_FLOOR, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT(MercuryLighthouse_ReleaseUpperGateFloorSwitch) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_JET_ROOM_PIPE_PUSH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushJetRoomPipes) },
    { STEP_TRIGGER_WALKING, CELL_BEHAVIOR_ROW_STATUE_PUSH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PushRowStatue) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SEALED_GATE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLEN_STATUE, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT(MercuryLighthouse_OpenUpperGateWithMovedStatue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RecordRowStatues) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 1, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RecordRowStatues) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 2, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RecordRowStatues) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_ROW_STATUE + 3, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RecordRowStatues) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_ROW_STATUE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RecordRowStatues) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GAP_STATUE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_MoveGapStatueOffDoorway) },
    { TRIGGER_TABLE_END },
};

struct Trigger gMercuryLighthouseExteriorTriggers[] = {
    { TRIGGER_EXIT, CELL_BEHAVIOR_LIGHTHOUSE_DOOR, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE },
    { TRIGGER_EXIT, CELL_BEHAVIOR_CAUSEWAY_FOOT, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST },
    { TRIGGER_EXIT, EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EXIT_CELL_BEHAVIOR_OFFSET + MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH, CONDITION_ALWAYS, EVENT(MercuryLighthouse_WaterfallToUpperPath) },
    { TRIGGER_TALK, ACTOR_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { TRIGGER_TALK, ACTOR_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, EVENT(MercuryLighthouse_MiaDiscussesBlockedEntrance) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, MSGNO(MsgMercuryLighthouseSomeoneDoesntWantMeToGet) },
    { TRIGGER_TALK, ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseTheLegendsSaidThatIfThe) },
    { TRIGGER_TALK, ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseOurFountainIsBackTheySay) },
    { TRIGGER_TALK, ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIDrankTheHealingWaterAnd) },
    { TRIGGER_TALK, ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseWeDontHaveToFearAny) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseHowDidTheFountainKnowThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseAllThatLivesThatMeansIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIKnewItWouldWorkBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseINeverBelievedThoseStoriesBut) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_SEALED_DOOR, CONDITION_ALWAYS, EVENT(MercuryLighthouse_InspectSealedDoor) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_FOUNTAIN_SIGN, CONDITION_ALWAYS, EVENT(MercuryLighthouse_ReadHermesFountainSign) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_HEALING_FOUNTAIN, FLAG_HERMES_WATER_TAKEN, EVENT(MercuryLighthouse_InspectHealingFountain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_IMIL_MET_MIA, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, EVENT(MercuryLighthouse_MiaOpensEntranceWithPly) },
    { PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_WATERFALL_STATUE, CONDITION_ALWAYS, EVENT(MercuryLighthouse_PlyOpensWaterfallPassage) },
    { PSYNERGY_ANYWHERE(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { USE_ITEM(ITEM_EMPTY_BOTTLE), CELL_BEHAVIOR_HEALING_FOUNTAIN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), EVENT(MercuryLighthouse_FillHermesWaterBottle) },
    { TRIGGER_TABLE_END },
};
// clang-format on
