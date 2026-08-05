#include "types.h"

struct Particle_080dfa48 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 timer;
};

typedef void (*Transfer_080dfa48)(
    void *destination,
    const void *source,
    u32 size);
typedef void (*Renderer_080dfa48)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

void *Func_08002f40(s32 resource);
void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *object, void *position);
void Func_080b5088(s32 id, s32 arg1);
u8 **Func_080b5098(s32 id);
void Func_080b50e8(s32 sound);
void Func_080cd52c(void);
void Func_080cd594(s32 mode);
void Func_080cdbc0(void);
void Func_080d6888(s32 id, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080df90c(s32 arg0, s32 arg1, s32 arg2);
void Func_080df9d0(void *source, void *destination, s32 arg2, s32 arg3);
void Func_080e0524(s32 resource, void *destination, s32 arg2, s32 arg3);
void Func_080e155c(s32 arg0, s32 arg1);
void Func_080e38b8(struct Particle_080dfa48 *particle, s32 arg1, s32 arg2);
void Func_080e3944(struct Particle_080dfa48 *particle, s32 *point);
void Func_080e3980(s32 id, s32 *point);
void Func_080ed408(s32 resource, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void Func_080dfa48(void *argument, s32 variant)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *graphics = (u8 *)runtime_header[2];
    void *camera = *(void **)0x03001e50;
    Transfer_080dfa48 transfer = (Transfer_080dfa48)0x03001388;
    Renderer_080dfa48 renderers[2];
    struct Particle_080dfa48 *particles =
        (struct Particle_080dfa48 *)(runtime + 0x7080);
    s32 main_point[3];
    s32 point[3];
    u32 frame;
    u32 index;
    s32 palette_resource;

    *(void **)(runtime + 0x7828) = argument;
    Func_080cd594(0);

    if (*(s32 *)((u8 *)argument + 4) == 0) {
        Func_080ed408(46, 7, 7, 3, 2);
        Func_080ed408(47, 7, 7, 11, 2);
    } else {
        Func_080ed408(46, 7, 7, 7, 2);
        Func_080ed408(47, 7, 7, 15, 2);
    }
    renderers[0] = (Renderer_080dfa48)runtime_header[46];
    renderers[1] = (Renderer_080dfa48)runtime_header[47];

    Func_080e0524(0x73, graphics, 0, 0);
    Func_080e0524(0x99, runtime, 1, 0);
    Func_080df9d0(runtime, (void *)0x02010000, 40, 288);
    Func_080e0524(0xbd, runtime, 1, 1);

    if (variant == 0)
        palette_resource = 0xc2;
    else if (variant == 1)
        palette_resource = 0xb9;
    else if (variant == 2)
        palette_resource = 0xbb;
    else
        palette_resource = 0xc0;
    transfer(
        (void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    Func_080df90c(
        *(s32 *)((u8 *)argument + 8),
        *(s16 *)((u8 *)argument + 36),
        10);

    {
        u8 *actor =
            *Func_080b5098(*(s16 *)((u8 *)argument + 36));

        for (index = 0; index < 64; index++) {
            struct Particle_080dfa48 *particle = &particles[index];

            particle->x = *(s32 *)(actor + 8);
            particle->y = *(s32 *)(actor + 12) + 0xa0000;
            particle->z = *(s32 *)(actor + 16);
            particle->velocity_x =
                (s32)((Func_08004458() & 0x1ff) << 11);
            particle->velocity_y =
                (s32)((u32)((Func_08004458() & 255) - 64) << 11);
            particle->velocity_z =
                (s32)((u32)((Func_08004458() & 255) - 128) << 11);
            if (particle->x > 0)
                particle->velocity_x = -particle->velocity_x;
            particle->timer = index / 2 + 16;
        }
    }

    Func_080e3980(
        *(s16 *)((u8 *)argument + 36),
        main_point);

    for (frame = 0; frame < 60; frame++) {
        if (frame <= 14) {
            Func_080e3980(
                *(s32 *)((u8 *)argument + 8),
                point);
            renderers[0](
                render_context,
                runtime,
                point[0] / 2 - 16,
                point[1] - 48,
                40,
                32);
            renderers[1](
                render_context,
                runtime,
                point[0] / 2 - 16,
                point[1] - 16,
                40,
                32);
        }

        if (frame == 10) {
            s32 id = *(s16 *)((u8 *)argument + 36);

            Func_080d6888(id, 7, 5, 0, 8);
            Func_080b5088(id, 4);
            Func_080b50e8(134);
            *(s32 *)(runtime + 0x77a8) = 8;
        }

        if ((u32)(frame - 8) <= 11) {
            s32 stage = (frame - 8) / 2;

            renderers[0](
                render_context,
                (u8 *)0x02010000 + stage * 960,
                main_point[0] / 2 - 16,
                main_point[1] - 40,
                20,
                48);
        }

        if ((u32)(frame - 8) <= 55) {
            Func_080049ac();
            Func_080051d8(camera, (u8 *)camera + 12);

            for (index = 0; index < 64; index++) {
                struct Particle_080dfa48 *particle = &particles[index];

                if (particle->timer > 0) {
                    s32 size;
                    const void *source;

                    Func_080e3944(particle, point);
                    point[0] /= 2;
                    size = (particle->timer >> 4) + 2;
                    source =
                        graphics +
                        ((const u16 *)0x080ede48)[size - 1];
                    renderers[0](
                        render_context,
                        source,
                        point[0] - size / 2,
                        point[1] - size,
                        size,
                        size * 2);
                    Func_080e38b8(particle, 60, -512);
                    particle->timer--;
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
