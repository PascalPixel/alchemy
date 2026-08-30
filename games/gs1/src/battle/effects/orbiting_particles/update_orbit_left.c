#include "types.h"

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle;

void RotateVectorByMagnitude(s32, s32, struct OrbitingParticleVector *);
void BattleEffect_UpdateOrbitingParticleFade(void *object);

void BattleEffect_UpdateOrbitingParticleLeft(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 remaining_lifetime;
    s32 lifetime_counter;

    if (arg != 0) {
        lifetime_counter = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = lifetime_counter;
        remaining_lifetime = (s16)lifetime_counter;
        if (remaining_lifetime != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            RotateVectorByMagnitude(remaining_lifetime << 17,
                          *(s16 *)(arg + 102) + (remaining_lifetime << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)BattleEffect_UpdateOrbitingParticleFade;
        }
    }
}
