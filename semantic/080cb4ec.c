#include "layout_guard.h"
#include "types.h"

struct Particle_080cb4ec {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 unknown_18;
};

typedef void (*Renderer_080cb4ec)(
    void *,
    const void *,
    s32,
    s32,
    s32,
    s32);

LAYOUT_SIZE_GUARD(Particle080cb4ec, struct Particle_080cb4ec, 0x1c);

s32 Func_08002304(s32, s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(volatile void *, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e396c(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080cb4ec(void *object)
{
    static const u8 *const widths = (const u8 *)0x080edf7f;
    static const u8 *const heights = (const u8 *)0x080edf83;
    static const u16 *const image_offsets = (const u16 *)0x080edf88;
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    Renderer_080cb4ec renderers[2];
    struct Particle_080cb4ec *particles =
        (struct Particle_080cb4ec *)(runtime + 0x7080);
    s32 point[3];
    u32 frame;
    u32 index;

    *(void **)(runtime + 0x7828) = object;
    Func_080cd594(1);
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000052 = 0x1000;

    Func_080ed408(46, 7, 7, 3, 1);
    renderers[0] = (Renderer_080cb4ec)runtime_header[7];
    Func_080ed408(47, 7, 7, 7, 1);
    renderers[1] = (Renderer_080cb4ec)runtime_header[8];
    Func_080e0524((volatile void *)0x78, runtime, 1, 1);

    *(s32 *)(runtime + 0x7780) = 1;
    *(s32 *)(runtime + 0x7784) = 0;
    Func_080041d8((const void *)0x080cd261, 0x480);

    Func_080e396c(*(s16 *)((u8 *)object + 36), point);
    *(volatile s32 *)0x04000028 = (64 - point[0]) << 8;

    for (index = 0; index < 64; index++) {
        s32 x = Func_08002304(Func_08004458(), 96) + 16;
        s32 direction;

        particles[index].x = x << 16;
        particles[index].y = (24 - index / 4) << 16;
        if (x <= 43)
            direction = 3;
        else if (x <= 51)
            direction = 2;
        else if (x <= 59)
            direction = 1;
        else if (x <= 67)
            direction = 0;
        else if (x <= 75)
            direction = -1;
        else if (x <= 83)
            direction = -2;
        else
            direction = -3;
        particles[index].velocity_x = direction * 0x20000;
        particles[index].velocity_y = 0x80000;
    }

    Func_080f9010(212);

    for (frame = 0; frame < 120; frame++) {
        if (frame <= 16) {
            *(volatile u16 *)0x04000052 = frame | 0x1000;
            if (frame == 16)
                *(volatile u16 *)0x04000050 = 0;
        }
        if (frame > 103) {
            *(volatile u16 *)0x04000052 =
                (120 - frame) | 0x1000;
            if (frame == 104)
                *(volatile u16 *)0x04000050 = 0x3f44;
        }

        for (index = 15; index >= 0; index--) {
            struct Particle_080cb4ec *particle = &particles[index];
            s32 velocity = particle->velocity_x;
            s32 image = (velocity < 0 ? -velocity : velocity) >> 17;
            s32 width = widths[image];
            s32 height = heights[image];
            Renderer_080cb4ec render =
                renderers[velocity < 0 ? 1 : 0];

            render(
                render_context,
                runtime + image_offsets[image],
                (s16)(particle->x >> 16) - (width >> 1),
                (s16)(particle->y >> 16) - (height >> 1),
                width,
                frame < index * 4 + 25 ? height : height - 4);

            if (frame < index * 4 + 25 &&
                frame >= index * 4 + 16) {
                particle->x += particle->velocity_x;
                particle->y += particle->velocity_y;
            }
        }

        if ((u32)(frame - 23) <= 64 && (frame & 3) == 0) {
            Func_080d6888(
                *(s16 *)(*(u8 **)(runtime + 0x7828) + 36),
                7,
                5,
                0,
                2);
            *(s32 *)(runtime + 0x77a8) = 1;
            if ((frame & 7) == 0)
                Func_080f9010(133);
        }

        Func_080e155c(8, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
