#include "types.h"
#include "scene.h"
#include "abi/owner/action/check_level_threshold.h"


s32 OwnerAction_CheckLevelThreshold(void)
{
    return Sys_Check();
}
