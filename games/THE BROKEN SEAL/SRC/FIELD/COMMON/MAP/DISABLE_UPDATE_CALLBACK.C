#include "MAP_RENDER_WORK.H"

s32 Func_0800439c(u32 value);
#define Scheduler_DisableCallbacks Func_0800439c
void Func_0801179c(void);

void Map_DisableUpdateCallback(void)
{
    if (Data_03001e70->active == 0)
        Scheduler_DisableCallbacks((u32)Func_0801179c);
}
