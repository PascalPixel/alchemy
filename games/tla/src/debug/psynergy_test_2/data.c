#include "types.h"
#include "scene_tables.h"

void Func_02000054(void);
void Func_02000064(void);
void Func_0200007c(void);
void Func_02000088(void);
void Func_020000b8(void);
void Func_020000d8(void);

void Func_02000078(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 408, 0, 88, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {1, CONDITION_ALWAYS, 408, 0, 408, FACING_NORTH, 0, 280, 0, 780, 464, 0},
    {2, CONDITION_ALWAYS, 504, 0, 232, FACING_SOUTH, 0, 280, 0, 780, 464, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(322),
    SCENE_EXIT(20, 322, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {306, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {406, CONDITION_ALWAYS, 1, PIXELS(504), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {407, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(136), FACING_SOUTH, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020000d8)},
    {2, 30, CONDITION_ALWAYS, EVENT(Func_020000b8)},
    {0x2115, 8, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008615, 8, CONDITION_ALWAYS, EVENT(Func_02000064)},
    {0x8715, 9, CONDITION_ALWAYS, (u32)(Func_02000078)},
    {0x9815, 9, CONDITION_ALWAYS, EVENT(Func_0200007c)},
    {0x8515, 10, CONDITION_ALWAYS, 0},
    {0x50009705, 50, CONDITION_ALWAYS, EVENT(Func_02000088)},
    {TRIGGERS_END},
};
