#include "types.h"
#include "field_scene.h"

void Func_02000038(void);
void Func_020000b0(void);
void Func_020000c4(void);
void Func_020000d8(void);
void Func_02000110(void);
void Func_020003bc(void);
void Func_020003dc(void);
void Func_020003fc(void);
void Func_0200041c(void);
void Func_0200043c(void);
void Func_0200045c(void);
void Func_02000468(void);
void Func_02000534(void);
void Func_02000830(void);

void Func_020000d4(void);

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS(201),
    SCENE_EXIT_IF(1, 201, 4, 0xffffffff),
    SCENE_EXIT_IF(2, 200, 2, 0xffffffff),
    SCENE_EXIT_IF(3, 202, 4, 0xffffffff),
    SCENE_EXIT_IF(4, 201, 1, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    { 289, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(584), FACING_SOUTH, 2, 0 },
    { 157, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(504), 0x3000, 0, 0 },
    { 157, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(504), 0x5000, 0, 0 },
    { 56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0 },
    { 7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0 },
    { 244, 0x51, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 0, 0 },
    { 289, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(184), FACING_SOUTH, 2, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSceneEvents[] = {
    { 1, 1, CONDITION_ALWAYS, 1 },
    { 2, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000534) },
    { 33, 3, CONDITION_ALWAYS, 3 },
    { 1, 4, CONDITION_ALWAYS, 4 },
    { 2, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000b0) },
    { 2, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000c4) },
    { 0x602, 22, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000830) },
    { 0, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000038) },
    { 0x10008c15, 8, 0x961, (u32)(Func_020000d4) },
    { 0x8c15, 8, 0x961, EVENT_SCRIPT(Func_020000d8) },
    { 0x50008905, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000110) },
    { 0x50008905, 31, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020003bc) },
    { 0x50008905, 32, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020003dc) },
    { 0x50008905, 33, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020003fc) },
    { 0x50008905, 34, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200041c) },
    { 0x50008905, 35, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200043c) },
    { 0x10008715, 13, 0x196e, EVENT_SCRIPT(Func_0200045c) },
    { 0x8715, 13, 0x196e, EVENT_SCRIPT(Func_02000468) },
    { SCENE_EVENTS_END },
};

// clang-format off
u32 gData_02001c30[] = {
    0x62c85f0c, 0x6a406684, 0x69c06a00, 0x69c06980, 0x6a406a00, 0x62c86684,
};
// clang-format on
