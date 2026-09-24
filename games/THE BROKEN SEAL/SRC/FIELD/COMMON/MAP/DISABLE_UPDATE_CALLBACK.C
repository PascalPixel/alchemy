#include "MAP_RENDER_WORK.H"

s32 Scheduler_DisableCallbacks(u32 value);
void MapAnimation_Update(void);

void Map_DisableUpdateCallback(void)
{
    if (Data_03001e70->active == 0)
        Scheduler_DisableCallbacks((u32)MapAnimation_Update);
}
