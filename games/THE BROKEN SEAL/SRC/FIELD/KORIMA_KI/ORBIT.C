#include "KORIMAKI.H"

void PaletteScene_AdvanceOrbit(struct OrbitingPaletteEffect *effect)
{
    s32 position[3];
    s32 step = effect->step;
    s32 heading;

    if (step <= 119) {
        position[0] = effect->anchor_x;
        position[1] = effect->anchor_y;
        position[2] = effect->anchor_z;
        heading = effect->heading;
        Vector_AddPolarOffset(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Resource_ResetEntry(effect->owner[0x1c]);
        Engine_ObjectDispatchRelease(effect);
    }
}
