#include "TYPES.H"

void ObjectEffect_PrepareContextEffect(s32 effect);
void GameFlag_SetBit(s32 flag);

/* Prepares context effect 26 and raises flag 0x120, which
   ObjectEffect_RunPendingFlagEvent answers. */
void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_SetBit(0x120);
}
