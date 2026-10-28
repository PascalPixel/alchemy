#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_020001b4(void);
void Func_020001d4(void);
void Func_020001f4(void);

extern u32 gData_02002050[];
extern u32 gData_0200208c[];
extern u32 gData_020020c8[];

// clang-format off
u32 gData_02002050[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_0200208c[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_020020c8[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(254),
    SCENE_EXIT(1, 255, 1),
    SCENE_EXIT(2, 251, 2),
    SCENE_EXIT(3, 251, 3),
    SCENE_EXIT(4, 254, 5),
    SCENE_EXIT(5, 254, 4),
    SCENE_EXIT(6, 252, 6),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(744), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(744), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(536), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {2, 20, 0xa01, EVENT(Func_020001b4)},
    {2, 21, 0xa01, EVENT(Func_020001d4)},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x8c15, 10, 0xa01, EVENT(Func_020001f4)},
    {TRIGGERS_END},
};

s32 gData_0200222c[] = {
    -1,
};

u32 gData_02002230[] = {
    1,
};

u32 gData_02002234[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_02002248[] = {
    (s32)gData_02002050,
    (s32)gData_0200208c,
    (s32)gData_020020c8,
};
