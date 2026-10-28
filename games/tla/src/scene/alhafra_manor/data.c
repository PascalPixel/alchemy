#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_0200006c(void);
void Func_020000b8(void);
void Func_02000150(void);
void Func_0200019c(void);
void Func_02000238(void);
void Func_02000370(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 760, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 312, 0, 120, FACING_NORTHEAST, 0, 224, 16, 464, 192,
     0},
    {99, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(97),
    SCENE_EXIT_IF(1, 95, 14, 0xffffffff),
    SCENE_EXIT_IF(2, 97, 7, 0xffffffff),
    SCENE_EXIT_IF(3, 97, 8, 0xffffffff),
    SCENE_EXIT_IF(4, 97, 9, 0xffffffff),
    SCENE_EXIT_IF(5, 97, 10, 0xffffffff),
    SCENE_EXIT_IF(6, 97, 15, 0xffffffff),
    SCENE_EXIT_IF(7, 97, 2, 0xffffffff),
    SCENE_EXIT_IF(8, 97, 3, 0xffffffff),
    SCENE_EXIT_IF(9, 97, 4, 0xffffffff),
    SCENE_EXIT_IF(10, 97, 5, 0xffffffff),
    SCENE_EXIT_IF(11, 97, 12, 0xffffffff),
    SCENE_EXIT_IF(12, 97, 11, 0xffffffff),
    SCENE_EXIT_IF(13, 97, 14, 0xffffffff),
    SCENE_EXIT_IF(14, 97, 13, 0xffffffff),
    SCENE_EXIT_IF(15, 97, 6, 0xffffffff),
    SCENE_EXIT_IF(16, 97, 17, 0xffffffff),
    SCENE_EXIT_IF(17, 97, 16, 0xffffffff),
    SCENE_EXITS_END,
};

// clang-format off
u32 gData_02001708[] = {
    0xffff0070, 1, 0x1180000, 0, 0x980000, 0x10000,
    0xffff0018, 1, 0x1740000, 0, 0x5c0000, 0x1c000,
    0xffff0073, 1, 0x1280000, 0, 0x1580000, 0xe000,
    0xffff0074, 1, 0x980000, 0, 0x1480000, 0x2000,
    0xffff0080, 1, 0x2380000, 0, 0x680000, 0x1c000,
    0xffff0071, 1, 0x2680000, 0, 0x780000, 0,
    0xffff0072, 1, 0x3280000, 0, 0x1280000, 0xa000,
    0xffff0070, 1, 0x2d80000, 0, 0xf80000, 0x1a000,
    0xffff00d1, 1, 0x980000, 0, 0x2a80000, 0,
    0xffff00ba, 2, 0x680000, 0, 0x1f80000, 0x4000,
    0xffff0014, 1, 0x580000, 0, 0x780000, 0x8000,
    0xffff0013, 1, 0x800000, 0, 0x700000, 0x4000,
    0xffff, 0, 0, 0, 0, 0,
};

s16 gData_02001840[] = {
    112, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0x8000, 1, 24, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0x8000, 1,
    115, -1, 1, 0, 0, 344, 0, 0,
    0, 320, 0, 1, 231, -1, 2, 0,
    0, -56, 0, 0, 0, -56, 0xc000, 1,
    195, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0xc000, 1, 56, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0, 2,
    5, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0xe000, 2, 6, -1, 1, 0,
    0, -56, 0, 0, 0, -56, 0xe000, 2,
    7, -1, 1, 0, 0, -56, 0, 0,
    0, -56, 0xe000, 2, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
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
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {1, 14, CONDITION_ALWAYS, 14},
    {1, 15, CONDITION_ALWAYS, 15},
    {1, 16, CONDITION_ALWAYS, 16},
    {1, 17, CONDITION_ALWAYS, 17},
    {0, 10, 0x18a7, MSGNO(MsgAlhafraManor30)},
    {0x8d15, 10, 0x18a7, MSGNO(MsgAlhafraManor31)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor01)},
    {0, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200006c)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor03)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor04)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor05)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor06)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor07)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor08)},
    {0, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000b8)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor11)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor12)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor13)},
    {0, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000150)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor15)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor16)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor17)},
    {0, 18, 0x8ba, EVENT_SCRIPT(Func_0200019c)},
    {0x8d15, 1042, 0x8ba, EVENT_SCRIPT(Func_0200019c)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor19)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor20)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor21)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor22)},
    {0, 8, 0x8bb, EVENT_SCRIPT(Func_02000238)},
    {0x8d15, 1032, 0x8bb, EVENT_SCRIPT(Func_02000238)},
    {0, 9, 0x8bc, EVENT_SCRIPT(Func_02000370)},
    {0x8d15, 1033, 0x8bc, EVENT_SCRIPT(Func_02000370)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor25)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor26)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor27)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor28)},
    {371, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGERS_END},
};
