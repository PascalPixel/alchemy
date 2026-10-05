#include "TYPES.H"
#include "OBJDISP.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "FRAME.H"
#include "OBJECT_DISPATCH.H"
#include "OBJECT_RUNTIME.H"

struct OrbitingParticle;

/* ☀️'s main orbit step, which in ⚓️ returns 0 to its dispatcher. */
s32 BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle)
{
    s32 phase = gFrameCount & 7;

    if (phase == 0) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 2);
    } else if (phase == 2) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)particle, 0);
    }
    return 0;
}

/* Shrinks a particle each frame and hands it to the common particle
   script once it is small enough. */
void BattleFx_UpdateOrbitingParticleFade(struct ObjectRuntime *object)
{
    s32 scale;

    if (object != NULL) {
        scale = object->scale_x - 0x1000;
        object->scale_y -= 0x1000;
        object->scale_x = scale;
        if (scale <= 0x1000)
            ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)BattleFx_CommonParticleScript);
    }
}
