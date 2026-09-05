#include "types.h"

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

extern struct OrbitingParticleState *Data_03001f30;
void BattleEffect_RunOrbitingParticles(void);

void BattleEffect_StartOrbitingParticles(void)
{
    struct OrbitingParticleState *state = Data_03001f30;
    struct OrbitingParticleChild *child = state->child;

    if (child != 0) {
        if (state->battle_mode != 0) {
            state->active = 1;
        }
        child->flags |= 2;
        BattleEffect_RunOrbitingParticles();
    }
}
