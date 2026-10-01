#include "TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
#if defined(TLA_EDITION_JA)
void Func_08118068(s32, s16, s32, s32);
#else
void BattleMotion_ApproachTargetFar(s32, s16, s32, s32);
#endif
s32 BattleFx_RunSparkGroups(void *, s32);

/* battle/effects/wait_then_set_field.c */

void BattleFx_WaitThenSetField18To4(void *arg0)
{
  unsigned int ofs;
#if defined(TLA_EDITION_JA)
  /* Japanese effects use the preceding motion dispatch entry. */
  Func_08118068(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
#else
  BattleMotion_ApproachTargetFar(*((s32 *)(arg0 + 8)), *((s16 *)(((u8 *)arg0) + 0x24)), 0x18, 0xC3333);
#endif
  WaitFrames(0x1D);
  ofs = 0x18;
  *((s32 *)(((u8 *)arg0) + ofs)) = 4;
  BattleFx_RunSparkGroups(arg0, 2);
}
