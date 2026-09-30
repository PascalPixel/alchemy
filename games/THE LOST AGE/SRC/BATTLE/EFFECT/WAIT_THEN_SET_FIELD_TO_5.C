#include "TYPES.H"

s32 WaitFrames(s32);
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

struct SparkObject {
    u8 padding_00[8];
    s32 target;
    u8 padding_0c[0xc];
    s32 phase;
    u8 padding_1c[8];
    s16 angle;
};

/* ⚓️ adds this step between ☀️'s two: a longer approach, then phase 5. */
void BattleFx_WaitThenSetField18To5(struct SparkObject *obj)
{
    BattleMotion_ApproachTargetFar(obj->target, obj->angle, 0x18, 0xc3333);
    WaitFrames(26);
    obj->phase = 5;
    BattleFx_RunSparkGroups(obj, 2);
}
