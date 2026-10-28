#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_0200004c(void);
void Func_020002c8(void);
void Func_020002e8(void);
void Func_020002fc(void);
void Func_0200030c(void);
void Func_020006c8(void);
void Func_02000abc(void);
void Func_020010e4(void);
void Func_02001154(void);
void Func_020011c4(void);
void Func_02001234(void);
void Func_020013a0(void);

extern s32 gData_020032c8[];

s32 gData_020032c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
u32 gData_020032d4[] = {
    SC_WAIT(30),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02003304[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02003348[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_0200338c[] = {
    201, 202, 203, 204, 205, 206, 207, -1,
};

s16 gData_0200339c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020033c8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020033f4[] = {
    0xc900ce, 0xcb00ca, 0xcd00cc, 0xce00cd, 0xca00c9, 0xcc00cb,
    0xcd00cc, 0xc900ce, 0xcb00ca, 0xcc00cb, 0xce00cd, 0xca00c9,
    0xcb00ca, 0xcd00cc, 0xc900ce, 0xca00c9, 0xcc00cb, 0xce00cd,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(296),
    SCENE_EXIT(1, 295, 5),
    SCENE_EXIT(2, 293, 8),
    SCENE_EXIT(3, 292, 4),
    SCENE_EXIT(4, 294, 15),
    SCENE_EXITS(287),
    SCENE_EXIT(1, 297, 1),
    SCENE_EXIT(2, 280, 2),
    SCENE_EXIT(3, 291, 1),
    SCENE_EXIT(4, 288, 1),
    SCENE_EXIT(5, 290, 1),
    SCENE_EXIT(6, 289, 1),
    SCENE_EXIT(7, 287, 8),
    SCENE_EXIT(8, 287, 7),
    SCENE_EXIT(9, 287, 10),
    SCENE_EXIT(10, 287, 9),
    SCENE_EXIT(11, 287, 12),
    SCENE_EXIT(12, 287, 11),
    SCENE_EXIT(13, 287, 14),
    SCENE_EXIT(14, 287, 13),
    SCENE_EXITS(288),
    SCENE_EXIT(1, 287, 4),
    SCENE_EXIT(2, 293, 1),
    SCENE_EXITS(289),
    SCENE_EXIT(1, 287, 6),
    SCENE_EXIT(2, 294, 1),
    SCENE_EXITS(290),
    SCENE_EXIT(1, 287, 5),
    SCENE_EXIT(2, 292, 1),
    SCENE_EXITS(291),
    SCENE_EXIT(1, 287, 3),
    SCENE_EXIT(2, 295, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020034f0[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003508[] = {
    {418, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(184), FACING_EAST, 2, 0},
    {365, CONDITION_ALWAYS, 7, PIXELS(504), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(112), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(232), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(232), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003598[] = {
    {416, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_EAST, 2, 0},
    {416, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_WEST, 2, 1},
    {416, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(216), FACING_EAST, 2, 1},
    {416, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_WEST, 2, 1},
    {365, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(224), 0, PIXELS(144), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_020036b8[] = {
    {TRIGGERS_END},
};

s32 gData_020036c4[] = {
    -1,
};

struct Trigger gData_020036c8[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_020002c8)},
    {0x10009a15, 8, CONDITION_ALWAYS, EVENT(Func_020002e8)},
    {0x60009a15, 10, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {0x20009a15, 10, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {TRIGGERS_END},
};

s32 gData_02003704[] = {
    -1,
};

struct Trigger gData_02003708[] = {
    {0x31, 2, CONDITION_ALWAYS, 2},
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
    {1, 13, CONDITION_ALWAYS, 13},
    {1, 14, CONDITION_ALWAYS, 14},
    {2, 20, 0x1a30, EVENT(Func_020006c8)},
    {2, 21, 0x1a30, EVENT(Func_020006c8)},
    {2, 22, 0x1a30, EVENT(Func_020006c8)},
    {2, 23, 0x1a30, EVENT(Func_020006c8)},
    {2, 24, 0x1a30, EVENT(Func_020006c8)},
    {2, 25, 0x1a30, EVENT(Func_020006c8)},
    {2, 26, 0x1a30, EVENT(Func_020006c8)},
    {2, 27, 0x1a30, EVENT(Func_020006c8)},
    {0x9c05, 1, CONDITION_ALWAYS, EVENT(Func_02000abc)},
    {3, 30, CONDITION_ALWAYS, EVENT(Func_020010e4)},
    {3, 31, CONDITION_ALWAYS, EVENT(Func_02001154)},
    {3, 32, CONDITION_ALWAYS, EVENT(Func_020011c4)},
    {3, 33, CONDITION_ALWAYS, EVENT(Func_02001234)},
    {2, 34, 0xa2e, EVENT(Func_020013a0)},
    {TRIGGERS_END},
};

struct Trigger gData_02003858[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};

s32 gData_0200387c[] = {
    23, 9, 0x3000, 23, 10, 0x3000,
    23, 6, -0x1800, 0, 1, 13,
    8, 0, (s32)0x80010000, 23, 9, -0x3000,
    23, 10, -0x3000, 23, 6, 0x1800,
    0, 1, 13, 12, (s32)0xc0010000, 3,
    0, 1, 38,
};

u32 gData_02003900[] = {
    0, 20, 3, 0, 1, 38,
};
// clang-format on
