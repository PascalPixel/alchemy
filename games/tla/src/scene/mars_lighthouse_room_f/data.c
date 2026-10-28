#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_0200004c(void);
void Func_020006f4(void);
void Func_02000d80(void);
void Func_02003478(void);
void Func_0200351c(void);

s32 gData_020056c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
u32 gData_020056d4[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(680), 0, PIXELS(304)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(760), 0, PIXELS(304)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02005748[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(584), 0, PIXELS(304)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(664), 0, PIXELS(304)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_020057bc[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_FLAG_CHECK(544),
    SC_IFNOT(SC_TO(1)),
    SC_LABEL(2),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(248), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(176), 0, PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(104), 0, PIXELS(576)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(2)),
};

u32 gData_02005890[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(168), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_0200593c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_LABEL(1),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(232), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(80), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(360), PIXELS(32), PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_02005a3c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_END,
};

u32 gData_02005a88[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(776), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(872), 0, PIXELS(80)),
    SC_SYNC,
    SC_MOVE(PIXELS(968), 0, PIXELS(80)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s16 gData_02005b5c[] = {
    24, 25, 26, 24, 25, 0,
};

u32 gData_02005b68[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005bac[] = {
    0, 42, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u16 gData_02005bf0[] = {
    8, 67, 0,
};

u16 gData_02005bf6[] = {
    10, 1, 0,
};

u16 gData_02005bfc[] = {
    18, 17, 19, 49, 20, 19, 21, 3,
    0,
};

u16 gData_02005c0e[] = {
    22, 0, 0,
};

u16 gData_02005c14[] = {
    13, 17, 14, 33, 15, 49, 16, 131,
    0,
};

u16 gData_02005c26[] = {
    19, 1, 0,
};

u16 gData_02005c2c[] = {
    13, 3, 0,
};

u16 gData_02005c32[] = {
    14, 1, 0,
};

u16 gData_02005c38[] = {
    8, 3, 9, 3, 10, 3, 0,
};

u16 gData_02005c46[] = {
    12, 0, 0,
};

u16 gData_02005c4c[] = {
    13, 3, 14, 3, 15, 3, 0,
};

u16 gData_02005c5a[] = {
    10, 0, 0,
};

u16 gData_02005c60[] = {
    24, 3, 25, 3, 26, 3, 0,
};

u16 gData_02005c6e[] = {
    18, 0, 19, 0, 20, 0, 21, 0,
    22, 0, 0,
};

s32 gData_02005c84[] = {
    46,
    (s32)Func_02003478,
    4,
    0xf80000,
    0x200000,
    0x2a00000,
    1,
    4,
    0xf80000,
    0x160000,
    0x2a00000,
    1,
    4,
    0xf80000,
    0x160000,
    0x2b00000,
    1,
    22,
    37,
    (s32)Func_020006f4,
    46,
    (s32)Func_0200351c,
    17,
};

s32 gData_02005ce0[] = {
    46,
    (s32)Func_02003478,
    4,
    0xb80000,
    0x400000,
    0x2300000,
    1,
    4,
    0xb80000,
    0x360000,
    0x2300000,
    1,
    4,
    0xb80000,
    0x360000,
    0x2400000,
    1,
    22,
    37,
    (s32)Func_02000d80,
    46,
    (s32)Func_0200351c,
    17,
};

s32 gData_02005d3c[] = {
    SC_CALL(Func_02003478),
    SC_MOVE(PIXELS(872), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_MOVE(PIXELS(872), PIXELS(22), PIXELS(128)),
    SC_SYNC,
    SC_MOVE(PIXELS(872), PIXELS(22), PIXELS(144)),
    SC_SYNC,
    SC_CALL(Func_0200351c),
    SC_END,
};

u32 gData_02005d8c[] = {
    SC_WAIT(30),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02005dbc[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005e00[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005e44[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005e80[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005ebc[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s32 gData_02005ef8[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000,
};

s16 gData_02005f18[] = {
    -1, 0, -0x8000, -1, -0x4000, -1, -1, -0x4000,
    0x4000, -1, -1, 0x4000, -1, 0, -0x8000, -1,
};

s32 gData_02005f38[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u16 gData_02005f58[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02005f64[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 gData_02005f7c[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
};
// clang-format on
