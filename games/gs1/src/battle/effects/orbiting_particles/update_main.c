#include "types.h"
#include "global_cells.h"

struct OrbitingParticle;

void Func_08009240(struct OrbitingParticle *, s32);

#define UpdateOrbitingParticleMain Func_08099018
void UpdateOrbitingParticleMain(struct OrbitingParticle *particle) {
    s32 mode = *(s32 *)ADDR_03001E40 & 7;
    if (mode == 0) {
        Func_08009240(particle, 2);
    } else if (mode == 2) {
        Func_08009240(particle, 0);
    }
}
