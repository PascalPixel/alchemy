#include "TYPES.H"
#include "RAM_BUFFER.H"
#include "MAP.H"
#include "CALLBACK_SCHEDULER.H"

void MapAnimation_Update(void);

void Map_EnableUpdateCallback(void)
{
    if (((struct MapViewWork *)Ram_HeapSlots->map_work)->animation_active == 0)
        Scheduler_EnableCallbacks(MapAnimation_Update);
}

void MapAnimation_Update(void);

void Map_DisableUpdateCallback(void)
{
    if (((struct MapViewWork *)Ram_HeapSlots->map_work)->animation_active == 0)
        Scheduler_DisableCallbacks(MapAnimation_Update);
}
