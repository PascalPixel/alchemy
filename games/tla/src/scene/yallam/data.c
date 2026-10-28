#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"
#include "edition.h"

void Func_02000044(void);
void Func_02000270(void);
void Func_02000328(void);
void Func_0200036c(void);
void Func_0200038c(void);
void Func_0200041c(void);
void Func_0200043c(void);
void Func_020004cc(void);
void Func_02000544(void);
void Func_02000584(void);
void Func_02000614(void);
void Func_020022f8(void);

void Func_020022f4(void);

extern s32 gData_020025a8[];
extern s32 gData_020025d4[];
extern s32 gData_020025f8[];
extern u32 gData_02002634[];
extern u32 gData_020026d8[];
extern s16 gData_02002c8c[];
extern s16 gData_02002ca2[];
extern s16 gData_02002cb8[];
extern s16 gData_02002cce[];

s32 gData_020025a8[] = {
    39, 3, 22, 26, 0, 23, 6, 0x20000, 46, (s32)Func_02000044, 17,
};

s32 gData_020025d4[] = {
    22, 26, 0, 23, 6, -0x100000, 46, (s32)Func_02000044, 17,
};

s32 gData_020025f8[] = {
    22, 9, 0x9999, 22, 10,        0x9999, 22,
    26, 0, 23,     6,  -0x100000, 46,     (s32)Func_02000044,
    17,
};

// clang-format off
u32 gData_02002634[] = {
    39, 2, 22, 15, 0x10000, 22,
    16, 0x6666, 4, 0x1480000, 0, 0x2f00000,
    1, 4, 0x1680000, 0, 0x2f00000, 1,
    4, 0x1780000, 0, 0x3080000, 1, 4,
    0x1680000, 0, 0x3180000, 1, 4, 0x1480000,
    0, 0x3180000, 1, 4, 0x1380000, 0,
    0x3080000, 1, 13, 0xffff, 0,
};

u32 gData_020026d8[] = {
    39, 2, 22, 15, 0x10000, 22,
    16, 0x6666, 4, 0x1680000, 0, 0x3180000,
    1, 4, 0x1480000, 0, 0x3180000, 1,
    4, 0x1380000, 0, 0x3080000, 1, 4,
    0x1480000, 0, 0x2f00000, 1, 4, 0x1680000,
    0, 0x2f00000, 1, 4, 0x1780000, 0,
    0x3080000, 1, 13, 0xffff, 0,
};

u32 gData_0200277c[] = {
    39, 2, 4, 0x1500000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gData_020027c0[] = {
    39, 2, 4, 0x1600000, 0, 0x3180000,
    1, 39, 1, 0, 1, 22,
    3, 0x5000, 0, 10, 17,
};

u32 gData_02002804[] = {
    39, 2, 4, 0x13c0000, 0, 0x3780000,
    1, 4, 0x1500000, 0, 0x3780000, 1,
    4, 0x1580000, 0, 0x3680000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gData_02002870[] = {
    39, 2, 4, 0x16a0000, 0, 0x3180000,
    1, 4, 0x1800000, 0, 0x3500000, 1,
    4, 0x17e0000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gData_020028dc[] = {
    39, 2, 4, 0x16c0000, 0, 0x3280000,
    1, 4, 0x1760000, 0, 0x36e0000, 1,
    4, 0x1680000, 0, 0x3780000, 1, 39,
    1, 0, 1, 22, 3, 0x8000,
    0, 10, 17,
};

u32 gData_02002948[] = {
    39, 2, 4, 0x1120000, 0, 0x3780000,
    1, 4, 0x1480000, 0, 0x3780000, 1,
    39, 1, 22, 3, 0x8000, 0,
    10, 17,
};

u32 gData_02002998[] = {
    39, 2, 4, 0x1580000, 0, 0x3480000,
    1, 39, 1, 17,
};

u32 gData_020029c0[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x3580000, 1,
    39, 1, 0, 1, 17,
};

u32 gData_02002a04[] = {
    39, 2, 4, 0x1580000, 0, 0x3780000,
    1, 4, 0x1580000, 0, 0x36a0000, 1,
    39, 1, 0, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(139),
    SCENE_EXIT(1, 141, 1),
    SCENE_EXIT(2, 141, 2),
    SCENE_EXIT(3, 142, 1),
    SCENE_EXIT(4, 141, 4),
    SCENE_EXIT(5, 142, 3),
    SCENE_EXIT(6, 142, 2),
    SCENE_EXIT(7, 141, 3),
    SCENE_EXIT(8, 58, 4),
    SCENE_EXIT(9, 140, 6),
    SCENE_EXIT(10, 2, 34),
    SCENE_EXIT(11, 2, 33),
    SCENE_EXIT(12, 139, 15),
    SCENE_EXIT(13, 140, 7),
    SCENE_EXIT(15, 139, 12),
    SCENE_EXITS(140),
    SCENE_EXIT(6, 139, 9),
    SCENE_EXIT(7, 139, 13),
    SCENE_EXITS_END,
};

s32 gData_02002ac4[] = {
    (s32)0xffff0146,
    (s32)gData_020025d4,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0146,
    (s32)gData_020025a8,
    0xb80000,
    0,
    0xc80000,
    0x8000,
    (s32)0xffff0147,
    (s32)gData_020025f8,
    0x80000,
    0,
    0x80000,
    0x8000,
    (s32)0xffff0073,
    1,
    0x2680000,
    0,
    0xd80000,
    0x32000,
    (s32)0xffff0074,
    1,
    0x2c80000,
    0,
    0xe80000,
    0x14000,
    (s32)0xffff0072,
#if (LANGUAGE == LANGUAGE_JAPANESE)
    2,
    0x2280000,
    0,
    0xe80000,
#else
    3,
    0x2280000,
    0,
    0xf80000,
#endif
    0x16000,
    (s32)0xffff0080,
    2,
    0x1680000,
    0,
    0x1680000,
    0x1a000,
    (s32)0xffff0070,
    1,
    0x1c80000,
    0,
    0x1180000,
    0x12000,
    (s32)0xffff0071,
    1,
    0xd80000,
    0,
    0x1780000,
    0x1e000,
    (s32)0xffff00e2,
    1,
    0x1d00000,
    0,
    0x880000,
    0x16000,
    (s32)0xffff00e2,
    1,
    0x1880000,
    0,
    0x1080000,
    0x14000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct ActorPlacement gData_02002be4[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002bfc[] = {
    {136, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(872), FACING_WEST, 1, 0},
    {136, CONDITION_ALWAYS, (s32)gData_02002634, PIXELS(312), 0, PIXELS(776),
     FACING_NORTH, 2, 0},
    {138, CONDITION_ALWAYS, (s32)gData_020026d8, PIXELS(376), 0, PIXELS(776),
     FACING_SOUTH, 2, 0},
    {138, CONDITION_ALWAYS, 1, PIXELS(272), 0, PIXELS(800), 0xb000, 0, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

s16 gData_02002c8c[] = {
    93, 28, 1, 2, 6, 92, 28, 1,
    2, 6, -1,
};

s16 gData_02002ca2[] = {
    95, 28, 1, 2, 6, 94, 28, 1,
    2, 6, -1,
};

s16 gData_02002cb8[] = {
    89, 28, 1, 2, 6, 90, 28, 1,
    2, 6, -1,
};

s16 gData_02002cce[] = {
    92, 30, 2, 2, 6, 94, 30, 2,
    2, 6, -1,
};

s32 gData_02002ce4[] = {
    (s32)gData_02002c8c,
    0x130062,
    (s32)gData_02002ca2,
    0x14005c,
    (s32)gData_02002cb8,
    0x120054,
    (s32)gData_02002c8c,
    0x7004f,
    (s32)gData_02002cb8,
    0x100050,
    (s32)gData_02002cb8,
    0x11004d,
    (s32)gData_02002ca2,
    0x80063,
    (s32)gData_02002cce,
    0x6005a,
};

struct Trigger gData_02002d24[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 7, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_02000270)},
    {0xc401, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {0x4401, 13, CONDITION_ALWAYS, 13},
    {0x50008905, 20, CONDITION_ALWAYS, EVENT(Func_02000328)},
    {0x50008905, 21, CONDITION_ALWAYS, EVENT(Func_0200036c)},
    {0x50008905, 22, CONDITION_ALWAYS, EVENT(Func_0200038c)},
    {0x50008905, 23, CONDITION_ALWAYS, EVENT(Func_0200041c)},
    {0x50008905, 24, CONDITION_ALWAYS, EVENT(Func_0200043c)},
    {0x50008905, 25, CONDITION_ALWAYS, EVENT(Func_020004cc)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgYallamDoesnT)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgOurMasterBoatsman)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgIfYeppWere)},
    {0, 14, CONDITION_ALWAYS, MSGNO(MsgYeppFoundThis)},
    {0, 15, CONDITION_ALWAYS, MSGNO(MsgThereUsedTo)},
    {0, 16, CONDITION_ALWAYS, MSGNO(MsgWhenWeLost)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgYeppWasThe)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgYeppToldMe)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgYeppWasTheOne)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgSingingWithYepp)},
    {0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgThosePearlsYepp)},
    {0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgYeppNeverTold)},
    {0x8d15, 15, CONDITION_ALWAYS, MSGNO(MsgYeppWasAmazing)},
    {0x8d15, 16, CONDITION_ALWAYS, MSGNO(MsgSunshineIsA)},
    {0x8d15, 17, CONDITION_ALWAYS, MSGNO(MsgIVeSearched)},
    {0x8d15, 18, CONDITION_ALWAYS, MSGNO(MsgYeppAlwaysSang)},
    {0x10008e15, 8, 0x300, (s32)(Func_020022f4)},
    {0x8e15, 8, 0x300, EVENT(Func_020022f8)},
    {TRIGGERS_END},
};

struct Trigger gData_02002eec[] = {
    {1, 6, CONDITION_ALWAYS, 6},
    {0xc401, 7, CONDITION_ALWAYS, 7},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02002f10[] = {
    {1, 15, CONDITION_ALWAYS, 15},
    {0x400, 8, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x2400, 8, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0xe400, 8, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x4400, 8, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgYeppSSongs)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000584)},
    {0x8d15, 9, CONDITION_ALWAYS, EVENT(Func_02000584)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000614)},
    {0x8d15, 10, CONDITION_ALWAYS, EVENT(Func_02000614)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgYeppWroteAll)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgTheStoryIs)},
    {TRIGGERS_END},
};
