#include "TYPES.H"
extern u8 gBattleFxWork[];

void BattleFx_BeginCanvasLayer();
void BattleFx_EndCanvasLayer();
void BattleFx_PrepareCanvasEffect();

void ObjectGroup_StoreObjectAndRunStep7(s32 a0, s32 a1, s32 a2)
{
    u8 *p5;
    u8 slot12[4];
    u8 slot8[4];

    p5 = *(s32 *)((u32)gBattleFxWork);
    *(s32 *)(((s32)p5 + 0x7828)) = a0;
    BattleFx_BeginCanvasLayer(0);
    BattleFx_PrepareCanvasEffect(a0, 7, (*(s32 *)(*(s32 *)(((s32)p5 + 0x7828)) + 4) ^ 1), 0, slot12, slot8);
    BattleFx_EndCanvasLayer();
}
