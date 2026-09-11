#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/cfg_13.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
} StageActor;

extern StageActor *Actor_Run(s32);

void Colosso_ConfigureActorThirteen(void)
{
    StageActor *actor;
    s32 x;

    actor = Actor_Run(13);
    x = actor->x >> 20;
    Actor_Apply(880, x);
    Actor_SetRect(18, 10, 3, 1, 18, 11);
    Actor_SetRect2(17, 11, 1, 1, x, 11);
}
