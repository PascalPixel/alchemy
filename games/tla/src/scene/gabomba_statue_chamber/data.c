#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000184(void);
void Func_02000b60(void);
void Func_02000b70(void);
void Func_02000d3c(void);
void Func_02000e50(void);
void Func_02000e60(void);

extern s32 gData_02002048[];

// clang-format off
u32 gData_02001f18[] = {
    39, 2, 4, 0x1ac0000, 0, 0xea0000,
    1, 4, 0x1e80000, 0, 0xea0000, 1,
    17,
};

u32 gData_02001f4c[] = {
    39, 2, 4, 0x1680000, 0, 0xd80000,
    1, 39, 1, 3, 17,
};

u32 gData_02001f78[] = {
    23, 3, 0x800, 0, 1, 13,
    32, 0, 17,
};

s32 gData_02001f9c[] = {
    2, 0x1880000, 0x400000, 0x7c0000, 0, 1,
    (s32)0x80010000, 23, 6, -0x10000, 0, 1,
    13, 64, (s32)0xc0010000, 0, 1, 17,
};

u32 gData_02001fe4[] = {
    23, 6, 0x20000, 0, 1, 13,
    80, 0, 3, 17,
};

u32 gData_0200200c[] = {
    39, 2, 4, 0x1880000, 0, 0x940000,
    1, 4, 0x1880000, 0, 0xec0000, 1,
    39, 1, 17,
};

s32 gData_02002048[] = {
    22, 9, -0x10000, 17,
};

u32 gData_02002058[] = {
    0,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200a08c[] = {
    0x1001d0, 0x1e00090, 0xa00020, 0x2ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(120),
    SCENE_EXIT_IF(1, 119, 2, 0xffffffff),
    SCENE_EXIT_IF(2, 121, 1, 0xffffffff),
    SCENE_EXIT_IF(3, 111, 4, 0x86b),
    SCENE_EXIT_IF(3, 114, 4, 0xffffffff),
    SCENE_EXIT_IF(4, 122, 1, 0xffffffff),
    SCENE_EXITS(122),
    SCENE_EXIT(1, 120, 4),
    SCENE_EXIT(2, 124, 1),
    SCENE_EXITS(124),
    SCENE_EXIT(1, 122, 2),
    SCENE_EXIT(2, 126, 1),
    SCENE_EXIT(3, 112, 9),
    SCENE_EXITS_END,
};

u32 gData_020020f8[] = {
    0xffff0177, 1, 0x1880000, 0, 0x980000, 0x24000,
    0xffff0169, 1, 0x1480000, 0, 0x1000000, 0x20000,
    0xffff0169, 1, 0x1c80000, 0, 0x1000000, 0x1020000,
    0xffff0007, 1, 0, 0, 0, 0x24000,
    0xffff0038, 1, 0, 0, 0, 0x24000,
    0xffff0006, 1, 0, 0, 0, 0x24000,
    0xffff0005, 1, 0, 0, 0, 0x24000,
    0xffff001a, 1, 0, 0, 0, 0x24000,
    0xffff01e9, 1, 0, 0, 0, 0x24000,
    0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020021e8[] = {
    (s32)0xffff0177,
    1,
    0x1880000,
    0,
    0x580000,
    0x24000,
    (s32)0xffff0038,
    1,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff001a,
    1,
    0x1880000,
    0,
    0x880000,
    0x2d000,
    (s32)0xffff01e9,
    7,
    0x1880000,
    0,
    0x540000,
    0x24000,
    (s32)0xffff01e7,
    7,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff0151,
    7,
    0,
    0,
    0,
    0x24000,
    (s32)0xffff001b,
    1,
    0,
    0,
    0,
    0x2d000,
    (s32)0xffff014a,
    1,
    0x1500000,
    0,
    0x1080000,
    0x24000,
    (s32)0xffff014a,
    1,
    0x1600000,
    0,
    0x1080000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1700000,
    0,
    0x1080000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1a00000,
    0,
    0x1080000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1b00000,
    0,
    0x1080000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1c00000,
    0,
    0x1080000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1500000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1600000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1700000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1a00000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1b00000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1c00000,
    0,
    0x1480000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1500000,
    0,
    0x1880000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1600000,
    0,
    0x1880000,
    0x1024000,
    (s32)0xffff014a,
    1,
    0x1700000,
    0,
    0x1880000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1a00000,
    0,
    0x1880000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1b00000,
    0,
    0x1880000,
    0x1024000,
    (s32)0xffff014a,
    (s32)gData_02002048,
    0x1c00000,
    0,
    0x1880000,
    0x1024000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

u32 gData_02002458[] = {
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

s32 gData_02002470[] = {
    0xc602,
    (s32)0xffff0001,
    (s32)Func_02000b60,
    1,
    (s32)0xffff0003,
    3,
    1,
    (s32)0xffff0004,
    4,
    -1,
    0,
    0,
};

s32 gData_020024a0[] = {
    1,
    (s32)0xffff0001,
    1,
    1,
    (s32)0xffff0002,
    2,
    2,
    0x2060003,
    (s32)Func_02000d3c,
    2,
    (s32)0xffff0004,
    (s32)Func_02000e50,
    -1,
    0,
    0,
};

s32 gData_020024dc[] = {
    1,
    (s32)0xffff0001,
    1,
    514,
    (s32)0xffff0002,
    (s32)Func_02000b70,
    2,
    0x8fe000a,
    (s32)Func_02000e60,
    0,
    (s32)0xffff000a,
    MSGNO(MsgYourBlackOrb),
    0x8d15,
    (s32)0xffff000a,
    MSGNO(MsgOeiaWhatAm),
    0xc403,
    0x8ff000b,
    (s32)Func_02000184,
    -1,
    0,
    0,
};

s32 gData_02002530[] = {
    -1,
    0,
    0,
};
