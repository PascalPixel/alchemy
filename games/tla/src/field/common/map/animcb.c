#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "callback_scheduler.h"

void MapAnimation_Update(void);

void Map_EnableUpdateCallback(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Scheduler_EnableCallbacks(MapAnimation_Update);
}

void MapAnimation_Update(void);

void Map_DisableUpdateCallback(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Scheduler_DisableCallbacks(MapAnimation_Update);
}
