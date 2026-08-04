#include "types.h"

struct Effect_080981b0 {
    u8 padding0[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    u8 padding14[4];
    s32 scale_x;
    s32 scale_y;
};

struct Particle_080981b0 {
    u8 padding0[0x28];
    s32 randomOffset;
    u8 padding2c[4];
    s32 scale;
    s32 baseScale;
    u8 padding38[0x10];
    s32 field48;
    u8 padding4c[9];
    u8 mode;
};

void Func_080f9010(s32 sound);
void Func_080030f8(s32 frames);
struct Particle_080981b0 *Func_08096c80(
    s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(struct Particle_080981b0 *particle, const void *callback);
u32 Func_08004458(void);
void Func_08096bec(struct Particle_080981b0 *particle, s32 speed, s32 angle);
void Func_080090d0(struct Effect_080981b0 *effect);
extern u8 Data_0809f0d4;

void Func_080981b0(struct Effect_080981b0 *effect)
{
    s32 count;
    s32 fallingOffset;
    s32 baseScale;

    Func_080f9010(0x9a);
    fallingOffset = -0x800;
    count = 30;
    do {
        effect->y += 0x10000;
        effect->angle += 0x2000;
        effect->scale_x += fallingOffset;
        effect->scale_y += fallingOffset;
        Func_080030f8(1);
        count--;
    } while (count >= 0);

    count = 7;
    baseScale = 0x10000;
    do {
        struct Particle_080981b0 *particle;

        particle = Func_08096c80(
            0x11d, effect->x, effect->y, effect->z);
        if (particle != 0) {
            u32 scale;
            u32 random;
            s32 speed;

            Func_08009098(particle, &Data_0809f0d4);
            scale = Func_08004458();
            particle->baseScale = baseScale;
            scale += baseScale;
            particle->scale = (s32)scale;
            particle->mode = 2;
            particle->field48 = 0xa3d;
            random = Func_08004458();
            particle->randomOffset = (s32)(random - Func_08004458());
            speed = Func_08004458() * 24 + 0x80000;
            Func_08096bec(particle, speed, Func_08004458());
        }
        count--;
    } while (count >= 0);

    Func_080f9010(0x83);
    Func_080090d0(effect);
}
