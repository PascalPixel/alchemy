#include "types.h"
#include "field_scene.h"

void Func_02000054(void);

extern u32 gData_02000090[];

// clang-format off
u32 gData_02000090[] = {
    22, 26, 0, 22, 22, 0x40000,
    22, 6, 0x30000, 22, 10, 0x18000,
    17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS(157),
    SCENE_EXIT_IF(1, 157, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 157, 1, 0xffffffff),
    SCENE_EXIT_IF(3, 155, 3, 0xffffffff),
    SCENE_EXIT_IF(4, 155, 4, 0xffffffff),
    SCENE_EXIT_IF(5, 155, 5, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    { 307, 0x8e9, (s32)gData_02000090, PIXELS(120), 0, PIXELS(608), FACING_NORTH, 2, 0 },
    { 307, 0x8ea, (s32)gData_02000090, PIXELS(184), 0, PIXELS(592), FACING_NORTH, 2, 0 },
    { 307, 0x8eb, (s32)gData_02000090, PIXELS(216), 0, PIXELS(592), FACING_NORTH, 2, 0 },
    { 307, 0x8ec, (s32)gData_02000090, PIXELS(280), 0, PIXELS(640), FACING_NORTH, 2, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSceneEvents[] = {
    { 1, 1, 0x18eb, 1 },
    { 1, 2, CONDITION_ALWAYS, 2 },
    { 1, 3, CONDITION_ALWAYS, 3 },
    { 1, 4, CONDITION_ALWAYS, 4 },
    { 1, 5, CONDITION_ALWAYS, 5 },
    { 0x4e15, 8, 0x8e9, EVENT_SCRIPT(Func_02000054) },
    { 0x4e15, 9, 0x8ea, EVENT_SCRIPT(Func_02000054) },
    { 0x4e15, 10, 0x8eb, EVENT_SCRIPT(Func_02000054) },
    { 0x4e15, 11, 0x8ec, EVENT_SCRIPT(Func_02000054) },
    { SCENE_EVENTS_END },
};
