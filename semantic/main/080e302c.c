#include "types.h"

typedef void (*Transfer_080e302c)(void *, const void *, u32);
typedef void (*Renderer_080e302c)(
    void *, const void *, s32, s32, s32, s32);

struct Scene_080e302c {
    u8 unknown_00[4];
    s32 direction;
    u8 unknown_08[12];
    s32 object_count;
    s32 unknown_18;
    s32 mode;
    u8 unknown_1c[8];
    s16 object_ids[1];
};

struct Particle_080e302c {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 phase;
    s32 velocity_y;
    s32 unknown_14;
    s32 timer;
};

struct Visual_080e302c {
    u8 unknown_00[9];
    u8 flags09;
    u8 unknown_0a[28];
    u8 field26;
};

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002304(s32, s32);
void Func_08002dd8(s32);
void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_080041d8(const void *, u32);
void Func_08004278(const void *);
u32 Func_08004458(void);
void *Func_08009030(s32);
void Func_08009008(void *, s32 *, s32 *, s32);
void Func_08009020(void *, s32);
void Func_08009038(void *);
void Func_080b5088(s16, s32);
void Func_080b50e8(s32);
void Func_080cd52c(void);
void Func_080cd594(s32);
void Func_080cdbc0(void);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
void Func_080e155c(s32, s32);
void Func_080ed408(s32, s32, s32, s32, s32);
void Func_080f9010(s32);

void Func_080e302c(struct Scene_080e302c *scene)
{
    u32 *header = (u32 *)0x03001eec;
    u8 *runtime = (u8 *)header[0];
    void *render_context = (void *)header[1];
    u8 *graphics = (u8 *)header[2];
    Transfer_080e302c transfer = (Transfer_080e302c)0x03001388;
    Renderer_080e302c renderers[2];
    struct Particle_080e302c *orbiters =
        (struct Particle_080e302c *)(runtime + 0x7198);
    struct Particle_080e302c *fixed_particles =
        (struct Particle_080e302c *)(runtime + 0x7240);
    struct Particle_080e302c *falling =
        (struct Particle_080e302c *)0x02010000;
    struct Visual_080e302c **visuals =
        (struct Visual_080e302c **)(runtime + 0x77d8);
    const u8 *block_lengths = (const u8 *)0x080eed90;
    const u8 *block_counts = (const u8 *)0x080eeda0;
    const u8 *block_heights = (const u8 *)0x080eeda3;
    const u16 *main_offsets = (const u16 *)0x080eed9a;
    const u16 *strip_offsets = (const u16 *)0x080eed90;
    const u16 *particle_offsets = (const u16 *)0x080eed7e;
    s32 left_x;
    s32 right_x;
    s32 frame;
    s32 i;

    *(struct Scene_080e302c **)(runtime + 0x7828) = scene;
    Func_080cd594(0);

    if (scene->mode == 0)
        *(volatile u16 *)0x04000020 = 0x00cc;
    else if (scene->mode == 1)
        *(volatile u16 *)0x04000020 = 0x00aa;

    if (scene->direction == 1) {
        left_x = 8;
        if (scene->mode == 0)
            right_x = 40;
        else if (scene->mode == 1)
            right_x = 36;
        else
            right_x = 40;
    } else {
        left_x = -16;
        right_x = -12;
    }

    *(volatile s32 *)0x04000028 =
        ((const s8 *)0x080eeda6)
            [scene->direction * 3 + scene->mode] << 8;
    Func_080e0524(0x7b, runtime, 1, 0);
    transfer(
        (void *)0x05000000, Func_08002f40(0x7c), 0x80);

    for (i = 0; i < 63; i++) {
        volatile u16 *palette = (volatile u16 *)0x05000002;
        s32 color = palette[i];
        s32 red = (color & 31) - 8;
        s32 green = ((color >> 5) & 31) - 8;
        s32 blue = ((color >> 10) & 31) - 8;

        if (red < 0) red = 0;
        if (green < 0) green = 0;
        if (blue < 0) blue = 0;
        palette[i] = (u16)(red | (green << 5) | (blue << 10));
    }

    {
        const u8 *source = (const u8 *)0x02010000;
        u8 *destination = runtime;
        s32 group;

        for (group = 0; group < 9; group++) {
            s32 repeat;

            for (repeat = 0; repeat < 32; repeat++) {
                s32 byte;

                for (byte = 0; byte < block_lengths[group]; byte++)
                    *destination++ = source[byte];
            }
            source += block_lengths[group];
        }

        for (group = 0; group < 32; group++) {
            s32 repeat;

            for (repeat = 0; repeat < 3; repeat++) {
                s32 byte;

                for (byte = 0; byte < 48; byte++)
                    *destination++ = source[byte];
            }
            source += 48;
        }

        for (i = 0; i < 1008; i++)
            *destination++ = *source++;

        for (group = 0; group < 3; group++) {
            s32 bytes =
                block_counts[group] * block_heights[group];

            for (i = 0; i < bytes; i++)
                *destination++ = *source++;
        }
    }

    Func_080e0524(0x73, graphics, 0, 0);

    for (i = 0; i < 11; i++) {
        visuals[i] = Func_08009030(390);
        if (visuals[i] != 0) {
            visuals[i]->field26 = 0;
            Func_08009020(visuals[i], i / 4);
            visuals[i]->flags09 =
                (visuals[i]->flags09 & (u8)-13) | 4;
        }
    }

    Func_080ed408(46, 7, 7, 3, 2);
    renderers[0] = *(Renderer_080e302c *)0x03001f08;
    Func_080ed408(47, 7, 7, 7, 2);
    renderers[1] = *(Renderer_080e302c *)0x03001f0c;
    *(volatile u16 *)0x04000050 = 0x3f46;
    *(volatile u16 *)0x04000052 = 0x1010;
    *(s32 *)(runtime + 0x7780) = 2;
    *(s32 *)(runtime + 0x7784) = 75;
    Func_080041d8((const void *)0x080cd261, 0x480);

    for (i = 0; i < 11; i++) {
        orbiters[i].x = (Func_08004458() & 15) + 88;
        orbiters[i].y = 128;
        orbiters[i].velocity_x =
            (Func_08004458() & 15) + 2;
        orbiters[i].phase = i;
        orbiters[i].velocity_y = 1;
        orbiters[i].unknown_14 = 0x10000;
        orbiters[i].timer = 44 - i * 4;
    }

    for (i = 0; i < 6; i++)
        fixed_particles[i].x = ((const u8 *)0x080eedac)[i];

    for (i = 0; i < 256; i++) {
        falling[i].x =
            ((Func_08004458() & 63) + left_x + 32) << 16;
        falling[i].y =
            ((Func_08004458() & 7) + 96) << 16;
        falling[i].velocity_y =
            ((Func_08004458() & 63) + 32) << 13;
        falling[i].timer = (Func_08004458() & 31) + 24;
    }

    *(volatile u16 *)0x0400000c = 0x0785;
    *(s32 *)(runtime + 0x77a8) = 250;

    for (frame = 0; frame < 192; frame++) {
        if (frame == 0)
            Func_080f9010(212);
        if (frame == 40)
            Func_080f9010(141);
        if (frame == 96)
            Func_080f9010(145);
        if (frame == 120)
            Func_080b50e8(134);

        if (frame <= 81) {
            s32 stage = frame / 4;
            s32 width;
            s32 height;

            if (stage > 2)
                stage = (stage & 1) + 1;
            width = block_counts[stage];
            height = block_heights[stage];
            renderers[0](
                render_context,
                runtime + main_offsets[stage],
                left_x + 64 - width,
                116 - height,
                width,
                height);
            renderers[1](
                render_context,
                runtime + main_offsets[stage],
                right_x + 64,
                116 - height,
                width,
                height);
        }

        if ((u32)(frame - 12) <= 75) {
            s32 stage = Func_080022ec(frame - 64, 3);
            s32 width;
            s32 y;

            if (stage < 0) stage = 0;
            if (stage > 7) stage = 7;
            width = block_counts[stage];
            for (i = 0, y = -12; i < 4; i++, y += 32) {
                renderers[0](
                    render_context,
                    runtime + strip_offsets[stage],
                    left_x + 64 - width,
                    y,
                    width,
                    32);
                renderers[1](
                    render_context,
                    runtime + strip_offsets[stage],
                    right_x + 64,
                    y,
                    width,
                    32);
            }
        }

        if ((u32)(frame - 160) <= 23) {
            s32 stage = 7 - Func_080022ec(frame - 160, 3);
            s32 width;
            s32 y;

            if (stage < 0) stage = 0;
            if (stage > 7) stage = 7;
            width = block_counts[stage];
            for (i = 0, y = -12; i < 4; i++, y += 32) {
                renderers[0](
                    render_context,
                    runtime + strip_offsets[stage],
                    left_x + 64 - width,
                    y,
                    width,
                    32);
                renderers[1](
                    render_context,
                    runtime + strip_offsets[stage],
                    right_x + 64,
                    y,
                    width,
                    32);
            }
        }

        if ((u32)(frame - 88) <= 71) {
            renderers[0](
                render_context, runtime + 0x13c0,
                left_x + 16, 0, 48, 96);
            renderers[1](
                render_context, runtime + 0x13c0,
                right_x + 64, 0, 48, 96);
            renderers[0](
                render_context, runtime + 0x25c0,
                left_x + 16, 96, 48, 21);
            renderers[1](
                render_context, runtime + 0x25c0,
                right_x + 64, 96, 48, 21);
        }

        if (frame > 87) {
            for (i = 0; i < 64; i++) {
                struct Particle_080e302c *particle = &falling[i];

                if (particle->timer == 0) {
                    s32 size = (i & 3) + 5;

                    renderers[0](
                        render_context,
                        graphics + particle_offsets[size - 1],
                        *(s16 *)((u8 *)&particle->x + 2) -
                            (size >> 1),
                        *(s16 *)((u8 *)&particle->y + 2) - size,
                        size,
                        size * 2);
                    particle->y -= particle->velocity_y;
                    if (particle->y < 0 && frame <= 159)
                        particle->y = 0x600000;
                } else {
                    particle->timer--;
                }
            }
        }

        if (frame > 4) {
            s32 count = frame <= 71 ? 6 : 11;

            for (i = 0; i < count; i++) {
                struct Particle_080e302c *orbiter = &orbiters[i];

                if (orbiter->timer == 0) {
                    s32 position[2];
                    s32 transform[4];
                    s32 coordinate;
                    s32 animation;

                    if (frame > 71) {
                        coordinate =
                            0x10000 + i * 0x1000 +
                            scene->mode * 0x4000;
                    } else {
                        coordinate = 0x10000;
                    }
                    position[0] = coordinate;
                    position[1] = coordinate;
                    transform[0] =
                        (orbiter->x + right_x * 2) << 16;
                    transform[1] =
                        0x2000000 - (orbiter->y << 16);
                    transform[2] = 0x2000000;
                    transform[3] = 0;
                    animation =
                        Func_080022fc(frame / 2 + i, 11);
                    if (animation != -1)
                        Func_08009008(
                            visuals[animation],
                            transform,
                            position,
                            0);

                    orbiter->y -= orbiter->velocity_x;
                    orbiter->phase += orbiter->velocity_y;
                    if (orbiter->phase > 12)
                        orbiter->phase -= 12;
                    if (orbiter->y < 0) {
                        if (frame > 159) {
                            orbiter->timer = -1;
                        } else {
                            if (frame > 87) {
                                orbiter->velocity_x =
                                    (Func_08004458() & 7) + 8;
                                if (scene->mode == 0)
                                    orbiter->x =
                                        Func_08002304(
                                            Func_08004458(), 96) + 42;
                                else if (scene->mode == 1)
                                    orbiter->x =
                                        Func_08002304(
                                            Func_08004458(), 112) + 34;
                                else
                                    orbiter->x =
                                        Func_08002304(
                                            Func_08004458(), 160) + 10;
                            }
                            orbiter->y = 128;
                            orbiter->timer = 8;
                        }
                    }
                } else {
                    orbiter->timer--;
                }
            }
        }

        if (frame <= 158 && scene->object_count != 0) {
            for (i = 0; i < scene->object_count; i++) {
                if (frame > 85 &&
                    Func_080022fc(frame, 12) == 0)
                    Func_080d6888(
                        scene->object_ids[i], 7, 5, i, 6);
                if ((frame & 3) == 0)
                    Func_080b5088(scene->object_ids[i], 5);
            }
        }

        if ((u32)(frame - 90) <= 70)
            Func_080e155c(8, 8);
        else
            Func_080e155c(2, 2);
        Func_080cd52c();
        *(s32 *)(runtime + 0x7824) = 1;
        Func_080030f8(1);
    }

    Func_08004278((const void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    for (i = 0; i < 11; i++)
        Func_08009038(visuals[i]);
    Func_080cdbc0();
}
