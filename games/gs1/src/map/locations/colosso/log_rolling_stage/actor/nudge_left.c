#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/nudge_left.h"
#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x; /* 16.16 position */
    u8 filler0c[4];
    s32 z;
} StageActor;

extern s16 gCell[];
extern u8 *gWork;

StageActor *Actor_Run();

void Colosso_NudgeActorsLeft(void)
{
    u8 *workspace = gWork;
    s16 *table = gCell;
    s32 id = *(s32 *)&table[250];
    StageActor *subject = *(StageActor **)(workspace + 480);
    StageActor *actor = Actor_Run(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}
