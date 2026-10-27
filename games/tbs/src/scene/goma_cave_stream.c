/// @file
/// The first and last rooms of Goma Cave, the cave in the Goma Range between
/// Vault and Bilibin: SceneId_GomaCaveStream and SceneId_GomaCaveBridge, both
/// under the area name Goma Cave. The getters give SceneId_GomaCaveBridge its
/// own tables and any other map SceneId_GomaCaveStream's; the two share one
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
///   SceneId_GomaCaveBridge. Vale's VALE_PLAZA_TO_GOMA_CAVE_STREAM leads in at
///   GOMA_CAVE_STREAM_FROM_VALE_PLAZA, which arrives without the area name in
///   a passage walled off from the rest of the room, its own camera box with
///   a chest at its north end (its render,
///   places/goma-cave-stream/map-5.png). Its only way out is the metatile
///   behavior 1 cell at its south tip, cell (38, 55) in map_023's
///   map.txt, two cells south of the arrival, which leads back to
///   Vale (GOMA_CAVE_STREAM_TO_VALE_PLAZA).
/// - SceneId_GomaCaveBridge, the way out, where a wooden bridge crosses the
///   river. It has no actors and only its two exits: back south into
///   SceneId_GomaCaveStream and out onto the world map toward Bilibin
///   (GOMA_CAVE_BRIDGE_TO_WORLD_MAP).
#include "save_block.h"
#include "unit/scene/goma_cave_stream.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "actor.h"
#include "types.h"
#include "script_layer.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "call.h"
#include "fixed_math.h"
#include "objcmd.h"
#include "flags.h"
#include "songs.h"
#include "item_ids.h"
#include "ability_ids.h"
#include "vector3.h"

/// SceneId_GomaCaveStream's actors, numbering its actor template
/// rows from ACTOR_FIRST_PLACED.
enum GomaCaveStreamActorId {
  /// The stump Move pushes, as Garet calls the one at the Goma Cave Entrance.
  /// ★ Named from the game: "It's no use. Our Psynergy isn't strong enough to
  /// free that stump." (MsgGomaNoUsePsynergy)
  ACTOR_STUMP = ACTOR_FIRST_PLACED
};

/// SceneId_GomaCaveStream's entrance that no exit names, standing the
/// leader just east of the stump, facing it. Only the debug map selector, which
/// exits to any entrance by number, reaches it.
enum { ENTRANCE_BESIDE_STUMP = 99 };

static void GomaCaveStream_LeapStump(s32 stumpId);

#include "common/effect_create.h"
#include "common/effect_spawn.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveBridgeEntrances;
  }
  return gGomaCaveStreamEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gGomaCaveStreamAndBridgeExits; }

const struct ActorTemplate *Scene_GetActorTemplates(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveBridgeActorTemplates;
  }
  return gGomaCaveStreamActorTemplates;
}

/// Move on the stump in Goma Cave (SceneId_GomaCaveStream), until it has leapt
/// (FLAG_GOMA_CAVE_STREAM_STUMP_MOVED): from column 24 it spins, leaps away
/// and lies where it lands.
static void EventScript_MoveStump(void) {
  struct Actor *stump;
  struct Actor *landed;
  s32 column;

  stump = Actor_GetById(ACTOR_STUMP);
  column = stump->x.fixed / PIXELS(16);
  if (column == 24) {
    GomaCaveStream_LeapStump(ACTOR_STUMP);
    Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(19, 74, 9, 3, 19, 17);
    landed = Actor_GetById(ACTOR_STUMP);
    Actor_SetSpriteFlags(landed, 0);
    GameFlag_Set(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED);
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_GomaCaveBridge) {
    return gGomaCaveBridgeTriggers;
  }
  return gGomaCaveStreamTriggers;
}

/// Sets up SceneId_GomaCaveStream as the party enters (SceneId_GomaCaveBridge
/// needs nothing).
/// Arriving from Vale (GOMA_CAVE_STREAM_FROM_VALE_PLAZA) shows no area
/// name; otherwise the stump lies where the party moved it.
s32 Scene_Initialize(void) {
  struct Actor *stump;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_GomaCaveStream) {
    if (gSaveBlock.entrance == GOMA_CAVE_STREAM_FROM_VALE_PLAZA) {
      GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    } else {
      Actor_GetById(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
      if (GameFlag_Get(FLAG_GOMA_CAVE_STREAM_STUMP_MOVED) != 0) {
        Import_ActorSetPosition(ACTOR_STUMP, PIXELS(346), PIXELS(292));
        stump = Actor_GetById(ACTOR_STUMP);
        Actor_SetSpriteFlags(stump, 0);
        Actor_GetById(ACTOR_STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        Import_ActorSetAnimation(ACTOR_STUMP, STUMP_ANIMATION_SINK);
        MapGrid_CopyCellAttributes(19, 74, 9, 3, 19, 17);
      }
    }
  }
  return 0;
}

/// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveStream_SpinStump(union ActorOrEffect *fieldObject) {
  struct Sprite *sprite = fieldObject->actor.sprite;

  sprite->rotation -= 0x800;
}

#include "common/object_waitdown.h"

/// The stump spins in place, leaps and lands with a shake and a ring of dust,
/// then hops on into the water with a splash and sinks
/// (STUMP_ANIMATION_SPLASH).
static void GomaCaveStream_LeapStump(s32 stumpId) {
  struct Actor *stump;
  u32 i;
  struct Vector3 dir;
  struct EffectOptions options;

  stump = Actor_GetById(stumpId);
  stump->motionFlags = 0;
  for (i = 0; i < 18; i++) {
    WaitFrames(1);
    stump->sprite->rotation -= 256;
    stump->x.fixed -= Trig_Cos(stump->sprite->rotation) / 2;
    stump->targetX = ACTOR_NO_TARGET;
  }
  stump->update = GomaCaveStream_SpinStump;
  Actor_SetSpeed(stumpId, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(stumpId, 376, 288);
  stump->gravity = 4 * PIXELS(1) / 5;
  stump->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  stump->layer = 0;
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, 0);
  Sound_PlaySong(SE_THUMP);
  MapGrid_SetShake(PIXELS(5), PIXELS(5), Q16_ONE);
  Sound_PlaySong(SE_RUMBLE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
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
  Actor_WaitForMoveById(stumpId);
  Actor_WaitBelowHeight(stump, 0);
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
  Sound_PlaySong(SE_CAST);
  Import_ActorSetAnimation(stumpId, STUMP_ANIMATION_SPLASH);
  MapGrid_WaitForShakeToSettle();
}

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
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
    {GOMA_CAVE_STREAM_FROM_VALE_PLAZA, CONDITION_ALWAYS, 614, 0, 848,
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
    SCENE_EXIT(GOMA_CAVE_STREAM_TO_VALE_PLAZA, ValePlaza,
               VALE_PLAZA_FROM_GOMA_CAVE_STREAM),
    SCENE_EXITS(GomaCaveBridge),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM, GomaCaveStream,
               GOMA_CAVE_STREAM_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXIT(GOMA_CAVE_BRIDGE_TO_WORLD_MAP, WorldMap,
               WORLD_MAP_FROM_GOMA_CAVE_BRIDGE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorTemplate gGomaCaveStreamActorTemplates[] = {
    { SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct ActorTemplate gGomaCaveBridgeActorTemplates[] = {
    {SCENE_TABLE_END},
};

struct Trigger gGomaCaveStreamTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_VALE_PLAZA},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_ENTRANCE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_WEST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS,
     GOMA_CAVE_STREAM_TO_GOMA_CAVE_PILLAR_EAST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, GOMA_CAVE_STREAM_TO_GOMA_CAVE_BRIDGE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, 0xf28, FIND_ITEM(ITEM_POWER_BREAD)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP,
     FLAG_GOMA_CAVE_STREAM_STUMP_MOVED, EVENT_SCRIPT(EventScript_MoveStump)},
    {TRIGGERS_END},
};

struct Trigger gGomaCaveBridgeTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_GOMA_CAVE_STREAM},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, GOMA_CAVE_BRIDGE_TO_WORLD_MAP},
    {TRIGGERS_END},
};
