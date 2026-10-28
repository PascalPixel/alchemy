#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_020000f0(void);
void Func_020002b4(void);
void Func_0200049c(void);
void Func_02000640(void);
void Func_020006a8(void);
void Idejima_RunAlexDeparture(void);

// clang-format off
u32 gData_02003330[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(264), 0, PIXELS(164)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200336c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(216), 0, PIXELS(182)),
    SC_SYNC,
    SC_MOVE(PIXELS(242), 0, PIXELS(164)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_020033a8[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(224), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(392), 0, PIXELS(172)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003400[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(224), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(304), 0, PIXELS(172)),
    SC_SYNC,
    SC_MOVE(PIXELS(336), 0, PIXELS(192)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_0200346c[] = {
    39, 2, 4, 0xd80000, 0, 0xac0000,
    1, 4, 0x1280000, 0, 0xac0000, 1,
    4, 0x13c0000, 0, 0xd00000, 1, 39,
    1, 0, 1, 22, 3, 0,
    0, 1, 22, 35, 1, 17,
};

u32 gData_020034e4[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(420), 0, PIXELS(200)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_0200350c[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(408), 0, PIXELS(232)),
    SC_SYNC,
    SC_ARG(1),
    SC_END,
};

u32 gData_02003534[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(424), 0, PIXELS(200)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003578[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(408), 0, PIXELS(228)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_020035bc[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(400), 0, PIXELS(214)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003600[] = {
    39, 2, 4, 0x19c0000, 0, 0xf80000,
    1, 39, 1, 0, 1, 22,
    3, 0, 0, 1, 22, 35,
    1, 17,
};

s32 gData_02003650[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(4)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003734[] = {
    SC_POSITION(PIXELS(152), 0, PIXELS(756)),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(PIXELS(146), 0, PIXELS(736)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gData_02003790[] = {
    39, 2, 4, 0x980000, 0, 0x2f40000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gIdejima_ActorDepartureScript[] = {
    39, 2, 4, 0x680000, 0, 0x2d80000,
    1, 39, 1, 3, 0, 1,
    17,
};

s32 gIdejimaEntrances[] = {
    -0x10000, 172, 0x40000095, -0x10000, -1, 0xffff,
    0xffff, 0, 0, 0, 0, 0,
};

u32 gIdejimaExits[] = {
    9, 0x303009, 0x401002, 0xb40002, 0xc03000, 0xd41002,
    511,
};

u32 gIdejimaPlacements[] = {
    0xffff0038, 1, 0, 0, 0, 0x24000,
    0xffff0005, 1, 0, 0, 0, 0x24000,
    0xffff0006, 1, 0, 0, 0, 0x20000,
    0xffff015a, 7, 0x1000000, 0, 0xc80000, 0x28000,
    0xffff0039, 1, 0, 0, 0, 0x24000,
    0xffff01e9, 7, 0, 0, 0, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gIdejimaEvents[] = {
    -1,
    0,
    0,
};

s32 gIdejimaEventsEntrance1[] = {
    0xc602, (s32)0xffff0003, (s32)Func_02000640, -1, 0, 0,
};

s32 gIdejimaEventsWake[] = {
    1,
    (s32)0xffff0004,
    4,
    3,
    0x863000a,
    (s32)Func_020000f0,
    3,
    0x864000b,
    (s32)Func_020002b4,
    3,
    0x865000c,
    (s32)Func_0200049c,
    0x8d15,
    (s32)0xffff0408,
    (s32)Func_020000f0,
    0x8d15,
    (s32)0xffff0405,
    (s32)Func_020002b4,
    2,
    (s32)0xffff0009,
    (s32)Func_020006a8,
    2,
    0x8670008,
    (s32)Idejima_RunAlexDeparture,
    -1,
    0,
    0,
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
