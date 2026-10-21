/// @file
/// Mercury Lighthouse (SceneId_MercuryLighthouseLower,
/// SceneId_MercuryLighthouseMiddle, SceneId_MercuryLighthouseUpper and
/// SceneId_MercuryLighthouseExterior), four of the lighthouse's scenes. They
/// share the game's area name Mercury Lighthouse with
/// SceneId_MercuryLighthouseEntrance (mercury_lighthouse_entrance.c), the floor
/// inside the door; the Lighthouse Aerie at the top is
/// SceneId_MercuryLighthouseAerie (mercury_lighthouse_aerie.c). The four share
/// one exit table.
///
/// Where it falls in the story: the waters of the lighthouse's fountain might
/// save Tret, the tree of Kolima Forest, but they have long since dried up.
/// Mia, the healer of Imil, is of the Mercury Clan, "sworn protectors of a
/// special power", Ply; she has hurried there (FLAG_IMIL_MET_MIA), and only she
/// and Alex, her apprentice who disappeared, can enter. Every entry sets
/// FLAG_MERCURY_LIGHTHOUSE_LIT, under which Mercury Adepts regain PP faster,
/// and Retreat leads out to MercuryLighthouseExterior's
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP.
///
/// - SceneId_MercuryLighthouseExterior, the lighthouse's door and the ground
///   around it, reached from the world map
///   (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP). Until Mia has hurried here
///   the door stays shut. Then she stands before it, blocked by a statue
///   (ACTOR_DOOR_STATUE): she senses someone inside
///   (MercuryLighthouse_MiaSensesSomeoneInside,
///   FLAG_MIA_SENSED_SOMEONE_INSIDE). Move on the statue
///   (MercuryLighthouse_MiaOpensEntranceWithPly): Mia saw the force that pushed
///   it, learns the word Psynergy, opens the door with Ply, the power handed
///   down in her clan, and goes in (FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE).
///   MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, through the
///   door, leads into MercuryLighthouseEntrance.
/// - SceneId_MercuryLighthouseLower. Mia is blocked again by a statue that was
///   not there before; Move pushes it aside, and she joins the party with Fizz,
///   a Mercury Djinni (MercuryLighthouse_MiaJoinsAfterRescue,
///   FLAG_MIA_JOINED_PARTY). Three channel pillars pushed into place let the
///   water run (FLAG_PIPE_1_JOINED to _3_JOINED) and carry the party down to
///   MercuryLighthouseEntrance
///   (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE,
///   FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN). Once Ply has worked in
///   MercuryLighthouseEntrance (FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), water
///   pillars rise underfoot to cross on, as there
///   (common/mercury_lighthouse_pillar.h). A chest holds a Sleep Bomb.
/// - SceneId_MercuryLighthouseMiddle. One pillar raises or lowers stairs
///   (FLAG_STAIRS_RAISED), another starts a fountain (FLAG_FOUNTAIN_RUNNING),
///   and a statue moved onto a floor switch holds a gate open for good
///   (FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN); standing on the switch opens it
///   only while the leader stays. Sleet, a Mercury Djinni, fights when spoken
///   to. Chests hold a Psy Crystal and a Nut.
/// - SceneId_MercuryLighthouseUpper. A pillar pushed along a row opens one of
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
///   (MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_WALKWAY) shows its
///   riddle, `He who honors
///   the goddess of rainbows shall be guided to the heavens upon wings of fluid
///   grace.`; Ply on its statue stops it and opens the way up
///   (MercuryLighthouse_PlyOpensWaterfallPassage). The first time, Mia rises
///   first and the party follows
///   (MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, the world map's
///   WORLD_MAP_MERCURY_LIGHTHOUSE_LIT, whose lighthouse transition ends at the
///   Lighthouse Aerie); there the beacon has already been lit, and the party
///   fights Saturos (mercury_lighthouse_aerie.c). Once Saturos is beaten
///   (FLAG_SATUROS_BEATEN_AT_MERCURY) the leader rises alone
///   (MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, the aerie's
///   MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR), the fountain
///   flows, people come to drink from it, and an Empty Bottle used at it
///   becomes Hermes' Water, once a visit (FLAG_SCENE_TEMP_00).

#include "script.h"
#include "game_state.h"
#include "heap_state.h"
#include "iwram_call.h"
#include "keystate.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/scene/mercury_lighthouse.h"
#include "unit/scene/common/effect_spawn.h"
#include "staged_actor.h"
#include "unit/scene/common/mercury_lighthouse_lights.h"
#include "unit/scene/common/mercury_lighthouse_pillar.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
#include "field_obj.h"
#include "mem.h"
#include "mercury.h"
#include "field_event.h"
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
#include "field_event.h"
#include "engine_call.h"
#include "field_scene.h"
#include "djinn_ids.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "global_cells.h"
#include "map_render_work.h"
#include "io_reg.h"
#include "dma.h"
#include "object_runtime.h"
#include "animspr.h"
#include "map.h"
#include "map_scroll.h"
#include "fx_scene.h"
#include "field_runtime.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"
#include "iowrite.h"
#include "battle_unit.h"
#include "flags.h"
#include "songs.h"
#include "macros.h"

#if TBS_PATCH_1
#define MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY Actor_MoveToAndWait
#else
#define MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY Actor_SetDestination
#endif

#ifdef UBFIX
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT                              \
  Engine_ActorSetAnimationAndWait
#else
#define MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT                              \
  ((s32 (*)())Engine_ActorSetAnimationAndWait)
#endif

static void MercuryLighthouse_OpenGateWithMovedPillar(void);
static void MercuryLighthouse_OpenStairGateWithMovedPillar(void);
static void MercuryLighthouse_SyncCrossingStatueFlags(void);
static void SceneEffect_SpawnParticleRowsAndDrawTiles(void);
static void MercuryLighthouse_OpenThreeStepStair(void);
static void SceneEffect_SpawnParticleEveryFourthFrame(void);
static void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z);
static void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z);
static void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 angle);
static void MercuryLighthouse_RideLift(void);
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void);
static void MercuryLighthouse_ReplaceInventoryItem(s32 oldItem, s32 newItem);

#define NULL ((void *)0)

/// MercuryLighthouseLower: Mia, the statue that blocks her and the channel
/// pillars.
enum MercuryLighthouseLowerActor {
  /// ★ Named from the game: "But this statue is blocking my way..."
  /// (MsgImilSomebodyHere)
  ACTOR_STATUE_BLOCKING_MIA = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_CHANNEL_PILLAR_1,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_CHANNEL_PILLAR_2,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_CHANNEL_PILLAR_3,
  /// A pillar sprite of one still frame, the one after the moving pillars' two
  /// in the sprite table, 59 pixels south of where ACTOR_CHANNEL_PILLAR_3 comes
  /// to rest; pushing it does nothing, and nothing else moves it.
  /// ★ Named from what the player sees: a small stone pillar like the channel
  /// pillars (its render)
  ACTOR_CHANNEL_FIXED_PILLAR
};

/// MercuryLighthouseMiddle: the stair and fountain pillars, the statue for the
/// gate's floor switch and Sleet, a Mercury Djinni.
enum MercuryLighthouseMiddleActor {
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_STAIR_PILLAR = ACTOR_FIRST_PLACED,
  /// ACTOR_CHANNEL_FIXED_PILLAR's sprite, 59 pixels south of where
  /// ACTOR_STAIR_PILLAR rests once the stairs are raised; nothing moves it.
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_STAIR_FIXED_PILLAR,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_FOUNTAIN_PILLAR,
  /// ★ Named from what the player sees: a blue statue of a woman holding a jar
  /// (its render)
  ACTOR_GATE_STATUE,
  /// ★ Named from the game: "Sleet" (MsgDjinnName)
  ACTOR_SLEET
};

/// MercuryLighthouseUpper: the sliding statue and where it lands below, the
/// pillars before the stone doors, the five statues of the crossing and the gap
/// statue.
enum MercuryLighthouseUpperActor {
  /// The statue of ACTOR_SLIDING_STATUE once it has fallen to the floor below.
  /// ★ Named from what the player sees: the blue statue ACTOR_SLIDING_STATUE
  /// shows on its ledge above, before it falls (its render)
  ACTOR_FALLEN_STATUE = ACTOR_FIRST_PLACED,
  /// Slides off its ledge when the door switch pillar stops at column 48.
  /// ★ Named from what the player sees: a blue statue under a jet of water on
  /// its ledge (its render)
  ACTOR_SLIDING_STATUE,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_DOOR_ROOM_PILLAR,
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_DOOR_SWITCH_PILLAR,
  /// Three with ACTOR_CHANNEL_FIXED_PILLAR's sprite, in a row south of the
  /// stone doors' pillars; nothing moves them.
  /// ★ Named from what the player sees: a small stone pillar by the water
  /// grates (its render)
  ACTOR_FIRST_DOOR_FIXED_PILLAR,
  /// Five in a row along the north wall.
  /// ★ Named from what the player sees: stone statues of a robed figure (its
  /// render)
  ACTOR_FIRST_CROSSING_STATUE = ACTOR_FIRST_DOOR_FIXED_PILLAR + 3,
  /// ★ Named from what the player sees: a stone statue of a robed figure (its
  /// render)
  ACTOR_LAST_CROSSING_STATUE = ACTOR_FIRST_CROSSING_STATUE + 4,
  /// ★ Named from what the player sees: a stone statue of a robed figure
  /// before a gap in the floor (its render)
  ACTOR_GAP_STATUE
};

/// MercuryLighthouseExterior: Garet, Ivan and Mia in their own party slots, the
/// statues by the way in, the statue at the door, the Ply target, the statue by
/// the waterfall, the statue on the lift and the people at the fountain.
enum MercuryLighthouseExteriorActor {
  /// A pair of statues either side of the way in from the world map
  /// (MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP); the east one is drawn
  /// mirrored.
  /// ★ Named from what the player sees: blue statues either side of the way in
  /// (its render)
  ACTOR_WEST_APPROACH_STATUE = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: blue statues either side of the way in
  /// (its render)
  ACTOR_EAST_APPROACH_STATUE,
  ACTOR_DOOR_STATUE,
  /// ★ Named from the game: "used Ply." (MsgMercuryDidThat)
  ACTOR_PLY_TARGET,
  /// ★ Named from what the player sees: a blue statue beside the waterfall (its
  /// render)
  ACTOR_WATERFALL_STATUE,
  /// ★ Named from what the player sees: the blue statue that stands on the lift
  /// in the Lighthouse Aerie (its render)
  ACTOR_LIFT_STATUE,
  /// The first of four people at the Fountain of Healing once Saturos is
  /// beaten, in order a boy, a woman with fair hair, an old man with a cane and
  /// an old woman.
  /// ★ Named from what the player sees: people gathered round the fountain
  /// (its render)
  ACTOR_FIRST_VISITOR
};

/// Puzzle states kept until the party leaves the lighthouse.
enum {
  FLAG_STAIRS_RAISED = 0x302,
  FLAG_FOUNTAIN_RUNNING = 0x306,
  FLAG_FOUNTAIN_SHOWN = 0x307, ///< the camera has shown it start once
  /// MercuryLighthouseLower's first pipe piece has been pushed into place,
  /// joining the water pipes; with all three joined the water carries the party
  /// down.
  /// ★ Named from what the player sees: a pipe piece pushed along until it
  /// joins the pipes and water sprays through (its render)
  FLAG_PIPE_1_JOINED = 0x310,
  /// ★ Named from what the player sees: the second pipe piece joining the
  /// pipes (its render)
  FLAG_PIPE_2_JOINED = 0x311,
  /// ★ Named from what the player sees: the third pipe piece joining the pipes
  /// (its render)
  FLAG_PIPE_3_JOINED = 0x312,
  FLAG_PIPE_313 = 0x313, ///< cleared with the three, never set here
  FLAG_DOOR_ROOM_PILLAR_PUSHED = 0x318,
  /// The door switch pillar's column: each opens a different stone door.
  FLAG_DOOR_SWITCH_PILLAR_AT_47 = 0x319,
  FLAG_DOOR_SWITCH_PILLAR_AT_48 = 0x31a,
  FLAG_DOOR_SWITCH_PILLAR_AT_53 = 0x31b,
  /// Two flags per statue of MercuryLighthouseUpper's row along the north wall,
  /// one for each column it can rest at, then the last statue's.
  /// ★ Named from what the player sees: a row of statues along the wall, each
  /// pushed aside with Move (its render)
  FLAG_FIRST_ROW_STATUE = 0x330,
  /// ★ Named from what the player sees: the last statue of the row (its render)
  FLAG_LAST_ROW_STATUE_WEST = 0x338,
  /// ★ Named from what the player sees: the last statue of the row (its render)
  FLAG_LAST_ROW_STATUE_EAST = 0x339,
  /// The statue in front of a passage has been moved aside for good.
  /// ★ Named from what the player sees: Move shifts the statue west, uncovering
  /// an arched passage in the wall behind it (its render)
  FLAG_PASSAGE_STATUE_MOVED = 0x348
};

/// Scene flags, kept while the party stays on the map.
enum {
  /// The Empty Bottle has been filled at the Fountain of Healing this visit.
  /// ★ Named from the game: "Hermes' Water" (MsgItemName)
  FLAG_HERMES_WATER_TAKEN = FLAG_SCENE_TEMP_00,
  /// ★ Named from what the player sees: the floor switch before the gate,
  /// which sinks under what stands on it (its render)
  FLAG_LEADER_ON_FLOOR_SWITCH = FLAG_SCENE_TEMP_00 + 0x56
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
#include "common/object_staged_actor.h"

/// MercuryLighthouseMiddle: talking to Sleet, a Mercury Djinni, starts its
/// battle.
static void MercuryLighthouse_StartDjinnBattle(void) {
  Field_RequestDjinnBattle(ACTOR_SLEET, DJINNI_SLEET);
}

#if TBS_PATCH_3
/// Scheduler callback: the fade overlay stops following the leader while he
/// crosses the patched floor region (MAP_LAYER_GROUND, 16-pixel rows 13 to 18).
static void MercuryLighthouse_UpdateLeaderEffectTarget(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct FadeOverlayWork *effectWork =
      *(struct FadeOverlayWork **)Ram_ActorEffectWork;

  if (leader->terrainId == 0 && leader->z.fixed >> 20 > 12 &&
      leader->z.fixed >> 20 <= 18)
    effectWork->actor = NULL;
  else
    effectWork->actor = leader;
}
#endif

#include "common/mercury_lighthouse_sparks.h"
#include "common/mercury_lighthouse_palette_cycle.h"

const struct SceneEntrance *MercuryLighthouse_GetEntrances(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MercuryLighthouseExterior) {
    return gMercuryLighthouseExteriorEntrances;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperEntrances;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddleEntrances;
  }
  return gMercuryLighthouseEntrancesOther;
}

const struct SceneRegion *MercuryLighthouse_GetRegions(void) { return NULL; }

/// Returns the packed exits shared by the lighthouse scenes.
const u32 *MercuryLighthouse_GetExits(void) { return gMercuryLighthouse_Exits; }

/// Selects the actors and obstacles for the current lighthouse scene.
const struct ScenePlacement *MercuryLighthouse_GetPlacements(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MercuryLighthouseLower) {
    return gMercuryLighthouseLowerPlacements;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddlePlacements;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperPlacements;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseExterior) {
    return gMercuryLighthouseExteriorPlacements;
  }
  return gMercuryLighthousePlacementsOther;
}

/// MercuryLighthouseLower, as in MercuryLighthouseEntrance, once Ply has worked
/// there: touching trigger 21 keeps rising sparks appearing on the pillar.
static void MercuryLighthouse_SpawnTimedRiseEffect(void) {
  MercuryLighthouse_SparkleAtLeader();
}

/// MercuryLighthouseLower, touching trigger 31 or 32: the raised pillar is let
/// go.
static void MercuryLighthouse_ReleaseRaisedPillar(void) {
  MercuryLighthouse_LoseLights();
}

/// MercuryLighthouseLower, walking into trigger 11, 21 or 31: one step across
/// the water pillars.
static void MercuryLighthouse_TryPillarStep(void) {
  MercuryLighthouse_HopAcrossPool();
}

/// MercuryLighthouseLower, touching trigger 11: the pillar underfoot rises to
/// its full height.
static void MercuryLighthouse_RaiseTouchedPillar(void) {
  MercuryLighthouse_ChargeLights();
}

/// MercuryLighthouseExterior, searching trigger 10. "Strange forces are at
/// work..."
static void MercuryLighthouse_InspectBlockedPassage(void) {
  Engine_EventBegin();
  Engine_MessageShowCentered(MsgMercuryLighthouseStrangeForcesBlockWay, 1);
  Engine_EventEnd();
}

/// Selects the puzzle interactions and dialogue for the current scene.
const struct SceneEvent *MercuryLighthouse_GetEvents(void) {
  s16 selector = gGameState.scene;

  if (selector == (s32)&SceneId_MercuryLighthouseLower) {
    return gMercuryLighthouseLowerEvents;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseMiddle) {
    return gMercuryLighthouseMiddleEvents;
  }
  if (selector == (s32)&SceneId_MercuryLighthouseUpper) {
    return gMercuryLighthouseUpperEvents;
  }
  return gMercuryLighthouseEventsOther;
}

#include "common/mercury_lighthouse_risedrop.h"

/// MercuryLighthouseUpper, walking north into trigger 53: the leader rises
/// through MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST (the
/// trigger less 50), as MercuryLighthouseEntrance's risers do.
static void MercuryLighthouse_ExitWithSparks(void) {
  MercuryLighthouse_LeaveThroughPassage(gEventWork->touchedTrigger - 50);
}

/// MercuryLighthouseUpper, walking north into trigger 63: the same, through
/// MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP.
static void MercuryLighthouse_ExitAfterSparkStep(void) {
  MercuryLighthouse_LeaveThroughPassage(gEventWork->touchedTrigger - 50);
}

/// MercuryLighthouseExterior, walking north into trigger 55: the same, through
/// MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_WALKWAY.
static void MercuryLighthouse_LeaveWithSparkTrail(void) {
  MercuryLighthouse_LeaveThroughPassage(gEventWork->touchedTrigger - 50);
}

/// MercuryLighthouseLower: with all three channel pillars in place the floor
/// shakes, the screen fades and the party is carried down to
/// MercuryLighthouseEntrance
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE); moving one
/// out again clears the flag.
static void MercuryLighthouse_ExitWhenChannelsOpen(void) {
  if (GameFlag_IsSet(FLAG_PIPE_1_JOINED) != 0 &&
      GameFlag_IsSet(FLAG_PIPE_2_JOINED) != 0 &&
      GameFlag_IsSet(FLAG_PIPE_3_JOINED) != 0) {
    GameFlag_Set(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
    Engine_EventWait(30);
    Map_SetShake(0x10000, 0x10000, 0x10000);
    Engine_AudioPlayCue(SE_141);
    Engine_EventWait(60);
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_AudioPlayCue(SE_289);
    Map_SetShake(-1, -1, 0xe666);
    Engine_MapRenderWaitForValues();
    Engine_EventRequestExit(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE);
  } else {
    GameFlag_Clear(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN);
  }
}

/// MercuryLighthouseLower, pushing a channel pillar (walking into trigger 25):
/// each pillar opens one channel when it reaches its place, and the water runs
/// on through the ones before it.
static void MercuryLighthouse_MoveChannelPillars(void) {
  struct StagedActorProbe probe;
  s32 column;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    // Each pillar controls one channel; restore the connected water tiles too.
    switch ((u32)probe.actorSlot) {
    case ACTOR_CHANNEL_PILLAR_1:
      if (probe.position.z >> 20 == 8) {
        SceneActor_MoveAndRedraw(probe);
        Engine_EventWait(20);
        Map_CopyCellsTo(119, 9, 109, 11, 1, 1);
        SceneEffect_SpawnRandomizedBurst(0x2d60000, 0, 0xb40000, 0x8000);
        GameFlag_Set(FLAG_PIPE_1_JOINED);
      } else {
        s32 cellCount = 1;

        Map_CopyCellsTo(117, 9, 104, 7, cellCount, cellCount);
        Map_CopyCellsTo(119, 8, 109, 11, cellCount, cellCount);
        Map_CopyCellsTo(118, 8, 104, 13, cellCount, cellCount);
        SceneActor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_PIPE_1_JOINED);
      }
      break;
    case ACTOR_CHANNEL_PILLAR_2:
      if (probe.position.z >> 20 == 12) {
        SceneActor_MoveAndRedraw(probe);
        Engine_EventWait(10);
        if (GameFlag_IsSet(FLAG_PIPE_1_JOINED)) {
          Map_CopyCellsTo(118, 9, 104, 13, 1, 1);
          SceneEffect_SpawnRandomizedBurst(0x2840000, 0, 0xd20000, 0x4000);
        }
        GameFlag_Set(FLAG_PIPE_2_JOINED);
      } else {
        s32 cellCount = 1;

        Map_CopyCellsTo(119, 8, 109, 11, cellCount, cellCount);
        if (GameFlag_IsSet(FLAG_PIPE_1_JOINED)) {
          Map_CopyCellsTo(119, 9, 109, 11, cellCount, cellCount);
          Map_CopyCellsTo(118, 8, 104, 13, cellCount, cellCount);
        }
        SceneActor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_PIPE_2_JOINED);
      }
      break;
    case ACTOR_CHANNEL_PILLAR_3:
      column = probe.position.x;
      if (column >> 20 == 40) {
        SceneActor_MoveAndRedraw(probe);
        GameFlag_Set(FLAG_PIPE_3_JOINED);
      } else {
        SceneActor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_PIPE_3_JOINED);
      }
      break;
    }
    MercuryLighthouse_ExitWhenChannelsOpen();
  }
  Engine_EventEnd();
}

/// MercuryLighthouseMiddle, touching trigger 2, 3 or 4 once the stairs are
/// raised, or 46: the leader is carried to the landing in a trail of sparkles.
static void MercuryLighthouse_WalkLeaderIn(void) {
  struct EventWork *eventWork;

  eventWork = gEventWork;
  Engine_EventBegin();
  FieldScheduler_AddCallback(SceneEffect_SpawnParticleEveryFourthFrame,
                             TASK_PRIORITY_SCENE);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x28000, 0x14000);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_AudioPlayCue(SE_228);
  // Each touched entry has its own landing point; the patched path waits first.
  if (eventWork->touchedTrigger == 2) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 232, 616);
  } else if (eventWork->touchedTrigger == 3) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 360, 728);
  } else if (eventWork->touchedTrigger == 4) {
    Actor_SetDestination(ACTOR_PARTY_LEADER, 248, 792);
  } else {
    MERCURY_LIGHTHOUSE_MOVE_LEADER_TO_ENTRY(ACTOR_PARTY_LEADER, 696, 592);
#if TBS_PATCH_1
    Actor_SetDestination(ACTOR_PARTY_LEADER, 696, 600);
    Engine_EventWait(30);
#endif
  }
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Object_GetById(ACTOR_PARTY_LEADER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_TaskRemoveCallback(SceneEffect_SpawnParticleEveryFourthFrame);
  Engine_EventEnd();
}

/// MercuryLighthouseMiddle, pushing the gate statue (walking into trigger 10)
/// while the gate is shut: it moves a step, and may come to rest on the floor
/// switch.
static void MercuryLighthouse_PushGateSwitchPillar(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  MercuryLighthouse_OpenGateWithMovedPillar();
}

/// MercuryLighthouseMiddle, Move on the gate statue, or after a push: resting
/// on the floor switch at (5, 13), it sinks two pixels and the gate opens for
/// good.
static void MercuryLighthouse_OpenGateWithMovedPillar(void) {
  s32 column;
  s32 row;
  struct FieldActor *pillar;

  column = Object_GetById(ACTOR_GATE_STATUE)->x.fixed / 0x100000;
  row = Object_GetById(ACTOR_GATE_STATUE)->z.fixed / 0x100000;
  Engine_EventBegin();
  if (column == 5 && row == 13) {
    Object_GetById(ACTOR_GATE_STATUE)->y.fixed += 0xfffe0000;
    pillar = Object_GetById(ACTOR_GATE_STATUE);
    pillar->targetY = Object_GetById(ACTOR_GATE_STATUE)->y.fixed;
    Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
    Engine_AudioPlayCue(SE_217);
    Engine_MapAnimateCells(gMercuryLighthouse_GateCells, 9, 7);
    {
      s32 gateColumn = 9;
      s32 gateRow = 10;
      Map_CopyCellAttributes(9, 5, 1, 1, gateColumn, gateRow);
    }
    Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN);
  }
  Engine_EventEnd();
}

/// MercuryLighthouseMiddle, touching trigger 12 while the gate is shut:
/// standing on the floor switch sinks the leader two pixels and opens the gate.
static void MercuryLighthouse_PressGateFloorSwitch(void) {
  s32 x;
  s32 z;
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_LEADER_ON_FLOOR_SWITCH) == 0) {
    x = Object_GetById(ACTOR_PARTY_LEADER)->x.part.pixel;
    z = Object_GetById(ACTOR_PARTY_LEADER)->z.part.pixel;
    if ((u32)(x - 84) <= 7 && z > 211 && z <= 219) {
      Engine_EventBegin();
      Engine_GameFlagSet(FLAG_LEADER_ON_FLOOR_SWITCH);
      Engine_EventWait(5);
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed += -0x20000;
      leader = Object_GetById(ACTOR_PARTY_LEADER);
      leader->targetY = Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
      Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
      Engine_AudioPlayCue(SE_217);
      Engine_MapAnimateCells(gMercuryLighthouse_GateCells, 9, 7);
      Engine_EventEnd();
    }
  }
}

/// MercuryLighthouseMiddle, touching trigger 10 or 13: stepping off the switch
/// closes the gate again.
static void MercuryLighthouse_CloseFloorSwitch(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    Engine_EventBegin();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed += 0x20000;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->targetY = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
    Engine_EventWait(5);
    Map_CopyCellsTo(7, 2, 5, 11, 1, 1);
    Engine_AudioPlayCue(SE_217);
    Map_AnimateCells(gMercuryLighthouse_GateCloseCells, 9, 7);
    Engine_EventEnd();
  }
}

/// Animates the fountain's map strips between the requested steps.
static void MercuryLighthouse_AnimateFountainTiles(s32 delayFrames,
                                                   s32 firstStep, s32 endStep) {
  u32 step;
  s32 sourceColumn;

  if (delayFrames != 0) {
    Engine_AudioPlayCue(SE_219);
  }
  step = firstStep;
  if (step < endStep) {
    do {
      Map_CopyCellsTo(45 - (step << 1), 32, 44 - (step << 1), 32, (step + 1),
                      6);
      Map_CopyCellsTo(45 - step, 51, 45 - step, 32, 1, 6);
      sourceColumn = 109 - step;
      Map_CopyCellsTo(sourceColumn, 32, 108 - step, 32, 1, 4);
      Map_CopyCellsTo(sourceColumn, 51, sourceColumn, 32, 1, 4);
      if (delayFrames != 0) {
        Map_SetShake(0x50000, 0x50000, 0x10000);
        Map_SetShake(-1, -1, 0xe666);
        Engine_EventWait(delayFrames);
      }
      step++;
    } while (step < endStep);
  }
  Map_CopyCellAttributes(42, 52, 4, 5, 42, 33);
}

/// Restores the fountain's map strips after moving its pillar away.
static void MercuryLighthouse_RestoreFountainTiles(s32 delayFrames) {
  u32 step;
  s32 destinationColumn;
  s32 sourceColumn;
  s32 copyWidth;
  s32 copyHeight;
  s32 column;
  s32 stripHeight;
  Engine_AudioPlayCue(SE_219);
  copyHeight = 6;
  step = 0;
  destinationColumn = 41;
  sourceColumn = 40;
  do {
    Map_CopyCellsTo(sourceColumn, 32, destinationColumn, 32, 3 - step,
                    copyHeight);
    copyWidth = 2;
    Map_CopyCellsTo(39, 51, sourceColumn, 32, 1, copyHeight);
    column = step + 106;
    stripHeight = 4;
    /* FAKEMATCH(temporary): the strip's width and height in locals, so loop.c
     * hoists the 2 into r9; as the literals 2 and 4, 11 of 90 instructions
     * differ in tbs-en. */
    Engine_MapCopyCellsTo(105, 51, column, 32, copyWidth, stripHeight);
    if (delayFrames != 0) {
      Map_SetShake(0x50000, 0x50000, 0x10000);
      Map_SetShake(-1, -1, 0xe666);
      Engine_EventWait(delayFrames);
    }
    step++;
    destinationColumn += 2;
    sourceColumn += 2;
  } while (step <= 2);
  Engine_AudioPlayCue(SE_288);
  Map_CopyCellAttributes(106, 33, 4, 5, 42, 33);
  Engine_MapRenderWaitForValues();
}

/// Emits the scheduled spray at the fountain's fixed position.
static void MercuryLighthouse_SprayAtFountain(void) {
  SceneEffect_SpawnRandomEveryFourFrames(0x2b20000, 0, 0x2480000);
}

/// Opens the fountain row by row and starts its continuous spray.
static void MercuryLighthouse_OpenFountainWithSpray(s32 delayFrames) {
  struct EffectOptions *effectOptions;
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
  struct EffectOptions options;

  Map_CopyCellsTo(78, 59, 110, 36, 1, 1);
  Map_CopyCellsTo(76, 59, 109, 36, 1, 1);
  effectOptions = &options;
  // The shared initializer grows this path from 0x150 to 0x154 bytes and
  // moves its palette store after the scales; the stop path also reorders
  // stores. Keep the ordinary assignments, without a new steering wrapper.
  effectOptions->palette = 7;
  effectOptions->startScaleX = 0x8000;
  effectOptions->startScaleY = 0x8000;
  row = 0;
  // Reveal each row while its edge emits particles in the two horizontal axes.
  do {
    rowOffset = (row << 4);
    particleIndex = 0;
    negativeRowOffset = -(row << 20);
    particleX = (0x2d80000 + negativeRowOffset);
    do {
      if ((particleIndex & 1) != 0) {
        randomX = Random_Next();
        // The original add/shift tree is multiplication by 0x3333.
        velocityX = ((u32)(randomX << 3) >> 16) * 0x3333 - 0xcccc;
        negativeIndex = -particleIndex;
        randomZ = Random_Next();
        velocityZ = ((u32)(randomZ << 3) >> 16) * 0x3333 - 0xcccc;
        FieldFx_Spawn(particleX, 0, 0x2480000, velocityX, 0, velocityZ, 0x90000,
                      effectOptions);
        Engine_EventWait(1);
      } else {
        negativeIndex = -particleIndex;
      }
      SceneEffect_SpawnRandomEveryFourFrames(
          (MERCURY_LIGHTHOUSE_OFFSET_FIXED(negativeIndex - rowOffset) +
           0x2d80000),
          0, 0x2480000);
      particleIndex = (particleIndex + 1);
      particleX = (particleX + -0x10000);
    } while ((u32)particleIndex <= 7);
    Map_CopyCellsTo(76, 59, (108 - row), 36, 2, 1);
    nextRow = row + 1;
    MercuryLighthouse_AnimateFountainTiles(delayFrames, row, nextRow);
    row = nextRow;
  } while ((u32)nextRow <= 1);
  Engine_EventWait(delayFrames);
  MercuryLighthouse_AnimateFountainTiles(0, nextRow, (nextRow + 1));
  Engine_AudioPlayCue(SE_211);
  // Keep the opened fountain spraying after the tile animation completes.
  FieldScheduler_AddCallback(MercuryLighthouse_SprayAtFountain,
                             TASK_PRIORITY_SCENE);
  Engine_MapRenderWaitForValues();
}

/// Restores the fountain tiles and stops its continuous spray.
static void MercuryLighthouse_StopFountainSpray(void) {
  struct EffectOptions *effectOptions;
  s32 row;
  s32 particleIndex;
  s32 offsetX;
  s32 random;
  s32 particleX;
  s32 particleZ;
  struct EffectOptions options;

  Map_CopyCellsTo(78, 58, 110, 36, 1, 1);
  effectOptions = &options;
  effectOptions->palette = 5;
  effectOptions->startScaleX = 0x8000;
  effectOptions->startScaleY = 0x8000;
  row = 0;
  do {
    offsetX = -0x20000;
    particleIndex = 1;
    do {
      if ((particleIndex & 1) != 0) {
        random = Engine_RandomNext();
        particleZ = ((0x248 - ((u32)((random << 2) + random) >> 16)) & 0xffff)
                    << 16;
        particleX = (offsetX - (row << 19)) + 0x2d80000;
        FieldFx_Spawn(particleX, 0, particleZ, -0x4000, 0, 0, 0x90000,
                      effectOptions);
        Engine_EventWait(1);
      }
      particleIndex++;
      offsetX = offsetX + -0x20000;
    } while ((u32)particleIndex <= 7);
    Map_CopyCellsTo(111, 35, (109 - row), 36, 1, 1);
    row++;
  } while ((u32)row <= 2);
  Engine_TaskRemoveCallback(MercuryLighthouse_SprayAtFountain);
}

/// MercuryLighthouseMiddle, pushing a pillar (walking into trigger 1 or 45):
/// the stair pillar raises the stairs at column 11 and lowers them anywhere
/// else; the fountain pillar starts the fountain at row 40, the first time with
/// the camera watching, and stops it at row 42.
static void MercuryLighthouse_MoveStairAndFountainPillars(void) {
  struct StagedActorProbe probe;
  s32 pillarId;
  s32 column;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    pillarId = probe.actorSlot;
    // The first pillar opens the stairs; the other starts or stops the
    // fountain.
    if (pillarId == ACTOR_STAIR_PILLAR) {
      column = probe.position.x;
      if ((column >> 20) == 11) {
        SceneActor_MoveAndRedraw(probe);
        Engine_EventWait(30);
        Engine_AudioPlayCue(SE_211);
        SceneEffect_SpawnParticleRowsAndDrawTiles();
        Map_CopyCellsTo(76, 60, 74, 38, 3, 1);
        Map_CopyCellsTo(77, 60, 76, 38, 2, 1);
        Map_CopyCellsTo(75, 58, 86, 41, 1, 3);
        Map_CopyCellsTo(75, 59, 86, 43, 1, 2);
        Map_CopyCellsTo(76, 59, 80, 49, 2, 1);
        Map_CopyCellsTo(77, 59, 82, 49, 2, 1);
        Engine_GameFlagSet(FLAG_STAIRS_RAISED);
      } else {
        probe.callback = MercuryLighthouse_OpenThreeStepStair;
        Map_CopyCellsTo(75, 57, 86, 41, 1, 1);
        Map_CopyCellsTo(71, 59, 86, 42, 1, 1);
        Map_CopyCellsTo(71, 59, 86, 43, 1, 1);
        Map_CopyCellsTo(71, 59, 86, 44, 1, 1);
        Map_CopyCellsTo(71, 59, 80, 49, 1, 1);
        Map_CopyCellsTo(71, 59, 81, 49, 1, 1);
        Map_CopyCellsTo(71, 59, 82, 49, 1, 1);
        Map_CopyCellsTo(78, 58, 83, 49, 1, 1);
        SceneActor_MoveAndRedraw(probe);
        GameFlag_Clear(FLAG_STAIRS_RAISED);
      }
    } else if (pillarId == ACTOR_FOUNTAIN_PILLAR) {
      if ((probe.position.z >> 20) == 40) {
        SceneActor_MoveAndRedraw(probe);
        if (GameFlag_IsSet(FLAG_FOUNTAIN_SHOWN) == 0) {
          Camera_SetSpeed(0x18000, 0x3000);
          Camera_MoveTo(PIXELS(714), -1, PIXELS(592), 1);
          Engine_CameraWaitForMove();
          GameFlag_Set(FLAG_FOUNTAIN_SHOWN);
          MercuryLighthouse_OpenFountainWithSpray(5);
          Engine_EventWait(50);
        } else {
          MercuryLighthouse_OpenFountainWithSpray(5);
        }
        GameFlag_Set(FLAG_FOUNTAIN_RUNNING);
      } else if ((probe.position.z >> 20) == 42) {
        probe.callback = MercuryLighthouse_StopFountainSpray;
        SceneActor_MoveAndRedraw(probe);
        MercuryLighthouse_RestoreFountainTiles(5);
        GameFlag_Clear(FLAG_FOUNTAIN_RUNNING);
      }
    }
  }
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, pushing the fallen statue (walking into trigger 10
/// or 12) while the stair gate is shut: it moves a step and may come to rest on
/// the floor switch.
static void MercuryLighthouse_PushStairSwitchPillar(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  Engine_EventEnd();
  MercuryLighthouse_OpenStairGateWithMovedPillar();
}

/// MercuryLighthouseUpper, Move on the fallen statue, or after a push: resting
/// on the switch at (10, 23), it sinks two pixels and the stair gate opens for
/// good.
static void MercuryLighthouse_OpenStairGateWithMovedPillar(void) {
  s32 column = Object_GetById(ACTOR_FALLEN_STATUE)->x.fixed / 0x100000;
  s32 row = Object_GetById(ACTOR_FALLEN_STATUE)->z.fixed / 0x100000;

  Engine_EventBegin();
  if (column == 10 && row == 23) {
    struct FieldActor *pillar;
    Object_GetById(ACTOR_FALLEN_STATUE)->y.fixed += 0xfffe0000;
    pillar = Object_GetById(ACTOR_FALLEN_STATUE);
    pillar->targetY = Object_GetById(ACTOR_FALLEN_STATUE)->y.fixed;
    Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
    Engine_AudioPlayCue(SE_217);
    Engine_MapAnimateCells(gMercuryLighthouse_FloorSwitchCells, 10, 18);
    Map_CopyCellAttributes(10, 16, 1, 1, column, 19);
    Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN);
  }
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, touching trigger 12 while the stair gate is shut:
/// standing on its switch opens it.
static void MercuryLighthouse_PressStairFloorSwitch(void) {
  s32 x;
  s32 z;

  if (GameFlag_IsSet(FLAG_LEADER_ON_FLOOR_SWITCH) != 0)
    return;
  x = Object_GetById(ACTOR_PARTY_LEADER)->x.part.pixel;
  z = Object_GetById(ACTOR_PARTY_LEADER)->z.part.pixel;
  if (x < 164 || x > 171)
    return;
  if (z < 372)
    return;
  if (z >= 380)
    return;
  Engine_EventBegin();
  Engine_GameFlagSet(FLAG_LEADER_ON_FLOOR_SWITCH);
  Engine_EventWait(5);
  Object_GetById(ACTOR_PARTY_LEADER)->y.fixed -= 0x20000;
  Object_GetById(ACTOR_PARTY_LEADER)->targetY =
      Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
  Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
  Engine_AudioPlayCue(SE_217);
  Engine_MapAnimateCells(gMercuryLighthouse_FloorSwitchCells, 10, 18);
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, touching trigger 10: stepping off the switch closes
/// the stair gate.
static void MercuryLighthouse_ReleaseStairFloorSwitch(void) {
  struct FieldActor *leader;

  if (GameFlag_IsSet(FLAG_LEADER_ON_FLOOR_SWITCH) != 0) {
    Engine_EventBegin();
    GameFlag_Clear(FLAG_LEADER_ON_FLOOR_SWITCH);
    Actor_Get(ACTOR_PARTY_LEADER)->y.fixed += 0x20000;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    leader->targetY = Actor_Get(ACTOR_PARTY_LEADER)->y.fixed;
    Engine_EventWait(5);
    Map_CopyCellsTo(8, 29, 10, 23, 1, 1);
    Engine_AudioPlayCue(SE_217);
    Map_AnimateCells(gMercuryLighthouse_FloorSwitchCloseCells, 10, 18);
    Engine_EventEnd();
  }
}

/// Chooses the random speed component used by stone-door particles.
static __inline__ s32 MercuryLighthouse_RandomDoorParticleSpeed(void) {
  // Sharing a centered-velocity helper with other sprays shrinks the fountain
  // from 0x150 to 0x128 bytes and grows this door from 0x27c to 0x284 in
  // English.
  u32 drift = Engine_RandomNext();

  return ((drift << 3) >> 16) * 0x3333;
}

/// Opens the selected stone door with particles along its moving edge.
static void MercuryLighthouse_OpenStoneDoor(s32 side) {
  struct EffectOptions options;
  u32 particleIndex;
  struct EffectOptions *effectOptions;
  u32 row;

  Engine_AudioPlayCue(SE_211);
  if (side == 0) {
    Map_CopyCellsTo(111, 57, 113, 42, 1, 1);
    Map_CopyCellsTo(111, 59, 113, 43, 1, 1);
  } else if (side == 1) {
    Map_CopyCellsTo(113, 58, 112, 46, side, side);
    Map_CopyCellsTo(115, 58, 113, 46, side, side);
  } else {
    Map_CopyCellsTo(115, 57, 116, 44, 1, 1);
    Map_CopyCellsTo(113, 57, 115, 44, 1, 1);
  }
  effectOptions = &options;
  effectOptions->palette = 7;
  effectOptions->startScaleX = 0x8000;
  effectOptions->startScaleY = 0x8000;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (side == 0) {
          FieldFx_Spawn(
              0x3180000, 0, 0x2c00000 + (row * 16 + particleIndex) * 0x10000,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0x90000,
              effectOptions);
        } else if (side == 1) {
          FieldFx_Spawn(
              0x3200000 + (row * 16 + particleIndex) * 0x10000, 0, 0x2ea0000,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0x90000,
              effectOptions);
        } else {
          FieldFx_Spawn(
              0x32c0000 + (row * 16 + particleIndex) * -0x10000, 0, 0x2ca0000,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0,
              MercuryLighthouse_RandomDoorParticleSpeed() + -0xcccc, 0x90000,
              effectOptions);
        }
        Engine_EventWait(1);
      }
    }
    if (side == 0) {
      Map_CopyCellsTo(111, 58, 113, row + 43, 1, 1);
      Map_CopyCellsTo(111, 59, 113, row + 44, 1, 1);
    } else if (side == 1) {
      Map_CopyCellsTo(114, 58, row + 113, 46, side, side);
      Map_CopyCellsTo(115, 58, row + 114, 46, side, side);
    } else {
      Map_CopyCellsTo(114, 57, 115 - row, 44, 1, 1);
      Map_CopyCellsTo(113, 57, 114 - row, 44, 1, 1);
    }
  }
}

/// Restores the selected stone door's tiles with trailing particles.
static void MercuryLighthouse_CloseStoneDoor(s32 side) {
  struct EffectOptions options;
  u32 particleIndex, row;

  Map_CopyCellsTo(0x70, 0x39, 0x71, 0x2a, 1, 1);
  Map_CopyCellsTo(0x75, 0x3a, 0x70, 0x2e, 1, 1);
  Map_CopyCellsTo(0x75, 0x39, 0x74, 0x2c, 1, 1);
  Engine_AudioPlayCue(SE_289);
  options.palette = 5;
  options.startScaleX = 0x8000;
  options.startScaleY = 0x8000;
  for (row = 0; row <= 2; row++) {
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        if (side == 0) {
          FieldFx_Spawn((0x319 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16,
                        0, (((row << 2) + particleIndex) << 17) + 0x02b70000, 0,
                        side, 0x4000, 0x90000, &options);
        } else if (side == 1) {
          FieldFx_Spawn((((row << 2) + particleIndex) << 17) + 0x03120000, 0,
                        ((((u32)Engine_RandomNext() * 5) >> 16) << 16) +
                            0x2e80000,
                        0x4000, 0, 0, 0x90000, &options);
        } else {
          FieldFx_Spawn(0x3380000 - (particleIndex << 17) - (row << 19), 0,
                        ((((u32)Engine_RandomNext() * 5) >> 16) << 16) +
                            0x2c80000,
                        0x4000, 0, 0, 0x90000, &options);
        }
        Engine_EventWait(1);
      }
    }
    if (side == 0)
      Map_CopyCellsTo(0x70, 0x3a, 0x71, row + 43, 1, 1);
    else if (side == 1)
      Map_CopyCellsTo(0x70, 0x3a, row + 113, 0x2e, side, side);
    else
      Map_CopyCellsTo(0x70, 0x3a, 115 - row, 0x2c, 1, 1);
  }
}

/// The camera turns to the sliding statue, which moves on only for the door
/// switch pillar positions recorded; returns 1 when it slides off its ledge.
static s32 MercuryLighthouse_StartStatueSlide(void) {
  s32 cellX;
  s32 cellZ;

  cellX = Object_GetById(ACTOR_SLIDING_STATUE)->x.fixed / 0x100000;
  cellZ = Object_GetById(ACTOR_SLIDING_STATUE)->z.fixed / 0x100000;
  Engine_CameraSetSpeed(0x50000, 0xa000);
  Engine_CameraMoveTo(PIXELS(816), -1, PIXELS(712), 1);
  Engine_CameraWaitForMove();
  // Only the recorded pillar arrangements allow the pillar's next push.
  if (!Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL)) {
    if (cellX == 50 && Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47)) {
      Actor_SetActionScript(ACTOR_SLIDING_STATUE,
                            gMercuryLighthouse_PushScriptA);
    } else if (cellX == 49) {
      if (cellZ == 44 && !Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47) &&
          !Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_48) &&
          !Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_53)) {
        Actor_SetActionScript(ACTOR_SLIDING_STATUE,
                              gMercuryLighthouse_PushScriptB);
      } else if (cellZ == 44 &&
                 Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47)) {
        Actor_SetActionScript(ACTOR_SLIDING_STATUE,
                              gMercuryLighthouse_PushScriptC);
      } else if (cellZ == 46 &&
                 Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_48)) {
        Actor_SetActionScript(ACTOR_SLIDING_STATUE,
                              gMercuryLighthouse_PushScriptD);
        Engine_EventWait(30);
        return 1;
      }
    }
  }
  Engine_EventWait(30);
  return 0;
}

/// Sparkles rise from the sliding statue as it moves.
static void MercuryLighthouse_SprayAboveStatue(void) {
  struct FieldActor *pillar = Object_GetById(ACTOR_SLIDING_STATUE);
  u32 random = Engine_RandomNext();

  s32 particleY = pillar->y.fixed + (((random << 2) >> 16) << 16);
  s32 particleZ = pillar->z.fixed;

  SceneEffect_SpawnRandomEveryFourFrames(pillar->x.fixed, particleY, particleZ);
}

/// The slide scripts start the sparkles as the statue sets off.
static s32
MercuryLighthouse_StartStatueSpray(struct ScriptInterpreter *interpreter) {
  s32 priority = TASK_PRIORITY_SCENE;

  FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue, priority);
  return 0;
}

// The sparkles stop when the statue halts; slide scripts and code call it.
// SC_CALL passes the interpreter. The native no-argument form also has two
// direct callers; adding NULL to those calls inserts two r0 = 0 instructions.
#ifdef UBFIX
static s32
MercuryLighthouse_StopStatueSpray(struct ScriptInterpreter *interpreter) {
#else
static s32 MercuryLighthouse_StopStatueSpray(void) {
#endif
  Engine_TaskRemoveCallback(MercuryLighthouse_SprayAboveStatue);
  return 0;
}

/// MercuryLighthouseUpper, pushing a pillar before the stone doors (walking
/// into trigger 40): the door switch pillar opens a different stone door at
/// each column, and at column 48, once the statue is in place, the statue
/// slides out and falls to the floor below.
static void MercuryLighthouse_MoveStoneDoorPillars(void) {
  struct StagedActorProbe probe;
  s32 destinationColumn;
  s32 startColumn;

  Engine_EventBegin();
  if (StagedActor_FindClearPosition(&probe) != 0) {
    switch (probe.actorSlot) {
    case ACTOR_DOOR_ROOM_PILLAR:
      SceneActor_MoveAndRedraw(probe);
      if (probe.position.z >> 20 == 38)
        Engine_GameFlagSet(FLAG_DOOR_ROOM_PILLAR_PUSHED);
      else
        Engine_GameFlagClear(FLAG_DOOR_ROOM_PILLAR_PUSHED);
      break;
    case ACTOR_DOOR_SWITCH_PILLAR:
      // Close the previous opening before revealing the new door edge.
      startColumn = Object_GetById(ACTOR_DOOR_SWITCH_PILLAR)->x.fixed >> 20;
      SceneActor_MoveAndRedraw(probe);
      destinationColumn = probe.position.x >> 20;
      if (destinationColumn == 47) {
        Engine_GameFlagSet(FLAG_DOOR_SWITCH_PILLAR_AT_47);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_48);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_53);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == 54)
          MercuryLighthouse_CloseStoneDoor(0);
        else if (startColumn == 48)
          MercuryLighthouse_CloseStoneDoor(1);
        MercuryLighthouse_OpenStoneDoor(2);
        Engine_EventWait(60);
      } else if (destinationColumn == 48) {
        Engine_GameFlagSet(FLAG_DOOR_SWITCH_PILLAR_AT_48);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_53);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_47);
        if (MercuryLighthouse_StartStatueSlide() != 0) {
          s32 x;

          MercuryLighthouse_CloseStoneDoor(2);
          x = PIXELS(840);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSpray(NULL);
#else
          MercuryLighthouse_StopStatueSpray();
#endif
          MercuryLighthouse_OpenStoneDoor(1);
          Object_RefreshSelectorById(ACTOR_SLIDING_STATUE);
          OverlayObject_PrepareObject(x, 0, PIXELS(786), 223);
          OverlayObject_PrepareObject(x, 0, PIXELS(818), 223);
          Engine_ActorSetDestination(ACTOR_SLIDING_STATUE, 840, 744);
          Engine_EventWait(5);
          Engine_AudioPlayCue(SE_189);
          Engine_ActorWaitForMove(ACTOR_SLIDING_STATUE);
          Engine_EventWait(40);
          Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL);
          Engine_ColorBufferApplySource(0x10000, 0);
          gEventWork->startTransition =
              SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
          Engine_EventCloseScreen();
          Engine_EventWaitForScreen();
          Engine_EventRequestExit(
              MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_STAIR_GATE_STATUE_FALLS);
        } else {
          MercuryLighthouse_CloseStoneDoor(2);
#ifdef UBFIX
          MercuryLighthouse_StopStatueSpray(NULL);
#else
          MercuryLighthouse_StopStatueSpray();
#endif
          MercuryLighthouse_OpenStoneDoor(1);
          Engine_EventWait(60);
        }
      } else if (destinationColumn == 53) {
        Engine_GameFlagSet(FLAG_DOOR_SWITCH_PILLAR_AT_53);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_47);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_48);
        MercuryLighthouse_StartStatueSlide();
        MercuryLighthouse_CloseStoneDoor(0);
        Engine_EventWait(60);
      } else {
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_47);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_48);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_53);
        MercuryLighthouse_StartStatueSlide();
        if (startColumn == 47)
          MercuryLighthouse_CloseStoneDoor(2);
        else if (startColumn == 48)
          MercuryLighthouse_CloseStoneDoor(1);
        MercuryLighthouse_OpenStoneDoor(0);
        Engine_EventWait(60);
      }
      break;
    }
  }
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, pushing a crossing statue (walking into trigger 47):
/// it moves a step and where the statues rest is saved.
static void MercuryLighthouse_PushAndSaveCrossingStatuePositions(void) {
  Engine_EventBegin();
  StagedActor_AdvancePair();
  MercuryLighthouse_SyncCrossingStatueFlags();
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, Move on a crossing statue, or after a push: each
/// statue's column is saved in its pair of flags, and the last opens the
/// crossing at column 57 or 59.
static void MercuryLighthouse_SyncCrossingStatueFlags(void) {
  s32 statueIndex;
  s32 positionFlag;
  s32 column;

  Engine_EventBegin();
  // Four pillars use paired flags for their two saved stopping columns.
  for (statueIndex = 0, positionFlag = FLAG_FIRST_ROW_STATUE; statueIndex <= 3;
       statueIndex++, positionFlag += 2) {
    column =
        Object_GetById(statueIndex + ACTOR_FIRST_CROSSING_STATUE)->x.fixed /
        0x100000;
    if (column == (statueIndex << 2) + 39) {
      Engine_GameFlagSet(positionFlag);
      Engine_GameFlagClear(positionFlag + 1);
    } else if (column == (statueIndex << 2) + 41) {
      Engine_GameFlagSet(positionFlag + 1);
      Engine_GameFlagClear(positionFlag);
    } else {
      Engine_GameFlagClear(positionFlag);
      Engine_GameFlagClear(positionFlag + 1);
    }
  }
  // The last statue also determines whether the crossing cell is open.
  column = Object_GetById(ACTOR_LAST_CROSSING_STATUE)->x.fixed / 0x100000;
  if (column == 57) {
    Engine_GameFlagSet(FLAG_LAST_ROW_STATUE_WEST);
    Engine_GameFlagClear(FLAG_LAST_ROW_STATUE_EAST);
    Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
  } else if (column == 59) {
    Engine_GameFlagSet(FLAG_LAST_ROW_STATUE_EAST);
    Engine_GameFlagClear(FLAG_LAST_ROW_STATUE_WEST);
    Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
  } else {
    Engine_GameFlagClear(FLAG_LAST_ROW_STATUE_WEST);
    Engine_GameFlagClear(FLAG_LAST_ROW_STATUE_EAST);
    Map_CopyCellAttributes(53, 11, 1, 1, 58, 7);
  }
  Engine_EventEnd();
}

/// MercuryLighthouseUpper, Move on the gap statue: at column 28 it fills the
/// gap for good.
static void MercuryLighthouse_OpenMovedStatueCrossing(void) {
  s32 column;

  Engine_EventBegin();
  column = Object_GetById(ACTOR_GAP_STATUE)->x.fixed / 0x100000;
  if (column == 28) {
    Engine_GameFlagSet(FLAG_PASSAGE_STATUE_MOVED);
    {
      s32 destinationColumn = 31;
      s32 destinationRow = 20;

      Map_CopyCellAttributes(29, 20, 1, 1, destinationColumn, destinationRow);
    }
  }
  Engine_EventEnd();
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
  struct FieldActor *mia;

  Engine_EventBegin();
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Engine_EventSetMessage(MsgImilSomebodyHere);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_WalkToAndWait(MIA, 840, 648);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT, 60);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Engine_ActorSetAnimation(MIA, 16);
  mia = Object_GetById(MIA);
  mia->scaleX = -0x10000;
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  mia = Object_GetById(MIA);
  mia->scaleX = 0x10000;
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  Event_OpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Event_ShowMessageAndWait(MIA, 0, 20);
    gEventWork->message += 1;
  } else {
    gEventWork->message += 1;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Event_ShowMessageAndWait(MIA, 0, 20);
  }
  Engine_EventWait(20);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(PIXELS(840), -1, PIXELS(632), 1);
  Actor_WalkToAndWait(MIA, 840, 632);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 2);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_GameFlagSet(FLAG_MIA_SENSED_SOMEONE_INSIDE);
  Engine_EventEnd();
}

/// MercuryLighthouseExterior, talking to Mia again. "This statue is blocking
/// the entrance."
static void MercuryLighthouse_MiaDiscussesBlockedEntrance(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventSetMessage(MSGNO(MsgImilStatueBlocksEntrance));
  Engine_EventShowMessageAndWait(MIA, 0, 20);
  Engine_EventEnd();
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
    Engine_TaskWait(2);
  } while (whiteColors <= 7);
}

/// MercuryLighthouseExterior, Move on the door statue: once it stands at column
/// 51 Mia asks whether the party did that, learns of Psynergy, and uses Ply to
/// open the door; she goes in, and Garet and Ivan rejoin the leader.
static void MercuryLighthouse_MiaOpensEntranceWithPly(void) {
  struct FieldActor *actor;
  s32 column;

  actor = Object_GetById(ACTOR_DOOR_STATUE);
  column = actor->x.fixed;
  if (column < 0)
    column += 0xfffff;
  column >>= 20;
  Engine_EventBegin();
  if (column != 51) {
  } else {
    // Mia turns: "Did you do that?"
    Engine_ActorSetSpeed(MIA, 0xcccc, 0x6666);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP,
                              10);
    Engine_EventSetMessage(MsgMercuryDidThat);
    Engine_EventOpenMessage(MIA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      gEventWork->message += 1;
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
      Engine_EventShowMessageAndWait(MIA, 0, 20);
    } else {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
      Event_ShowMessageAndWait(MIA, 0, 20);
      gEventWork->message += 1;
    }
    Engine_ActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT, 60);
    // Garet and Ivan step out to explain Psynergy; Mia tells of Ply.
    actor = Object_GetById(GARET);
    SetEffectRecordMode(actor, 1);
    actor = Object_GetById(IVAN);
    SetEffectRecordMode(actor, 1);
    Actor_SetSpeed(GARET, 0xcccc, 0x6666);
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_SetPosition(GARET, PIXELS(872), PIXELS(600));
    Actor_SetPosition(IVAN, PIXELS(872), PIXELS(600));
    Actor_WalkTo(IVAN, 888, 632);
    Actor_WalkToAndWait(GARET, 880, 616);
    Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
    Engine_ActorWaitForMove(IVAN);
    Actor_FaceDirection(IVAN, FACING_WEST, 0);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(GARET, 0, 20);
    Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 1, 60);
    Event_ShowMessageAndWait(MIA, 0, 20);
    Actor_FaceDirection(GARET, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 20);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(30);
    Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(IVAN, FACING_WEST, 20);
    Engine_EventShowMessageAndWait(IVAN, 0, 20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(MIA, 0, 20);
    Engine_ActorWalkTo(MIA, 840, 656);
    Engine_EventWait(5);
    Engine_ActorFaceDirection(IVAN, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorWaitForMove(MIA);
    Engine_EventWait(10);
    Engine_EventShowMessageAndWait(MIA, 0, 20);
    Actor_FaceDirection(MIA, FACING_NORTH + FACING_STEP, 20);
    Engine_EventShowMessageAndWait(MIA, 0, 20);
    Engine_ActorRunRepeatedMotion(GARET, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(GARET, FACING_NORTH, 20);
    Engine_EventOpenMessage(GARET, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
      gEventWork->message += 1;
    } else {
      gEventWork->message += 1;
      Engine_EventWait(20);
      Engine_ActorSetAnimationAndWait(GARET, ANIM_SHAKE_HEAD);
      Engine_EventShowMessageAndWait(GARET, 0, 20);
    }
    Engine_ActorSetAnimation(MIA, 16);
    Engine_EventWait(30);
    Engine_ActorRunRepeatedMotion(MIA, 1);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(MIA, 0, 20);
    Actor_WalkToAndWait(MIA, 840, 632);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(GARET, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(IVAN, FACING_WEST, 0);
    Engine_EventWait(30);
    // Mia casts Ply on the door: a flash, the light turns white and the door
    // opens.
    MercuryLighthouse_MiaCastsPlyOnEntrance();
    Engine_EventWait(50);
    Engine_AudioPlayCue(SE_131);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x207e9f, 0);
    Engine_ColorBufferInterpolate(10);
    Engine_TaskWait(1);
    Engine_AudioPlayCue(SE_220);
    Engine_TaskWait(40);
    Engine_ColorBufferApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    Engine_AudioPlayCue(SE_209);
    MercuryLighthouse_FadePaletteToWhite();
    Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
    Engine_MapAnimateCells(gMercuryLighthouse_ColumnCells, 116, 35);
    Engine_TaskRemoveCallback(MercuryLighthouse_CycleEntrancePalette);
    Engine_EventWait(20);
    // She nods, jumps up and goes in.
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    Actor_SetSpeed(MIA, 0x30000, 0x18000);
    Engine_ActorJump(MIA, 4, 0);
    Actor_WalkTo(MIA, 840, 600);
    Engine_ActorWaitForMove(MIA);
    Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
    OverlayObject_PrepareObject(PIXELS(840), PIXELS(56), PIXELS(608), 223);
    Actor_SetSpeed(MIA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(MIA, 840, 560);
    Engine_ActorSetPosition(MIA, 0, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Engine_ActorFaceDirection(IVAN, FACING_NORTHWEST + FACING_STEP, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(20);
    // Garet and Ivan rejoin the leader.
    Engine_ActorSetAnimation(GARET, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Engine_ActorSetDestination(GARET, actor->x.part.pixel,
                                 actor->z.part.pixel);
    }
    Engine_ActorSetAnimation(IVAN, ANIM_WALK);
    actor = Object_GetById(ACTOR_PARTY_LEADER);
    if (actor != 0) {
      Actor_SetDestination(IVAN, actor->x.part.pixel, actor->z.part.pixel);
    }
    Engine_ActorWaitForMove(GARET);
    Engine_ActorSetPosition(GARET, 0, 0);
    Engine_ActorWaitForMove(IVAN);
    Engine_ActorSetPosition(IVAN, 0, 0);
    Engine_GameFlagSet(FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE);
  }
  Engine_EventEnd();
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
    Engine_TaskWait(5);
  } while (blackColors != 7);
}

/// Raises an actor into the passage with sparks for 48 ticks, two frames each.
/// Its upward displacement accelerates by about 0.1 pixel per tick.
static void MercuryLighthouse_RaiseActorWithSparks(s32 actorId) {
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  struct FieldActor *actor;
  u32 frame;
  s32 particleX;
  s32 particleY;
  s32 riseSpeed;

  actor = Object_GetById(actorId);
  actor->motionFlags = 0;
  Engine_ActorSetSpriteFlags(actor, 0);
  Actor_SetChildValue(actorId, 0x100);
  Engine_AudioPlayCue(SE_221);
  effectOptions = &options;
  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 0x11e;
  effectOptions->script = gMercuryLighthouse_SparkBurstScript;
  for (frame = 0; frame <= 47; frame++) {
    if (frame <= 31)
      SceneEffect_SpawnWithRandomOffset(actor->x.fixed, actor->y.fixed,
                                        actor->z.fixed);
    if (frame & 1) {
      Engine_AudioPlayCue(SE_246);
      particleX = actor->x.fixed +
                  ((((u32)Engine_RandomNext() * 24) >> 16) << 16) + -0xc0000;
      particleY = actor->y.fixed +
                  ((((u32)Engine_RandomNext() << 5) >> 16) << 16) + -0x100000;
      riseSpeed = ((((u32)Engine_RandomNext() << 2) >> 16) << 15) + 0x8000;
      FieldFx_Spawn(particleX, particleY, actor->z.fixed, 0, riseSpeed, 0,
                    0x330000, effectOptions);
    }
    actor->y.fixed += frame * 0x1999;
    actor->targetY = actor->y.fixed;
    Engine_TaskWait(2);
  }
}

/// MercuryLighthouseExterior, Ply on the waterfall statue: the waterfall
/// darkens and stops, sparkles rise and the way behind it opens. Once Saturos
/// is beaten the leader rises through it alone to the Lighthouse Aerie
/// (MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE); before that Mia
/// rises first and the party follows
/// (MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, the world map's
/// WORLD_MAP_MERCURY_LIGHTHOUSE_LIT, whose lighthouse transition ends at the
/// aerie).
static void MercuryLighthouse_PlyOpensWaterfallPassage(void) {
  struct EffectOptions *effectOptions;
  s32 rowOffset;
  u32 random;
  s32 zero;
  s32 wave;
  s32 particleIndex;
  s32 particleX;
  s32 particleVelocityX;
  struct EffectOptions options;

  Engine_EventBegin();
  Engine_EventWait(20);
  // Remove the colored waterfall before rebuilding the passage tiles.
  MercuryLighthouse_FadePaletteToBlack();
  Engine_TaskRemoveCallback(MercuryLighthouse_CycleWaterfallPalette);
  Map_CopyCellsTo(45, 77, 45, 73, 9, 4);
  Engine_EventWait(30);
  effectOptions = &options;
  effectOptions->priority = 1;
  effectOptions->palette = 5;
  effectOptions->type = 0x11e;
  effectOptions->script = gMercuryLighthouse_SparkBurstScript;
  zero = 0;
  wave = zero;
  // Two bursts spread sparks across the water before it begins to part.
  do {
    u32 particleX, z;

    if ((1 & wave) != 0) {
      Engine_AudioPlayCue(SE_246);
    }
    random = Engine_RandomNext();
    particleX = random * 48;
    particleX >>= 16;
    particleX <<= 16;
    particleX += 0x3000000;
    random = Engine_RandomNext();
    z = random * 56;
    z >>= 16;
    z <<= 16;
    z += 0x880000;
    FieldFx_Spawn(particleX, 0, z, 0, 0, 0, 0x330001, effectOptions);
    Engine_EventWait(2);
    wave = (wave + 1);
  } while ((u32)wave <= 15);
  Engine_EventWait(40);
  zero = 0;
  wave = zero;
  do {
    u32 particleX, z;
    s32 particleSpeed;

    if ((1 & wave) != 0) {
      Engine_AudioPlayCue(SE_246);
    }
    random = Engine_RandomNext();
    particleX = random * 48;
    particleX >>= 16;
    particleX <<= 16;
    particleX += 0x3000000;
    random = Engine_RandomNext();
    z = random * 56;
    z >>= 16;
    z <<= 16;
    z += 0x980000;
    random = Engine_RandomNext();
    particleSpeed = -((random * 10 >> 16) * 0x3333) - 0x3333;
    FieldFx_Spawn(particleX, 0, z, 0, 0, particleSpeed, 0x330001,
                  effectOptions);
    Engine_EventWait(2);
    wave = (wave + 1);
  } while ((u32)wave <= 15);
  Engine_EventWait(60);
  Engine_AudioPlayCue(SE_141);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Engine_EventWait(60);
  effectOptions->palette = 7;
  effectOptions->startScaleX = 0xb333;
  effectOptions->startScaleY = 0xb333;
  effectOptions->targetScaleX = 0x13333;
  effectOptions->targetScaleY = 0x13333;
  zero = 0;
  wave = zero;
  // Reveal the opening one map row at a time, trailing a stream of particles.
  do {
    s32 particleSpeed;

    Map_CopyCellsTo(59, (12 - wave), 48, (12 - wave), 3, 1);
    particleIndex = 0;
    rowOffset = (wave << 4);
    do {
      random = Engine_RandomNext();
      particleX = ((((((random << 1) + random) << 4) >> 16) << 16) + 0x3000000);
      particleVelocityX =
          (0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16)) - 0x6664;
      particleSpeed = 0x1999 * ((u32)(Engine_RandomNext() << 3) >> 16);
      FieldFx_Spawn(particleX, 0,
                    ((-((u32)particleIndex >> 1) - rowOffset) << 16) + 0xc00000,
                    particleVelocityX, 0, particleSpeed, 0xd0001,
                    effectOptions);
      particleIndex = (particleIndex + 1);
      Engine_EventWait(2);
    } while ((u32)particleIndex <= 31);
    wave = (wave + 1);
  } while ((u32)wave <= 3);
  Engine_AudioPlayCue(SE_289);
  Map_SetShake(-1, -1, 0xe666);
  Engine_MapRenderWaitForValues();
  BattleFx_PlayQueuedSound();
  Engine_EventWait(30);
  // After Saturos the leader goes alone; the first time Mia leads the party.
  if (GameFlag_IsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) != 0) {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 824, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 792, 200);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 18);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_PARTY_LEADER);
    Engine_EventWait(60);
    Engine_TaskRemoveCallback(MercuryLighthouse_CycleWaterfallPalette);
    ColorBuffer_ApplySource(0x10000, 0);
    ColorBuffer_ApplyTarget(0x10005, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(60);
    Engine_EventRequestExit(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE);
    Engine_EventEnd();
  } else {
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GARET, 0xcccc, 0x6666);
    Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
    Actor_SetSpeed(MIA, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 824, 240);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
    Actor_SetPosition(MIA, PIXELS(824), PIXELS(240));
    Actor_WalkToAndWait(MIA, 792, 232);
    Actor_FaceDirection(MIA, FACING_NORTH, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(MIA, FACING_SOUTHEAST, 20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(40);
    Actor_FaceDirection(MIA, FACING_NORTH, 20);
    Actor_WalkToAndWait(MIA, 792, 200);
    MercuryLighthouse_RaiseActorWithSparks(MIA);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_EventWait(30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 232);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_SetPosition(GARET, PIXELS(792), PIXELS(232));
    Actor_SetPosition(IVAN, PIXELS(792), PIXELS(232));
    Actor_WalkTo(GARET, 816, 224);
    Actor_WalkToAndWait(IVAN, 768, 224);
    Engine_ActorWaitForMove(GARET);
    Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 80);
    Actor_FaceDirection(GARET, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IVAN, FACING_SOUTHEAST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_EventWait(60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 224);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Actor_FaceDirection(IVAN, FACING_EAST, 0);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 208);
    Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 792, 200);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 18);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT, 0);
    Engine_ActorStartRepeatedMotion(GARET, 2);
    Engine_ActorStartRepeatedMotion(IVAN, 2);
    MercuryLighthouse_RaiseActorWithSparks(ACTOR_PARTY_LEADER);
    Engine_EventWait(60);
    Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 0);
    Actor_ShowEmote(IVAN, EMOTE_IN_FRONT | 2, 80);
    Engine_ActorFaceEachOther(GARET, IVAN, 20);
    Engine_ActorSetAnimation(GARET, ANIM_NOD);
    Engine_ActorSetAnimationAndWait(IVAN, ANIM_NOD);
    Engine_EventWait(40);
    Actor_WalkToAndWait(GARET, 792, 216);
    Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
    Actor_WalkToAndWait(GARET, 792, 200);
    Engine_EventWait(30);
    MercuryLighthouse_RaiseActorWithSparks(GARET);
    Actor_WalkToAndWait(IVAN, 792, 216);
    Actor_WalkToAndWait(IVAN, 792, 200);
    Engine_EventWait(30);
    MercuryLighthouse_RaiseActorWithSparks(IVAN);
    ColorBuffer_ApplySource(0x10000, 0);
    Engine_TaskRemoveCallback(MercuryLighthouse_CycleWaterfallPalette);
    ColorBuffer_ApplyTarget(0x10005, 0);
    Engine_ColorBufferInterpolate(120);
    Engine_EventWait(120);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_EventWait(60);
    Engine_EventEnd();
    Engine_EventRequestExit(
        MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT);
  }
}

/// MercuryLighthouseExterior, searching trigger 42 shows the fountain's sign.
/// "Fountain of Healing / Water of Hermes: It brings rejuvenation to all that
/// lives."
static void MercuryLighthouse_ReadHermesFountainInscription(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MSGNO(MsgImilHermesHealingWaterFountain), 1);
  Engine_EventEnd();
}

/// MercuryLighthouseMiddle or MercuryLighthouseUpper, searching trigger 20: the
/// door is locked tight.
static void MercuryLighthouse_InspectLockedDoor(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Engine_EventEnd();
}

/// MercuryLighthouseExterior, searching the Fountain of Healing (trigger 5)
/// before taking its water: it seems dry, or once Saturos is beaten flows with
/// water; carrying an Empty Bottle, the item menu then opens.
static void MercuryLighthouse_InspectHealingFountain(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  if (Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY) == 0)
    Engine_MessageShowCentered(MSGNO(MsgMercuryLighthouseFountainSeemsDry), 1);
  else
    Engine_MessageShowCentered(
        MSGNO(MsgMercuryLighthouseFountainFlowsWithWater), 1);
  if (PartyInventory_FindOwner(ITEM_EMPTY_BOTTLE) != -1) {
    // The field loop handles this menu request after the inspection ends.
    s16 *menuRequest = &((struct FieldWork *)gEventWork)->menu;
    s32 enabled = 1;

    *menuRequest = enabled;
  }
  Engine_EventEnd();
}

/// MercuryLighthouseExterior, using the Empty Bottle at the fountain
/// (USE_ITEM(ITEM_EMPTY_BOTTLE)) once Saturos is beaten: the bottle becomes
/// Hermes' Water, once a visit.
static void MercuryLighthouse_FillHermesWaterBottle(void) {
  Engine_EventBegin();
  Engine_AudioPlayCue(MUS_TREASURE_FOUND);
  Engine_ItemShowFound(ITEM_HERMES_WATER, 3);
  MercuryLighthouse_ReplaceInventoryItem(ITEM_EMPTY_BOTTLE, ITEM_HERMES_WATER);
  UiWork_PushValueSlot(PartyInventory_FindOwner(ITEM_HERMES_WATER), 1);
  UiWork_PushValueSlot(ITEM_HERMES_WATER, 2);
  Engine_MessageShowCentered(MSGNO(MsgMercuryLighthouseIsaacGot), 1);
  Engine_GameFlagSet(FLAG_HERMES_WATER_TAKEN);
  Engine_EventEnd();
}

/// MercuryLighthouseLower or MercuryLighthouseMiddle, walking south into
/// trigger 15: on the first the leader steps to the hole and drops into
/// MercuryLighthouseEntrance
/// (MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE, its
/// MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_LOWER_HOLE); on the
/// second the motion runs alone.
static void MercuryLighthouse_EnterRisingPassage(void) {
  Engine_EventBegin();
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseLower) {
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 472, 600);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
    Camera_MoveTo(PIXELS(464), -1, PIXELS(656), 1);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
    OverlayObject_PrepareObject(Object_GetById(ACTOR_PARTY_LEADER)->x.fixed, 0,
                                PIXELS(702), 223);
    Map_CopyCellsTo(92, 46, 92, 40, 3, 2);
    Object_GetById(ACTOR_PARTY_LEADER)->gravity = 0x8000;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
    FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, -1);
    gEventWork->startTransition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
    Engine_EventWait(60);
    Engine_EventRequestExit(
        MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_HOLE);
  } else {
    FieldBattleFx_RunRisingObjectSequence(ACTOR_PARTY_LEADER, 6, -1);
  }
  Engine_EventEnd();
}

#include "common/mercury_lighthouse_arrive.h"

/// Reveals the stair's map rows with particles along their edges.
static void SceneEffect_SpawnParticleRowsAndDrawTiles(void) {
  struct EffectOptions options;
  u32 particleIndex, row;

  Map_CopyCellsTo(0x4a, 0x3a, 0x46, 0x22, 1, 1);
  options.palette = 7;
  options.startScaleX = 0x8000;
  options.startScaleY = 0x8000;
  for (row = 0; row <= 1; row++) {
    for (particleIndex = 0; particleIndex <= 7; particleIndex++) {
      if ((particleIndex & 1) != 0) {
        s32 velocityX =
            (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
        s32 velocityZ =
            (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

        FieldFx_Spawn(0x690000, 0,
                      ((-particleIndex - (row << 4)) << 16) + 0x2200000,
                      velocityX, 0, velocityZ, 0x90000, &options);
        Engine_EventWait(1);
      }
    }
    Map_CopyCellsTo(0x4a, 0x3b, 0x46, 34 - row, 1, 1);
    Map_CopyCellsTo(0x4a, 0x3a, 0x46, 33 - row, 1, 1);
  }
}

/// Restores the stair tiles in three particle-covered steps.
static void MercuryLighthouse_OpenThreeStepStair(void) {
  struct EffectOptions options;
  struct EffectOptions *effectOptions;
  u32 row;
  u32 particleIndex;
  s32 offsetZ;

  Map_CopyCellsTo(76, 61, 74, 38, 1, 1);
  effectOptions = &options;
  effectOptions->palette = 5;
  effectOptions->startScaleX = 0x8000;
  effectOptions->startScaleY = 0x8000;
  for (row = 0; row <= 2; row++) {
    offsetZ = -0x20000;
    for (particleIndex = 1; particleIndex <= 7; particleIndex++) {
      if (particleIndex & 1) {
        if (particleIndex & 2) {
          FieldFx_Spawn((105 - (((u32)Engine_RandomNext() * 5) >> 16)) << 16, 0,
                        offsetZ - (row << 19) + 0x22e0000, 0, 0, -0x4000,
                        0x90000, effectOptions);
        } else {
          FieldFx_Spawn((((row << 2) + particleIndex) << 17) + 0xb70000, 0,
                        (0x26c - (((u32)Engine_RandomNext() * 5) >> 16)) << 16,
                        0x4000, 0, 0, 0x90000, effectOptions);
        }
        Engine_EventWait(1);
      }
      offsetZ += -0x20000;
    }
    Map_CopyCellsTo(71, 59, 70, 34 - row, 1, 1);
    Map_CopyCellsTo(71, 59, row + 75, 38, 1, 1);
  }
}

/// Emits particles around the leader once every four frames.
static void SceneEffect_SpawnParticleEveryFourthFrame(void) {
  struct EffectOptions options;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  s32 framePhase = gFrameCount & 3;

  if (framePhase == 0) {
    options.palette = 7;
    if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
      options.palette = 5;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    {
      s32 y = leader->y.fixed + ((((u32)Engine_RandomNext() << 2) >> 16) << 16);
      s32 velocityX =
          (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
      s32 velocityY =
          (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

      FieldFx_Spawn(leader->x.fixed, y, leader->z.fixed, velocityX, velocityY,
                    framePhase, 0x90000, &options);
    }
  }
}

/// Emits particles at a fixed point once every four frames.
static void SceneEffect_SpawnRandomEveryFourFrames(s32 x, s32 y, s32 z) {
  struct EffectOptions options;
  s32 framePhase = gFrameCount & 3;

  if (framePhase == 0) {
    options.palette = 7;
    if (((((u32)Engine_RandomNext() << 1) >> 16) & 1) == 0)
      options.palette = 5;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    {
      s32 velocityX =
          (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;
      s32 velocityY =
          (((u32)Engine_RandomNext() << 3) >> 16) * 0x3333 + 0xffff3334;

      FieldFx_Spawn(x, y, z, velocityX, velocityY, framePhase, 0x90000,
                    &options);
    }
  }
}

/// Emits a stationary particle near the supplied position.
static void SceneEffect_SpawnWithRandomOffset(s32 x, s32 y, s32 z) {
  struct EffectOptions options;

  options.palette = 7;
  options.priority = 1;
  options.startScaleX = 0xb333;
  options.startScaleY = 0xb333;
  {
    s32 particleX =
        x + ((((u32)Engine_RandomNext() << 4) >> 16) << 16) + 0xfff80000;
    s32 particleZ =
        z + ((((u32)Engine_RandomNext() << 3) >> 16) << 16) + 0xfffc0000;

    FieldFx_Spawn(particleX, y, particleZ, 0, 0, 0, 0xb0000, &options);
  }
}

/// Emits a short burst with velocities spreading along the supplied angle.
static void SceneEffect_SpawnRandomizedBurst(s32 x, s32 y, s32 z, s32 angle) {
  struct EffectOptions options;
  s32 velocity[3];
  u32 particleIndex;

  Engine_AudioPlayCue(SE_216);
  particleIndex = 0;
  do {
    if ((particleIndex & 1) != 0) {
      options.palette = 7;
      if ((particleIndex & 2) != 0)
        options.palette = 5;
      options.startScaleX = 0x9999;
      options.startScaleY = 0x9999;
      SET_VECTOR(velocity, 0, 0, 0);
      Vector_AddPolarOffset((6 - (particleIndex >> 1)) * 0x1999, angle,
                            velocity);
      {
        s32 particleX =
            x + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);
        s32 particleZ =
            z + ((6 - (((u32)Engine_RandomNext() * 6) >> 16)) << 16);

        FieldFx_Spawn(particleX, y, particleZ, velocity[0], velocity[1],
                      velocity[2], 0x90000, &options);
      }
    }
    Engine_TaskWait(2);
    particleIndex++;
  } while (particleIndex <= 11);
}

/// Restores each lighthouse scene's puzzle state and entrance sequence.
s32 MercuryLighthouse_Initialize(void) {
  u32 statueIndex;
  s32 positionFlag;
  s32 pillarX;
  s32 keepPartyPosition;

  Engine_GameFlagSet(FLAG_MERCURY_LIGHTHOUSE_LIT);
  // Set up common lighting before restoring each scene's puzzle-specific state.
  gGameState.retreatEntrance = MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP;
  gGameState.retreatScene = (s32)&SceneId_MercuryLighthouseExterior;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Io_Write16(BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND, &REG_BLDCNT);
  {
    s32 alpha = BLEND_ALPHA(16, 16);
    REG_BLDALPHA = alpha;
  }
  MercuryLighthouse_RestoreLights(
      21, (struct MercuryLighthouseLights *)gSceneState);
  BattleFx_StartFadeOverlay(0);
  if (Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
    FieldScheduler_AddCallback(MercuryLighthouse_CyclePalette,
                               TASK_PRIORITY_SCENE);
  else
    MercuryLighthouse_ClearPalette();
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseLower) {
    // Entrance pillars, the channel puzzle and Mia's rescue share this overlay.
    switch (gGameState.entrance) {
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST:
    case MERCURY_LIGHTHOUSE_LOWER_POOL_FROM_STATUE:
      if (Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED))
        Map_CopyCellAttributes(66, 5, 27, 23, 2, 5);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_FROM_STATUE:
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_PIPE:
      SetEffectRecordMode(Object_GetById(ACTOR_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST:
      if (Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_CHANNELS_OPEN)) {
        Engine_TaskWait(1);
        Actor_SetPosition(ACTOR_CHANNEL_PILLAR_1, PIXELS(672), PIXELS(128));
        Actor_SetPosition(ACTOR_CHANNEL_PILLAR_2, PIXELS(704), PIXELS(192));
        Actor_SetPosition(ACTOR_CHANNEL_PILLAR_3, PIXELS(648), PIXELS(240));
      } else if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        Engine_GameFlagClear(FLAG_PIPE_1_JOINED);
        Engine_GameFlagClear(FLAG_PIPE_2_JOINED);
        Engine_GameFlagClear(FLAG_PIPE_3_JOINED);
        Engine_GameFlagClear(FLAG_PIPE_313);
      }
      SetEffectRecordMode(Object_GetById(ACTOR_CHANNEL_PILLAR_1), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_CHANNEL_PILLAR_2), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_CHANNEL_PILLAR_3), 1);
      FieldScene_RedrawActorFootprint(ACTOR_CHANNEL_PILLAR_1);
      FieldScene_RedrawActorFootprint(ACTOR_CHANNEL_PILLAR_2);
      FieldScene_RedrawActorFootprint(ACTOR_CHANNEL_PILLAR_3);
      SetEffectRecordMode(Object_GetById(ACTOR_CHANNEL_FIXED_PILLAR), 1);
      if (Engine_GameFlagIsSet(FLAG_PIPE_1_JOINED)) {
        Map_CopyCellsTo(119, 9, 109, 11, 1, 1);
        if (Engine_GameFlagIsSet(FLAG_PIPE_2_JOINED))
          Map_CopyCellsTo(118, 9, 104, 13, 1, 1);
        Engine_MapRedraw();
        Engine_TaskWait(1);
      }
      break;
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_POOL:
    case MERCURY_LIGHTHOUSE_LOWER_STATUE_FROM_WATERFALLS:
      if (!Engine_GameFlagIsSet(FLAG_MIA_JOINED_PARTY)) {
        Actor_SetPosition(MIA, PIXELS(696), PIXELS(632));
        Engine_ActorFaceDirection(MIA, FACING_EAST, 0);
      } else {
        Actor_SetPosition(ACTOR_STATUE_BLOCKING_MIA, PIXELS(776), PIXELS(632));
        Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
      }
      break;
    }
  }
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseMiddle) {
    // Restore stairs, the floor-switch gate and the fountain's moved pillar.
    switch (gGameState.entrance) {
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_SOUTH:
    case MERCURY_LIGHTHOUSE_MIDDLE_PIPES_FROM_GATE:
      SetEffectRecordMode(Object_GetById(ACTOR_STAIR_PILLAR), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_STAIR_FIXED_PILLAR), 1);
      if (Engine_GameFlagIsSet(FLAG_STAIRS_RAISED)) {
        Engine_TaskWait(1);
        Engine_AudioPlayCue(SE_211);
        Actor_SetPosition(ACTOR_STAIR_PILLAR, PIXELS(184), PIXELS(528));
        Map_CopyCellAttributes(11, 31, 1, 4, 9, 31);
        Map_CopyCellAttributes(7, 30, 1, 4, 11, 31);
        Map_CopyCellsTo(74, 58, 70, 32, 1, 2);
        Map_CopyCellsTo(74, 59, 70, 34, 1, 1);
        Map_CopyCellsTo(76, 60, 74, 38, 3, 1);
        Map_CopyCellsTo(77, 60, 76, 38, 2, 1);
        Map_CopyCellsTo(75, 58, 86, 41, 1, 3);
        Map_CopyCellsTo(75, 59, 86, 43, 1, 2);
        Map_CopyCellsTo(76, 59, 80, 49, 2, 1);
        Map_CopyCellsTo(77, 59, 82, 49, 2, 1);
      }
      break;
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_LOWER_NORTH:
    case MERCURY_LIGHTHOUSE_MIDDLE_GATE_FROM_PIPES:
      Engine_MapRedraw();
      Engine_TaskWait(1);
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
          Engine_GameFlagIsSet(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
        Map_CopyCellsTo(9, 1, 9, 7, 1, 2);
      }
      if (Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN)) {
        Engine_ActorSetPosition(ACTOR_GATE_STATUE, PIXELS(88), PIXELS(216));
        Object_GetById(ACTOR_GATE_STATUE)->y.fixed += -0x20000;
        Object_GetById(ACTOR_GATE_STATUE)->targetY =
            Object_GetById(ACTOR_GATE_STATUE)->y.fixed;
        Map_CopyCellsTo(9, 1, 9, 7, 1, 2);
        Map_CopyCellsTo(5, 2, 5, 11, 1, 1);
        Map_CopyCellAttributes(9, 5, 1, 1, 9, 10);
      }
#if TBS_PATCH_3
      FieldScheduler_AddCallback(MercuryLighthouse_UpdateLeaderEffectTarget,
                                 TASK_PRIORITY_SCENE);
#endif
      break;
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST:
    case MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST:
      SetEffectRecordMode(Object_GetById(ACTOR_FOUNTAIN_PILLAR), 1);
      if (Engine_GameFlagIsSet(FLAG_FOUNTAIN_RUNNING)) {
        MercuryLighthouse_OpenFountainWithSpray(0);
        Map_CopyCellAttributes(42, 41, 4, 1, 42, 39);
        Map_CopyCellAttributes(42, 40, 4, 1, 42, 41);
        Actor_SetPosition(ACTOR_FOUNTAIN_PILLAR, PIXELS(704), PIXELS(640));
      }
      break;
    }
  }
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseUpper) {
    // Restore movable pillars and the pillar-room door arrangement.
    switch (gGameState.entrance) {
    case MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_STAIR_GATE:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST:
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_CROSSING_STATUE), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_CROSSING_STATUE + 1), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_CROSSING_STATUE + 2), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_CROSSING_STATUE + 3), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_LAST_CROSSING_STATUE), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_PARTY_LEADER), 1);
      statueIndex = 0;
      pillarX = PIXELS(632);
      positionFlag = FLAG_FIRST_ROW_STATUE;
      for (; statueIndex <= 3; statueIndex++) {
        if (Engine_GameFlagIsSet(positionFlag))
          Engine_ActorSetPosition(statueIndex + ACTOR_FIRST_CROSSING_STATUE,
                                  pillarX, PIXELS(88));
        else if (Engine_GameFlagIsSet(positionFlag + 1))
          Engine_ActorSetPosition(statueIndex + ACTOR_FIRST_CROSSING_STATUE,
                                  pillarX + PIXELS(32), PIXELS(88));
        pillarX += PIXELS(64);
        positionFlag += 2;
      }
      if (Engine_GameFlagIsSet(FLAG_LAST_ROW_STATUE_WEST)) {
        Actor_SetPosition(ACTOR_LAST_CROSSING_STATUE, PIXELS(920), PIXELS(88));
        Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      } else if (Engine_GameFlagIsSet(FLAG_LAST_ROW_STATUE_EAST)) {
        Actor_SetPosition(ACTOR_LAST_CROSSING_STATUE, PIXELS(952), PIXELS(88));
        Map_CopyCellAttributes(53, 10, 1, 1, 58, 7);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST:
    case MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS:
      if (Engine_GameFlagIsSet(FLAG_PASSAGE_STATUE_MOVED)) {
        Actor_SetPosition(ACTOR_GAP_STATUE, PIXELS(456), PIXELS(328));
        Map_CopyCellAttributes(29, 20, 1, 1, 31, 20);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHWEST:
    case MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP:
      MercuryLighthouse_ArriveWithSparks();
      // Fall through.
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST:
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST:
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      break;
    case MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER:
    case MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_FIVE_PILLARS:
    case MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS:
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) &&
          Engine_GameFlagIsSet(FLAG_LEADER_ON_FLOOR_SWITCH)) {
        Object_GetById(ACTOR_PARTY_LEADER)->y.fixed = -0x20000;
        Object_GetById(ACTOR_PARTY_LEADER)->targetY =
            Object_GetById(ACTOR_PARTY_LEADER)->y.fixed;
        Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
        Map_CopyCellsTo(10, 28, 10, 18, 1, 2);
      }
      if (Engine_GameFlagIsSet(FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN)) {
        Engine_ActorSetPosition(ACTOR_FALLEN_STATUE, PIXELS(168), PIXELS(376));
        Object_GetById(ACTOR_FALLEN_STATUE)->y.fixed += -0x20000;
        Object_GetById(ACTOR_FALLEN_STATUE)->targetY =
            Object_GetById(ACTOR_FALLEN_STATUE)->y.fixed;
        Map_CopyCellsTo(6, 29, 10, 23, 1, 1);
        Map_CopyCellsTo(10, 28, 10, 18, 1, 2);
        Map_CopyCellAttributes(10, 16, 1, 1, 10, 19);
        Engine_MapRedraw();
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE_STATUE_FALLS:
      Engine_TaskWait(1);
#if TBS_PATCH_2
      if (Engine_GameFlagIsSet(FLAG_DOOR_ROOM_PILLAR_PUSHED))
        Actor_SetPosition(ACTOR_DOOR_ROOM_PILLAR, PIXELS(816), PIXELS(608));
      if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_48))
        Actor_SetPosition(ACTOR_DOOR_SWITCH_PILLAR, PIXELS(776), PIXELS(576));
      else if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47))
        Actor_SetPosition(ACTOR_DOOR_SWITCH_PILLAR, PIXELS(760), PIXELS(576));
      else if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_53))
        Actor_SetPosition(ACTOR_DOOR_SWITCH_PILLAR, PIXELS(856), PIXELS(576));
#else
      Actor_SetPosition(ACTOR_DOOR_ROOM_PILLAR, PIXELS(816), PIXELS(608));
      Engine_ActorSetPosition(ACTOR_DOOR_SWITCH_PILLAR, PIXELS(776),
                              PIXELS(576));
#endif
      SetEffectRecordMode(Object_GetById(ACTOR_PARTY_LEADER), 1);
      MercuryLighthouse_CloseStoneDoor(0);
#if TBS_PATCH_2
      if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47)) {
        MercuryLighthouse_OpenStoneDoor(2);
        if (Object_GetById(ACTOR_SLIDING_STATUE)->z.fixed >> 20 == 44)
          FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue,
                                     TASK_PRIORITY_SCENE);
      } else if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_48)) {
        MercuryLighthouse_OpenStoneDoor(1);
      } else if (!Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_53)) {
        MercuryLighthouse_OpenStoneDoor(0);
      }
#else
      MercuryLighthouse_OpenStoneDoor(1);
#endif
      // Fall through.
    case MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE:
      if (gGameState.entrance ==
          MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE)
        Engine_AudioPlayCue(SE_211);
      SetEffectRecordMode(Object_GetById(ACTOR_SLIDING_STATUE), 1);
      Engine_ActorSetSpritePriority(ACTOR_DOOR_ROOM_PILLAR, 2);
      Object_GetById(ACTOR_DOOR_ROOM_PILLAR)->terrainId = 2;
      SetEffectRecordMode(Object_GetById(ACTOR_DOOR_SWITCH_PILLAR), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_DOOR_FIXED_PILLAR), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_DOOR_FIXED_PILLAR + 1), 1);
      SetEffectRecordMode(Object_GetById(ACTOR_FIRST_DOOR_FIXED_PILLAR + 2), 1);
      FieldScene_RedrawActorFootprint(ACTOR_DOOR_ROOM_PILLAR);
      FieldScene_RedrawActorFootprint(ACTOR_DOOR_SWITCH_PILLAR);
      if (gGameState.entrance !=
          MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE)
        break;
      if (!Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION)) {
        Engine_GameFlagClear(FLAG_DOOR_ROOM_PILLAR_PUSHED);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_47);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_48);
        Engine_GameFlagClear(FLAG_DOOR_SWITCH_PILLAR_AT_53);
        break;
      }
      MercuryLighthouse_CloseStoneDoor(0);
      if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_47)) {
        MercuryLighthouse_OpenStoneDoor(2);
        if (Object_GetById(ACTOR_SLIDING_STATUE)->z.fixed >> 20 == 44)
          FieldScheduler_AddCallback(MercuryLighthouse_SprayAboveStatue,
                                     TASK_PRIORITY_SCENE);
      } else if (Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_48)) {
        MercuryLighthouse_OpenStoneDoor(1);
      } else if (!Engine_GameFlagIsSet(FLAG_DOOR_SWITCH_PILLAR_AT_53)) {
        MercuryLighthouse_OpenStoneDoor(0);
      }
      break;
    case MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS_STATUE_FALLS:
      Engine_EventBegin();
      SetEffectRecordMode(Object_GetById(ACTOR_FALLEN_STATUE), 1);
      Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
      Object_GetById(ACTOR_FALLEN_STATUE)->y.fixed = PIXELS(144);
      Object_GetById(ACTOR_FALLEN_STATUE)->motionFlags = 0;
      Object_GetById(ACTOR_FALLEN_STATUE)->bounce = 0;
      Object_GetById(ACTOR_FALLEN_STATUE)->gravity = 0x4ccc;
      gEventWork->startTransition =
          SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      Object_GetById(ACTOR_FALLEN_STATUE)->motionFlags = 3;
      Engine_AudioPlayCue(SE_189);
      Engine_EventWait(32);
      Engine_AudioPlayCue(SE_188);
      SetEffectRecordMode(Object_GetById(ACTOR_FALLEN_STATUE), 2);
      Map_SetShake(192 << 10, 192 << 10, 128 << 9);
      Map_SetShake(-1, -1, 0xe666);
      Engine_MapRenderWaitForValues();
      Engine_EventWait(60);
      Engine_EventRequestExit(
          MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_STONE_DOORS_STATUE_FALLS);
      Engine_EventEnd();
      break;
    }
  }
  if (gGameState.scene == (s32)&SceneId_MercuryLighthouseExterior) {
    // This scene contains Mia's entrance, the lift and the healing fountain.
    switch (gGameState.entrance) {
    case MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_WALKWAY:
      FieldScheduler_AddCallback(MercuryLighthouse_CycleWaterfallPalette,
                                 TASK_PRIORITY_SCENE);
      if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION))
        break;
      MercuryLighthouse_ArriveWithSparks();
      BattleFx_SetQueuedSoundAndPlay(SE_170);
      ColorBuffer_ApplySource(128 << 9, 0);
      Engine_ColorBufferApplyTarget(0x10003, 1);
      Engine_ColorBufferInterpolate(30);
      Engine_EventWaitForScreen();
      Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Engine_EventWait(30);
      UiText_ShowCenteredMessage(MsgMercuryHonorsGoddessRainbows, 0, 0);
      Engine_ColorBufferApplyTarget(128 << 9, 0);
      Engine_ColorBufferInterpolate(30);
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE:
      Engine_ActorSetPosition(MIA, 0, 0);
      DisplayBlend_SuspendScript();
      Io_Write16(0, &REG_BLDCNT);
      Object_GetById(ACTOR_EAST_APPROACH_STATUE)->scaleX = -0x10000;
      Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR, 1);
      Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR + 1, 1);
      Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR + 2, 1);
      keepPartyPosition = Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION);
      if (keepPartyPosition) {
        Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
        Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
        break;
      }
      Actor_SetPosition(ACTOR_DOOR_STATUE, PIXELS(824), PIXELS(600));
      Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
      Object_GetById(ACTOR_DOOR_STATUE)->motionFlags = keepPartyPosition;
      SetEffectRecordMode(Object_GetById(ACTOR_DOOR_STATUE), 1);
      Engine_EventBegin();
      Camera_MoveTo(-1, -1, -1, 0);
      Engine_EventGetViewCenter()->motionFlags = keepPartyPosition;
      Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
      Engine_ActorSetSpritePriority(ACTOR_LIFT_STATUE, 1);
      MercuryLighthouse_RideLift();
      DisplayBlend_ResumeScript();
      Engine_EventEnd();
      break;
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_ENTRANCE:
      Engine_EventWait(1);
      SetEffectRecordMode(Object_GetById(ACTOR_PARTY_LEADER), 1);
      // Fall through.
    case MERCURY_LIGHTHOUSE_EXTERIOR_FROM_WORLD_MAP:
      Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
      Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
      if (!Engine_GameFlagIsSet(FLAG_SATUROS_BEATEN_AT_MERCURY)) {
        Map_CopyCellsTo(30, 45, 50, 45, 5, 6);
        Map_CopyCellsTo(50, 105, 50, 109, 5, 3);
        Engine_MapRedraw();
        Engine_TaskWait(1);
      } else {
        Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR, 1);
        Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR + 1, 1);
        Engine_ActorSetSpritePriority(ACTOR_FIRST_VISITOR + 2, 1);
      }
      Object_GetById(ACTOR_EAST_APPROACH_STATUE)->scaleX = -0x10000;
      // Until Mia has hurried here from Imil (FLAG_IMIL_MET_MIA, set in
      // imil.c), she is not here and the door stays shut.
      if (!Engine_GameFlagIsSet(FLAG_IMIL_MET_MIA)) {
        Engine_ActorSetPosition(MIA, 0, 0);
        Actor_SetPosition(ACTOR_DOOR_STATUE, PIXELS(824), PIXELS(600));
        Engine_ActorSetSpritePriority(ACTOR_DOOR_STATUE, 1);
        Map_CopyCellAttributes(116, 37, 3, 3, 52, 37);
        Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
        FieldScheduler_AddCallback(MercuryLighthouse_CycleEntrancePalette,
                                   TASK_PRIORITY_SCENE);
        break;
      }
      if (!Engine_GameFlagIsSet(FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE)) {
        if (!Engine_GameFlagIsSet(FLAG_MIA_SENSED_SOMEONE_INSIDE)) {
          Engine_ActorFaceDirection(MIA, FACING_EAST, 0);
          Engine_ActorSetAnimation(MIA, 16);
        } else {
          Actor_SetPosition(MIA, PIXELS(840), PIXELS(632));
          Actor_FaceDirection(MIA, FACING_NORTH, 0);
        }
        SetEffectRecordMode(Object_GetById(MIA), 1);
        Map_CopyCellsTo(126, 35, 116, 35, 1, 2);
        FieldScheduler_AddCallback(MercuryLighthouse_CycleEntrancePalette,
                                   TASK_PRIORITY_SCENE);
      } else {
        Engine_ActorSetPosition(MIA, 0, 0);
        Actor_SetPosition(ACTOR_DOOR_STATUE, PIXELS(824), PIXELS(600));
        Map_CopyCellAttributes(116, 36, 3, 4, 52, 36);
        Object_GetById(ACTOR_DOOR_STATUE)->collisionFlags = 254;
        Engine_EventWait(1);
      }
      Object_GetById(ACTOR_DOOR_STATUE)->motionFlags = 0;
      SetEffectRecordMode(Object_GetById(ACTOR_DOOR_STATUE), 1);
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
  struct FieldActor *leader;

  pillarColumn = Actor_Get(ACTOR_STATUE_BLOCKING_MIA)->x.fixed / 0x100000;
  if (pillarColumn != 48) {
    return;
  }
  Engine_EventBegin();
  firstMessage = MsgMercurySavedMeAgain;
  Engine_EventSetMessage(firstMessage);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventWait(60);
  Engine_ActorSetAnimation(MIA, 16);
  Engine_EventWait(50);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_EventOpenMessage(MIA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    // Each "no" brings "Please, don't hide anything." until the party says
    // yes.
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(MIA, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventShowMessageAndWait(MIA, 0, 20);
    Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
    Engine_EventWait(20);
    Engine_EventOpenMessage(MIA, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Engine_EventWait(20);
      /* FAKEMATCH(wrapper): called through an unprototyped cast, which loads
       * r1 before r0; called by its prototype, 1 of 222 instructions differ in
       * tbs-en. */
      MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(MIA, ANIM_SHAKE_HEAD);
      Engine_EventWait(20);
      questionMessage = firstMessage + 5;
      for (;;) {
        Engine_EventSetMessage(questionMessage);
        Engine_EventOpenMessage(MIA, 0);
        if (Engine_EventChooseYesNo(0, 0) != 1) {
          break;
        }
        Engine_EventWait(20);
        /* FAKEMATCH(wrapper): called through an unprototyped cast, which
         * loads r1 before r0; called by its prototype, 1 of 222 instructions
         * differ in tbs-en. */
        MERCURY_LIGHTHOUSE_ANIMATE_ACTOR_AND_WAIT(MIA, ANIM_SHAKE_HEAD);
        Engine_EventWait(20);
        questionMessage = MsgMercuryDontHideTruth;
      }
    }
  }
  Engine_EventSetMessage(MsgMercuryThoughtSo);
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Engine_ActorWalkToAndWait(MIA, 728, 632);
  Engine_EventWait(20);
  Engine_EventShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimation(MIA, 16);
  Engine_EventShowMessageAndWait(MIA, 0, 20);
  Engine_ActorSetAnimation(MIA, ANIM_STAND);
  Engine_ActorFaceActor(MIA, ACTOR_PARTY_LEADER, 20);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Actor_ShowEmote(MIA, EMOTE_IN_FRONT | 5, 90);
  Engine_ActorSetAnimationAndWait(MIA, ANIM_NOD);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(MIA, 0, 20);
  Event_PrepareObjectAndApplyValue(MIA, 1);
  // "The Adept Mia joined your party!" Fizz, a Mercury Djinni, comes with
  // her and is set; her stats are worked out again.
  Engine_GameFlagSet(FLAG_DJINNI_0 + DJINNI_FIZZ);
  Djinn_AddToOwner(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  Djinn_Activate(MIA, ELEMENT_MERCURY, DJINNI_INDEX(DJINNI_FIZZ));
  UnitCalc(MIA);
  Engine_ActorSetAnimation(MIA, ANIM_WALK);
  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Engine_ActorSetDestination(MIA, leader->x.part.pixel, leader->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Engine_ActorSetPosition(MIA, 0, 0);
  Map_CopyCellAttributes(110, 39, 5, 1, 46, 39);
  Engine_GameFlagSet(FLAG_MIA_JOINED_PARTY);
  Engine_EventEnd();
}

/// MercuryLighthouseExterior,
/// MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_AERIE: the leader rides
/// the lift down from the Lighthouse Aerie beside its statue, hops off onto the
/// landing by the Fountain of Healing, and the lift goes back up.
static void MercuryLighthouse_RideLift(void) {
  struct MapLayerScroll *liftLayer;
  s32 liftSpeed;

  liftLayer =
      &((struct MapScrollWork *)gMapWork[0])->layers[MAP_LAYER_OVERHEAD];
  liftSpeed = 0x9c28;
  liftLayer->offsetY = PIXELS(1161);
  liftLayer->speedY = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 0;
  Actor_Get(ACTOR_PARTY_LEADER)->z.fixed += -PIXELS(137);
  Actor_Get(ACTOR_PARTY_LEADER)->targetZ =
      Actor_Get(ACTOR_PARTY_LEADER)->z.fixed;
  Actor_Get(ACTOR_LIFT_STATUE)->motionFlags = 0;
  Actor_SetPosition(ACTOR_LIFT_STATUE, PIXELS(680), PIXELS(440));
  Actor_Get(ACTOR_LIFT_STATUE)->z.fixed += -PIXELS(137);
  Actor_Get(ACTOR_LIFT_STATUE)->targetZ = Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
  Engine_MapRedraw();
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(60);
  DisplayBlend_SuspendScript();
  Engine_AudioPlayCue(SE_223);
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
    if (liftLayer->offsetY > PIXELS(1024)) {
      if ((gFrameCount & 15) == 0 && liftSpeed > 0xccb)
        liftSpeed += -0x560;
      Engine_TaskWait(1);
      continue;
    }
    break;
  }
  liftLayer->offsetY = PIXELS(1024);
  Engine_MapRedraw();
  Engine_TaskWait(2);
  Actor_Get(ACTOR_PARTY_LEADER)->motionFlags = 3;
  Actor_Get(ACTOR_LIFT_STATUE)->z.fixed = PIXELS(440);
  Actor_Get(ACTOR_LIFT_STATUE)->targetZ = Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
  Engine_EventWait(30);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 704, 584);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 7);
  Actor_Get(ACTOR_PARTY_LEADER)->speed = 0x30000;
  Actor_Get(ACTOR_PARTY_LEADER)->acceleration = 0x20000;
  Engine_AudioPlayCue(SE_JUMP);
  Actor_Get(ACTOR_PARTY_LEADER)->velocityY = 0x40000;
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
  Actor_SetDestination(ACTOR_PARTY_LEADER, 736, 584);
  Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
  Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 1);
  Actor_Get(ACTOR_PARTY_LEADER)->bounce = 0x4000;
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
  Engine_EventWait(6);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
  Engine_AudioPlayCue(SE_223);
  // The lift and its statue go back up without the leader.
  for (;;) {
    liftLayer->offsetY += liftSpeed;
    Actor_Get(ACTOR_LIFT_STATUE)->z.fixed -= liftSpeed;
    Actor_Get(ACTOR_LIFT_STATUE)->targetZ =
        Actor_Get(ACTOR_LIFT_STATUE)->z.fixed;
    if (liftLayer->offsetY >= PIXELS(1161))
      break;
    if ((gFrameCount & 7) == 0 && liftSpeed <= 0xcccc)
      liftSpeed += 0x1999;
    Engine_TaskWait(1);
  }
  liftLayer->offsetY = PIXELS(1024);
  Map_CopyCellsTo(45, 91, 40, 91, 5, 4);
  Map_CopyCellAttributes(104, 34, 5, 4, 40, 34);
  Engine_MapRedraw();
  Engine_TaskWait(2);
  Actor_SetPosition(ACTOR_LIFT_STATUE, 0, 0);
  Engine_EventWait(30);
  Actor_Get(ACTOR_PARTY_LEADER)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
}

/// Mia casts Ply on the Ply target, moved onto the door.
static void MercuryLighthouse_MiaCastsPlyOnEntrance(void) {
  struct BattleFxScene *effectWork;
  u8 flags;

  effectWork = gEffectWork;
  Actor_SetPosition(ACTOR_PLY_TARGET, PIXELS(840), PIXELS(600));
  Engine_PsynergyBegin(ABILITY_PLY, 1);
  Engine_PsynergySetTarget(MIA, ACTOR_PLY_TARGET);
  flags = (u8)effectWork->messageMode | 8;
  effectWork->messageMode = flags;
  Engine_PsynergyRaiseHands();
  Engine_PsynergyPlayEffect(1);
  Engine_PsynergyLowerHands();
}

/// Replaces one inventory item in the owner and slot that contain it.
static void MercuryLighthouse_ReplaceInventoryItem(s32 oldItem, s32 newItem) {
  s32 owner = PartyInventory_FindOwner(oldItem);

  if (owner != -1) {
    s32 inventorySlot = Inventory_Find(owner, oldItem);

    if (inventorySlot != -1) {
      UnitGet(owner)->inventory[inventorySlot] = newItem;
    }
  }
}

#include "common/mercury_lighthouse_lights.h"

/// Creates the rising effect at the source actor and sets its sprite priority.
static void OverlayObject_SpawnKind24AtObject(struct FieldActor *source) {
  struct FieldActor *effect;
  struct AnimationObject *sprite;

  effect = Engine_ObjectCreate(24, source->x.fixed, source->y.fixed,
                               source->z.fixed);
  if (effect == 0) {
    return;
  }

  sprite = effect->sprite;
  Engine_ObjectSetScript(effect, gMercuryLighthouse_RiseScript);

  effect->motionFlags = 0;
  effect->terrainId = 1;
  effect->priorityFlags = 2;

  if (sprite != 0) {
    AnimationObjects_SelectAnimation(sprite, 2);
    sprite->flags = 0;
    sprite->image.priority = 3;
  }
}

#include "common/mercury_lighthouse_pillar.h"

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// Pixel step by direction: X in the upper halfword, Z in the lower. Pack
/// negative steps through unsigned words rather than left-shifting a negative.
#define DIRECTION_STEP(dx, dz) (((u32)(dx) << 16) | ((u32)(dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

// clang-format off
u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8, 32, 8,
    -8, -32, 8, 32,
    -32, -16, 32, 0,
    -8, -32, 8, 32,
    -32, -8, 32, 8,
    -8, -32, 8, 32,
};

s32 gMercuryLighthouse_RiseScript[] = {
    SC_WAIT(30),
    SC_FREE,
};

s32 gMercuryLighthouse_SparkScript[] = {
    SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
    SC_FREE,
};

s32 gMercuryLighthouse_RampScript[] = {
    SC_CALL(OverlayObject_RampWords24And28Over16Frames),
    SC_END,
};

s32 gMercuryLighthouse_ScaleCounterScript[] = {
    SC_CALL(OverlayObject_AdvanceScaleCounter),
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

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gMercuryLighthouse_SparkBurstScript[] = {
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0x4000),
    SC_SET(SW_SCALE_X, 0x4000),
    SC_WAIT(6),
    SC_SET(SW_SCALE_Y, 0xa000),
    SC_SET(SW_SCALE_X, 0xa000),
    SC_WAIT(6),
    SC_LOOP(4, SC_TOP),
    SC_WAIT(1),
    SC_POS(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

s32 gMercuryLighthouse_PushScriptA[] = {
    SC_WAIT(45),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_MOVE(PIXELS(792), 0, PIXELS(712)),
    SC_SYNC,
    SC_END,
};

s32 gMercuryLighthouse_PushScriptB[] = {
    SC_WAIT(60),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_CALL(MercuryLighthouse_StartStatueSpray),
    SC_MOVE(PIXELS(792), 0, PIXELS(744)),
    SC_SYNC,
    SC_CALL(MercuryLighthouse_StopStatueSpray),
    SC_END,
};

s32 gMercuryLighthouse_PushScriptD[] = {
    SC_WAIT(60),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
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
    MAP_CELL_STEP(15, 1, 1, 2, 10), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(9, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gMercuryLighthouse_GateCloseCells[] = {
    MAP_CELL_STEP(9, 1, 1, 2, 10), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(15, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gMercuryLighthouse_FloorSwitchCells[] = {
    MAP_CELL_STEP(16, 28, 1, 2, 10), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(10, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gMercuryLighthouse_FloorSwitchCloseCells[] = {
    MAP_CELL_STEP(10, 28, 1, 2, 10), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(16, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 gMercuryLighthouse_ColumnCells[] = {
    MAP_CELL_STEP(126, 35, 1, 2, 2), MAP_CELL_STEP(126, 38, 1, 2, 2), MAP_CELL_STEP(126, 41, 1, 2, 2), MAP_CELL_STEP(126, 44, 1, 2, 2), MAP_CELL_STEP(126, 47, 1, 2, 2), MAP_CELL_STEPS_END,
};

struct SceneEntrance gMercuryLighthouseEntrancesOther[] = {
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
    { MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_STAIR_GATE, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_LOWER, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_FIVE_PILLARS, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS_STATUE_FALLS, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
    { MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE_STATUE_FALLS, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
    { SCENE_TABLE_END },
};

struct SceneEntrance gMercuryLighthouseExteriorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_MIDDLE, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_WALKWAY_FROM_WATERFALL, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
    { MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_WALKWAY, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
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
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_TO_STAIR_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_FIVE_PILLARS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER, MercuryLighthouseLower, MERCURY_LIGHTHOUSE_LOWER_FROM_MERCURY_LIGHTHOUSE_UPPER),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_STONE_DOORS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_FIVE_PILLARS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_FROM_STAIR_GATE),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_FROM_GAP),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_FROM_MERCURY_LIGHTHOUSE_UPPER_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_WATERFALLS_TO_GAP, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_GAP_FROM_WATERFALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_STAIR_GATE, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_STAIR_GATE_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_FROM_STONE_DOORS_STATUE_FALLS),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_STONE_DOORS_STATUE_FALLS, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_FROM_STAIR_GATE_STATUE_FALLS),
    SCENE_EXITS(MercuryLighthouseExterior),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE, MercuryLighthouseMiddle, MERCURY_LIGHTHOUSE_MIDDLE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_WEST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST, MercuryLighthouseUpper, MERCURY_LIGHTHOUSE_UPPER_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_WALKWAY_TO_WATERFALL, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_FROM_UPPER_WALKWAY),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_WATERFALL_TO_UPPER_WALKWAY, MercuryLighthouseExterior, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_WALKWAY_FROM_WATERFALL),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_MERCURY_LIGHTHOUSE_EXTERIOR_EAST),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE, MercuryLighthouseEntrance, MERCURY_LIGHTHOUSE_ENTRANCE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_LIGHTHOUSE_LIT, WorldMap, WORLD_MAP_MERCURY_LIGHTHOUSE_LIT),
    SCENE_EXIT(MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_AERIE, MercuryLighthouseAerie, MERCURY_LIGHTHOUSE_AERIE_FROM_MERCURY_LIGHTHOUSE_EXTERIOR),
    SCENE_EXITS_END,
};

struct ScenePlacement gMercuryLighthousePlacementsOther[] = {
    { SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthouseLowerPlacements[] = {
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe4, CONDITION_ALWAYS, 7,
        PIXELS(672), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe4, CONDITION_ALWAYS, 7,
        PIXELS(704), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe5, CONDITION_ALWAYS, 7,
        PIXELS(744), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe6, CONDITION_ALWAYS, 7,
        PIXELS(648), 0, PIXELS(299), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthouseMiddlePlacements[] = {
    { 0xe5, CONDITION_ALWAYS, 7,
        PIXELS(152), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe6, CONDITION_ALWAYS, 7,
        PIXELS(184), 0, PIXELS(587), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe4, CONDITION_ALWAYS, 7,
        PIXELS(704), 0, PIXELS(672), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { MERCURY_DJINNI, FLAG_DJINNI_0 + DJINNI_SLEET, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthouseUpperPlacements[] = {
    { SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL), ACTOR_STAND,
        PIXELS(200), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, FLAG_MERCURY_LIGHTHOUSE_STATUE_FELL, ACTOR_STAND,
        PIXELS(808), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe4, CONDITION_ALWAYS, 7,
        PIXELS(816), 0, PIXELS(544), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe5, CONDITION_ALWAYS, 7,
        PIXELS(872), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe6, CONDITION_ALWAYS, 7,
        PIXELS(760), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe6, CONDITION_ALWAYS, 7,
        PIXELS(776), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xe6, CONDITION_ALWAYS, 7,
        PIXELS(872), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(712), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(776), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(936), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gMercuryLighthouseExteriorPlacements[] = {
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(664), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(792), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(888), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SPRITE_STONE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(840), 0, PIXELS(600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, 7,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(856), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_BLUE_STATUE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), ACTOR_STAND,
        PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), ACTOR_STAND,
        PIXELS(824), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), ACTOR_WANDER,
        PIXELS(760), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), ACTOR_WANDER,
        PIXELS(920), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), ACTOR_STAND,
        PIXELS(856), 0, PIXELS(792), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gMercuryLighthouseLowerEvents[] = {
    { EVENT_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST_NORTHWEST },
    { EVENT_EXIT, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_POOL_TO_STATUE },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_POOL },
    { EVENT_EXIT, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_STATUE_TO_WATERFALLS },
    { EVENT_EXIT, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_EAST_NORTHEAST },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_WATERFALLS_TO_STATUE },
    { EVENT_EXIT, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_SOUTH },
    { EVENT_EXIT, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_ENTRANCE_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_LOWER_TO_MERCURY_LIGHTHOUSE_UPPER },
    { EVENT_TOUCH, 32, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_ReleaseRaisedPillar) },
    { TOUCH_WALKING, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_TryPillarStep) },
    { EVENT_TOUCH, 31, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_ReleaseRaisedPillar) },
    { 0xa02, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_TryPillarStep) },
    { EVENT_TOUCH, 21, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_SpawnTimedRiseEffect) },
    { TOUCH_WALKING, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_TryPillarStep) },
    { EVENT_TOUCH, 11, IF_FLAG_SET(FLAG_MERCURY_LIGHTHOUSE_HEART_HONORED), EVENT_SCRIPT(MercuryLighthouse_RaiseTouchedPillar) },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_EnterRisingPassage) },
    { TOUCH_WALKING, 25, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_MoveChannelPillars) },
    { EVENT_TALK, MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThePathIsBlockedAgainWhat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MIA, FLAG_MIA_JOINED_PARTY, MSGNO(MsgMercuryLighthouseThisStatueWasntHereBeforeI) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STATUE_BLOCKING_MIA, FLAG_MIA_JOINED_PARTY, EVENT_SCRIPT(MercuryLighthouse_MiaJoinsAfterRescue) },
    { SEARCH(SEARCH_CHEST), 100, FLAG_CHEST_SLEEP_BOMB, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseMiddleEvents[] = {
    { EVENT_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_GATE_TO_PIPES },
    { EVENT_EXIT, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_NORTH },
    { EVENT_EXIT, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_LOWER_SOUTH },
    { EVENT_EXIT, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_PIPES_TO_GATE },
    { EVENT_EXIT, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_WEST_NORTHWEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_EXTERIOR },
    { EVENT_EXIT, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHWEST },
    { EVENT_EXIT, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTHEAST },
    { EVENT_EXIT, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_EAST_NORTHEAST },
    { EVENT_EXIT, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_MIDDLE_TO_MERCURY_LIGHTHOUSE_UPPER_NORTH_NORTHEAST },
    { TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_EnterRisingPassage) },
    { TOUCH_WALKING, 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_MoveStairAndFountainPillars) },
    { EVENT_TOUCH, 2, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { EVENT_TOUCH, 3, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { EVENT_TOUCH, 4, IF_FLAG_SET(FLAG_STAIRS_RAISED), EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { TOUCH_WALKING, 10, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushGateSwitchPillar) },
    { EVENT_TOUCH, 12, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PressGateFloorSwitch) },
    { EVENT_TOUCH, 10, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
    { EVENT_TOUCH, 13, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_CloseFloorSwitch) },
    { TOUCH_WALKING, 45, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_MoveStairAndFountainPillars) },
    { EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_WalkLeaderIn) },
    { EVENT_TALK, ACTOR_SLEET, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_StartDjinnBattle) },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATE_STATUE, FLAG_MERCURY_LIGHTHOUSE_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_OpenGateWithMovedPillar) },
    { SEARCH(SEARCH_CHEST), 101, FLAG_CHEST_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL) },
    { SEARCH(SEARCH_CHEST), 102, FLAG_CHEST_NUT, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseUpperEvents[] = {
    { EVENT_EXIT, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_WEST_NORTHWEST },
    { EVENT_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTH_NORTHEAST },
    { TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_ExitWithSparks) },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 57, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_LOWER },
    { EVENT_EXIT, 59, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_FIVE_PILLARS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 58, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_STAIR_GATE_TO_STONE_DOORS },
    { EVENT_EXIT, 61, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_GAP_TO_WATERFALLS },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED, 60, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_EAST },
    { EVENT_EXIT, 62, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_EXTERIOR_WEST },
    { TOUCH_WALKING_INTO(FACING_NORTH), 63, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_ExitAfterSparkStep) },
    { EVENT_EXIT, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_FIVE_PILLARS_TO_STAIR_GATE },
    { EVENT_EXIT, 55, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_NORTHEAST },
    { EVENT_EXIT, 56, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_TO_MERCURY_LIGHTHOUSE_MIDDLE_EAST_NORTHEAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 64, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_UPPER_STONE_DOORS_TO_STAIR_GATE },
    { TOUCH_WALKING, 10, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushStairSwitchPillar) },
    { TOUCH_WALKING, 12, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PushStairSwitchPillar) },
    { EVENT_TOUCH, 12, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_PressStairFloorSwitch) },
    { EVENT_TOUCH, 10, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_ReleaseStairFloorSwitch) },
    { TOUCH_WALKING, 40, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_MoveStoneDoorPillars) },
    { TOUCH_WALKING, 47, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PushAndSaveCrossingStatuePositions) },
    { SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectLockedDoor) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FALLEN_STATUE, FLAG_MERCURY_LIGHTHOUSE_UPPER_GATE_HELD_OPEN, EVENT_SCRIPT(MercuryLighthouse_OpenStairGateWithMovedPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CROSSING_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CROSSING_STATUE + 1, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CROSSING_STATUE + 2, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CROSSING_STATUE + 3, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LAST_CROSSING_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_SyncCrossingStatueFlags) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GAP_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_OpenMovedStatueCrossing) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gMercuryLighthouseEventsOther[] = {
    { EVENT_EXIT, 44, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_ENTRANCE },
    { EVENT_EXIT, 43, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_WORLD_MAP_EAST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 51, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_MIDDLE },
    { EVENT_EXIT, 52, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_WEST },
    { EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 53, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_TO_MERCURY_LIGHTHOUSE_UPPER_EAST },
    { EVENT_EXIT, 54, CONDITION_ALWAYS, MERCURY_LIGHTHOUSE_EXTERIOR_UPPER_WALKWAY_TO_WATERFALL },
    { TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_LeaveWithSparkTrail) },
    { EVENT_TALK, MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT_SCRIPT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x400 | MIA, FLAG_MIA_SENSED_SOMEONE_INSIDE, EVENT_SCRIPT(MercuryLighthouse_MiaSensesSomeoneInside) },
    { EVENT_TALK, MIA, FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE, EVENT_SCRIPT(MercuryLighthouse_MiaDiscussesBlockedEntrance) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), MIA, FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE, MSGNO(MsgMercuryLighthouseSomeoneDoesntWantMeToGet) },
    { EVENT_TALK, ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseTheLegendsSaidThatIfThe) },
    { EVENT_TALK, ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseOurFountainIsBackTheySay) },
    { EVENT_TALK, ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIDrankTheHealingWaterAnd) },
    { EVENT_TALK, ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseWeDontHaveToFearAny) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseHowDidTheFountainKnowThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 1, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseAllThatLivesThatMeansIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 2, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseIKnewItWouldWorkBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_VISITOR + 3, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), MSGNO(MsgMercuryLighthouseINeverBelievedThoseStoriesBut) },
    { SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_InspectBlockedPassage) },
    { SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_ReadHermesFountainInscription) },
    { SEARCH(SEARCH_UNNAMED), 5, FLAG_HERMES_WATER_TAKEN, EVENT_SCRIPT(MercuryLighthouse_InspectHealingFountain) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_IMIL_MET_MIA, 0 },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOOR_STATUE, FLAG_MIA_OPENED_MERCURY_LIGHTHOUSE, EVENT_SCRIPT(MercuryLighthouse_MiaOpensEntranceWithPly) },
    { PSYNERGY_ON_ACTOR(ABILITY_PLY), ACTOR_WATERFALL_STATUE, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_PlyOpensWaterfallPassage) },
    { PSYNERGY_IN_AREA(ABILITY_RETREAT, PSYNERGY_PHASE_BEFORE_CAST), 0, CONDITION_ALWAYS, EVENT_SCRIPT(MercuryLighthouse_RemoveColumnOnRetreat) },
    { USE_ITEM(ITEM_EMPTY_BOTTLE), 5, IF_FLAG_SET(FLAG_SATUROS_BEATEN_AT_MERCURY), EVENT_SCRIPT(MercuryLighthouse_FillHermesWaterBottle) },
    { SCENE_EVENTS_END },
};
// clang-format on
