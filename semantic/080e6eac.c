#include "layout_guard.h"
#include "types.h"

struct Particle_080e6eac {
    s32 x, y, z, velocity_x, velocity_y, unknown_14, timer;
};
typedef void (*Renderer_080e6eac)(
    void *, const void *, s32, s32, s32, s32);
typedef void (*Fill_080e6eac)(void *, s32);

LAYOUT_SIZE_GUARD(Particle080e6eac, struct Particle_080e6eac, 0x1c);

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e3908(struct Particle_080e6eac *, s32, s32);
void Func_080e6d3c(s32, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e6eac(s32 argument, s32 x, s32 y)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    Renderer_080e6eac renderer46;
    Renderer_080e6eac renderer47;
    Fill_080e6eac fill = (Fill_080e6eac)0x03000164;
    struct Particle_080e6eac *first =
        (struct Particle_080e6eac *)(runtime + 0x7080);
    struct Particle_080e6eac *radial =
        (struct Particle_080e6eac *)(runtime + 0x772c);
    struct Particle_080e6eac *second =
        (struct Particle_080e6eac *)0x02010000;
    s32 center_x = (x + 0x280000) / 2;
    s32 frame;
    s32 index;

    *(volatile u16 *)0x04000020 = 0x80;
    *(volatile s32 *)0x04000028 = 0;
    *(volatile u16 *)0x04000050 = 0x3f46;
    Func_080ed408(46, 7, 7, 3, 2);
    renderer46 = (Renderer_080e6eac)header[7];
    Func_080ed408(47, 7, 7, 3, 3);
    renderer47 = (Renderer_080e6eac)header[8];
    Func_080e0524(0x73, effect_tiles, 0, 0);
    Func_080e0524(0x5e, runtime, 1, 0);
    Func_080e0524(0x5f, runtime + 0x59d8, 0, 0);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (index = 0; index < 64; index++) {
        s32 magnitude = (Func_08004458() & 0xff) + 0x100;
        s32 angle = Func_08004458() & 0xffff;
        first[index].x = center_x;
        first[index].y = y;
        first[index].velocity_x =
            magnitude * Func_08002322(angle) >> 7;
        first[index].velocity_y =
            -(magnitude * Func_0800231c(angle) >> 6);
        first[index].timer = (Func_08004458() & 15) + 16;
    }
    for (index = 0; index < 3; index++) {
        s32 angle = index * 0x5555;
        radial[index].x = center_x;
        radial[index].y = y;
        radial[index].velocity_x =
            (Func_08002322(angle) << 5) >> 6;
        radial[index].velocity_y =
            -(Func_0800231c(angle) << 5 >> 5);
    }
    for (index = 0; index < 64; index++) {
        s32 magnitude = (Func_08004458() & 0xff) + 32;
        s32 angle = Func_08004458() & 0xffff;
        second[index].x = center_x;
        second[index].y = y;
        second[index].velocity_x =
            magnitude * Func_08002322(angle) >> 6;
        second[index].velocity_y =
            -(magnitude * Func_0800231c(angle) >> 5);
        second[index].timer = (Func_08004458() & 15) + 20;
    }

    for (frame = 0; frame < 72; frame++) {
        if (frame == 4) Func_080f9010(0x9a);
        if (frame == 32) Func_080f9010(0xd4);

        if (frame <= 47) {
            s32 image = Func_080022ec(frame - 8, 5);
            s32 width;
            s32 height;
            if (image < 0) image = 0;
            width = ((const u8 *)0x080eee56)[image];
            height = ((const u8 *)0x080eee5e)[image];
            renderer46(
                render_context,
                runtime + ((const u16 *)0x080eee66)[image],
                (center_x >> 16) - width / 2,
                (y >> 16) - height / 2,
                width, height);
        }

        for (index = 0; index < 30; index++) {
            struct Particle_080e6eac *p = &first[index];
            if (frame > index / 2 && p->timer > 0) {
                s32 size;
                p->timer--;
                Func_080e3908(p, 60, 0);
                size = p->timer / 16 + 3;
                renderer47(
                    render_context,
                    effect_tiles +
                        ((const u16 *)0x080ede48)[size - 1],
                    (s16)(p->x >> 16) - size / 2,
                    (s16)(p->y >> 16) - size,
                    size, size * 2);
            }
        }

        for (index = 0; index < 60; index++) {
            struct Particle_080e6eac *p = &second[index];
            if (frame > 35 && p->timer > 0) {
                s32 size;
                p->timer--;
                Func_080e3908(p, 60, 0);
                size = p->timer / 16 + 1;
                renderer47(
                    render_context,
                    effect_tiles +
                        ((const u16 *)0x080ede48)[size - 1],
                    (s16)(p->x >> 16) - size / 2,
                    (s16)(p->y >> 16) - size,
                    size, size * 2);
            }
        }

        for (index = 0; index < 3; index++) {
            if ((u32)(frame - 36) <= 27) {
                s32 image = Func_080022ec(frame - 36, 7);
                Func_080e3908(&radial[index], 64, 0);
                renderer46(
                    render_context,
                    runtime + 0x59d8 + image * 0x120,
                    (s16)(radial[index].x >> 16) - 6,
                    (s16)(radial[index].y >> 16) - 12,
                    12, 24);
            }
        }
        if (frame <= 35)
            Func_080e6d3c(argument, x, y);
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    fill((void *)0x06004000, 0x4000);
    Func_08002dd8(47);
    Func_08002dd8(46);
}
