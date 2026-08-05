#include "layout_guard.h"
#include "types.h"

struct Particle_080ed104 {
    s32 x;
    s32 y;
    s32 unknown_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 phase;
};

typedef void (*Renderer_080ed104)(
    void *,
    const void *,
    s32,
    s32,
    s32,
    s32);

LAYOUT_SIZE_GUARD(Particle080ed104, struct Particle_080ed104, 0x1c);

s32 Func_080022ec(s32, s32);
s32 Func_0800231c(u16);
s32 Func_08002322(u16);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(volatile void *);
u32 Func_08004458(void);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, u32 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080e3980(s16, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);

void Func_080ed104(void *object)
{
    static const u16 *const image_offsets = (const u16 *)0x080eef88;
    static const u16 *const image_sizes = (const u16 *)0x080eef96;
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *effect_tiles = (void *)runtime_header[2];
    struct Particle_080ed104 *particles =
        (struct Particle_080ed104 *)(runtime + 0x7080);
    Renderer_080ed104 renderers[2];
    s32 point[3];
    s32 scroll = 0;
    u32 frame;
    u32 index;

    *(void **)(runtime + 0x7828) = object;
    Func_080cd594(0);
    *(volatile u16 *)0x04000020 = 0x0100;
    Func_080e0524(0x73, effect_tiles, 0, 0);
    Func_080e0524(0x51, runtime, 1, 1);
    Func_080e0524(0xc0, runtime + 0x460, 1, 0);
    Func_080cef64(*(s32 *)((u8 *)object + 4), (u32 *)renderers);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080e3980(*(s16 *)((u8 *)object + 36), point);
    *(volatile s32 *)0x04000028 = (64 - point[0]) << 8;

    for (index = 0; index < 16; index++) {
        s32 radius = (Func_08004458() & 0x1ff) + 128;
        u16 angle = Func_08004458();

        particles[index].x = 0x400000;
        particles[index].y = 0x700000;
        particles[index].velocity_x =
            (radius * Func_08002322(angle)) >> 8;
        particles[index].velocity_y =
            (radius * Func_0800231c(angle)) >> 9;
        particles[index].phase = Func_08004458() & 7;
    }

    *(s32 *)(runtime + 0x77a8) = 8;

    for (frame = 0; frame < 54; frame++, scroll += 6) {
        if (frame == 10) {
            s16 actor = *(s16 *)(*(u8 **)(runtime + 0x7828) + 36);

            *(s32 *)(runtime + 0x77a8) = 8;
            Func_080b50e8(212);
            Func_080b5088(actor, 0);
            Func_080d6888(actor, 7, 5, 0, 8);
        }

        if (frame > 7) {
            s32 height =
                frame <= 31 ? frame * 12 - 96 : 272 - scroll;

            if (height > 0) {
                s32 mode;
                u32 strip;

                if (height > 80) {
                    height = 80;
                    mode = 2;
                } else {
                    mode = 3;
                }

                for (strip = 0; strip < 2; strip++) {
                    Renderer_080ed104 strip_renderer;

                    Func_080ed408(
                        46,
                        7,
                        7,
                        strip == 0 ? 3 : 7,
                        mode);
                    strip_renderer =
                        *(Renderer_080ed104 *)0x03001f08;
                    strip_renderer(
                        render_context,
                        runtime,
                        50 + strip * 14,
                        112 - height,
                        14,
                        height);
                    Func_08002dd8(46);
                }
            }
        }

        Func_080cef64(
            *(s32 *)(*(u8 **)(runtime + 0x7828) + 4),
            (u32 *)renderers);
        for (index = 0; index < 16; index++) {
            struct Particle_080ed104 *particle = &particles[index];

            if (frame >= index / 2 + 8 && particle->phase <= 28) {
                s32 image = Func_080022ec(particle->phase, 3);
                s32 size;

                if (image > 6)
                    image = 6;
                size = image_sizes[image];
                renderers[0](
                    render_context,
                    runtime + 0x460 + image_offsets[image],
                    (s16)(particle->x >> 16) - (size >> 1),
                    (s16)(particle->y >> 16) - (size >> 1),
                    size,
                    size);
                particle->phase++;
                Func_080e3908(particle, 62, -0x2000);
            }
        }

        Func_08002dd8(47);
        Func_08002dd8(46);
        if (frame <= 7)
            Func_080e155c(2, 2);
        else
            Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((volatile void *)0x080cd261);
    Func_080cdbc0();
}
