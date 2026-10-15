#include "types.h"

void GameFlag_SetBit(s32 flag);
s32  GameFlag_Test(s32 flag);

void Scene_SetArrivalFlags(s32 unused)
{
    GameFlag_SetBit(0x113);
    if (GameFlag_Test(0xa25))
    {
        GameFlag_SetBit(0x161);
        GameFlag_SetBit(0x144);
    }
}
