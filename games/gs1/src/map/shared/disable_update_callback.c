#include "map_render_work.h"

#define Map_DisableUpdateCallback Func_080119a8

s32 Func_0800439c(u32 value);
void Func_0801179c(void);

void Map_DisableUpdateCallback(void)
{
    if (Data_03001e70->active == 0)
        Func_0800439c((u32)Func_0801179c);
}
