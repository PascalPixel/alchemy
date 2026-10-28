#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_02000054(void);
void Func_020000c8(void);
void Func_02000224(void);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(53),
    SCENE_EXIT_IF(1, 2, 10, -1),
    SCENE_EXIT_IF(2, 2, 11, -1),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {73, CONDITION_ALWAYS, 1, PIXELS(188), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {88, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(260), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {2, 10, 0x913, EVENT(Func_020000c8)},
    {2, 10, 0x916, EVENT(Func_02000224)},
    {0, 8, 0x913, EVENT(Func_02000054)},
    {0, 8, CONDITION_ALWAYS, MSGNO(MsgMadraMikasallaVillage)},
    {0, 9, 0x913, MSGNO(MsgMadraCanCrossWithPermission)},
    {0, 9, CONDITION_ALWAYS, MSGNO(MsgMadraAvoidYampiDesert)},
    {0x8d15, 8, 0x913, MSGNO(MsgMadraForTheirSafety)},
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgMadraWouldNeedBoat)},
    {0x8d15, 9, 0x913, MSGNO(MsgMadraElderReturn)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgMadraRatherStayHome)},
    {TRIGGERS_END},
};
