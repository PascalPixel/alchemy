/// @file
/// The first and last rooms of Goma Cave (SceneId_GomaCaveStream and
/// SceneId_GomaCaveBridge), the cave in the Goma Range that the
/// mayor of Vault tells the party to pass through on the way to Bilibin.
///
/// - Goma Cave, SceneId_GomaCaveStream, where a stream runs the length of the
/// room. The party
///   walks in from the Goma Cave Entrance
///   (GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE) at the bottom of the room. A
///   stump answers Move: pushed to column 24, it spins in place, leaps away,
///   lands with a shake and a ring of dust and lies flat underfoot, and the
///   cells where it lands change (FLAG_GOMA_CAVE_STREAM_STUMP_MOVED, a story
///   flag, so it stays there for good). A chest holds Power Bread. Exits lead
///   back to the Goma Cave Entrance, into the room of two pillars
///   (goma_cave_pillar.c; GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST and
///   GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST) and north into
///   SceneId_GomaCaveBridge (GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE).
///   GOMA_CAVE_STREAM_TO_VALE_GATE, on collision code 1, leads to Vale
///   (SceneId_ValeGate, vale_gate.c), and Vale's VALE_GATE_TO_GOMA_CAVE_STREAM
///   leads back in at GOMA_CAVE_STREAM_FROM_VALE_GATE, which arrives
///   without the place name. No exit leads to entrance 99.
/// - Goma Cave, SceneId_GomaCaveBridge, the way out, where a wooden bridge
/// crosses the river.
///   It has no actors and only its two exits: back south into
///   SceneId_GomaCaveStream (GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM) and out onto
///   the world map (GOMA_CAVE_BRIDGE_TO_WORLD_MAP), toward Bilibin; the world
///   map leads back in at GOMA_CAVE_BRIDGE_FROM_WORLD_MAP.
///
/// The getters give SceneId_GomaCaveBridge its own tables; any other scene
/// takes SceneId_GomaCaveStream's.
#include "game_state.h"
#include "unit/scene/goma_cave_waterway.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "field_obj.h"
#include "types.h"
#include "field_event.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "fixed_math.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "event.h"
#include "item_ids.h"
#include "ability_ids.h"
#include "vector3.h"

/// SceneId_GomaCaveStream's actors, numbering its placement rows from
/// ACTOR_FIRST_PLACED.
enum GomaCaveWaterwayActor {
  /// The stump Move pushes, as Garet calls the one at the Goma
  /// Cave Entrance: "Our Psynergy isn't strong enough to free that stump."
  ACTOR_STUMP = ACTOR_FIRST_PLACED
};

// Forward declarations.
static void GomaCaveWaterway_LeapStump(s32 no);

#include "common/effect_create.h"
#include "common/effect_spawn.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveWaterwayEntrances2;
  }
  return gGomaCaveWaterwayEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gGomaCaveWaterwayExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveWaterwayPlacements2;
  }
  return gGomaCaveWaterwayPlacementsOther;
}

/// Move on the stump in Goma Cave (SceneId_GomaCaveStream), until it has leapt
/// (FLAG_GOMA_CAVE_STREAM_STUMP_MOVED): from column 24 it spins, leaps away
/// and lies where it lands.
static void Event_MoveStump(void) {
  struct FieldActor *stump;
  struct FieldActor *landed;
  s32 column;

  stump = Object_GetById(ACTOR_STUMP);
  column = stump->x.fixed / PIXELS(16);
  if (column == 24) {
    GomaCaveWaterway_LeapStump(ACTOR_STUMP);
    Object_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
    landed = Object_GetById(ACTOR_STUMP);
    Engine_ActorSetSpriteFlags(landed, 0);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED);
  }
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveWaterwayEvents2;
  }
  return gGomaCaveWaterwayEventsOther;
}

/// Sets up SceneId_GomaCaveStream as the party enters (SceneId_GomaCaveBridge
/// needs nothing).
/// Arriving from Vale (GOMA_CAVE_STREAM_FROM_VALE_GATE) shows no place
/// name; otherwise the stump lies where the party moved it.
s32 Scene_Initialize(void) {
  struct FieldActor *stump;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.scene == (s32)&SceneId_GomaCaveStream) {
    if (gGameState.entrance == GOMA_CAVE_STREAM_FROM_VALE_GATE) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    } else {
      Object_GetById(ACTOR_STUMP)->collisionFlags |= 16;
      if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED) != 0) {
        Engine_ActorSetPosition(ACTOR_STUMP, PIXELS(346), PIXELS(292));
        stump = Object_GetById(ACTOR_STUMP);
        Engine_ActorSetSpriteFlags(stump, 0);
        Object_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        OBJ_MODE(ACTOR_STUMP, 2);
        Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
      }
    }
  }
  return 0;
}

/// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveWaterway_SpinStump(union FieldObject *object) {
  struct AnimationObject *sprite = object->actor.sprite;

  sprite->rotation -= 0x800;
}

#include "common/object_waitdown.h"

/// The stump spins in place, leaps and lands with a shake and a ring of dust,
/// then comes to rest a little further on.
static void GomaCaveWaterway_LeapStump(s32 no) {
  struct FieldActor *work;
  u32 cnt;
  struct Vector3 vec;
  struct EffectOptions opt;

  work = Object_GetById(no);
  work->motionFlags = 0;
  for (cnt = 0; cnt < 18; cnt++) {
    Engine_TaskWait(1);
    work->sprite->rotation -= 256;
    work->x.fixed -= Trig_Cos(work->sprite->rotation) / 2;
    work->targetX = ACTOR_NO_TARGET;
  }
  work->update = GomaCaveWaterway_SpinStump;
  OBJ_SPEED(no, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(no, 376, 288);
  work->gravity = 0xcccc;
  work->motionFlags = 3;
  work->terrainId = 0;
  Engine_ActorWaitForMove(no);
  SceneActor_WaitObjectBelowHeight(work, 0);
  Engine_AudioPlayCue(SE_188);
  Map_SetShake(0x50000, 0x50000, 0x10000);
  Engine_AudioPlayCue(SE_141);
  Map_SetShake(-1, -1, 0xe666);
  for (cnt = 0; cnt < 17; cnt++) {
    vec.x = Trig_Cos(cnt << 12);
    vec.y = 0;
    vec.z = Trig_Sin(cnt << 12);
    vec.x -= vec.x / 4;
    vec.z -= vec.z / 2;
    Effect_Spawn(work->x.fixed, work->y.fixed, work->z.fixed, vec.x, vec.y,
                 vec.z, 0, NULL);
  }
  work->velocityY = 0x50000;
  Actor_SetDestination(no, 346, 292);
  Engine_ActorWaitForMove(no);
  SceneActor_WaitObjectBelowHeight(work, 0);
  // UB: the native integer store aliases the update pointer. A typed clear
  // swaps this store with the following sprite-pointer load.
#ifdef UBFIX
  work->update = NULL;
#else
  *(u32 *)&work->update = 0;
#endif
  work->sprite->rotation = 0x1000;
  opt.type = 214;
  opt.startScaleX = 0x8000;
  opt.startScaleY = 0xcccc;
  opt.targetScaleX = 0x18000;
  opt.targetScaleY = 0x13333;
  Effect_Spawn(work->x.fixed, work->y.fixed, work->z.fixed, 0, 0, 0, 0x1c0000,
               &opt);
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(no, 3);
  Map_WaitForShakeToSettle();
}

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

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gGomaCaveWaterwayEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE, CONDITION_ALWAYS, 376, 0, 776,
     FACING_NORTH, 0, 88, 8, 664, 832, 0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST, CONDITION_ALWAYS, 168, 0, 552,
     FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST, CONDITION_ALWAYS, 600, 0, 520,
     FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE, CONDITION_ALWAYS, 376, 0, 168,
     FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {GOMA_CAVE_STREAM_FROM_VALE_GATE, CONDITION_ALWAYS, 614, 0, 848,
     FACING_NORTH, 0, 480, 605, 720, 900, 0},
    {99, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88, 8, 664, 832, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gGomaCaveWaterwayEntrances2[] = {
    {GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 154, 0, 350,
     FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {GOMA_CAVE_BRIDGE_FROM_WORLD_MAP, CONDITION_ALWAYS, 552, 0, 312,
     FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCaveWaterwayExits[] = {
    SCENE_EXITS(GomaCaveStream),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE, GomaCaveEntrance,
               GOMA_CAVE_ENTRANCE_FROM_GOMA_CAVE_STREAM),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST, GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST, GomaCavePillar,
               GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE, GomaCaveBridge,
               GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM),
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_VALE_GATE, ValeGate,
               VALE_GATE_FROM_GOMA_CAVE_STREAM),
    SCENE_EXITS(GomaCaveBridge),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM, GomaCaveStream,
               GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaCaveWaterwayPlacementsOther[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct ScenePlacement gGomaCaveWaterwayPlacements2[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gGomaCaveWaterwayEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_VALE_GATE},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 6, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED_ALTERNATE, 7, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE},
    {SEARCH(SEARCH_CHEST), 100, 0xf28, FIND_ITEM(ITEM_POWER_BREAD)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP,
     FLAG_GOMA_CAVE_STREAM_STUMP_MOVED, EVENT_SCRIPT(Event_MoveStump)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gGomaCaveWaterwayEvents2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_WORLD_MAP},
    {SCENE_EVENTS_END},
};
