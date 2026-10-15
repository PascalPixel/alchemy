/* Near miss, differ 10: as Func_02000568, the layer pointers and the 0x8000
 * constant are scheduled a few instructions away from the listing. */
#include "../../../../games/tla/src/field/overlay_654/shrine_of_the_sea_god.h"

void Func_0200087c(void)
{
    struct KabeMapWork *map = (struct KabeMapWork *)Ram_HeapSlots->map_work;
    struct KabeMapLayer *layer0 = &map->layers[0];
    struct KabeMapLayer *layer1 = &map->layers[1];
    struct KabeMapLayer *layer2 = &map->layers[2];
    struct KabeObject *leader = Object_GetById(gPartyState.current_owner);
    struct KabeObject *camera = Func_02003ad0();
    struct KabeObject *object8 = Object_GetById(8);
    struct KabeObject *object9 = Object_GetById(9);
    struct KabeObject *object10 = Object_GetById(10);

    layer0->y += 0x8000;
    layer1->y += -0x8000;
    layer2->y += -0x8000;
    leader->z += 0x8000;
    camera->y += 0x8000;
    object8->z += 0x8000;
    object9->z += 0x8000;
    object10->z += 0x8000;
    WaitFrames(4);
}
