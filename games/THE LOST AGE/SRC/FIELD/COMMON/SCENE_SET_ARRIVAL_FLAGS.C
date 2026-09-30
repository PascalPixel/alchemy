#include "TYPES.H"

void GameFlag_SetBit(s32 flag);
s32 GameFlag_Test(s32 flag);

/*
 * Story flags a scene of one family sets as its map is prepared: 0x113
 * always, and 0x161 and 0x144 once 0xa25 is set. The scenes pass 0, which
 * it ignores.
 */
void Scene_SetArrivalFlags(s32 unused)
{
    GameFlag_SetBit(0x113);
    if (GameFlag_Test(0xa25)) {
        GameFlag_SetBit(0x161);
        GameFlag_SetBit(0x144);
    }
}
