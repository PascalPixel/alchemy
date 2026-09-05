#include "types.h"
#include "global_cells.h"

struct OrbitingParticle;

void Func_08009240(struct OrbitingParticle *, s32);

void BattleEffect_UpdateOrbitingParticleMain(struct OrbitingParticle *particle) {
    s32 battle_mode = *(s32 *)ADDR_03001E40 & 7;
    if (battle_mode == 0) {
        Func_08009240(particle, 2);
    } else if (battle_mode == 2) {
        Func_08009240(particle, 0);
    }
}
