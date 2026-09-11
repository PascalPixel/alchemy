#include "types.h"
#include "scene.h"
#include "abi/battle/effects/orbiting_particles/start.h"

struct OrbitingParticleChild {
    u8 reserved_00[35];
    u8 flags;
};

struct OrbitingParticleState {
    u8 reserved_00[20];
    struct OrbitingParticleChild *child;
    u8 reserved_18[8];
    u8 active;
    u8 reserved_21[20];
    s8 battle_mode;
};

extern struct OrbitingParticleState *gIw;
void BattleFx_RunOrbitingParticles(void);

void BattleFx_StartOrbitingParticles(void)
{
    struct OrbitingParticleState *state = gIw;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->battle_mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        BattleFx_RunOrbitingParticles();
    }
}
