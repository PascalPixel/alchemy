#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"

/* battle/effects/wait_then_set_field.c */

void BattleFx_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
  BattleMotion_ApproachTargetFar(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *)(((u8 *)arg0) + ofs)) = 4;
  BattleFx_RunSparkGroups(arg0, 2);
}
