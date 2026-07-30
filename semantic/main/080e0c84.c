#include "types.h"

typedef void (*Renderer_080e0c84)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080e0c84 {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[20];
    s32 show_actor;
    u8 unknown_20[4];
    s16 object_ids[1];
};

struct Particle_080e0c84 {
    s32 x;
    s32 y;
    s32 variant_seed;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 phase;
};

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e0c84 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080de2f8(
    void *, s32, s32, s32, s32 *, s32 *);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(struct Particle_080e0c84 *, s32, s32);
void Func_080e3980(s16, s32 *);

static s32 HalfTowardZero_080e0c84(s32 value)
{
    return (value + ((u32)value >> 31)) >> 1;
}

void Func_080e0c84(struct Scene_080e0c84 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    struct Particle_080e0c84 *particles =
        (struct Particle_080e0c84 *)0x02010000;
    Renderer_080e0c84 renderers[2];
    s32 actor_x;
    s32 actor_y;
    s32 object_position[2];
    s32 frame;
    s32 i;

    *(struct Scene_080e0c84 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080de2f8(
        scene, 1, scene->direction, 2, &actor_x, &actor_y);
    Func_080cef64(scene->direction, renderers);
    Func_080e0524(0x6e, runtime, 1, 1);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);
    Func_080e3980(scene->object_ids[0], object_position);

    for (i = 0; i < 64; i++) {
        s32 angle = (Func_08004458() & 0x7fff) + 0x4000;
        s32 speed = (Func_08004458() & 0x1ff) + 0x80;

        particles[i].x =
            (HalfTowardZero_080e0c84(object_position[0])
                + (Func_08004458() & 15) - 8) << 16;
        particles[i].y = (object_position[1] + 8) << 16;
        particles[i].velocity_x =
            (speed * Func_08002322(angle)) >> 9;
        particles[i].velocity_y =
            (speed * Func_0800231c(angle)) >> 6;
        particles[i].variant_seed = Func_08004458() & 0x7f;
        particles[i].unknown_14 = Func_08004458() & 0x7f;
        particles[i].phase = (Func_08004458() & 15) + 32;
    }

    for (frame = 0; frame < 64; frame++) {
        if (frame > 47)
            *(volatile u16 *)0x04000052 =
                (64 - frame) | 0x1000;

        if (frame == 1) {
            Func_080e0524(0xb8, runtime + 0x400, 1, 1);
            Func_080e0524(0x92, runtime + 0x65c0, 1, 0);
        }

        if (scene->show_actor == 1) {
            s32 angle = frame << 11;
            s32 x = ((-Func_08002322(angle) * 4) >> 16)
                + HalfTowardZero_080e0c84(actor_x) - 10;
            s32 y = ((Func_0800231c(angle) * 2) >> 16)
                + actor_y - 22;

            if (frame > 69)
                y = y - frame * 2 + 138;
            renderers[1](
                render_context,
                runtime + 0x65c0,
                x,
                y,
                20,
                40);
            if (frame <= 3)
                renderers[1](
                    render_context,
                    runtime + 0x65c0,
                    x,
                    y,
                    20,
                    40);
        }

        for (i = 0; i < 64; i++) {
            struct Particle_080e0c84 *particle = &particles[i];

            if (frame >= i / 4 + 4) {
                s32 variant = (particle->variant_seed / 127) & 3;
                s32 width = *(u8 *)(0x080eec5f + variant);
                s32 height = *(u8 *)(0x080eec63 + variant);

                renderers[i & 1](
                    render_context,
                    runtime + 0x400
                        + *(u16 *)(0x080eec68 + variant * 2),
                    (s16)(particle->x >> 16) - (width >> 1),
                    (s16)(particle->y >> 16) - (height >> 1),
                    width,
                    height);
                Func_080e38b8(particle, 63, 0x1000);
            }
        }

        if (frame == 8) {
            *(s32 *)(runtime + 0x77a8) = 8;
            Func_080b50e8(0x86);
            Func_080d6888(scene->object_ids[0], 7, 5, 0, 16);
            Func_080b5088(scene->object_ids[0], 3);
        }

        {
            s32 fade = frame * 4;
            s32 scroll = (frame / 4) & 31;

            if (fade > 32)
                fade = 32;
            if (scene->direction == 0) {
                for (i = 0; i < 5; i++)
                    renderers[0](
                        render_context,
                        runtime,
                        i * 32 - scroll,
                        120 - fade,
                        32,
                        32);
            } else {
                for (i = 0; i < 5; i++)
                    renderers[0](
                        render_context,
                        runtime,
                        i * 32 + scroll - 32,
                        120 - fade,
                        32,
                        32);
            }
        }

        Func_080e155c(4, 8);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
