#include "types.h"

struct Particle_080cb7f8 {
    s32 angle;
    s32 y;
    s32 radius;
    s32 unused0c;
    s32 unused10;
    s32 unused14;
    s32 timer;
};

typedef void (*Transfer_080cb7f8)(void *, const void *, u32);
typedef void (*Renderer_080cb7f8)(
    void *, const void *, s32, s32, s32, s32);

s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08002dd8(s32 resource);
void *Func_08002f40(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
u8 *Func_080048b0(s32 resource, u32 size);
void Func_080049ac(void);
void Func_080051d8(void *object, void *position);
u8 **Func_080b5098(s32 id);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e3944(s32 *world, s32 *screen);
void Func_080e3980(s32 id, s32 *point);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080f9010(s32 sound);

void Func_080cb7f8(void *argument)
{
    u8 *runtime = Func_080048b0(39, 0x100c);
    u8 *resource40 = Func_080048b0(40, 0x4000);
    u8 *resource41 = Func_080048b0(41, 0x100);
    void *camera = *(void **)0x03001e80;
    Transfer_080cb7f8 transfer = (Transfer_080cb7f8)0x03001388;
    Renderer_080cb7f8 renderer46;
    Renderer_080cb7f8 renderer47;
    struct Particle_080cb7f8 *particles =
        (struct Particle_080cb7f8 *)0x02010000;
    s32 point[3];
    s32 world[3];
    s32 screen[3];
    s32 frame;
    s32 index;
    s32 palette_resource;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);
    *(s32 *)(runtime + 0x782c) = 24;
    *(s32 *)(runtime + 0x77b4) = 0;
    *(s32 *)(runtime + 0x77b8) = 0;
    *(volatile u16 *)0x04000052 = 0x060e;
    *(volatile u16 *)0x04000020 = 0x0100;
    Func_080e0524(0x57, runtime, 1, 0);
    Func_080e0524(0x76, resource41, 0, 0);

    if (*(s32 *)argument == 0)
        palette_resource = 0x48;
    else if (*(s32 *)argument == 1)
        palette_resource = 0x57;
    else if (*(s32 *)argument == 2)
        palette_resource = 0x47;
    else
        palette_resource = 0x46;
    transfer(
        (void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80);

    for (index = 0; index < 128; index++) {
        particles[index].y = 0;
        particles[index].angle = Func_08004458() & 0xffff;
        particles[index].radius =
            (Func_08004458() & 0x1ff) + index * 2;
        particles[index].timer = -index;
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080ed408(46, 7, 7, 3, 3);
    renderer46 = (Renderer_080cb7f8)
        *(u32 *)0x03001fa4;

    (*(s32 *)((u8 *)argument + 24))++;
    if (*(s32 *)((u8 *)argument + 24) <= 0)
        *(s32 *)((u8 *)argument + 24) = 1;
    if (*(s32 *)((u8 *)argument + 24) > 4)
        *(s32 *)((u8 *)argument + 24) = 4;

    Func_080f9010(212);

    for (frame = 0; frame < 56; frame++) {
        s32 quarter;
        s32 phase;
        s32 strip;

        Func_080e3980(*(s32 *)((u8 *)argument + 8), point);
        *(s32 *)0x04000028 =
            (s32)((u32)(64 - point[0]) << 8);
        if (frame > 49)
            *(volatile u16 *)0x04000052 =
                (0x70 - frame * 2) | 0x1000;

        if (frame == 16) {
            Func_080d6888(
                *(s16 *)((u8 *)argument + 36),
                7,
                -1,
                0,
                20);
        }

        quarter = (frame / 2) / 4;
        phase = frame / 2 - quarter * 4;
        strip = Func_080022fc(frame / 4, 3);

        Func_080ed408(47, 7, 7, 3, 2);
        renderer47 = (Renderer_080cb7f8)
            *(u32 *)0x03001f0c;
        renderer47(
            resource40,
            runtime + phase * 1088,
            47,
            point[1] - 64,
            17,
            64);
        renderer47(
            resource40,
            runtime + 0x1100 + strip * 1032,
            40,
            point[1] - 36,
            24,
            43);
        Func_08002dd8(47);

        Func_080ed408(47, 7, 7, 7, 2);
        renderer47 = (Renderer_080cb7f8)
            *(u32 *)0x03001f0c;
        renderer47(
            resource40,
            runtime + phase * 1088,
            64,
            point[1] - 64,
            17,
            64);
        renderer47(
            resource40,
            runtime + 0x1100 + strip * 1032,
            64,
            point[1] - 36,
            24,
            43);
        Func_08002dd8(47);

        (void)Func_080b5098(*(s32 *)((u8 *)argument + 8));
        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 12);

        for (index = 0; index < 32; index++) {
            struct Particle_080cb7f8 *particle = &particles[index];

            if (particle->timer >= 0) {
                s32 size =
                    *(s32 *)((u8 *)argument + 24) + (index & 1);
                const void *source =
                    resource41 +
                    ((const u16 *)0x080ede5c)[size - 1];

                world[0] =
                    particle->radius *
                    Func_08002322(particle->angle) >> 4;
                world[1] = particle->y;
                world[2] =
                    -(particle->radius *
                      Func_0800231c(particle->angle) >> 4);
                particle->angle += 0x400;
                particle->y += 0x50000;
                particle->radius += 64;
                Func_080e3944(world, screen);
                screen[0] /= 2;
                renderer46(
                    resource40,
                    source,
                    screen[0] - size,
                    screen[1] - size,
                    size * 2,
                    size * 2);
            }
            particle->timer++;
        }

        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08002dd8(46);
    Func_08004278((const void *)0x080cd261);
    Func_080cdbc0();
    Func_08002dd8(41);
    Func_08002dd8(40);
    Func_08002dd8(39);
}
