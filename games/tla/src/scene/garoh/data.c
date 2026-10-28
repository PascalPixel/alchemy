#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000420(void);
void Func_020004cc(void);
void Func_02000580(void);
void Func_020005cc(void);
void Func_02000618(void);
void Func_02000664(void);
void Func_02000698(void);
void Func_020006ec(void);
void Func_02000808(void);
void Func_02000960(void);
void Func_020009a4(void);
void Func_02000e58(void);
void Func_02000e68(void);
void Func_02000f74(void);
void Func_02000f80(void);
void Func_02000f8c(void);
void Func_02000f98(void);
void Func_02000fa4(void);
void Func_02000fbc(void);
void Func_02000fec(void);
void Func_0200101c(void);
void Func_0200105c(void);
void Func_0200108c(void);
void Func_02001098(void);
void Func_020010d4(void);
void Func_02001104(void);
void Func_02001144(void);
void Func_020011c8(void);
void Func_02001490(void);
void Func_020022f4(void);

extern u32 gData_0200539c[];
extern u32 gData_020053d8[];
extern u32 gData_02005414[];
extern s32 gData_0200552c[];
extern s16 gData_02005d9c[];
extern s16 gData_02005db2[];
extern s16 gData_02005dc8[];

// clang-format off
u32 gData_0200539c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020053d8[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005414[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s16 gData_02005450[] = {
    8, 9, 10, 11, -1, 0,
};

u32 gData_0200545c[] = {
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(12),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_SOUTHEAST + FACING_STEP),
    SC_WAIT(7),
    SC_SET(SW_FACING, FACING_SOUTH + FACING_STEP),
    SC_WAIT(34),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 gData_020054b8[] = {
    31, 31, 31, 29, 29, 30, 27, 28,
    29, 25, 27, 28, 23, 25, 27, 21,
    24, 26, 15, 18, 19, 10, 12, 13,
    5, 6, 6,
};

u16 gData_020054ee[] = {
    24, 28, 30, 19, 24, 27, 15, 20,
    25, 11, 16, 23, 7, 12, 21, 3,
    8, 19, 27, 27, 27, 16, 24, 29,
    0,
};

s32 gData_02005520[] = {
    (s32)gData_0200539c,
    (s32)gData_020053d8,
    (s32)gData_02005414,
};

s32 gData_0200552c[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(-4)),
    SC_ADD(SW_X, PIXELS(6)),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_CALL(Func_02000420),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(59),
    SCENE_EXIT_IF(1, 64, 1, -1),
    SCENE_EXIT_IF(2, 65, 4, -1),
    SCENE_EXIT_IF(3, 64, 3, -1),
    SCENE_EXIT_IF(4, 64, 4, -1),
    SCENE_EXIT_IF(5, 64, 5, -1),
    SCENE_EXIT_IF(6, 66, 2, -1),
    SCENE_EXIT_IF(7, 62, 4, -1),
    SCENE_EXIT_IF(8, 62, 1, -1),
    SCENE_EXIT_IF(9, 62, 11, -1),
    SCENE_EXIT_IF(10, 60, 2, -1),
    SCENE_EXIT_IF(11, 60, 3, -1),
    SCENE_EXIT_IF(12, 63, 1, -1),
    SCENE_EXITS(60),
    SCENE_EXIT_IF(1, 61, 2, -1),
    SCENE_EXIT_IF(2, 59, 10, -1),
    SCENE_EXIT_IF(3, 59, 11, -1),
    SCENE_EXITS(61),
    SCENE_EXIT_IF(1, 2, 15, -1),
    SCENE_EXIT_IF(2, 60, 1, -1),
    SCENE_EXITS(62),
    SCENE_EXIT_IF(1, 59, 8, -1),
    SCENE_EXIT_IF(2, 62, 3, -1),
    SCENE_EXIT_IF(3, 62, 2, -1),
    SCENE_EXIT_IF(4, 59, 7, -1),
    SCENE_EXIT_IF(5, 65, 3, -1),
    SCENE_EXIT_IF(6, 65, 5, -1),
    SCENE_EXIT_IF(7, 64, 6, -1),
    SCENE_EXIT_IF(8, 64, 7, -1),
    SCENE_EXIT_IF(9, 62, 10, -1),
    SCENE_EXIT_IF(10, 62, 9, -1),
    SCENE_EXIT_IF(11, 59, 9, -1),
    SCENE_EXITS(63),
    SCENE_EXIT_IF(1, 59, 12, -1),
    SCENE_EXIT_IF(2, 63, 4, -1),
    SCENE_EXIT_IF(3, 63, 5, -1),
    SCENE_EXIT_IF(4, 63, 2, -1),
    SCENE_EXIT_IF(5, 63, 3, -1),
    SCENE_EXIT_IF(6, 63, 7, -1),
    SCENE_EXIT_IF(7, 63, 6, -1),
    SCENE_EXIT_IF(11, 64, 99, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020056dc[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020056f4[] = {
    {131, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(400), FACING_EAST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(192), 0, PIXELS(304), FACING_EAST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(344), FACING_WEST, 1, 0},
    {131, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(312), FACING_SOUTH, 1, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(232), FACING_NORTH, 2, 1},
    {304, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(312), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(296), FACING_NORTH, 2, 0},
    {304, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(408), FACING_NORTH, 2, 1},
    {42, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02005874[] = {
    192, -1, 1, 0, 0, 232, 0, 0,
    0, 400, 0, 1, 190, -1, 1, 0,
    0, 416, 0, 0, 0, 424, 0x4000, 1,
    185, -1, 1, 0, 0, 192, 0, 0,
    0, 304, 0, 1, 196, -1, 1, 0,
    0, 280, 0, 0, 0, 344, 0x8000, 1,
    188, -1, 1, 0, 0, 368, 0, 0,
    0, 312, 0x4000, 1, 191, -1, 2, 0,
    0, 384, 0, 0, 0, 232, 0x4000, 1,
    304, -1, 1, 0, 0, 312, 0, 0,
    0, 232, 0xc000, 2, 304, -1, 1, 0,
    0, 408, 0, 0, 0, 232, 0xc000, 258,
    304, -1, 1, 0, 0, 184, 0, 0,
    0, 232, 0xc000, 2, 304, -1, 1, 0,
    0, 88, 0, 0, 0, 312, 0xc000, 2,
    304, -1, 1, 0, 0, 152, 0, 0,
    0, 280, 0xc000, 2, 304, -1, 1, 0,
    0, 264, 0, 0, 0, 248, 0xc000, 2,
    304, -1, 1, 0, 0, 408, 0, 0,
    0, 296, 0xc000, 2, 304, -1, 1, 0,
    0, 296, 0, 0, 0, 408, 0xc000, 258,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

struct ActorPlacement gData_020059dc[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(472), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(456), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(392), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(392), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005a54[] = {
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(128), 0, PIXELS(168), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(208), 0, PIXELS(56), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(152), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(248), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(352), 0, PIXELS(328), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(216), 0, PIXELS(424), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(96), 0, PIXELS(488), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(328), 0, PIXELS(440), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(456), 0, PIXELS(352), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(136), 0, PIXELS(664), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b5c[] = {
    {22, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(120), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(120), FACING_NORTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005c1c[] = {
    {22, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {42, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {246, 0x75, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(64), 0, PIXELS(562), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(370), 0, PIXELS(56), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(256), 0, PIXELS(80), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(184), 0, PIXELS(352), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(392), 0, PIXELS(368), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(280), 0, PIXELS(626), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_0200552c, PIXELS(336), 0, PIXELS(784), FACING_NORTH, 2, 1},
    {SCENE_TABLE_END},
};

s16 gData_02005d9c[] = {
    82, 32, 1, 2, 6, 80, 32, 1,
    2, 6, -1,
};

s16 gData_02005db2[] = {
    82, 35, 1, 2, 6, 80, 35, 1,
    2, 6, -1,
};

s16 gData_02005dc8[] = {
    78, 32, 2, 2, 6, 78, 35, 2,
    2, 6, -1, 0,
};

s32 gData_02005de0[] = {
    (s32)gData_02005d9c,
    0x15004e,
    (s32)gData_02005d9c,
    0,
    (s32)gData_02005d9c,
    0xa0048,
    (s32)gData_02005db2,
    0x16004a,
    (s32)gData_02005d9c,
    0x170055,
    (s32)gData_02005dc8,
    0xd004e,
};

struct Trigger gData_02005e10[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02005e34[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0xc401, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {0xc402, 12, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {2, 20, 0x91e, EVENT(Func_02000808)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000580)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh03)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgGaroh05)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020005cc)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgGaroh07)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh08)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgGaroh09)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgGaroh10)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgGaroh11)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaroh31))},
    {TRIGGERS_END},
};

struct Trigger gData_02005f78[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc401, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0xc401, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {0xc402, 12, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgGaroh15)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh16)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000664)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_02000618)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgGaroh19)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgGaroh22)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgGaroh23)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgGaroh24)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgGaroh25)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgGaroh26)},
    {0x8d15, 1037, 0x301, EVENT(Func_020006ec)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgGaroh27)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_0200105c)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaroh31))},
    {TRIGGERS_END},
};

struct Trigger gData_020060d4[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {2, 30, 0x91c, EVENT(Func_02001490)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_0200108c)},
    {TRIGGERS_END},
};

s32 gData_0200611c[] = {
    1,          (s32)0xffff0001, 1,
    1,          (s32)0xffff0002, 2,
    2,          0x2100014,       (s32)Func_02001144,
    2,          0x2110015,       (s32)Func_020011c8,
    0x10008c15, 0x2010008,       (s32)Func_02000e58,
    0x8c15,     0x2010008,       (s32)Func_02000f74,
    0x10008c15, 0x2020009,       (s32)Func_02000e58,
    0x8c15,     0x2020009,       (s32)Func_02000f80,
    0x10008c15, 0x203000a,       (s32)Func_02000e58,
    0x8c15,     0x203000a,       (s32)Func_02000f8c,
    0x10008c15, 0x204000b,       (s32)Func_02000e58,
    0x8c15,     0x204000b,       (s32)Func_02000f98,
    8,          0x2010000,       (s32)Func_02000e68,
    9,          0x2010000,       (s32)Func_02000fa4,
    0x40009085, -0x10000,        (s32)Func_02001098,
};

struct Trigger gData_020061d0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x21, 8, CONDITION_ALWAYS, 8},
    {0x21, 9, CONDITION_ALWAYS, 9},
    {0x31, 10, CONDITION_ALWAYS, 10},
    {0x21, 11, CONDITION_ALWAYS, 11},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_02000fec)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_020010d4)},
    {TRIGGERS_END},
};

struct Trigger gData_02006278[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {0xc402, 6, CONDITION_ALWAYS, EVENT(Func_02000960)},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {2, 30, 0x91d, EVENT(Func_020022f4)},
    {2, 31, CONDITION_ALWAYS, EVENT(Func_020009a4)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgGaroh29)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgGaroh30)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_0200101c)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_02001104)},
    {TRIGGERS_END},
};

u32 gData_0200632c[] = {
    0,
};

u32 gData_02006330[] = {
    38,
};
