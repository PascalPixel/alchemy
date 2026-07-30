#include "types.h"

struct Particle_080e698c {
    s32 x;
    s32 y;
    s32 unused08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused14;
    s32 timer;
};

typedef s32 (*SquareRoot_080e698c)(s32 value);
typedef void (*Renderer_080e698c)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

#define MUL_SHIFT_080E698C(left, right, shift) \
    ((s32)((u32)(left) * (u32)(right)) >> (shift))

s32 Func_080022ec(s32 value, s32 divisor);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
void Func_08009080(void *object, s32 mode);
void Func_08009140(void *object);
void Func_08009150(void *object, s32 x, s32 y, s32 z);
void Func_080b5088(s32 id, s32 arg1);
u8 **Func_080b5098(s32 id);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080cef64(s32 value, Renderer_080e698c *renderers);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e3908(struct Particle_080e698c *particle, s32 arg1, s32 arg2);
void Func_080e3980(s32 id, s32 *point);

void Func_080e698c(void *argument)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *graphics = (void *)runtime_header[2];
    SquareRoot_080e698c square_root =
        (SquareRoot_080e698c)0x030001d8;
    Renderer_080e698c renderers[2];
    struct Particle_080e698c *particles =
        (struct Particle_080e698c *)0x02010000;
    s32 point[2];
    s32 frame;
    s32 index;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x0100;
    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x61, runtime, 1, 1);
    Func_080e0524(0x6d, runtime + 0x3e80, 1, 0);
    Func_080cef64(*(s32 *)((u8 *)argument + 4), renderers);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (index = 0; index < 1024; index++)
        particles[index].timer = 0;

    {
        u8 *first =
            *Func_080b5098(*(s32 *)((u8 *)argument + 8));
        u8 *second =
            *Func_080b5098(*(s16 *)((u8 *)argument + 36));
        s32 delta_x = Func_080022ec(
            (*(s32 *)(second + 8) - *(s32 *)(first + 8)) * 80,
            100);
        s32 delta_y = Func_080022ec(
            (*(s32 *)(second + 16) - *(s32 *)(first + 16)) * 80,
            100);
        s32 target_x = *(s32 *)(first + 8) + delta_x;
        s32 target_y = *(s32 *)(first + 16) + delta_y;
        s32 speed = Func_080022ec(
            square_root(
                (delta_x >> 8) * (delta_x >> 8) +
                (delta_y >> 8) * (delta_y >> 8)) << 8,
            20);

        *(s32 *)(first + 48) = speed;
        *(s32 *)(first + 52) = speed;
        first[88] = 1;
        *(s32 *)(first + 40) = 0x70000;
        *(s32 *)(first + 72) = 0x0000deb8;
        *(s32 *)(first + 68) = 0;
        first[90] = 1;
        Func_08009140(first);
        Func_08009150(first, target_x, 0, target_y);
        Func_08009080(first, 2);
    }

    for (frame = 0; frame < 70; frame++) {
        Func_080e3980(*(s32 *)((u8 *)argument + 8), point);
        *(s32 *)0x04000028 =
            (s32)((u32)(80 - point[0]) << 8);

        if ((u32)(frame - 8) <= 15) {
            s32 stage = (frame - 8) / 2;
            s32 offset;
            s32 x;
            s32 y;
            s32 width;
            s32 height;

            if (stage > 6)
                stage = 6;
            offset = ((const u16 *)0x080eee02)[stage];
            width = ((const u8 *)0x080eedf4)[stage];
            height = ((const u8 *)0x080eedfb)[stage];
            y = point[1] + ((const s8 *)0x080eee17)[stage] - 60;

            if (*(s32 *)((u8 *)argument + 4) == 0) {
                x = ((const s8 *)0x080eee10)[stage] + 30;
                renderers[0](
                    render_context,
                    runtime + offset,
                    x,
                    y,
                    width,
                    height);
            } else {
                x = 108 - ((const s8 *)0x080eee10)[stage] - width;
                renderers[0](
                    render_context,
                    runtime + offset,
                    x,
                    y,
                    width,
                    height);
            }
        }

        if (frame == 18) {
            s32 id = *(s16 *)((u8 *)argument + 36);

            Func_080b50e8(134);
            Func_080d6888(id, 7, 5, 0, 8);
            Func_080b5088(id, 6);
            *(s32 *)(runtime + 0x77a8) = 4;

            for (index = 0; index < 16; index++) {
                struct Particle_080e698c *particle = &particles[index];
                s32 magnitude = (Func_08004458() & 63) + 256;
                s32 angle = Func_08004458() & 0xffff;

                particle->x = 0x400000;
                particle->y = 0x500000;
                particle->velocity_x = MUL_SHIFT_080E698C(
                    magnitude,
                    Func_08002322(angle),
                    7);
                particle->velocity_y = MUL_SHIFT_080E698C(
                    -magnitude,
                    Func_0800231c(angle),
                    6);
                particle->timer = (Func_08004458() & 15) + 16;
            }
        }

        for (index = 0; index < 128; index++) {
            struct Particle_080e698c *particle = &particles[index];

            if (particle->timer > 0) {
                s32 x;
                s32 y;

                particle->timer--;
                Func_080e3908(particle, 60, 0);
                if (particle->y > 0x680000) {
                    particle->velocity_y = -particle->velocity_y / 2;
                } else if (
                    (u32)particle->x <= 0x007effff &&
                    particle->y >= 0) {
                    s32 tile =
                        Func_080022fc((frame + index) / 4, 6);

                    x = (particle->x >> 16) - 8;
                    y = (particle->y >> 16) - 8;
                    renderers[0](
                        render_context,
                        runtime + 0x3e80 + (tile << 8),
                        x,
                        y,
                        16,
                        16);
                }
            }
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
