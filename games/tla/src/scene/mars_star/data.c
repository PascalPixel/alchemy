#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000038(void);
void Func_02000060(void);
void Func_02000074(void);
void Func_02000188(void);
void Func_020001e8(void);
void Func_0200024c(void);
void Func_020005b0(void);
void Func_02000ba4(void);
void Func_02000bd8(void);
void Func_02000e50(void);
void Func_02000e80(void);
void Func_0200236c(void);
void Func_02002c44(void);
void Func_02002ce8(void);

void Func_0200007c(void);

extern u32 gData_0200485c[];
extern u32 gData_02004898[];
extern u32 gData_020048d4[];
extern s32 gData_02004910[];

// clang-format off
u32 gData_02004474[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020044b8[] = {
    46,
    (s32)Func_02002c44,
    4,
    0x2680000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2680000,
    0,
    0xf80000,
    1,
    0,
    2,
    4,
    0x2380000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x2380000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2400000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x2500000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_02000188,
    46,
    (s32)Func_02002ce8,
    17,
};

s32 gData_020045b8[] = {
    46,
    (s32)Func_02002c44,
    4,
    0x2a80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x2a80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1280000,
    1,
    0,
    2,
    4,
    0x2d80000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0x1080000,
    1,
    0,
    2,
    4,
    0x3280000,
    0,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3280000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x180000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xf00000,
    1,
    0,
    2,
    4,
    0x3180000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x200000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3100000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    4,
    0x3000000,
    0x160000,
    0xd80000,
    1,
    0,
    2,
    22,
    37,
    (s32)Func_020001e8,
    46,
    (s32)Func_02002ce8,
    17,
};

u32 gData_02004744[] = {
    0, 30, 22, 38, 0, 2,
    0, 0, 0, 0, 1, 38,
};

u32 gData_02004774[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_020047b8[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s32 gData_020047fc[] = {
    0xffff, -0x8000, (s32)0xe000c000, (s32)0xc000a000, 0x20004000, 0x40006000,
    0xffff, -0x8000, 0xffff, -0x8000, -0x4000, (s32)0xc000ffff,
    -0xc000, 0x4000ffff, 0xffff, -0x8000,
};

s32 gData_0200483c[] = {
    0xffff, -0x8000, 0xc000, (s32)0xc0008000, 0x4000, 0x40008000,
    0xffff, -0x8000,
};

u32 gData_0200485c[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02004898[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_020048d4[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

s32 gData_02004910[] = {
    46,
    (s32)Func_02000038,
    17,
};

s32 gData_0200491c[] = {
    23, 9, 0x400, 23, 10, 0x400,
    23, 6, -0x4000, 0, 1, 13,
    40, 0, 17,
};

s32 gData_02004958[] = {
    23, 9, -2048, 23, 10, -2048,
    23, 6, 0x4000, 0, 1, 13,
    24, 0, 3, 0, 1, 17,
};

s32 gData_020049a0[] = {
    23, 9, -1024, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};

s32 gData_020049dc[] = {
    23, 9, 0x400, 23, 10, -1024,
    0, 1, 13, 40, 0, 3,
    0, 1, 17,
};
// clang-format on

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS(286),
    SCENE_EXIT(1, 286, 2),
    SCENE_EXIT(2, 286, 1),
    SCENE_EXIT(3, 286, 4),
    SCENE_EXIT(4, 286, 3),
    SCENE_EXIT(5, 285, 5),
    SCENE_EXIT(6, 284, 6),
    SCENE_EXIT(7, 285, 7),
    SCENE_EXIT(8, 284, 8),
    SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
    { 418, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(712), 0, PIXELS(264), FACING_SOUTH, 2, 0 },
    { 419, CONDITION_ALWAYS, (s32)gData_02004910, PIXELS(648), 0, PIXELS(264), FACING_SOUTH, 2, 0 },
    { 291, CONDITION_ALWAYS, 7, PIXELS(680), 0, PIXELS(240), FACING_SOUTH, 2, 0 },
    { 181, 0xa30, 7, PIXELS(664), 0, PIXELS(216), FACING_SOUTH, 2, 0 },
    { 181, 0xa30, 7, PIXELS(696), 0, PIXELS(216), FACING_SOUTH, 2, 1 },
    { 33, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 23, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 303, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 1 },
    { 673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { 412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gSceneEvents[] = {
    { 1, 1, CONDITION_ALWAYS, 1 },
    { 81, 2, CONDITION_ALWAYS, 2 },
    { 81, 3, CONDITION_ALWAYS, 3 },
    { 1, 4, CONDITION_ALWAYS, 4 },
    { 33, 5, CONDITION_ALWAYS, 5 },
    { 33, 6, CONDITION_ALWAYS, 6 },
    { 33, 7, CONDITION_ALWAYS, 7 },
    { 33, 8, CONDITION_ALWAYS, 8 },
    { 33, 9, CONDITION_ALWAYS, 9 },
    { 1, 10, CONDITION_ALWAYS, 10 },
    { 1, 11, CONDITION_ALWAYS, 11 },
    { 2, 90, 0x1a4b, EVENT_SCRIPT(Func_0200236c) },
    { 0x10009a15, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000060) },
    { 0x60009a15, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000074) },
    { 0x20009a15, 9, CONDITION_ALWAYS, (u32)(Func_0200007c) },
    { 6, 20, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020005b0) },
    { 0x50009705, 20, 0x1233, EVENT_SCRIPT(Func_0200024c) },
    { 0, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000ba4) },
    { 0x8d15, 14, CONDITION_ALWAYS, MSGNO(MsgMarsStar05) },
    { 0, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000bd8) },
    { 0x8d15, 13, CONDITION_ALWAYS, MSGNO(MsgMarsStar04) },
    { 3, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000e50) },
    { 0xde04, 1045, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000e80) },
    { SCENE_EVENTS_END },
};

s32 gData_02004ce0[] = {
    (s32)gData_0200485c,
    (s32)gData_02004898,
    (s32)gData_020048d4,
};
