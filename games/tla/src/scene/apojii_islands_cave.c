/// @file
/// Overlay 0x67a's scene: its code, then its tables, in the order the ROM has them.

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "ram_buffer.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

#include "event_inline.h"

void Func_02000054(s32 a0, s32 a1) { GameFlag_SetBit((a1 + 0x8e1)); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  FIELD_AT(((s32)Ram_HeapSlots->eventWork + 0x1ac), s32, 0) = 0x201;
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gData_02000090[];

// clang-format off
u32 gData_02000090[] = {
    22, 26, 0, 22, 22, 0x40000,
    22, 6, 0x30000, 22, 10, 0x18000,
    17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(157),
    SCENE_EXIT_IF(1, 157, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 157, 1, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 155, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 155, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 155, 5, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {307, 0x8e9, (s32)gData_02000090, PIXELS(120), 0, PIXELS(608), FACING_NORTH,
     2, 0},
    {307, 0x8ea, (s32)gData_02000090, PIXELS(184), 0, PIXELS(592), FACING_NORTH,
     2, 0},
    {307, 0x8eb, (s32)gData_02000090, PIXELS(216), 0, PIXELS(592), FACING_NORTH,
     2, 0},
    {307, 0x8ec, (s32)gData_02000090, PIXELS(280), 0, PIXELS(640), FACING_NORTH,
     2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, 0x18eb, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 8, 0x8e9, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, 0x8ea, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, 0x8eb, EVENT(Func_02000054)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, 0x8ec, EVENT(Func_02000054)},
    {TRIGGERS_END},
};
