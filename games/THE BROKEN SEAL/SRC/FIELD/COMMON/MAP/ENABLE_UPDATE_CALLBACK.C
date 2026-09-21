#include "MAP_RENDER_WORK.H"

s32 Func_080042c8(u32 value);
#define Scheduler_EnableCallbacks Func_080042c8
void Func_0801179c(void);

void Map_EnableUpdateCallback(void)
{
    if (Data_03001e70->active == 0)
        Scheduler_EnableCallbacks((u32)Func_0801179c);
}
