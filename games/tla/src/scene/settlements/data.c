#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_02000280(void);
void Func_02000290(void);
void Func_020002a0(void);
void Func_020002b0(void);
void Func_020002bc(void);
void Func_02000404(void);
void Func_0200043c(void);
void Func_02000488(void);
void Func_020004a8(void);
void Func_020005cc(void);
void Func_020005f4(void);
void Func_02000624(void);
void Func_02000644(void);
void Func_02000c1c(void);
void Func_02000c2c(void);
void Func_02000c3c(void);
void Func_02000c44(void);
void Func_02000cfc(void);
void Func_02000ef0(void);
void Func_02000f00(void);
void Func_020011bc(void);
void Func_020011c8(void);
void Func_020011d4(void);
void Func_0200125c(void);
void Func_020012c0(void);
void Func_02001304(void);
void Func_0200155c(void);
void Func_020016a4(void);

extern u32 gData_020021b8[];
extern u32 gData_020021f4[];
extern u32 gData_02002230[];
extern s32 gData_020022e4[];
extern u32 gData_0200231c[];

// clang-format off
u32 gData_020021b8[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020021f4[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002230[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_0200226c[] = {
    9, 10, -1,
};

s16 gData_02002272[] = {
    10, -1,
};

s16 gData_02002276[] = {
    -1, 0, 0x8000, -1, 0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000, -1, 0, 0x8000, -1,
    -1, 0, 0x8000, -1, 0xc000, -1, -1, 0xc000,
    0x4000, -1, -1, 0x4000, -1, 0, 0x8000, -1,
};

s16 gData_020022b6[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

s32 gData_020022d8[] = {
    (s32)gData_020021b8,
    (s32)gData_020021f4,
    (s32)gData_02002230,
};

s32 gData_020022e4[] = {
    39,
    3,
    22,
    26,
    0,
    23,
    6,
    -0x100000,
    46,
    (s32)Func_020002b0,
    17,
};

u32 gData_02002310[] = {
    39, 2, 17,
};

u32 gData_0200231c[] = {
    22, 26, 0, 17,
};

s32 gData_0200232c[] = {
    46,
    (s32)Func_020002bc,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 216, 0, 216, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 320, 0, 830, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02002380[] = {
    0x2e02a0, 0x2b00190, 0x1a0003e, 0xeffff, 0xffff, 0,
    0, 0,
};

u32 gData_020023a0[] = {
    0xc00d4, 0xdc0084, 0x8c0014, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(213),
    SCENE_EXIT_IF(1, 2, 48, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 214, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 215, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 215, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(214),
    SCENE_EXIT_IF(1, 213, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(215),
    SCENE_EXIT_IF(1, 213, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 216, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(216),
    SCENE_EXIT_IF(1, 2, 49, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 217, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(3, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(5, 217, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(6, 218, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(218),
    SCENE_EXIT_IF(2, 216, 4, EXIT_FLAG_NONE),
    SCENE_EXITS(217),
    SCENE_EXIT_IF(11, 216, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(12, 217, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(13, 217, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(14, 216, 3, EXIT_FLAG_NONE),
    SCENE_EXITS(219),
    SCENE_EXIT_IF(1, 2, 50, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 220, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(220),
    SCENE_EXIT_IF(1, 219, 2, EXIT_FLAG_NONE),
    SCENE_EXITS(221),
    SCENE_EXIT_IF(1, 2, 47, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(2, 222, 11, EXIT_FLAG_NONE),
    SCENE_EXITS(222),
    SCENE_EXIT_IF(1, 221, 2, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020024ac[] = {
    {56, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(808), FACING_NORTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020024dc[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(192), 0, PIXELS(240), FACING_NORTH, 1, 0},
    {106, CONDITION_ALWAYS, 2, PIXELS(336), 0, PIXELS(240), FACING_NORTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002524[] = {
    {116, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(632), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002554[] = {
    {114, CONDITION_ALWAYS, 2, PIXELS(392), 0, PIXELS(280), FACING_NORTH, 1, 0},
    {299, CONDITION_ALWAYS, 7, PIXELS(56), 0, PIXELS(240), FACING_NORTH, 2, 0},
    {287, CONDITION_ALWAYS, (s32)gData_0200231c, PIXELS(280), 0, PIXELS(184),
     FACING_NORTH, 2, 0},
    {245, 0x66, 1, PIXELS(312), 0, PIXELS(248), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020025cc[] = {
    {116, CONDITION_ALWAYS, 1, PIXELS(664), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {106, CONDITION_ALWAYS, 3, PIXELS(696), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002614[] = {
    {93, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(176), FACING_SOUTH, 1, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(200), FACING_SOUTH, 1, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(168), FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(120), 0, PIXELS(72),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(72), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(88), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(120), 0, PIXELS(104),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(72), 0, PIXELS(136),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(104), 0, PIXELS(136),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(104), 0, PIXELS(152),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(120), 0, PIXELS(184),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(152), 0, PIXELS(184),
     FACING_SOUTH, 1, 1},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, PIXELS(152), 0, PIXELS(152),
     FACING_SOUTH, 1, 0},
    {274, CONDITION_ALWAYS, (s32)gData_020022e4, 0, 0, 0, FACING_SOUTH, 1, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(92), FACING_SOUTH, 1, 0},
    {246, 0x7f, 1, PIXELS(72), 0, PIXELS(104), FACING_WEST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020027ac[] = {
    {93, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(816), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020027dc[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {244, 0x52, 1, PIXELS(312), 0, PIXELS(296), FACING_WEST, 0, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(88), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002854[] = {
    {75, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(216), FACING_SOUTH, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_0200289c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 11, CONDITION_ALWAYS, 9},
    {1, 12, CONDITION_ALWAYS, 9},
    {1, 13, CONDITION_ALWAYS, 9},
    {1, 14, CONDITION_ALWAYS, 9},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_020028f0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0x4602, 3, CONDITION_ALWAYS, EVENT(Func_02000624)},
    {0x602, 4, CONDITION_ALWAYS, EVENT(Func_020005cc)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements03)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements04)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements06)},
    {0x50008905, 10, CONDITION_ALWAYS, EVENT(Func_02000488)},
    {0x50008905, 11, CONDITION_ALWAYS, EVENT(Func_020004a8)},
    {TRIGGERS_END},
};

struct Trigger gData_02002974[] = {
    {1, 11, CONDITION_ALWAYS, 1},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements02)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements05)},
    {TRIGGERS_END},
};

struct Trigger gData_020029a4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000c44)},
    {0x4602, 3, CONDITION_ALWAYS, EVENT(Func_02000624)},
    {0x602, 4, CONDITION_ALWAYS, EVENT(Func_020005cc)},
    {0x8602, 5, CONDITION_ALWAYS, EVENT(Func_020005f4)},
    {0xc401, 6, CONDITION_ALWAYS, 6},
    {2, 34, 0x220, EVENT(Func_0200125c)},
    {2, 35, 0x1220, EVENT(Func_020012c0)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements07)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements10)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_02000290)},
    {0x10008c15, 10, CONDITION_ALWAYS, EVENT(Func_02001304)},
    {0x8c15, 10, CONDITION_ALWAYS, EVENT(Func_0200155c)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02001304)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_0200155c)},
    {0xc15, 9, 0x230, EVENT(Func_02000644)},
    {TRIGGERS_END},
};

struct Trigger gData_02002a70[] = {
    {1, 11, CONDITION_ALWAYS, 11},
    {33, 12, CONDITION_ALWAYS, 12},
    {49, 13, CONDITION_ALWAYS, 13},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements08)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements09)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements11)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements12)},
    {TRIGGERS_END},
};

struct Trigger gData_02002ad0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000c44)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02000c1c)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_02000c2c)},
    {2, 20, 0x201, EVENT(Func_02000c3c)},
    {0x8515, 8, 0x203, EVENT(Func_02000cfc)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000280)},
    {0x9415, 11, 0xf5d, EVENT(Func_02000404)},
    {TRIGGERS_END},
};

struct Trigger gData_02002b3c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements13)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements14)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements15)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgSettlements16)},
    {0x173, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSettlements21))},
    {TRIGGERS_END},
};

struct Trigger gData_02002b90[] = {
    {0xc401, 1, CONDITION_ALWAYS, 1},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements18)},
    {0, 23, CONDITION_ALWAYS, EVENT(Func_020002a0)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements20)},
    {0x10008c15, 9, 0x204, EVENT(Func_02000ef0)},
    {0x8c15, 9, 0x204, EVENT(Func_020011bc)},
    {0x10008c15, 10, 0x205, EVENT(Func_02000ef0)},
    {0x8c15, 10, 0x205, EVENT(Func_020011c8)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000f00)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_020011d4)},
    {0x8f15, 22, 0x208, EVENT(Func_020016a4)},
    {TRIGGERS_END},
};

struct Trigger gData_02002c2c[] = {
    {1, 11, CONDITION_ALWAYS, 1},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements17)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgSettlements19)},
    {TRIGGERS_END},
};

s32 gData_02002c5c[] = {
    -1,
};
