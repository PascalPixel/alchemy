#include "types.h"
#include "scene.h"
#include "abi/battle/effects/orbiting_particles/update_main.h"
#include "global_cells.h"

struct OrbitingParticle;

void BattleFx_UpdateOrbitingParticleMain(struct OrbitingParticle *particle)
{
    s32 battle_mode = *(s32 *)ADDR_03001E40 & 7;
    if (battle_mode == 0) {
        Battle_Apply(particle, 2);
    } else if (battle_mode == 2) {
        Battle_Apply(particle, 0);
    }
}
