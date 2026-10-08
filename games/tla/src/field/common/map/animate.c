#include "types.h"
#include "ram_buffer.h"
#include "map.h"

void Map_ClearLayerEntryFlag(u32 no)
{
    struct MapWork *state = Ram_HeapSlots->map_work;
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 0;
}

void Map_SetLayerEntryFlag(u32 no)
{
    struct MapWork *state = Ram_HeapSlots->map_work;
    struct MapAnimation *anim = &state->anim[no];

    anim->paused = 1;
}
