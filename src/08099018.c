#include "types.h"

struct Particle_08099160;

void Func_08009240(struct Particle_08099160 *, s32);

void Func_08099018(struct Particle_08099160 *particle) {
    s32 mode = *(s32 *)0x03001E40 & 7;
    if (mode == 0) {
        Func_08009240(particle, 2);
    } else if (mode == 2) {
        Func_08009240(particle, 0);
    }
}
