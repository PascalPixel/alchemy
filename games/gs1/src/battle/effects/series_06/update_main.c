#include "types.h"
#include "global_cells.h"

struct Effect06Particle;

void Func_08009240(struct Effect06Particle *, s32);

#define UpdateEffect06Main Func_08099018
void UpdateEffect06Main(struct Effect06Particle *particle) {
    s32 mode = *(s32 *)ADDR_03001E40 & 7;
    if (mode == 0) {
        Func_08009240(particle, 2);
    } else if (mode == 2) {
        Func_08009240(particle, 0);
    }
}
