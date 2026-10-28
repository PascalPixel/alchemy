#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_0200036c(void);
void Func_020005dc(void);
void Func_020005f8(void);
void Func_02000610(void);
void Func_02000678(void);
void Func_020006e0(void);
void Func_020006f8(void);
void Func_02000718(void);
void Func_02000748(void);
void Func_020007f4(void);
void Func_020008b4(void);
void Func_02000900(void);
void Func_02000af8(void);
void Func_02000b6c(void);
void Func_02000c38(void);
void Func_02000c74(void);
void Func_02000cd8(void);
void Func_02000cf8(void);
void Func_02000d18(void);
void Func_02000edc(void);
void Func_02001048(void);
void Func_020011c8(void);
void Func_020012ac(void);
void Func_0200136c(void);
void Func_0200142c(void);
void Func_020014d0(void);
void Func_0200151c(void);
void Func_020019cc(void);
void Func_02001d14(void);
void Func_02001ebc(void);
void Func_02001f68(void);
void Func_02001f84(void);
void Func_02001fa0(void);
void Func_02001fd8(void);
void Func_02002020(void);
void Func_0200225c(void);
void Func_020024b8(void);
void Func_02002718(void);
void Func_02002974(void);
void Func_02002a14(void);
void Func_02002a24(void);
void Func_02002a34(void);
void Func_0200485c(void);

extern u32 gData_0200501c[];
extern u32 gData_02005058[];
extern u32 gData_02005094[];
extern u32 gData_02005528[];
extern u32 gData_02005a30[];

// clang-format off
u32 gData_0200501c[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005058[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005094[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020050d0[] = {
    0, 230, 22, 38, 0, 3,
    0, 1, 38,
};

u32 gData_020050f4[] = {
    0, 25, 22, 38, 0, 3,
    0, 1, 38,
};

s32 gData_02005118[] = {
    SC_MOVE_BY(0, 0, PIXELS(2)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-4)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(2)),
    SC_SYNC,
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gData_02005168[] = {
    SC_MOVE_BY(0, 0, PIXELS(4)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(-8)),
    SC_SYNC,
    SC_MOVE_BY(0, 0, PIXELS(4)),
    SC_SYNC,
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_020051b8[] = {
    SC_MOVE(PIXELS(744), 0, PIXELS(432)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u16 gData_020051d8[] = {
    42, 30, 44, 20, 46, 21, 47, 23,
    47, 24, 46, 26, 44, 27, 43, 27,
    41, 26, 40, 24, 40, 23, 41, 21,
};

u32 gData_02005208[] = {
    0, 6, 38, 17,
};

u32 gData_02005218[] = {
    SC_MOVE_BY(0, PIXELS(160), 0),
    SC_SYNC,
    SC_FREE,
    SC_END,
};

s32 gData_02005234[] = {
    SC_CALL(Func_0200136c),
    SC_FREE,
    SC_END,
};

s32 gData_02005244[] = {
    SC_CALL(Func_0200142c),
    SC_CALL(Func_020014d0),
    SC_CALL(Func_0200151c),
    SC_FREE,
    SC_END,
};

u32 gData_02005264[] = {
    SC_MOVE_BY(0, PIXELS(24), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

u32 gData_02005288[] = {
    SC_MOVE_BY(0, PIXELS(16), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 4),
    SC_END,
};

s32 gData_020052ac[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, PIXELS(-24), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 0),
    SC_END,
};

s32 gData_020052dc[] = {
    SC_SET(SW_MOTION, 0),
    SC_MOVE_BY(0, PIXELS(-16), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

u16 gData_0200530c[] = {
    0, 0, 113, 47, 115, 47, 115, 45,
    117, 45, 119, 45, 119, 47, 121, 47,
    117, 49, 119, 49, 119, 51, 121, 51,
};

u32 gData_0200533c[] = {
    0x33002d, 0x35001f, 0x39001f, 0x3b0027, 0x3b0029, 0x3f002b,
    0x3f002d, 0x450023, 0x450025, 0, 1, 13,
    44, 0, 0, 1, 2, 0,
    0, 0, 0, 1, 38, 17,
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020053d8[] = {
    0, 1, 17,
};

u32 gData_020053e4[] = {
    0x10043f42, 0x1006001e, 0x1008001e, 0x1006001e, 0xfe00001e, 0xffff,
};

u32 gData_020053fc[] = {
    0x1010000, 0x1210111, 0xc310131, 0x22311731, 0x2d212d31, 0x2d012d11,
    0x17012201, 0xc01,
};

s32 gData_0200541c[] = {
    (s32)gData_0200501c,
    (s32)gData_02005058,
    (s32)gData_02005094,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02005458[] = {
    0xffe202bc, 0x2d0014a, 0x15efff6, 0x8ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(177),
    SCENE_EXIT(1, 177, 4),
    SCENE_EXIT(2, 177, 3),
    SCENE_EXIT(3, 177, 2),
    SCENE_EXIT(4, 177, 1),
    SCENE_EXIT(9, 178, 4),
    SCENE_EXIT(21, 181, 4),
    SCENE_EXIT(22, 179, 3),
    SCENE_EXIT(23, 179, 3),
    SCENE_EXIT(24, 179, 3),
    SCENE_EXIT(250, 178, 4),
    SCENE_EXIT(251, 177, 9),
    SCENE_EXITS(178),
    SCENE_EXIT(1, 174, 4),
    SCENE_EXIT(2, 178, 3),
    SCENE_EXIT(3, 178, 2),
    SCENE_EXIT(4, 177, 1),
    SCENE_EXIT(5, 178, 6),
    SCENE_EXIT(6, 178, 5),
    SCENE_EXIT(7, 180, 1),
    SCENE_EXIT(8, 178, 9),
    SCENE_EXIT(9, 178, 8),
    SCENE_EXIT(10, 180, 2),
    SCENE_EXITS(179),
    SCENE_EXIT(1, 180, 3),
    SCENE_EXIT(2, 181, 7),
    SCENE_EXIT(3, 181, 6),
    SCENE_EXIT(20, 177, 20),
    SCENE_EXITS(180),
    SCENE_EXIT(1, 178, 10),
    SCENE_EXIT(2, 178, 7),
    SCENE_EXIT(3, 179, 1),
    SCENE_EXITS(181),
    SCENE_EXIT(1, 181, 2),
    SCENE_EXIT(2, 181, 1),
    SCENE_EXIT(3, 181, 4),
    SCENE_EXIT(4, 181, 3),
    SCENE_EXIT(5, 181, 6),
    SCENE_EXIT(6, 181, 5),
    SCENE_EXIT(7, 179, 2),
    SCENE_EXIT(8, 179, 3),
    SCENE_EXIT(9, 179, 6),
    SCENE_EXIT(20, 177, 20),
    SCENE_EXITS_END,
};

u32 gData_02005528[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, 0),
    SC_END,
};

s32 gData_02005544[] = {
    22,
    15,
    0x10000,
    22,
    30,
    129,
    22,
    26,
    0,
    22,
    2,
    0x3000,
    2,
    0x1c80000,
    -0x200000,
    0x1c80000,
    22,
    22,
    -0x200000,
    46,
    (s32)Func_020005dc,
    17,
};

u32 gData_0200559c[] = {
    SC_SET(SW_SPEED, 0x4000),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_END,
};

s32 gData_020055c0[] = {
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_COLLISION, 129),
    SC_SET(SW_MOTION, 3),
    SC_MOVE(PIXELS(472), PIXELS(-32), PIXELS(344)),
    SC_SYNC,
    SC_SET(SW_COLLISION, 0),
    SC_CALL(Func_020005dc),
    SC_END,
};

s32 gData_02005610[] = {
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_COLLISION, 0),
    SC_MOVE_BY(PIXELS(4), PIXELS(8), 0),
    SC_SYNC,
    SC_LABEL(1),
    SC_CALL(Func_020005f8),
    SC_WAIT(2),
    SC_LOOP(8, SC_TO(1)),
    SC_CMD_2A(197),
    SC_CALL(Func_02000678),
    SC_WAIT(240),
    SC_LABEL(2),
    SC_CALL(Func_02000610),
    SC_WAIT(1),
    SC_LOOP(8, SC_TO(2)),
    SC_MOVE_BY(PIXELS(-4), PIXELS(-8), 0),
    SC_SYNC,
    SC_SET(SW_MOTION, 3),
    SC_END,
};

s32 gData_020056c4[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_MOVE(PIXELS(424), 0, PIXELS(344)),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_SYNC,
    SC_CALL(Func_020007f4),
    SC_WAIT(2),
    SC_POSITION(0, 0, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gData_02005728[] = {
    SC_SET(SW_SPEED, 0x20000),
    SC_MOVE(PIXELS(512), 0, PIXELS(384)),
    SC_SET(SW_VELOCITY_Y, PIXELS(8)),
    SC_SYNC,
    SC_CALL(Func_020007f4),
    SC_WAIT(2),
    SC_POSITION(0, 0, 0),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_HOOK, (s32)Func_02000900),
    SC_SET(SW_COLLISION, 129),
    SC_LABEL(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_CALL(Func_020006e0),
    SC_WAIT(15),
    SC_SET(SW_SPEED, 0x4000),
    SC_SET(SW_STEERING, 0),
    SC_ARG(9),
    SC_CALL(Func_020006f8),
    SC_MOVE_BY(PIXELS(16), 0, 0),
    SC_SYNC,
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_STEERING, 1),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(2),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_MOVE_BY(PIXELS(-16), 0, 0),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_NORTH),
    SC_SET(SW_SPEED, 0x8000),
    SC_CALL(Func_02000718),
    SC_ARG(7),
    SC_WAIT(15),
    SC_MOVE(PIXELS(472), 0, PIXELS(360)),
    SC_SYNC,
    SC_WAIT(10),
    SC_ARG(0),
    SC_CALL(Func_02000748),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(260),
    SC_SET(SW_FACING, FACING_NORTH),
    SC_WAIT(20),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(10),
    SC_CALL(Func_020008b4),
    SC_WAIT(10),
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_FACING, FACING_SOUTH),
    SC_ARG(2),
    SC_MOVE(PIXELS(472), 0, PIXELS(456)),
    SC_SYNC,
    SC_SET(SW_FACING, FACING_WEST),
    SC_ARG(0),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

struct ActorPlacement gData_02005970[] = {
    {394, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020059a0[] = {
    {148, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(328), FACING_SOUTH, 2, 0},
    {398, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {29, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(456), FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {323, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

u32 gData_02005a30[] = {
    SC_SET(SW_SCALE_X, 0xc000),
    SC_SET(SW_SCALE_Y, 0xc000),
    SC_WAIT(1),
    SC_END,
};

struct ActorPlacement gData_02005a54[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {388, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(688), FACING_SOUTH, 2, 0},
    {273, CONDITION_ALWAYS, (s32)gData_02005a30, PIXELS(920), 0, PIXELS(360), FACING_SOUTH, 2, 0},
    {273, CONDITION_ALWAYS, (s32)gData_02005a30, PIXELS(952), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(496), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005afc[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(352), FACING_SOUTH, 2, 0},
    {388, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(352), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(528), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(672), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b8c[] = {
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {320, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005c7c[] = {
    {388, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(368), FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, 1, PIXELS(336), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {372, CONDITION_ALWAYS, 1, PIXELS(680), 0, PIXELS(648), FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {378, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {397, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {385, CONDITION_ALWAYS, (s32)gData_02005528, PIXELS(704), 0, PIXELS(384), FACING_SOUTH, 2, 0},
    {347, CONDITION_ALWAYS, (s32)gData_02005528, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s32 gData_02005d6c[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_Y, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_Y, -4096),
    SC_WAIT(1),
    SC_END,
};

s32 gData_02005e28[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x1800),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_LOOP(2, SC_TO(4)),
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_ADD(SW_SCALE_X, -4096),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02005ee4[] = {
    0,
};

struct Trigger gData_02005ee8[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {2, 2, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {2, 3, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {2, 4, CONDITION_ALWAYS, EVENT(Func_0200485c)},
    {0xc15, 8, 0x200, EVENT(Func_02000af8)},
    {TRIGGERS_END},
};

struct Trigger gData_02005f30[] = {
    {6, 42, CONDITION_ALWAYS, EVENT(Func_02000edc)},
    {1, 9, CONDITION_ALWAYS, 9},
    {2, 40, 0x9de, EVENT(Func_02001048)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_020011c8)},
    {0x8d15, 10, CONDITION_ALWAYS, EVENT(Func_020012ac)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020019cc)},
    {0x53, 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside11))},
    {3, 55, CONDITION_ALWAYS, EVENT(Func_02001ebc)},
    {3, 56, CONDITION_ALWAYS, EVENT(Func_02001ebc)},
    {0x53, 77, 0xe41, EVENT(Func_02001fa0)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001f68)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_02001f84)},
    {TRIGGERS_END},
};

struct Trigger gData_02005fcc[] = {
    {1, 9, CONDITION_ALWAYS, 9},
    {2, 42, CONDITION_ALWAYS, EVENT(Func_02001d14)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_020019cc)},
    {0x53, 50, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside11))},
    {3, 55, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgGaiaRockInside08))},
    {0x53, 77, 0xe41, EVENT(Func_02001fa0)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001f68)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_02001f84)},
    {TRIGGERS_END},
};

struct Trigger gData_02006038[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {0x21, 10, CONDITION_ALWAYS, 10},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_02001fd8)},
    {0x1c314, 1036, CONDITION_ALWAYS, EVENT(Func_02002020)},
    {0x8c15, 10, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {0x8c15, 11, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02002974)},
    {TRIGGERS_END},
};

s32 gData_020060f8[] = {
    1,
    (s32)0xffff0001,
    1,
    33,
    (s32)0xffff0002,
    2,
    1,
    (s32)0xffff0003,
    3,
    0,
    (s32)0xffff000a,
    (s32)Func_02001fd8,
    0x1c314,
    (s32)0xffff040a,
    (s32)Func_0200225c,
    0,
    (s32)0xffff000b,
    (s32)Func_02001fd8,
    0x1c314,
    (s32)0xffff040b,
    (s32)Func_020024b8,
    0x50008905,
    (s32)0xffff0014,
    (s32)Func_02000b6c,
    0x50008905,
    0xffff,
    0,
};

struct Trigger gData_02006164[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {0x50008905, 20, CONDITION_ALWAYS, EVENT(Func_02000c38)},
    {TRIGGERS_END},
};

struct Trigger gData_020061a0[] = {
    {0xce01, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0xce01, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0xce01, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x4602, 43, CONDITION_ALWAYS, EVENT(Func_0200036c)},
    {2, 30, 0x303, EVENT(Func_02002a14)},
    {2, 31, 0x304, EVENT(Func_02002a24)},
    {2, 32, 0x305, EVENT(Func_02002a34)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02001fd8)},
    {0x1c314, 1033, CONDITION_ALWAYS, EVENT(Func_02002718)},
    {0x4e15, 10, CONDITION_ALWAYS, 0},
    {0x50008905, 40, CONDITION_ALWAYS, EVENT(Func_02000cd8)},
    {0x50008905, 41, CONDITION_ALWAYS, EVENT(Func_02000cf8)},
    {0x50008905, 42, CONDITION_ALWAYS, EVENT(Func_02000d18)},
    {0x50008905, 43, CONDITION_ALWAYS, EVENT(Func_02000d18)},
    {0x50008905, 44, CONDITION_ALWAYS, EVENT(Func_02000c74)},
    {TRIGGERS_END},
};

u32 gData_02006290[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020062d4[] = {
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x8000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x8000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(4),
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x8000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x4000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x2000),
    SC_WAIT(4),
    SC_WAIT(20),
    SC_LOOP(3, SC_TOP),
    SC_END,
};
// clang-format on
