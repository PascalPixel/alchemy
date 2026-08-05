#include "types.h"

typedef void (*Transfer_080e08c0)(void *, const void *, u32);
typedef void (*Renderer_080e08c0)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080e08c0 {
    u8 unknown_00[20];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Point_080e08c0 {
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_0c;
    s32 unknown_10;
    s32 unknown_14;
    s32 timer;
};

struct Particle_080e08c0 {
    s32 x;
    s32 y;
    s32 angle;
    s32 velocity_x;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080b5088(s32, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080cef64(s32, Renderer_080e08c0 *);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(void *, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080e08c0 *, s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e08c0(struct Scene_080e08c0 *scene)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    u8 *effect_tiles = (u8 *)runtime_header[2];
    Renderer_080e08c0 renderers[2];
    Transfer_080e08c0 transfer = (Transfer_080e08c0)0x03001388;
    struct Point_080e08c0 *points =
        (struct Point_080e08c0 *)(runtime + 0x7080);
    struct Particle_080e08c0 *particles =
        (struct Particle_080e08c0 *)0x02010000;
    s32 frame;
    s32 group;
    s32 index;
    s32 label_y;

    *(struct Scene_080e08c0 **)(runtime + 0x7828) = scene;
    Func_080cd594(0);
    Func_080cef64(0, renderers);
    Func_080e0524((void *)0x73, effect_tiles, 0, 0);
    Func_080e0524((void *)0x8e, runtime, 1, 0);
    Func_080e0524((void *)0xb7, runtime + 0x320, 1, 1);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (group = 0; group < 9; group++) {
        s32 angle = group << 11;
        s32 x = Func_08002322(angle) * 24 >> 16;

        points[group].x = (group & 1) ? 32 - x : x + 32;
        points[group].y =
            (Func_0800231c(angle) * 4 >> 16) + 52;
        points[group].timer = -(group * 2);

        for (index = 0; index < 16; index++) {
            struct Particle_080e08c0 *particle =
                &particles[group * 16 + index];

            particle->x =
                ((Func_08004458() & 15) + points[group].x - 8) << 16;
            particle->y =
                ((Func_08004458() & 7) + 96) << 16;
            particle->velocity_x =
                ((s32)(Func_08004458() & 0x7f) - 64) << 11;
            particle->velocity_y =
                ((s32)(Func_08004458() & 0x7f) - 64) << 10;
            particle->angle = Func_08004458() & 0xffff;
            particle->unknown_14 = Func_08004458() & 0xffff;
        }
    }

    Func_080f9010(0x88);
    label_y = -172;
    for (frame = 0; frame < 112; frame++, label_y += 8) {
        if (frame == 56)
            Func_080b50e8(0x85);

        if (frame <= 23) {
            renderers[0](
                render_context,
                runtime + 0x320 + (frame / 4) * 0x640,
                40, 20, 40, 40);
        }

        if (frame == 20) {
            transfer((void *)0x05000000, Func_08002f40(0x8e), 0x80);
        }

        if ((u32)(frame - 20) <= 11) {
            if (frame > 23) {
                renderers[1](
                    render_context, runtime,
                    146 - frame * 4, label_y,
                    20, 40);
            } else {
                renderers[0](
                    render_context, runtime,
                    50, 20, 20, 40);
            }
        }

        if (frame == 32) {
            Func_080f9010(0x91);
            *(s32 *)(runtime + 0x77a8) = 8;
            Func_080e0524((void *)0xb4, runtime, 1, 1);
        }

        if (frame > 31) {
            for (index = 0; index < 9; index++) {
                s32 timer = points[index].timer;

                if ((u32)timer <= 47) {
                    s32 stage = timer / 8;
                    s32 width = *(u8 *)(0x080ede9f + stage);
                    s32 height = *(u8 *)(0x080edea5 + stage);
                    const void *source = runtime +
                        *(u16 *)(0x080edeb2 + stage * 2);

                    renderers[0](
                        render_context,
                        source,
                        points[index].x - width / 2,
                        points[index].y +
                            *(u8 *)(0x080edeab + stage),
                        width,
                        height);
                }
                points[index].timer++;
            }
        }

        for (index = 0; index < 144; index++) {
            struct Particle_080e08c0 *particle = &particles[index];

            if (frame >= (index / 16) * 2 + 40) {
                s32 size = (index & 1) + 3;
                s32 height = size * 2;
                const void *source = effect_tiles +
                    *(u16 *)(0x080ede48 + height - 2);
                s32 x = *(s16 *)((u8 *)particle + 2) +
                    (Func_08002322(particle->angle) * 4 >> 16);

                renderers[1](
                    render_context,
                    source,
                    x - size / 2,
                    *(s16 *)((u8 *)particle + 6) - size,
                    size,
                    height);
                Func_080e3908(particle, 64, -0x2000);
                {
                    s32 old_angle = particle->angle;

                    particle->angle = old_angle + 0x800;
                    if (particle->angle > 0xffff)
                        particle->angle = old_angle - 0xf7ff;
                }
            }
        }

        if (frame == 38) {
            for (index = 0; index < scene->object_count; index++) {
                Func_080d6888(
                    scene->object_ids[index], 7, 5, index, 16);
                Func_080b5088(scene->object_ids[index], 6);
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
