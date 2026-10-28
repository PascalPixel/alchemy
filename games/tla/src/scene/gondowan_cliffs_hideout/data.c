#include "types.h"
#include "scene_tables.h"
#include "text/msg_ids.h"

void Func_02000074(void);

extern u32 gData_02000224[];

u32 gData_02000224[] = {
    39,
    0,
    17,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 194, 0, 215, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(104),
    SCENE_EXIT_IF(1, 102, 3, 0xffffffff),
    SCENE_EXITS_END,
};

s32 gData_02000270[] = {
    (s32)0xffff019a,
    (s32)gData_02000224,
    0xd80000,
    0,
    0xb80000,
    0x4000,
    (s32)0xffff0132,
    1,
    0xd80000,
    0,
    0xb80000,
    0x4000,
    (s32)0xffff0053,
    1,
    1,
    0,
    0,
    0x2008000,
    (s32)0xffff0053,
    1,
    2,
    0,
    0,
    0x200c000,
    (s32)0xffff0053,
    1,
    3,
    0,
    0,
    0x2004000,
    (s32)0xffff0053,
    1,
    4,
    0,
    0,
    0x2000000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

s32 gData_02000318[] = {
    (s32)0xffff019a,
    (s32)gData_02000224,
    0xd80000,
    0,
    0xb80000,
    0x4000,
    (s32)0xffff0132,
    1,
    0xd80000,
    0,
    0xb80000,
    0x4000,
    0xffff,
    0,
    0,
    0,
    0,
    0,
};

struct Trigger gSceneEvents[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {2, 10, 0x98f, EVENT_SCRIPT(Func_02000074)},
    {0, 10, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorIntroducesHimself)},
    {0, 11, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorsBlameContinentsCollision)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsClaimCamping)},
    {0, 13, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsFriendsComing)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsRegretMadraRaid)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsLostFood)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgKibomboWarriorsThreatenTown)},
    {0x8d15, 13, CONDITION_ALWAYS,
     MSGNO(MsgKibomboWarriorsWonderAboutPrisoners)},
    {TRIGGERS_END},
};
