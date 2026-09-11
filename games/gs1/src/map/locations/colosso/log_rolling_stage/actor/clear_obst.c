#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/clear_obst.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x;
    u8 filler0C[4];
    s32 z;
    u8 filler14[0xf];
    u8 direction_and_kind;
    u8 filler24[0x31];
    u8 state;
} StageActor;

extern StageActor *Actor_Run(s32);

void Colosso_ActivateClearObstacleActors(void)
{
    StageActor *actor;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        actor = Actor_Run(slot);
        if (Actor_Place(0, actor->x, actor->z) == 0) {
            actor->direction_and_kind = 2;
            actor->state = 0;
            x = actor->x >> 20;
            z = actor->z >> 20;
            Actor_SetRect(83, 13, 1, 1, x, z);
            x2 = actor->x >> 20;
            z2 = actor->z >> 20;
            Actor_SetRect2(83, 13, 1, 1, x2, z2 + 52);
            Actor_Do(slot + 517);
        }
    }
}
