#include "types.h"
#include "global_cells.h"

s32 WaitFrames(s32);
s32 Func_0800430c(void);
s32 Func_080043e0(void);
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void BattleRuntime_InitializeRenderObject(void);
void Func_080936a0(s32, s32);
void BattleEffect_UpdateObjectVisibilityBounds(void);

void BattleEffect_RunVisibilityTransition(void)
{
  if ((*(s16 *) (((u8 *) Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Func_0800430c();
    BattleEffect_UpdateObjectVisibilityBounds();
    BattleRuntime_InitializeRenderObject();
    Func_080936a0(0x9D89, 6);
    if ((*((volatile u32 *) ADDR_03001AE8)) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ADDR_03001AE8)) & 0x200);
    }
    Func_080936a0(0x10000, 6);
    Func_080043e0();
  }
}
