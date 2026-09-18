#include "TYPES.H"
#define ADDR_030011E0 0x030011e0
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = (u32 *)ADDR_030011E0;
    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}
