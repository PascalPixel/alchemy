#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_02000058(void);
void Func_02000104(void);
void Func_02000154(void);
void Func_020001a0(void);
void Func_020001ec(void);
void Func_02000a5c(void);
void Func_020013f0(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gSceneRegions[] = {
    416, 32, 592, 432, 48, 608, -1, 13,
    688, -24, 608, 704, -8, 624, -1, 14,
    -1, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

u32 gSceneExits[] = {
    SCENE_EXITS(203),
    SCENE_EXIT_IF(1, 202, 1, 0xffffffff),
    SCENE_EXIT_IF(2, 202, 2, 0xffffffff),
    SCENE_EXIT_IF(3, 202, 3, 0xffffffff),
    SCENE_EXIT_IF(4, 203, 6, 0xffffffff),
    SCENE_EXIT_IF(5, 203, 7, 0xffffffff),
    SCENE_EXIT_IF(6, 203, 4, 0xffffffff),
    SCENE_EXIT_IF(7, 203, 5, 0xffffffff),
    SCENE_EXIT_IF(8, 203, 10, 0xffffffff),
    SCENE_EXIT_IF(9, 203, 11, 0xffffffff),
    SCENE_EXIT_IF(10, 203, 8, 0xffffffff),
    SCENE_EXIT_IF(11, 203, 9, 0xffffffff),
    SCENE_EXIT_IF(12, 202, 12, 0xffffffff),
    SCENE_EXIT_IF(13, 203, 14, 0xffffffff),
    SCENE_EXIT_IF(14, 203, 13, 0xffffffff),
    SCENE_EXIT_IF(15, 202, 15, 0xffffffff),
    SCENE_EXIT_IF(16, 202, 16, 0xffffffff),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {157, CONDITION_ALWAYS, 2, PIXELS(480), 0, PIXELS(144), FACING_SOUTH, 0, 0},
    {156, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(136), 0xd000, 1, 0},
    {160, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(104), FACING_WEST, 0, 0},
    {219, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(88), FACING_EAST, 0, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(328), 0xb000, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(588), 0xb000, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(552), 0x5000, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(40), 0, PIXELS(328), 0x3000, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(552), 0x3000, 3, 0},
    {43, CONDITION_ALWAYS, 1, PIXELS(400), 0, PIXELS(416), 0xd000, 3, 0},
    {35, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(792), 0xb000, 1, 0},
    {SCENE_TABLE_END},
};

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
    {0x4401, 15, CONDITION_ALWAYS, 15},
    {1, 16, CONDITION_ALWAYS, 16},
    {2, 30, 0x966, EVENT(Func_02000a5c)},
    {2, 31, 0x969, EVENT(Func_020013f0)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors02)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors03)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors04)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000058)},
    {0x8d15, 10, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_02000154)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors14)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors15)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors16)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors17)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_020001a0)},
    {0x8d15, 1037, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 1038, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 1039, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 1040, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 1041, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 1043, 0x967, EVENT(Func_020001ec)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors20)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors21)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors22)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors23)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors24)},
    {0x8d15, 19, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors25)},
    {243, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {243, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {243, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {243, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {243, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {243, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {243, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {243, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {451, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {TRIGGERS_END},
};

u32 gData_02002dec[] = {
    38,
};
