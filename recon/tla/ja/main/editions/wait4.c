#include "types.h"
#include "scene.h"
#include "system.h"
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
s32 BattleFx_RunSparkGroups(void *, s32);

void BattleFx_WaitThenSetField18To4(void *arg0) {
  unsigned int offset;
  BattleMotion_ApproachTargetFar(
      *((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
  WaitFrames(0x1D);
  offset = 0x18;
  *((s32 *)(((u8 *)arg0) + offset)) = 4;
  BattleFx_RunSparkGroups(arg0, 2);
}
