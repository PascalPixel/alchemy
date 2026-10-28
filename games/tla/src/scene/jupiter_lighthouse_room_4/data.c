#include "types.h"
#include "scene_tables.h"

void Func_02000054(void);
void Func_020000c0(void);
void Func_02000108(void);
void Func_02000114(void);
void Func_02000164(void);
void Func_02000170(void);
void Func_020001dc(void);
void Func_020001fc(void);
void Func_0200021c(void);

extern u32 gData_02002384[];
extern u32 gData_020023c0[];
extern u32 gData_020023fc[];

// clang-format off
u32 gData_02002384[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020023c0[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020023fc[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(257),
    SCENE_EXIT(1, 258, 1),
    SCENE_EXIT(2, 256, 2),
    SCENE_EXIT(3, 257, 4),
    SCENE_EXIT(4, 257, 3),
    SCENE_EXIT(5, 257, 6),
    SCENE_EXIT(6, 257, 5),
    SCENE_EXIT(7, 257, 8),
    SCENE_EXIT(8, 257, 7),
    SCENE_EXIT(9, 263, 9),
    SCENE_EXIT(10, 263, 10),
    SCENE_EXIT(11, 257, 11),
    SCENE_EXIT(12, 256, 12),
    SCENE_EXIT(13, 256, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gScenePlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(136), FACING_SOUTHEAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(136), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, -0x80000, 0, -0x80000, FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(520), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(520), FACING_SOUTHEAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(520), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(520), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {3, 20, 0xa07, EVENT_SCRIPT(Func_0200021c)},
    {3, 21, 0xa08, EVENT_SCRIPT(Func_0200021c)},
    {0xf304, 20, 0xa07, EVENT_SCRIPT(Func_020001dc)},
    {0xf404, 21, 0xa08, EVENT_SCRIPT(Func_020001fc)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000c0)},
    {0x10008c15, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000108)},
    {0x8c15, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000114)},
    {0x50008905, 40, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000164)},
    {0x50008905, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000170)},
    {TRIGGERS_END},
};

s32 gData_020026f0[] = {
    -1,
};

u32 gData_020026f4[] = {
    1,
};

u32 gData_020026f8[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_0200270c[] = {
    (s32)gData_02002384,
    (s32)gData_020023c0,
    (s32)gData_020023fc,
};
