#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_020003b0(void);
void Func_020003bc(void);
void Func_020003c8(void);
void Func_020003d4(void);
void Func_02000508(void);
void Func_02000544(void);
void Func_020005fc(void);
void Func_020009a4(void);
void Func_020009b0(void);
void Func_020009bc(void);
void Func_020009c8(void);
void Func_020009d4(void);
void Func_020009e0(void);
void Func_020009ec(void);
void Func_02000aec(void);
void Func_02000b4c(void);
void Func_02000bb0(void);
void Func_02000c1c(void);
void Func_02000c8c(void);
void Func_02000cf0(void);
void Func_02000d54(void);
void Func_02000db8(void);
void Func_02000e1c(void);
void Func_02000e7c(void);
void Func_02000ee0(void);
void Func_02000f44(void);
void Func_02000fa8(void);
void Func_02001034(void);
void Func_020010ac(void);
void Func_0200115c(void);

extern u32 gData_020013fc[];
extern s32 gData_02001418[];
extern s16 gData_020019d4[];

// clang-format off
s16 gData_0200134c[] = {
    9, 10, 11, 12, 13, 14, 15, -1,
};

s16 gData_0200135c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_02001388[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u16 gData_020013b4[] = {
    14, 9, 10, 11, 12, 13, 13, 14,
    9, 10, 11, 12, 12, 13, 14, 9,
    10, 11, 11, 12, 13, 14, 9, 10,
    10, 11, 12, 13, 14, 9, 9, 10,
    11, 12, 13, 14,
};

u32 gData_020013fc[] = {
    SC_SET(SW_SCALE_Y, 0x14ccc),
    SC_SET(SW_SCALE_X, 0x14ccc),
    SC_END,
};

s32 gData_02001418[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, PIXELS(-4)),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_CALL(Func_020003c8),
    SC_END,
};

s32 gData_02001454[] = {
    SC_CALL(Func_020003d4),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(136),
    SCENE_EXIT_IF(1, 135, 5, -1),
    SCENE_EXIT_IF(2, 136, 3, -1),
    SCENE_EXIT_IF(3, 136, 2, -1),
    SCENE_EXIT_IF(4, 136, 5, -1),
    SCENE_EXIT_IF(5, 136, 4, -1),
    SCENE_EXIT_IF(6, 136, 7, -1),
    SCENE_EXIT_IF(7, 136, 6, -1),
    SCENE_EXIT_IF(8, 137, 1, -1),
    SCENE_EXIT_IF(9, 136, 10, -1),
    SCENE_EXIT_IF(10, 136, 9, -1),
    SCENE_EXITS(137),
    SCENE_EXIT_IF(1, 136, 8, -1),
    SCENE_EXIT_IF(2, 137, 3, -1),
    SCENE_EXIT_IF(3, 137, 2, -1),
    SCENE_EXIT_IF(4, 137, 5, -1),
    SCENE_EXIT_IF(5, 137, 4, -1),
    SCENE_EXIT_IF(6, 137, 7, -1),
    SCENE_EXIT_IF(7, 137, 6, -1),
    SCENE_EXIT_IF(8, 137, 9, -1),
    SCENE_EXIT_IF(9, 137, 8, -1),
    SCENE_EXIT_IF(10, 138, 1, -1),
    SCENE_EXITS(138),
    SCENE_EXIT_IF(1, 137, 10, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_02001548[] = {
    {170, 0x9cf, (s32)gData_020013fc, PIXELS(184), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(152), 0, PIXELS(144), FACING_NORTH, 2, 0},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(216), 0, PIXELS(144), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(136), 0, PIXELS(176), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(232), 0, PIXELS(176), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(152), 0, PIXELS(224), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(216), 0, PIXELS(224), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(136), 0, PIXELS(256), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(232), 0, PIXELS(256), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(136), 0, PIXELS(304), FACING_NORTH, 2, 1},
    {306, CONDITION_ALWAYS, (s32)gData_02001418, PIXELS(232), 0, PIXELS(304), FACING_NORTH, 2, 1},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001668[] = {
    {298, CONDITION_ALWAYS, 7, PIXELS(168), 0, PIXELS(504), FACING_SOUTH, 2, 0},
    {298, CONDITION_ALWAYS, 7, PIXELS(344), 0, PIXELS(232), FACING_SOUTH, 2, 0},
    {298, CONDITION_ALWAYS, 7, PIXELS(776), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {298, CONDITION_ALWAYS, 7, PIXELS(872), 0, PIXELS(152), FACING_SOUTH, 2, 0},
    {298, CONDITION_ALWAYS, 7, PIXELS(776), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {298, CONDITION_ALWAYS, 7, PIXELS(872), 0, PIXELS(376), FACING_SOUTH, 2, 0},
    {297, CONDITION_ALWAYS, 7, PIXELS(824), 0, PIXELS(472), FACING_SOUTH, 2, 0},
    {243, 0x3c, 1, PIXELS(824), 0, PIXELS(88), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02001740[] = {
    {252, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {244, 0x55, 1, PIXELS(336), 0, PIXELS(184), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_02001788[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x21, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {0x602, 20, CONDITION_ALWAYS, EVENT(Func_02000aec)},
    {0x8602, 20, CONDITION_ALWAYS, EVENT(Func_02000b4c)},
    {0x602, 21, CONDITION_ALWAYS, EVENT(Func_02000bb0)},
    {0x8602, 21, CONDITION_ALWAYS, EVENT(Func_02000c1c)},
    {0x602, 22, CONDITION_ALWAYS, EVENT(Func_02000c8c)},
    {0x8602, 22, CONDITION_ALWAYS, EVENT(Func_02000cf0)},
    {0x602, 23, CONDITION_ALWAYS, EVENT(Func_02000d54)},
    {0x8602, 23, CONDITION_ALWAYS, EVENT(Func_02000db8)},
    {0x602, 24, CONDITION_ALWAYS, EVENT(Func_02000e1c)},
    {0x8602, 24, CONDITION_ALWAYS, EVENT(Func_02000e7c)},
    {0x602, 25, CONDITION_ALWAYS, EVENT(Func_02000ee0)},
    {0x8602, 25, CONDITION_ALWAYS, EVENT(Func_02000f44)},
    {0xc602, 26, CONDITION_ALWAYS, EVENT(Func_02000fa8)},
    {0x4602, 26, CONDITION_ALWAYS, EVENT(Func_02001034)},
    {2, 20, CONDITION_ALWAYS, EVENT(Func_020009a4)},
    {2, 21, CONDITION_ALWAYS, EVENT(Func_020009b0)},
    {2, 22, CONDITION_ALWAYS, EVENT(Func_020009bc)},
    {2, 23, CONDITION_ALWAYS, EVENT(Func_020009c8)},
    {2, 24, CONDITION_ALWAYS, EVENT(Func_020009d4)},
    {2, 25, CONDITION_ALWAYS, EVENT(Func_020009e0)},
    {2, 26, CONDITION_ALWAYS, EVENT(Func_020009ec)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_020003b0)},
    {0x9c05, 50, CONDITION_ALWAYS, EVENT(Func_02000508)},
    {0x9c05, 51, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {TRIGGERS_END},
};

struct Trigger gData_02001914[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 9, CONDITION_ALWAYS, 9},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {0xc602, 6, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {0xc602, 8, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {0xc602, 10, CONDITION_ALWAYS, EVENT(Func_020005fc)},
    {0x8715, 8, 0x230, EVENT(Func_0200115c)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020003bc)},
    {TRIGGERS_END},
};

struct Trigger gData_020019b0[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {2, 10, 0x9cf, EVENT(Func_020010ac)},
    {TRIGGERS_END},
};

s16 gData_020019d4[] = {
    72, 4, 2, 2, 2, 73, 4, 2,
    2, 2, -1, 0,
};
// clang-format on

s32 gData_020019ec[] = {
    (s32)gData_020019d4, 0x40048, (s32)gData_020019d4, 0x40054,
    (s32)gData_020019d4, 0x40060, (s32)gData_020019d4, 0x4006b,
    (s32)gData_020019d4, 0x40076,
};
