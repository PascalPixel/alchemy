#include "TYPES.H"

/* Mode entries of the particle pool effect. */

s32 BattleFx_RunParticlePool(s32, s32);

void BattleFx_RunParticlePoolMode1(s32 arg0)
{
    BattleFx_RunParticlePool(arg0, 1);
}

void BattleFx_RunParticlePoolMode0(s32 arg0)
{
    BattleFx_RunParticlePool(arg0, 0);
}
