#include "types.h"

#define RunParticleFieldEffect Func_080db6e0
#define RunParticleFieldEffectMode0 Func_080db6d4

s32 RunParticleFieldEffect(s32, s32);

void RunParticleFieldEffectMode0(s32 effect) {
    RunParticleFieldEffect(effect, 0);
}
