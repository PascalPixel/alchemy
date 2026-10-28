#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000078(void);
void Func_02000168(void);
void Func_02000ce0(void);
void Func_02001064(void);
void Func_02001074(void);
void Func_02001178(void);
void Func_0200128c(void);
void Func_020014c0(void);
void Func_02001da0(void);

// clang-format off
s16 gData_020020dc[] = {
    8, 512, -1, 0,
};

u32 gData_020020e4[] = {
    39, 2, 4, 0x1070000, 0, 0xb00000,
    1, 39, 1, 0, 1, 3,
    0, 1, 17,
};

u32 gData_02002120[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(368), 0, PIXELS(448)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(10),
    SC_END,
};

u32 gData_0200218c[] = {
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(30),
    SC_SET(SW_FACING, FACING_SOUTHWEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(70),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_020021e8[] = {
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_SOUTHEAST),
    SC_WAIT(70),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_0200221c[] = {
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(70),
    SC_SET(SW_FACING, FACING_NORTHWEST + FACING_STEP),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH + FACING_STEP),
    SC_WAIT(10),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(30),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(43),
    SCENE_EXIT(1, 40, 1),
    SCENE_EXIT(1, 2, 68),
    SCENE_EXIT(2, 2, 8),
    SCENE_EXITS(42),
    SCENE_EXIT(1, 2, 42),
    SCENE_EXIT(2, 2, 8),
    SCENE_EXIT(10, 2, 71),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020022cc[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200235c[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_020023ec[] = {
    {TRIGGERS_END},
};

struct Trigger gData_020023f8[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000078)},
    {1, 2, CONDITION_ALWAYS, 2},
    {2, 10, 0x980, EVENT(Func_02000ce0)},
    {2, 11, 0x981, EVENT(Func_020014c0)},
    {2, 12, 0x989, EVENT(Func_02001da0)},
    {2, 1, 0x985, EVENT(Func_0200128c)},
    {3, 14, 0x985, EVENT(Func_02000168)},
    {2, 15, 0x985, EVENT(Func_02001074)},
    {2, 16, 0x985, EVENT(Func_02001178)},
    {0x1815, 8, 0x200, EVENT(Func_02001064)},
    {TRIGGERS_END},
};

u32 gData_0200247c[] = {
    0, 10, 1, 3, 0, 1,
    38,
};

u32 gData_02002498[] = {
    0, 15, 3, 0, 1, 38,
};
// clang-format on
