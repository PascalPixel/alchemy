/// @file
/// Overlay 0x6b6's scene: its code, then its tables, in the order the ROM has
/// them.

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

void Test_ReceiveFizz(void);

void Test_ReceiveForge(void);

#include "event_inline.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Test_ReceiveFlint(void) {
  Djinn_JoinParty(8, 0, 0);
  GameFlag_Clear(48);
}

void Test_ReceiveFizz(void) {
  Djinn_JoinParty(9, 1, 0);
  GameFlag_Clear(68);
}

void Test_ReceiveForge(void) {
  Djinn_JoinParty(10, 2, 0);
  GameFlag_Clear(88);
}

void Test_ReceiveGust(void) {
  Djinn_JoinParty(11, 3, 0);
  GameFlag_Clear(108);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Func_020000ac(void) { return 0; }

s32 Scene_PrepareMap(void) { return 0; }

void Test_ReceiveFlint(void);
void Test_ReceiveGust(void);

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
    {0, 8, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0, 9, CONDITION_ALWAYS, EVENT(Test_ReceiveFizz)},
    {0, 10, CONDITION_ALWAYS, EVENT(Test_ReceiveForge)},
    {0, 11, CONDITION_ALWAYS, EVENT(Test_ReceiveGust)},
    {TRIGGER_TABLE_END},
};

// clang-format off
s32 gData_0200015c[] = {
    0x2000200, 0x3600400, -1, -1,
};
// clang-format on
