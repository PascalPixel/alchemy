#include "types.h"
#include "scene.h"

/* battle/effects/particle_field/mode_1.c */
s32 RunParticleFieldEffect(s32, s32);

void BattleFx_RunParticleFieldMode1(s32 effect)
{
    RunParticleFieldEffect(effect, 1);
}

/* battle/effects/particle_field/mode_0.c */
s32 RunParticleFieldEffect(s32, s32);

void BattleFx_RunParticleFieldMode0(s32 effect)
{
    RunParticleFieldEffect(effect, 0);
}
