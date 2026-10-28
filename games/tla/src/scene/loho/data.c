#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

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
    SC_WAIT(50),
    SC_ARG(2),
    SC_MOVE_BY(PIXELS(4), 0, PIXELS(-40)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gData_0200154c[] = {
    SC_WAIT(70),
    SC_ARG(2),
    SC_MOVE_BY(0, 0, PIXELS(-40)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(512), PIXELS(303)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200158c[] = {
    39, 2, 4, 0x23a0000, 0, 0xf80000,
    1, 4, 0x2470000, 0, 0xf80000, 1,
    4, 0x2460000, 0, 0x10a0000, 1, 3,
    0, 20, 17,
};

u32 gData_020015e0[] = {
    SC_POSITION(PIXELS(583), 0, PIXELS(327)),
    SC_SET(SW_MOTION, 0),
    SC_POSITION(PIXELS(583), 0, PIXELS(300)),
    SC_MOVE_XZ(PIXELS(583), PIXELS(327)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(554), PIXELS(332)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02001638[] = {
    SC_ARG(2),
    SC_MOVE_XZ(PIXELS(472), PIXELS(331)),
    SC_SYNC,
    SC_MOVE_XZ(PIXELS(472), PIXELS(361)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

s32 gData_0200166c[] = {
    SC_MOVE_BY(0, PIXELS(2), 0),
    SC_SYNC,
    SC_END,
    SC_ARG(5),
    SC_WAIT(1),
    SC_END,
    SC_WAIT(5),
    SC_ARG(5),
    SC_SET(SW_SCALE_X, -0x10000),
    SC_END,
};

u32 gData_020016b8[] = {
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gData_0200171c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(120)),
    SC_SYNC,
    SC_ARG(1),
    SC_LABEL(1),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(50),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_END,
};

u32 gData_020017a4[] = {
    SC_SET(SW_SPEED, 0xe666),
    SC_ARG(2),
    SC_MOVE(PIXELS(128), 0, PIXELS(144)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_020017d8[] = {
    SC_SET(SW_SPEED, 0xe666),
    SC_ARG(2),
    SC_MOVE(PIXELS(159), 0, PIXELS(128)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
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
    SCENE_EXIT_IF(1, 267, 1, -1),
    SCENE_EXIT_IF(2, 267, 2, -1),
    SCENE_EXIT_IF(3, 267, 3, -1),
    SCENE_EXIT_IF(4, 267, 4, -1),
    SCENE_EXIT_IF(5, 267, 5, -1),
    SCENE_EXIT_IF(6, 267, 6, -1),
    SCENE_EXIT_IF(8, 267, 6, -1),
    SCENE_EXIT_IF(9, 267, 6, -1),
    SCENE_EXIT_IF(7, 2, 59, -1),
    SCENE_EXITS(267),
    SCENE_EXIT_IF(1, 266, 1, -1),
    SCENE_EXIT_IF(2, 266, 2, -1),
    SCENE_EXIT_IF(3, 266, 3, -1),
    SCENE_EXIT_IF(4, 266, 4, -1),
    SCENE_EXIT_IF(5, 266, 5, -1),
    SCENE_EXIT_IF(6, 266, 6, -1),
    SCENE_EXIT_IF(7, 267, 8, -1),
    SCENE_EXIT_IF(8, 267, 7, -1),
    SCENE_EXIT_IF(9, 267, 10, -1),
    SCENE_EXIT_IF(10, 267, 9, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001900[] = {
    {221, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(304), FACING_SOUTHWEST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(392), FACING_SOUTHEAST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(264), FACING_WEST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(320), 0, PIXELS(320), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(176), 0, PIXELS(416), FACING_NORTHWEST, 1, 0},
    {221, CONDITION_ALWAYS, 2, PIXELS(88), 0, PIXELS(568), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(544), FACING_EAST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(544), FACING_NORTH, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(336), FACING_NORTHWEST, 1, 0},
    {154, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(336), FACING_EAST, 1, 0},
    {221, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(360), FACING_WEST, 1, 0},
    {246, 0x7c, 1, PIXELS(232), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {421, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(360), FACING_NORTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {329, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(136), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001a80[] = {
    {130, CONDITION_ALWAYS, (s32)gData_020016b8, PIXELS(128), 0, PIXELS(120), FACING_NORTHWEST, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(120), FACING_SOUTH, 1, 0},
    {202, CONDITION_ALWAYS, 1, PIXELS(448), 0, PIXELS(408), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001ae0[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02001aec[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000eec)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000e54)},
    {0x4602, 6, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x8602, 8, CONDITION_ALWAYS, EVENT(Func_0200102c)},
    {0x602, 9, CONDITION_ALWAYS, EVENT(Func_02000ffc)},
    {1, 7, CONDITION_ALWAYS, 7},
    {0, 8, 0x1a5f, MSGNO(MsgLoho29)},
    {0, 9, 0x1a5f, MSGNO(MsgLoho30)},
    {0, 10, 0x1a5f, MSGNO(MsgLoho31)},
    {0, 11, 0x1a5f, EVENT(Func_02000bf4)},
    {0x8d15, 8, 0x1a5f, MSGNO(MsgLoho33)},
    {0x8d15, 9, 0x1a5f, MSGNO(MsgLoho34)},
    {0x8d15, 10, 0x1a5f, MSGNO(MsgLoho35)},
    {0x8d15, 11, 0x1a5f, EVENT(Func_02000c24)},
    {0, 12, 0x1a5f, MSGNO(MsgLoho37)},
    {0, 13, 0x1a5f, MSGNO(MsgLoho38)},
    {0, 14, 0x1a5f, MSGNO(MsgLoho39)},
    {0, 15, 0x1a5f, MSGNO(MsgLoho40)},
    {0, 16, 0x1a5f, MSGNO(MsgLoho41)},
    {0, 18, 0x1a5f, MSGNO(MsgLoho42)},
    {0, 17, 0x1a5f, MSGNO(MsgLoho43)},
    {0x8d15, 12, 0x1a5f, MSGNO(MsgLoho44)},
    {0x8d15, 13, 0x1a5f, MSGNO(MsgLoho45)},
    {0x8d15, 14, 0x1a5f, MSGNO(MsgLoho46)},
    {0x8d15, 15, 0x1a5f, MSGNO(MsgLoho47)},
    {0x8d15, 16, 0x1a5f, MSGNO(MsgLoho48)},
    {0x8d15, 18, 0x1a5f, MSGNO(MsgLoho49)},
    {0x8d15, 17, 0x1a5f, MSGNO(MsgLoho50)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgLoho01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgLoho02)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgLoho03)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgLoho04)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgLoho09)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgLoho10)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgLoho11)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgLoho12)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgLoho21)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgLoho22)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_0200012c)},
    {0, 20, CONDITION_ALWAYS, EVENT(Func_02000178)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgLoho05)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgLoho06)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgLoho07)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgLoho08)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgLoho13)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgLoho14)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgLoho15)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgLoho16)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLoho24)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgLoho25)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgLoho26)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLoho58))},
    {0x1cc14, 20, CONDITION_ALWAYS, EVENT(Func_02000198)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {0x50008805, 101, 0x300, EVENT(Func_02000c54)},
    {0x50008805, 102, 0x301, EVENT(Func_02000c64)},
    {0x50008805, 103, 0x302, EVENT(Func_02000c74)},
    {0x8f15, 22, 0x201, EVENT(Func_02000c84)},
    {TRIGGERS_END},
};

struct Trigger gData_02001dd4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {0xc402, 20, 0x320, EVENT(Func_02000e24)},
    {0x8402, 22, 0x320, EVENT(Func_02000e0c)},
    {2, 21, 0x1320, EVENT(Func_02000e3c)},
    {2, 23, 0x1320, EVENT(Func_02000e3c)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000d4c)},
    {0x8d15, 8, 0xa5f, MSGNO(MsgLoho18)},
    {0x8d15, 8, 0x1a5f, MSGNO(MsgLoho52)},
    {0, 9, 0x1320, EVENT(Func_02000d40)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000da4)},
    {0x8d15, 10, 0xa5f, MSGNO(MsgLoho20)},
    {0x8d15, 10, 0x1a5f, MSGNO(MsgLoho54)},
    {3, 20, CONDITION_ALWAYS, EVENT(Func_02000d40)},
    {3, 30, CONDITION_ALWAYS, EVENT(Func_02000d00)},
    {TRIGGERS_END},
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
