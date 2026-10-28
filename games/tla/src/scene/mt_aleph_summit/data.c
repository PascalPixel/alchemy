#include "types.h"
#include "scene_tables.h"

extern u32 gData_0200159c[];
extern u32 gData_020015d8[];
extern u32 gData_02001614[];

// clang-format off
u32 gData_0200159c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020015d8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001614[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};
// clang-format on

s32 gData_02001650[] = {
    (s32)gData_0200159c,
    (s32)gData_020015d8,
    (s32)gData_02001614,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gScenePlacements[] = {
    {57, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(344), FACING_NORTHWEST, 0,
     0},
    {311, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(24), FACING_SOUTHEAST, 0,
     0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGERS_END},
};
