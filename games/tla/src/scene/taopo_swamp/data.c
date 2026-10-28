#include "types.h"
#include "scene_tables.h"
#include "edition.h"

void Func_02000038(void);
void Func_02000104(void);
void Func_02000118(void);
void Func_0200013c(void);
void Func_020002f4(void);
void Func_02000304(void);
void Func_02000314(void);
void Func_02000334(void);
void Func_020003c4(void);
void Func_02000428(void);
void Func_02000464(void);
void Func_020004c8(void);
void Func_0200065c(void);
void Func_02000744(void);
void Func_02000784(void);
void Func_020007bc(void);
void Func_02000920(void);
void Func_0200096c(void);
void Func_02000adc(void);
void Func_02000afc(void);
void Func_02000b58(void);
void Func_02000b90(void);
void Func_02000d2c(void);
void Func_02000f9c(void);
void Func_02000fbc(void);
void Func_02001018(void);
void Func_02001038(void);
void Func_020010bc(void);
void Func_020010c4(void);
void Func_02001100(void);
void Func_02001128(void);
void Func_02001138(void);
void Func_02001228(void);
void Func_02001564(void);
void Func_0200166c(void);
void Func_02002144(void);
void Func_020022ac(void);
void Func_02002334(void);

u32 gData_020029b8[] = {
    0x6090200,
    0x1050604,
};

u32 gData_020029c0[] = {
    0x10003,
    1,
};

// clang-format off
u32 gData_020029c8[] = {
    22, 26, 0, 39, 2, 0x80010000,
    23, 6, 0x10000, 0, 1, 13,
    30, 0xc0010000, 38,
};

u32 gData_02002a04[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02002a48[] = {
    0, 30, 38,
};

s32 gData_02002a54[] = {
    46,
    (s32)Func_02001228,
    38,
};

s32 gData_02002a60[] = {
    46,
    (s32)Func_02001564,
    38,
};

s32 gData_02002a6c[] = {
    46,
    (s32)Func_0200166c,
    38,
};

u16 gData_02002a78[] = {
    200, 200, 4, 264, 200, 4, 232, 216,
    4, 280, 216, 4, 568, 200, 4, 632,
    200, 4, 536, 216, 4, 600, 216, 4,
    408, 232, 4, 472, 232, 4, 0,
};

u16 gData_02002ab6[] = {
    9, 10, 0,
};

u16 gData_02002abc[] = {
    424, 104, 4, 536, 136, 4, 0,
};

u16 gData_02002aca[] = {
    408, 408, 4, 472, 360, 4, 0,
};

u16 gData_02002ad8[] = {
    216, 184, 4, 312, 136, 4, 0, 0,
};

u32 gData_02002ae8[] = {
    0, 32, 38,
};

u32 gData_02002af4[] = {
    0, 44, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(143),
    SCENE_EXIT(1, 2, 35),
    SCENE_EXIT(2, 144, 1),
    SCENE_EXIT(3, 144, 2),
    SCENE_EXITS(144),
    SCENE_EXIT(1, 143, 2),
    SCENE_EXIT(2, 143, 3),
    SCENE_EXIT(3, 145, 3),
    SCENE_EXIT(4, 148, 4),
    SCENE_EXIT(5, 145, 5),
    SCENE_EXITS(145),
    SCENE_EXIT(3, 144, 3),
    SCENE_EXIT(5, 144, 5),
    SCENE_EXIT(6, 147, 2),
    SCENE_EXIT(7, 147, 3),
    SCENE_EXIT(8, 145, 9),
    SCENE_EXIT(9, 145, 8),
    SCENE_EXITS(146),
    SCENE_EXIT(1, 147, 1),
    SCENE_EXIT(2, 147, 4),
    SCENE_EXITS(147),
    SCENE_EXIT(1, 146, 1),
    SCENE_EXIT(2, 145, 6),
    SCENE_EXIT(3, 145, 7),
    SCENE_EXIT(4, 146, 2),
    SCENE_EXIT(10, 146, 10),
    SCENE_EXIT(11, 146, 11),
    SCENE_EXIT(12, 146, 12),
    SCENE_EXIT(13, 146, 13),
    SCENE_EXIT(14, 146, 14),
    SCENE_EXIT(15, 146, 15),
    SCENE_EXIT(16, 146, 16),
    SCENE_EXITS(148),
    SCENE_EXIT(1, 149, 1),
    SCENE_EXIT(4, 144, 4),
    SCENE_EXITS(149),
    SCENE_EXIT(1, 148, 1),
    SCENE_EXIT(2, 150, 2),
    SCENE_EXIT(3, 150, 3),
    SCENE_EXIT(4, 149, 5),
    SCENE_EXIT(5, 149, 4),
    SCENE_EXIT(10, 150, 9),
    SCENE_EXIT(12, 150, 9),
    SCENE_EXITS(150),
    SCENE_EXIT(1, 152, 1),
    SCENE_EXIT(2, 149, 2),
    SCENE_EXIT(3, 149, 3),
    SCENE_EXIT(4, 150, 5),
    SCENE_EXIT(5, 150, 4),
    SCENE_EXITS(152),
    SCENE_EXIT(1, 150, 1),
    SCENE_EXIT(2, 153, 2),
    SCENE_EXIT(3, 151, 3),
    SCENE_EXIT(4, 151, 4),
    SCENE_EXITS(151),
    SCENE_EXIT(3, 152, 3),
    SCENE_EXIT(4, 152, 4),
    SCENE_EXITS(153),
    SCENE_EXIT(2, 152, 2),
    SCENE_EXIT(3, 153, 4),
    SCENE_EXIT(4, 153, 3),
    SCENE_EXIT(5, 153, 6),
    SCENE_EXIT(6, 153, 5),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02002c54[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002c6c[] = {
    {307, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(128), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(112), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(112), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(176), FACING_NORTH, 2, 0},
    {307, CONDITION_ALWAYS, 1, PIXELS(712), 0, PIXELS(256), FACING_NORTH, 2, 0},
    {299, CONDITION_ALWAYS, 1, PIXELS(760), 0, PIXELS(272), FACING_NORTH, 2, 0},
    {273, 0x8e5, 1, PIXELS(216), 0, PIXELS(264), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002d2c[] = {
    {243, 0x3b, 1, PIXELS(440), 0, PIXELS(632), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002d5c[] = {
    {243, 0x3b, 1, PIXELS(408), 0, PIXELS(312), FACING_SOUTH, 0, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(232), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002dbc[] = {
    {290, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(136), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(424), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002e04[] = {
    {407, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(200), FACING_NORTH, 2, 0},
    {290, CONDITION_ALWAYS, 1, PIXELS(408), 0, PIXELS(440), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002e64[] = {
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(296), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(312), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(552), 0, PIXELS(360), FACING_NORTH, 2, 1},
    {272, CONDITION_ALWAYS, 1, PIXELS(456), 0, PIXELS(376), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(320), FACING_NORTH, 2, 0},
    {328, CONDITION_ALWAYS, 1, PIXELS(416), 0, PIXELS(336), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(368), 0, PIXELS(352), FACING_NORTH, 2, 1},
    {328, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002f9c[] = {
    {292, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(360), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(216), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02002fe4[] = {
    {292, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(632), FACING_NORTH, 2, 0},
    {292, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(632), FACING_NORTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_0200302c[] = {
    {TRIGGERS_END},
};

struct Trigger gData_02003038[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {TRIGGERS_END},
};

struct Trigger gData_02003068[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {0xce01, 3, 0x18e0, 3},
    {0xce01, 4, 0x18e3, 4},
    {0xce01, 5, 0x18e4, 5},
    {2, 60, CONDITION_ALWAYS, EVENT(Func_02002144)},
    {2, 61, CONDITION_ALWAYS, EVENT(Func_020022ac)},
    {2, 62, CONDITION_ALWAYS, EVENT(Func_02002334)},
    {0x4e15, 8, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0x4e15, 9, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0x4e15, 10, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0x4e15, 11, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0x4e15, 12, CONDITION_ALWAYS, EVENT(Func_02000104)},
    {0xc15, 13, CONDITION_ALWAYS, EVENT(Func_02000118)},
    {0x8c15, 14, CONDITION_ALWAYS, EVENT(Func_0200013c)},
    {TRIGGERS_END},
};

struct Trigger gData_02003128[] = {
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 5, CONDITION_ALWAYS, 5},
    {49, 6, CONDITION_ALWAYS, 6},
    {33, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {33, 9, CONDITION_ALWAYS, 9},
    {0x50008805, 51, 0x301, EVENT(Func_020002f4)},
    {0x50008805, 50, 0x300, EVENT(Func_02000304)},
    {TRIGGERS_END},
};

struct Trigger gData_02003194[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000038)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000314)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000334)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020003c4)},
    {TRIGGERS_END},
};

struct Trigger gData_020031e8[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {49, 4, CONDITION_ALWAYS, 4},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 11, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 12, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 13, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 14, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 15, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {0x4602, 16, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 12, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 13, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 14, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 15, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 16, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 17, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 18, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {0x8c15, 9, 0xa6d, EVENT(Func_02000464)},
    {0x8c15, 10, 0xa6e, EVENT(Func_02000464)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000464)},
    {6, 100, 0x310, EVENT(Func_0200065c)},
    {TRIGGERS_END},
};

struct Trigger gData_02003314[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {1, 4, CONDITION_ALWAYS, 4},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gData_02003338[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {49, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {0x4602, 10, CONDITION_ALWAYS, EVENT(Func_02000428)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 12, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {2, 11, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {0x8c15, 9, 0xa6f, EVENT(Func_02000744)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000744)},
    {0x50002115, 8, 0x220, EVENT(Func_02000784)},
    {0x1815, 8, 0x1222, EVENT(Func_020007bc)},
    {TRIGGERS_END},
};

struct Trigger gData_020033e0[] = {
    {49, 1, CONDITION_ALWAYS, 1},
    {33, 2, CONDITION_ALWAYS, 2},
    {33, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_020004c8)},
    {0x8515, 8, 0x308, 0},
    {0x50002115, 10, 0x220, EVENT(Func_02000920)},
    {0x1815, 10, 0x1222, EVENT(Func_0200096c)},
    {TRIGGERS_END},
};

u32 gData_02003458[] = {
    112,
};

u32 gData_0200345c[] = {
    120,
};

u32 gData_02003460[] = {
    24,
};

struct Trigger gData_02003464[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0x2115, 9, 0x331, EVENT(Func_02000adc)},
    {0x2115, 10, 0x334, EVENT(Func_02000b58)},
    {0x8c15, 9, 0x332, EVENT(Func_02000afc)},
    {0x8c15, 10, 0x335, EVENT(Func_02000b90)},
    {9, 0, 0x335, EVENT(Func_02000b90)},
    {0x8715, 11, 0x330, EVENT(Func_02000d2c)},
    {TRIGGERS_END},
};

s32 gData_020034e8[] = {
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    2,
    (s32)0xffff000a,
    (s32)Func_020004c8,
    2,
    (s32)0xffff000b,
    (s32)Func_020004c8,
#if (LANGUAGE != LANGUAGE_JAPANESE) /* Absent from the Japanese edition. */
    0x602,
    (s32)0xffff0040,
    (s32)Func_020010bc,
#endif
    0x2115,
    0x3380008,
    (s32)Func_02000f9c,
    0x2115,
    0x33b0009,
    (s32)Func_02001018,
    0x8c15,
    0x3390008,
    (s32)Func_02000fbc,
    0x8c15,
    0x33c0009,
    (s32)Func_02001038,
    -1,
    0,
    0,
};

struct Trigger gData_02003560[] = {
    {33, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {83, 50, 0x348, EVENT(Func_02001138)},
    {0x2115, 8, 0x340, EVENT(Func_020010c4)},
    {0x2115, 9, 0x342, EVENT(Func_02001100)},
    {0x8c15, 8, 0x341, 0},
    {0x8c15, 9, 0x343, 0},
    {0x50008805, 50, 0x348, EVENT(Func_02001128)},
    {TRIGGERS_END},
};

u32 gData_020035f0[] = {
    0,
};
