#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000068(void);
void Func_02000094(void);

void Func_02000090(void);

extern u32 gData_020027a8[];
extern u32 gData_020027e4[];
extern u32 gData_02002820[];

// clang-format off
u32 gData_020027a8[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020027e4[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002820[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(261),
    SCENE_EXIT(1, 261, 2),
    SCENE_EXIT(2, 261, 1),
    SCENE_EXIT(3, 261, 4),
    SCENE_EXIT(4, 261, 3),
    SCENE_EXIT(5, 261, 6),
    SCENE_EXIT(6, 261, 5),
    SCENE_EXIT(7, 263, 7),
    SCENE_EXIT(8, 259, 8),
    SCENE_EXIT(9, 261, 10),
    SCENE_EXIT(10, 261, 9),
    SCENE_EXIT(11, 261, 12),
    SCENE_EXIT(12, 261, 11),
    SCENE_EXIT(13, 261, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {340, CONDITION_ALWAYS, 1, PIXELS(203), 0, PIXELS(904), FACING_SOUTHEAST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(296), FACING_SOUTHWEST, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(160), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(160), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(480), 0, PIXELS(200), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 1, PIXELS(560), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {49, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {49, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {514, 30, CONDITION_ALWAYS, EVENT(Func_02000068)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x10008c15, 10, CONDITION_ALWAYS, EVENT(Func_02000090)},
    {0x8c15, 10, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {TRIGGERS_END},
};

s32 gData_02002a84[] = {
    -1,
};

u32 gData_02002a88[] = {
    1,
};

u32 gData_02002a8c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02002aa0[] = {
    (s32)gData_020027a8,
    (s32)gData_020027e4,
    (s32)gData_02002820,
};
