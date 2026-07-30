#include "types.h"

typedef void (*Transfer_080cc5d8)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080cc5d8)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080cc5d8 {
    s32 palette;
    u8 unknown_04[4];
    s32 focus_id;
    u8 unknown_0c[24];
    s16 object_ids[1];
};

struct Particle_080cc5d8 {
    s32 angle;
    s32 radius;
    s32 distance;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 phase;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void *Func_080048b0(s32, u32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e396c(s32, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080cc5d8(struct Scene_080cc5d8 *scene)
{
    u8 *runtime = Func_080048b0(39, 0x782c);
    void *render_context = Func_080048b0(40, 0x4000);
    u8 *graphics = Func_080048b0(41, 0x60e);
    struct Particle_080cc5d8 *background_particles =
        (struct Particle_080cc5d8 *)0x02010000;
    struct Particle_080cc5d8 *particles =
        (struct Particle_080cc5d8 *)(runtime + 0x7080);
    Transfer_080cc5d8 transfer = (Transfer_080cc5d8)0x03001388;
    Renderer_080cc5d8 renderer;
    s32 position[2];
    s32 palette_resource;
    s32 frame;
    s32 i;

    *(struct Scene_080cc5d8 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    *(s32 *)(runtime + 0x77b4) = 24;
    *(s32 *)(runtime + 0x77b8) = 0;
    *(volatile u16 *)0x04000052 = 0x100c;
    *(volatile u16 *)0x04000020 = 0x100;
    Func_080e0524(0x45, runtime, 1, 0);
    Func_080e0524(0x76, graphics, 0, 0);

    switch (scene->palette) {
    case 0:
        palette_resource = 0x48;
        break;
    case 1:
        palette_resource = 0x57;
        break;
    case 2:
        palette_resource = 0x47;
        break;
    default:
        palette_resource = 0x46;
        break;
    }
    transfer(
        (void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80);

    for (i = 0; i < 128; i++) {
        background_particles[i].radius = 0x800000;
        background_particles[i].angle = Func_08004458() & 0xffff;
        background_particles[i].distance =
            (Func_08004458() & 0x1ff) + 0x400;
        background_particles[i].phase = -i;
    }
    for (i = 0; i < 64; i++) {
        particles[i].angle = Func_08004458() & 0xffff;
        particles[i].radius = (Func_08004458() & 31) + 16;
        particles[i].phase = (i & 15) + 16;
    }

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080ed408(46, 7, 7, 7, 3);
    renderer = *(Renderer_080cc5d8 *)0x03001f08;
    Func_080f9010(0x8c);

    for (frame = 0; frame < 56; frame++) {
        Func_080e396c(scene->focus_id, position);
        *(volatile s32 *)0x04000028 = (64 - position[0]) << 8;
        if (frame > 49)
            *(volatile u16 *)0x04000052 =
                (0x70 - frame * 2) | 0x1000;

        if (frame == 26) {
            Func_080f9010(0xd4);
            Func_080d6888(scene->object_ids[0], 7, -1, 0, 20);
        }

        if ((u32)(frame - 28) <= 20) {
            s32 animation = Func_080022ec(frame - 28, 3);

            renderer(
                render_context,
                runtime + 0x1400 + animation * 0x900,
                40,
                position[1] - 24,
                48,
                48);
        }

        if ((u32)frame <= 14) {
            s32 animation =
                Func_080022fc(Func_080022ec(frame, 3), 5);

            for (i = 0; i < 4; i++) {
                s8 x_offset = *(s8 *)(0x080ee058 + i);
                s8 y_offset = *(s8 *)(0x080ee05c + i);
                u8 mode = *(u8 *)(0x080ee060 + i);

                Func_080ed408(47, 7, 7, mode | 3, 2);
                (*(Renderer_080cc5d8 *)0x03001f0c)(
                    render_context,
                    runtime + animation * 0x400,
                    x_offset + 32,
                    position[1] + y_offset - 32,
                    32,
                    32);
                Func_08002dd8(47);
            }
        }

        for (i = 0; i < 64; i++) {
            struct Particle_080cc5d8 *particle = &particles[i];

            if (particle->phase >= 0 && particle->radius > 0) {
                s32 half_size = (particle->phase >> 3) + 1;
                s32 size;
                s32 x;
                s32 y;

                if (half_size <= 0)
                    half_size = 1;
                size = half_size * 2;
                x = ((particle->radius *
                    Func_08002322(particle->angle)) >> 16) + 64;
                y = ((particle->radius *
                    Func_0800231c(particle->angle)) >> 16) + position[1];
                renderer(
                    render_context,
                    graphics + *(u16 *)(0x080ede5c + size - 2),
                    x - half_size,
                    y - half_size,
                    size,
                    size);
                particle->radius -= 2;
                particle->phase--;
            }
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
