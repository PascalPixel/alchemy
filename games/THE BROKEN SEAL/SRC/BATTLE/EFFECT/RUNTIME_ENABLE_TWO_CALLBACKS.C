#include "TYPES.H"
#include "SCENE.H"
s32 Scheduler_EnableCallbacks(u32 value);

extern u8 DisplayTransition_UpdateScanlineTable;
extern u8 BattleFx_StartWindowHBlankDma;

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)&DisplayTransition_UpdateScanlineTable);
    Scheduler_EnableCallbacks((u32)&BattleFx_StartWindowHBlankDma);
}
