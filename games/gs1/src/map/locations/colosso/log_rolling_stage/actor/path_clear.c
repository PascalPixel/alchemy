#include "scene.h"
#include "abi/map/locations/colosso/log_rolling_stage/actor/path_clear.h"
#include "colosso_log_rolling_stage.h"

s32 Colosso_CheckPathClearance(s32 x, s32 y)
{
    if (Actor_Apply(x, y - 0x180000) != 0
     || Actor_Apply2(x, y - 0x80000) != 0
     || Actor_Apply3(x, y + 0x80000) != 0
     || Actor_Apply4(x, y + 0x180000) != 0) {
        return -1;
    }
    return 0;
}
