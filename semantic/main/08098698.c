#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void Func_08097384(void);
u8 *Func_08096c80(s32, s32, s32, s32);
u32 Func_08004458(void);
void Func_0800447c(s32, s32, s32 *);
void Func_08009240(void *, s32);
void Func_08009080(void *, s32);
void Func_08009098(void *, const void *);
void Func_080091e0(void *, s32);
void Func_08009150(void *, s32, s32, s32);
void Func_080f9010(s32);
void Func_080030f8(s32);

/*
 * Emit twenty-four copies of the scene projectile. Each copy inherits the
 * base sprite's render attributes, receives a randomized velocity, and is
 * launched from a direction-dependent point around the reference object.
 */
void Func_08098698(void)
{
    u8 *state = *(u8 **)0x03001F30;
    u8 *reference = M2C_FIELD(state, u8 **, 0x10);
    s32 position[3];
    s32 count;

    Func_08097384();
    for (count = 0; count < 24; count++) {
        s32 direction = M2C_FIELD(state, s32 *, 0);
        u8 *projectile;
        u8 *sprite;
        u8 *base_sprite;

        position[0] = M2C_FIELD(reference, s32 *, 8);
        position[2] = M2C_FIELD(reference, s32 *, 0x10);
        if (direction == 0x4000) {
            position[1] = M2C_FIELD(reference, s32 *, 0xC) + 0xA0000;
        } else if (direction == 0xC000) {
            position[1] = M2C_FIELD(reference, s32 *, 0xC) + 0x180000;
        } else {
            position[1] = M2C_FIELD(reference, s32 *, 0xC) + 0xA0000;
            Func_0800447c(0xA0000, direction, position);
        }

        projectile = Func_08096c80(
            0x11C, position[0], position[1], position[2]);
        base_sprite = M2C_FIELD(projectile, u8 **, 0x50);
        sprite = base_sprite + 0xC;

        sprite[5] = (sprite[5] & ~0x20) | (base_sprite[5] & 0x20);
        sprite[5] =
            (sprite[5] & 0x3F) | ((base_sprite[5] >> 6) << 6);
        sprite[7] =
            (sprite[7] & 0x3F) | ((base_sprite[7] >> 6) << 6);
        M2C_FIELD(sprite, u16 *, 8) =
            (M2C_FIELD(sprite, u16 *, 8) & 0xFC00) |
            (M2C_FIELD(base_sprite, u16 *, 8) & 0x3FF);
        sprite[9] =
            (sprite[9] & 0xF) | ((base_sprite[9] >> 4) << 4);

        if (projectile != 0) {
            s32 velocity[3];
            s32 magnitude;

            M2C_FIELD(projectile, s32 *, 0x1C) = 0xB333;
            M2C_FIELD(projectile, s32 *, 0x18) = 0xB333;
            M2C_FIELD(projectile, s32 *, 0x34) = 0x18000;
            M2C_FIELD(projectile, s32 *, 0x30) = 0x18000;
            M2C_FIELD(projectile, u8 *, 0x55) = 0;
            Func_08009240(projectile, 0xB);
            Func_08009080(projectile, 7);
            Func_08009098(projectile, (const void *)0x0809F0B4);
            Func_080091e0(projectile, 1);

            velocity[0] = M2C_FIELD(state, s32 *, 4);
            velocity[1] = M2C_FIELD(state, s32 *, 8);
            velocity[2] = M2C_FIELD(state, s32 *, 0xC);
            if (direction == 0xC000) {
                Func_0800447c(0xE0000, direction, velocity);
            }
            magnitude = Func_08004458() * 6 + 0x40000;
            Func_0800447c(magnitude, Func_08004458(), velocity);
            Func_08009150(
                projectile, velocity[0], velocity[1], velocity[2]);
        }

        Func_080f9010(0x83);
        Func_080030f8(2);
    }
    Func_080030f8(8);
}
