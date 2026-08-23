#include "colosso_log_rolling_stage.h"

typedef struct StageActor {
    u8 filler00[8];
    s32 x; /* 16.16 position */
    u8 filler0c[4];
    s32 z;
} StageActor;

extern s16 Data_02000240[];
extern u8 *Data_03001ebc;

StageActor *Func_02004c36();

void ColossoLogRollingStage_NudgeActorsLeft(void)
{
    u8 *workspace = Data_03001ebc;
    s16 *table = Data_02000240;
    s32 id = *(s32 *)&table[250];
    StageActor *subject = *(StageActor **)(workspace + 480);
    StageActor *actor = Func_02004c36(id);
    s32 z = *(s16 *)((u8 *)actor + 0x12);

    /* Nudge both records left while the actor occupies rows 183 through 186. */
    if ((u32)(z - 183) <= 3) {
        subject->x += -0xcccc;
        actor->x += -0xcccc;
    }
}
