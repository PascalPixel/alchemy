#include "types.h"
#include "scene.h"
#include "abi/battle/effects/particle_pool/run_mode0.h"

void BattleFx_RunParticlePoolMode0(s32 arg0)
{
    Battle_Apply(arg0, 0);
}
