#include "TYPES.H"
#include "DMA.H"

struct BattleSceneBuffers {
    u8 *scene;
    u8 unknown_04[16];
    u8 *work;
};

extern struct BattleSceneBuffers Data_03001ebc;
extern s32 Data_03001e40;
extern const s32 Data_080a0108[];
extern const u8 Value_00000149;
extern const u8 Value_0000014b;
extern const u8 Value_0000014d;
extern const u8 Value_0000014f;
s32 GameFlag_TestFar(s32);
void BattleFx_ApplyColorToTargetBuffer(s32, s32);
void BattleFx_StartBufferInterpolation(s32);

void BattleEffect_InitializeSharedScene(void)
{
    u8 *scene;
    u8 *work;
    s32 no;

    scene = Data_03001ebc.scene;
    work = Data_03001ebc.work;
    Dma_Set(work + 0x1340, scene + 0x776, 0x84000150, (volatile u32 *)0x040000d4);
    if (*(s16 *)(scene + 0xcb8) == 0)
        Dma_Set(work + 0xe00, scene + 0x236, 0x84000150, (volatile u32 *)0x040000d4);
    Dma_Set(work + 0xe00, work + 0x380, 0x840002a0, (volatile u32 *)0x040000d4);

    no = Data_03001e40 & 7;
    if (GameFlag_TestFar(0x148)) no = 0;
    if (GameFlag_TestFar((s32)&Value_00000149)) no = 1;
    if (GameFlag_TestFar(0x14a)) no = 2;
    if (GameFlag_TestFar((s32)&Value_0000014b)) no = 3;
    if (GameFlag_TestFar(0x14c)) no = 4;
    if (GameFlag_TestFar((s32)&Value_0000014d)) no = 5;
    if (GameFlag_TestFar(0x14e)) no = 6;
    if (GameFlag_TestFar((s32)&Value_0000014f)) no = 7;
    BattleFx_ApplyColorToTargetBuffer(Data_080a0108[no], 1);
    BattleFx_StartBufferInterpolation(8);
}
