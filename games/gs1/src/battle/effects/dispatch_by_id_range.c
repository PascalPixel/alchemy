#include "types.h"

#define BattleEffect_DispatchByIdRange Func_080e3a3c

void *Func_08002dd8(s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void *Func_080e3aa0(s32 *);
void *Func_080e40a4(s32 *);
void *Func_080e47b8(s32 *);
void BattleEffect_DispatchByIdRange(s32 *arg0)
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
  if (((u32) tmp) <= 0x23U)
  {
    Func_080e47b8(arg0);
  }
  else
    if (no > 0xC7)
  {
    Func_080e40a4(arg0);
  }
  else
  {
    Func_080e3aa0(arg0);
  }
  Func_08002dd8(0x28);
  Func_08002dd8(0x27);
  Func_08002dd8(0x29);
}
