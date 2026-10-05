#include "TYPES.H"
#include "OBJDISP.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
#include "FRAME.H"
#include "OBJECT_DISPATCH.H"
#include "OBJECT_RUNTIME.H"
#include "FX_SCENE.H"
#include "RAM_BUFFER.H"

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

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, struct OrbitingParticleVector *pos);

/* Counts the particle's life down, placing it on a shrinking orbit around its
   centre that turns one way; at zero it fades. */
void BattleFx_UpdateOrbitingParticleLeft(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 life;
    s32 cnt;

    if (arg != 0) {
        cnt = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = cnt;
        life = (s16)cnt;
        if (life != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(life << 17,
                          *(s16 *)(arg + 102) + (life << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

/* The same orbit turning the other way. */
void BattleFx_UpdateOrbitingParticleRight(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 battle_value;
    s32 raw_value;

    if (arg != 0) {
        raw_value = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw_value;
        battle_value = (s16)raw_value;
        if (battle_value != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Vector_AddPolarOffset(battle_value << 17,
                          *(s16 *)(arg + 102) - (battle_value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleFx_UpdateOrbitingParticleFade;
        }
    }
}

struct OrbitingParticleChild {
    u8 reserved_00[35];
    u8 flags;
};

void BattleFx_RunOrbitingParticles(void);

/* Starts the orbit: enables the scene when its child runs a mode, marks
   the child and runs the particles. */
void BattleFx_StartOrbitingParticles(void)
{
    struct BattleFxScene *state = Ram_HeapSlots->effect_work;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->child_mode != 0) {
            state->enabled = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}
