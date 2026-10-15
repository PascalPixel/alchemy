#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000038(void);
void Func_0200012c(void);
void Func_02000178(void);
void Func_02000198(void);
void Func_02000bf4(void);
void Func_02000c24(void);
void Func_02000c54(void);
void Func_02000c64(void);
void Func_02000c74(void);
void Func_02000c84(void);
void Func_02000d00(void);
void Func_02000d40(void);
void Func_02000d4c(void);
void Func_02000da4(void);
void Func_02000e0c(void);
void Func_02000e24(void);
void Func_02000e3c(void);
void Func_02000e54(void);
void Func_02000eec(void);
void Func_02000ffc(void);
void Func_0200102c(void);
void Func_0200105c(void);

extern u32 gData_020016b8[];
extern s16 gData_02001ef4[];

// clang-format off
u32 gData_02001470[] = {
    39, 2, 48, 0x21b0000, 0x1470000, 1,
    48, 0x2470000, 0x1470000, 1, 48, 0x2470000,
    0x12c0000, 1, 3, 0, 150, 22,
    26, 1, 2, 0x2460000, 0, 0x10a0000,
    4, 0x2460000, 0, 0xf80000, 1, 48,
    0x23a0000, 0xf80000, 1, 48, 0x23a0000, 0x1160000,
    1, 39, 1, 22, 3, 0x6000,
    17,
};

s32 gData_0200151c[] = {
    0, 50, 39, 2, 5, 0x40000,
    0, -0x280000, 1, 39, 1, 17,
};

s32 gData_0200154c[] = {
    0, 70, 39, 2, 5, 0,
    0, -0x280000, 1, 48, 0x2000000, 0x12f0000,
    1, 39, 1, 17,
};

u32 gData_0200158c[] = {
    39, 2, 4, 0x23a0000, 0, 0xf80000,
    1, 4, 0x2470000, 0, 0xf80000, 1,
    4, 0x2460000, 0, 0x10a0000, 1, 3,
    0, 20, 17,
};

u32 gData_020015e0[] = {
    2, 0x2470000, 0, 0x1470000, 22, 26,
    0, 2, 0x2470000, 0, 0x12c0000, 48,
    0x2470000, 0x1470000, 1, 48, 0x22a0000, 0x14c0000,
    1, 39, 1, 17,
};

u32 gData_02001638[] = {
    39, 2, 48, 0x1d80000, 0x14b0000, 1,
    48, 0x1d80000, 0x1690000, 1, 39, 1,
    17,
};

s32 gData_0200166c[] = {
    5, 0, 0x20000, 0, 1, 17,
    39, 5, 0, 1, 17, 0,
    5, 39, 5, 22, 9, -0x10000,
    17,
};

u32 gData_020016b8[] = {
    0x80010000, 0, 50, 22, 3, 0x8000,
    0, 50, 22, 3, 0xc000, 0,
    50, 22, 3, 0, 0, 50,
    22, 3, 0xc000, 13, 0xffff, 0xc0010000,
    17,
};

u32 gData_0200171c[] = {
    39, 2, 4, 0x800000, 0, 0x780000,
    1, 39, 1, 0x80010000, 0, 50,
    22, 3, 0x8000, 0, 50, 22,
    3, 0xc000, 0, 50, 22, 3,
    0, 0, 50, 22, 3, 0xc000,
    13, 0xffff, 0xc0010000, 17,
};

u32 gData_020017a4[] = {
    22, 15, 0xe666, 39, 2, 4,
    0x800000, 0, 0x900000, 1, 39, 1,
    17,
};

u32 gData_020017d8[] = {
    22, 15, 0xe666, 39, 2, 4,
    0x9f0000, 0, 0x800000, 1, 39, 1,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200183c[] = {
    0x1c01c4, 0x1cc02e4, 0x2ec0024, 0x6ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(266),
    SCENE_EXIT_IF(1, 267, 1, 0xffffffff),
    SCENE_EXIT_IF(2, 267, 2, 0xffffffff),
    SCENE_EXIT_IF(3, 267, 3, 0xffffffff),
    SCENE_EXIT_IF(4, 267, 4, 0xffffffff),
    SCENE_EXIT_IF(5, 267, 5, 0xffffffff),
    SCENE_EXIT_IF(6, 267, 6, 0xffffffff),
    SCENE_EXIT_IF(8, 267, 6, 0xffffffff),
    SCENE_EXIT_IF(9, 267, 6, 0xffffffff),
    SCENE_EXIT_IF(7, 2, 59, 0xffffffff),
    SCENE_EXITS(267),
    SCENE_EXIT_IF(1, 266, 1, 0xffffffff),
    SCENE_EXIT_IF(2, 266, 2, 0xffffffff),
    SCENE_EXIT_IF(3, 266, 3, 0xffffffff),
    SCENE_EXIT_IF(4, 266, 4, 0xffffffff),
    SCENE_EXIT_IF(5, 266, 5, 0xffffffff),
    SCENE_EXIT_IF(6, 266, 6, 0xffffffff),
    SCENE_EXIT_IF(7, 267, 8, 0xffffffff),
    SCENE_EXIT_IF(8, 267, 7, 0xffffffff),
    SCENE_EXIT_IF(9, 267, 10, 0xffffffff),
    SCENE_EXIT_IF(10, 267, 9, 0xffffffff),
    SCENE_EXITS_END,
};

u32 gData_02001900[] = {
    0xffff00dd, 1, 0x380000, 0, 0x1300000, 0x16000,
    0xffff00dd, 1, 0x480000, 0, 0x1880000, 0x12000,
    0xffff00dd, 1, 0xd80000, 0, 0x1080000, 0x18000,
    0xffff009a, 1, 0x1400000, 0, 0x1400000, 0x1c000,
    0xffff00dd, 1, 0xb00000, 0, 0x1a00000, 0x1a000,
    0xffff00dd, 2, 0x580000, 0, 0x2380000, 0x1c000,
    0xffff00dd, 1, 0xd80000, 0, 0x2200000, 0x10000,
    0xffff009a, 1, 0x880000, 0, 0x2200000, 0x1c000,
    0xffff00dd, 1, 0x2080000, 0, 0x1500000, 0x1a000,
    0xffff009a, 1, 0x1c80000, 0, 0x1500000, 0x10000,
    0xffff00dd, 1, 0x2100000, 0, 0x1680000, 0x18000,
    0x7c00f6, 1, 0xe80000, 0, 0xf80000, 0x4000,
    0xffff01a5, 1, 0x1e80000, 0, 0x1680000, 0x1c000,
    0xffff01e9, 1, 0, 0, 0, 0x2c000,
    0xffff0149, 1, 0x2080000, 0, 0x880000, 0x2c000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02001a80[] = {
    (s32)0xffff0082,
    (s32)gData_020016b8,
    0x800000,
    0,
    0x780000,
    0x1a000,
    (s32)0xffff01e9,
    1,
    0xa80000,
    0,
    0x780000,
    0x14000,
    (s32)0xffff00ca,
    1,
    0x1c00000,
    0,
    0x1980000,
    0x14000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02001ae0[] = {
    -1, 0, 0,
};

s32 gData_02001aec[] = {
    0xc602,
    (s32)0xffff0001,
    (s32)Func_02000eec,
    0xc602,
    (s32)0xffff0002,
    (s32)Func_02000eec,
    0xc602,
    (s32)0xffff0003,
    (s32)Func_02000eec,
    0xc602,
    (s32)0xffff0004,
    (s32)Func_02000eec,
    0xc602,
    (s32)0xffff0005,
    (s32)Func_02000e54,
    0x4602,
    (s32)0xffff0006,
    (s32)Func_0200105c,
    0x8602,
    (s32)0xffff0008,
    (s32)Func_0200102c,
    0x602,
    (s32)0xffff0009,
    (s32)Func_02000ffc,
    1,
    (s32)0xffff0007,
    7,
    0,
    0x1a5f0008,
    MSGNO(MsgLoho29),
    0,
    0x1a5f0009,
    MSGNO(MsgLoho30),
    0,
    0x1a5f000a,
    MSGNO(MsgLoho31),
    0,
    0x1a5f000b,
    (s32)Func_02000bf4,
    0x8d15,
    0x1a5f0008,
    MSGNO(MsgLoho33),
    0x8d15,
    0x1a5f0009,
    MSGNO(MsgLoho34),
    0x8d15,
    0x1a5f000a,
    MSGNO(MsgLoho35),
    0x8d15,
    0x1a5f000b,
    (s32)Func_02000c24,
    0,
    0x1a5f000c,
    MSGNO(MsgLoho37),
    0,
    0x1a5f000d,
    MSGNO(MsgLoho38),
    0,
    0x1a5f000e,
    MSGNO(MsgLoho39),
    0,
    0x1a5f000f,
    MSGNO(MsgLoho40),
    0,
    0x1a5f0010,
    MSGNO(MsgLoho41),
    0,
    0x1a5f0012,
    MSGNO(MsgLoho42),
    0,
    0x1a5f0011,
    MSGNO(MsgLoho43),
    0x8d15,
    0x1a5f000c,
    MSGNO(MsgLoho44),
    0x8d15,
    0x1a5f000d,
    MSGNO(MsgLoho45),
    0x8d15,
    0x1a5f000e,
    MSGNO(MsgLoho46),
    0x8d15,
    0x1a5f000f,
    MSGNO(MsgLoho47),
    0x8d15,
    0x1a5f0010,
    MSGNO(MsgLoho48),
    0x8d15,
    0x1a5f0012,
    MSGNO(MsgLoho49),
    0x8d15,
    0x1a5f0011,
    MSGNO(MsgLoho50),
    0,
    (s32)0xffff0008,
    MSGNO(MsgLoho01),
    0,
    (s32)0xffff0009,
    MSGNO(MsgLoho02),
    0,
    (s32)0xffff000a,
    MSGNO(MsgLoho03),
    0,
    (s32)0xffff000b,
    MSGNO(MsgLoho04),
    0,
    (s32)0xffff000c,
    MSGNO(MsgLoho09),
    0,
    (s32)0xffff000d,
    MSGNO(MsgLoho10),
    0,
    (s32)0xffff000e,
    MSGNO(MsgLoho11),
    0,
    (s32)0xffff000f,
    MSGNO(MsgLoho12),
    0,
    (s32)0xffff0010,
    MSGNO(MsgLoho21),
    0,
    (s32)0xffff0012,
    MSGNO(MsgLoho22),
    0,
    (s32)0xffff0011,
    (s32)Func_0200012c,
    0,
    (s32)0xffff0014,
    (s32)Func_02000178,
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgLoho05),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgLoho06),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgLoho07),
    0x8d15,
    (s32)0xffff000b,
    MSGNO(MsgLoho08),
    0x8d15,
    (s32)0xffff000c,
    MSGNO(MsgLoho13),
    0x8d15,
    (s32)0xffff000d,
    MSGNO(MsgLoho14),
    0x8d15,
    (s32)0xffff000e,
    MSGNO(MsgLoho15),
    0x8d15,
    (s32)0xffff000f,
    MSGNO(MsgLoho16),
    0x8d15,
    (s32)0xffff0010,
    MSGNO(MsgLoho24),
    0x8d15,
    (s32)0xffff0012,
    MSGNO(MsgLoho25),
    0x8d15,
    (s32)0xffff0011,
    MSGNO(MsgLoho26),
    499,
    (s32)0xffff00c8,
    0x00400000 | MSGNO(MsgLoho58),
    0x1cc14,
    (s32)0xffff0014,
    (s32)Func_02000198,
    0,
    (s32)0xffff0013,
    (s32)Func_02000038,
    0x50008805,
    0x3000065,
    (s32)Func_02000c54,
    0x50008805,
    0x3010066,
    (s32)Func_02000c64,
    0x50008805,
    0x3020067,
    (s32)Func_02000c74,
    0x8f15,
    0x2010016,
    (s32)Func_02000c84,
    -1,
    0,
    0,
};

s32 gData_02001dd4[] = {
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
    1,
    (s32)0xffff000a,
    10,
    0xc402,
    0x3200014,
    (s32)Func_02000e24,
    0x8402,
    0x3200016,
    (s32)Func_02000e0c,
    2,
    0x13200015,
    (s32)Func_02000e3c,
    2,
    0x13200017,
    (s32)Func_02000e3c,
    0,
    (s32)0xffff0008,
    (s32)Func_02000d4c,
    0x8d15,
    0xa5f0008,
    MSGNO(MsgLoho18),
    0x8d15,
    0x1a5f0008,
    MSGNO(MsgLoho52),
    0,
    0x13200009,
    (s32)Func_02000d40,
    0,
    (s32)0xffff000a,
    (s32)Func_02000da4,
    0x8d15,
    0xa5f000a,
    MSGNO(MsgLoho20),
    0x8d15,
    0x1a5f000a,
    MSGNO(MsgLoho54),
    3,
    (s32)0xffff0014,
    (s32)Func_02000d40,
    3,
    (s32)0xffff001e,
    (s32)Func_02000d00,
    -1,
    0,
    0,
};

s16 gData_02001ef4[] = {
    64, 50, 2, 2, 2, 64, 52, 2,
    2, 2, 64, 54, 2, 2, 2, -1,
};
// clang-format on

s32 gData_02001f14[] = {
    (s32)gData_02001ef4,
    0xf004f,
};
