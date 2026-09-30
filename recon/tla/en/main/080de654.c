#include "TYPES.H"
#include "SCENE.H"
#include "OBJECT_EFX.H"
#include "FIXED_MATH.H"
#include "SYSTEM.H"
extern u8 Data_03001e40[];
void BattleEffect_InitializeSharedScene(void);
s32 BattleFx_RunEventAction(void *resource, s32 battle_mode, s32 size);

/* battle/effects/orbiting_particles/update_main.c */
struct OrbitingParticle;
void Animation_ApplyChildValuesFar(struct OrbitingParticle *particle, s32 battle_mode);

void BattleFx_StartOrbitingParticles(void)
{
    struct OrbitingParticleState *state = gEffectWork;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->battle_mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}
