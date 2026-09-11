#include "types.h"
#include "scene.h"
#include "abi/map/locations/heidia/village/scene/status.h"

extern u8 gOv;

u8 *GetSoftFloatStatusStorage(void)
{
    return &gOv;
}
