/// @file
/// Overlay 0x6b6's scene: its code, then its tables, in the order the ROM has them.

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

#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000054(void) {
  Func_080d7788(8, 0, 0);
  GameFlag_ClearBit(48);
}

void Func_02000068(void) {
  Func_080d7788(9, 1, 0);
  GameFlag_ClearBit(68);
}

void Func_0200007c(void) {
  Func_080d7788(10, 2, 0);
  GameFlag_ClearBit(88);
}

void Func_02000090(void) {
  Func_080d7788(11, 3, 0);
  GameFlag_ClearBit(108);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_020000ac(void) { return 0; }

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000054(void);
void Func_02000068(void);
void Func_0200007c(void);
void Func_02000090(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(313), SCENE_EXITS(314), SCENE_EXITS(315),
    SCENE_EXITS(316), SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000068)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_0200007c)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_02000090)},
    {TRIGGERS_END},
};

// clang-format off
s32 gData_0200015c[] = {
    0x2000200, 0x3600400, -1, -1,
};
// clang-format on
