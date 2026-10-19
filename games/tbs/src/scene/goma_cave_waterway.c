// Goma Cave: the waterway rooms, where a stump leaps away under Move.
#include "game_state.h"
#include "unit/scene/goma_cave_waterway.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "field_obj.h"
#include "types.h"
#include "configured_effect_spawn.h"
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

#define STUMP 8

// Forward declarations.
static void GomaCaveWaterway_LeapStump(s32 no);

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"

const struct SceneEntrance *Scene_GetEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveWaterway2) {
    return gGomaCaveWaterwayEntrances2;
  }
  return gGomaCaveWaterwayEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gGomaCaveWaterwayExits; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveWaterway2) {
    return gGomaCaveWaterwayPlacements2;
  }
  return gGomaCaveWaterwayPlacementsOther;
}

// Move on the stump: from column 24 it spins, leaps away and lies where it
// lands.
static void Event_MoveStump(void) {
  struct FieldActor *stump;
  struct FieldActor *landed;
  s32 column;

  stump = Object_GetById(STUMP);
  column = stump->x.fixed / PIXELS(16);
  if (column == 24) {
    GomaCaveWaterway_LeapStump(STUMP);
    Object_GetById(STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
    landed = Object_GetById(STUMP);
    Engine_ActorSetSpriteFlags((s32)landed, 0);
    Engine_GameFlagSet(FLAG_GOMA_CAVE_WATERWAY_STUMP_MOVED);
  }
}

const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_GomaCaveWaterway2) {
    return gGomaCaveWaterwayEvents2;
  }
  return gGomaCaveWaterwayEventsOther;
}

// Arriving by entrance 5 shows no place name; otherwise the stump lies
// where the party moved it.
s32 Scene_Initialize(void) {
  struct FieldActor *stump;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (gGameState.scene == (s32)&SceneId_GomaCaveWaterway1) {
    if (gGameState.entrance == 5) {
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    } else {
      Object_GetById(STUMP)->collisionFlags |= 16;
      if (Engine_GameFlagIsSet(FLAG_GOMA_CAVE_WATERWAY_STUMP_MOVED) != 0) {
        Engine_ActorSetPosition(STUMP, PIXELS(346), PIXELS(292));
        stump = Object_GetById(STUMP);
        Engine_ActorSetSpriteFlags(stump, 0);
        Object_GetById(STUMP)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
        OBJ_MODE(STUMP, 2);
        Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
      }
    }
  }
  return 0;
}

// The leaping stump's update hook turns its sprite a fixed amount per frame.
static void GomaCaveWaterway_SpinStump(RotatingEffectWork *work) {
  RotatingEffectRecord *record = work->record;

  record->angle -= 0x800;
}

// Shared source is included here in ROM order.
#include "common/object_waitdown.h"

union SceneActor {
  struct {
    u32 unk00[2];
    s32 x, y, z;
    u32 unk14[3];
    u8 unk20[2];
    u8 field22;
    u8 unk23[5];
    s32 field28;
    u32 unk2c[3];
    u32 field38;
    u32 unk3c[3];
    s32 velocityY;
    u32 unk4c;
    struct Sprite *sprite;
    u8 unk54;
    u8 mode;
    u8 unk56[22];
    u32 callback;
  } fields;
  u8 bytes[112];
};
struct Vector {
  s32 x, y, z;
};

void Effect_Spawn();

// The stump spins in place, leaps and lands with a shake and a ring of dust,
// then comes to rest a little further on.
static void GomaCaveWaterway_LeapStump(s32 no) {
  union SceneActor *work;
  u32 cnt;
  struct Vector vec;
  union {
    struct ConfiguredEffectOptions fields;
    u8 bytes[sizeof(struct ConfiguredEffectOptions)];
  } opt;

  work = (union SceneActor *)Object_GetById(no);
  work->fields.mode = 0;
  for (cnt = 0; cnt < 18; cnt++) {
    Engine_TaskWait(1);
    work->fields.sprite->angle -= 256;
    work->fields.x -= Trig_Cos(work->fields.sprite->angle) / 2;
    work->fields.field38 = 0x80000000;
  }
  work->fields.callback = (u32)GomaCaveWaterway_SpinStump;
  OBJ_SPEED(no, PIXELS(3), 3 * PIXELS(1) / 2);
  Actor_SetDestination(no, 376, 288);
  work->fields.velocityY = 0xcccc;
  work->fields.mode = 3;
  work->fields.field22 = 0;
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
    Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, vec.x, vec.y,
                 vec.z, 0, NULL);
  }
  work->fields.field28 = 0x50000;
  Actor_SetDestination(no, 346, 292);
  Engine_ActorWaitForMove(no);
  SceneActor_WaitObjectBelowHeight(work, 0);
  work->fields.callback = 0;
  work->fields.sprite->angle = 0x1000;
  opt.fields.kind = 214;
  opt.fields.accum18 = 0x8000;
  opt.fields.accum1c = 0xcccc;
  opt.fields.target30 = 0x18000;
  opt.fields.target34 = 0x13333;
  Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0,
               0x1c0000, &opt.fields);
  Engine_AudioPlayCue(SE_CAST);
  OBJ_MODE(no, 3);
  Map_WaitWorkValuesBelow256();
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
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 376, 0, 776, FACING_NORTH, 0, 88, 8, 664, 832, 0},
    {2, CONDITION_ALWAYS, 168, 0, 552, FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {3, CONDITION_ALWAYS, 600, 0, 520, FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {4, CONDITION_ALWAYS, 376, 0, 168, FACING_SOUTH, 0, 88, 8, 664, 832, 0},
    {5, CONDITION_ALWAYS, 614, 0, 848, FACING_NORTH, 0, 480, 605, 720, 900, 0},
    {99, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88, 8, 664, 832, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gGomaCaveWaterwayEntrances2[] = {
    {1, CONDITION_ALWAYS, 154, 0, 350, FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {2, CONDITION_ALWAYS, 552, 0, 312, FACING_NORTH, 0, 16, 48, 632, 360, 0},
    {SCENE_TABLE_END},
};

u32 gGomaCaveWaterwayExits[] = {
    SCENE_EXITS(28),      SCENE_EXIT(1, 25, 2), SCENE_EXIT(2, 27, 1),
    SCENE_EXIT(3, 27, 2), SCENE_EXIT(4, 29, 1), SCENE_EXIT(5, 5, 11),
    SCENE_EXIT(0, 29, 0), SCENE_EXIT(1, 28, 4), SCENE_EXIT(2, 2, 40),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gGomaCaveWaterwayPlacementsOther[] = {
    { 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct ScenePlacement gGomaCaveWaterwayPlacements2[] = {
    {SCENE_TABLE_END},
};

struct SceneEvent gGomaCaveWaterwayEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 5},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, 1},
    {0x31, 6, CONDITION_ALWAYS, 2},
    {0x31, 7, CONDITION_ALWAYS, 3},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, 4},
    {SEARCH(SEARCH_CHEST), 100, 0xf28, FIND_ITEM(ITEM_POWER_BREAD)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), STUMP,
     FLAG_GOMA_CAVE_WATERWAY_STUMP_MOVED, EVENT_SCRIPT(Event_MoveStump)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gGomaCaveWaterwayEvents2[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {SCENE_EVENTS_END},
};
