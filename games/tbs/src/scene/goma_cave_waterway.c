/// @file
/// The first and last rooms of Goma Cave, the cave in the Goma Range between
/// Vault and Bilibin: SceneId_GomaCaveStream and SceneId_GomaCaveBridge, both
/// under the area name Goma Cave. The getters give SceneId_GomaCaveBridge its
/// own tables and any other scene SceneId_GomaCaveStream's; the two share one
/// exit table.
///
/// - SceneId_GomaCaveStream, the room a stream runs the length of. The party
///   walks in from the Goma Cave Entrance
///   (GOMA_CAVE_STREAM_FROM_GOMA_CAVE_ENTRANCE) at its bottom. A stump answers
///   Move: once it stands on column 24 it spins in place, leaps, lands with a
///   shake and a ring of dust, then hops on into the water with a splash and
///   sinks until its top shows, and the cells where it lies change
///   (FLAG_GOMA_CAVE_STREAM_STUMP_MOVED, a story flag, so it stays there for
///   good). A chest holds Power Bread. Exits lead back to the Goma Cave
///   Entrance, into the room of two pillars (goma_cave_pillar.c;
///   GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST and _EAST) and north into
///   SceneId_GomaCaveBridge. Vale's VALE_GATE_TO_GOMA_CAVE_STREAM leads in at
///   GOMA_CAVE_STREAM_FROM_VALE_GATE, which arrives without the place name in
///   a passage walled off from the rest of the room, its own camera box with
///   a chest at its north end (its render,
///   places/goma-cave-stream/map-5.png). Its only way out is the collision
///   code 1 cell at its south tip, cell (38, 55) in map_023's
///   grid_collision.tsv, two cells south of the arrival, which leads back to
///   Vale (GOMA_CAVE_STREAM_TO_VALE_GATE).
/// - SceneId_GomaCaveBridge, the way out, where a wooden bridge crosses the
///   river. It has no actors and only its two exits: back south into
///   SceneId_GomaCaveStream and out onto the world map toward Bilibin
///   (GOMA_CAVE_BRIDGE_TO_WORLD_MAP).
///
/// Open questions: none.
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
  /// The stump Move pushes, as Garet calls the one at the Goma Cave Entrance.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  ACTOR_STUMP = ACTOR_FIRST_PLACED
};

/// SceneId_GomaCaveStream's entrance that no exit names, standing the leader
/// just east of the stump, facing it. Only the debug scene selector, which
/// warps to any entrance by number, reaches it.
enum { ENTRANCE_BESIDE_STUMP = 99 };

static void GomaCaveWaterway_LeapStump(s32 stumpId);

#include "common/effect_create.h"
#include "common/effect_spawn.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveBridgeEntrances;
  }
  return gGomaCaveStreamEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gGomaCaveStreamAndBridgeExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveBridgePlacements;
  }
  return gGomaCaveStreamPlacements;
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
    return gGomaCaveBridgeEvents;
  }
  return gGomaCaveStreamEvents;
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
      Object_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED) != 0) {
        Engine_ActorSetPosition(ACTOR_STUMP, PIXELS(346), PIXELS(292));
        stump = Object_GetById(ACTOR_STUMP);
        Engine_ActorSetSpriteFlags(stump, 0);
        Object_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        OBJ_MODE(ACTOR_STUMP, STUMP_ANIM_SINK);
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
/// then hops on into the water with a splash and sinks (STUMP_ANIM_SPLASH).
static void GomaCaveWaterway_LeapStump(s32 stumpId) {
  struct FieldActor *stump;
  u32 i;
  struct Vector3 dir;
  struct EffectOptions options;

  stump = Object_GetById(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i < 18; i++) {
    Engine_TaskWait(1);
    stump->sprite->rotation -= 256;
    stump->x.fixed -= Trig_Cos(stump->sprite->rotation) / 2;
    stump->targetX = ACTOR_NO_TARGET;
  }
  stump->update = GomaCaveWaterway_SpinStump;
  OBJ_SPEED(stumpId, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(stumpId, 376, 288);
  stump->gravity = 4 * PIXELS(1) / 5;
  stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  stump->terrainId = 0;
  Engine_ActorWaitForMove(stumpId);
  SceneActor_WaitObjectBelowHeight(stump, 0);
  Engine_AudioPlayCue(SE_188);
  Map_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Engine_AudioPlayCue(SE_141);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
  for (i = 0; i < 17; i++) {
    dir.x = Trig_Cos(i << 12);
    dir.y = 0;
    dir.z = Trig_Sin(i << 12);
    dir.x -= dir.x / 4;
    dir.z -= dir.z / 2;
    FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, dir.x,
                      dir.y, dir.z, 0, NULL);
  }
  stump->velocityY = PIXELS(5);
  Actor_SetDestination(stumpId, 346, 292);
  Engine_ActorWaitForMove(stumpId);
  SceneActor_WaitObjectBelowHeight(stump, 0);
  // UB: the native integer store aliases the update pointer. A typed clear
  // swaps this store with the following sprite-pointer load.
#ifdef UBFIX
  stump->update = NULL;
#else
  *(u32 *)&stump->update = 0;
#endif
  stump->sprite->rotation = 0x1000;
  options.type = SPRITE_SPLASH;
  options.startScaleX = Q16_ONE / 2;
  options.startScaleY = 4 * Q16_ONE / 5;
  options.targetScaleX = 3 * Q16_ONE / 2;
  options.targetScaleY = 6 * Q16_ONE / 5;
  FieldEffect_Spawn(stump->x.fixed, stump->y.fixed, stump->z.fixed, 0, 0, 0,
                    EFFECT_USE_TYPE | EFFECT_USE_START_SCALE |
                        EFFECT_SCALE_TO_TARGET,
                    &options);
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(stumpId, STUMP_ANIM_SPLASH);
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

struct SceneEntrance gGomaCaveStreamEntrances[] = {
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
    {ENTRANCE_BESIDE_STUMP, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88,
     8, 664, 832, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gGomaCaveBridgeEntrances[] = {
    {GOMA_CAVE_BRIDGE_FROM_GOMA_CAVE_STREAM, CONDITION_ALWAYS, 154, 0, 350,
     FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {GOMA_CAVE_BRIDGE_FROM_WORLD_MAP, CONDITION_ALWAYS, 552, 0, 312,
     FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCaveStreamAndBridgeExits[] = {
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
struct ScenePlacement gGomaCaveStreamPlacements[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct ScenePlacement gGomaCaveBridgePlacements[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gGomaCaveStreamEvents[] = {
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

struct SceneEvent gGomaCaveBridgeEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_WORLD_MAP},
    {SCENE_EVENTS_END},
};
