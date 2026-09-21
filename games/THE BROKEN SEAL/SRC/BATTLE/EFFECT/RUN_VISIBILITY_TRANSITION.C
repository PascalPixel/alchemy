#include "TYPES.H"
#include "GLOBAL_CELLS.H"

s32 WaitFrames(s32);
s32 Func_0800430c(void);
#define Scheduler_EnableUnmaskedOverlayCallbacks Func_0800430c
s32 Func_080043e0(void);
#define Scheduler_DisableOverlayCallbacks Func_080043e0
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);
void Func_08091660(void);
#define Battle_InitializeRenderObject Func_08091660
void Func_080936a0(s32, s32);
#define BattleFx_ScheduleRatioTransition Func_080936a0
void Func_0809c314(void);
#define BattleFx_UpdateObjectVisibilityBounds Func_0809c314

void BattleFx_RunVisibilityTransition(void)
{
  if ((*(s16 *)(((u8 *)Runtime_AllocateBlock(0x1B, 0xCCC)) + 0x19E)) == 3)
  {
    Scheduler_EnableUnmaskedOverlayCallbacks();
    BattleFx_UpdateObjectVisibilityBounds();
    Battle_InitializeRenderObject();
    BattleFx_ScheduleRatioTransition(0x9D89, 6);
    if ((*((volatile u32 *) ADDR_03001AE8)) & 0x200)
    {
      do
      {
        WaitFrames(1);
      }
      while ((*((volatile u32 *) ADDR_03001AE8)) & 0x200);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_DisableOverlayCallbacks();
  }
}
