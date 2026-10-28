#include "types.h"
#include "scene_tables.h"

void Func_02000038(void);
void Func_020000a8(void);
void Func_02000108(void);
void Func_020001d8(void);

extern s32 gData_02000b80[];

s32 gData_02000b80[] = {
    46, (s32)Func_02000038, 39, 7, 17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 470, 0, 192, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(268),
    SCENE_EXIT_IF(1, 2, 60, EXIT_FLAG_NONE),
    SCENE_EXIT_IF(4, 2, 61, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {8, CONDITION_ALWAYS, (s32)gData_02000b80, PIXELS(120), 0, PIXELS(280),
     FACING_SOUTH, 2, 0},
    {263, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {285, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_EAST, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTHEAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 4, CONDITION_ALWAYS, 4},
    {2, 20, 0x190a, EVENT(Func_02000108)},
    {2, 21, 0x190a, EVENT(Func_020001d8)},
    {2, 22, 0x90a, EVENT(Func_020000a8)},
    {TRIGGERS_END},
};
