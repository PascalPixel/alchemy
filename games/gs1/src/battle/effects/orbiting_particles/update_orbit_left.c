#include "types.h"

struct OrbitingParticleVector {
    s32 x;
    s32 y;
    s32 z;
};

struct OrbitingParticle;

void Func_0800447c(s32, s32, struct OrbitingParticleVector *);
#define UpdateOrbitingParticleFade Func_08099040
void UpdateOrbitingParticleFade(void *object);

#define UpdateOrbitingParticleLeft Func_08099070
void UpdateOrbitingParticleLeft(struct OrbitingParticle *particle)
{
    u8 *arg = (u8 *)particle;
    struct OrbitingParticleVector local;
    s16 value;
    s32 raw;

    if (arg != 0) {
        raw = *(u16 *)(arg + 100) - 1;
        *(u16 *)(arg + 100) = raw;
        value = (s16)raw;
        if (value != 0) {
            local.x = *(s32 *)(arg + 56);
            local.y = *(s32 *)(arg + 60);
            local.z = *(s32 *)(arg + 64);
            Func_0800447c(value << 17,
                          *(s16 *)(arg + 102) + (value << 11),
                          &local);
            *(s32 *)(arg + 8) = local.x;
            *(s32 *)(arg + 12) = local.y;
            *(s32 *)(arg + 16) = local.z;
        } else {
            *(s32 *)(arg + 108) = (s32)UpdateOrbitingParticleFade;
        }
    }
}
