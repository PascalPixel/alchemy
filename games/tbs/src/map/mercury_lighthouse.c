/// @file
/// Mercury Lighthouse (MapId_MercuryLighthouseLower,
/// MapId_MercuryLighthouseMiddle, MapId_MercuryLighthouseUpper and
/// MapId_MercuryLighthouseExterior), four of the lighthouse's maps. They
/// share the game's area name Mercury Lighthouse with
/// MapId_MercuryLighthouseEntrance (mercury_lighthouse_entrance.c), the floor
/// inside the door; the Lighthouse Aerie at the top is
/// MapId_MercuryLighthouseAerie (mercury_lighthouse_aerie.c). The four share
/// one warp table.
///
/// Every entry sets FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts
/// regain PP faster, and Retreat leads out to MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP. The story flags order the
/// floors: the door opens only to Mia's Ply, once the party has met her in
/// Imil (FLAG_IMIL_MET_MIA), and the fountain whose water heals Tret flows
/// only once Saturos is beaten.
///
/// - MapId_MercuryLighthouseExterior, the lighthouse's door and the ground
///   around it, reached from the world map
///   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP). Until Mia has hurried here
///   the door stays shut. Then she stands before it, blocked by a statue
///   (LOCALID_DOOR_STATUE): she senses someone inside
///   (MercuryLighthouse_MiaSensesSomeoneInside,
///   FLAG_MIA_SENSED_SOMEONE_INSIDE). Move on the statue
///   (MercuryLighthouse_MiaOpensEntranceWithPly): Mia saw the force that pushed
///   it, learns the word Psynergy, opens the door with Ply, the power handed
///   down in her clan, and goes in (FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA).
///   MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, through the
///   door, leads into MercuryLighthouseEntrance.
/// - MapId_MercuryLighthouseLower. Mia is blocked again by a statue that was
///   not there before; Move pushes it aside, and she joins the party with Fizz,
///   a Mercury Djinni (MercuryLighthouse_MiaJoinsAfterRescue,
///   FLAG_MIA_JOINED_PARTY). Three pipe sections pushed into place let the
///   water run (FLAG_PIPE_1_JOINED to _3_JOINED) and carry the party down to
///   MercuryLighthouseEntrance
///   (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE,
///   FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN; the pipe room before and after is
///   places/mercury-lighthouse-lower/map-4.png and
///   places/mercury-lighthouse-lower_mercury-lighthouse-channels-open/map-4.png).
///   Once Ply has worked in MercuryLighthouseEntrance
///   (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), the pool's heart gives the
///   leader golden rings of light (SPRITE_GOLDEN_RINGS) that carry him across
///   the water, a ring spent with each hop, as there
///   (common/mercury_lighthouse_pillar.h). A chest holds a Sleep Bomb.
/// - MapId_MercuryLighthouseMiddle. One pipe section raises or lowers stairs
///   (FLAG_STAIRS_RAISED), another starts a fountain (FLAG_FOUNTAIN_RUNNING),
///   and a statue moved onto a floor switch holds a gate open for good
///   (FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN); standing on the switch opens it
///   only while the leader stays. Sleet, a Mercury Djinni, fights when spoken
///   to. Chests hold a Psy Crystal and a Nut.
/// - MapId_MercuryLighthouseUpper. A pipe section pushed along a row opens
/// one of
///   three stone doors by its column, and at column 48 sends a statue sliding
///   off its ledge to the floor below (FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL),
///   where Move sets it on the stair gate's switch
///   (FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN). Five stone statues pushed
///   into a row, and one more into a gap, open crossings.
/// - MercuryLighthouseExterior again, higher up, reached from
///   MercuryLighthouseMiddle
///   (MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR) and
///   MercuryLighthouseUpper
///   (MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST and
///   MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST): a lift down
///   from the Lighthouse Aerie
///   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE), the Fountain
///   of Healing, whose sign promises the Water of Hermes, and a waterfall.
///   Arriving at the waterfall
///   (MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH) shows its
///   riddle, `He who honors
///   the goddess of rainbows shall be guided to the heavens upon wings of fluid
///   grace.`; Ply on its statue stops it and opens the way up
///   (MercuryLighthouse_PlyOpensWaterfallPassage). The first time, Mia rises
///   first and the party follows
///   (MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, the world map's
///   WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT, whose lighthouse transition ends
///   at the Lighthouse Aerie); there the beacon has already been lit, and the
///   party fights Saturos (mercury_lighthouse_aerie.c). Once Saturos is beaten
///   (FLAG_SATUROS_BEATEN_AT_MERCURY) the leader rises alone
///   (MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, the aerie's
///   MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR), the fountain
///   flows, people come to drink from it, and an Empty Bottle used at it
///   becomes Hermes' Water, once a visit (FLAG_HERMES_WATER_TAKEN).
///
/// Open questions: none.

#include "action_script.h"
#include "save_block.h"
#include "heap.h"
#include "iwram_call.h"
#include "keystate.h"
#include "map_ids.h"
#include "unit/import.h"
#include "unit/map/mercury_lighthouse.h"
#include "unit/map/common/effect_spawn.h"
#include "pushed_object.h"
#include "unit/map/common/mercury_lighthouse_lights.h"
#include "unit/map/common/mercury_lighthouse_pillar.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/screens.h"
#include "unit/runtime/runtime.h"
#include "object_event.h"
#include "mem.h"
#include "mercury.h"
#include "script_layer.h"
#include "field_effect.h"
#include "object_effect.h"
#include "ram_buffer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "script_layer.h"
#include "engine_call.h"
#include "map_tables.h"
#include "djinn_ids.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "heap_slots.h"
#include "world_map_render_work.h"
#include "io_reg.h"
#include "dma.h"
#include "object.h"
#include "animspr.h"
#include "map_grid.h"
#include "map_grid_scroll.h"
#include "fx_work.h"
#include "map_header.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "battle_unit.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "macros.h"

#if TBS_PATCH_1
#define MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY ObjectEvent_MoveToAndWait
#else
#define MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY ObjectEvent_SetDestination
#endif

#ifdef UBFIX
#define MERCURY_LIGHTHOUSE_ANIMATE_OBJ_EVENT_AND_WAIT                          \
  ObjectMotion_SetAnimationAndWait
#else
#define MERCURY_LIGHTHOUSE_ANIMATE_OBJ_EVENT_AND_WAIT                          \
  ((s32 (*)())ObjectMotion_SetAnimationAndWait)
#endif

static void MercuryLighthouse_OpenGateWithMovedStatue(void);
static void MercuryLighthouse_OpenStairGateWithMovedStatue(void);
static void MercuryLighthouse_SyncCrossingStatueFlags(void);
static void MercuryLighthouse_RaiseStairsWithSparkles(void);
static void MercuryLighthouse_LowerStairsWithSparkles(void);
static void MercuryLighthouse_SparkleAroundLeader(void);
static void MercuryLighthouse_SparkleAt(s32 x, s32 y, s32 z);
static void MercuryLighthouse_SparkleNear(s32 x, s32 y, s32 z);
static void MercuryLighthouse_SpawnRandomizedBurst(s32 x, s32 y, s32 z,
                                                   s32 angle);
static void MercuryLighthouse_RideLift(void);
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void);
static void MercuryLighthouse_ReplaceInventoryItem(s32 oldItem, s32 newItem);

#define NULL ((void *)0)

/// MercuryLighthouseLower: Mia, the statue that blocks her and the pipe pieces
/// that join the channels.
enum MercuryLighthouseLowerLocalId {
  /// ★ Named from the game: "But this statue is blocking my way..."
  /// (MsgImilSomebodyHere)
  LOCALID_STATUE_BLOCKING_MIA = LOCALID_FIRST_PLACED,
  /// Move pushes the pipe pieces (SPRITE_PIPE_WEST_EAST for the first two,
  /// SPRITE_PIPE_NORTH_SOUTH for the third) along until each joins its
  /// channel's pipes.
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east, spr_228.png (its render)
  LOCALID_FIRST_PIPE,
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east, spr_228.png (its render)
  LOCALID_SECOND_PIPE,
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south, spr_229.png (its render)
  LOCALID_THIRD_PIPE,
  /// SPRITE_PIPE_END, one still frame, 59 pixels south of where
  /// LOCALID_THIRD_PIPE comes to rest; pushing it does nothing, and nothing
  /// else
  /// moves it: the rim of the fixed pipe the third section joins.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// in spr_230.png (its render)
  LOCALID_PIPE_END
};

/// MercuryLighthouseMiddle: the stair and fountain pipes, the statue for the
/// gate's floor switch and Sleet, a Mercury Djinni.
enum MercuryLighthouseMiddleLocalId {
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south, spr_229.png (its render)
  LOCALID_STAIR_PIPE = LOCALID_FIRST_PLACED,
  /// 59 pixels south of where LOCALID_STAIR_PIPE rests once the stairs are
  /// raised; nothing moves it.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// in spr_230.png (its render)
  LOCALID_STAIR_PIPE_END,
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east, spr_228.png (its render)
  LOCALID_FOUNTAIN_PIPE,
  /// ★ Named from what the player sees: a blue statue of a woman holding a jar
  /// (its render)
  LOCALID_GATE_STATUE,
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  LOCALID_SLEET
};

/// MercuryLighthouseUpper: the sliding statue and where it lands below, the
/// pipes before the stone doors, the five statues of the crossing and the gap
/// statue.
enum MercuryLighthouseUpperLocalId {
  /// The statue of LOCALID_SLIDING_STATUE once it has fallen to the floor
  /// below.
  /// ★ Named from what the player sees: the blue statue LOCALID_SLIDING_STATUE
  /// shows on its ledge above, before it falls (its render)
  LOCALID_FALLEN_STATUE = LOCALID_FIRST_PLACED,
  /// Slides off its ledge when the door switch pipe stops at column 48.
  /// ★ Named from what the player sees: a blue statue under a jet of water on
  /// its ledge (its render)
  LOCALID_SLIDING_STATUE,
  /// ★ Named from what the player sees: a stone pipe section lying west to
  /// east, spr_228.png (its render)
  LOCALID_DOOR_ROOM_PIPE,
  /// ★ Named from what the player sees: a stone pipe section lying north to
  /// south, spr_229.png (its render)
  LOCALID_DOOR_SWITCH_PIPE,
  /// Three in a row south of the stone doors' pipes; nothing moves them.
  /// ★ Named from what the player sees: the rim at the top of an upright pipe
  /// in spr_230.png (its render)
  LOCALID_FIRST_DOOR_PIPE_END,
  /// Five in a row along the north wall.
  /// ★ Named from what the player sees: stone statues of a robed figure (its
  /// render)
  LOCALID_FIRST_CROSSING_STATUE = LOCALID_FIRST_DOOR_PIPE_END + 3,
  /// ★ Named from what the player sees: a stone statue of a robed figure (its
  /// render)
  LOCALID_LAST_CROSSING_STATUE = LOCALID_FIRST_CROSSING_STATUE + 4,
  /// ★ Named from what the player sees: a stone statue of a robed figure
  /// before a gap in the floor (its render)
  LOCALID_GAP_STATUE
};

/// MercuryLighthouseExterior: Garet, Ivan and Mia in their own party slots, the
/// statues by the way in, the statue at the door, the Ply target, the statue by
/// the waterfall, the statue on the lift and the people at the fountain.
enum MercuryLighthouseExteriorLocalId {
  /// A pair of statues either side of the way in from the world map
  /// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP); the east one is drawn
  /// mirrored.
  /// ★ Named from what the player sees: blue statues either side of the way in
  /// (its render)
  LOCALID_WEST_APPROACH_STATUE = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: blue statues either side of the way in
  /// (its render)
  LOCALID_EAST_APPROACH_STATUE,
  /// ★ Named from the game: "checked the stone statue..." (MsgCheckedGround)
  LOCALID_DOOR_STATUE,
  /// ★ Named from the game: "used Ply." (MsgMercuryDidThat)
  LOCALID_PLY_TARGET,
  /// ★ Named from what the player sees: a blue statue beside the waterfall (its
  /// render)
  LOCALID_WATERFALL_STATUE,
  /// ★ Named from what the player sees: the blue statue that stands on the lift
  /// in the Lighthouse Aerie (its render)
  LOCALID_LIFT_STATUE,
  /// The first of four people at the Fountain of Healing once Saturos is
  /// beaten, in order a young man, a woman with fair hair, an old man with a
  /// cane and
  /// an old woman.
  /// ★ Named from what the player sees: people gathered round the fountain
  /// (its render)
  LOCALID_FIRST_VISITOR
};

/// Puzzle states kept until the party leaves the lighthouse.
enum {
  /// ★ Named from its area name: Mercury Lighthouse
  FLAG_STAIRS_RAISED = FLAG_MAPSEC_TEMP_02,
  /// ★ Named from the game: "peered into the fountain..." (MsgCheckedGround)
  FLAG_FOUNTAIN_RUNNING = FLAG_MAPSEC_TEMP_06,
  /// ★ Named from the game: "peered into the fountain..." (MsgCheckedGround)
  FLAG_FOUNTAIN_SHOWN =
      FLAG_MAPSEC_TEMP_07, ///< the camera has shown it start once
  /// MercuryLighthouseLower's first pipe piece has been pushed into place,
  /// joining the water pipes; with all three joined the water carries the party
  /// down.
  /// ★ Named from what the player sees: a pipe piece pushed along until it
  /// joins the pipes and water sprays through (its render)
  FLAG_PIPE_1_JOINED = FLAG_MAPSEC_TEMP_10,
  /// ★ Named from what the player sees: the second pipe piece joining the
  /// pipes (its render)
  FLAG_PIPE_2_JOINED = FLAG_MAPSEC_TEMP_11,
  /// ★ Named from what the player sees: the third pipe piece joining the pipes
  /// (its render)
  FLAG_PIPE_3_JOINED = FLAG_MAPSEC_TEMP_12,
  /// Unused: cleared with the three, but nothing sets or tests it.
  /// ★ Named from what the player sees: a stone pipe section (its render)
  FLAG_UNUSED_PIPE_4_JOINED = FLAG_MAPSEC_TEMP_13,
  /// ★ Named from what the player sees: a stone pipe section (its render)
  FLAG_DOOR_ROOM_PIPE_PUSHED = FLAG_MAPSEC_TEMP_18,
  /// Where the door switch pipe rests: each column opens a different stone door
  /// (MercuryLighthouse_PushStoneDoorPipes). At column 47 it opens
  /// STONE_DOOR_NORTH, along the sliding statue's row.
  /// ★ Named from what the player sees: a stone pipe section (its render)
  FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR = FLAG_MAPSEC_TEMP_19,
  /// At column 48 it opens STONE_DOOR_SOUTH, where the statue slides out and
  /// falls.
  /// ★ Named from what the player sees: a stone pipe section (its render)
  FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR = FLAG_MAPSEC_TEMP_1A,
  /// At column 53 it leaves every door shut; with none of the three set it
  /// opens STONE_DOOR_WEST.
  /// ★ Named from what the player sees: a stone pipe section (its render)
  FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS = FLAG_MAPSEC_TEMP_1B,
  /// Two flags per statue of MercuryLighthouseUpper's row along the north wall,
  /// one for each column it can rest at, then the last statue's.
  /// ★ Named from what the player sees: a row of statues along the wall, each
  /// pushed aside with Move (its render)
  FLAG_FIRST_ROW_STATUE = FLAG_MAPSEC_TEMP_30,
  /// ★ Named from what the player sees: the last statue of the row (its render)
  FLAG_LAST_ROW_STATUE_WEST = FLAG_MAPSEC_TEMP_38,
  /// ★ Named from what the player sees: the last statue of the row (its render)
  FLAG_LAST_ROW_STATUE_EAST = FLAG_MAPSEC_TEMP_39,
  /// The statue in front of a passage has been moved aside for good.
  /// ★ Named from what the player sees: Move shifts the statue west, uncovering
  /// an arched passage in the wall behind it (its render)
  FLAG_PASSAGE_STATUE_MOVED = FLAG_MAPSEC_TEMP_48
};

/// Map flags, kept while the party stays on the map.
enum {
  /// The Empty Bottle has been filled at the Fountain of Healing this visit.
  /// ★ Named from the game: "Hermes' Water" (MsgItemName)
  FLAG_HERMES_WATER_TAKEN = FLAG_MAP_TEMP_00,
  /// ★ Named from what the player sees: the floor switch before the gate,
  /// which sinks under what stands on it (its render)
  FLAG_LEADER_ON_FLOOR_SWITCH = FLAG_MAP_TEMP_56
};

/// The chests: a Sleep Bomb, a Psy Crystal and a Nut.
enum {
  /// ★ Named from the game: "Sleep Bomb" (MsgItemName)
  FLAG_CHEST_SLEEP_BOMB = 0xf64,
  /// ★ Named from the game: "Psy Crystal" (MsgItemName)
  FLAG_CHEST_PSY_CRYSTAL = 0xf66,
  /// ★ Named from the game: "Nut" (MsgItemName)
  FLAG_CHEST_NUT = 0xf67
};

// UB: the fountain rows include negative pixel offsets. Their range is
// -23..0, so multiplication is defined and fits the fixed-point coordinate.
#ifdef UBFIX
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) ((value) * 0x10000)
#else
#define MERCURY_LIGHTHOUSE_OFFSET_FIXED(value) ((value) << 16)
#endif

#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "common/object_staged_object_event.h"

/// MercuryLighthouseMiddle: talking to Sleet, a Mercury Djinni, starts its
/// battle.
static void MercuryLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(LOCALID_SLEET, DJINNI_SLEET);
}

#if TBS_PATCH_3
/// Task: the fade overlay stops following the leader while he
/// crosses the patched floor region (MAPGRID_LAYER_GROUND, 16-pixel rows 13 to
/// 18).
static void MercuryLighthouse_UpdateLeaderEffectTarget(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FadeOverlayWork *effectWork =
      *(struct FadeOverlayWork **)Ram_ObjectEventEffectWork;

  if (leader->layer == 0 && leader->z.fixed >> 20 > 12 &&
      leader->z.fixed >> 20 <= 18)
    effectWork->objectEvent = NULL;
  else
    effectWork->objectEvent = leader;
}
#endif

#include "common/mercury_lighthouse_sparks.h"
#include "common/mercury_lighthouse_palette_cycle.h"

const struct MapWarpDest *MercuryLighthouse_GetWarpDests(void) {
  s16 selector = gSaveBlock.mapId;

  if (selector == (s32)&MapId_MercuryLighthouseExterior) {
    return gMercuryLighthouseExteriorWarpDests;
  }
  if (selector == (s32)&MapId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperWarpDests;
  }
  if (selector == (s32)&MapId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddleWarpDests;
  }
  return gMercuryLighthouseWarpDestsOther;
}

const struct MapWarpRegion *MercuryLighthouse_GetRegions(void) { return NULL; }

/// Returns the packed warps shared by the lighthouse maps.
const u32 *MercuryLighthouse_GetWarps(void) { return gMercuryLighthouse_Warps; }

/// Selects the object events and obstacles for the current lighthouse map.
const struct ObjectEventTemplate *
MercuryLighthouse_GetObjectEventTemplates(void) {
  s16 selector = gSaveBlock.mapId;

  if (selector == (s32)&MapId_MercuryLighthouseLower) {
    return gMercuryLighthouseLowerObjectEventTemplates;
  }
  if (selector == (s32)&MapId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddleObjectEventTemplates;
  }
  if (selector == (s32)&MapId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperObjectEventTemplates;
  }
  if (selector == (s32)&MapId_MercuryLighthouseExterior) {
    return gMercuryLighthouseExteriorObjectEventTemplates;
  }
  return gMercuryLighthouseObjectEventTemplatesOther;
}

/// MercuryLighthouseLower, as in MercuryLighthouseEntrance, once Ply has worked
/// there: touching metatile behavior 21 keeps blue ripples spreading from the
/// leader.
static void MercuryLighthouse_RippleWhileInPool(void) {
  MercuryLighthouse_RippleAroundLeader();
}

/// MercuryLighthouseLower, touching metatile behavior 31 or 32: the leader
/// loses his lights.
static void MercuryLighthouse_LoseLightsOnLower(void) {
  MercuryLighthouse_LoseLights();
}

/// MercuryLighthouseLower, walking into metatile behavior 11, 21 or 31: one
/// step across the pool, spending a light.
static void MercuryLighthouse_HopAcrossPoolOnLower(void) {
  MercuryLighthouse_HopAcrossPool();
}

/// MercuryLighthouseLower, touching metatile behavior 11, the pool's heart: the
/// column of lights over the leader grows a stage.
static void MercuryLighthouse_ChargeLightsOnLower(void) {
  MercuryLighthouse_ChargeLights();
}

/// MercuryLighthouseExterior, searching metatile behavior 10. "Strange forces
/// are at work..."
static void MercuryLighthouse_InspectBlockedPassage(void) {
  FieldScript_Start();
  FieldMessage_Show(MsgMercuryLighthouseStrangeForcesBlockWay, 1);
  FieldScript_End();
}

/// Selects the puzzle interactions and dialogue for the current map.
const struct MapEvent *MercuryLighthouse_GetEvents(void) {
  s16 selector = gSaveBlock.mapId;

  if (selector == (s32)&MapId_MercuryLighthouseLower) {
    return gMercuryLighthouseLowerEvents;
  }
  if (selector == (s32)&MapId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddleEvents;
  }
  if (selector == (s32)&MapId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperEvents;
  }
  return gMercuryLighthouseEventsOther;
}

#include "common/mercury_lighthouse_passage.h"

/// MercuryLighthouseUpper, walking north into metatile behavior 53: the leader
/// rises through
/// MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST (the
/// metatile behavior less 50), a sparkling passage as in
/// MercuryLighthouseEntrance.
static void MercuryLighthouse_LeaveUpperByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(gFieldWork->coordEvent - 50);
}

/// MercuryLighthouseUpper, walking north into metatile behavior 63: the same,
/// through MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP.
static void MercuryLighthouse_LeaveUpperWaterfallsByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(gFieldWork->coordEvent - 50);
}

/// MercuryLighthouseExterior, walking north into metatile behavior 55: the
/// same, through MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH.
static void MercuryLighthouse_LeaveWaterfallByPassage(void) {
  MercuryLighthouse_LeaveThroughPassage(gFieldWork->coordEvent - 50);
}

/// MercuryLighthouseLower: with all three channel pipes in place the floor
/// shakes, the screen fades and the party is carried down to
/// MercuryLighthouseEntrance
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE); moving one
/// out again clears the flag.
static void MercuryLighthouse_ExitWhenChannelsOpen(void) {
  if (GameFlag_Get(FLAG_PIPE_1_JOINED) != 0 &&
      GameFlag_Get(FLAG_PIPE_2_JOINED) != 0 &&
      GameFlag_Get(FLAG_PIPE_3_JOINED) != 0) {
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
    FieldScript_WaitFrames(30);
    MapGrid_SetShake(PIXELS(1), PIXELS(1), Q16_ONE);
    Sound_PlaySong(SE_RUMBLE);
    FieldScript_WaitFrames(60);
    gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    FieldScript_FinishScreenTransition();
    FieldScript_WaitTransitionFrames();
    Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
    MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
    MapGrid_WaitForShakeToSettle();
    Import_ScriptRequestWarp(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE);
  } else {
    GameFlag_Clear(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
  }
}

/// MercuryLighthouseLower, pushing a channel pipe (walking into metatile
/// behavior 25): each pipe opens one channel when it reaches its place, and the
/// water runs on through the ones before it.
static void MercuryLighthouse_PushChannelPipes(void) {
  struct PushedObjectProbe probe;
  s32 column;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    // Each pipe controls one channel; restore the connected water tiles too.
    switch ((u32)probe.objectEventSlot) {
    case LOCALID_FIRST_PIPE:
      if (probe.position.z >> 20 == 8) {
        ObjectEvent_MoveAndRedraw(probe);
        FieldScript_WaitFrames(20);
        MapGrid_CopyCellsTo(119, 9, 109, 11, 1, 1);
        MercuryLighthouse_SpawnRandomizedBurst(PIXELS(726), 0, PIXELS(180),
                                               0x8000);
        GameFlag_Set(FLAG_PIPE_1_JOINED);
      } else {
        s32 cellCount = 1;

        MapGrid_CopyCellsTo(117, 9, 104, 7, cellCount, cellCount);
        MapGrid_CopyCellsTo(119, 8, 109, 11, cellCount, cellCount);
        MapGrid_CopyCellsTo(118, 8, 104, 13, cellCount, cellCount);
        ObjectEvent_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_PIPE_1_JOINED);
      }
      break;
    case LOCALID_SECOND_PIPE:
      if (probe.position.z >> 20 == 12) {
        ObjectEvent_MoveAndRedraw(probe);
        FieldScript_WaitFrames(10);
        if (Script_GetFlag(FLAG_PIPE_1_JOINED)) {
          MapGrid_CopyCellsTo(118, 9, 104, 13, 1, 1);
          MercuryLighthouse_SpawnRandomizedBurst(PIXELS(644), 0, PIXELS(210),
                                                 0x4000);
        }
        GameFlag_Set(FLAG_PIPE_2_JOINED);
      } else {
        s32 cellCount = 1;

        MapGrid_CopyCellsTo(119, 8, 109, 11, cellCount, cellCount);
        if (GameFlag_Get(FLAG_PIPE_1_JOINED)) {
          MapGrid_CopyCellsTo(119, 9, 109, 11, cellCount, cellCount);
          MapGrid_CopyCellsTo(118, 8, 104, 13, cellCount, cellCount);
        }
        ObjectEvent_MoveAndRedraw(probe);
        Script_ClearFlag(FLAG_PIPE_2_JOINED);
      }
      break;
    case LOCALID_THIRD_PIPE:
      column = probe.position.x;
      if (column >> 20 == 40) {
        ObjectEvent_MoveAndRedraw(probe);
        Script_SetFlag(FLAG_PIPE_3_JOINED);
      } else {
        ObjectEvent_MoveAndRedraw(probe);
        Script_ClearFlag(FLAG_PIPE_3_JOINED);
      }
      break;
    }
    MercuryLighthouse_ExitWhenChannelsOpen();
  }
  FieldScript_End();
}

/// MercuryLighthouseMiddle, touching metatile behavior 2, 3 or 4 once the
/// stairs are raised, or 46: the leader is carried to the landing in a trail of
/// sparkles.
static void MercuryLighthouse_WalkLeaderIn(void) {
  struct FieldWork *work;

  work = gFieldWork;
  FieldScript_Start();
  FieldScheduler_AddCallback(MercuryLighthouse_SparkleAroundLeader,
                             TASK_PRIORITY_MAP);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(5) / 2, PIXELS(5) / 4);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags &=
      ~OBJ_EVENT_FACES_MOTION;
  Sound_PlaySong(SE_228);
  // Each touched entry has its own landing point; the patched path waits first.
  if (work->coordEvent == 2) {
    ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 232, 616);
  } else if (work->coordEvent == 3) {
    ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 360, 728);
  } else if (work->coordEvent == 4) {
    ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 248, 792);
  } else {
    MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY(LOCALID_PARTY_LEADER, 696, 592);
#if TBS_PATCH_1
    ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 696, 600);
    FieldScript_WaitFrames(30);
#endif
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->steeringFlags |=
      OBJ_EVENT_FACES_MOTION;
  Task_Remove(MercuryLighthouse_SparkleAroundLeader);
  FieldScript_End();
}

/// MercuryLighthouseMiddle, pushing the gate statue (walking into metatile
/// behavior 10) while the gate is shut: it moves a step, and may come to rest
/// on the floor switch.
static void MercuryLighthouse_PushGateSwitchStatue(void) {
  FieldScript_Start();
  PushedObject_AdvancePair();
  FieldScript_End();
  MercuryLighthouse_OpenGateWithMovedStatue();
}

/// MercuryLighthouseMiddle, Move on the gate statue, or after a push: resting
/// on the floor switch at (5, 13), it sinks two pixels and the gate opens for
/// good.
static void MercuryLighthouse_OpenGateWithMovedStatue(void) {
  s32 column;
  s32 row;
  struct ObjectEvent *statue;

  column = ObjectEvent_GetById(LOCALID_GATE_STATUE)->x.fixed / PIXELS(16);
  row = ObjectEvent_GetById(LOCALID_GATE_STATUE)->z.fixed / PIXELS(16);
  FieldScript_Start();
  if (column == 5 && row == 13) {
    ObjectEvent_GetById(LOCALID_GATE_STATUE)->y.fixed += -PIXELS(2);
    statue = ObjectEvent_GetById(LOCALID_GATE_STATUE);
    statue->targetY = ObjectEvent_GetById(LOCALID_GATE_STATUE)->y.fixed;
    MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
    Sound_PlaySong(SE_217);
    MapGrid_PlayMetatileCopySequence(gMercuryLighthouse_GateCells, 9, 7);
    {
      s32 gateColumn = 9;
      s32 gateRow = 10;
      MapGrid_CopyCellAttributes(9, 5, 1, 1, gateColumn, gateRow);
    }
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN);
  }
  FieldScript_End();
}

/// MercuryLighthouseMiddle, touching metatile behavior 12 while the gate is
/// shut: standing on the floor switch sinks the leader two pixels and opens the
/// gate.
static void MercuryLighthouse_PressGateFloorSwitch(void) {
  s32 x;
  s32 z;
  struct ObjectEvent *leader;

  if (Script_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) == 0) {
    x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.part.pixel;
    z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.part.pixel;
    if ((u32)(x - 84) <= 7 && z > 211 && z <= 219) {
      FieldScript_Start();
      GameFlag_Set(FLAG_LEADER_ON_FLOOR_SWITCH);
      FieldScript_WaitFrames(5);
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed += -PIXELS(2);
      leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
      leader->targetY = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
      MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
      Sound_PlaySong(SE_217);
      MapGrid_PlayMetatileCopySequence(gMercuryLighthouse_GateCells, 9, 7);
      FieldScript_End();
    }
  }
}

/// MercuryLighthouseMiddle, touching metatile behavior 10 or 13: stepping off
/// the switch closes the gate again.
static void MercuryLighthouse_CloseFloorSwitch(void) {
  struct ObjectEvent *leader;

  if (Script_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    FieldScript_Start();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed += PIXELS(2);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    leader->targetY = ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed;
    FieldScript_WaitFrames(5);
    MapGrid_CopyCellsTo(7, 2, 5, 11, 1, 1);
    Sound_PlaySong(SE_217);
    MapGrid_AnimateCells(gMercuryLighthouse_GateCloseCells, 9, 7);
    FieldScript_End();
  }
}

/// Animates the fountain's map grid strips between the requested steps.
static void MercuryLighthouse_AnimateFountainTiles(s32 delayFrames,
                                                   s32 firstStep, s32 endStep) {
  u32 step;
  s32 sourceColumn;

  if (delayFrames != 0) {
    Sound_PlaySong(SE_219);
  }
  step = firstStep;
  if (step < endStep) {
    do {
      MapGrid_CopyCellsTo(45 - (step << 1), 32, 44 - (step << 1), 32,
                          (step + 1), 6);
      MapGrid_CopyCellsTo(45 - step, 51, 45 - step, 32, 1, 6);
      sourceColumn = 109 - step;
      MapGrid_CopyCellsTo(sourceColumn, 32, 108 - step, 32, 1, 4);
      MapGrid_CopyCellsTo(sourceColumn, 51, sourceColumn, 32, 1, 4);
      if (delayFrames != 0) {
        MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
        MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
        FieldScript_WaitFrames(delayFrames);
      }
      step++;
    } while (step < endStep);
  }
  MapGrid_CopyCellAttributes(42, 52, 4, 5, 42, 33);
}

/// Restores the fountain's map grid strips after moving its pipe away.
static void MercuryLighthouse_RestoreFountainTiles(s32 delayFrames) {
  u32 step;
  s32 destinationColumn;
  s32 sourceColumn;
  s32 copyWidth;
  s32 copyHeight;
  s32 column;
  s32 stripHeight;
  Sound_PlaySong(SE_219);
  copyHeight = 6;
  step = 0;
  destinationColumn = 41;
  sourceColumn = 40;
  do {
    MapGrid_CopyCellsTo(sourceColumn, 32, destinationColumn, 32, 3 - step,
                        copyHeight);
    copyWidth = 2;
    MapGrid_CopyCellsTo(39, 51, sourceColumn, 32, 1, copyHeight);
    column = step + 106;
    stripHeight = 4;
    /* FAKEMATCH(temporary): the strip's width and height in locals, so loop.c
     * hoists the 2 into r9; as the literals 2 and 4, 11 of 90 instructions
     * differ in tbs-en. */
    MapGrid_CopyMetatileIndicesRect(105, 51, column, 32, copyWidth,
                                    stripHeight);
    if (delayFrames != 0) {
      MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      FieldScript_WaitFrames(delayFrames);
    }
    step++;
    destinationColumn += 2;
    sourceColumn += 2;
  } while (step <= 2);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  MapGrid_CopyCellAttributes(106, 33, 4, 5, 42, 33);
  MapGrid_WaitForShakeToSettle();
}

/// Task: a spray at the fountain's fixed position.
static void MercuryLighthouse_SprayAtFountain(void) {
  MercuryLighthouse_SparkleAt(PIXELS(690), 0, PIXELS(584));
}

/// Opens the fountain row by row and starts its continuous spray.
static void MercuryLighthouse_OpenFountainWithSpray(s32 delayFrames) {
  struct EffectOptions *options;
  s32 row;
  s32 rowOffset;
  s32 particleX;
  s32 particleIndex;
  s32 negativeIndex;
  s32 negativeRowOffset;
  s32 randomX;
  s32 randomZ;
  s32 velocityX;
  s32 velocityZ;
  s32 nextRow;
  struct EffectOptions optionsStorage;

  MapGrid_CopyCellsTo(78, 59, 110, 36, 1, 1);
  MapGrid_CopyCellsTo(76, 59, 109, 36, 1, 1);
  options = &optionsStorage;
  // The shared initializer grows this path from 0x150 to 0x154 bytes and
  // moves its palette store after the scales; the stop path also reorders
  // stores. Keep the ordinary assignments, without a new steering wrapper.
  options->palette = 7;
  options->startScaleX = Q16_ONE / 2;
  options->startScaleY = Q16_ONE / 2;
  row = 0;
  // Reveal each row while its edge emits particles in the two horizontal axes.
  do {
    rowOffset = (row << 4);
    particleIndex = 0;
    negativeRowOffset = -(row << 20);
    particleX = (PIXELS(728) + negativeRowOffset);
    do {
      if ((particleIndex & 1) != 0) {
        randomX = Random_Next();
        // The original add/shift tree is multiplication by 0x3333.
        velocityX = ((u32)(randomX << 3) >> 16) * 0x3333 - 0xcccc;
        negativeIndex = -particleIndex;
        randomZ = Random_Next();
        velocityZ = ((u32)(randomZ << 3) >> 16) * 0x3333 - 0xcccc;
        FieldEffect_Spawn(particleX, 0, PIXELS(584), velocityX, 0, velocityZ,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        FieldScript_WaitFrames(1);
      } else {
        negativeIndex = -particleIndex;
      }
      MercuryLighthouse_SparkleAt(
          (MERCURY_LIGHTHOUSE_OFFSET_FIXED(negativeIndex - rowOffset) +
           PIXELS(728)),
          0, PIXELS(584));
      particleIndex = (particleIndex + 1);
      particleX = (particleX + -PIXELS(1));
    } while ((u32)particleIndex <= 7);
    MapGrid_CopyCellsTo(76, 59, (108 - row), 36, 2, 1);
    nextRow = row + 1;
    MercuryLighthouse_AnimateFountainTiles(delayFrames, row, nextRow);
    row = nextRow;
  } while ((u32)nextRow <= 1);
  FieldScript_WaitFrames(delayFrames);
  MercuryLighthouse_AnimateFountainTiles(0, nextRow, (nextRow + 1));
  Sound_PlaySong(SE_211);
  // Keep the opened fountain spraying after the tile animation completes.
  FieldScheduler_AddCallback(MercuryLighthouse_SprayAtFountain,
                             TASK_PRIORITY_MAP);
  MapGrid_WaitForShakeToSettle();
}

/// Restores the fountain tiles and stops its continuous spray.
static void MercuryLighthouse_StopFountainSpray(void) {
  struct EffectOptions *options;
  s32 row;
  s32 particleIndex;
  s32 offsetX;
  s32 random;
  s32 particleX;
  s32 particleZ;
  struct EffectOptions optionsStorage;

  MapGrid_CopyCellsTo(78, 58, 110, 36, 1, 1);
  options = &optionsStorage;
  options->palette = 5;
  options->startScaleX = Q16_ONE / 2;
  options->startScaleY = Q16_ONE / 2;
  row = 0;
  do {
    offsetX = -PIXELS(2);
    particleIndex = 1;
    do {
      if ((particleIndex & 1) != 0) {
        random = Import_RandomNext();
        particleZ = ((0x248 - ((u32)((random << 2) + random) >> 16)) & 0xffff)
                    << 16;
        particleX = (offsetX - (row << 19)) + PIXELS(728);
        FieldEffect_Spawn(particleX, 0, particleZ, -0x4000, 0, 0,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        FieldScript_WaitFrames(1);
      }
      particleIndex++;
      offsetX = offsetX + -PIXELS(2);
    } while ((u32)particleIndex <= 7);
    MapGrid_CopyCellsTo(111, 35, (109 - row), 36, 1, 1);
    row++;
  } while ((u32)row <= 2);
  Task_Remove(MercuryLighthouse_SprayAtFountain);
}

/// MercuryLighthouseMiddle, pushing a pipe (walking into metatile behavior 1 or
/// 45): the stair pipe raises the stairs at column 11 and lowers them anywhere
/// else; the fountain pipe starts the fountain at row 40, the first time with
/// the camera watching, and stops it at row 42.
static void MercuryLighthouse_PushStairAndFountainPipes(void) {
  struct PushedObjectProbe probe;
  s32 pillarId;
  s32 column;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    pillarId = probe.objectEventSlot;
    // The first pipe opens the stairs; the other starts or stops the
    // fountain.
    if (pillarId == LOCALID_STAIR_PIPE) {
      column = probe.position.x;
      if ((column >> 20) == 11) {
        ObjectEvent_MoveAndRedraw(probe);
        FieldScript_WaitFrames(30);
        Sound_PlaySong(SE_211);
        MercuryLighthouse_RaiseStairsWithSparkles();
        MapGrid_CopyCellsTo(76, 60, 74, 38, 3, 1);
        MapGrid_CopyCellsTo(77, 60, 76, 38, 2, 1);
        MapGrid_CopyCellsTo(75, 58, 86, 41, 1, 3);
        MapGrid_CopyCellsTo(75, 59, 86, 43, 1, 2);
        MapGrid_CopyCellsTo(76, 59, 80, 49, 2, 1);
        MapGrid_CopyCellsTo(77, 59, 82, 49, 2, 1);
        GameFlag_Set(FLAG_STAIRS_RAISED);
      } else {
        probe.callback = MercuryLighthouse_LowerStairsWithSparkles;
        MapGrid_CopyCellsTo(75, 57, 86, 41, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 42, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 43, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 86, 44, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 80, 49, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 81, 49, 1, 1);
        MapGrid_CopyCellsTo(71, 59, 82, 49, 1, 1);
        MapGrid_CopyCellsTo(78, 58, 83, 49, 1, 1);
        ObjectEvent_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_STAIRS_RAISED);
      }
    } else if (pillarId == LOCALID_FOUNTAIN_PIPE) {
      if ((probe.position.z >> 20) == 40) {
        ObjectEvent_MoveAndRedraw(probe);
        if (Script_GetFlag(FLAG_FOUNTAIN_SHOWN) == 0) {
          FieldCamera_SetSpeed(PIXELS(3) / 2, PIXELS(3) / 16);
          FieldCamera_MoveTo(PIXELS(714), -1, PIXELS(592), 1);
          FieldCamera_WaitForArrival();
          GameFlag_Set(FLAG_FOUNTAIN_SHOWN);
          MercuryLighthouse_OpenFountainWithSpray(5);
          FieldScript_WaitFrames(50);
        } else {
          MercuryLighthouse_OpenFountainWithSpray(5);
        }
        GameFlag_Set(FLAG_FOUNTAIN_RUNNING);
      } else if ((probe.position.z >> 20) == 42) {
        probe.callback = MercuryLighthouse_StopFountainSpray;
        ObjectEvent_MoveAndRedraw(probe);
        MercuryLighthouse_RestoreFountainTiles(5);
        GameFlag_Clear(FLAG_FOUNTAIN_RUNNING);
      }
    }
  }
  FieldScript_End();
}

/// MercuryLighthouseUpper, pushing the fallen statue (walking into metatile
/// behavior 10 or 12) while the stair gate is shut: it moves a step and may
/// come to rest on the floor switch.
static void MercuryLighthouse_PushStairSwitchStatue(void) {
  FieldScript_Start();
  PushedObject_AdvancePair();
  FieldScript_End();
  MercuryLighthouse_OpenStairGateWithMovedStatue();
}

/// MercuryLighthouseUpper, Move on the fallen statue, or after a push: resting
/// on the switch at (10, 23), it sinks two pixels and the stair gate opens for
/// good.
static void MercuryLighthouse_OpenStairGateWithMovedStatue(void) {
  s32 column = ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->x.fixed / PIXELS(16);
  s32 row = ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->z.fixed / PIXELS(16);

  FieldScript_Start();
  if (column == 10 && row == 23) {
    struct ObjectEvent *statue;
    ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->y.fixed += -PIXELS(2);
    statue = ObjectEvent_GetById(LOCALID_FALLEN_STATUE);
    statue->targetY = ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->y.fixed;
    MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
    Sound_PlaySong(SE_217);
    MapGrid_PlayMetatileCopySequence(gMercuryLighthouse_FloorSwitchCells, 10,
                                     18);
    MapGrid_CopyCellAttributes(10, 16, 1, 1, column, 19);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN);
  }
  FieldScript_End();
}

/// MercuryLighthouseUpper, touching metatile behavior 12 while the stair gate
/// is shut: standing on its switch opens it.
static void MercuryLighthouse_PressStairFloorSwitch(void) {
  s32 x;
  s32 z;

  if (Script_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0)
    return;
  x = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.part.pixel;
  z = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.part.pixel;
  if (x < 164 || x > 171)
    return;
  if (z < 372)
    return;
  if (z >= 380)
    return;
  FieldScript_Start();
  GameFlag_Set(FLAG_LEADER_ON_FLOOR_SWITCH);
  FieldScript_WaitFrames(5);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed -= PIXELS(2);
  ObjectEvent_GetById(LOCALID_PARTY_LEADER)->targetY =
      ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
  MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
  Sound_PlaySong(SE_217);
  MapGrid_PlayMetatileCopySequence(gMercuryLighthouse_FloorSwitchCells, 10, 18);
  FieldScript_End();
}

/// MercuryLighthouseUpper, touching metatile behavior 10: stepping off the
/// switch closes the stair gate.
static void MercuryLighthouse_ReleaseStairFloorSwitch(void) {
  struct ObjectEvent *leader;

  if (Script_GetFlag(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    FieldScript_Start();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed += PIXELS(2);
    leader = ObjectEvent_Get(LOCALID_PARTY_LEADER);
    leader->targetY = ObjectEvent_Get(LOCALID_PARTY_LEADER)->y.fixed;
    FieldScript_WaitFrames(5);
    MapGrid_CopyCellsTo(8, 29, 10, 23, 1, 1);
    Sound_PlaySong(SE_217);
    MapGrid_AnimateCells(gMercuryLighthouse_FloorSwitchCloseCells, 10, 18);
    FieldScript_End();
  }
}

/// MercuryLighthouseUpper's three stone doors, named by where they stand: the
/// west one opens along its column, the other two along their rows, the north
/// one two cells above the south one.
enum { STONE_DOOR_WEST = 0, STONE_DOOR_SOUTH = 1, STONE_DOOR_NORTH = 2 };

/// Chooses the random speed component used by stone-door particles.
static __inline__ s32 MercuryLighthouse_RandomDoorParticleSpeed(void) {
  // Sharing a centered-velocity helper with other sprays shrinks the fountain
  // from 0x150 to 0x128 bytes and grows this door from 0x27c to 0x284 in
  // English.
  u32 drift = Import_RandomNext();

  return ((drift << 3) >> 16) * 0x3333;
}

/// Opens the selected stone door with particles along its moving edge.
static void MercuryLighthouse_OpenStoneDoor(s32 side) {
  struct EffectOptions optionsStorage;
  u32 particleIndex;
  struct EffectOptions *options;
  u32 row;

  Sound_PlaySong(SE_211);
  if (side == STONE_DOOR_WEST) {
    MapGrid_CopyCellsTo(111, 57, 113, 42, 1, 1);
    MapGrid_CopyCellsTo(111, 59, 113, 43, 1, 1);
  } else if (side == STONE_DOOR_SOUTH) {
    MapGrid_CopyCellsTo(113, 58, 112, 46, side, side);
    MapGrid_CopyCellsTo(115, 58, 113, 46, side, side);
  } else {
    MapGrid_CopyCellsTo(115, 57, 116, 44, 1, 1);
    MapGrid_CopyCellsTo(113, 57, 115, 44, 1, 1);
  }
  options = &optionsStorage;
  options->palette = 7;
  options->startScaleX = Q16_ONE / 2;
  options->startScaleY = Q16_ONE / 2;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (side == STONE_DOOR_WEST) {
          FieldEffect_Spawn(
              PIXELS(792), 0,
              PIXELS(704) + (row * 16 + particleIndex) * PIXELS(1),
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        } else if (side == STONE_DOOR_SOUTH) {
          FieldEffect_Spawn(
              PIXELS(800) + (row * 16 + particleIndex) * PIXELS(1), 0,
              PIXELS(746),
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        } else {
          FieldEffect_Spawn(
              PIXELS(812) + (row * 16 + particleIndex) * -PIXELS(1), 0,
              PIXELS(714),
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        }
        FieldScript_WaitFrames(1);
      }
    }
    if (side == STONE_DOOR_WEST) {
      MapGrid_CopyCellsTo(111, 58, 113, row + 43, 1, 1);
      MapGrid_CopyCellsTo(111, 59, 113, row + 44, 1, 1);
    } else if (side == STONE_DOOR_SOUTH) {
      MapGrid_CopyCellsTo(114, 58, row + 113, 46, side, side);
      MapGrid_CopyCellsTo(115, 58, row + 114, 46, side, side);
    } else {
      MapGrid_CopyCellsTo(114, 57, 115 - row, 44, 1, 1);
      MapGrid_CopyCellsTo(113, 57, 114 - row, 44, 1, 1);
    }
  }
}

/// Restores the selected stone door's tiles with trailing particles.
static void MercuryLighthouse_CloseStoneDoor(s32 side) {
  struct EffectOptions options;
  u32 particleIndex, row;

  MapGrid_CopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
  MapGrid_CopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
  MapGrid_CopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  options.palette = 5;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = Q16_ONE / 2;
  for (row = 0; row <= 2; row++) {
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        if (side == STONE_DOOR_WEST) {
          FieldEffect_Spawn(
              (0x319 - (((u32)Import_RandomNext() * 5) >> 16)) << 16, 0,
              (((row << 2) + particleIndex) << 17) + PIXELS(695), 0, side,
              0x4000, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        } else if (side == STONE_DOOR_SOUTH) {
          FieldEffect_Spawn(
              (((row << 2) + particleIndex) << 17) + PIXELS(786), 0,
              ((((u32)Import_RandomNext() * 5) >> 16) << 16) + PIXELS(744),
              0x4000, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
              &options);
        } else {
          FieldEffect_Spawn(
              PIXELS(824) - (particleIndex << 17) - (row << 19), 0,
              ((((u32)Import_RandomNext() * 5) >> 16) << 16) + PIXELS(712),
              0x4000, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
              &options);
        }
        FieldScript_WaitFrames(1);
      }
    }
    if (side == STONE_DOOR_WEST)
      MapGrid_CopyCellsTo(0x70, 0x3a, 0x71, row + 43, 1, 1);
    else if (side == STONE_DOOR_SOUTH)
      MapGrid_CopyCellsTo(0x70, 0x3a, row + 113, 0x2e, side, side);
    else
      MapGrid_CopyCellsTo(0x70, 0x3a, 115 - row, 0x2c, 1, 1);
  }
}

/// The camera turns to the sliding statue, which moves on only for the door
/// switch pipe positions recorded; returns 1 when it slides off its ledge.
static s32 MercuryLighthouse_StartStatueSlide(void) {
  s32 cellX;
  s32 cellZ;

  cellX = ObjectEvent_GetById(LOCALID_SLIDING_STATUE)->x.fixed / PIXELS(16);
  cellZ = ObjectEvent_GetById(LOCALID_SLIDING_STATUE)->z.fixed / PIXELS(16);
  FieldCamera_SetSpeedParameters(PIXELS(5), PIXELS(5) / 8);
  FieldCamera_MoveWithinMapBounds(PIXELS(816), -1, PIXELS(712), 1);
  FieldCamera_WaitForArrival();
  // Only the recorded pipe arrangements allow the pipe's next push.
  if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL)) {
    if (cellX == 50 && GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR)) {
      ObjectEvent_SetActionScript(LOCALID_SLIDING_STATUE,
                                  gMercuryLighthouse_PushScriptA);
    } else if (cellX == 49) {
      if (cellZ == 44 &&
          !GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR) &&
          !GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR) &&
          !GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS)) {
        ObjectEvent_SetActionScript(LOCALID_SLIDING_STATUE,
                                    gMercuryLighthouse_PushScriptB);
      } else if (cellZ == 44 &&
                 GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR)) {
        ObjectEvent_SetActionScript(LOCALID_SLIDING_STATUE,
                                    gMercuryLighthouse_PushScriptC);
      } else if (cellZ == 46 &&
                 GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR)) {
        ObjectEvent_SetActionScript(LOCALID_SLIDING_STATUE,
                                    gMercuryLighthouse_PushScriptD);
        FieldScript_WaitFrames(30);
        return 1;
      }
    }
  }
  FieldScript_WaitFrames(30);
  return 0;
}

/// Sparkles rise from the sliding statue as it moves.
static void MercuryLighthouse_SprayAboveStatue(void) {
  struct ObjectEvent *statue = ObjectEvent_GetById(LOCALID_SLIDING_STATUE);
  u32 random = Import_RandomNext();

  s32 particleY = statue->y.fixed + (((random << 2) >> 16) << 16);
  s32 particleZ = statue->z.fixed;

  MercuryLighthouse_SparkleAt(statue->x.fixed, particleY, particleZ);
}

/// The slide scripts start the sparkles as the statue sets off.
static s32 MercuryLighthouse_StartStatueSpray(struct Object *interpreter) {
  s32 priority = TASK_PRIORITY_MAP;

  FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue, priority);
  return 0;
}

// The sparkles stop when the statue halts; slide scripts and code call it.
// SC_CALL passes the interpreter. The native no-argument form also has two
// direct callers; adding NULL to those calls inserts two r0 = 0 instructions.
#ifdef UBFIX
static s32 MercuryLighthouse_StopStatueSpray(struct Object *interpreter) {
#else
static s32 MercuryLighthouse_StopStatueSpray(void) {
#endif
  Task_Remove(MercuryLighthouse_SprayAboveStatue);
  return 0;
}

/// MercuryLighthouseUpper, pushing a pipe before the stone doors (walking
/// into metatile behavior 40): the door switch pipe opens a different stone
/// door at each column, and at column 48, once the statue is in place, the
/// statue slides out and falls to the floor below.
static void MercuryLighthouse_PushStoneDoorPipes(void) {
  struct PushedObjectProbe probe;
  s32 destinationColumn;
  s32 startColumn;

  FieldScript_Start();
  if (PushedObject_FindClearPosition(&probe) != 0) {
    switch (probe.objectEventSlot) {
    case LOCALID_DOOR_ROOM_PIPE:
      ObjectEvent_MoveAndRedraw(probe);
      if (probe.position.z >> 20 == 38)
        GameFlag_Set(FLAG_DOOR_ROOM_PIPE_PUSHED);
      else
        GameFlag_Clear(FLAG_DOOR_ROOM_PIPE_PUSHED);
      break;
    case LOCALID_DOOR_SWITCH_PIPE:
      // Close the previous opening before revealing the new door edge.
      startColumn =
          ObjectEvent_GetById(LOCALID_DOOR_SWITCH_PIPE)->x.fixed >> 20;
      ObjectEvent_MoveAndRedraw(probe);
      destinationColumn = probe.position.x >> 20;
      if (destinationColumn == 47) {
        GameFlag_Set(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == 54)
          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_WEST);
        else if (startColumn == 48)
          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_SOUTH);
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_NORTH);
        FieldScript_WaitFrames(60);
      } else if (destinationColumn == 48) {
        GameFlag_Set(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR);
        if (MercuryLighthouse_StartStatueSlide() != 0) {
          s32 x;

          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_NORTH);
          x = PIXELS(840);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSpray(NULL);
#else
          MercuryLighthouse_StopStatueSpray();
#endif
          MercuryLighthouse_OpenStoneDoor(STONE_DOOR_SOUTH);
          ObjectEvent_RefreshSelectorById(LOCALID_SLIDING_STATUE);
          ObjectEvent_PrepareObject(x, 0, PIXELS(786), 223);
          ObjectEvent_PrepareObject(x, 0, PIXELS(818), 223);
          ObjectMotion_ResetAndSetPosition(LOCALID_SLIDING_STATUE, 840, 744);
          FieldScript_WaitFrames(5);
          Sound_PlaySong(SE_189);
          ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SLIDING_STATUE);
          FieldScript_WaitFrames(40);
          GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL);
          PaletteFade_SetSource(PALETTE_TINT_NONE, 0);
          gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
          FieldScript_FinishScreenTransition();
          FieldScript_WaitTransitionFrames();
          Import_ScriptRequestWarp(
              MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE_STATUE_FALLS);
        } else {
          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_NORTH);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSpray(NULL);
#else
          MercuryLighthouse_StopStatueSpray();
#endif
          MercuryLighthouse_OpenStoneDoor(STONE_DOOR_SOUTH);
          FieldScript_WaitFrames(60);
        }
      } else if (destinationColumn == 53) {
        GameFlag_Set(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR);
        MercuryLighthouse_StartStatueSlide();
        MercuryLighthouse_CloseStoneDoor(STONE_DOOR_WEST);
        FieldScript_WaitFrames(60);
      } else {
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == 47)
          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_NORTH);
        else if (startColumn == 48)
          MercuryLighthouse_CloseStoneDoor(STONE_DOOR_SOUTH);
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_WEST);
        FieldScript_WaitFrames(60);
      }
      break;
    }
  }
  FieldScript_End();
}

/// MercuryLighthouseUpper, pushing a crossing statue (walking into metatile
/// behavior 47): it moves a step and where the statues rest is saved.
static void MercuryLighthouse_PushAndSaveCrossingStatuePositions(void) {
  FieldScript_Start();
  PushedObject_AdvancePair();
  MercuryLighthouse_SyncCrossingStatueFlags();
  FieldScript_End();
}

/// MercuryLighthouseUpper, Move on a crossing statue, or after a push: each
/// statue's column is saved in its pair of flags, and the last opens the
/// crossing at column 57 or 59.
static void MercuryLighthouse_SyncCrossingStatueFlags(void) {
  s32 statueIndex;
  s32 positionFlag;
  s32 column;

  FieldScript_Start();
  // Four pillars use paired flags for their two saved stopping columns.
  for (statueIndex = 0, positionFlag = FLAG_FIRST_ROW_STATUE; statueIndex <= 3;
       statueIndex++, positionFlag += 2) {
    column = ObjectEvent_GetById(statueIndex + LOCALID_FIRST_CROSSING_STATUE)
                 ->x.fixed /
             PIXELS(16);
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
  // The last statue also determines whether the crossing cell is open.
  column =
      ObjectEvent_GetById(LOCALID_LAST_CROSSING_STATUE)->x.fixed / PIXELS(16);
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
  FieldScript_End();
}

/// MercuryLighthouseUpper, Move on the gap statue: at column 28 it fills the
/// gap for good.
static void MercuryLighthouse_OpenMovedStatueCrossing(void) {
  s32 column;

  FieldScript_Start();
  column = ObjectEvent_GetById(LOCALID_GAP_STATUE)->x.fixed / PIXELS(16);
  if (column == 28) {
    GameFlag_Set(FLAG_PASSAGE_STATUE_MOVED);
    {
      s32 destinationColumn = 31;
      s32 destinationRow = 20;

      MapGrid_CopyCellAttributes(29, 20, 1, 1, destinationColumn,
                                 destinationRow);
    }
  }
  FieldScript_End();
}

/// Rotates the selected palette range once every eight frames. It writes the
/// saved first color at the end before shifting, preserving the palette access
/// order.
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

/// While the lighthouse door is shut its light shimmers: background palette
/// 2, colors 8 to 15, rotates every eight frames.
static void MercuryLighthouse_CycleEntrancePalette(void) {
  MercuryLighthouse_RotatePaletteRange((u16 *)&BG_PLTT_COLOR(2, 8), 8);
}

/// MercuryLighthouseExterior, talking to Mia at the door or Mind Read on her,
/// before it is open: she senses someone. "Somebody...is here." She asks
/// whether the party hears it, says only Alex can enter besides her, and that
/// the statue blocks her way.
static void MercuryLighthouse_MiaSensesSomeoneInside(void) {
  struct ObjectEvent *mia;

  FieldScript_Start();
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
  FieldScript_SetMessageId(MsgImilSomebodyHere);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 840, 648);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_WEST, 20);
  Import_ObjectEventSetAnimation(LOCALID_MIA, 16);
  mia = ObjectEvent_GetById(LOCALID_MIA);
  mia->scaleX = -Q16_ONE;
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  mia = ObjectEvent_GetById(LOCALID_MIA);
  mia->scaleX = Q16_ONE;
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTH, 20);
  Script_OpenMessage(LOCALID_MIA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
    gFieldWork->message += 1;
  } else {
    gFieldWork->message += 1;
    FieldScript_WaitFrames(20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  }
  FieldScript_WaitFrames(20);
  ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 20);
  FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
  FieldCamera_MoveTo(PIXELS(840), -1, PIXELS(632), 1);
  ObjectEvent_WalkToAndWait(LOCALID_MIA, 840, 632);
  FieldCamera_WaitForArrival();
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
  FieldScript_WaitFrames(10);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  GameFlag_Set(FLAG_MIA_SENSED_SOMEONE_INSIDE);
  FieldScript_End();
}

/// MercuryLighthouseExterior, talking to Mia again. "This statue is blocking
/// the entrance."
static void MercuryLighthouse_MiaDiscussesBlockedEntrance(void) {
  FieldScript_Start();
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  FieldScript_SetMessageId(MSGNO(MsgImilStatueBlocksEntrance));
  FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
  FieldScript_End();
}

/// The door's light brightens to white as it opens.
static void MercuryLighthouse_FadePaletteToWhite(void) {
  // Palette RAM is hardware (its volatile is source, not a device): volatile
  // makes each read of palette RAM a load of its own: the native reads every
  // color with three ldrh, one per component; without it one load is shifted
  // for all three and the function is 104 bytes for 106 in tbs-en.
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
      red = *palette & 31;
      green = (*palette >> 5) & 31;
      blue = (*palette >> 10) & 31;
      if (red == 31 && green == 31 && blue == 31) {
        whiteColors++;
      } else {
        if (red <= 30)
          red++;
        if (green <= 30)
          green++;
        if (blue <= 30)
          blue++;
        *palette = (blue << 10) | (green << 5) | red;
      }
      palette++;
    }
    WaitFrames(2);
  } while (whiteColors <= 7);
}

/// MercuryLighthouseExterior, Move on the door statue: once it stands at column
/// 51 Mia asks whether the party did that, learns of Psynergy, and uses Ply to
/// open the door; she goes in, and Garet and Ivan rejoin the leader.
static void MercuryLighthouse_MiaOpensEntranceWithPly(void) {
  struct ObjectEvent *objectEvent;
  s32 column;

  objectEvent = ObjectEvent_GetById(LOCALID_DOOR_STATUE);
  column = objectEvent->x.fixed;
  if (column < 0)
    column += 0xfffff;
  column >>= 20;
  FieldScript_Start();
  if (column != 51) {
  } else {
    // Mia turns: "Did you do that?"
    ObjectMotion_SetSpeedParameters(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH + FACING_STEP, 0);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                             10);
    FieldScript_SetMessageId(MsgMercuryDidThat);
    FieldMessage_ShowAtObject(LOCALID_MIA, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      gFieldWork->message += 1;
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
      FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    } else {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
      Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
      gFieldWork->message += 1;
    }
    FieldEffect_SpawnLinked(LOCALID_PARTY_LEADER,
                            EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
    // Garet and Ivan step out to explain Psynergy; Mia tells of Ply.
    objectEvent = ObjectEvent_GetById(LOCALID_GARET);
    SetEffectRecordMode(objectEvent, 1);
    objectEvent = ObjectEvent_GetById(LOCALID_IVAN);
    SetEffectRecordMode(objectEvent, 1);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(872), PIXELS(600));
    ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(872), PIXELS(600));
    ObjectEvent_WalkTo(LOCALID_IVAN, 888, 632);
    ObjectEvent_WalkToAndWait(LOCALID_GARET, 880, 616);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 20);
    ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
    Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHWEST + FACING_STEP, 20);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(30);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 20);
    FieldScript_ShowMessageThenWait(LOCALID_IVAN, 0, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    ObjectMotion_ResetAndSetPositionInMode2(LOCALID_MIA, 840, 656);
    FieldScript_WaitFrames(5);
    ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_SOUTH + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    ObjectMotion_ArmCallback(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
    FieldScript_WaitFrames(10);
    FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH + FACING_STEP, 20);
    FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_GARET, 1);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTH, 20);
    FieldMessage_ShowAtObject(LOCALID_GARET, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 0) {
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_NOD);
      FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 20);
      gFieldWork->message += 1;
    } else {
      gFieldWork->message += 1;
      FieldScript_WaitFrames(20);
      ObjectMotion_SetAnimationAndWait(LOCALID_GARET, ANIM_SHAKE_HEAD);
      FieldScript_ShowMessageThenWait(LOCALID_GARET, 0, 20);
    }
    Import_ObjectEventSetAnimation(LOCALID_MIA, 16);
    FieldScript_WaitFrames(30);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    ObjectEvent_WalkToAndWait(LOCALID_MIA, 840, 632);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                              0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTH + FACING_STEP, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_WEST, 0);
    FieldScript_WaitFrames(30);
    // Mia casts Ply on the door: a flash, the light turns white and the door
    // opens.
    MercuryLighthouse_MiaCastsPlyOnEntrance();
    FieldScript_WaitFrames(50);
    Sound_PlaySong(SE_131);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_CUSTOM | RGB(31, 20, 31), 0);
    PaletteFade_StartInterpolation(10);
    WaitFrames(1);
    Sound_PlaySong(SE_220);
    WaitFrames(40);
    PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
    PaletteFade_StartInterpolation(60);
    WaitFrames(60);
    Sound_PlaySong(SE_MERCURY_LIGHTHOUSE_DOOR_OPEN);
    MercuryLighthouse_FadePaletteToWhite();
    MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
    MapGrid_PlayMetatileCopySequence(gMercuryLighthouse_ColumnCells, 116, 35);
    Task_Remove(MercuryLighthouse_CycleEntrancePalette);
    FieldScript_WaitFrames(20);
    // She nods, jumps up and goes in.
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(3), PIXELS(3) / 2);
    ObjectMotion_Launch(LOCALID_MIA, 4, 0);
    ObjectEvent_WalkTo(LOCALID_MIA, 840, 600);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
    MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
    ObjectEvent_PrepareObject(PIXELS(840), PIXELS(56), PIXELS(608), 223);
    ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_MIA, 840, 560);
    Import_ObjectEventSetPosition(LOCALID_MIA, 0, 0);
    FieldScript_WaitFrames(20);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 0);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
    ObjectMotion_ArmCallback(LOCALID_IVAN, FACING_NORTHWEST + FACING_STEP, 0);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_NOD);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(20);
    // Garet and Ivan rejoin the leader.
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectMotion_ResetAndSetPosition(LOCALID_GARET, objectEvent->x.part.pixel,
                                       objectEvent->z.part.pixel);
    }
    Import_ObjectEventSetAnimation(LOCALID_IVAN, ANIM_WALK);
    objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
    if (objectEvent != 0) {
      ObjectEvent_SetDestination(LOCALID_IVAN, objectEvent->x.part.pixel,
                                 objectEvent->z.part.pixel);
    }
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    Import_ObjectEventSetPosition(LOCALID_GARET, 0, 0);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_IVAN);
    Import_ObjectEventSetPosition(LOCALID_IVAN, 0, 0);
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA);
  }
  FieldScript_End();
}

/// The waterfall shimmers: background palette 6, colors 1 to 7, rotates every
/// eight frames.
static void MercuryLighthouse_CycleWaterfallPalette(void) {
  MercuryLighthouse_RotatePaletteRange((u16 *)&BG_PLTT_COLOR(6, 1), 7);
}

/// The waterfall's colors fade to black as it stops.
static void MercuryLighthouse_FadePaletteToBlack(void) {
  // Palette RAM is hardware (its volatile is source, not a device): volatile
  // makes each read of palette RAM a load of its own: the native reads every
  // color with three ldrh, one per component, and again after the store;
  // without it one load is shifted for all three and the function is 98 bytes
  // for 100 in tbs-en.
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
      red = *palette & 31;
      green = (*palette >> 5) & 31;
      blue = (*palette >> 10) & 31;
      if (red > 0)
        red--;
      if (green > 0)
        green--;
      if (blue > 0)
        blue--;
      *palette = (blue << 10) | (green << 5) | red;
      if (*palette == 0)
        blackColors++;
      palette++;
    }
    WaitFrames(5);
  } while (blackColors != 7);
}

/// Raises an object event into the passage with sparks for 48 ticks, two frames
/// each. Its upward displacement accelerates by about 0.1 pixel per tick.
static void MercuryLighthouse_RaiseObjectEventWithSparks(s32 localId) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  struct ObjectEvent *objectEvent;
  u32 frame;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  objectEvent = ObjectEvent_GetById(localId);
  objectEvent->motionFlags = 0;
  ObjectEvent_SetSpriteFlags(objectEvent, 0);
  ObjectEvent_SetSpriteColorById(localId, 0x100);
  Sound_PlaySong(SE_221);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_GLINT;
  options->script = gMercuryLighthouse_SparkBurstScript;
  for (frame = 0; frame <= 47; frame++) {
    if (frame <= 31)
      MercuryLighthouse_SparkleNear(objectEvent->x.fixed, objectEvent->y.fixed,
                                    objectEvent->z.fixed);
    if (frame & 1) {
      Sound_PlaySong(SE_246);
      particleX = objectEvent->x.fixed +
                  ((((u32)Import_RandomNext() * 24) >> 16) << 16) + -PIXELS(12);
      particleY = objectEvent->y.fixed +
                  ((((u32)Import_RandomNext() << 5) >> 16) << 16) + -PIXELS(16);
      riseSpeed = ((((u32)Import_RandomNext() << 2) >> 16) << 15) + 0x8000;
      FieldEffect_Spawn(particleX, particleY, objectEvent->z.fixed, 0,
                        riseSpeed, 0,
                        EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                            EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE,
                        options);
    }
    objectEvent->y.fixed += frame * 0x1999;
    objectEvent->targetY = objectEvent->y.fixed;
    WaitFrames(2);
  }
}

/// MercuryLighthouseExterior, Ply on the waterfall statue: the waterfall
/// darkens and stops, sparkles rise and the way behind it opens. Once Saturos
/// is beaten the leader rises through it alone to the Lighthouse Aerie
/// (MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE); before that Mia
/// rises first and the party follows
/// (MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, the world map's
/// WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT, whose lighthouse transition ends at
/// the aerie).
static void MercuryLighthouse_PlyOpensWaterfallPassage(void) {
  struct EffectOptions *options;
  s32 rowOffset;
  u32 random;
  s32 zero;
  s32 wave;
  s32 particleIndex;
  s32 particleX;
  s32 particleVelocityX;
  struct EffectOptions optionsStorage;

  FieldScript_Start();
  FieldScript_WaitFrames(20);
  // Remove the colored waterfall before rebuilding the passage tiles.
  MercuryLighthouse_FadePaletteToBlack();
  Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
  MapGrid_CopyCellsTo(45, 77, 45, 73, 9, 4);
  FieldScript_WaitFrames(30);
  options = &optionsStorage;
  options->priority = 1;
  options->palette = 5;
  options->type = SPRITE_GLINT;
  options->script = gMercuryLighthouse_SparkBurstScript;
  zero = 0;
  wave = zero;
  // Two bursts spread sparks across the water before it begins to part.
  do {
    u32 particleX, z;

    if ((1 & wave) != 0) {
      Sound_PlaySong(SE_246);
    }
    random = Import_RandomNext();
    particleX = random * 48;
    particleX >>= 16;
    particleX <<= 16;
    particleX += PIXELS(768);
    random = Import_RandomNext();
    z = random * 56;
    z >>= 16;
    z <<= 16;
    z += PIXELS(136);
    FieldEffect_Spawn(particleX, 0, z, 0, 0, 0,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      options);
    FieldScript_WaitFrames(2);
    wave = (wave + 1);
  } while ((u32)wave <= 15);
  FieldScript_WaitFrames(40);
  zero = 0;
  wave = zero;
  do {
    u32 particleX, z;
    s32 particleSpeed;

    if ((1 & wave) != 0) {
      Sound_PlaySong(SE_246);
    }
    random = Import_RandomNext();
    particleX = random * 48;
    particleX >>= 16;
    particleX <<= 16;
    particleX += PIXELS(768);
    random = Import_RandomNext();
    z = random * 56;
    z >>= 16;
    z <<= 16;
    z += PIXELS(152);
    random = Import_RandomNext();
    particleSpeed = -((random * 10 >> 16) * 0x3333) - 0x3333;
    FieldEffect_Spawn(particleX, 0, z, 0, 0, particleSpeed,
                      EFFECT_USE_SCRIPT | EFFECT_USE_TYPE |
                          EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                          EFFECT_SCRIPT_MEDIUM,
                      options);
    FieldScript_WaitFrames(2);
    wave = (wave + 1);
  } while ((u32)wave <= 15);
  FieldScript_WaitFrames(60);
  Sound_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  FieldScript_WaitFrames(60);
  options->palette = 7;
  options->startScaleX = 7 * Q16_ONE / 10;
  options->startScaleY = 7 * Q16_ONE / 10;
  options->targetScaleX = 6 * Q16_ONE / 5;
  options->targetScaleY = 6 * Q16_ONE / 5;
  zero = 0;
  wave = zero;
  // Reveal the opening one map row at a time, trailing a stream of particles.
  do {
    s32 particleSpeed;

    MapGrid_CopyCellsTo(59, (12 - wave), 48, (12 - wave), 3, 1);
    particleIndex = 0;
    rowOffset = (wave << 4);
    do {
      random = Import_RandomNext();
      particleX =
          ((((((random << 1) + random) << 4) >> 16) << 16) + PIXELS(768));
      particleVelocityX =
          (0x1999 * ((u32)(Import_RandomNext() << 3) >> 16)) - 0x6664;
      particleSpeed = 0x1999 * ((u32)(Import_RandomNext() << 3) >> 16);
      FieldEffect_Spawn(particleX, 0,
                        ((-((u32)particleIndex >> 1) - rowOffset) << 16) +
                            PIXELS(192),
                        particleVelocityX, 0, particleSpeed,
                        EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET |
                            EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                        options);
      particleIndex = (particleIndex + 1);
      FieldScript_WaitFrames(2);
    } while ((u32)particleIndex <= 31);
    wave = (wave + 1);
  } while ((u32)wave <= 3);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
  MapGrid_WaitForShakeToSettle();
  Map_PlayQueuedMusic();
  FieldScript_WaitFrames(30);
  // After Saturos the leader goes alone; the first time Mia leads the party.
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 824, 232);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 232);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 208);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
    ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
    ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 792, 200);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 18);
    MercuryLighthouse_RaiseObjectEventWithSparks(LOCALID_PARTY_LEADER);
    FieldScript_WaitFrames(60);
    Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
    PaletteFade_ApplyTarget(PALETTE_TINT_PALE, 0);
    PaletteFade_StartInterpolation(120);
    FieldScript_WaitFrames(120);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_StartInterpolation(60);
    FieldScript_WaitFrames(60);
    Import_ScriptRequestWarp(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE);
    FieldScript_End();
  } else {
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_GARET, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_IVAN, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 824, 240);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTHWEST, 20);
    ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(824), PIXELS(240));
    ObjectEvent_WalkToAndWait(LOCALID_MIA, 792, 232);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
    FieldScript_WaitFrames(60);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_SOUTHEAST, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(40);
    ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 20);
    ObjectEvent_WalkToAndWait(LOCALID_MIA, 792, 200);
    MercuryLighthouse_RaiseObjectEventWithSparks(LOCALID_MIA);
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 2);
    FieldScript_WaitFrames(30);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 232);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_NORTH, 0);
    ObjectEvent_SetPosition(LOCALID_GARET, PIXELS(792), PIXELS(232));
    ObjectEvent_SetPosition(LOCALID_IVAN, PIXELS(792), PIXELS(232));
    ObjectEvent_WalkTo(LOCALID_GARET, 816, 224);
    ObjectEvent_WalkToAndWait(LOCALID_IVAN, 768, 224);
    ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_GARET);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 20);
    ObjectEvent_ShowEmote(LOCALID_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE,
                          0);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_SOUTHWEST, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_SOUTHEAST, 20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_PARTY_LEADER, 1);
    FieldScript_WaitFrames(60);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 224);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_WEST, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_EAST, 0);
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 792, 208);
    ObjectEvent_FaceDirection(LOCALID_GARET, FACING_NORTHWEST, 0);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
    FieldScript_WaitFrames(20);
    ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(2), PIXELS(1));
    ObjectMotion_Launch(LOCALID_PARTY_LEADER, 4, 0);
    ObjectEvent_WalkTo(LOCALID_PARTY_LEADER, 792, 200);
    FieldScript_WaitFrames(10);
    Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, 18);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
    ObjectMotion_SetVariantCallback(LOCALID_GARET, 2);
    ObjectMotion_SetVariantCallback(LOCALID_IVAN, 2);
    MercuryLighthouse_RaiseObjectEventWithSparks(LOCALID_PARTY_LEADER);
    FieldScript_WaitFrames(60);
    ObjectEvent_ShowEmote(LOCALID_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
    ObjectEvent_ShowEmote(LOCALID_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
    ObjectEvent_LinkPair(LOCALID_GARET, LOCALID_IVAN, 20);
    Import_ObjectEventSetAnimation(LOCALID_GARET, ANIM_NOD);
    ObjectMotion_SetAnimationAndWait(LOCALID_IVAN, ANIM_NOD);
    FieldScript_WaitFrames(40);
    ObjectEvent_WalkToAndWait(LOCALID_GARET, 792, 216);
    ObjectEvent_FaceDirection(LOCALID_IVAN, FACING_NORTHEAST, 0);
    ObjectEvent_WalkToAndWait(LOCALID_GARET, 792, 200);
    FieldScript_WaitFrames(30);
    MercuryLighthouse_RaiseObjectEventWithSparks(LOCALID_GARET);
    ObjectEvent_WalkToAndWait(LOCALID_IVAN, 792, 216);
    ObjectEvent_WalkToAndWait(LOCALID_IVAN, 792, 200);
    FieldScript_WaitFrames(30);
    MercuryLighthouse_RaiseObjectEventWithSparks(LOCALID_IVAN);
    PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
    Task_Remove(MercuryLighthouse_CycleWaterfallPalette);
    PaletteFade_ApplyTarget(PALETTE_TINT_PALE, 0);
    PaletteFade_StartInterpolation(120);
    FieldScript_WaitFrames(120);
    PaletteFade_ApplyTarget(0x7fff, 0);
    PaletteFade_StartInterpolation(60);
    FieldScript_WaitFrames(60);
    FieldScript_End();
    Import_ScriptRequestWarp(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT);
  }
}

/// MercuryLighthouseExterior, searching metatile behavior 42 shows the
/// fountain's sign. "Fountain of Healing / Water of Hermes: It brings
/// rejuvenation to all that lives."
static void MercuryLighthouse_ReadHermesFountainInscription(void) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgImilHermesHealingWaterFountain), 1);
  FieldScript_End();
}

/// MercuryLighthouseMiddle or MercuryLighthouseUpper, searching metatile
/// behavior 20: the door is locked tight.
static void MercuryLighthouse_InspectLockedDoor(void) {
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
  FieldScript_End();
}

/// MercuryLighthouseExterior, searching the Fountain of Healing (metatile
/// behavior 5) before taking its water: it seems dry, or once Saturos is beaten
/// flows with water; carrying an Empty Bottle, the item menu then opens.
static void MercuryLighthouse_InspectHealingFountain(void) {

  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  if (GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY) == 0)
    FieldMessage_Show(MSGNO(MsgMercuryLighthouseFountainSeemsDry), 1);
  else
    FieldMessage_Show(MSGNO(MsgMercuryLighthouseFountainFlowsWithWater), 1);
  if (PartyInventory_FindMember(ITEM_EMPTY_BOTTLE) != -1) {
    // The field loop handles this menu request after the inspection ends.
    s16 *menuRequest = &gFieldWork->menu;
    s32 enabled = 1;

    *menuRequest = enabled;
  }
  FieldScript_End();
}

/// MercuryLighthouseExterior, using the Empty Bottle at the fountain
/// (USE_ITEM(ITEM_EMPTY_BOTTLE)) once Saturos is beaten: the bottle becomes
/// Hermes' Water, once a visit.
static void MercuryLighthouse_FillHermesWaterBottle(void) {
  FieldScript_Start();
  Sound_PlaySong(MUS_TREASURE_FOUND);
  ItemDisplay_ShowItem(ITEM_HERMES_WATER, 3);
  MercuryLighthouse_ReplaceInventoryItem(ITEM_EMPTY_BOTTLE, ITEM_HERMES_WATER);
  MessageArgument_Push(PartyInventory_FindMember(ITEM_HERMES_WATER), 1);
  MessageArgument_Push(ITEM_HERMES_WATER, 2);
  FieldMessage_Show(MSGNO(MsgMercuryLighthouseIsaacGot), 1);
  GameFlag_Set(FLAG_HERMES_WATER_TAKEN);
  FieldScript_End();
}

/// MercuryLighthouseLower or MercuryLighthouseMiddle, walking south into
/// metatile behavior 15: on the first the leader steps to the hole and drops
/// into MercuryLighthouseEntrance
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE, its
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE); on the
/// second the motion runs alone.
static void MercuryLighthouse_EnterRisingPassage(void) {
  FieldScript_Start();
  if (gSaveBlock.mapId == (s32)&MapId_MercuryLighthouseLower) {
    ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 472, 600);
    ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_SOUTH, 10);
    FieldCamera_MoveTo(PIXELS(464), -1, PIXELS(656), 1);
    ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
    ObjectEvent_PrepareObject(
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->x.fixed, 0, PIXELS(702),
        223);
    MapGrid_CopyCellsTo(92, 46, 92, 40, 3, 2);
    ObjectEvent_GetById(LOCALID_PARTY_LEADER)->gravity = 0x8000;
    ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 2);
    FieldObjectEvent_JumpDownLedge(LOCALID_PARTY_LEADER, 6, -1);
    gFieldWork->transition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    FieldScript_WaitFrames(60);
    Import_ScriptRequestWarp(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE);
  } else {
    FieldObjectEvent_JumpDownLedge(LOCALID_PARTY_LEADER, 6, -1);
  }
  FieldScript_End();
}

#include "common/mercury_lighthouse_arrive.h"

/// Raises the stairs: their cells are redrawn row by row, two rows, with
/// sparkles rising along their edge.
static void MercuryLighthouse_RaiseStairsWithSparkles(void) {
  struct EffectOptions options;
  u32 particleIndex, row;

  MapGrid_CopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
  options.palette = 7;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = Q16_ONE / 2;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        s32 velocityX =
            (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
        s32 velocityZ =
            (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

        FieldEffect_Spawn(
            PIXELS(105), 0, ((-particleIndex - (row << 4)) << 16) + PIXELS(544),
            velocityX, 0, velocityZ,
            EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
        FieldScript_WaitFrames(1);
      }
    }
    MapGrid_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - row, 1, 1);
    MapGrid_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - row, 1, 1);
  }
}

/// Lowers the stairs as the stair pipe moves away (the push's callback):
/// three steps of cells redrawn, each in a scatter of sparkles.
static void MercuryLighthouse_LowerStairsWithSparkles(void) {
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;
  u32 row;
  u32 particleIndex;
  s32 offsetZ;

  MapGrid_CopyCellsTo(76, 61, 74, 38, 1, 1);
  options = &optionsStorage;
  options->palette = 5;
  options->startScaleX = Q16_ONE / 2;
  options->startScaleY = Q16_ONE / 2;
  for (row = 0; row <= 2; row++) {
    offsetZ = -PIXELS(2);
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (particleIndex & 2) {
          FieldEffect_Spawn(
              (105 - (((u32)Import_RandomNext() * 5) >> 16)) << 16, 0,
              offsetZ - (row << 19) + PIXELS(558), 0, 0, -0x4000,
              EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        } else {
          FieldEffect_Spawn(
              (((row << 2) + particleIndex) << 17) + PIXELS(183), 0,
              (0x26c - (((u32)Import_RandomNext() * 5) >> 16)) << 16, 0x4000, 0,
              0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
        }
        FieldScript_WaitFrames(1);
      }
      offsetZ += -PIXELS(2);
    }
    MapGrid_CopyCellsTo(71, 59, 70, 34 - row, 1, 1);
    MapGrid_CopyCellsTo(71, 59, row + 75, 38, 1, 1);
  }
}

/// Task: a sparkle from the leader every fourth frame, as he is
/// carried to a landing.
static void MercuryLighthouse_SparkleAroundLeader(void) {
  struct EffectOptions options;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  s32 framePhase = gFrameCount & 3;

  if (framePhase == 0) {
    options.palette = 7;
    if (((((u32)Import_RandomNext() << 1) >> 16) & 1) == 0)
      options.palette = 5;
    options.startScaleX = 7 * Q16_ONE / 10;
    options.startScaleY = 7 * Q16_ONE / 10;
    {
      s32 y = leader->y.fixed + ((((u32)Import_RandomNext() << 2) >> 16) << 16);
      s32 velocityX =
          (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
      s32 velocityY =
          (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

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
    options.palette = 7;
    if (((((u32)Import_RandomNext() << 1) >> 16) & 1) == 0)
      options.palette = 5;
    options.startScaleX = 7 * Q16_ONE / 10;
    options.startScaleY = 7 * Q16_ONE / 10;
    {
      s32 velocityX =
          (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
      s32 velocityY =
          (((u32)Import_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

      FieldEffect_Spawn(x, y, z, velocityX, velocityY, framePhase,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
    }
  }
}

/// One still sparkle near a point, up to 8 pixels either side and 4 north or
/// south.
static void MercuryLighthouse_SparkleNear(s32 x, s32 y, s32 z) {
  struct EffectOptions options;

  options.palette = 7;
  options.priority = 1;
  options.startScaleX = 7 * Q16_ONE / 10;
  options.startScaleY = 7 * Q16_ONE / 10;
  {
    s32 particleX =
        x + ((((u32)Import_RandomNext() << 4) >> 16) << 16) + 0xfff80000;
    s32 particleZ =
        z + ((((u32)Import_RandomNext() << 3) >> 16) << 16) + 0xfffc0000;

    FieldEffect_Spawn(particleX, y, particleZ, 0, 0, 0,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE,
                      &options);
  }
}

/// Six sparkles over 24 frames, spraying the way angle points, as the water
/// runs through a joined pipe.
static void MercuryLighthouse_SpawnRandomizedBurst(s32 x, s32 y, s32 z,
                                                   s32 angle) {
  struct EffectOptions options;
  s32 velocity[3];
  u32 particleIndex;

  Sound_PlaySong(SE_216);
  particleIndex = 0;
  do {
    if ((particleIndex & 1) != 0) {
      options.palette = 7;
      if ((particleIndex & 2) != 0)
        options.palette = 5;
      options.startScaleX = 3 * Q16_ONE / 5;
      options.startScaleY = 3 * Q16_ONE / 5;
      SET_VECTOR(velocity, 0, 0, 0);
      Vector_AddPolarOffset((6 - (particleIndex >> 1)) * 0x1999, angle,
                            velocity);
      {
        s32 particleX =
            x + ((6 - (((u32)Import_RandomNext() * 6) >> 16)) << 16);
        s32 particleZ =
            z + ((6 - (((u32)Import_RandomNext() * 6) >> 16)) << 16);

        FieldEffect_Spawn(
            particleX, y, particleZ, velocity[0], velocity[1], velocity[2],
            EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
      }
    }
    WaitFrames(2);
    particleIndex++;
  } while (particleIndex <= 11);
}

/// Restores each lighthouse map's puzzle state and its warp destination's
/// sequence.
s32 MercuryLighthouse_Initialize(void) {
  u32 statueIndex;
  s32 positionFlag;
  s32 pillarX;
  s32 keepPartyPosition;

  GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_LIT);
  // Set up common lighting before restoring each map's puzzle-specific state.
  gSaveBlock.retreatWarpDest = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gSaveBlock.retreatMap = (s32)&MapId_MercuryLighthouseExterior;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Io_Write16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
  {
    s32 alpha = BLEND_ALPHA(16, 16);
    REG_BLDALPHA = alpha;
  }
  MercuryLighthouse_RestoreLights(
      21, (struct MercuryLighthouseLights *)gMapLocalState);
  FieldEffect_StartFadeOverlay(0);
  if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_MAP);
  else
    MercuryLighthouse_ClearPalette();
  if (gSaveBlock.mapId == (s32)&MapId_MercuryLighthouseLower) {
    // Entrance pillars, the channel puzzle and Mia's rescue share this overlay.
    switch (gSaveBlock.warpDest) {
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST:
    case MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE:
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
        MapGrid_CopyCellAttributes(66, 5, 27, 23, 2, 5);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH:
      Map_QueueMusic(SE_170);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE:
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST:
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN)) {
        WaitFrames(1);
        ObjectEvent_SetPosition(LOCALID_FIRST_PIPE, PIXELS(672), PIXELS(128));
        ObjectEvent_SetPosition(LOCALID_SECOND_PIPE, PIXELS(704), PIXELS(192));
        ObjectEvent_SetPosition(LOCALID_THIRD_PIPE, PIXELS(648), PIXELS(240));
      } else if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        GameFlag_Clear(FLAG_PIPE_1_JOINED);
        GameFlag_Clear(FLAG_PIPE_2_JOINED);
        GameFlag_Clear(FLAG_PIPE_3_JOINED);
        GameFlag_Clear(FLAG_UNUSED_PIPE_4_JOINED);
      }
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FIRST_PIPE), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_SECOND_PIPE), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_THIRD_PIPE), 1);
      Map_RedrawObjectEventFootprint(LOCALID_FIRST_PIPE);
      Map_RedrawObjectEventFootprint(LOCALID_SECOND_PIPE);
      Map_RedrawObjectEventFootprint(LOCALID_THIRD_PIPE);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_PIPE_END), 1);
      if (GameFlag_Get(FLAG_PIPE_1_JOINED)) {
        MapGrid_CopyCellsTo(119, 9, 109, 11, 1, 1);
        if (GameFlag_Get(FLAG_PIPE_2_JOINED))
          MapGrid_CopyCellsTo(118, 9, 104, 13, 1, 1);
        MapGrid_CenterOnFollowedPoint();
        WaitFrames(1);
      }
      break;
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL:
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS:
      if (!GameFlag_Get(FLAG_MIA_JOINED_PARTY)) {
        ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(696), PIXELS(632));
        ObjectMotion_ArmCallback(LOCALID_MIA, FACING_EAST, 0);
      } else {
        ObjectEvent_SetPosition(LOCALID_STATUE_BLOCKING_MIA, PIXELS(776),
                                PIXELS(632));
        MapGrid_CopyCellAttributes(110, 39, 5, 1, 46, 39);
      }
      break;
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_MercuryLighthouseMiddle) {
    // Restore stairs, the floor-switch gate and the fountain's moved pipe.
    switch (gSaveBlock.warpDest) {
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH:
    case MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE:
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_STAIR_PIPE), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_STAIR_PIPE_END), 1);
      if (GameFlag_Get(FLAG_STAIRS_RAISED)) {
        WaitFrames(1);
        Sound_PlaySong(SE_211);
        ObjectEvent_SetPosition(LOCALID_STAIR_PIPE, PIXELS(184), PIXELS(528));
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
      WaitFrames(1);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          GameFlag_Get(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
        MapGrid_CopyCellsTo(9, 1, 9, 7, 1, 2);
      }
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN)) {
        Import_ObjectEventSetPosition(LOCALID_GATE_STATUE, PIXELS(88),
                                      PIXELS(216));
        ObjectEvent_GetById(LOCALID_GATE_STATUE)->y.fixed += -PIXELS(2);
        ObjectEvent_GetById(LOCALID_GATE_STATUE)->targetY =
            ObjectEvent_GetById(LOCALID_GATE_STATUE)->y.fixed;
        MapGrid_CopyCellsTo(9, 1, 9, 7, 1, 2);
        MapGrid_CopyCellsTo(5, 2, 5, 11, 1, 1);
        MapGrid_CopyCellAttributes(9, 5, 1, 1, 9, 10);
      }
#if TBS_PATCH_3
      FieldScheduler_AddCallback(MercuryLighthouse_UpdateLeaderEffectTarget,
                                 TASK_PRIORITY_MAP);
#endif
      break;
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST:
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FOUNTAIN_PIPE), 1);
      if (GameFlag_Get(FLAG_FOUNTAIN_RUNNING)) {
        MercuryLighthouse_OpenFountainWithSpray(0);
        MapGrid_CopyCellAttributes(42, 41, 4, 1, 42, 39);
        MapGrid_CopyCellAttributes(42, 40, 4, 1, 42, 41);
        ObjectEvent_SetPosition(LOCALID_FOUNTAIN_PIPE, PIXELS(704),
                                PIXELS(640));
      }
      break;
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_MercuryLighthouseUpper) {
    // Restore the movable pipes and the stone doors they opened.
    switch (gSaveBlock.warpDest) {
    case MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_GATE:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST:
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FIRST_CROSSING_STATUE),
                          1);
      SetEffectRecordMode(
          ObjectEvent_GetById(LOCALID_FIRST_CROSSING_STATUE + 1), 1);
      SetEffectRecordMode(
          ObjectEvent_GetById(LOCALID_FIRST_CROSSING_STATUE + 2), 1);
      SetEffectRecordMode(
          ObjectEvent_GetById(LOCALID_FIRST_CROSSING_STATUE + 3), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_LAST_CROSSING_STATUE), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
      statueIndex = 0;
      pillarX = PIXELS(632);
      positionFlag = FLAG_FIRST_ROW_STATUE;
      for (; statueIndex <= 3; statueIndex++) {
        if (GameFlag_Get(positionFlag))
          Import_ObjectEventSetPosition(
              statueIndex + LOCALID_FIRST_CROSSING_STATUE, pillarX, PIXELS(88));
        else if (GameFlag_Get(positionFlag + 1))
          Import_ObjectEventSetPosition(statueIndex +
                                            LOCALID_FIRST_CROSSING_STATUE,
                                        pillarX + PIXELS(32), PIXELS(88));
        pillarX += PIXELS(64);
        positionFlag += 2;
      }
      if (GameFlag_Get(FLAG_LAST_ROW_STATUE_WEST)) {
        ObjectEvent_SetPosition(LOCALID_LAST_CROSSING_STATUE, PIXELS(920),
                                PIXELS(88));
        MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      } else if (GameFlag_Get(FLAG_LAST_ROW_STATUE_EAST)) {
        ObjectEvent_SetPosition(LOCALID_LAST_CROSSING_STATUE, PIXELS(952),
                                PIXELS(88));
        MapGrid_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST:
    case MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS:
      if (GameFlag_Get(FLAG_PASSAGE_STATUE_MOVED)) {
        ObjectEvent_SetPosition(LOCALID_GAP_STATUE, PIXELS(456), PIXELS(328));
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
      Map_QueueMusic(SE_170);
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER:
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_FIVE_PILLARS:
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS:
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
          GameFlag_Get(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed = -PIXELS(2);
        ObjectEvent_GetById(LOCALID_PARTY_LEADER)->targetY =
            ObjectEvent_GetById(LOCALID_PARTY_LEADER)->y.fixed;
        MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
        MapGrid_CopyCellsTo(10, 28, 10, 18, 1, 2);
      }
      if (GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN)) {
        Import_ObjectEventSetPosition(LOCALID_FALLEN_STATUE, PIXELS(168),
                                      PIXELS(376));
        ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->y.fixed += -PIXELS(2);
        ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->targetY =
            ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->y.fixed;
        MapGrid_CopyCellsTo(6, 29, 10, 23, 1, 1);
        MapGrid_CopyCellsTo(10, 28, 10, 18, 1, 2);
        MapGrid_CopyCellAttributes(10, 16, 1, 1, 10, 19);
        MapGrid_CenterOnFollowedPoint();
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE_STATUE_FALLS:
      WaitFrames(1);
#if TBS_PATCH_2
      if (GameFlag_Get(FLAG_DOOR_ROOM_PIPE_PUSHED))
        ObjectEvent_SetPosition(LOCALID_DOOR_ROOM_PIPE, PIXELS(816),
                                PIXELS(608));
      if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR))
        ObjectEvent_SetPosition(LOCALID_DOOR_SWITCH_PIPE, PIXELS(776),
                                PIXELS(576));
      else if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR))
        ObjectEvent_SetPosition(LOCALID_DOOR_SWITCH_PIPE, PIXELS(760),
                                PIXELS(576));
      else if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS))
        ObjectEvent_SetPosition(LOCALID_DOOR_SWITCH_PIPE, PIXELS(856),
                                PIXELS(576));
#else
      ObjectEvent_SetPosition(LOCALID_DOOR_ROOM_PIPE, PIXELS(816), PIXELS(608));
      Import_ObjectEventSetPosition(LOCALID_DOOR_SWITCH_PIPE, PIXELS(776),
                                    PIXELS(576));
#endif
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
      MercuryLighthouse_CloseStoneDoor(STONE_DOOR_WEST);
#if TBS_PATCH_2
      if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_NORTH);
        if (ObjectEvent_GetById(LOCALID_SLIDING_STATUE)->z.fixed >> 20 == 44)
          FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue,
                                     TASK_PRIORITY_MAP);
      } else if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_SOUTH);
      } else if (!GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_WEST);
      }
#else
      MercuryLighthouse_OpenStoneDoor(STONE_DOOR_SOUTH);
#endif
      // Fall through.
    case MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE:
      if (gSaveBlock.warpDest == MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE)
        Sound_PlaySong(SE_211);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_SLIDING_STATUE), 1);
      ObjectMotion_SetActionVariant(LOCALID_DOOR_ROOM_PIPE, 2);
      ObjectEvent_GetById(LOCALID_DOOR_ROOM_PIPE)->layer = 2;
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_DOOR_SWITCH_PIPE), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FIRST_DOOR_PIPE_END), 1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FIRST_DOOR_PIPE_END + 1),
                          1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FIRST_DOOR_PIPE_END + 2),
                          1);
      Map_RedrawObjectEventFootprint(LOCALID_DOOR_ROOM_PIPE);
      Map_RedrawObjectEventFootprint(LOCALID_DOOR_SWITCH_PIPE);
      if (gSaveBlock.warpDest != MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE)
        break;
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        GameFlag_Clear(FLAG_DOOR_ROOM_PIPE_PUSHED);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR);
        GameFlag_Clear(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS);
        break;
      }
      MercuryLighthouse_CloseStoneDoor(STONE_DOOR_WEST);
      if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_NORTH_DOOR)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_NORTH);
        if (ObjectEvent_GetById(LOCALID_SLIDING_STATUE)->z.fixed >> 20 == 44)
          FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue,
                                     TASK_PRIORITY_MAP);
      } else if (GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_OPENS_SOUTH_DOOR)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_SOUTH);
      } else if (!GameFlag_Get(FLAG_DOOR_SWITCH_PIPE_SHUTS_DOORS)) {
        MercuryLighthouse_OpenStoneDoor(STONE_DOOR_WEST);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS_STATUE_FALLS:
      FieldScript_Start();
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FALLEN_STATUE), 1);
      ObjectEvent_ConfigureSpriteColor(LOCALID_PARTY_LEADER, 15);
      ObjectEvent_SetSpriteFlags(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 0);
      ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->y.fixed = PIXELS(144);
      ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->motionFlags = 0;
      ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->bounce = 0;
      ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->gravity = 0x4ccc;
      gFieldWork->transition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
      FieldScript_StartScreenTransition();
      FieldScript_WaitTransitionFrames();
      ObjectEvent_GetById(LOCALID_FALLEN_STATUE)->motionFlags = 3;
      Sound_PlaySong(SE_189);
      FieldScript_WaitFrames(32);
      Sound_PlaySong(SE_188);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_FALLEN_STATUE), 2);
      MapGrid_SetShake(192 << 10, 192 << 10, 128 << 9);
      MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
      MapGrid_WaitForShakeToSettle();
      FieldScript_WaitFrames(60);
      Import_ScriptRequestWarp(
          MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS_STATUE_FALLS);
      FieldScript_End();
      break;
    }
  }
  if (gSaveBlock.mapId == (s32)&MapId_MercuryLighthouseExterior) {
    // This map contains Mia's entrance, the lift and the healing fountain.
    switch (gSaveBlock.warpDest) {
    case MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH:
      FieldScheduler_AddCallback(MercuryLighthouse_CycleWaterfallPalette,
                                 TASK_PRIORITY_MAP);
      if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION))
        break;
      MercuryLighthouse_ArriveWithSparks();
      Map_QueueMusic(SE_170);
      PaletteFade_ApplySource(128 << 9, 0);
      PaletteFade_SetTarget(0x10003, 1);
      PaletteFade_StartInterpolation(30);
      FieldScript_WaitTransitionFrames();
      Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
      FieldScript_WaitFrames(30);
      FieldMessage_ShowCentered(MsgMercuryHonorsGoddessRainbows, 0, 0);
      PaletteFade_SetTarget(128 << 9, 0);
      PaletteFade_StartInterpolation(30);
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE:
      Import_ObjectEventSetPosition(LOCALID_MIA, 0, 0);
      BlendScript_Suspend();
      Io_Write16(0, &REG_BLDCNT);
      ObjectEvent_GetById(LOCALID_EAST_APPROACH_STATUE)->scaleX = -Q16_ONE;
      ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR, 1);
      ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR + 1, 1);
      ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR + 2, 1);
      keepPartyPosition = GameFlag_Get(FLAG_KEEP_PARTY_POSITION);
      if (keepPartyPosition) {
        MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
        MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
        break;
      }
      ObjectEvent_SetPosition(LOCALID_DOOR_STATUE, PIXELS(824), PIXELS(600));
      MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
      ObjectEvent_GetById(LOCALID_DOOR_STATUE)->motionFlags = keepPartyPosition;
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_DOOR_STATUE), 1);
      FieldScript_Start();
      FieldCamera_MoveTo(-1, -1, -1, 0);
      FieldCamera_GetScriptObject()->motionFlags = keepPartyPosition;
      ObjectMotion_SetActionVariant(LOCALID_PARTY_LEADER, 1);
      ObjectMotion_SetActionVariant(LOCALID_LIFT_STATUE, 1);
      MercuryLighthouse_RideLift();
      BlendScript_Resume();
      FieldScript_End();
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE:
      FieldScript_WaitFrames(1);
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP:
      MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
      MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
      if (!GameFlag_Get(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        MapGrid_CopyCellsTo(30, 45, 50, 45, 5, 6);
        MapGrid_CopyCellsTo(50, 105, 50, 109, 5, 3);
        MapGrid_CenterOnFollowedPoint();
        WaitFrames(1);
      } else {
        ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR, 1);
        ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR + 1, 1);
        ObjectMotion_SetActionVariant(LOCALID_FIRST_VISITOR + 2, 1);
      }
      ObjectEvent_GetById(LOCALID_EAST_APPROACH_STATUE)->scaleX = -Q16_ONE;
      // Until Mia has hurried here from Imil (FLAG_IMIL_MET_MIA, set in
      // imil.c), she is not here and the door stays shut.
      if (!GameFlag_Get(FLAG_IMIL_MET_MIA)) {
        Import_ObjectEventSetPosition(LOCALID_MIA, 0, 0);
        ObjectEvent_SetPosition(LOCALID_DOOR_STATUE, PIXELS(824), PIXELS(600));
        ObjectMotion_SetActionVariant(LOCALID_DOOR_STATUE, 1);
        MapGrid_CopyCellAttributes(116, 37, 3, 3, 52, 37);
        MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
        FieldScheduler_AddCallback(MercuryLighthouse_CycleEntrancePalette,
                                   TASK_PRIORITY_MAP);
        break;
      }
      if (!GameFlag_Get(FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA)) {
        if (!GameFlag_Get(FLAG_MIA_SENSED_SOMEONE_INSIDE)) {
          ObjectMotion_ArmCallback(LOCALID_MIA, FACING_EAST, 0);
          Import_ObjectEventSetAnimation(LOCALID_MIA, 16);
        } else {
          ObjectEvent_SetPosition(LOCALID_MIA, PIXELS(840), PIXELS(632));
          ObjectEvent_FaceDirection(LOCALID_MIA, FACING_NORTH, 0);
        }
        SetEffectRecordMode(ObjectEvent_GetById(LOCALID_MIA), 1);
        MapGrid_CopyCellsTo(126, 35, 116, 35, 1, 2);
        FieldScheduler_AddCallback(MercuryLighthouse_CycleEntrancePalette,
                                   TASK_PRIORITY_MAP);
      } else {
        Import_ObjectEventSetPosition(LOCALID_MIA, 0, 0);
        ObjectEvent_SetPosition(LOCALID_DOOR_STATUE, PIXELS(824), PIXELS(600));
        MapGrid_CopyCellAttributes(116, 36, 3, 4, 52, 36);
        ObjectEvent_GetById(LOCALID_DOOR_STATUE)->collisionFlags = 254;
        FieldScript_WaitFrames(1);
      }
      ObjectEvent_GetById(LOCALID_DOOR_STATUE)->motionFlags = 0;
      SetEffectRecordMode(ObjectEvent_GetById(LOCALID_DOOR_STATUE), 1);
      break;
    }
  }
  return 0;
}

/// MercuryLighthouseLower, Move on the statue blocking Mia, before she has
/// joined: once it stands at column 48, Mia thanks the party, asks whether it
/// has business in the lighthouse and presses until it says yes, then joins
/// with Fizz. "You've saved me again, haven't you!"
static void MercuryLighthouse_MiaJoinsAfterRescue(void) {
  s32 firstMessage;
  s32 pillarColumn;
  s32 questionMessage;
  struct ObjectEvent *leader;

  pillarColumn =
      ObjectEvent_Get(LOCALID_STATUE_BLOCKING_MIA)->x.fixed / PIXELS(16);
  if (pillarColumn != 48) {
    return;
  }
  FieldScript_Start();
  firstMessage = MsgMercurySavedMeAgain;
  FieldScript_SetMessageId(firstMessage);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 1);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 20);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  ObjectMotion_SetAnimationAndWait(LOCALID_PARTY_LEADER, ANIM_NOD);
  FieldScript_WaitFrames(20);
  FieldScript_WaitFrames(60);
  Import_ObjectEventSetAnimation(LOCALID_MIA, 16);
  FieldScript_WaitFrames(50);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  FieldMessage_ShowAtObject(LOCALID_MIA, 0);
  if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
    // Each "no" brings "Please, don't hide anything." until the party says
    // yes.
    FieldScript_WaitFrames(20);
    ObjectMotion_SetVarCbAndRefresh(LOCALID_MIA, 2);
    FieldScript_WaitFrames(20);
    Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
    ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
    FieldScript_WaitFrames(20);
    FieldMessage_ShowAtObject(LOCALID_MIA, 0);
    if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) == 1) {
      FieldScript_WaitFrames(20);
      /* FAKEMATCH(wrapper): called through an unprototyped cast, which loads
       * r1 before r0; called by its prototype, 1 of 222 instructions differ in
       * tbs-en. */
      MERCURY_LIGHTHOUSE_ANIMATE_OBJ_EVENT_AND_WAIT(LOCALID_MIA,
                                                    ANIM_SHAKE_HEAD);
      FieldScript_WaitFrames(20);
      questionMessage = firstMessage + 5;
      for (;;) {
        FieldScript_SetMessageId(questionMessage);
        FieldMessage_ShowAtObject(LOCALID_MIA, 0);
        if (FieldScript_AskYesNoWithObjectEventAnimation(0, 0) != 1) {
          break;
        }
        FieldScript_WaitFrames(20);
        /* FAKEMATCH(wrapper): called through an unprototyped cast, which
         * loads r1 before r0; called by its prototype, 1 of 222 instructions
         * differ in tbs-en. */
        MERCURY_LIGHTHOUSE_ANIMATE_OBJ_EVENT_AND_WAIT(LOCALID_MIA,
                                                      ANIM_SHAKE_HEAD);
        FieldScript_WaitFrames(20);
        questionMessage = MsgMercuryDontHideTruth;
      }
    }
  }
  FieldScript_SetMessageId(MsgMercuryThoughtSo);
  ObjectEvent_SetSpeed(LOCALID_MIA, PIXELS(4) / 5, PIXELS(2) / 5);
  ObjectMotion_SetPositionAndReset(LOCALID_MIA, 728, 632);
  FieldScript_WaitFrames(20);
  FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_MIA, 16);
  FieldScript_ShowMessageThenWait(LOCALID_MIA, 0, 20);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_STAND);
  ObjectMotion_SetAngleToward(LOCALID_MIA, LOCALID_PARTY_LEADER, 20);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_SHAKE_HEAD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  ObjectEvent_ShowEmote(LOCALID_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 90);
  ObjectMotion_SetAnimationAndWait(LOCALID_MIA, ANIM_NOD);
  FieldScript_WaitFrames(20);
  Script_ShowMessageAndWait(LOCALID_MIA, 0, 20);
  FieldScript_PrepareObjectAndApplyValue(MIA, 1);
  // "The Adept Mia joined your party!" Fizz, a Mercury Djinni, comes with
  // her and is set; her stats are worked out again.
  GameFlag_Set(FLAG_DJINNI(DJINNI_FIZZ));
  Djinn_Give(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Djinn_Set(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Unit_Calculate(MIA);
  Import_ObjectEventSetAnimation(LOCALID_MIA, ANIM_WALK);
  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (leader != 0) {
    ObjectMotion_ResetAndSetPosition(LOCALID_MIA, leader->x.part.pixel,
                                     leader->z.part.pixel);
  }
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_MIA);
  Import_ObjectEventSetPosition(LOCALID_MIA, 0, 0);
  MapGrid_CopyCellAttributes(110, 39, 5, 1, 46, 39);
  GameFlag_Set(FLAG_MIA_JOINED_PARTY);
  FieldScript_End();
}

/// MercuryLighthouseExterior,
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE: the leader rides
/// the lift down from the Lighthouse Aerie beside its statue, hops off onto the
/// landing by the Fountain of Healing, and the lift goes back up.
static void MercuryLighthouse_RideLift(void) {
  struct MapGridLayerScroll *liftLayer;
  s32 liftSpeed;

  liftLayer = &((struct MapGridScrollWork *)gMapGridWork[0])
                   ->layers[MAPGRID_LAYER_OVERHEAD];
  liftSpeed = 0x9c28;
  liftLayer->offsetY = PIXELS(1161);
  liftLayer->speedY = 0;
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 0;
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed += -PIXELS(137);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->targetZ =
      ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed;
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->motionFlags = 0;
  ObjectEvent_SetPosition(LOCALID_LIFT_STATUE, PIXELS(680), PIXELS(440));
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed += -PIXELS(137);
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->targetZ =
      ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed;
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(1);
  FieldScript_StartScreenTransition();
  FieldScript_WaitTransitionFrames();
  FieldScript_WaitFrames(60);
  BlendScript_Suspend();
  Sound_PlaySong(SE_LIFT);
  // The lift layer scrolls down with both riders, 137 pixels, slowing as it
  // nears the landing.
  for (;;) {
    liftLayer->offsetY -= liftSpeed;
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed += liftSpeed;
    ObjectEvent_Get(LOCALID_PARTY_LEADER)->targetZ =
        ObjectEvent_Get(LOCALID_PARTY_LEADER)->z.fixed;
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed += liftSpeed;
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->targetZ =
        ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed;
    if (liftLayer->offsetY > PIXELS(1024)) {
      if ((gFrameCount & 15) == 0 && liftSpeed > 0xccb)
        liftSpeed += -0x560;
      WaitFrames(1);
      continue;
    }
    break;
  }
  liftLayer->offsetY = PIXELS(1024);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(2);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->motionFlags = 3;
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed = PIXELS(440);
  ObjectEvent_Get(LOCALID_LIFT_STATUE)->targetZ =
      ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed;
  FieldScript_WaitFrames(30);
  ObjectEvent_WalkToAndWait(LOCALID_PARTY_LEADER, 704, 584);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->bounce = 0;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_SPRING);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_LEAP);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->speed = PIXELS(3);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->acceleration = PIXELS(2);
  Sound_PlaySong(SE_JUMP);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->velocityY = PIXELS(4);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 0);
  ObjectEvent_SetDestination(LOCALID_PARTY_LEADER, 736, 584);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_PARTY_LEADER);
  ObjectEvent_SetSpriteFlags(ObjectEvent_Get(LOCALID_PARTY_LEADER), 1);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->bounce = 0x4000;
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_LEADER_SPRING);
  FieldScript_WaitFrames(6);
  ObjectEvent_FaceDirection(LOCALID_PARTY_LEADER, FACING_WEST, 30);
  Sound_PlaySong(SE_LIFT);
  // The lift and its statue go back up without the leader.
  for (;;) {
    liftLayer->offsetY += liftSpeed;
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed -= liftSpeed;
    ObjectEvent_Get(LOCALID_LIFT_STATUE)->targetZ =
        ObjectEvent_Get(LOCALID_LIFT_STATUE)->z.fixed;
    if (liftLayer->offsetY >= PIXELS(1161))
      break;
    if ((gFrameCount & 7) == 0 && liftSpeed <= 0xcccc)
      liftSpeed += 0x1999;
    WaitFrames(1);
  }
  liftLayer->offsetY = PIXELS(1024);
  MapGrid_CopyCellsTo(45, 91, 40, 91, 5, 4);
  MapGrid_CopyCellAttributes(104, 34, 5, 4, 40, 34);
  MapGrid_CenterOnFollowedPoint();
  WaitFrames(2);
  ObjectEvent_SetPosition(LOCALID_LIFT_STATUE, 0, 0);
  FieldScript_WaitFrames(30);
  ObjectEvent_Get(LOCALID_PARTY_LEADER)->priorityFlags |=
      OBJ_EVENT_PRIORITY_AUTOMATIC;
}

/// Mia casts Ply on the Ply target, moved onto the door.
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void) {
  struct FieldEffectWork *effectWork;
  u8 flags;

  effectWork = gFieldEffectWork;
  ObjectEvent_SetPosition(LOCALID_PLY_TARGET, PIXELS(840), PIXELS(600));
  FieldEffect_LoadActionEffectResources(ABILITY_PLY, 1);
  FieldEffect_SetupObjectPair(LOCALID_MIA, LOCALID_PLY_TARGET);
  flags = (u8)effectWork->messageMode | FIELD_MESSAGE_LOWER;
  effectWork->messageMode = flags;
  FieldEffect_StartCast();
  Field_DispatchTypeHandler(1);
  FieldEffect_StopCasterHook();
}

/// Replaces one inventory item in the member and slot that contain it.
static void MercuryLighthouse_ReplaceInventoryItem(s32 oldItem, s32 newItem) {
  s32 member = PartyInventory_FindMember(oldItem);

  if (member != -1) {
    s32 inventorySlot = Inventory_Find(member, oldItem);

    if (inventorySlot != -1) {
      Unit_Get(member)->inventory[inventorySlot] = newItem;
    }
  }
}

#include "common/mercury_lighthouse_lights.h"

/// Spreads one blue ripple (SPRITE_RIPPLE) from an object event, for 30
/// frames.
static void MercuryLighthouse_SpawnRipple(struct ObjectEvent *source) {
  struct ObjectEvent *effect;
  struct Sprite *sprite;

  effect = ObjectEvent_Create(SPRITE_RIPPLE, source->x.fixed, source->y.fixed,
                              source->z.fixed);
  if (effect == 0) {
    return;
  }

  sprite = effect->sprite;
  ObjectEvent_SetScript(effect, gMercuryLighthouse_RippleScript);

  effect->motionFlags = 0;
  effect->layer = 1;
  effect->priorityFlags = 2;

  if (sprite != 0) {
    Sprite_StartAnim(sprite, 2);
    sprite->flags = 0;
    sprite->image.priority = 3;
  }
}

#include "common/mercury_lighthouse_pillar.h"

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower. Pack
/// negative steps through unsigned words rather than left-shifting a negative.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))

s32 gPushedObject_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gPushedObject_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedObject_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouse_RippleScript[] = {
    SC_WAIT(30),
    SC_FREE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(MercuryLighthouse_AdvanceAnchoredRiseFrame),
    SC_FREE,
};

s32 gMercuryLighthouse_RampScript[] = {
    SC_CALL(MercuryLighthouse_GrowLight),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(ObjectEvent_AdvanceScaleCounter),
    SC_FREE,
};

s32 gMercuryLighthouse_PillarScript[] = {
    SC_CALL(MercuryLighthouse_FollowLeader),
    SC_END,
};

u16 gMercuryLighthouse_MoveAngles[] = {
    0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
    FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gMercuryLighthouse_SparkBurstScript[] = {
    SC_SET(SW_SCALE_Y, Q16_ONE),
    SC_SET(SW_SCALE_X, Q16_ONE),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, Q16_ONE / 4),
    SC_SET(SW_SCALE_X, Q16_ONE / 4),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 5 * Q16_ONE / 8),
    SC_SET(SW_SCALE_X, 5 * Q16_ONE / 8),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gMercuryLighthouse_PushScriptA[] = {
    SC_WAIT(45),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCELERATION, 0x3333),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_MOVE(PIXELS(792), 0, PIXELS(712)),
    SC_SYNC,
    SC_END,
};

s32 gMercuryLighthouse_PushScriptB[] = {
    SC_WAIT(60),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCELERATION, 0x3333),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_MOVE(PIXELS(792), 0, PIXELS(744)),
    SC_SYNC,
    SC_CALL(MercuryLighthouse_StopStatueSpray),
    SC_END,
};

s32 gMercuryLighthouse_PushScriptD[] = {
    SC_WAIT(60),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCELERATION, 0x3333),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_MOVE(PIXELS(824), 0, PIXELS(744)),
    SC_SYNC,
    SC_CALL(MercuryLighthouse_StopStatueSpray),
    SC_END,
};

s32 gMercuryLighthouse_PushScriptC[] = {
    SC_WAIT(45),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

u16 gMercuryLighthouse_GateCells[] = {
    MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

u16 gMercuryLighthouse_GateCloseCells[] = {
    MAPGRID_ANIMATION_STEP(9, 1, 1, 2, 10), MAPGRID_ANIMATION_STEP(11, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(13, 1, 1, 2, 6), MAPGRID_ANIMATION_STEP(15, 1, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

u16 gMercuryLighthouse_FloorSwitchCells[] = {
    MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

u16 gMercuryLighthouse_FloorSwitchCloseCells[] = {
    MAPGRID_ANIMATION_STEP(10, 28, 1, 2, 10), MAPGRID_ANIMATION_STEP(12, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(14, 28, 1, 2, 6), MAPGRID_ANIMATION_STEP(16, 28, 1, 2, 6), MAPGRID_ANIMATION_STEPS_END,
};

u16 gMercuryLighthouse_ColumnCells[] = {
    MAPGRID_ANIMATION_STEP(126, 35, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 38, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 41, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 44, 1, 2, 2), MAPGRID_ANIMATION_STEP(126, 47, 1, 2, 2), MAPGRID_ANIMATION_STEPS_END,
};

struct MapWarpDest gMercuryLighthouseWarpDestsOther[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gMercuryLighthouseMiddleWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { MAP_TABLE_END },
};

struct MapWarpDest gMercuryLighthouseUpperWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, CONDITION_ALWAYS, 88, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, CONDITION_ALWAYS, 504, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, CONDITION_ALWAYS, 264, 0, 120, FACING_SOUTH, 0, 32, 32, 544, 208, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_GATE, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_FIVE_PILLARS, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS_STATUE_FALLS, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE_STATUE_FALLS, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { MAP_TABLE_END },
};

struct MapWarpDest gMercuryLighthouseExteriorWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP, CONDITION_ALWAYS, 840, 0, 856, FACING_NORTH, 0, 576, 328, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE, CONDITION_ALWAYS, 840, 0, 600, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_UNUSED, CONDITION_ALWAYS, 872, 0, 600, FACING_SOUTH, 0, 720, 544, 960, 912, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, 680, 0, 584, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
    { MAP_TABLE_END },
};

u32 gMercuryLighthouse_Warps[] = {
    MAP_WARPS(MercuryLighthouseLower),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_WEST_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_EAST_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH),
    MAP_WARP(MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_PIPE),
    MAP_WARPS(MercuryLighthouseMiddle),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST),
    MAP_WARPS(MercuryLighthouseUpper),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_FIVE_PILLARS),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_FIVE_PILLARS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_GATE),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GATE_FROM_STONE_DOORS_STATUE_FALLS),
    MAP_WARP(MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_GATE_STATUE_FALLS),
    MAP_WARPS(MercuryLighthouseExterior),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_PATH),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_PATH, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_FROM_WATERFALL),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, WorldMap, WORLD_MAP_AFTER_MERCURY_LIGHTHOUSE_LIT),
    MAP_WARP(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, MercuryLighthouseAerie, MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gMercuryLighthouseObjectEventTemplatesOther[] = {
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMercuryLighthouseLowerObjectEventTemplates[] = {
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(632), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(672), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(704), 0, PIXELS(272), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(744), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(648), 0, PIXELS(299), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMercuryLighthouseMiddleObjectEventTemplates[] = {
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(152), 0, PIXELS(528), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(184), 0, PIXELS(587), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(704), 0, PIXELS(672), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SLEET), MOVEMENT_TYPE_STAND,
        PIXELS(888), 0, PIXELS(392), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMercuryLighthouseUpperObjectEventTemplates[] = {
    { SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL), MOVEMENT_TYPE_STAND,
        PIXELS(200), 0, PIXELS(360), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL, MOVEMENT_TYPE_STAND,
        PIXELS(808), 0, PIXELS(712), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(816), 0, PIXELS(544), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(872), 0, PIXELS(576), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(760), 0, PIXELS(635), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(776), 0, PIXELS(635), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(872), 0, PIXELS(635), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(712), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(776), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(936), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(472), 0, PIXELS(328), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gMercuryLighthouseExteriorObjectEventTemplates[] = {
    { LOCALID_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(664), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { LOCALID_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { LOCALID_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(792), 0, PIXELS(872), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(888), 0, PIXELS(872), FACING_SOUTH, TALK_FACING_KEEP, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(840), 0, PIXELS(600), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(216), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        PIXELS(824), 0, PIXELS(696), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
        PIXELS(760), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_WANDER,
        PIXELS(920), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MOVEMENT_TYPE_STAND,
        PIXELS(856), 0, PIXELS(792), FACING_NORTHWEST + FACING_STEP, TALK_FACING_PARTY, 0 },
    { MAP_TABLE_END },
};

struct MapEvent gMercuryLighthouseLowerEvents[] = {
    { MAP_EVENT_WARP, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST },
    { MAP_EVENT_WARP, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL },
    { MAP_EVENT_WARP, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS },
    { MAP_EVENT_WARP, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST },
    { MAP_EVENT_WARP, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE },
    { MAP_EVENT_WARP, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH },
    { MAP_EVENT_WARP, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER },
    { MAP_EVENT_COORD, 32, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_LoseLightsOnLower) },
    { COORD_EVENT_WALKING, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { MAP_EVENT_COORD, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_LoseLightsOnLower) },
    { COORD_EVENT_WALKING | COORD_EVENT_FLAG_QUICK, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { MAP_EVENT_COORD, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_RippleWhileInPool) },
    { COORD_EVENT_WALKING, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_HopAcrossPoolOnLower) },
    { MAP_EVENT_COORD, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_ChargeLightsOnLower) },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_EnterRisingPassage) },
    { COORD_EVENT_WALKING, 25, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushChannelPipes) },
    { MAP_EVENT_TALK, LOCALID_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThePathIsBlockedAgainWhat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThisStatueWasntHereBeforeI) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_STATUE_BLOCKING_MIA, FLAG_MIA_JOINED_PARTY, EVENT_SCRIPT(MercuryLighthouse_MiaJoinsAfterRescue) },
    { BG_EVENT(BG_EVENT_CHEST), 100, FLAG_CHEST_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { MAP_EVENTS_END },
};

struct MapEvent gMercuryLighthouseMiddleEvents[] = {
    { MAP_EVENT_WARP, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES },
    { MAP_EVENT_WARP, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH },
    { MAP_EVENT_WARP, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH },
    { MAP_EVENT_WARP, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE },
    { MAP_EVENT_WARP, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { MAP_EVENT_WARP, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST },
    { MAP_EVENT_WARP, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST },
    { MAP_EVENT_WARP, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST },
    { MAP_EVENT_WARP, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST },
    { COORD_EVENT_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_EnterRisingPassage) },
    { COORD_EVENT_WALKING, 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushStairAndFountainPipes) },
    { MAP_EVENT_COORD, 2, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { MAP_EVENT_COORD, 3, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { MAP_EVENT_COORD, 4, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { COORD_EVENT_WALKING, 10, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushGateSwitchStatue) },
    { MAP_EVENT_COORD, 12, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PressGateFloorSwitch) },
    { MAP_EVENT_COORD, 10, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
    { MAP_EVENT_COORD, 13, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
    { COORD_EVENT_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushStairAndFountainPipes) },
    { MAP_EVENT_COORD, 46, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { MAP_EVENT_TALK, LOCALID_SLEET, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_StartDjinnBattle) },
    { BG_EVENT(BG_EVENT_SILENT), 20, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_GATE_STATUE, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_OpenGateWithMovedStatue) },
    { BG_EVENT(BG_EVENT_CHEST), 101, FLAG_CHEST_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { BG_EVENT(BG_EVENT_CHEST), 102, FLAG_CHEST_NUT, FIND_ITEM(ITEM_NUT) },
    { MAP_EVENTS_END },
};

struct MapEvent gMercuryLighthouseUpperEvents[] = {
    { MAP_EVENT_WARP, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST },
    { MAP_EVENT_WARP, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_LeaveUpperByPassage) },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER },
    { MAP_EVENT_WARP, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_FIVE_PILLARS },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GATE_TO_STONE_DOORS },
    { MAP_EVENT_WARP, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_UP, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { MAP_EVENT_WARP, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 63, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_LeaveUpperWaterfallsByPassage) },
    { MAP_EVENT_WARP, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_TO_GATE },
    { MAP_EVENT_WARP, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST },
    { MAP_EVENT_WARP, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 64, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_GATE },
    { COORD_EVENT_WALKING, 10, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushStairSwitchStatue) },
    { COORD_EVENT_WALKING, 12, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushStairSwitchStatue) },
    { MAP_EVENT_COORD, 12, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PressStairFloorSwitch) },
    { MAP_EVENT_COORD, 10, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_ReleaseStairFloorSwitch) },
    { COORD_EVENT_WALKING, 40, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushStoneDoorPipes) },
    { COORD_EVENT_WALKING, 47, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushAndSaveCrossingStatuePositions) },
    { BG_EVENT(BG_EVENT_SILENT), 20, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FALLEN_STATUE, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_OpenStairGateWithMovedStatue) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CROSSING_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CROSSING_STATUE + 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CROSSING_STATUE + 2, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_CROSSING_STATUE + 3, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LAST_CROSSING_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_GAP_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_OpenMovedStatueCrossing) },
    { MAP_EVENTS_END },
};

struct MapEvent gMercuryLighthouseEventsOther[] = {
    { MAP_EVENT_WARP, 44, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE },
    { MAP_EVENT_WARP, 43, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE },
    { MAP_EVENT_WARP, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST },
    { MAP_EVENT_WARP | WARP_STYLE_STAIRS_DOWN, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST },
    { MAP_EVENT_WARP, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_PATH_TO_WATERFALL },
    { COORD_EVENT_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_LeaveWaterfallByPassage) },
    { MAP_EVENT_TALK, LOCALID_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT_SCRIPT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), 0x400 | LOCALID_MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT_SCRIPT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { MAP_EVENT_TALK, LOCALID_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, EVENT_SCRIPT(MercuryLighthouse_MiaDiscussesBlockedEntrance) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MIA, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, MSGNO(MsgMercuryLighthouseSomeoneDoesntWantMeToGet) },
    { MAP_EVENT_TALK, LOCALID_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseTheLegendsSaidThatIfThe) },
    { MAP_EVENT_TALK, LOCALID_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseOurFountainIsBackTheySay) },
    { MAP_EVENT_TALK, LOCALID_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIDrankTheHealingWaterAnd) },
    { MAP_EVENT_TALK, LOCALID_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseWeDontHaveToFearAny) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseHowDidTheFountainKnowThat) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseAllThatLivesThatMeansIt) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIKnewItWouldWorkBut) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseINeverBelievedThoseStoriesBut) },
    { BG_EVENT(BG_EVENT_SILENT), 10, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectBlockedPassage) },
    { BG_EVENT(BG_EVENT_SILENT), 42, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_ReadHermesFountainInscription) },
    { BG_EVENT(BG_EVENT_SILENT), 5, FLAG_HERMES_WATER_TAKEN, EVENT_SCRIPT(MercuryLighthouse_InspectHealingFountain) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_DOOR_STATUE, FLAG_IMIL_MET_MIA, 0 },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_DOOR_STATUE, FLAG_MERCURY_LIGHTHOUSE_OPENED_BY_MIA, EVENT_SCRIPT(MercuryLighthouse_MiaOpensEntranceWithPly) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_PLY), LOCALID_WATERFALL_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PlyOpensWaterfallPassage) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { USE_ITEM(ITEM_EMPTY_BOTTLE), 5, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), EVENT_SCRIPT(MercuryLighthouse_FillHermesWaterBottle) },
    { MAP_EVENTS_END },
};
// clang-format on
