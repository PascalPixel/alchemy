#include "types.h"
#include "field_scene.h"

void Func_02000038(void);
void Func_02000104(void);
void Func_02000118(void);
void Func_0200013c(void);
void Func_020002f4(void);
void Func_02000304(void);
void Func_02000314(void);
void Func_02000334(void);
void Func_020003c4(void);
void Func_02000428(void);
void Func_02000464(void);
void Func_020004c8(void);
void Func_0200065c(void);
void Func_02000744(void);
void Func_02000784(void);
void Func_020007bc(void);
void Func_02000920(void);
void Func_0200096c(void);
void Func_02000adc(void);
void Func_02000afc(void);
void Func_02000b58(void);
void Func_02000b90(void);
void Func_02000d2c(void);
void Func_02000f9c(void);
void Func_02000fbc(void);
void Func_02001018(void);
void Func_02001038(void);
void Func_020010bc(void);
void Func_020010c4(void);
void Func_02001100(void);
void Func_02001128(void);
void Func_02001138(void);
void Func_02001228(void);
void Func_02001564(void);
void Func_0200166c(void);
void Func_02002144(void);
void Func_020022ac(void);
void Func_02002334(void);

u32 gData_020029b8[] = {
    0x6090200,
    0x1050604,
};

u32 gData_020029c0[] = {
    0x10003,
    1,
};

// clang-format off
u32 gData_020029c8[] = {
    22, 26, 0, 39, 2, 0x80010000,
    23, 6, 0x10000, 0, 1, 13,
    30, 0xc0010000, 38,
};

u32 gData_02002a04[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02002a48[] = {
    0, 30, 38,
};

s32 gData_02002a54[] = {
    46,
    (s32)Func_02001228,
    38,
};

s32 gData_02002a60[] = {
    46,
    (s32)Func_02001564,
    38,
};

s32 gData_02002a6c[] = {
    46,
    (s32)Func_0200166c,
    38,
};

u16 gData_02002a78[] = {
    200, 200, 4, 264, 200, 4, 232, 216,
    4, 280, 216, 4, 568, 200, 4, 632,
    200, 4, 536, 216, 4, 600, 216, 4,
    408, 232, 4, 472, 232, 4, 0,
};

u16 gData_02002ab6[] = {
    9, 10, 0,
};

u16 gData_02002abc[] = {
    424, 104, 4, 536, 136, 4, 0,
};

u16 gData_02002aca[] = {
    408, 408, 4, 472, 360, 4, 0,
};

u16 gData_02002ad8[] = {
    216, 184, 4, 312, 136, 4, 0, 0,
};

u32 gData_02002ae8[] = {
    0, 32, 38,
};

u32 gData_02002af4[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(143),
    SCENE_EXIT(1, 2, 35),
    SCENE_EXIT(2, 144, 1),
    SCENE_EXIT(3, 144, 2),
    SCENE_EXITS(144),
    SCENE_EXIT(1, 143, 2),
    SCENE_EXIT(2, 143, 3),
    SCENE_EXIT(3, 145, 3),
    SCENE_EXIT(4, 148, 4),
    SCENE_EXIT(5, 145, 5),
    SCENE_EXITS(145),
    SCENE_EXIT(3, 144, 3),
    SCENE_EXIT(5, 144, 5),
    SCENE_EXIT(6, 147, 2),
    SCENE_EXIT(7, 147, 3),
    SCENE_EXIT(8, 145, 9),
    SCENE_EXIT(9, 145, 8),
    SCENE_EXITS(146),
    SCENE_EXIT(1, 147, 1),
    SCENE_EXIT(2, 147, 4),
    SCENE_EXITS(147),
    SCENE_EXIT(1, 146, 1),
    SCENE_EXIT(2, 145, 6),
    SCENE_EXIT(3, 145, 7),
    SCENE_EXIT(4, 146, 2),
    SCENE_EXIT(10, 146, 10),
    SCENE_EXIT(11, 146, 11),
    SCENE_EXIT(12, 146, 12),
    SCENE_EXIT(13, 146, 13),
    SCENE_EXIT(14, 146, 14),
    SCENE_EXIT(15, 146, 15),
    SCENE_EXIT(16, 146, 16),
    SCENE_EXITS(148),
    SCENE_EXIT(1, 149, 1),
    SCENE_EXIT(4, 144, 4),
    SCENE_EXITS(149),
    SCENE_EXIT(1, 148, 1),
    SCENE_EXIT(2, 150, 2),
    SCENE_EXIT(3, 150, 3),
    SCENE_EXIT(4, 149, 5),
    SCENE_EXIT(5, 149, 4),
    SCENE_EXIT(10, 150, 9),
    SCENE_EXIT(12, 150, 9),
    SCENE_EXITS(150),
    SCENE_EXIT(1, 152, 1),
    SCENE_EXIT(2, 149, 2),
    SCENE_EXIT(3, 149, 3),
    SCENE_EXIT(4, 150, 5),
    SCENE_EXIT(5, 150, 4),
    SCENE_EXITS(152),
    SCENE_EXIT(1, 150, 1),
    SCENE_EXIT(2, 153, 2),
    SCENE_EXIT(3, 151, 3),
    SCENE_EXIT(4, 151, 4),
    SCENE_EXITS(151),
    SCENE_EXIT(3, 152, 3),
    SCENE_EXIT(4, 152, 4),
    SCENE_EXITS(153),
    SCENE_EXIT(2, 152, 2),
    SCENE_EXIT(3, 153, 4),
    SCENE_EXIT(4, 153, 3),
    SCENE_EXIT(5, 153, 6),
    SCENE_EXIT(6, 153, 5),
    SCENE_EXITS_END,
};

u32 gData_02002c54[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002c6c[] = {
    0xffff0133, 1, 0xd80000, 0, 0x800000, 0x2c000,
    0xffff0133, 1, 0x1080000, 0, 0x700000, 0x2c000,
    0xffff0133, 1, 0x1280000, 0, 0x700000, 0x2c000,
    0xffff0133, 1, 0x2580000, 0, 0xb00000, 0x2c000,
    0xffff0133, 1, 0x2c80000, 0, 0x1000000, 0x2c000,
    0xffff012b, 1, 0x2f80000, 0, 0x1100000, 0x2c000,
    0x8e50111, 1, 0xd80000, 0, 0x1080000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002d2c[] = {
    0x3b00f3, 1, 0x1b80000, 0, 0x2780000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002d5c[] = {
    0x3b00f3, 1, 0x1980000, 0, 0x1380000, 0x4000,
    0xffff0110, 1, 0x1280000, 0, 0xe80000, 0x2c000,
    0xffff0110, 1, 0x1b80000, 0, 0xf80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002dbc[] = {
    0xffff0122, 1, 0x1a80000, 0, 0x880000, 0x2c000,
    0xffff0110, 1, 0xf80000, 0, 0x1a80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002e04[] = {
    0xffff0197, 1, 0x780000, 0, 0xc80000, 0x2c000,
    0xffff0197, 1, 0x980000, 0, 0xc80000, 0x2c000,
    0xffff0122, 1, 0x1980000, 0, 0x1b80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002e64[] = {
    0xffff01e9, 1, 0, 0, 0, 0x2c000,
    0xffff0124, 1, 0x880000, 0, 0xf80000, 0x2c000,
    0xffff0124, 1, 0, 0, 0, 0x2c000,
    0xffff0110, 1, 0x1f80000, 0, 0xf80000, 0x2c000,
    0xffff0110, 1, 0x2380000, 0, 0x1280000, 0x102c000,
    0xffff0110, 1, 0x1e80000, 0, 0x1380000, 0x102c000,
    0xffff0110, 1, 0x2280000, 0, 0x1680000, 0x102c000,
    0xffff0110, 1, 0x1c80000, 0, 0x1780000, 0x102c000,
    0xffff0148, 1, 0x1800000, 0, 0x1400000, 0x2c000,
    0xffff0148, 1, 0x1a00000, 0, 0x1500000, 0x102c000,
    0xffff0148, 1, 0x1700000, 0, 0x1600000, 0x102c000,
    0xffff0148, 1, 0x1f80000, 0, 0xf80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002f9c[] = {
    0xffff0124, 1, 0xb80000, 0, 0x1680000, 0x2c000,
    0xffff0124, 1, 0x2080000, 0, 0xd80000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02002fe4[] = {
    0xffff0124, 1, 0x780000, 0, 0x2780000, 0x2c000,
    0xffff0124, 1, 0x2080000, 0, 0x2780000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_0200302c[] = {
    -1, 0, 0,
};

s16 gData_02003038[] = {
    1, 0, 1, -1, 1, 0, 1, 0,
    2, -1, 2, 0, 1, 0, 3, -1,
    3, 0, -1, -1, 0, 0, 0, 0,
};

s32 gData_02003068[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    0xce01,
    0x18e00003,
    3,
    0xce01,
    0x18e30004,
    4,
    0xce01,
    0x18e40005,
    5,
    2,
    (s32)0xffff003c,
    (s32)Func_02002144,
    2,
    (s32)0xffff003d,
    (s32)Func_020022ac,
    2,
    (s32)0xffff003e,
    (s32)Func_02002334,
    0x4e15,
    (s32)0xffff0008,
    (s32)Func_02000104,
    0x4e15,
    (s32)0xffff0009,
    (s32)Func_02000104,
    0x4e15,
    (s32)0xffff000a,
    (s32)Func_02000104,
    0x4e15,
    (s32)0xffff000b,
    (s32)Func_02000104,
    0x4e15,
    (s32)0xffff000c,
    (s32)Func_02000104,
    0xc15,
    (s32)0xffff000d,
    (s32)Func_02000118,
    0x8c15,
    (s32)0xffff000e,
    (s32)Func_0200013c,
    -1,
    0,
    0,
};

s32 gData_02003128[] = {
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0005,
    5,
    49,
    (s32)0xffff0006,
    6,
    33,
    (s32)0xffff0007,
    7,
    49,
    (s32)0xffff0008,
    8,
    33,
    (s32)0xffff0009,
    9,
    0x50008805,
    0x3010033,
    (s32)Func_020002f4,
    0x50008805,
    0x3000032,
    (s32)Func_02000304,
    -1,
    0,
    0,
};

s32 gData_02003194[] = {
    33,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    0,
    (s32)0xffff0008,
    (s32)Func_02000038,
    0x50008905,
    (s32)0xffff001e,
    (s32)Func_02000314,
    0x50008905,
    (s32)0xffff001f,
    (s32)Func_02000334,
    0x50008905,
    (s32)0xffff0020,
    (s32)Func_020003c4,
    -1,
    0,
    0,
};

s32 gData_020031e8[] = {
    49,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    49,
    (s32)0xffff0003,
    3,
    49,
    (s32)0xffff0004,
    4,
    0x4602,
    (s32)0xffff000a,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff000b,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff000c,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff000d,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff000e,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff000f,
    (s32)Func_02000428,
    0x4602,
    (s32)0xffff0010,
    (s32)Func_02000428,
    2,
    (s32)0xffff000a,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000b,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000c,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000d,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000e,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000f,
    (s32)Func_020004c8,
    2,
    (s32)0xffff0010,
    (s32)Func_020004c8,
    2,
    (s32)0xffff0011,
    (s32)Func_020004c8,
    2,
    (s32)0xffff0012,
    (s32)Func_020004c8,
    0x8c15,
    0xa6d0009,
    (s32)Func_02000464,
    0x8c15,
    0xa6e000a,
    (s32)Func_02000464,
    9,
    -0x10000,
    (s32)Func_02000464,
    6,
    0x3100064,
    (s32)Func_0200065c,
    -1,
    0,
    0,
};

s32 gData_02003314[] = {
    49, (s32)0xffff0001, 1, 1, (s32)0xffff0004, 4,
    -1, 0, 0,
};
// clang-format on

s32 gData_02003338[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    49,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    0x4602,
    (s32)0xffff000a,
    (s32)Func_02000428,
    2,
    (s32)0xffff000a,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000c,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000b,
    (s32)Func_020004c8,
    0x8c15,
    0xa6f0009,
    (s32)Func_02000744,
    9,
    -0x10000,
    (s32)Func_02000744,
    0x50002115,
    0x2200008,
    (s32)Func_02000784,
    0x1815,
    0x12220008,
    (s32)Func_020007bc,
    -1,
    0,
    0,
};

s32 gData_020033e0[] = {
    49,
    (s32)0xffff0001,
    1,
    33,
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
    2,
    (s32)0xffff000a,
    (s32)Func_020004c8,
    0x8515,
    0x3080008,
    0,
    0x50002115,
    0x220000a,
    (s32)Func_02000920,
    0x1815,
    0x1222000a,
    (s32)Func_0200096c,
    -1,
    0,
    0,
};

u32 gData_02003458[] = {
    112,
};

u32 gData_0200345c[] = {
    120,
};

u32 gData_02003460[] = {
    24,
};

s32 gData_02003464[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    0x2115,
    0x3310009,
    (s32)Func_02000adc,
    0x2115,
    0x334000a,
    (s32)Func_02000b58,
    0x8c15,
    0x3320009,
    (s32)Func_02000afc,
    0x8c15,
    0x335000a,
    (s32)Func_02000b90,
    9,
    0x3350000,
    (s32)Func_02000b90,
    0x8715,
    0x330000b,
    (s32)Func_02000d2c,
    -1,
    0,
    0,
};

s32 gData_020034e8[] = {
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    2,
    (s32)0xffff000a,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000b,
    (s32)Func_020004c8,
#if !defined(TLA_EDITION_JA) /* Absent from the Japanese edition. */
    0x602,
    (s32)0xffff0040,
    (s32)Func_020010bc,
#endif
    0x2115,
    0x3380008,
    (s32)Func_02000f9c,
    0x2115,
    0x33b0009,
    (s32)Func_02001018,
    0x8c15,
    0x3390008,
    (s32)Func_02000fbc,
    0x8c15,
    0x33c0009,
    (s32)Func_02001038,
    -1,
    0,
    0,
};

s32 gData_02003560[] = {
    33,
    (s32)0xffff0002,
    2,
    1,
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
    83,
    0x3480032,
    (s32)Func_02001138,
    0x2115,
    0x3400008,
    (s32)Func_020010c4,
    0x2115,
    0x3420009,
    (s32)Func_02001100,
    0x8c15,
    0x3410008,
    0,
    0x8c15,
    0x3430009,
    0,
    0x50008805,
    0x3480032,
    (s32)Func_02001128,
    -1,
    0,
    0,
};

u32 gData_020035f0[] = {
    0,
};
