#include "TYPES.H"
#include "CALL.H"

s32 Engine_ActorGet();
void SetMapCellCollision();

void SoruSekizo_CheckTileTrigger016A4(void)
{
    s32 x = *(s32 *)(Engine_ActorGet(0) + 8) >> 20;
    s32 y = *(s32 *)(Engine_ActorGet(0) + 16) >> 20;

    if (y == 7 && (u32)(x - 21) <= 1) {
        Call4(SetMapCellCollision, 2, 0x1600000, 0x700000, 255);
    }
}
