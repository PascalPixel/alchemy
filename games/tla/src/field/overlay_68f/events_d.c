#include "ov68f.h"

void Func_0200137c(void)
{
    if ((Object_GetById(gPartyState.current_owner)->y >> 19) != 0)
        MapCell_SetAttributes(0, 14, 17, 0);
    else
        MapCell_SetAttributes(0, 14, 17, 0x3200);
}
