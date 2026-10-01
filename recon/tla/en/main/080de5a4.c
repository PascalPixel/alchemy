/* Near miss: score 60. ⚓️ shifts life << 17 into r0 between loading and
   storing the middle coordinate, a load-delay fill that differs with the
   approved game flags. It would join
   UPDATE_ORBITING_PARTICLE_MAIN.C after the fade. */
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

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

void Vector_AddPolarOffset(s32 radius, s32 angle, struct OrbitingParticleVector *position);
void BattleFx_UpdateOrbitingParticleFade(void *object);
void Animation_ApplyChildValuesFar(struct OrbitingParticle *particle, s32 battle_mode);

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
