#include "local.h"
#include "map.h"

void Func_02000e90(void)
{
    struct MapLayer *layer = &Ram_HeapSlots->map_work->layers[2];
    s16 y = layer->y >> 16;

    /* FAKEMATCH: a one-pass block builds the store's address before 1448 (one reordered row), as in overlay 679. */
    do {
        Data_02002ce0 = 1448 - y;
    } while (0);
    Data_02002ce4 = layer->x >> 16;
    Data_02002ce6 = (s16)(layer->x >> 16) - (gFrameCount >> 2);
}

void Func_02000ed0(void)
{
    s32 priority;

    Func_02001fa8(1, 0, Func_02000e30);
    priority = 0xc80;
    Scheduler_AddOrUpdateCallback(Func_02000e90, priority);
}

void Func_02000ef0(void)
{
    Func_02002190(Data_0200226c);
}

void Func_02000f00(void)
{
    Object_GetById(gPartyState.current_owner)->priority_flags |= 2;
    Func_02002190(Data_0200226c);
}
