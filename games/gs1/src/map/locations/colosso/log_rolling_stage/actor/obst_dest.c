#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/obst_dest.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
} StageActor;

extern StageActor *Actor_Run(s32);
extern StageActor *Actor_Run2(s32);
extern StageActor *Actor_Run3(s32);

s32 Colosso_CheckObstacleDestination(s32 x, s32 z)
{
    StageActor *actor;

    if (Actor_Place(0, x, z) == 255) {
        return -2;
    }
    actor = Actor_Run(15);
    x = x >> 20;
    z = z >> 20;
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Actor_Run2(16);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    actor = Actor_Run3(17);
    if (actor->x >> 20 == x && actor->z >> 20 == z) {
        return -1;
    }
    return 0;
}
