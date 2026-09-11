#include "types.h"
#include "scene.h"

void State_SetWorkHalfwordDc(s16 v)
{
    FIELD_AT_OFFSET(*(void **)0x03001F3C, s16 *, 0xDC) = v;
}
