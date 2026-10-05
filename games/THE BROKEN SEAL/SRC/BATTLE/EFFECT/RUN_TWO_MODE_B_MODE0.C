#include "TYPES.H"
#include "SCENE.H"
s32 BattleFx_RunSparkGroups(s32, s32);

void BattleFx_RunTwoModeBMode0(s32 arg0)
{
    BattleFx_RunSparkGroups(arg0, 0);
}

void BattleFx_RunTwoModeBMode1(s32 arg0)
{
    BattleFx_RunSparkGroups(arg0, 1);
}
