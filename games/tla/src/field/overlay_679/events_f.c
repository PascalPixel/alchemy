#include "local.h"
#include "map.h"

void Func_02000f58(void)
{
    struct MapLayer *layer = &Ram_HeapSlots->map_work->layers[2];
    s16 y = layer->y >> 16;

    /* FAKEMATCH: a one-pass block builds the store's address before 1448 (one reordered row). */
    do {
        Data_02002a98 = 1448 - y;
    } while (0);
    Data_02002a9c = layer->x >> 16;
    Data_02002a9e = (s16)(layer->x >> 16) - (gFrameCount >> 2);
}

void Func_02000f98(void)
{
    s32 priority;

    Func_02001c0c(1, 0, Func_02000ef8);
    priority = 0xc80;
    Scheduler_AddOrUpdateCallback(Func_02000f58, priority);
}
