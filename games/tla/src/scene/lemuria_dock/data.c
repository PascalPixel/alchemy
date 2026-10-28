#include "types.h"
#include "scene_tables.h"
#include "edition.h"

void Func_0200006c(void);
void Func_02000158(void);
void Func_020002dc(void);

extern s16 gData_02000604[];

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s32 gData_0200057c[] = {
    200, 0x101030c7, -1, 0x102020c9, -1, 0x10301028,
    -1, 511,
};

s32 gData_0200059c[] = {
    200, 0x101050c7, -1, 0x102050c9, -1, 0x10301028,
    -1, 511,
};

struct ActorPlacement gActorPlacements[] = {
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
#if (LANGUAGE != LANGUAGE_JAPANESE)
    {420, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(392), FACING_EAST, 2, 0},
#endif
    {SCENE_TABLE_END},
};

s16 gData_02000604[] = {
    51, 38, 1, 1, 6, -1,
};
// clang-format on

s32 gData_02000610[] = {
    (s32)gData_02000604,
    0x160046,
};

struct Trigger gSceneEvents[] = {
    {1, 2, CONDITION_ALWAYS, 2},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Func_020002dc)},
    {2, 21, CONDITION_ALWAYS, EVENT(Func_0200006c)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000158)},
    {0x8d15, 1032, CONDITION_ALWAYS, EVENT(Func_02000158)},
    {TRIGGERS_END},
};
