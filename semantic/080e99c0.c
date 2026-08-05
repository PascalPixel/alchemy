#include "types.h"

typedef void (*Renderer_080e99c0)(
    void *, const void *, s32, s32, s32, s32);
typedef void (*Fill_080e99c0)(void *, u32, u32);

struct Scene_080e99c0 {
    u8 unknown_00[4];
    s32 direction;
    s32 focus_id;
    u8 unknown_0c[8];
    s32 object_count;
    u8 unknown_18[12];
    s16 object_ids[1];
};

struct Particle_080e99c0 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_0800231c(s32);
s32 Func_08002322(s32);
void Func_08002dd8(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080e3908(struct Particle_080e99c0 *, s32, s32);
void Func_080e3980(s16, s32 *);
void Func_080e46f0(s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

static void InitializeBurstParticle_080e99c0(
    struct Particle_080e99c0 *particle, s32 center_x)
{
    s32 magnitude = Func_08004458() & 0x3f;
    s32 angle = Func_08004458() & 0xffff;

    particle->x =
        center_x + (magnitude * Func_08002322(angle) >> 3);
    particle->y =
        0x600000 + (magnitude * Func_0800231c(angle) >> 2);
    particle->velocity_x =
        ((Func_08004458() & 0x3f) - 32) << 14;
    particle->velocity_y =
        (-(s32)(Func_08004458() & 0x3f) - 8) << 13;
    particle->life = 0;
}

void Func_080e99c0(struct Scene_080e99c0 *scene)
{
    static const u8 *const orbit_frames =
        (const u8 *)0x080eef12;
    static const s32 *const accelerations =
        (const s32 *)0x080eef18;
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *effect_tiles = (u8 *)header[2];
    Renderer_080e99c0 renderers[2];
    Fill_080e99c0 fill = (Fill_080e99c0)0x03000168;
    struct Particle_080e99c0 *orbit =
        (struct Particle_080e99c0 *)(runtime + 0x7080);
    struct Particle_080e99c0 *particles =
        (struct Particle_080e99c0 *)0x02010000;
    struct Particle_080e99c0 *falling =
        (struct Particle_080e99c0 *)0x02014ad0;
    s32 half_height;
    s32 center_x;
    u32 frame;
    u32 i;

    *(struct Scene_080e99c0 **)(runtime + 0x7828) = scene;
    Func_080cd594(1);
    *(volatile u16 *)0x04000052 = 0x1010;
    Func_080e3980(scene->object_ids[0], &half_height);
    half_height =
        (half_height + ((u32)half_height >> 31)) >> 1;

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = (Renderer_080e99c0)header[7];
    Func_080ed408(47, 7, 7, 3, 1);
    renderers[1] = (Renderer_080e99c0)header[8];

    Func_080e0524(0x56, runtime + 0x4e20, 1, 1);
    Func_080e0524(0x85, runtime, 1, 0);
    Func_080e0524(0x7d, runtime + 0xdd0, 1, 0);
    Func_080e0524(0x73, effect_tiles, 0, 0);
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 32; i++) {
        s32 angle = Func_08004458() & 0xffff;

        orbit[i].x = i * 2 * Func_08002322(angle);
        orbit[i].y = -(i * 2 * Func_0800231c(angle));
        orbit[i].life = i / 2 + 25;
    }
    for (i = 0; i < 684; i++)
        particles[i].life = -1;

    center_x = half_height << 16;
    for (i = 0; i < 340; i++) {
        s32 magnitude = (Func_08004458() & 0x1ff) + 32;
        s32 angle = Func_08004458() & 0xffff;

        falling[i].x = center_x;
        falling[i].y = 0x580000;
        falling[i].velocity_x =
            magnitude * Func_08002322(angle) >> 5;
        falling[i].velocity_y =
            -(magnitude * Func_0800231c(angle)) >> 6;
        falling[i].life = (Func_08004458() & 7) + 32;
    }

    for (frame = 0; frame < 102; frame++) {
        if ((u32)(frame - 25) <= 22)
            Func_080e46f0(0xc0);
        if (frame > 56)
            Func_080e46f0(0xc4);
        if (frame == 8)
            *(s32 *)(runtime + 0x77a8) = 8;
        if (frame == 48)
            *(s32 *)(runtime + 0x77a8) = 8;
        if (frame == 60)
            *(s32 *)(runtime + 0x77a8) = 16;
        if (frame == 4)
            Func_080f9010(0xd4);
        if (frame == 32)
            Func_080f9010(0xa4);
        if (frame == 60) {
            Func_080f9010(0x91);
            Func_080b50e8(0x86);
        }

        if (frame > 55) {
            for (i = 0; i < 16; i++) {
                if ((u32)orbit[i].life <= 17) {
                    s32 image =
                        orbit_frames[
                            Func_080022ec(orbit[i].life, 3)];

                    renderers[0](
                        render_context,
                        runtime + 0xdd0 + image * 0x800,
                        (s16)(orbit[i].x >> 16)
                            + half_height - 16,
                        (s16)(orbit[i].y >> 16) + 48,
                        32, 64);
                }
                orbit[i].life =
                    orbit[i].life > 0
                        ? orbit[i].life - 1
                        : -1;
            }
        }

        if (frame == 28) {
            for (i = 0; i < 256; i++) {
                if (particles[i].life == -1)
                    InitializeBurstParticle_080e99c0(
                        &particles[i], center_x);
            }
        }

        if ((u32)(frame - 32) <= 31) {
            s32 created = 0;

            for (i = 0; i < 684 && created != 16; i++) {
                if (particles[i].life == -1) {
                    InitializeBurstParticle_080e99c0(
                        &particles[i], center_x);
                    created++;
                }
            }
        }

        if ((u32)(frame - 32) <= 31) {
            s32 split =
                Func_080022fc(frame * 16 - 0x8000, 104);
            s32 x = half_height - 17;

            renderers[0](
                render_context, runtime,
                x, 4 - split, 34, 104);
            renderers[0](
                render_context, runtime,
                x, 108 - split, 34, split);
        }

        if (frame <= 71) {
            for (i = 0; i < 340; i++) {
                struct Particle_080e99c0 *particle =
                    &falling[i];

                if (particle->life >= 0) {
                    s32 size = Func_080022fc(i, 3) + 2;
                    s32 diameter;
                    s32 x;
                    s32 y;

                    if (particle->velocity_y > 0)
                        size += 2;
                    if (frame > 68 && size <= 5)
                        size = 6;
                    if (frame > 70 && size <= 6)
                        size = 7;
                    if (frame > 72 && size <= 7)
                        size = 8;
                    if (frame > 74 && size <= 8)
                        size = 9;
                    if (frame > 76)
                        size = 10;
                    diameter = size * 2;
                    x = (s16)(particle->x >> 16)
                        - ((size + ((u32)size >> 31)) >> 1);
                    y = (s16)(particle->y >> 16) - size;
                    renderers[0](
                        render_context,
                        effect_tiles
                            + ((const u16 *)0x080ede48)
                                  [size - 1],
                        x, y, size, diameter);

                    particle->x += particle->velocity_x;
                    particle->y += particle->velocity_y;
                    if (frame > 80)
                        particle->velocity_y += -450;
                    else
                        particle->velocity_y +=
                            accelerations[i & 3];
                    particle->velocity_x =
                        (particle->velocity_x * 62
                             + (particle->velocity_x < 0 ? 63 : 0)) >>
                        6;
                    particle->velocity_y =
                        (particle->velocity_y * 62
                             + (particle->velocity_y < 0 ? 63 : 0)) >>
                        6;
                    particle->life++;
                    if (particle->velocity_y > 0 &&
                        (s16)(particle->y >> 16) > 108)
                        particle->life = -1;
                }
            }
        }

        if (frame <= 95) {
            s32 x = half_height - 18;
            s32 y;
            s32 height = 120;

            if (frame > 60)
                y = frame * 8 - 0x8000;
            else if (frame > 32)
                y = (frame - 32) / 2 + 16;
            else if (frame <= 9)
                y = frame * 16 - 128;
            else
                y = 16;
            if (y + 120 > 108)
                height = 108 - y;
            if (height > 0)
                renderers[1](
                    render_context, runtime + 0x4e20,
                    x, y, 36, height);
        }

        if (frame > 59) {
            for (i = 0; i < 340; i++) {
                struct Particle_080e99c0 *particle =
                    &falling[i];

                if (particle->life > 0) {
                    s32 particle_x;
                    s32 particle_y;
                    s32 size;

                    Func_080e3908(particle, 64, 0x2000);
                    particle->life--;
                    if (particle->y > 0x6c0000) {
                        s32 velocity = -particle->velocity_y;
                        particle->velocity_y =
                            (velocity + ((u32)velocity >> 31))
                            >> 1;
                    } else if ((u32)particle->x <= 0x7effff &&
                               particle->y >= 0) {
                        size =
                            Func_080022ec(particle->life, 5) + 1;
                        particle_x =
                            (s16)(particle->x >> 16)
                            - ((size + ((u32)size >> 31)) >> 1);
                        particle_y =
                            (s16)(particle->y >> 16) - size;
                        renderers[0](
                            render_context,
                            effect_tiles
                                + ((const u16 *)0x080ede48)
                                      [size - 1],
                            particle_x, particle_y,
                            size, size * 2);
                    }
                }
            }
        }

        if (frame == 68) {
            for (i = 0; i < scene->object_count; i++) {
                Func_080d6888(
                    scene->object_ids[i], 7, 5, i, 16);
                Func_080b5088(scene->object_ids[i], 7);
            }
        }
        if (frame == 9)
            fill(render_context, 0x4000, 0x3f3f3f3f);
        if (frame == 60)
            fill(render_context, 0x4000, 0x3f3f3f3f);

        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
