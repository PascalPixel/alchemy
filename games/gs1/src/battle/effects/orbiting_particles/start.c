#include "types.h"

struct OrbitingParticleChild {
    u8 padding[35];
    u8 flags;
};

struct OrbitingParticleState {
    u8 padding_00[20];
    struct OrbitingParticleChild *child;
    u8 padding_18[8];
    u8 active;
    u8 padding_21[20];
    s8 mode;
};

extern struct OrbitingParticleState *Data_03001f30;
#define RunOrbitingParticleEffect Func_08099160
void RunOrbitingParticleEffect(void);

#define StartOrbitingParticleEffect Func_08099128
void StartOrbitingParticleEffect(void)
{
    struct OrbitingParticleState *state = Data_03001f30;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        RunOrbitingParticleEffect();
    }
}
