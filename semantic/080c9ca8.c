#include "types.h"

typedef void (*Transfer_080c9ca8)(
    void *destination,
    const void *source,
    u32 size);

typedef void (*Renderer_080c9ca8)(
    void *context,
    const void *source,
    s32 x,
    s32 y,
    s32 width,
    s32 height);

struct Scene_080c9ca8 {
    u8 unknown_00[4];
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Object_080c9ca8 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Particle_080c9ca8 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 phase;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(u32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b5088(s16);
struct Object_080c9ca8 **Func_080b5098(s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, u32);
void Func_080e38b8(struct Particle_080c9ca8 *, s32, s32);
void Func_080e3944(const struct Particle_080c9ca8 *, s32 *);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080c9ca8(struct Scene_080c9ca8 *scene, s32 mode)
{
    u32 *runtime_header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)runtime_header[0];
    void *render_context = (void *)runtime_header[1];
    void *projection_context = *(void **)0x03001e80;
    Transfer_080c9ca8 transfer = (Transfer_080c9ca8)0x03001388;
    Renderer_080c9ca8 renderers[2];
    struct Object_080c9ca8 *focus;
    struct Particle_080c9ca8 *particles =
        (struct Particle_080c9ca8 *)(runtime + 0x7080);
    s8 *configuration = (s8 *)0x080edee8 + mode * 4;
    s32 duration;
    s32 palette_resource;
    s32 secondary_mode;
    s32 frame;
    s32 i;

    *(struct Scene_080c9ca8 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    if (scene->direction == 0) {
        Func_080ed408(46, 7, 7, 11, 2);
        renderers[0] = (Renderer_080c9ca8)runtime_header[7];
        secondary_mode = 3;
    } else {
        Func_080ed408(46, 7, 7, 15, 2);
        renderers[0] = (Renderer_080c9ca8)runtime_header[7];
        secondary_mode = 7;
    }
    Func_080ed408(47, 7, 7, secondary_mode, 2);
    renderers[1] = (Renderer_080c9ca8)runtime_header[8];
    Func_080e0524(0x58, runtime, 0, 0);

    switch (mode) {
    case 0:
        palette_resource = 0xb4;
        break;
    case 1:
        palette_resource = 0xa0;
        break;
    case 2:
        palette_resource = 0xcb;
        break;
    case 3:
        palette_resource = 0x86;
        break;
    default:
        palette_resource = 0xa3;
        break;
    }
    transfer(
        (void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80);

    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 50;
    Func_080041d8((const void *)0x080cd261, 0x480);

    focus = *Func_080b5098(scene->focus_id);
    duration = scene->object_count * 18 + 48;
    for (i = 0; i < scene->object_count; i++) {
        struct Object_080c9ca8 *target =
            *Func_080b5098(scene->object_ids[i]);
        s32 j;

        for (j = 0; j < 3; j++) {
            struct Particle_080c9ca8 *particle =
                &particles[i * 3 + j];

            particle->x = focus->x;
            particle->y = focus->y + 0x140000;
            particle->z = focus->z;
            particle->velocity_x =
                Func_080022ec(target->x - particle->x, 24);
            particle->velocity_y =
                Func_080022ec(
                    target->y + 0x140000 - particle->y, 24);
            particle->velocity_z =
                Func_080022ec(target->z - particle->z, 24);
            particle->phase = 0;
        }
    }

    for (frame = 0; frame < duration; frame++) {
        s32 *wave = (s32 *)(runtime + 0x6980);
        s32 angle = frame << 12;
        s32 object_index;

        for (i = 0; i < 160; i++, angle += 0x1000)
            wave[i] = (0x80000 - Func_08002322(angle) * 2) >> 10;

        if (frame > duration - 16)
            *(volatile u16 *)0x04000052 =
                (duration - frame) | 0x1000;

        Func_080049ac();
        Func_080051d8(
            projection_context,
            (u8 *)projection_context + 12);

        for (object_index = 0;
             object_index < scene->object_count;
             object_index++) {
            s32 start_frame = object_index * 32;
            s32 j;

            for (j = 0; j < 3; j++) {
                s32 particle_frame = start_frame + j * 6;

                if (frame >= particle_frame) {
                    struct Particle_080c9ca8 *particle =
                        &particles[object_index * 3 + j];
                    s32 projected[2];
                    s32 animation;
                    const void *source;

                    Func_080e3944(particle, projected);
                    projected[0] >>= 1;
                    animation = particle->phase / 8;
                    if (animation > 5)
                        animation = 5;

                    if (configuration[0] != 0) {
                        source = runtime + animation * 0x320
                            + Func_080022fc(frame / 2, 3) * 0x12c0;
                        renderers[0](
                            render_context,
                            source,
                            projected[0] - 10,
                            projected[1] - 40,
                            20,
                            40);
                        renderers[1](
                            render_context,
                            source,
                            projected[0] - 10,
                            projected[1],
                            20,
                            40);
                    } else {
                        source = runtime + 0x2580 + animation * 0x320;
                        renderers[0](
                            render_context,
                            source,
                            projected[0] - 10,
                            projected[1] - 40,
                            20,
                            40);
                        renderers[1](
                            render_context,
                            source,
                            projected[0] - 10,
                            projected[1],
                            20,
                            40);
                    }
                    Func_080e38b8(particle, 64, 0);
                    particle->phase++;
                }
            }

            if (configuration[3] != 0
                && frame >= start_frame + 30
                && frame < start_frame + 62) {
                struct Object_080c9ca8 *target =
                    *Func_080b5098(scene->object_ids[object_index]);
                s32 local_frame = frame - start_frame - 30;
                s32 x = target->x
                    + (*(s8 *)(0x080edefc + (local_frame & 7)) << 16);

                if (x > 0)
                    x += 0x8000;
                else
                    x -= 0x8000;
                target->x = x;
                Func_080d6888(
                    scene->object_ids[object_index], -1, 5, -1, 0);
            }

            if (configuration[1] != 0) {
                if (frame == start_frame + 24) {
                    Func_080f9010(0x85);
                    if (object_index == 0)
                        Func_080b50e8(-1);
                    Func_080d6888(
                        scene->object_ids[object_index],
                        7, 5, object_index, 8);
                }
                if (frame == start_frame + 40)
                    Func_080d6888(
                        scene->object_ids[object_index],
                        7, 5, object_index, 8);
            }

            if (configuration[2] != -1
                && frame == start_frame + 24) {
                *(s32 *)(runtime + 0x77a8) = 4;
                Func_080b5088(scene->object_ids[object_index]);
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
