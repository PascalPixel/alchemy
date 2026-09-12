#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "object_efx.h"
#include "fixed_math.h"

/* battle/effects/orbiting_particles/update_main.c */
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
