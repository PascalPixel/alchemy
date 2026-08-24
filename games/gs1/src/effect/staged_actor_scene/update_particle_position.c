#include "types.h"

extern void Func_0200b7d4(s32, s32, s32 *);
extern void Func_0200b838(s32, s32, s32, s32);

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Func_0200b7d4(delta_x, delta_z, position);
        Func_0200b838((s32)particle, position[0], position[1], position[2]);
    }
}
