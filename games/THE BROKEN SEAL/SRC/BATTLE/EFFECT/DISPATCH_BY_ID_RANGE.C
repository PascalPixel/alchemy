#include "TYPES.H"

void *Runtime_ReleaseHeapBlock(s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void *BattlePres_RunBeamSequence(s32 *);
void *Unnamed_080e40a4(s32 *);
void *FunctionHead_080e47b8(s32 *);
void BattleFx_DispatchByIdRange(s32 *arg0)
{
  s32 no;
  s32 tmp;
  tmp = (tmp = 0x60E);
  Runtime_AllocateBlock(0x29, tmp);
  Runtime_AllocateHeapBlock(0x27, 0x782C);
  Runtime_AllocateHeapBlock(0x28, 0x4000);
  tmp = *arg0;
  no = tmp;
  tmp = no - 0x64;
  if (((u32)tmp) <= 0x23U)
  {
    FunctionHead_080e47b8(arg0);
  } else
    if (no > 0xC7)
  {
    Unnamed_080e40a4(arg0);
  } else
  {
    BattlePres_RunBeamSequence(arg0);
  }
  Runtime_ReleaseHeapBlock(0x28);
  Runtime_ReleaseHeapBlock(0x27);
  Runtime_ReleaseHeapBlock(0x29);
}
