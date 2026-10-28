#include "types.h"
#include "scene_tables.h"

void Func_02000054(void);
void Func_02000074(void);
void Func_02000094(void);
void Func_020000b4(void);
void Func_020000d4(void);
void Func_020000f4(void);
void Func_02000184(void);

extern u32 gData_02001fc0[];
extern u32 gData_02001ffc[];
extern u32 gData_02002038[];

// clang-format off
u32 gData_02001fc0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02001ffc[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002038[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(251),
    SCENE_EXIT(1, 2, 55),
    SCENE_EXIT(2, 254, 2),
    SCENE_EXIT(3, 254, 3),
    SCENE_EXIT(4, 252, 4),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(360), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(360), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_020000b4)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_020000d4)},
    {0x50008905, 35, CONDITION_ALWAYS, EVENT(Func_02000184)},
    {0x50008905, 36, CONDITION_ALWAYS, EVENT(Func_020000f4)},
    {TRIGGERS_END},
};

s32 gData_02002194[] = {
    -1,
};

u32 gData_02002198[] = {
    1,
};

u32 gData_0200219c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020021b0[] = {
    (s32)gData_02001fc0,
    (s32)gData_02001ffc,
    (s32)gData_02002038,
};
