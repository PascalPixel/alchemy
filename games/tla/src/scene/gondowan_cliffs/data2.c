#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000058(void);
void Func_020003f8(void);
void Func_02000584(void);
void Func_020005bc(void);
void Func_02000600(void);
void Func_02000610(void);
void Func_02000660(void);
void Func_020006f4(void);

extern u8 gData_02001324[];
extern u8 gData_02001360[];
extern u8 gData_0200139c[];

extern s32 gData_0200168c[];

s32 gData_0200168c[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

// clang-format off
s16 gData_02001698[] = {
    0, -1, 680, 0, 280, 0x4000, 0, -1,
    -1, -1, -1, 0, 1, -1, 536, 0,
    488, 0, 0, 512, 0, 0x800, 864, 0,
    2, -1, 0x7e8, 0, 792, 0x8000, 0, 512,
    0, 0x800, 864, 0, 3, -1, 0x628, 0,
    680, 0x4000, 0, 512, 0, 0x800, 864, 0,
    99, -1, 0x4f8, 0, 552, 0x4000, 0, 512,
    0, 0x800, 864, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

s32 gData_02001728[] = {
    -0x10000, 168, 0x40000118, -0x10000, -1, 0xffff,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(102),
    SCENE_EXIT_IF(1, 2, 21, -1),
    SCENE_EXIT_IF(2, 2, 22, -1),
    SCENE_EXIT_IF(3, 104, 1, -1),
    SCENE_EXITS(103),
    SCENE_EXIT_IF(1, 2, 85, -1),
    SCENE_EXIT_IF(2, 2, 86, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_0200178c[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(728), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(1560), 0, PIXELS(760), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(1624), 0, PIXELS(760), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(1304), 0, PIXELS(456), FACING_SOUTH, 0, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(1880), 0, PIXELS(568), FACING_SOUTH, 0, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(1576), 0, PIXELS(672), FACING_NORTH, 2, 0},
    {273, CONDITION_ALWAYS, 1, PIXELS(1232), 0, PIXELS(544), FACING_SOUTH, 2, 0},
    {189, CONDITION_ALWAYS, 1, PIXELS(1928), 0, PIXELS(728), FACING_WEST, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {83, 0x8ab, 1, PIXELS(888), 0, PIXELS(200), FACING_WEST, 1, 0},
    {83, 0x8ab, 1, PIXELS(888), 0, PIXELS(232), FACING_WEST, 1, 0},
    {424, 0x8e6, 1, PIXELS(816), 0, PIXELS(232), FACING_WEST, 2, 0},
    {245, 0x61, 1, PIXELS(1336), 0, PIXELS(648), FACING_SOUTH, 0, 0},
    {5, 0x18f3, 1, 0, 0, 0, FACING_EAST, 0, 0},
    {6, 0x18f3, 1, 0, 0, 0, FACING_EAST, 0, 0},
    {56, 0x18f3, 1, 0, 0, 0, FACING_EAST, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_0200193c[] = {
    {8, CONDITION_ALWAYS, (s32)gData_0200168c, PIXELS(330), 0, PIXELS(79), FACING_EAST, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {424, 0x8e6, 1, PIXELS(304), 0, PIXELS(232), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

s32 gData_020019b4[] = {
    (s32)gData_02001324,
    (s32)gData_02001360,
    (s32)gData_0200139c,
};

struct Trigger gData_020019c0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0x9815, 11, CONDITION_ALWAYS, EVENT(Func_020006f4)},
    {TRIGGERS_END},
};

struct Trigger gData_020019f0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0x51, 3, 0x1885, 3},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02000660)},
    {0x8d15, 16, 0x886, MSGNO(MsgDigHereYou)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgIKnewIt)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgITriedTo)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgIWonderWho)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgIfTheOther)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgILongTo)},
    {0, 21, CONDITION_ALWAYS, EVENT(Func_02000058)},
    {0x8515, 10, 0x200, 0},
    {0x1815, 12, 0x201, EVENT(Func_02000584)},
    {0x1815, 13, 0x202, EVENT(Func_020005bc)},
    {0x4e15, 14, CONDITION_ALWAYS, EVENT(Func_02000600)},
    {0x8c15, 15, 0x884, EVENT(Func_020003f8)},
    {0x50008805, 10, 0x886, EVENT(Func_02000610)},
    {TRIGGERS_END},
};

s32 gData_02001ac8[] = {
    0x2000200, 0x3600400, 0x600, 0x1601000, -1, -1,
};
// clang-format on
