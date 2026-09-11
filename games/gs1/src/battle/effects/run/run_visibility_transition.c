#include "types.h"
#include "scene.h"
#include "abi/battle/effects/run/run_visibility_transition.h"
#include "global_cells.h"

s32 WaitFrames(s32);

void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Battle_InitializeRenderObject(void);

void BattleFx_UpdateObjectVisibilityBounds(void);

void BattleFx_RunVisibilityTransition(void)
{
  if ((*(s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Battle_Check();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    Battle_Apply(0x9D89, 6);
    if ((*((volatile u32 *) ADDR_03001AE8)) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ADDR_03001AE8)) & 0x200);
    }
    Battle_Apply(0x10000, 6);
    Battle_Check2();
  }
}
