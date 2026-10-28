#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_0200006c(void);
void Func_020000c8(void);
void Func_02000190(void);
void Func_02000200(void);
void Func_02000394(void);
void Func_0200042c(void);
void Func_02000514(void);
void Func_020006ec(void);
void Func_02000730(void);
void Func_02000838(void);
void Func_02000940(void);
void Func_02000954(void);
void Func_02000958(void);
void Func_02000968(void);
void Func_02001574(void);
void Func_02002048(void);
void Func_02002a08(void);

extern s16 gData_02005284[];

// clang-format off
u16 gData_02004ca4[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02004cb0[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 gData_02004cc8[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
    10, -1,
};

s16 gData_02004d0c[] = {
    14, 1, 0x8a2, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 488, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 424, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 440, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {99, CONDITION_ALWAYS, 136, 0, 568, FACING_NORTH, 0, 32, 0, 608, 704, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(98),
    SCENE_EXIT_IF(1, 95, 11, CONDITION_ALWAYS),
    SCENE_EXIT_IF(2, 99, 1, CONDITION_ALWAYS),
    SCENE_EXITS_END,
};

s16 gData_02004dbc[] = {
    407, -1, 1, 0, 0, 280, 0, 0,
    0, 232, FACING_SOUTH, 2, 407, -1, 1, 0,
    0, 376, 0, 0, 0, 232, FACING_SOUTH, 258,
    287, -1, 1, 0, 0, 552, 0, 0,
    0, 520, FACING_SOUTH, 2, 287, -1, 1, 0,
    0, 408, 0, 0, 0, 520, FACING_SOUTH, 2,
    489, -1, 1, 0, 0, 344, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 1, 0,
    0, 440, 0, 0, 0, 312, FACING_SOUTH, 2,
    406, -1, 1, 0, 0, 488, 0, 0,
    0, 312, FACING_SOUTH, 2, 258, -1, 7, 0,
    0, 336, 0, 0, 0, 456, FACING_SOUTH, 2,
    467, -1, 1, 0, 0, 344, 0, 0,
    0, 472, FACING_SOUTH, 2, 467, -1, 1, 0,
    0, 440, 0, 0, 0, 456, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 482, 0, 0, 0, 312, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 376, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 492, 0, 0, 0, 312, FACING_SOUTH, 2,
    39, -1, 1, 0, 0, 312, 0, 0,
    0, 440, 0, 2, 39, -1, 1, 0,
    0, 440, 0, 0, 0, 424, 0x5000, 1,
    39, -1, 1, 0, 0, 344, 0, 0,
    0, 488, FACING_NORTH, 1, 39, -1, 1, 0,
    0, 440, 0, 0, 0, 376, FACING_NORTHEAST, 1,
    5, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 6, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    7, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0,
};

s16 gData_02004fe4[] = {
    407, -1, 1, 0, 0, 280, 0, 0,
    0, 232, FACING_SOUTH, 2, 407, -1, 1, 0,
    0, 376, 0, 0, 0, 232, FACING_SOUTH, 258,
    287, -1, 1, 0, 0, 552, 0, 0,
    0, 520, FACING_SOUTH, 2, 287, -1, 1, 0,
    0, 408, 0, 0, 0, 520, FACING_SOUTH, 2,
    489, -1, 1, 0, 0, 344, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 1, 0,
    0, 440, 0, 0, 0, 312, FACING_SOUTH, 2,
    406, -1, 1, 0, 0, 488, 0, 0,
    0, 312, FACING_SOUTH, 2, 258, -1, 7, 0,
    0, 336, 0, 0, 0, 456, FACING_SOUTH, 2,
    467, -1, 1, 0, 0, 344, 0, 0,
    0, 472, FACING_SOUTH, 2, 467, -1, 1, 0,
    0, 440, 0, 0, 0, 456, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 312, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 482, 0, 0, 0, 312, FACING_SOUTH, 2,
    489, -1, 7, 0, 0, 444, 0, 0,
    0, 376, FACING_SOUTH, 2, 489, -1, 7, 0,
    0, 492, 0, 0, 0, 312, FACING_SOUTH, 2,
    16, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 1, 37, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 1,
    38, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_WEST, 1, 24, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    5, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 6, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    7, -1, 1, 0, 0, 8, 0, 0,
    0, 8, FACING_NORTH, 2, 56, -1, 1, 0,
    0, 8, 0, 0, 0, 8, FACING_NORTH, 2,
    112, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, 195, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    187, -1, 10, 0, 0, -56, 0, 0,
    0, -56, 0, 2, 115, -1, 10, 0,
    0, -56, 0, 0, 0, -56, FACING_SOUTH, 2,
    114, -1, 10, 0, 0, -56, 0, 0,
    0, -56, FACING_SOUTH, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02005284[] = {
    0, 0, 1, 2, 6, -1,
};

s32 gData_02005290[] = {
    (s32)gData_02005284,
    0x110010,
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000940)},
    {0, 22, CONDITION_ALWAYS, EVENT(Func_02000730)},
    {0x8d15, 1046, 0x8a8, EVENT(Func_02000730)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgWePaidBig)},
    {0, 24, CONDITION_ALWAYS, MSGNO(MsgWeSentMy)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgIfWeCould)},
    {0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgPeopleCallUs)},
    {0x8d15, 23, CONDITION_ALWAYS, MSGNO(MsgIThoughtIt)},
    {0x8d15, 24, CONDITION_ALWAYS, MSGNO(MsgThatLousyRat)},
    {0x8d15, 25, CONDITION_ALWAYS, MSGNO(MsgWhenIThink)},
    {0, 30, CONDITION_ALWAYS, MSGNO(MsgOooogOw2)},
    {0, 31, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {0, 28, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs2)},
    {0x8d15, 30, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {0x8d15, 31, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {0x8d15, 28, CONDITION_ALWAYS, MSGNO(MsgBriggsSaidTheBoat)},
    {0x50009705, 10, 0x8a7, EVENT(Func_0200006c)},
    {0x8515, 8, 0x8a3, 0},
    {0x50008615, 14, 0x8a2, EVENT(Func_020000c8)},
    {0x10008c15, 10, 0x8a0, EVENT(Func_02000190)},
    {0x8c15, 10, 0x8a0, EVENT(Func_02000200)},
    {0x10008c15, 11, 0x8a1, EVENT(Func_02000190)},
    {0x8c15, 11, 0x8a1, EVENT(Func_02000200)},
    {0x10008c15, 16, 0x8a4, EVENT(Func_02000394)},
    {0x8c15, 16, 0x8a4, EVENT(Func_0200042c)},
    {0x10008c15, 17, 0x8a5, EVENT(Func_02000190)},
    {0x8c15, 17, 0x8a5, EVENT(Func_02000200)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000190)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000200)},
    {0xc602, 12, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0x4602, 12, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0xc602, 13, CONDITION_ALWAYS, EVENT(Func_020006ec)},
    {0xc602, 11, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(Func_02000514)},
    {0xa602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xb602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x2602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x3602, 12, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xa602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0xb602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x2602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {0x3602, 11, CONDITION_ALWAYS, EVENT(Func_02000954)},
    {2, 20, CONDITION_ALWAYS, EVENT(Func_02000838)},
    {2, 21, 0x905, EVENT(Func_02002048)},
    {2, 22, 0x90f, EVENT(Func_02002a08)},
    {2, 23, 0x8ac, EVENT(Func_02001574)},
    {2, 24, 0x8bf, EVENT(Func_02000968)},
    {2, 25, 0x8be, EVENT(Func_02000958)},
    {TRIGGERS_END},
};

u32 gData_020054f0[] = {
    0,
};

s32 gData_020054f4[] = {
    39, 2, 5, 0, 0, 0x80000,
    1, 5, 0x100000, 0, 0x100000, 1,
    5, 0x100000, 0, -0x100000, 1, 5,
    0, 0, -0x100000, 1, 39, 1,
    22, 3, 0x8000, 0, 1, 17,
};

s32 gData_0200556c[] = {
    39, 2, 5, 0x200000, 0, 0x200000,
    1, 5, 0x100000, 0, -0x100000, 1,
    5, 0x20000, 0, -0x60000, 1, 39,
    1, 22, 3, 0x8000, 0, 1,
    17,
};

u32 gData_0200d5d0[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xa80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

u32 gData_02005620[] = {
    39, 2, 5, 0x300000, 0, 0,
    1, 4, 0xb80000, 0, 0x2480000, 1,
    39, 1, 22, 3, 0xe000, 0,
    1, 17,
};

s32 gData_02005670[] = {
    39, 2, 4, 0x980000, 0, 0x2800000,
    1, 5, -0x500000, 0, 0, 1,
    5, -0x200000, 0, -0x200000, 1, 5,
    -0x180000, 0, 0, 1, 0, 1,
    17,
};
// clang-format on

u32 gData_020056d4[] = {
    38,
};
