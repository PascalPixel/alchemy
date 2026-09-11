#include "types.h"
#include "scene.h"
#include "abi/battle/effects/set/dispatch_by_id_range.h"

void *Battle_Run(s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void *Battle_Run2(s32 *);
void *Battle_Run3(s32 *);
void *Battle_Run4(s32 *);
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
    Battle_Run4(arg0);
  } else
    if (no > 0xC7)
  {
    Battle_Run3(arg0);
  } else
  {
    Battle_Run2(arg0);
  }
  Battle_Run(0x28);
  Battle_Run(0x27);
  Battle_Run(0x29);
}
