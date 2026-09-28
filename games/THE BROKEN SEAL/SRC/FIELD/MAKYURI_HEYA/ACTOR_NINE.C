#include "PROBE.H"

void SceneActor_UseActorNinePositionWithYOffset(void)
{
    s32 *p = Actor_Get(9);
    u32 v = Random_Next();

    s32 b = p[3] + (((v << 2) >> 16) << 16);
    s32 c = p[4];

    SceneEffect_SpawnRandomEveryFourFrames(p[2], b, c);
}

s32 SceneData_LoadBlockA2c5(void)
{
    s32 n = 0xc80;

    Engine_TaskAddCallback((s32)SceneActor_UseActorNinePositionWithYOffset, n);
    return 0;
}

s32 SceneData_ApplyTableA2c5AndReturnZero(void)
{
    Engine_TaskRemoveCallback((s32)SceneActor_UseActorNinePositionWithYOffset);
    return 0;
}
