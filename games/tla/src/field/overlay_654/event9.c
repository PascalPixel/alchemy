#include "shrine_of_the_sea_god.h"

void Func_02000618(void)
{
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;
    struct KabeMapLayer *layer = &map->layers[0];

    layer->y += -0x10000;
    WaitFrames(4);
}
