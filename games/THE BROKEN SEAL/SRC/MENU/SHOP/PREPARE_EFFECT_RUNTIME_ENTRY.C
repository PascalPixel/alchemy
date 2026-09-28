#include "TYPES.H"
#include "GLOBAL_CELLS.H"
extern u8 Data_03001ebc[];

s32 BattleFx_ApplyColorToTargetBufferFar(s32, s32);
s32 BattleFx_StartBufferInterpolationFar(s32);

void Shop_InitEffect(void)
{
    BattleFx_ApplyColorToTargetBufferFar(*(s32 *)((u32)&Data_03001ebc) + 0x236, 1);
    BattleFx_StartBufferInterpolationFar(0x10);
}
