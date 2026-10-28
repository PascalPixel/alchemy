#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000064(void);
void Func_020002b8(void);
void Func_020006ac(void);
void Func_02000724(void);
void Func_020008a4(void);

extern u32 gData_02002ebc[];
extern u32 gData_02002ef8[];
extern u32 gData_02002f34[];

// clang-format off
u32 gData_02002ebc[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002ef8[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002f34[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002f70[] = {
    SC_ADD(SW_SCALE_X, 0x1000),
    SC_WAIT(1),
    SC_LOOP(32, SC_TOP),
    SC_END,
};

s32 gData_02002f94[] = {
    SC_ADD(SW_SCALE_X, -960),
    SC_WAIT(1),
    SC_LOOP(140, SC_TOP),
    SC_END,
};

s16 gData_02002fb8[] = {
    17, 18, 19, -1,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(258),
    SCENE_EXIT(1, 257, 1),
    SCENE_EXIT(2, 258, 3),
    SCENE_EXIT(3, 258, 2),
    SCENE_EXIT(5, 263, 5),
    SCENE_EXIT(6, 263, 6),
    SCENE_EXIT(7, 258, 8),
    SCENE_EXIT(8, 258, 7),
    SCENE_EXIT(9, 259, 9),
    SCENE_EXIT(10, 259, 10),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(608), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(608), FACING_SOUTHEAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(552), FACING_SOUTHEAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(600), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(552), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(616), 0, PIXELS(664), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(664), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(664), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(744), 0, PIXELS(664), FACING_EAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(280), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(296), FACING_SOUTH, 2, 0},
    {312, CONDITION_ALWAYS, 1, PIXELS(328), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(328), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(328), FACING_SOUTHEAST, 2, 1},
    {57, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 1, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {294, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

u32 gData_02003214[] = {
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_ARG(1),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(1)),
    SC_WAIT(1),
    SC_LOOP(16, SC_TO(1)),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02003274[] = {
    0,
};

u32 gData_02003278[] = {
    1,
};

struct Trigger gSceneEvents[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {33, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {2, 21, 0xa00, EVENT(Func_020006ac)},
    {2, 22, 0xa00, EVENT(Func_02000724)},
    {2, 100, CONDITION_ALWAYS, EVENT(Func_020008a4)},
    {0, 22, CONDITION_ALWAYS, MSGNO(MsgFieldFollowTrueFeelings)},
    {0x8d15, 22, CONDITION_ALWAYS, MSGNO(MsgFieldPartWithAgatio)},
    {8, -1, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {9, -1, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x10008c15, 17, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x8c15, 17, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x10008c15, 18, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x8c15, 18, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x10008c15, 19, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x8c15, 19, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x10008c15, 11, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x8c15, 11, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x50009985, 0, 0xa00, EVENT(Func_020002b8)},
    {TRIGGERS_END},
};

s32 gData_020033d8[] = {
    -1,
};

u32 gData_020033dc[] = {
    1,
};

u32 gData_020033e0[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020033f4[] = {
    (s32)gData_02002ebc,
    (s32)gData_02002ef8,
    (s32)gData_02002f34,
};
