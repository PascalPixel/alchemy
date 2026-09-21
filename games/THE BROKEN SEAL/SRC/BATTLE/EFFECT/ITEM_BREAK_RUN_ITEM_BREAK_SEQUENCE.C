#include "TYPES.H"

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *Data_03001f30;

void Func_08097384(void);
#define BattleEffect_InitializeSharedScene Func_08097384
void *Func_08098070(void *object);
#define BattleFx_StartItemBreak Func_08098070
void Func_08098184(void *object);
#define BattleFx_SnapScaleToFull Func_08098184
void Func_08009080(void *object, s32 mode);
#define Object_SetMode Func_08009080
void WaitFrames(s32 frames);
void Func_0809748c(void);
#define BattleFx_PrepareBufferInterpolation Func_0809748c
void UpdateRisingParticleBurst(void *object);

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = Data_03001f30;
    object = scene->main_object;

    do {
        BattleEffect_InitializeSharedScene();
    } while (0);
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}
