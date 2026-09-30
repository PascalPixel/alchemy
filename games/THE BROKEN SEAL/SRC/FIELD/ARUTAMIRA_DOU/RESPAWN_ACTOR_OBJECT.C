#include "DMA.H"
#include "TYPES.H"
extern u8 gEffectWork[];

s32 Engine_ObjectCreate();
void Animation_ApplyChildValues();
void Battle_WaitMode0();
void Engine_ActorFaceDirection();

void ArutamiraDou_RespawnActorObject(void)
{
    s32 rec6;
    s32 record;
    u8 *p6;
    u8 *p5;

    p6 = *(s32 *)gEffectWork;
    p5 = *(s32 *)((s32)p6 + 16);
    Engine_ActorFaceDirection(*(s16 *)((s32)p6 + 24), 0x4000, 0);
    Animation_ApplyChildValues((s32)p5, 0);
    Battle_WaitMode0(20);
    rec6 = Engine_ObjectCreate(0, *(s32 *)((s32)p5 + 8), *(s32 *)((s32)p5 + 12), *(s32 *)((s32)p5 + 16));
    if (rec6 != 0) {
        Dma_Set((const void *)((s32)p5), (void *)(rec6), -0x7bffffe4, (volatile u32 *)(0x40000d4));
        *(s32 *)((s32)p5 + 108) = 0;
        *(s32 *)((s32)p6 + 16) = rec6;
        p5[84] = 0;
    }
}
