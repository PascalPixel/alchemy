#include "TYPES.H"

/* Mode entries of the particle field effect. */

s32 RunParticleFieldEffect(s32, s32);

void BattleFx_RunParticleFieldMode1(s32 effect)
{
    RunParticleFieldEffect(effect, 1);
}

void BattleFx_RunParticleFieldMode0(s32 effect)
{
    RunParticleFieldEffect(effect, 0);
}
