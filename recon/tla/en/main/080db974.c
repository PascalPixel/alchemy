/*
 * Draft: Effect_UpdateParticlePosition does not yet match; it does not compile against ⚓️'s headers yet.
 * Links as recon/tla/raw/080db4b8.s.
 */
#include "STAGED_MOTION.H"

void Effect_UpdateParticlePosition(s32 *particle, s32 delta_x, s32 delta_z)
{
    s32 position[3];
    if (particle != 0) {
        position[0] = particle[2];
        position[1] = particle[3];
        position[2] = particle[4];
        Vector_AddPolarOffset(delta_x, delta_z, position);
        Object_SetPosition((s32)particle, position[0], position[1], position[2]);
    }
}
