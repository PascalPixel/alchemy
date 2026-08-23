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

extern StageActor *Func_02005002(s32);
extern s32 Func_02004f26(s32, s32, s32);
extern void Func_02004f60(s32, s32, s32, s32, s32, s32);
extern void Func_02004f7a(s32, s32, s32, s32, s32, s32);
extern void Func_02004ffa(s32);

void ColossoLogRollingStage_ActivateClearObstacleActors(void)
{
    StageActor *actor;
    s32 slot;
    s32 x;
    s32 z;
    s32 x2;
    s32 z2;

    for (slot = 15; slot <= 17; slot++) {
        actor = Func_02005002(slot);
        if (Func_02004f26(0, actor->x, actor->z) == 0) {
            actor->direction_and_kind = 2;
            actor->state = 0;
            x = actor->x >> 20;
            z = actor->z >> 20;
            Func_02004f60(83, 13, 1, 1, x, z);
            x2 = actor->x >> 20;
            z2 = actor->z >> 20;
            Func_02004f7a(83, 13, 1, 1, x2, z2 + 52);
            Func_02004ffa(slot + 517);
        }
    }
}
