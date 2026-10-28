#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "task.h"

void MapAnimation_Update(void);

void Map_EnableUpdateCallback(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_SuspendAll(MapAnimation_Update);
}

void MapAnimation_Update(void);

void Map_DisableUpdateCallback(void) {
  if (((struct MapViewWork *)Ram_HeapSlots->mapWork)->animationActive == 0)
    Task_ResumeAll(MapAnimation_Update);
}
