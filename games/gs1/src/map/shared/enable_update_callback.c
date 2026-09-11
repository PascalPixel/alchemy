#include "scene.h"
#include "abi/map/shared/enable_update_callback.h"
#include "map_render_work.h"

void Map_EnableUpdateCallback(void)
{
    if (gCam->active == 0)
        Map_Check((u32)Map_Run);
}
