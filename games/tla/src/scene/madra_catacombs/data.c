#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000474(void);
void Func_020004cc(void);
void Func_0200051c(void);
void Func_0200052c(void);
void Func_02000558(void);
void Func_02000568(void);
void Func_0200072c(void);
void Func_02000778(void);
void Func_020007a0(void);
void Func_02000874(void);
void Func_020008f4(void);
void Func_0200092c(void);
void Func_0200093c(void);
void Func_02000a88(void);
void Func_02000c24(void);
void Func_02000c40(void);
void Func_02000c5c(void);
void Func_02000c78(void);

// clang-format off
s32 gData_0200121c[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Z, -0x8000),
    SC_ADD(SW_SCALE_X, -1024),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(24, SC_TOP),
    SC_END,
};

u32 gData_02001264[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x4000),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_Y, 0x2000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s32 gData_020012f4[] = {
    SC_SET(SW_SCALE_X, 0x1000),
    SC_SET(SW_SCALE_Y, 0x1000),
    SC_WAIT(1),
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 0x8000),
    SC_ADD(SW_SCALE_Y, 0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, 0x6000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, -0x2000),
    SC_ADD(SW_SCALE_Y, -0x4000),
    SC_WAIT(1),
    SC_LOOP(4, SC_TO(3)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_END,
};

s16 gData_020013c0[] = {
    12, -1,
};

s16 gData_020013c4[] = {
    13, 514, -1,
};

s16 gData_020013ca[] = {
    8, -1, 0,
};

s32 gData_020013d0[] = {
    -0x10000, 172, 0x40000095, -0x10000, -1, 0xffff,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001400[] = {
    0xffff0003, 512, 0xc00000e0, 0x1880000, 0x2780028, 280,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001430[] = {
    0xffff0005, 728, 0x40000058, 0x2a80000, 0x3980020, 192,
    0xffff0006, 872, 0x40000058, 0x2a80000, 0x3980020, 192,
    0xffff0007, 872, 120, 0x2a80000, 0x3980020, 192,
    0xffff0008, 792, 0xc0000098, 0x2a80000, 0x3980020, 192,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_020014a8[] = {
    112, 48, 144, 128, 64, 160, -1, 1,
    336, 48, 144, 352, 64, 160, -1, 3,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(47),
    SCENE_EXIT(1, 51, 8),
    SCENE_EXIT(2, 50, 7),
    SCENE_EXIT(3, 49, 1),
    SCENE_EXIT(4, 50, 13),
    SCENE_EXIT(5, 49, 6),
    SCENE_EXITS(48),
    SCENE_EXIT(1, 44, 15),
    SCENE_EXIT(2, 49, 7),
    SCENE_EXIT(3, 44, 16),
    SCENE_EXIT(4, 49, 4),
    SCENE_EXITS(49),
    SCENE_EXIT(1, 47, 3),
    SCENE_EXIT(2, 49, 3),
    SCENE_EXIT(3, 49, 2),
    SCENE_EXIT(4, 48, 4),
    SCENE_EXIT(5, 49, 8),
    SCENE_EXIT(6, 47, 5),
    SCENE_EXIT(7, 48, 2),
    SCENE_EXIT(8, 49, 5),
    SCENE_EXITS(50),
    SCENE_EXIT(1, 51, 1),
    SCENE_EXIT(2, 50, 11),
    SCENE_EXIT(3, 50, 9),
    SCENE_EXIT(4, 51, 2),
    SCENE_EXIT(5, 52, 4),
    SCENE_EXIT(6, 52, 1),
    SCENE_EXIT(7, 47, 2),
    SCENE_EXIT(8, 50, 12),
    SCENE_EXIT(9, 50, 3),
    SCENE_EXIT(10, 51, 7),
    SCENE_EXIT(11, 50, 2),
    SCENE_EXIT(12, 50, 8),
    SCENE_EXIT(13, 47, 4),
    SCENE_EXITS(51),
    SCENE_EXIT(1, 50, 1),
    SCENE_EXIT(2, 50, 4),
    SCENE_EXIT(3, 51, 5),
    SCENE_EXIT(4, 51, 6),
    SCENE_EXIT(5, 51, 3),
    SCENE_EXIT(6, 51, 4),
    SCENE_EXIT(7, 50, 10),
    SCENE_EXIT(8, 47, 1),
    SCENE_EXITS(52),
    SCENE_EXIT(1, 50, 6),
    SCENE_EXIT(2, 52, 3),
    SCENE_EXIT(3, 52, 2),
    SCENE_EXIT(4, 50, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_0200159c[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020015b4[] = {
    {250, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(424), FACING_SOUTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(632), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(360), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200165c[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(776), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(696), 0, PIXELS(232), FACING_SOUTH, 2, 1},
    {71, CONDITION_ALWAYS, 1, PIXELS(794), 0, PIXELS(236), FACING_WEST, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020016bc[] = {
    {679, CONDITION_ALWAYS, 1, PIXELS(202), 0, PIXELS(408), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020016ec[] = {
    {330, CONDITION_ALWAYS, 7, PIXELS(150), 0, PIXELS(366), FACING_SOUTH, 2, 0},
    {330, CONDITION_ALWAYS, 7, PIXELS(170), 0, PIXELS(366), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {410, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001914[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02001920[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000a88)},
    {1, 5, CONDITION_ALWAYS, 5},
    {0x8515, 9, 0x201, 0},
    {9, 10, 0x8c8, EVENT(Func_020004cc)},
    {0x8c15, 11, 0x8c8, EVENT(Func_020004cc)},
    {0x10008c15, 12, 0x8ca, EVENT(Func_0200051c)},
    {0x8c15, 12, 0x8ca, EVENT(Func_0200052c)},
    {0x602, 11, 0x9ef, EVENT(Func_020008f4)},
    {0x10008c15, 8, 0x9ef, EVENT(Func_0200092c)},
    {0x8c15, 8, 0x9ef, EVENT(Func_0200093c)},
    {0x1815, 13, 0x202, EVENT(Func_02000558)},
    {0x50009085, 0, CONDITION_ALWAYS, EVENT(Func_0200072c)},
    {0x40009085, 0, CONDITION_ALWAYS, EVENT(Func_02000778)},
    {0xc403, 10, CONDITION_ALWAYS, EVENT(Func_020007a0)},
    {TRIGGERS_END},
};

struct Trigger gData_020019f8[] = {
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 4, CONDITION_ALWAYS, 4},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02001a1c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {0x8515, 8, 0x200, 0},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgFieldCaveFoundByChampa)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgFieldWhoDugCave)},
    {TRIGGERS_END},
};

struct Trigger gData_02001aac[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {0xc602, 9, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {TRIGGERS_END},
};

struct Trigger gData_02001b54[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000c40)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02000c5c)},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {0x8715, 8, CONDITION_ALWAYS, EVENT(Func_02000874)},
    {TRIGGERS_END},
};

struct Trigger gData_02001bcc[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000c78)},
    {1, 4, CONDITION_ALWAYS, 4},
    {0x1cb04, 10, CONDITION_ALWAYS, EVENT(Func_02000568)},
    {3, 10, 0x8c9, EVENT(Func_02000474)},
    {TRIGGERS_END},
};
