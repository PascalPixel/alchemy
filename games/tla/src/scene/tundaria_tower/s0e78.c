#include "tundaria_tower.h"

void Func_02002fd0(void);

void Func_02000e78(void)
{
    Func_02002fd0();
    GameFlag_SetBit(0x212);
    if ((u16)(gPartyState.entrance - 6) <= 1)
    {
        Func_02002fc0(43, 100, 53, 100, 3, 3);
        Call6((void (*)())Func_02002fe8, 43, 37, 3, 3, 53, 37);
        if (Object_GetById(gPartyState.currentOwner)->z >> 20 > 38)
            MapCell_SetAttributes(0, 54, 37, 0x1f00);
        else
            MapCell_SetAttributes(0, 54, 39, 0x1f00);
    }
}
