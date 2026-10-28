#include "types.h"
#include "scene_tables.h"

void Func_02000280(void);
void Func_02000548(void);
void Func_0200058c(void);
void Func_020005d0(void);
void Func_020007dc(void);
void Func_020008a8(void);
void Func_02000920(void);
void Func_0200095c(void);
void Func_02000964(void);
void Func_02000b34(void);
void Func_02000b3c(void);
void Func_02000bfc(void);
void Func_02000c64(void);
void Func_02000d2c(void);
void Func_02000de4(void);
void Func_02000e78(void);
void Func_02000f00(void);
void Func_02000f0c(void);
void Func_02000fac(void);
void Func_02000fb8(void);

extern u32 gData_020031b0[];
extern u32 gData_020031ec[];
extern u32 gData_02003228[];

// clang-format off
u32 gData_020031b0[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020031ec[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02003228[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s16 gData_02003264[] = {
    11, 12, 13, 14, -1,
};

s16 gData_0200326e[] = {
    8, -1,
};

s16 gData_02003272[] = {
    -1, 0, 0x8000, -1, 0xc000, 0xe000, 0xa000, 0xc000,
    0x4000, 0x2000, 0x6000, 0x4000, -1, 0, 0x8000, -1,
    -1, 0, 0x8000, -1, 0xc000, -1, -1, 0xc000,
    0x4000, -1, -1, 0x4000, -1, 0, 0x8000, -1,
};

s16 gData_020032b2[] = {
    -1, 0, 0x8000, -1, 0xc000, 0, 0x8000, 0xc000,
    0x4000, 0, 0x8000, 0x4000, -1, 0, 0x8000, -1,
    0,
};

u32 gData_020032d4[] = {
    0, 33, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02003304[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

s32 gData_02003334[] = {
    (s32)gData_020031b0,
    (s32)gData_020031ec,
    (s32)gData_02003228,
};

s16 gData_02003340[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, 11, 7, 515, -1,
};

s16 gData_0200335a[] = {
    8, 7, 512, 9, 7, 513, 10, 7,
    514, -1, 0,
};

u32 gData_02003370[] = {
    22, 3, 0, 0x80010000, 0, 1,
    23, 3, 0x1000, 13, 0xffff, 0x80010000,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_020033d0[] = {
    0x500330, 0x3400210, 0x2200060, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gData_020033f0[] = {
    0xf00b0, 0xc000c0, 0xd00021, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(206),
    SCENE_EXIT_IF(1, 2, 46, 0xffffffff),
    SCENE_EXIT_IF(2, 207, 1, 0xffffffff),
    SCENE_EXITS(207),
    SCENE_EXIT_IF(1, 206, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 207, 11, 0xffffffff),
    SCENE_EXIT_IF(3, 207, 7, 0xffffffff),
    SCENE_EXIT_IF(4, 207, 8, 0xffffffff),
    SCENE_EXIT_IF(5, 211, 5, 0xffffffff),
    SCENE_EXIT_IF(6, 207, 16, 0xffffffff),
    SCENE_EXIT_IF(7, 207, 3, 0xffffffff),
    SCENE_EXIT_IF(8, 207, 4, 0xffffffff),
    SCENE_EXIT_IF(9, 207, 18, 0xffffffff),
    SCENE_EXIT_IF(10, 208, 9, 0xffffffff),
    SCENE_EXIT_IF(11, 207, 2, 0xffffffff),
    SCENE_EXIT_IF(12, 211, 4, 0xffffffff),
    SCENE_EXIT_IF(13, 207, 15, 0xffffffff),
    SCENE_EXIT_IF(14, 207, 17, 0xffffffff),
    SCENE_EXIT_IF(15, 207, 13, 0xffffffff),
    SCENE_EXIT_IF(16, 207, 6, 0xffffffff),
    SCENE_EXIT_IF(17, 207, 14, 0xffffffff),
    SCENE_EXIT_IF(18, 207, 9, 0xffffffff),
    SCENE_EXITS(208),
    SCENE_EXIT_IF(1, 208, 3, 0xffffffff),
    SCENE_EXIT_IF(2, 208, 7, 0xffffffff),
    SCENE_EXIT_IF(3, 208, 1, 0xffffffff),
    SCENE_EXIT_IF(4, 208, 10, 0xffffffff),
    SCENE_EXIT_IF(5, 209, 2, 0xffffffff),
    SCENE_EXIT_IF(6, 208, 11, 0xffffffff),
    SCENE_EXIT_IF(7, 208, 2, 0xffffffff),
    SCENE_EXIT_IF(8, 208, 12, 0xffffffff),
    SCENE_EXIT_IF(9, 207, 10, 0xffffffff),
    SCENE_EXIT_IF(10, 208, 4, 0xffffffff),
    SCENE_EXIT_IF(11, 208, 6, 0xffffffff),
    SCENE_EXIT_IF(12, 208, 8, 0xffffffff),
    SCENE_EXITS(209),
    SCENE_EXIT_IF(1, 210, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 208, 5, 0xffffffff),
    SCENE_EXIT_IF(3, 210, 6, 0xffffffff),
    SCENE_EXIT_IF(4, 210, 5, 0xffffffff),
    SCENE_EXIT_IF(5, 209, 6, 0xffffffff),
    SCENE_EXIT_IF(6, 209, 5, 0xffffffff),
    SCENE_EXITS(210),
    SCENE_EXIT_IF(1, 210, 3, 0xffffffff),
    SCENE_EXIT_IF(2, 209, 1, 0xffffffff),
    SCENE_EXIT_IF(3, 210, 1, 0xffffffff),
    SCENE_EXIT_IF(4, 210, 8, 0xffffffff),
    SCENE_EXIT_IF(5, 209, 4, 0xffffffff),
    SCENE_EXIT_IF(6, 209, 3, 0xffffffff),
    SCENE_EXIT_IF(7, 211, 1, 0xffffffff),
    SCENE_EXIT_IF(8, 210, 4, 0xffffffff),
    SCENE_EXIT_IF(9, 211, 2, 0xffffffff),
    SCENE_EXIT_IF(10, 211, 9, 0xffffffff),
    SCENE_EXIT_IF(11, 209, 7, 0xffffffff),
    SCENE_EXITS(211),
    SCENE_EXIT_IF(1, 210, 7, 0xffffffff),
    SCENE_EXIT_IF(2, 210, 9, 0xffffffff),
    SCENE_EXIT_IF(3, 212, 2, 0xffffffff),
    SCENE_EXIT_IF(4, 207, 12, 0xffffffff),
    SCENE_EXIT_IF(5, 207, 5, 0xffffffff),
    SCENE_EXIT_IF(6, 212, 1, 0xffffffff),
    SCENE_EXIT_IF(7, 211, 8, 0xffffffff),
    SCENE_EXIT_IF(8, 211, 7, 0xffffffff),
    SCENE_EXIT_IF(9, 210, 10, 0xffffffff),
    SCENE_EXIT_IF(10, 211, 10, 0xffffffff),
    SCENE_EXITS(212),
    SCENE_EXIT_IF(1, 211, 6, 0xffffffff),
    SCENE_EXIT_IF(2, 211, 3, 0xffffffff),
    SCENE_EXITS_END,
};

u32 gData_02003618[] = {
    0xffff01e9, 1, 1, 0, 0, 0x2020000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003648[] = {
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003660[] = {
    0x6400f5, 7, 0x1a80000, 0, 0x2180000, 0x8000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003690[] = {
    0xffff0196, 1, 1, 0, 0, 0x2024000,
    0xffff0196, 1, 2, 0, 0, 0x2024000,
    0xffff0196, 1, 3, 0, 0, 0x2024000,
    0xffff0196, 1, 4, 0, 0, 0x2024000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02003708[] = {
    0xffff0196, 1, 1, 0, 0, 0x2024000,
    0xffff0196, 1, 2, 0, 0, 0x2024000,
    0xffff0196, 1, 3, 0, 0, 0x2024000,
    0xffff0110, 1, 4, 0, 0, 0x2024000,
    0xffff0110, 1, 5, 0, 0, 0x2024000,
    0xffff0110, 1, 6, 0, 0, 0x2024000,
    0xffff0110, 1, 7, 0, 0, 0x2024000,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020037c8[] = {
    0xffff0110, 1, 2, 0, 0, 0x2024000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020037f8[] = {
    1, (s32)0xffff0001, 1, 1, (s32)0xffff0002, 2,
    -1, 0, 0,
};
// clang-format on

s32 gData_0200381c[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    0xc602,
    (s32)0xffff0003,
    (s32)Func_020005d0,
    0xc602,
    (s32)0xffff0004,
    (s32)Func_020005d0,
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
    1,
    (s32)0xffff0009,
    9,
    33,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    1,
    (s32)0xffff000d,
    13,
    1,
    (s32)0xffff000e,
    14,
    1,
    (s32)0xffff000f,
    15,
    1,
    (s32)0xffff0010,
    16,
    1,
    (s32)0xffff0011,
    17,
    1,
    (s32)0xffff0012,
    18,
    3,
    (s32)0xffff0034,
    (s32)Func_02000920,
    0x50008a05,
    0x9420032,
    (s32)Func_02000548,
    0x50008a05,
    0x9430033,
    (s32)Func_0200058c,
    0x50009705,
    0x9440034,
    (s32)Func_020007dc,
    0x9705,
    0x9440034,
    (s32)Func_020008a8,
    -1,
    0,
    0,
};

s32 gData_0200393c[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    33,
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
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff000a,
    10,
    1,
    (s32)0xffff000b,
    11,
    1,
    (s32)0xffff000c,
    12,
    2,
    (s32)0xffff0032,
    (s32)Func_0200095c,
    -1,
    0,
    0,
};

s32 gData_020039e4[] = {
    33,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    33,
    (s32)0xffff0003,
    3,
    33,
    (s32)0xffff0004,
    4,
    1,
    (s32)0xffff0005,
    5,
    1,
    (s32)0xffff0006,
    6,
    2,
    (s32)0xffff0032,
    (s32)Func_0200095c,
    2,
    0x2000033,
    (s32)Func_02000964,
    0,
    (s32)0xffff0008,
    (s32)Func_02000280,
    -1,
    0,
    0,
};

s32 gData_02003a5c[] = {
    1,
    (s32)0xffff0001,
    1,
    49,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    33,
    (s32)0xffff0004,
    4,
    49,
    (s32)0xffff0005,
    5,
    49,
    (s32)0xffff0006,
    6,
    1,
    (s32)0xffff0007,
    7,
    49,
    (s32)0xffff0008,
    8,
    1,
    (s32)0xffff0009,
    9,
    33,
    (s32)0xffff000a,
    10,
    2,
    (s32)0xffff000b,
    (s32)Func_02000b3c,
    2,
    (s32)0xffff0032,
    (s32)Func_0200095c,
    0x50008615,
    0x2000008,
    (s32)Func_02000b34,
    0x50008615,
    0x2010009,
    (s32)Func_02000b34,
    0x50008615,
    0x202000a,
    (s32)Func_02000b34,
    0x50008615,
    0x203000b,
    (s32)Func_02000b34,
    -1,
    0,
    0,
};

s32 gData_02003b28[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
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
    (s32)0xffff0007,
    7,
    1,
    (s32)0xffff0008,
    8,
    49,
    (s32)0xffff0009,
    9,
    1,
    (s32)0xffff001e,
    10,
    2,
    (s32)0xffff001f,
    (s32)Func_02000f00,
    0x50008615,
    0x2000008,
    (s32)Func_02000b34,
    0x50008615,
    0x2010009,
    (s32)Func_02000b34,
    0x50008615,
    0x202000a,
    (s32)Func_02000b34,
    8,
    -0x10000,
    (s32)Func_02000bfc,
    9,
    -0x10000,
    (s32)Func_02000d2c,
    (s32)0x80008c15,
    (s32)0xffff000b,
    (s32)Func_02000fac,
    0x10008c15,
    (s32)0xffff000b,
    (s32)Func_02000bfc,
    0x50008c15,
    (s32)0xffff000b,
    (s32)Func_02000c64,
    0x10008c15,
    0x946000c,
    (s32)Func_02000bfc,
    0x8c15,
    0x946000c,
    (s32)Func_02000d2c,
    0x10008c15,
    0x947000d,
    (s32)Func_02000bfc,
    0x8c15,
    0x947000d,
    (s32)Func_02000d2c,
    (s32)0x80008c15,
    (s32)0xffff000e,
    (s32)Func_02000fac,
    0x10008c15,
    (s32)0xffff000e,
    (s32)Func_02000bfc,
    0x50008c15,
    (s32)0xffff000e,
    (s32)Func_02000c64,
    0x50009085,
    -0x10000,
    (s32)Func_02000e78,
    0x40009085,
    -0x10000,
    (s32)Func_02000de4,
    (s32)0x80009705,
    (s32)0xffff0034,
    (s32)Func_02000fac,
    0x50009705,
    0x94b0034,
    (s32)Func_02000f0c,
    -1,
    0,
    0,
};

s32 gData_02003c90[] = {
    1,
    (s32)0xffff0002,
    2,
    0x10008c15,
    (s32)0xffff0008,
    (s32)Func_02000bfc,
    0x8c15,
    (s32)0xffff0008,
    (s32)Func_02000d2c,
    0x50009705,
    0x9480032,
    (s32)Func_02000fb8,
    -1,
    0,
    0,
};
