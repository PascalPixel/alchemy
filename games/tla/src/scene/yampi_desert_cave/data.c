#include "types.h"
#include "scene_tables.h"

void Func_02000cec(void);
void Func_02000d04(void);
void Func_02000d84(void);
void Func_02000dac(void);
void Func_02000de0(void);
void Func_02000e14(void);
void Func_02000e3c(void);
void Func_02000e68(void);
void Func_02000f10(void);
void Func_02001040(void);
void Func_02001120(void);
void Func_020011b4(void);
void Func_02001238(void);
void Func_0200130c(void);
void Func_020013c8(void);

extern u32 gData_02001b48[];

// clang-format off
u16 gData_02001968[] = {
    258, 259, 293, 294, 332, 331,
};

u32 gData_02001974[] = {
    0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 gData_0200198c[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
};

u32 gData_020019cc[] = {
    0x270065, 0, 0x230061, 0, 0x250063, 0,
    0x21005f, 0,
};

s16 gData_020019ec[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

s16 gData_020019fc[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02001a28[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gData_02001a54[] = {
    14, 9, 10, 11, 12, 13, 13, 14,
    9, 10, 11, 12, 12, 13, 14, 9,
    10, 11, 11, 12, 13, 14, 9, 10,
    10, 11, 12, 13, 14, 9, 9, 10,
    11, 12, 13, 14,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(79),
    SCENE_EXIT(1, 74, 4),
    SCENE_EXIT(2, 81, 1),
    SCENE_EXIT(3, 79, 6),
    SCENE_EXIT(4, 79, 5),
    SCENE_EXIT(5, 79, 4),
    SCENE_EXIT(6, 79, 3),
    SCENE_EXIT(7, 81, 8),
    SCENE_EXIT(8, 80, 1),
    SCENE_EXITS(80),
    SCENE_EXIT(1, 79, 8),
    SCENE_EXIT(2, 80, 17),
    SCENE_EXIT(14, 81, 13),
    SCENE_EXIT(15, 81, 12),
    SCENE_EXIT(16, 81, 11),
    SCENE_EXIT(17, 80, 2),
    SCENE_EXITS(81),
    SCENE_EXIT(1, 79, 2),
    SCENE_EXIT(2, 81, 3),
    SCENE_EXIT(3, 81, 2),
    SCENE_EXIT(4, 81, 7),
    SCENE_EXIT(5, 81, 6),
    SCENE_EXIT(6, 81, 5),
    SCENE_EXIT(7, 81, 4),
    SCENE_EXIT(8, 79, 7),
    SCENE_EXIT(9, 81, 10),
    SCENE_EXIT(10, 81, 9),
    SCENE_EXIT(11, 80, 16),
    SCENE_EXIT(12, 80, 15),
    SCENE_EXIT(13, 80, 14),
    SCENE_EXITS_END,
};

u32 gData_02001b48[] = {
    22, 26, 0, 17,
};

struct ActorPlacement gData_02001b58[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(888), 0, PIXELS(336), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(832), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(912), 0, PIXELS(168), FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(712), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct ActorPlacement gData_02001bd0[] = {
    {272, CONDITION_ALWAYS, (s32)gData_02001b48, PIXELS(552), 0, PIXELS(200),
     FACING_SOUTH, 2, 0},
    {177, 0x9cd, 1, PIXELS(200), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001c18[] = {
    {331, CONDITION_ALWAYS, 7, PIXELS(712), 0, PIXELS(752), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(752), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {331, CONDITION_ALWAYS, 7, PIXELS(728), 0, PIXELS(784), FACING_SOUTH, 2, 0},
    {332, CONDITION_ALWAYS, 7, PIXELS(800), 0, PIXELS(760), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gData_02001b48, PIXELS(632), 0, PIXELS(264),
     FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, (s32)gData_02001b48, PIXELS(136), 0, PIXELS(168),
     FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, (s32)gData_02001b48, PIXELS(744), 0, PIXELS(200),
     FACING_SOUTH, 2, 0},
    {404, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(632), FACING_SOUTH, 2, 0},
    {251, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {243, 0x41, 1, 0, 0, 0, FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

u32 gData_02001d20[] = {
    14,
    0xffff0206,
};

struct Trigger gData_02001d28[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {0x202, 42, CONDITION_ALWAYS, EVENT(Func_02000d84)},
    {0x50009705, 33, 0x300, EVENT(Func_02000e14)},
    {0x9c05, 30, CONDITION_ALWAYS, EVENT(Func_02000dac)},
    {0x9c05, 31, CONDITION_ALWAYS, EVENT(Func_02000de0)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001040)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_02001120)},
    {TRIGGERS_END},
};

struct Trigger gData_02001ddc[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {1, 14, CONDITION_ALWAYS, 14},
    {1, 15, CONDITION_ALWAYS, 15},
    {1, 16, CONDITION_ALWAYS, 16},
    {33, 17, CONDITION_ALWAYS, 17},
    {2, 52, 0x9cd, EVENT(Func_0200130c)},
    {0x10008c15, 8, 0x303, EVENT(Func_02000e68)},
    {0x8c15, 8, 0x303, EVENT(Func_02000f10)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001040)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_02001120)},
    {0x50008805, 100, 0x302, EVENT(Func_020013c8)},
    {TRIGGERS_END},
};

struct Trigger gData_02001e78[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {33, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02000d04)},
    {0, 17, CONDITION_ALWAYS, EVENT(Func_02000cec)},
    {0x202, 42, CONDITION_ALWAYS, EVENT(Func_02000d84)},
    {0x50009705, 33, 0x301, EVENT(Func_02000e3c)},
    {0x10008c15, 12, 0x305, EVENT(Func_02000e68)},
    {0x8c15, 12, 0x305, EVENT(Func_02000f10)},
    {0x10008c15, 13, 0x304, EVENT(Func_02000e68)},
    {0x8c15, 13, 0x304, EVENT(Func_02000f10)},
    {8, 0, CONDITION_ALWAYS, EVENT(Func_02000e68)},
    {9, 0, CONDITION_ALWAYS, EVENT(Func_02000f10)},
    {0x50008615, 14, 0x206, EVENT(Func_020011b4)},
    {0x8b85, 0, CONDITION_ALWAYS, EVENT(Func_02001040)},
    {0x40008b85, 0, CONDITION_ALWAYS, EVENT(Func_02001120)},
    {0x50008805, 33, 0x1270, EVENT(Func_02001238)},
    {TRIGGERS_END},
};
