#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/obj/cfg_grid.h"
#include "colosso_log_rolling_stage.h"

void Colosso_ConfigureGridRegion(void)
{
    Map_Do(0x360);
    {
        s32 width = 49;
        s32 height = 61;

        Map_SetRect(47, 61, 1, 4, width, height);
    }
}
