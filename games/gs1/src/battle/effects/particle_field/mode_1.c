#include "types.h"

#define RunParticleFieldEffectMode1 Func_080db6c8
#define RunParticleFieldEffect Func_080db6e0
s32 RunParticleFieldEffect(s32, s32);

void RunParticleFieldEffectMode1(s32 effect) {
    RunParticleFieldEffect(effect, 1);
}
