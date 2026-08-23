#include "types.h"

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle;

void RotateVectorByMagnitude(s32, s32, struct OrbitingParticleVector *);
#define UpdateOrbitingParticleFade Func_08099040
void UpdateOrbitingParticleFade(void *object);

#define UpdateOrbitingParticleRight Func_080990cc
void UpdateOrbitingParticleRight(struct OrbitingParticle *particle)
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
            RotateVectorByMagnitude(battle_value << 17,
                          *(s16 *)(arg + 102) - (battle_value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)UpdateOrbitingParticleFade;
        }
    }
}
