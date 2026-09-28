#include "TYPES.H"
#include "SCENE.H"
s32 BattleFx_RunSparkGroups(s32, s32);

/* battle/effects/two_mode_b/run_mode0.c */
void BattleFx_RunTwoModeBMode0(s32 arg0)
{
    BattleFx_RunSparkGroups(arg0, 0);
}

/* battle/effects/two_mode_b/run_mode1.c */
void BattleFx_RunTwoModeBMode1(s32 arg0)
{
    BattleFx_RunSparkGroups(arg0, 1);
}
