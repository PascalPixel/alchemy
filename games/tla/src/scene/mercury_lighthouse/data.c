#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000038(void);

s32 gData_02000618[] = {
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_ADD(SW_Y, PIXELS(-2)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(30, SC_TOP),
    SC_LABEL(1),
    SC_ADD(SW_Y, PIXELS(-1)),
    SC_CALL(Func_02000038),
    SC_WAIT(1),
    SC_LOOP(40, SC_TO(1)),
    SC_FREE,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(309),      SCENE_EXIT(2, 309, 2), SCENE_EXIT(3, 297, 5),
    SCENE_EXIT(9, 309, 2), SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    {191, CONDITION_ALWAYS, 1, PIXELS(840), 0, PIXELS(696), FACING_SOUTH + FACING_STEP, 2, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(824), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, 2, 0},
    {190, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTHWEST + FACING_STEP, 2, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(906), 0, PIXELS(744), FACING_WEST, 2, 0},
    {194, CONDITION_ALWAYS, 1, PIXELS(782), 0, PIXELS(762), FACING_EAST, 2, 0},
    {3, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {1, CONDITION_ALWAYS, 1, PIXELS(904), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, PIXELS(780), 0, PIXELS(946), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(10), 0, PIXELS(10), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGERS_END},
};
