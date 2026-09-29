#include "TYPES.H"
extern u32 gCameraSceneParameters[3];
void Camera_StoreSceneParameters(u32 value0, u32 value1, u32 value2)
{
    u32 *work = gCameraSceneParameters;
    work[0] = value0;
    work[1] = value1;
    work[2] = value2;
}
