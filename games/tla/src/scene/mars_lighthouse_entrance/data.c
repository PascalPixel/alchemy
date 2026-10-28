#include "types.h"
#include "scene_tables.h"
#include "edition.h"
#include "objcmd.h"

void Func_02000038(void);
void Func_02000070(void);
void Func_02000808(void);
void Func_02000a24(void);
void Func_02000c1c(void);
void Func_02000c68(void);
void Func_0200308c(void);
void Func_02003138(void);
void Func_020031dc(void);

// clang-format off
u32 gData_02004de8[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(200), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(104), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02004e50[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(232), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), 0, PIXELS(112)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02004eb8[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(904), 0, PIXELS(528)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(840), 0, PIXELS(528)),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

u32 gData_02004f44[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_WAIT(10),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(30),
    SC_LABEL(1),
    SC_MOVE(PIXELS(152), 0, PIXELS(688)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(88), 0, PIXELS(688)),
    SC_SYNC,
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

u32 gData_02004ff0[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_END,
};

s32 gData_0200503c[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, PIXELS(32)),
    SC_SET(SW_GROUND, PIXELS(32)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(808), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(808), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(728), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(712), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(616), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(552), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(536), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(520), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(296), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(264), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(136), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(32), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(120), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(60),
    SC_CALL(Func_02000070),
    SC_END,
};

u32 gData_02005314[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_X, PIXELS(56)),
    SC_SET(SW_Z, PIXELS(128)),
    SC_SET(SW_Y, PIXELS(48)),
    SC_SET(SW_GROUND, PIXELS(48)),
    SC_SET(SW_SPEED, 0x8000),
    SC_SET(SW_ACCEL, 0x800),
    SC_MOVE(PIXELS(56), PIXELS(48), PIXELS(128)),
    SC_SYNC,
    SC_WAIT(20),
    SC_END,
};

u32 gData_02005394[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_020053d8[] = {
    8, 7, 512, 9, 7, 513, -1,
};

u16 gData_020053e6[] = {
    9, 3, 10, 3, 11, 3, 12, 3,
    0,
};

u16 gData_020053f8[] = {
    8, 0, 0,
};

s16 gData_020053fe[] = {
    16, 7, 514, 17, 7, 515, -1,
};

u16 gData_0200540c[] = {
    8, 3, 0,
};

u16 gData_02005412[] = {
    9, 1, 0,
};

s16 gData_02005418[] = {
    12, 7, 516, 13, 7, 517, 14, 7,
    518, 15, 7, 519, 16, 7, 520, -1,
};

u16 gData_02005438[] = {
    9, 3, 0,
};

u16 gData_0200543e[] = {
    8, 0, 10, 1, 0,
};

u16 gData_02005448[] = {
    11, 67, 0,
};

u16 gData_0200544e[] = {
    12, 1, 0,
};

s16 gData_02005454[] = {
    13, 7, 528, 14, 7, 529, 15, 7,
    530, 16, 7, 531, 17, 7, 532, 18,
    7, 533, -1, 0,
};

s32 gData_0200547c[] = {
    SC_CALL(Func_0200308c),
    SC_MOVE(PIXELS(112), PIXELS(32), PIXELS(728)),
    SC_SYNC,
    SC_MOVE(PIXELS(112), PIXELS(22), PIXELS(728)),
    SC_SYNC,
    SC_MOVE(PIXELS(128), PIXELS(22), PIXELS(728)),
    SC_SYNC,
    SC_CALL(Func_020031dc),
    SC_END,
};

s32 gData_020054cc[] = {
    46,
    (s32)Func_0200308c,
    4,
    0x500000,
    0x200000,
    0xa80000,
    1,
    4,
    0x500000,
    0x160000,
    0xa80000,
    1,
    4,
    0x600000,
    0x160000,
    0xa80000,
    1,
    22,
    37,
    (s32)Func_02000c68,
    46,
    (s32)Func_020031dc,
    17,
};

s32 gData_02005528[] = {
    SC_CALL(Func_0200308c),
    SC_MOVE(PIXELS(872), PIXELS(32), PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(872), PIXELS(30), PIXELS(576)),
    SC_SYNC,
    SC_MOVE(PIXELS(872), PIXELS(30), PIXELS(584)),
    SC_SYNC,
    SC_CALL(Func_020031dc),
    SC_END,
};

s32 gData_02005578[] = {
    46,
    (s32)Func_02003138,
    4,
    0x600000,
    0x200000,
    0x2a80000,
    1,
    4,
    0x600000,
    0x160000,
    0x2a80000,
    1,
    4,
    0x700000,
    0x160000,
    0x2a80000,
    1,
    22,
    37,
    (s32)Func_02000a24,
    46,
    (s32)Func_020031dc,
    17,
};

s32 gData_020055d4[] = {
    46,
    (s32)Func_02003138,
    4,
    0x3580000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x200000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x880000,
    1,
    4,
    0x3680000,
    0x160000,
    0x980000,
    1,
    22,
    37,
    (s32)Func_02000c1c,
    46,
    (s32)Func_020031dc,
    17,
};

s32 gData_02005644[] = {
    46,
    (s32)Func_02003138,
    4,
    0x700000,
    0x200000,
    0x2a80000,
    1,
    4,
    0x700000,
    0x160000,
    0x2a80000,
    1,
    4,
    0x800000,
    0x160000,
    0x2a80000,
    1,
    22,
    37,
    (s32)Func_02000808,
    46,
    (s32)Func_020031dc,
    17,
};

u32 gData_020056a0[] = {
    SC_WAIT(30),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_020056d0[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02005714[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_02005758[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000, 0xffff, -0x8000, -0x4000, (s32)0xc000ffff,
    -0xc000, 0x4000ffff, 0xffff, -0x8000,
};

s32 gData_02005798[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u32 gData_020057b8[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020057f4[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02005830[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

s16 gData_0200586c[] = {
    137, 138, 139, 140, 141, 142, 143, -1,
};

s16 gData_0200587c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020058a8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020058d4[] = {
    0x89008e, 0x8b008a, 0x8d008c, 0x8e008d, 0x8a0089, 0x8c008b,
    0x8d008c, 0x89008e, 0x8b008a, 0x8c008b, 0x8e008d, 0x8a0089,
    0x8b008a, 0x8d008c, 0x89008e, 0x8a0089, 0x8c008b, 0x8e008d,
};

s32 gData_0200591c[] = {
    SC_CALL(Func_02000038),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(279),
    SCENE_EXIT(1, 2, 84),
    SCENE_EXIT(2, 281, 2),
    SCENE_EXITS(280),
    SCENE_EXIT(1, 281, 1),
    SCENE_EXIT(2, 287, 2),
    SCENE_EXIT(3, 280, 4),
    SCENE_EXIT(4, 280, 3),
    SCENE_EXIT(5, 282, 5),
    SCENE_EXIT(6, 282, 6),
    SCENE_EXIT(7, 280, 8),
    SCENE_EXIT(8, 280, 7),
    SCENE_EXIT(9, 280, 10),
    SCENE_EXIT(10, 280, 9),
    SCENE_EXIT(20, 281, 20),
    SCENE_EXITS(281),
    SCENE_EXIT(1, 280, 1),
    SCENE_EXIT(2, 279, 2),
    SCENE_EXIT(3, 282, 3),
    SCENE_EXIT(4, 282, 4),
    SCENE_EXIT(5, 283, 5),
    SCENE_EXIT(6, 281, 7),
    SCENE_EXIT(7, 281, 6),
    SCENE_EXIT(8, 281, 9),
    SCENE_EXIT(9, 281, 8),
    SCENE_EXIT(10, 281, 11),
    SCENE_EXIT(11, 281, 10),
    SCENE_EXIT(12, 281, 13),
    SCENE_EXIT(13, 281, 12),
    SCENE_EXIT(14, 281, 15),
    SCENE_EXIT(15, 281, 14),
    SCENE_EXIT(16, 282, 16),
    SCENE_EXIT(17, 281, 18),
    SCENE_EXIT(18, 281, 17),
    SCENE_EXIT(20, 280, 3),
    SCENE_EXITS(282),
    SCENE_EXIT(1, 283, 1),
    SCENE_EXIT(2, 283, 2),
    SCENE_EXIT(3, 281, 3),
    SCENE_EXIT(4, 281, 4),
    SCENE_EXIT(5, 280, 5),
    SCENE_EXIT(6, 280, 6),
    SCENE_EXIT(7, 284, 7),
    SCENE_EXIT(8, 282, 9),
    SCENE_EXIT(9, 282, 8),
    SCENE_EXIT(10, 282, 11),
    SCENE_EXIT(11, 282, 10),
    SCENE_EXIT(14, 283, 14),
    SCENE_EXIT(16, 281, 16),
    SCENE_EXITS(283),
    SCENE_EXIT(1, 282, 1),
    SCENE_EXIT(2, 282, 2),
    SCENE_EXIT(3, 283, 4),
    SCENE_EXIT(4, 283, 3),
    SCENE_EXIT(5, 281, 5),
    SCENE_EXIT(6, 283, 7),
    SCENE_EXIT(7, 283, 6),
    SCENE_EXIT(8, 283, 9),
    SCENE_EXIT(9, 283, 8),
    SCENE_EXIT(10, 283, 11),
    SCENE_EXIT(11, 283, 10),
    SCENE_EXIT(12, 283, 13),
    SCENE_EXIT(13, 283, 12),
    SCENE_EXIT(14, 282, 14),
    SCENE_EXITS(284),
    SCENE_EXIT(1, 285, 1),
    SCENE_EXIT(2, 284, 3),
    SCENE_EXIT(3, 284, 2),
    SCENE_EXIT(4, 284, 5),
    SCENE_EXIT(5, 284, 4),
    SCENE_EXIT(6, 286, 6),
    SCENE_EXIT(7, 282, 7),
    SCENE_EXIT(8, 286, 8),
    SCENE_EXIT(9, 286, 9),
    SCENE_EXIT(10, 284, 11),
    SCENE_EXIT(11, 284, 10),
    SCENE_EXIT(12, 284, 13),
    SCENE_EXIT(13, 284, 12),
    SCENE_EXIT(14, 284, 15),
    SCENE_EXIT(15, 284, 14),
    SCENE_EXIT(16, 286, 10),
    SCENE_EXITS(285),
    SCENE_EXIT(1, 284, 1),
    SCENE_EXIT(2, 285, 3),
    SCENE_EXIT(3, 285, 2),
    SCENE_EXIT(4, 285, 6),
    SCENE_EXIT(5, 286, 5),
    SCENE_EXIT(6, 285, 4),
    SCENE_EXIT(7, 286, 7),
    SCENE_EXIT(11, 286, 11),
    SCENE_EXITS(286),
    SCENE_EXIT(1, 286, 2),
    SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4),
    SCENE_EXIT(4, 286, 3),
    SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6),
    SCENE_EXIT(7, 285, 7),
    SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

u32 gData_02005ae8[] = {
    39, 7, 17,
};

struct ActorPlacement gData_02005af4[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02005b0c[] = {
    {413, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(280), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(280), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gData_02005b54[] = {
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, 0, 0, 0, FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02005ae8, PIXELS(136), 0, PIXELS(424),
     FACING_SOUTH, 2, 0},
    {489, 800, 1, PIXELS(648), 0, PIXELS(376), FACING_SOUTH, 2, 0},
#if (LANGUAGE == LANGUAGE_JAPANESE)
    {245, 105, 1, PIXELS(664), 0, PIXELS(424), FACING_SOUTH, 0, 0},
#else
    {245, 105, 1, PIXELS(728), 0, PIXELS(408), FACING_NORTHWEST, 0, 0},
#endif
    {SCENE_TABLE_END},
};
