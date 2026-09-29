#include "TYPES.H"
#include "DISPLAY_SCROLL.H"

void SceneState_SetHalfwordB030(u16 value)
{
    gScrollTarget = value;
}
