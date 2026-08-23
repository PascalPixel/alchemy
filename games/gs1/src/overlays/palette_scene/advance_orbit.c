#include "palette_scene.h"

struct OrbitingPaletteEffect {
    s32 pad00[2];
    s32 x;
    s32 y;
    s32 z;
    s32 pad14;
    s32 angle_x;
    s32 angle_y;
    s32 pad20[6];
    s32 anchor_x;
    s32 anchor_y;
    s32 anchor_z;
    s32 pad44[3];
    u8 *owner;
    s32 pad54[4];
    s16 step;
    s16 heading;
};

void Func_02002e82();
void Func_02002eb4();
void Func_02002eda();

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
        Func_02002e82(step << 16, step * 768 + heading, position);
        effect->x = position[0];
        effect->y = position[1];
        effect->z = position[2];
        effect->angle_x += 0x147;
        effect->angle_y += 0x147;
        effect->step++;
    } else {
        Func_02002eb4(effect->owner[0x1c]);
        Func_02002eda(effect);
    }
}
