#include "types.h"
#include "field_scene.h"

void Func_020006ac(void);
void Func_0200078c(void);
void OverlayObject_UpdateEveryFourFrames(void);
void SceneActor_ApplyCounterLowBitsAsMode(void);
void SceneActor_ClearSingleChildField(void);
void SceneState_ApplySixRectsAfter161(void);
void SceneState_ApplySixRectsAfterFlag161(void);
void VenusLighthouse_ReadRelief(void);
void VenusLighthouse_ReadTrueHeart(void);

extern u32 gData_02001ce4[];
extern u32 gData_02001d1c[];
extern u32 gData_02001d54[];
extern s32 gData_0200215c[];
extern s32 gData_02002180[];

// clang-format off
u32 gData_02001ce4[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

u32 gData_02001d1c[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

u32 gData_02001d54[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38,
};

s16 gData_02001d8c[] = {
    0, 16, 0, 16, 0, 16, 16, 0,
    16, 0, 16, 0, 16, 0, 0, -16,
    0, -16, 0, -16, 0, -16, -16, 0,
    -16, 0, -16, 0, -16, 0, 0, 16,
};
// clang-format on

s32 gData_02001dcc[] = {
    (s32)gData_02001ce4,
    (s32)gData_02001d1c,
    (s32)gData_02001d54,
};

s32 gData_02001dd8[] = {
    39,
    12,
    22,
    38,
    (s32)OverlayObject_UpdateEveryFourFrames,
    (s32)0x80010000,
    22,
    9,
    0x11999,
    22,
    10,
    0x11999,
    0,
    2,
    22,
    9,
    0xe666,
    22,
    10,
    0xe666,
    0,
    2,
    13,
    10,
    (s32)0xc0010000,
    22,
    9,
    0x10000,
    22,
    10,
    0x10000,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 120, 0, 200, FACING_NORTH, 0, 0, 56, 240, 216, 0 },
    { 2, CONDITION_ALWAYS, 120, 0, 104, FACING_SOUTH, 0, 0, 56, 240, 216, 0 },
    { 3, CONDITION_ALWAYS, 344, 0, 232, FACING_NORTH, 0, 256, 48, 512, 248, 0 },
    { 4, CONDITION_ALWAYS, 472, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 5, CONDITION_ALWAYS, 376, 0, 184, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 6, CONDITION_ALWAYS, 344, 0, 104, FACING_SOUTH, 0, 256, 48, 512, 248, 0 },
    { 7, CONDITION_ALWAYS, 840, 0, 248, FACING_NORTH, 0, 672, 32, 1008, 256, 0 },
    { 8, CONDITION_ALWAYS, 968, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 9, CONDITION_ALWAYS, 856, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 10, CONDITION_ALWAYS, 808, 0, 88, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 11, CONDITION_ALWAYS, 712, 0, 168, FACING_SOUTH, 0, 672, 32, 1008, 256, 0 },
    { 12, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { 13, CONDITION_ALWAYS, 104, 0, 504, FACING_NORTH, 0, 16, 288, 256, 520, 0 },
    { 14, CONDITION_ALWAYS, 104, 0, 472, FACING_SOUTH, 0, 16, 288, 256, 520, 0 },
    { 20, CONDITION_ALWAYS, 520, 0, 696, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { 21, CONDITION_ALWAYS, 344, 0, 568, FACING_SOUTH, 0, 304, 344, 576, 728, 0 },
    { 22, CONDITION_ALWAYS, 728, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 23, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 24, CONDITION_ALWAYS, 392, 0, 952, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { 25, CONDITION_ALWAYS, 392, 0, 856, FACING_SOUTH, 0, 272, 808, 512, 984, 0 },
    { 26, CONDITION_ALWAYS, 760, 0, 936, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 30, CONDITION_ALWAYS, 88, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { 31, CONDITION_ALWAYS, 184, 0, 872, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { 32, CONDITION_ALWAYS, 184, 0, 648, FACING_SOUTH, 0, 16, 600, 256, 920, 0 },
    { 98, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 600, 736, 840, 952, 0 },
    { 99, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 576, 736, -1, 968, 0 },
    { SCENE_TABLE_END },
};

u32 gSceneExits[] = {
    SCENE_EXITS(5),
    SCENE_EXIT(1, 4, 9),
    SCENE_EXIT(2, 5, 3),
    SCENE_EXIT(3, 5, 2),
    SCENE_EXIT(4, 5, 31),
    SCENE_EXIT(5, 5, 30),
    SCENE_EXIT(6, 5, 7),
    SCENE_EXIT(7, 5, 6),
    SCENE_EXIT(8, 5, 32),
    SCENE_EXIT(9, 5, 22),
    SCENE_EXIT(10, 5, 12),
    SCENE_EXIT(11, 5, 13),
    SCENE_EXIT(12, 5, 10),
    SCENE_EXIT(13, 5, 11),
    SCENE_EXIT(14, 5, 24),
    SCENE_EXIT(20, 5, 26),
    SCENE_EXIT(21, 5, 25),
    SCENE_EXIT(22, 5, 9),
    SCENE_EXIT(24, 5, 14),
    SCENE_EXIT(25, 5, 21),
    SCENE_EXIT(26, 5, 20),
    SCENE_EXIT(30, 5, 5),
    SCENE_EXIT(31, 5, 4),
    SCENE_EXIT(32, 5, 8),
    SCENE_EXITS_END,
};

s32 gData_0200215c[] = {
    22,
    35,
    0x2126,
    22,
    39,
    1,
    46,
    (s32)SceneActor_ApplyCounterLowBitsAsMode,
    17,
};

s32 gData_02002180[] = {
    22,
    9,
    0x9999,
    22,
    10,
    0x9999,
    22,
    30,
    0,
    46,
    (s32)SceneActor_ClearSingleChildField,
    17,
};

struct ScenePlacement gScenePlacements[] = {
    { 56, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0 },
    { 57, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_NORTH, 2, 0 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(664), 0, PIXELS(792), FACING_SOUTH, 2, 0 },
    { 229, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(12), FACING_SOUTH, 2, 0 },
    { 230, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(12), FACING_SOUTH, 2, 0 },
    { 229, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_EAST, 0, 0 },
    { 315, CONDITION_ALWAYS, 1, PIXELS(872), 0, PIXELS(120), FACING_NORTH, 2, 0 },
    { 315, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(104), FACING_NORTH, 2, 1 },
    { 452, CONDITION_ALWAYS, (s32)gData_02002180, PIXELS(488), 0, PIXELS(536), FACING_SOUTH, 2, 0 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(72), 0, PIXELS(104), FACING_SOUTH, 2, 0 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(72), 0, PIXELS(168), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(168), 0, PIXELS(104), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(168), 0, PIXELS(168), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(72), 0, PIXELS(344), FACING_SOUTH, 2, 0 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(104), 0, PIXELS(344), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(136), 0, PIXELS(344), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(168), 0, PIXELS(344), FACING_SOUTH, 2, 1 },
    { 336, CONDITION_ALWAYS, (s32)gData_0200215c, PIXELS(200), 0, PIXELS(344), FACING_SOUTH, 2, 1 },
    { SCENE_TABLE_END },
};

s32 gScenePlacementsEntrance99[] = {
    (s32)0xffff0004,
    1,
    0x80000,
    0,
    0x80000,
    0x2c000,
    (s32)0xffff0005,
    1,
    0x80000,
    0,
    0x80000,
    0x2c000,
    (s32)0xffff0038,
    1,
    0x80000,
    0,
    0x80000,
    0x2c000,
    (s32)0xffff0039,
    1,
    0x80000,
    0,
    0x80000,
    0x2c000,
    (s32)0xffff0150,
    (s32)gData_0200215c,
    0x2980000,
    0,
    0x3180000,
    0x24000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct SceneEvent gSceneEvents[] = {
    { 1, 1, CONDITION_ALWAYS, 1 },
    { 1, 2, CONDITION_ALWAYS, 2 },
    { 1, 3, CONDITION_ALWAYS, 3 },
    { 49, 4, CONDITION_ALWAYS, 4 },
    { 49, 5, CONDITION_ALWAYS, 5 },
    { 1, 6, CONDITION_ALWAYS, 6 },
    { 1, 7, CONDITION_ALWAYS, 7 },
    { 49, 8, CONDITION_ALWAYS, 8 },
    { 33, 9, CONDITION_ALWAYS, 9 },
    { 1, 10, CONDITION_ALWAYS, 10 },
    { 1, 11, CONDITION_ALWAYS, 11 },
    { 1, 12, CONDITION_ALWAYS, 12 },
    { 1, 13, CONDITION_ALWAYS, 13 },
    { 33, 14, CONDITION_ALWAYS, 14 },
    { 49, 20, CONDITION_ALWAYS, 20 },
    { 49, 21, CONDITION_ALWAYS, 21 },
    { 49, 22, CONDITION_ALWAYS, 22 },
    { 49, 23, CONDITION_ALWAYS, 23 },
    { 49, 24, CONDITION_ALWAYS, 24 },
    { 33, 25, CONDITION_ALWAYS, 25 },
    { 33, 26, CONDITION_ALWAYS, 26 },
    { 33, 30, CONDITION_ALWAYS, 30 },
    { 33, 31, CONDITION_ALWAYS, 31 },
    { 33, 32, CONDITION_ALWAYS, 32 },
    { 2, 45, 0x200, EVENT_SCRIPT(Func_020006ac) },
    { 2, 46, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200078c) },
    { 3, 35, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadRelief) },
    { 3, 36, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadRelief) },
    { 3, 37, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadRelief) },
    { 3, 38, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadRelief) },
    { 3, 39, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadRelief) },
    { 3, 40, CONDITION_ALWAYS, EVENT_SCRIPT(VenusLighthouse_ReadTrueHeart) },
    { 0x50009085, 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplySixRectsAfterFlag161) },
    { 0x40009085, 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplySixRectsAfter161) },
    { SCENE_EVENTS_END },
};
