#include "types.h"
#include "global_cells.h"

#define Camera_StoreSceneParameters Func_08005258

void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = (u32 *)ADDR_03001CE0;

    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}
