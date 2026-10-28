#include "types.h"
#include "scene_tables.h"
#include "objcmd.h"

void Func_02000054(void);
void Func_02000074(void);
void Func_02000094(void);
void Func_020000f8(void);
void Func_02000118(void);
void Func_02000124(void);

extern u32 gData_020021c8[];
extern u32 gData_02002204[];
extern u32 gData_02002240[];

// clang-format off
u32 gData_020021c8[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002204[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_WAIT(1),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
    SC_END,
};

u32 gData_02002240[] = {
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
    SCENE_EXITS(252),
    SCENE_EXIT(1, 253, 1),
    SCENE_EXIT(2, 252, 3),
    SCENE_EXIT(3, 252, 2),
    SCENE_EXIT(4, 251, 4),
    SCENE_EXIT(6, 254, 6),
    SCENE_EXITS(253),
    SCENE_EXIT(1, 252, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {SCENE_TABLE_END},
};

struct Trigger gData_020022e8[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {0x50008905, 30, CONDITION_ALWAYS, EVENT(Func_02000054)},
    {0x50008905, 31, CONDITION_ALWAYS, EVENT(Func_02000074)},
    {0x50008905, 32, CONDITION_ALWAYS, EVENT(Func_020000f8)},
    {0x50008905, 33, CONDITION_ALWAYS, EVENT(Func_02000118)},
    {0x50008905, 34, CONDITION_ALWAYS, EVENT(Func_02000124)},
    {0x50008905, 38, CONDITION_ALWAYS, EVENT(Func_02000094)},
    {TRIGGERS_END},
};

struct Trigger gData_0200236c[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {TRIGGERS_END},
};

s32 gData_02002384[] = {
    -1,
};

u32 gData_02002388[] = {
    1,
};

u32 gData_0200238c[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gData_020023a0[] = {
    (s32)gData_020021c8,
    (s32)gData_02002204,
    (s32)gData_02002240,
};
