#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_020000c8(void);
void Func_0200058c(void);

extern s16 gData_0200233c[];

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 288, 0, 454, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS(204),
    SCENE_EXIT_IF(1, 202, 11, 0xffffffff),
    SCENE_EXIT_IF(2, 204, 6, 0xffffffff),
    SCENE_EXIT_IF(3, 204, 7, 0xffffffff),
    SCENE_EXIT_IF(4, 204, 8, 0xffffffff),
    SCENE_EXIT_IF(5, 204, 9, 0xffffffff),
    SCENE_EXIT_IF(6, 204, 2, 0xffffffff),
    SCENE_EXIT_IF(7, 204, 3, 0xffffffff),
    SCENE_EXIT_IF(8, 204, 4, 0xffffffff),
    SCENE_EXIT_IF(9, 204, 5, 0xffffffff),
    SCENE_EXIT_IF(10, 202, 10, 0xffffffff),
    SCENE_EXIT_IF(11, 204, 13, 0xffffffff),
    SCENE_EXIT_IF(12, 204, 14, 0xffffffff),
    SCENE_EXIT_IF(13, 204, 11, 0xffffffff),
    SCENE_EXIT_IF(14, 204, 12, 0xffffffff),
    SCENE_EXIT_IF(15, 204, 16, 0xffffffff),
    SCENE_EXIT_IF(16, 204, 15, 0xffffffff),
    SCENE_EXIT_IF(17, 202, 17, 0xffffffff),
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    { 157, CONDITION_ALWAYS, 1, PIXELS(912), 0, PIXELS(624), 0x5000, 1, 0 },
    { 156, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), 0x3000, 1, 0 },
    { 160, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(456), FACING_EAST, 3, 0 },
    { 165, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(360), FACING_EAST, 1, 0 },
    { 164, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(136), 0xd000, 1, 0 },
    { 31, CONDITION_ALWAYS, 1, PIXELS(864), 0, PIXELS(392), 0x3000, 1, 0 },
    { 43, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(408), 0x3000, 1, 0 },
    { 34, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(392), FACING_WEST, 1, 0 },
    { 231, CONDITION_ALWAYS, 1, PIXELS(888), 0, PIXELS(560), 0x5000, 1, 0 },
    { 56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0 },
    { 5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0 },
    { 6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0 },
    { 7, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(424), FACING_NORTHEAST, 2, 0 },
    { SCENE_TABLE_END },
};

// clang-format off
s16 gData_0200233c[] = {
    1, 52, 1, 2, 6, 2, 52, 1,
    2, 6, -1, 2, 54, 2, 2, 6,
    4, 54, 2, 2, 6, -1,
};

s32 gData_02002368[] = {
    (s32)gData_0200233c,
    0x170005,
    (s32)gData_0200233c,
    0x17001e,
};

s32 gData_02002378[] = {
    (s32)gData_0200233c,
    0x40038,
};

struct SceneEvent gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {0xc602, 2, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000c8)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000c8)},
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
    {0xc602, 16, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020000c8)},
    {1, 17, CONDITION_ALWAYS, 17},
    {2, 20, 0x96a, EVENT_SCRIPT(Func_0200058c)},
    {0, 8, 0x196a, MSGNO(MsgLemuriaPalace12)},
    {0, 9, 0x196a, MSGNO(MsgLemuriaPalace13)},
    {0, 10, 0x196a, MSGNO(MsgLemuriaPalace14)},
    {0, 11, 0x196a, MSGNO(MsgLemuriaPalace15)},
    {0, 12, 0x196a, MSGNO(MsgLemuriaPalace16)},
    {0x8d15, 8, 0x196a, MSGNO(MsgLemuriaPalace17)},
    {0x8d15, 9, 0x196a, MSGNO(MsgLemuriaPalace18)},
    {0x8d15, 10, 0x196a, MSGNO(MsgLemuriaPalace19)},
    {0x8d15, 11, 0x196a, MSGNO(MsgLemuriaPalace20)},
    {0x8d15, 12, 0x196a, MSGNO(MsgLemuriaPalace21)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace01)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace02)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace03)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace04)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace05)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace06)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace07)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace08)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace09)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace10)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace22)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace23)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace24)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace25)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace26)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaPalace27)},
    {243, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {243, 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {243, 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {243, 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {243, 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {243, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {243, 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {243, 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {451, 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {SCENE_EVENTS_END},
};

u32 gData_02002608[] = {
    0x76c07b00, 0x6e407280, 0x65c06a00, 0x65c06180, 0x6e406a00, 0x76c07280,
};
// clang-format on
