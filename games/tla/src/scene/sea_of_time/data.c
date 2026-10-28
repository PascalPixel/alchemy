#include "types.h"
#include "scene_tables.h"

void Func_0200033c(void);
void Func_0200043c(void);
void Func_0200054c(void);
void Func_02000818(void);
void Func_02000a80(void);
void Func_02000c94(void);
void Func_02000da0(void);
void Func_02000ef0(void);
void Func_02001068(void);
void Func_02001108(void);
void Func_02001480(void);
void Func_020014b4(void);
void Func_020014ec(void);
void Func_02001524(void);
void Func_0200155c(void);
void Func_02001594(void);
void Func_020015cc(void);
void Func_020015ec(void);

extern u32 gData_0200205c[];
extern u32 gData_02002098[];
extern u32 gData_020020d4[];
extern s32 gData_02002154[];

// clang-format off
u32 gData_0200205c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002098[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020020d4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002110[] = {
    0x18101810, 0x1810,
};

u16 gData_02002118[] = {
    0, 672, 0, 704, 0, 896, 0, 320,
    0, 512, 0, 192, 0, 320, 0, 416,
    0, 320, 0, 832, 0, 96, 0, 160,
};
// clang-format on

s32 gData_02002148[] = {
    (s32)gData_0200205c,
    (s32)gData_02002098,
    (s32)gData_020020d4,
};

s32 gData_02002154[] = {
    46,
    (s32)Func_0200033c,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(197),
    SCENE_EXIT_IF(1, 2, 42, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 197, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 42, 20, EXIT_FLAG_NONE),
    SCENE_EXITS(198),
    SCENE_EXIT_IF(1, 197, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 198, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 2, 44, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 2, 43, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 198, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020021e4[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02002154, PIXELS(120), 0, PIXELS(280),
     FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002214[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02002154, PIXELS(120), 0, PIXELS(280),
     FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {424, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

u16 gData_020022bc[] = {
    0,
};

u16 gData_020022be[] = {
    0,
};

u32 gData_020022c0[] = {
    0,
};

u32 gData_020022c4[] = {
    0,
};

u32 gData_020022c8[] = {
    0,
};

u32 gData_020022cc[] = {
    0,
};

struct Trigger gData_020022d0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {2, 49, CONDITION_ALWAYS, EVENT(Func_02001068)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 32, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 36, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 37, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 40, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {2, 41, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {2, 44, CONDITION_ALWAYS, EVENT(Func_02000ef0)},
    {2, 45, 0x200, EVENT(Func_02000818)},
    {2, 46, 0x200, EVENT(Func_02000818)},
    {2, 47, 0x200, EVENT(Func_02000818)},
    {2, 48, 0x200, EVENT(Func_02000818)},
    {2, 50, 0x200, EVENT(Func_02000a80)},
    {2, 51, 0x200, EVENT(Func_02000a80)},
    {2, 52, 0x200, EVENT(Func_02000a80)},
    {2, 53, 0x200, EVENT(Func_02000a80)},
    {2, 54, 0x200, EVENT(Func_02000a80)},
    {2, 55, 0x200, EVENT(Func_02000a80)},
    {2, 56, 0x200, EVENT(Func_02000a80)},
    {2, 57, 0x200, EVENT(Func_02000a80)},
    {6, 200, CONDITION_ALWAYS, EVENT(Func_02000c94)},
    {6, 201, CONDITION_ALWAYS, EVENT(Func_02000da0)},
    {TRIGGERS_END},
};

struct Trigger gData_0200242c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {2, 5, 0x940, EVENT(Func_02001108)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 32, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 33, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 34, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 35, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 36, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 37, CONDITION_ALWAYS, EVENT(Func_0200054c)},
    {2, 48, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 41, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_02001480)},
    {0x9815, 10, CONDITION_ALWAYS, EVENT(Func_020014b4)},
    {0x9815, 11, CONDITION_ALWAYS, EVENT(Func_020014ec)},
    {0x9815, 12, CONDITION_ALWAYS, EVENT(Func_02001524)},
    {0x9815, 13, CONDITION_ALWAYS, EVENT(Func_0200155c)},
    {TRIGGERS_END},
};

struct Trigger gData_0200251c[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_020015cc)},
    {2, 2, CONDITION_ALWAYS, EVENT(Func_020015ec)},
    {2, 45, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 46, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 47, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {2, 48, CONDITION_ALWAYS, EVENT(Func_02000818)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_02001594)},
    {TRIGGERS_END},
};

u16 gData_0200257c[] = {
    1,
};
