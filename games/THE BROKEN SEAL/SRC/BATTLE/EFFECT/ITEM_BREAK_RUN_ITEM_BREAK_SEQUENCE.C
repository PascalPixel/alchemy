#include "TYPES.H"

struct BattleEffectScene {
    u8 pad00[16];
    void *volatile main_object;
};

extern struct BattleEffectScene *gEffectWork;

void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(void *object);
void Object_SetMode(void *object, s32 mode);
void WaitFrames(s32 frames);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

void BattleFx_RunItemBreakSequence(void)
{
    s32 work[3];
    struct BattleEffectScene *scene;
    void *object;

    scene = gEffectWork;
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
