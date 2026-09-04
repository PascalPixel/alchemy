#include "map_render_work.h"

#define Map_EnableUpdateCallback Func_08011984

s32 Func_080042c8(u32 value);
void Func_0801179c(void);

void Map_EnableUpdateCallback(void)
{
    if (Data_03001e70->active == 0)
        Func_080042c8((u32)Func_0801179c);
}
