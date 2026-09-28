#include "TYPES.H"
#include "SCENE.H"
s32 Scheduler_EnableCallbacks(u32 value);

extern u8 Func_0808f52c;
extern u8 BattleFx_StartWindowHBlankDma;

void BattleFx_EnableTwoCallbacks(void)
{
    Scheduler_EnableCallbacks((u32)&Func_0808f52c);
    Scheduler_EnableCallbacks((u32)&BattleFx_StartWindowHBlankDma);
}
