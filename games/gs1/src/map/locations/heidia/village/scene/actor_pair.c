#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/scene/actor_pair.h"

extern u8 gWork[];

void Scene_RunScene3bf(void)
{
    u32 i;
    s32 record;

    Map_Run(11, 1);
    Map_Run2();
    Map_Run3(60);
    Map_Do(0x247c);
    Map_Run4(13, 0);
    Map_Place(11, 0x10000, 0x8000);
    Map_Place2(15, 0x10000, 0x8000);
    Map_Place3(11, 0x1d8, 180);
    Map_Place4(15, 0x1d8, 180);
    Map_Run5(11, 1);
    Map_Run6(11);
    Map_Run7(11, 4);
    Map_Run8(30);
}
