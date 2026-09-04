#include "types.h"

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *Data_03001f30;

void Func_08097384(void);
void *BattleEffect_StartItemBreak(void *object);
void Func_08098184(void *object);
void Func_08009080(void *object, s32 mode);
void WaitFrames(s32 frames);
void BattleEffect_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

#define BattleEffect_RunItemBreakSequence Func_0809802c
void BattleEffect_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = Data_03001f30;
    object = scene->main_object;

    do {
        Func_08097384();
    } while (0);
    object = BattleEffect_StartItemBreak(object);
    Func_08098184(object);
    if (object != 0) {
        Func_08009080(object, 4);
        WaitFrames(30);
    }
    BattleEffect_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}
