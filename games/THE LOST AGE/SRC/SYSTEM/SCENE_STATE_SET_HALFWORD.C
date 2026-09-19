#include "TYPES.H"

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x02007522 = value;
}
