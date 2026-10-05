#include "PROTO.H"
#include "TYPES.H"
#include "SYSTEM.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"

struct BattleFxScene {
    u8 unknown_00[16];
    void *volatile main_object;
};


void BattleFx_RunItemBreakSequence(void)
{
    /* FAKEMATCH: an unused 12-byte local explains the native 12-byte frame. */
    s32 work[3];
    struct BattleFxScene *scene;
    void *object;

    scene = Ram_HeapSlots->effect_work;
    object = scene->main_object;

    BattleEffect_InitializeSharedScene();
    object = BattleFx_StartItemBreak(object);
    BattleFx_SnapScaleToFull(object);
    if (object != 0) {
        Object_SetMode(object, 4);
        WaitFrames(30);
    }
    BattleFx_PrepareBufferInterpolation();
    UpdateRisingParticleBurst(object);
}
