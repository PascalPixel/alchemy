#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
} StageActor;

extern s32 Func_02004fea(s32, s32, s32);
extern StageActor *Func_020050da(s32);
extern StageActor *Func_020050f4(s32);
extern StageActor *Func_0200510a(s32);

s32 ColossoLogRollingStage_CheckObstacleDestination(s32 x, s32 z)
{
    StageActor *actor;

    if (Func_02004fea(0, x, z) == 255) {
        return -2;
    }
    actor = Func_020050da(15);
    x = x >> 20;
    z = z >> 20;
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Func_020050f4(16);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Func_0200510a(17);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    return 0;
}
