#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "edition.h"
#include "objcmd.h"

void Func_020000d8(void);
void Func_02000210(void);
void Func_020002ac(void);
void Func_0200030c(void);
void Func_02000380(void);
void Func_0200040c(void);
void Func_02000440(void);
void Func_020004f4(void);
void Func_02000520(void);
void Func_02000568(void);
void Func_020006f4(void);
void Func_02000e38(void);
void Func_020014e0(void);
void Func_02001a68(void);
void Func_02001bf4(void);
void Func_02001c38(void);
void Func_02001db0(void);
void Func_02001f74(void);
void Func_0200208c(void);
void Func_020020b4(void);
void Func_020020d0(void);
void Func_02002144(void);
void Func_020021d0(void);
void Func_02002430(void);
void Func_020024c8(void);
void Func_020024f0(void);
void Func_0200253c(void);
void Func_020025a4(void);
void Func_020025b0(void);
void Func_020025bc(void);
void Func_020025dc(void);
void Func_020025fc(void);
void Func_0200261c(void);
void Func_0200263c(void);
void Func_0200265c(void);
void Func_0200267c(void);
void Func_0200274c(void);
void Func_02002780(void);
void Func_02003620(void);
void Func_0200367c(void);

extern u32 gData_020040d4[];
extern u32 gData_02004198[];
extern u32 gData_02004640[];
extern s16 gData_020046a4[];
extern s16 gData_020046ba[];

// clang-format off
u32 gData_02004004[] = {
    SC_ADD(SW_FACING, FACING_STEP),
    SC_WAIT(1),
    SC_LOOP(16, SC_TOP),
    SC_END,
};

s32 gData_02004028[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_ADD(SW_SCALE_X, -1024),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

u16 gData_02004070[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_0200407c[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 gData_02004094[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
};

u32 gData_020040d4[] = {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    22, 30, 131, 22, 16, 0x6666,
#else
    22, 30, 129, 22, 16, 0x6666,
#endif
    22, 15, 0x6666, 39, 2, 4,
    0x1780000, 0, 0xd80000, 1, 4, 0x1880000,
    0, 0xe00000, 1, 39, 1, 0,
    1, 22, 3, 0x3000, 0, 80,
    39, 2, 4, 0x1780000, 0, 0xd80000,
    1, 4, 0x1780000, 0, 0xb80000, 1,
    39, 1, 0, 40, 13, 0xffff,
    0,
};

u32 gData_02004198[] = {
    SC_ARG(4),
    SC_SET(SW_SPEED, 0xcccc),
    SC_SET(SW_ACCEL, 0x3333),
    SC_SET(SW_Y, PIXELS(64)),
    SC_LABEL(1),
    SC_MOVE(PIXELS(352), PIXELS(64), PIXELS(336)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), PIXELS(64), PIXELS(336)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), PIXELS(64), PIXELS(328)),
    SC_SYNC,
    SC_MOVE(PIXELS(376), PIXELS(64), PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(368), PIXELS(64), PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(352), PIXELS(64), PIXELS(304)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), PIXELS(64), PIXELS(312)),
    SC_SYNC,
    SC_MOVE(PIXELS(344), PIXELS(64), PIXELS(328)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_02004274[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(234), 0, PIXELS(240)),
    SC_SYNC,
    SC_MOVE(PIXELS(276), 0, PIXELS(222)),
    SC_SYNC,
    SC_MOVE(PIXELS(308), 0, PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(252), 0, PIXELS(256)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(208)),
    SC_SYNC,
    SC_MOVE(PIXELS(232), 0, PIXELS(184)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gData_02004300[] = {
    SC_ARG(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(-16)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(-32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(32)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(280), 0, PIXELS(856)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 gData_020043f8[] = {
    SC_ARG(3),
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(16)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(0, 0, PIXELS(-32)),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE_BY(PIXELS(-32), 0, 0),
    SC_SYNC,
    SC_SE(152),
    SC_SET(SW_VELOCITY_Y, PIXELS(4)),
    SC_MOVE(PIXELS(280), 0, PIXELS(856)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_020044f0[] = {
    0, 15, 3, 0, 1, 38,
};

u32 gData_02004508[] = {
    SC_MOVE(PIXELS(248), 0, PIXELS(686)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(768)),
    SC_SYNC,
    SC_MOVE(PIXELS(152), 0, PIXELS(768)),
    SC_SYNC,
    SC_SET(SW_BYTE_63, 1),
    SC_END,
};

u32 gData_02004554[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(248), 0, PIXELS(686)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(768)),
    SC_SYNC,
    SC_MOVE(PIXELS(176), 0, PIXELS(777)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_020045a4[] = {
    SC_MOVE(PIXELS(248), 0, PIXELS(768)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(686)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(584)),
    SC_SYNC,
    SC_SET(SW_BYTE_63, 1),
    SC_END,
};

u32 gData_020045f0[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(248), 0, PIXELS(768)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(686)),
    SC_SYNC,
    SC_MOVE(PIXELS(240), 0, PIXELS(600)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02004640[] = {
    39, 2, 4, 0xb80000, 0, 0x3a80000,
    1, 39, 1, 1, 80, 39,
    2, 4, 0x580000, 0, 0x3a80000, 1,
    39, 1, 0, 80, 13, 0xffff,
    0,
};

s16 gData_020046a4[] = {
    92, 10, 1, 2, 6, 92, 8, 1,
    2, 6, -1,
};

s16 gData_020046ba[] = {
    -1,
};

s32 gData_020046bc[] = {
    (s32)gData_020046ba,
    0x400040,
    (s32)gData_020046a4,
    0xa0052,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(131),
    SCENE_EXIT(1, 2, 28),
    SCENE_EXIT(2, 183, 9),
    SCENE_EXITS(132),
    SCENE_EXIT(1, 2, 29),
    SCENE_EXIT(2, 57, 5),
    SCENE_EXIT(3, 132, 1),
    SCENE_EXITS(133),
    SCENE_EXIT(1, 2, 30),
    SCENE_EXIT(2, 2, 80),
    SCENE_EXIT(3, 11, 7),
    SCENE_EXIT(4, 2, 81),
    SCENE_EXITS(134),
    SCENE_EXIT(10, 2, 31),
    SCENE_EXIT(11, 183, 8),
    SCENE_EXITS(135),
    SCENE_EXIT(1, 2, 32),
    SCENE_EXIT(2, 156, 6),
    SCENE_EXIT(3, 135, 4),
    SCENE_EXIT(4, 135, 3),
    SCENE_EXIT(5, 136, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02004754[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200476c[] = {
    {73, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(744), FACING_WEST, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(920), FACING_NORTHWEST, 2, 0},
    {77, CONDITION_ALWAYS, (s32)gData_02004640, PIXELS(88), 0, PIXELS(936), FACING_EAST, 1, 0},
    {189, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(856), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020047e4[] = {
    {71, CONDITION_ALWAYS, (s32)gData_020040d4, PIXELS(376), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {399, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {258, CONDITION_ALWAYS, 7, PIXELS(176), 0, PIXELS(140), FACING_SOUTH, 2, 0},
    {402, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {402, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(296), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(328), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {402, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(360), FACING_SOUTH, 2, 1},
    {400, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(280), FACING_SOUTHEAST + FACING_STEP, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200494c[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(248), FACING_SOUTH, 0, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(184), FACING_SOUTHEAST, 1, 0},
    {80, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(176), FACING_WEST, 1, 0},
    {303, CONDITION_ALWAYS, 7, PIXELS(326), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020049c4[] = {
    {187, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(296), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(280), FACING_SOUTH, 1, 0},
    {405, CONDITION_ALWAYS, (s32)gData_02004198, PIXELS(360), 0, PIXELS(312), FACING_SOUTH, 1, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 0, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {288, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(88), 0, PIXELS(472), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02004a84[] = {
    {75, CONDITION_ALWAYS, 2, PIXELS(168), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {162, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(440), FACING_SOUTH, 0, 0},
    {163, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(202), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(266), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gData_02004b14[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02004b20[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02002780)},
    {0x602, 16, CONDITION_ALWAYS, EVENT(Func_0200367c)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgEverSinceThe)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02001bf4)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgYouReLucky)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgArffArff2)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgTheWaterwaysNorth)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgICanTWaitUntil)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgICanTWaitUntilThe)},
    {0x8d15, 11, CONDITION_ALWAYS, EVENT(Func_02000e38)},
    {TRIGGERS_END},
};

struct Trigger gData_02004bb0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_0200267c)},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(Func_0200208c)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(Func_0200367c)},
    {0x4602, 30, 0xa2c, EVENT(Func_020000d8)},
    {2, 20, 0xaa2, EVENT(Func_02001db0)},
    {2, 7, CONDITION_ALWAYS, EVENT(Func_020024f0)},
    {2, 8, CONDITION_ALWAYS, EVENT(Func_0200253c)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgMyMomSays)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000210)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {6, 200, CONDITION_ALWAYS, EVENT(Func_02003620)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgWhoInWeyard)},
    {0x8d15, 9, CONDITION_ALWAYS, EVENT(Func_020002ac)},
    {0x8d15, 19, CONDITION_ALWAYS, EVENT(Func_02000380)},
    {0, 21, CONDITION_ALWAYS, EVENT(Func_0200040c)},
    {0x8d15, 21, CONDITION_ALWAYS, MSGNO(MsgMayTheseTravelers)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_020025a4)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_020025b0)},
    {TRIGGERS_END},
};

s32 gData_02004ca0[] = {
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
    0,
    (s32)0xffff0008,
    MSGNO(MsgIJustWant),
    0,
    (s32)0xffff0009,
    MSGNO(MsgTheSeaOf),
    0,
    (s32)0xffff000a,
    MSGNO(MsgMooMoo2),
    0x8d15,
    (s32)0xffff0008,
    MSGNO(MsgDadAndGrandpa),
    0x8d15,
    (s32)0xffff0009,
    MSGNO(MsgIRememberOnce),
    0x8d15,
    (s32)0xffff000a,
    (s32)Func_020006f4,
    0x8b85,
    -0x10000,
    (s32)Func_020020b4,
    0x40008b85,
    -0x10000,
    (s32)Func_020020d0,
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
    0x50008905,
    (s32)0xffff000a,
    (s32)Func_020025bc,
    0x50008905,
    (s32)0xffff000b,
    (s32)Func_020025dc,
    0x50008905,
    (s32)0xffff000c,
    (s32)Func_020025fc,
    0x50008905,
    (s32)0xffff000d,
    (s32)Func_0200261c,
    0x50008905,
    (s32)0xffff000e,
    (s32)Func_0200263c,
    0x50008905,
    (s32)0xffff000f,
    (s32)Func_0200265c,
#endif
    -1,
    0,
    0,
};

struct Trigger gData_02004d84[] = {
    {1, 10, CONDITION_ALWAYS, 10},
    {0xce01, 11, CONDITION_ALWAYS, 11},
    {0x4602, 21, 0x212, EVENT(Func_02002430)},
    {8, 0, 0x212, EVENT(Func_02002144)},
    {0x10008c15, 12, 0x212, EVENT(Func_02002144)},
    {0x10008c15, 13, 0x212, EVENT(Func_02002144)},
    {9, 0, 0x212, EVENT(Func_020021d0)},
    {0x8c15, 12, 0x212, EVENT(Func_020021d0)},
    {0x8c15, 13, 0x212, EVENT(Func_020021d0)},
    {0x1815, 14, 0x213, EVENT(Func_020024c8)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000440)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020004f4)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000520)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgIfWeRe)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgSureThereAre)},
    {0x8d15, 10, CONDITION_ALWAYS, EVENT(Func_02000568)},
    {2, 20, 0x215, EVENT(Func_02001f74)},
    {TRIGGERS_END},
};

struct Trigger gData_02004e5c[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_0200274c)},
    {2, 2, CONDITION_ALWAYS, EVENT(Func_0200274c)},
    {0x31, 5, CONDITION_ALWAYS, 5},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgYouUsedTo)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgIRememberHow)},
    {0, 9, 0xaa5, MSGNO(MsgBlank3)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgYup2)},
    {0x8d15, 9, 0x217, EVENT(Func_020014e0)},
    {0x8d15, 9, CONDITION_ALWAYS, EVENT(Func_02001a68)},
    {0, 11, CONDITION_ALWAYS, EVENT(Func_02001c38)},
    {0, 12, CONDITION_ALWAYS, EVENT(Func_02001c38)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_020020b4)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_020020d0)},
    {TRIGGERS_END},
};
