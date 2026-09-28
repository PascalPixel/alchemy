#include "TYPES.H"

s32 BattleFx_BeginTiledCanvas(s32);
s32 BattleFx_EndCanvasLayer();

void BattleFx_InitTilemapAndFlushQueue(void)
{
    BattleFx_BeginTiledCanvas(1);
    BattleFx_EndCanvasLayer();
}
