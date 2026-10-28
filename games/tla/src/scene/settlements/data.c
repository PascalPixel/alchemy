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
    SCENE_EXIT_IF(1, 2, 48, 0xffffffff),
    SCENE_EXIT_IF(2, 214, 11, 0xffffffff),
    SCENE_EXIT_IF(3, 215, 1, 0xffffffff),
    SCENE_EXIT_IF(4, 215, 1, 0xffffffff),
    SCENE_EXITS(214),
    SCENE_EXIT_IF(1, 213, 2, 0xffffffff),
    SCENE_EXITS(215),
    SCENE_EXIT_IF(1, 213, 3, 0xffffffff),
    SCENE_EXIT_IF(2, 216, 4, 0xffffffff),
    SCENE_EXITS(216),
    SCENE_EXIT_IF(1, 2, 49, 0xffffffff),
    SCENE_EXIT_IF(2, 217, 11, 0xffffffff),
    SCENE_EXIT_IF(3, 217, 14, 0xffffffff),
    SCENE_EXIT_IF(4, 217, 14, 0xffffffff),
    SCENE_EXIT_IF(5, 217, 14, 0xffffffff),
    SCENE_EXIT_IF(6, 218, 2, 0xffffffff),
    SCENE_EXITS(218),
    SCENE_EXIT_IF(2, 216, 4, 0xffffffff),
    SCENE_EXITS(217),
    SCENE_EXIT_IF(11, 216, 2, 0xffffffff),
    SCENE_EXIT_IF(12, 217, 13, 0xffffffff),
    SCENE_EXIT_IF(13, 217, 12, 0xffffffff),
    SCENE_EXIT_IF(14, 216, 3, 0xffffffff),
    SCENE_EXITS(219),
    SCENE_EXIT_IF(1, 2, 50, 0xffffffff),
    SCENE_EXIT_IF(2, 220, 1, 0xffffffff),
    SCENE_EXITS(220),
    SCENE_EXIT_IF(1, 219, 2, 0xffffffff),
    SCENE_EXITS(221),
    SCENE_EXIT_IF(1, 2, 47, 0xffffffff),
    SCENE_EXIT_IF(2, 222, 11, 0xffffffff),
    SCENE_EXITS(222),
    SCENE_EXIT_IF(1, 221, 2, 0xffffffff),
    SCENE_EXITS_END,
};

u32 gData_020024ac[] = {
    0xffff0038, 1, 0x1380000, 0, 0x3280000, 0xc000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020024dc[] = {
    0xffff0073, 2, 0xc00000, 0, 0xf00000, 0x1c000,
    0xffff006a, 2, 0x1500000, 0, 0xf00000, 0x1c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002524[] = {
    0xffff0074, 3, 0x780000, 0, 0x2780000, 0x14000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02002554[] = {
    (s32)0xffff0072,
    2,
    0x1880000,
    0,
    0x1180000,
    0x1c000,
    (s32)0xffff012b,
    7,
    0x380000,
    0,
    0xf00000,
    0x2c000,
    (s32)0xffff011f,
    (s32)gData_0200231c,
    0x1180000,
    0,
    0xb80000,
    0x2c000,
    0x6600f5,
    1,
    0x1380000,
    0,
    0xf80000,
    0x8000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_020025cc[] = {
    0xffff0074, 1, 0x2980000, 0, 0xa80000, 0x14000,
    0xffff006a, 3, 0x2b80000, 0, 0x1c80000, 0x14000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02002614[] = {
    (s32)0xffff005d,
    1,
    0x1780000,
    0,
    0xb00000,
    0x14000,
    (s32)0xffff0110,
    1,
    0xd80000,
    0,
    0xc80000,
    0x14000,
    (s32)0xffff0120,
    1,
    0x1180000,
    0,
    0xa80000,
    0x14000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x780000,
    0,
    0x480000,
    0x14000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x480000,
    0,
    0x680000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x580000,
    0,
    0x680000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x780000,
    0,
    0x680000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x480000,
    0,
    0x880000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x680000,
    0,
    0x880000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x680000,
    0,
    0x980000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x780000,
    0,
    0xb80000,
    0x14000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x980000,
    0,
    0xb80000,
    0x1014000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0x980000,
    0,
    0x980000,
    0x14000,
    (s32)0xffff0112,
    (s32)gData_020022e4,
    0,
    0,
    0,
    0x14000,
    (s32)0xffff0149,
    1,
    0xc80000,
    0,
    0x5c0000,
    0x14000,
    0x7f00f6,
    1,
    0x480000,
    0,
    0x680000,
    0x8000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_020027ac[] = {
    0xffff005d, 1, 0x600000, 0, 0x3300000, 0x14000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020027dc[] = {
    0xffff0197, 1, 0x780000, 0, 0xc80000, 0x2c000,
    0xffff0197, 1, 0xb80000, 0, 0xc80000, 0x2c000,
    0x5200f4, 1, 0x1380000, 0, 0x1280000, 0x8000,
    0xffff01e9, 7, 0x580000, 0, 0x1500000, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002854[] = {
    0xffff004b, 1, 0xb00000, 0, 0xd80000, 0x14000,
    0xffff004c, 1, 0x980000, 0, 0x880000, 0x1c000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_0200289c[] = {
    1, 0, 1, -1, 1, 0, 1, 0,
    2, -1, 2, 0, 1, 0, 11, -1,
    9, 0, 1, 0, 12, -1, 9, 0,
    1, 0, 13, -1, 9, 0, 1, 0,
    14, -1, 9, 0, -1, -1, 0, 0,
    0, 0,
};
// clang-format on

s32 gData_020028f0[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    0x4602,
    (s32)0xffff0003,
    (s32)Func_02000624,
    0x602,
    (s32)0xffff0004,
    (s32)Func_020005cc,
    0,
    (s32)0xffff0008,
    (s32)Func_0200043c,
    0,
    (s32)0xffff0009,
    MSGNO(MsgSettlements03),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements04),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgSettlements06),
    0x50008905,
    (s32)0xffff000a,
    (s32)Func_02000488,
    0x50008905,
    (s32)0xffff000b,
    (s32)Func_020004a8,
    -1,
    0,
    0,
};

s32 gData_02002974[] = {
    1,
    (s32)0xffff000b,
    1,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements02),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements05),
    -1,
    0,
    0,
};

s32 gData_020029a4[] = {
    1,
    (s32)0xffff0001,
    1,
    0xc602,
    (s32)0xffff0002,
    (s32)Func_02000c44,
    0x4602,
    (s32)0xffff0003,
    (s32)Func_02000624,
    0x602,
    (s32)0xffff0004,
    (s32)Func_020005cc,
    0x8602,
    (s32)0xffff0005,
    (s32)Func_020005f4,
    0xc401,
    (s32)0xffff0006,
    6,
    2,
    0x2200022,
    (s32)Func_0200125c,
    2,
    0x12200023,
    (s32)Func_020012c0,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements07),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements10),
    0,
    (s32)0xffff000b,
    (s32)Func_02000290,
    0x10008c15,
    (s32)0xffff000a,
    (s32)Func_02001304,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_0200155c,
    8,
    -0x10000,
    (s32)Func_02001304,
    9,
    -0x10000,
    (s32)Func_0200155c,
    0xc15,
    0x2300009,
    (s32)Func_02000644,
    -1,
    0,
    0,
};

s32 gData_02002a70[] = {
    1,
    (s32)0xffff000b,
    11,
    33,
    (s32)0xffff000c,
    12,
    49,
    (s32)0xffff000d,
    13,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements08),
    0,
    (s32)0xffff0009,
    MSGNO(MsgSettlements09),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements11),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgSettlements12),
    -1,
    0,
    0,
};

s32 gData_02002ad0[] = {
    1,
    (s32)0xffff0001,
    1,
    0xc602,
    (s32)0xffff0002,
    (s32)Func_02000c44,
    2,
    (s32)0xffff000a,
    (s32)Func_02000c1c,
    2,
    (s32)0xffff000b,
    (s32)Func_02000c2c,
    2,
    0x2010014,
    (s32)Func_02000c3c,
    0x8515,
    0x2030008,
    (s32)Func_02000cfc,
    0,
    (s32)0xffff000a,
    (s32)Func_02000280,
    0x9415,
    0xf5d000b,
    (s32)Func_02000404,
    -1,
    0,
    0,
};

s32 gData_02002b3c[] = {
    1,
    (s32)0xffff0001,
    1,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements13),
    0,
    (s32)0xffff0009,
    MSGNO(MsgSettlements14),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements15),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgSettlements16),
    371,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgSettlements21),
    -1,
    0,
    0,
};

s32 gData_02002b90[] = {
    0xc401,
    (s32)0xffff0001,
    1,
    0xc401,
    (s32)0xffff0002,
    2,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements18),
    0,
    (s32)0xffff0017,
    (s32)Func_020002a0,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements20),
    0x10008c15,
    0x2040009,
    (s32)Func_02000ef0,
    0x8c15,
    0x2040009,
    (s32)Func_020011bc,
    0x10008c15,
    0x205000a,
    (s32)Func_02000ef0,
    0x8c15,
    0x205000a,
    (s32)Func_020011c8,
    8,
    -0x10000,
    (s32)Func_02000f00,
    9,
    -0x10000,
    (s32)Func_020011d4,
    0x8f15,
    0x2080016,
    (s32)Func_020016a4,
    -1,
    0,
    0,
};

s32 gData_02002c2c[] = {
    1,
    (s32)0xffff000b,
    1,
    0,
    (s32)0xffff0008,
    MSGNO(MsgSettlements17),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgSettlements19),
    -1,
    0,
    0,
};

s32 gData_02002c5c[] = {
    -1,
};
