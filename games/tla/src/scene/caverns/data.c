#include "types.h"
#include "scene_tables.h"

void Func_020006a8(void);
void Func_0200087c(void);
void Func_02000bf8(void);
void Func_02000cdc(void);
void Func_02000e20(void);
void Func_02000e44(void);
void Func_02000e6c(void);
void Func_02000e88(void);
void Func_02000ef4(void);
void Func_02000fe4(void);

extern u32 gData_0200116c[];
extern u32 gData_020011a8[];
extern u32 gData_020011e4[];

// clang-format off
u32 gData_0200116c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020011a8[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020011e4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02001220[] = {
    8, 9, -1, 0,
};

s32 gData_02001228[] = {
    (s32)gData_0200116c,
    (s32)gData_020011a8,
    (s32)gData_020011e4,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 467, 0, 153, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gData_02001264[] = {
    132, 28, 692, 140, 36, 700, -1, 4,
    292, 28, 692, 300, 36, 700, -1, 5,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(276),
    SCENE_EXIT_IF(1, 2, 82, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 276, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 276, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 276, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 276, 3, CONDITION_ALWAYS),
    SCENE_EXITS(275),
    SCENE_EXIT_IF(1, 2, 83, CONDITION_ALWAYS),
    SCENE_EXITS(277),
    SCENE_EXIT_IF(5, 2, 88, CONDITION_ALWAYS),
    SCENE_EXITS(278),
    SCENE_EXIT_IF(1, 2, 89, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 278, 3, CONDITION_ALWAYS),
    SCENE_EXIT_IF(3, 278, 2, CONDITION_ALWAYS),
    SCENE_EXIT_IF(4, 278, 5, CONDITION_ALWAYS),
    SCENE_EXIT_IF(5, 278, 4, CONDITION_ALWAYS),
    SCENE_EXIT_IF(6, 278, 7, CONDITION_ALWAYS),
    SCENE_EXIT_IF(7, 278, 6, CONDITION_ALWAYS),
    SCENE_EXIT_IF(8, 278, 9, CONDITION_ALWAYS),
    SCENE_EXIT_IF(9, 278, 8, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

u32 gData_02001328[] = {
    0xffff01e9, 7, 0, 0, 0, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001358[] = {
    0xffff0197, 1, 0x1480000, 0, 0xa80000, 0x2c000,
    0xffff0197, 1, 0x1180000, 0, 0xa80000, 0x2c000,
    0xffff0110, 1, 0xd80000, 0, 0xa80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020013b8[] = {
    0xffff013b, 1, 0x2180000, 0, 0x2280000, 0x2c000,
    0xffff013b, 1, 0x2580000, 0, 0x2180000, 0x2c000,
    0xffff0182, 1, 0x1e80000, 0, 0x2000000, 0x2c000,
    0xffff0182, 1, 0x2080000, 0, 0x2000000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_02001430[] = {
    1,
    (s32)0xffff0001,
    1,
    0x4602,
    (s32)0xffff0002,
    (s32)Func_02000e6c,
    0x8602,
    (s32)0xffff0002,
    (s32)Func_02000e44,
    0x602,
    (s32)0xffff0002,
    (s32)Func_02000e20,
    0x4602,
    (s32)0xffff0003,
    (s32)Func_02000e6c,
    0x8602,
    (s32)0xffff0003,
    (s32)Func_02000e44,
    0x602,
    (s32)0xffff0003,
    (s32)Func_02000e20,
    0x50008805,
    0x9af000a,
    (s32)Func_02000e88,
    -1,
    0,
    0,
};

s32 gData_0200149c[] = {
    1,
    (s32)0xffff0001,
    1,
    0x602,
    (s32)0xffff000a,
    (s32)Func_02000cdc,
    0x8515,
    0x2000008,
    0,
    0x8c15,
    (s32)0xffff000a,
    (s32)Func_02000bf8,
    -1,
    0,
    0,
};

s32 gData_020014d8[] = {
    1,
    (s32)0xffff0005,
    5,
    0x50008a05,
    (s32)0xffff003c,
    (s32)Func_02000ef4,
    -1,
    0,
    0,
};

s32 gData_020014fc[] = {
    1,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    33,
    (s32)0xffff0003,
    3,
    49,
    (s32)0xffff0004,
    4,
    33,
    (s32)0xffff0005,
    5,
    49,
    (s32)0xffff0006,
    6,
    33,
    (s32)0xffff0007,
    7,
    0xc602,
    (s32)0xffff0008,
    (s32)Func_02000fe4,
    1,
    (s32)0xffff0009,
    9,
    8,
    0x2120000,
    (s32)Func_020006a8,
    9,
    0x2120000,
    (s32)Func_0200087c,
    0x10008c15,
    0x2120008,
    (s32)Func_020006a8,
    0x8c15,
    0x2120008,
    (s32)Func_0200087c,
    0x10008c15,
    0x2120009,
    (s32)Func_020006a8,
    0x8c15,
    0x2120009,
    (s32)Func_0200087c,
    0x10009315,
    0x2120008,
    (s32)Func_020006a8,
    0x9315,
    0x2120008,
    (s32)Func_0200087c,
    0x10009315,
    0x2120009,
    (s32)Func_020006a8,
    0x9315,
    0x2120009,
    (s32)Func_0200087c,
    -1,
    0,
    0,
};
