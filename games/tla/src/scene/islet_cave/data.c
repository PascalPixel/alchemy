#include "types.h"
#include "field_scene.h"

void Func_020003b0(void);
void Func_020003bc(void);
void Func_020003c8(void);
void Func_020003d4(void);
void Func_02000508(void);
void Func_02000544(void);
void Func_020005fc(void);
void Func_020009a4(void);
void Func_020009b0(void);
void Func_020009bc(void);
void Func_020009c8(void);
void Func_020009d4(void);
void Func_020009e0(void);
void Func_020009ec(void);
void Func_02000aec(void);
void Func_02000b4c(void);
void Func_02000bb0(void);
void Func_02000c1c(void);
void Func_02000c8c(void);
void Func_02000cf0(void);
void Func_02000d54(void);
void Func_02000db8(void);
void Func_02000e1c(void);
void Func_02000e7c(void);
void Func_02000ee0(void);
void Func_02000f44(void);
void Func_02000fa8(void);
void Func_02001034(void);
void Func_020010ac(void);
void Func_0200115c(void);

extern u32 gData_020013fc[];
extern s32 gData_02001418[];
extern s16 gData_020019d4[];

// clang-format off
s16 gData_0200134c[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

s16 gData_0200135c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02001388[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gData_020013b4[] = {
    14, 9, 10, 11, 12, 13, 13, 14,
    9, 10, 11, 12, 12, 13, 14, 9,
    10, 11, 11, 12, 13, 14, 9, 10,
    10, 11, 12, 13, 14, 9, 9, 10,
    11, 12, 13, 14,
};

u32 gData_020013fc[] = {
    22, 10, 0x14ccc, 22, 9, 0x14ccc,
    17,
};

s32 gData_02001418[] = {
    22,
    26,
    0,
    23,
    6,
    -0x40000,
    22,
    9,
    0xcccc,
    22,
    10,
    0xcccc,
    46,
    (s32)Func_020003c8,
    17,
};

s32 gData_02001454[] = {
    46,
    (s32)Func_020003d4,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(136),
    SCENE_EXIT_IF(1, 135, 5, 0xffffffff),
    SCENE_EXIT_IF(2, 136, 3, 0xffffffff),
    SCENE_EXIT_IF(3, 136, 2, 0xffffffff),
    SCENE_EXIT_IF(4, 136, 5, 0xffffffff),
    SCENE_EXIT_IF(5, 136, 4, 0xffffffff),
    SCENE_EXIT_IF(6, 136, 7, 0xffffffff),
    SCENE_EXIT_IF(7, 136, 6, 0xffffffff),
    SCENE_EXIT_IF(8, 137, 1, 0xffffffff),
    SCENE_EXIT_IF(9, 136, 10, 0xffffffff),
    SCENE_EXIT_IF(10, 136, 9, 0xffffffff),
    SCENE_EXITS(137),
    SCENE_EXIT_IF(1, 136, 8, 0xffffffff),
    SCENE_EXIT_IF(2, 137, 3, 0xffffffff),
    SCENE_EXIT_IF(3, 137, 2, 0xffffffff),
    SCENE_EXIT_IF(4, 137, 5, 0xffffffff),
    SCENE_EXIT_IF(5, 137, 4, 0xffffffff),
    SCENE_EXIT_IF(6, 137, 7, 0xffffffff),
    SCENE_EXIT_IF(7, 137, 6, 0xffffffff),
    SCENE_EXIT_IF(8, 137, 9, 0xffffffff),
    SCENE_EXIT_IF(9, 137, 8, 0xffffffff),
    SCENE_EXIT_IF(10, 138, 1, 0xffffffff),
    SCENE_EXITS(138),
    SCENE_EXIT_IF(1, 137, 10, 0xffffffff),
    SCENE_EXITS_END,
};

s32 gData_02001548[] = {
    0x9cf00aa,
    (s32)gData_020013fc,
    0xb80000,
    0,
    0xb80000,
    0x24000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0x980000,
    0,
    0x900000,
    0x2c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0xd80000,
    0,
    0x900000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0x880000,
    0,
    0xb00000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0xe80000,
    0,
    0xb00000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0x980000,
    0,
    0xe00000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0xd80000,
    0,
    0xe00000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0x880000,
    0,
    0x1000000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0xe80000,
    0,
    0x1000000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0x880000,
    0,
    0x1300000,
    0x102c000,
    (s32)0xffff0132,
    (s32)gData_02001418,
    0xe80000,
    0,
    0x1300000,
    0x102c000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02001668[] = {
    0xffff012a, 7, 0xa80000, 0, 0x1f80000, 0x24000,
    0xffff012a, 7, 0x1580000, 0, 0xe80000, 0x24000,
    0xffff012a, 7, 0x3080000, 0, 0x980000, 0x24000,
    0xffff012a, 7, 0x3680000, 0, 0x980000, 0x24000,
    0xffff012a, 7, 0x3080000, 0, 0x1780000, 0x24000,
    0xffff012a, 7, 0x3680000, 0, 0x1780000, 0x24000,
    0xffff0129, 7, 0x3380000, 0, 0x1d80000, 0x24000,
    0x3c00f3, 1, 0x3380000, 0, 0x580000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001740[] = {
    0xffff00fc, 1, 0x1280000, 0, 0xb80000, 0x24000,
    0x5500f4, 1, 0x1500000, 0, 0xb80000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02001788[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    33,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    0x602,
    (s32)0xffff0014,
    (s32)Func_02000aec,
    0x8602,
    (s32)0xffff0014,
    (s32)Func_02000b4c,
    0x602,
    (s32)0xffff0015,
    (s32)Func_02000bb0,
    0x8602,
    (s32)0xffff0015,
    (s32)Func_02000c1c,
    0x602,
    (s32)0xffff0016,
    (s32)Func_02000c8c,
    0x8602,
    (s32)0xffff0016,
    (s32)Func_02000cf0,
    0x602,
    (s32)0xffff0017,
    (s32)Func_02000d54,
    0x8602,
    (s32)0xffff0017,
    (s32)Func_02000db8,
    0x602,
    (s32)0xffff0018,
    (s32)Func_02000e1c,
    0x8602,
    (s32)0xffff0018,
    (s32)Func_02000e7c,
    0x602,
    (s32)0xffff0019,
    (s32)Func_02000ee0,
    0x8602,
    (s32)0xffff0019,
    (s32)Func_02000f44,
    0xc602,
    (s32)0xffff001a,
    (s32)Func_02000fa8,
    0x4602,
    (s32)0xffff001a,
    (s32)Func_02001034,
    2,
    (s32)0xffff0014,
    (s32)Func_020009a4,
    2,
    (s32)0xffff0015,
    (s32)Func_020009b0,
    2,
    (s32)0xffff0016,
    (s32)Func_020009bc,
    2,
    (s32)0xffff0017,
    (s32)Func_020009c8,
    2,
    (s32)0xffff0018,
    (s32)Func_020009d4,
    2,
    (s32)0xffff0019,
    (s32)Func_020009e0,
    2,
    (s32)0xffff001a,
    (s32)Func_020009ec,
    0,
    (s32)0xffff000f,
    (s32)Func_020003b0,
    0x9c05,
    (s32)0xffff0032,
    (s32)Func_02000508,
    0x9c05,
    (s32)0xffff0033,
    (s32)Func_02000544,
    -1,
    0,
    0,
};

s32 gData_02001914[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0009,
    9,
    0xc602,
    (s32)0xffff0002,
    (s32)Func_020005fc,
    0xc602,
    (s32)0xffff0004,
    (s32)Func_020005fc,
    0xc602,
    (s32)0xffff0006,
    (s32)Func_020005fc,
    0xc602,
    (s32)0xffff0008,
    (s32)Func_020005fc,
    0xc602,
    (s32)0xffff000a,
    (s32)Func_020005fc,
    0x8715,
    0x2300008,
    (s32)Func_0200115c,
    0,
    (s32)0xffff0009,
    (s32)Func_020003bc,
    -1,
    0,
    0,
};

s32 gData_020019b0[] = {
    1,
    (s32)0xffff0001,
    1,
    2,
    0x9cf000a,
    (s32)Func_020010ac,
    -1,
    0,
    0,
};

s16 gData_020019d4[] = {
    72, 4, 2, 2, 2, 73, 4, 2,
    2, 2, -1, 0,
};
// clang-format on

s32 gData_020019ec[] = {
    (s32)gData_020019d4, 0x40048, (s32)gData_020019d4, 0x40054,
    (s32)gData_020019d4, 0x40060, (s32)gData_020019d4, 0x4006b,
    (s32)gData_020019d4, 0x40076,
};
