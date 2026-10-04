#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"

/* The battle effects' work, as far as the item break reads it. */
struct BattleFxScene {
    u8 unknown_00[16];
    void *volatile main_object;
};

void BattleEffect_InitializeSharedScene(void);
void *BattleFx_StartItemBreak(void *object);
void BattleFx_SnapScaleToFull(void *object);
void Object_SetMode(void *object, s32 mode);
void BattleFx_PrepareBufferInterpolation(void);
void UpdateRisingParticleBurst(void *object);

/* The item break: shatter the main object into fragments, snap it back to
   full scale, hold it for half a second, then lift and burst it. */
void BattleFx_RunItemBreakSequence(void)
{
    /* FAKEMATCH: an unused 12-byte local explains the native 12-byte frame. */
    s32 work[3];
    struct BattleFxScene *scene;
    void *object;

    scene = Ram_HeapSlots->effect_work;
    object = scene->main_object;

    /* FAKEMATCH: removing this one-pass block changes instruction scheduling, as in ☀️. */
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
